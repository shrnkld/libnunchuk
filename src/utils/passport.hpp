/*
 * This file is part of libnunchuk (https://github.com/nunchuk-io/libnunchuk).
 * Copyright (c) 2020 Enigmo.
 *
 * libnunchuk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * libnunchuk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libnunchuk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NUNCHUK_PASSPORT_H
#define NUNCHUK_PASSPORT_H

#include <nunchuk.h>
#include <utils/json.hpp>
#include <boost/algorithm/string.hpp>

namespace {

inline bool ParsePassportSignerConfig(
    nunchuk::Chain chain, const std::string& content,
    std::vector<nunchuk::SingleSigner>& signers) {
  using namespace nunchuk;
  using namespace boost::algorithm;
  using json = nlohmann::json;

  json data = json::parse(content);
  if (data["xfp"] == nullptr) return false;
  std::string xfp = to_lower_copy(data["xfp"].get<std::string>());
  std::string target_format = chain == Chain::MAIN ? "xpub" : "tpub";

  auto addSigner = [&](const json& j) {
    if (j == nullptr) return;
    std::string xpub = Utils::SanitizeBIP32Input(j["xpub"], target_format);
    signers.push_back({"Passport", xpub, {}, j["deriv"], xfp, 0});
  };
  addSigner(data["bip49"]);
  addSigner(data["bip44"]);
  addSigner(data["bip84"]);
  addSigner(data["bip48_2"]);
  addSigner(data["bip48_1"]);

  auto addMSigner = [&](const std::string& k) {
    std::string deriv = k + "_deriv";
    if (data[k] == nullptr || data[deriv] == nullptr) return;
    std::string xpub = Utils::SanitizeBIP32Input(data[k], target_format);
    signers.push_back({"Passport", xpub, {}, data[deriv], xfp, 0});
  };
  addMSigner("p2sh");
  addMSigner("p2wsh_p2sh");
  addMSigner("p2wsh");
  return true;
}

}  // namespace

#endif  // NUNCHUK_PASSPORT_H