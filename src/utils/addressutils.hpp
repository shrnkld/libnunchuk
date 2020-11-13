// Copyright (c) 2020 Enigmo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NUNCHUK_ADDRESSUTILS_H
#define NUNCHUK_ADDRESSUTILS_H

#include <nunchuk.h>
#include <util/strencodings.h>
#include <crypto/sha256.h>
#include <script/standard.h>
#include <key_io.h>
#include <core_io.h>

#include <string>
#include <vector>

namespace {

std::string AddressToScriptPubKey(const std::string& address) {
  using namespace nunchuk;
  CTxDestination dest = DecodeDestination(address);
  if (!IsValidDestination(dest)) {
    throw NunchukException(NunchukException::INVALID_ADDRESS,
                            "invalid address");
  }
  CScript scriptPubKey = GetScriptForDestination(dest);
  return HexStr(scriptPubKey);
}

std::string ScriptPubKeyToAddress(const CScript& script) {
  std::vector<std::vector<unsigned char>> solns;
  TxoutType type = Solver(script, solns);
  CTxDestination address;
  if (ExtractDestination(script, address) && type != TxoutType::PUBKEY) {
    return EncodeDestination(address);
  }
  return "";
}

std::string ScriptPubKeyToAddress(const std::string& script_pub_key) {
  CScript script;
  auto spk = ParseHex(script_pub_key);
  script.insert(script.end(), spk.begin(), spk.end());
  return ScriptPubKeyToAddress(script);
}

std::string AddressToScriptHash(const std::string& address) {
  CSHA256 hasher;
  auto stream = ParseHex(AddressToScriptPubKey(address));
  hasher.Write((unsigned char*)&(*stream.begin()),
               stream.end() - stream.begin());
  uint256 scripthash;
  hasher.Finalize(scripthash.begin());
  return scripthash.GetHex();
}

}  // namespace

#endif  //  NUNCHUK_ADDRESSUTILS_H
