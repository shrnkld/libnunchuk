// Copyright (c) 2020 Enigmo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NUNCHUK_ATTACHMENT_H
#define NUNCHUK_ATTACHMENT_H

#include <nunchuk.h>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <iostream>
#include <regex>

#include <util/strencodings.h>
#include <random.h>
#include <crypto/sha256.h>

#include <utils/json.hpp>
#include <utils/loguru.hpp>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <utils/httplib.h>

#include <fstream>

extern "C" {
#include <aes/aes.h>
}

namespace {

static const std::string DEFAULT_MATRIX_SERVER = "https://matrix.nunchuk.io";

inline std::vector<unsigned char> DownloadAttachment(const std::string& url) {
  auto id = url.substr(5);
  std::string body;
  httplib::Client cli(DEFAULT_MATRIX_SERVER.c_str());
  auto res = cli.Get(("/_matrix/media/r0/download" + id).c_str(),
                     [&](const char* data, size_t data_length) {
                       body.append(data, data_length);
                       return true;
                     });
  if (!res || res->status != 200) {
    throw nunchuk::NunchukException(
        nunchuk::NunchukException::SERVER_REQUEST_ERROR, "download file error");
  }
  return std::vector<unsigned char>(body.begin(), body.end());
}

inline std::string UploadAttachment(const std::string& accessToken,
                                    const char* body, size_t length) {
  std::string auth = (std::string("Bearer ") + accessToken);
  httplib::Headers headers = {{"Authorization", auth}};
  httplib::Client cli(DEFAULT_MATRIX_SERVER.c_str());
  auto res = cli.Post("/_matrix/media/r0/upload", headers, body, length,
                      "application/octet-stream");
  if (!res || res->status != 200) {
    throw nunchuk::NunchukException(
        nunchuk::NunchukException::SERVER_REQUEST_ERROR, "upload file error");
  }
  return res->body;
}

inline std::vector<unsigned char> LoadAttachmentFile(const std::string& path) {
  std::ifstream infile(path, std::ios_base::binary);
  return std::vector<unsigned char>{std::istreambuf_iterator<char>(infile),
                                    std::istreambuf_iterator<char>()};
}

inline std::string DecryptAttachment(const std::string& event_file) {
  using json = nlohmann::json;
  json file = json::parse(event_file);
  auto buf = DownloadAttachment(file["url"]);
  auto key = DecodeBase64(file["key"]["k"].get<std::string>().c_str());
  auto iv = DecodeBase64(file["iv"].get<std::string>().c_str());

  unsigned char ciphertext[buf.size()];
  aes_encrypt_ctx cx[1];
  aes_init();
  aes_encrypt_key256(&key[0], cx);
  aes_ctr_crypt(&buf[0], ciphertext, buf.size(), &iv[0], aes_ctr_cbuf_inc, cx);
  return std::string((char*)ciphertext);
}

inline std::string EncryptAttachment(const nunchuk::UploadFileFunc& uploadfunc,
                                     const std::string& body) {
  using json = nlohmann::json;
  json file;
  file["v"] = "v2";

  std::vector<unsigned char> key(32, 0);
  GetStrongRandBytes(key.data(), 32);
  file["key"] = {{"alg", "A256CTR"},
                 {"ext", true},
                 {"k", EncodeBase64(key)},
                 {"key_ops", {"encrypt", "decrypt"}},
                 {"kty", "oct"}};

  std::vector<unsigned char> iv(16, 0);
  GetStrongRandBytes(iv.data(), 8);
  file["iv"] = EncodeBase64(iv);
  iv.resize(8);

  std::vector<unsigned char> buf(body.begin(), body.end());
  unsigned char ciphertext[buf.size()];
  aes_encrypt_ctx cx[1];
  aes_init();
  aes_encrypt_key256(&key[0], cx);
  aes_ctr_crypt(&buf[0], ciphertext, buf.size(), &iv[0], aes_ctr_cbuf_inc, cx);

  file["mimetype"] = "application/octet-stream";
  file["url"] =
      uploadfunc("Backup", file["mimetype"], (char*)ciphertext, buf.size());

  CSHA256 hasher;
  hasher.Write(ciphertext, buf.size());
  uint256 hash;
  hasher.Finalize(hash.begin());
  file["hashes"] = {{"sha256", EncodeBase64(hash)}};
  return file.dump();
}

inline std::string DecryptTxId(const std::string& descriptor,
                               const std::string& encrypted) {
  using json = nlohmann::json;
  json file = json::parse(encrypted);

  std::vector<unsigned char> key(32, 0);
  CSHA256 hasher;
  std::vector<unsigned char> stream(descriptor.begin(), descriptor.end());
  hasher.Write((unsigned char*)&(*stream.begin()),
               stream.end() - stream.begin());
  hasher.Finalize(key.data());

  auto iv = DecodeBase64(file["iv"].get<std::string>().c_str());
  auto buf = DecodeBase64(file["d"].get<std::string>().c_str());

  unsigned char ciphertext[buf.size()];
  aes_encrypt_ctx cx[1];
  aes_init();
  aes_encrypt_key256(&key[0], cx);
  aes_ctr_crypt(&buf[0], ciphertext, buf.size(), &iv[0], aes_ctr_cbuf_inc, cx);
  return std::string((char*)ciphertext);
}

inline std::string EncryptTxId(const std::string& descriptor,
                               const std::string& txId) {
  using json = nlohmann::json;
  json encrypted;
  encrypted["v"] = "v1";

  std::vector<unsigned char> key(32, 0);
  CSHA256 hasher;
  std::vector<unsigned char> stream(descriptor.begin(), descriptor.end());
  hasher.Write((unsigned char*)&(*stream.begin()),
               stream.end() - stream.begin());
  hasher.Finalize(key.data());

  std::vector<unsigned char> iv(16, 0);
  GetStrongRandBytes(iv.data(), 8);
  encrypted["iv"] = EncodeBase64(iv);
  iv.resize(8);

  std::vector<unsigned char> buf(txId.begin(), txId.end());
  unsigned char ciphertext[buf.size()];
  aes_encrypt_ctx cx[1];
  aes_init();
  aes_encrypt_key256(&key[0], cx);
  aes_ctr_crypt(&buf[0], ciphertext, buf.size(), &iv[0], aes_ctr_cbuf_inc, cx);

  std::vector<unsigned char> d(
      ciphertext, ciphertext + sizeof(ciphertext) / sizeof(ciphertext[0]));
  encrypted["d"] = EncodeBase64(d);
  return encrypted.dump();
}

}  // namespace

#endif  // NUNCHUK_ATTACHMENT_H