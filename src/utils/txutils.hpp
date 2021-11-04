// Copyright (c) 2020 Enigmo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NUNCHUK_TXUTILS_H
#define NUNCHUK_TXUTILS_H

#include <nunchuk.h>
#include <tinyformat.h>
#include <util/strencodings.h>
#include <utils/addressutils.hpp>
#include <string>
#include <vector>
#include <psbt.h>
#include <core_io.h>

#include <signingprovider.h>
#include <script/sign.h>

namespace {

inline PartiallySignedTransaction DecodePsbt(const std::string& base64_psbt) {
  using namespace nunchuk;
  PartiallySignedTransaction psbtx;
  std::string error;
  if (!DecodeBase64PSBT(psbtx, base64_psbt, error)) {
    throw NunchukException(NunchukException::INVALID_PSBT, error.c_str());
  }
  return psbtx;
}

inline std::string EncodePsbt(const PartiallySignedTransaction& psbtx) {
  CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
  ssTx << psbtx;
  return EncodeBase64(MakeUCharSpan(ssTx));
}

inline std::string GetTxIdFromPsbt(const std::string& base64_psbt) {
  return DecodePsbt(base64_psbt).tx.get().GetHash().GetHex();
}

inline CMutableTransaction DecodeRawTransaction(const std::string& hex_tx) {
  using namespace nunchuk;
  CMutableTransaction mtx;
  if (!DecodeHexTx(mtx, hex_tx, true, true)) {
    throw NunchukException(NunchukException::INVALID_RAW_TX,
                           "TX decode failed");
  }
  return mtx;
}

inline nunchuk::Transaction GetTransactionFromCMutableTransaction(
    const CMutableTransaction& mtx,
    const std::vector<nunchuk::SingleSigner>& signers, int height) {
  using namespace nunchuk;

  Transaction tx{};
  tx.set_txid(mtx.GetHash().GetHex());
  tx.set_height(height);
  for (auto& input : mtx.vin) {
    tx.add_input({input.prevout.hash.GetHex(), input.prevout.n});
  }
  for (auto& output : mtx.vout) {
    std::string address = ScriptPubKeyToAddress(output.scriptPubKey);
    tx.add_output({address, output.nValue});
  }
  for (auto&& signer : signers) {
    tx.set_signer(signer.get_master_fingerprint(), height != -1);
  }
  if (height == 0) {
    tx.set_status(TransactionStatus::PENDING_CONFIRMATION);
  } else if (height == -2) {
    tx.set_status(TransactionStatus::NETWORK_REJECTED);
  } else if (height > 0) {
    tx.set_status(TransactionStatus::CONFIRMED);
  }
  return tx;
}

inline nunchuk::Transaction GetTransactionFromPartiallySignedTransaction(
    const PartiallySignedTransaction& psbtx,
    const std::vector<nunchuk::SingleSigner>& signers, int m) {
  using namespace nunchuk;
  Transaction tx =
      GetTransactionFromCMutableTransaction(psbtx.tx.get(), signers, -1);
  tx.set_m(m);

  // Parse partial sigs
  const PSBTInput& input = psbtx.inputs[0];

  if (!input.final_script_witness.IsNull() || !input.final_script_sig.empty()) {
    auto psbt = DecodePsbt(EncodePsbt(psbtx));
    for (auto&& signer : signers) {
      tx.set_signer(signer.get_master_fingerprint(), false);
    }

    auto txCredit = psbt.tx.get();
    auto input = psbt.inputs[0];
    auto txIn = input.non_witness_utxo.get();
    auto txSpend = CMutableTransaction(*txIn);
    txCredit.vin[0].scriptSig = input.final_script_sig;
    txCredit.vin[0].scriptWitness = input.final_script_witness;
    auto extract = DataFromTransaction(txCredit, 0,
                                       txSpend.vout[txCredit.vin[0].prevout.n]);
    for (auto&& sig : extract.signatures) {
      KeyOriginInfo info;
      if (SigningProviderCache::getInstance().GetKeyOrigin(sig.first, info)) {
        std::string master_fingerprint =
            strprintf("%08x", ReadBE32(info.fingerprint));
        tx.set_signer(master_fingerprint, true);
      }
    }

    if (FinalizePSBT(psbt)) {
      tx.set_status(TransactionStatus::READY_TO_BROADCAST);
    } else {
      tx.set_status(TransactionStatus::PENDING_SIGNATURES);
    }
    return tx;
  }

  std::vector<std::string> signed_pubkey;
  if (!input.partial_sigs.empty()) {
    for (const auto& sig : input.partial_sigs) {
      signed_pubkey.push_back(HexStr(sig.second.first));
    }
  }

  if (!input.hd_keypaths.empty()) {
    for (auto entry : input.hd_keypaths) {
      std::string pubkey = HexStr(entry.first);
      std::string master_fingerprint =
          strprintf("%08x", ReadBE32(entry.second.fingerprint));
      if (std::find(signed_pubkey.begin(), signed_pubkey.end(), pubkey) !=
          signed_pubkey.end()) {
        tx.set_signer(master_fingerprint, true);
      } else {
        tx.set_signer(master_fingerprint, false);
      }
    }
  }

  tx.set_status(signed_pubkey.size() == m
                    ? TransactionStatus::READY_TO_BROADCAST
                    : TransactionStatus::PENDING_SIGNATURES);
  return tx;
}

}  // namespace

#endif  //  NUNCHUK_TXUTILS_H
