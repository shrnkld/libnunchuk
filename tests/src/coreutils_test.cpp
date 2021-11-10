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

#include <nunchuk.h>
#include <coreutils.h>

#include <doctest.h>

TEST_CASE("testing CoreUtils") {
  using namespace nunchuk;
  CoreUtils::getInstance().SetChain(Chain::REGTEST);
  std::string desc =
      R"(wsh(sortedmulti(2,[534a4a82/48'/1'/0'/2']tpubDFeha94AzbvqSzMLj6iihYeP1zwfW3KgNcmd7oXvKD9dApjWK4KT1RzzbSNUgmsgBs8sshky7pLTUZahkfPTNVck2fwS5wXyn1nTAy8jZCJ/1/*,[4bda0966/48'/1'/0'/2']tpubDFTwhyhyq2m2eQGCGQvzgZocFVsQAyjYCAMdGs9ahzTsvd49M3ekAiZvpzyjXF57FpC5zm8NVEPgnptFGSdzM6aZcWVrB6cqVC7fXhXzW6s/1/*))#yufe9c9d)";
  std::string address =
      "bcrt1qfpsnqux3x0sjc4peamlv9vxntgr29jdzjzwavt32dkg394cfdggq6tr0l8";
  std::string psbt_1 =
      R"(cHNidP8BAKYCAAAAAgcbUNztmFpEWJgYQmJyBRj7mOqBCo/+qJbLiRz06K7YAAAAAAD/////B+6F6SmjUM2gM9I7bez98phSUf7riYEAv5/BQUdbWkcAAAAAAP////8CgJaYAAAAAAAWABRpaV51XHyRsmCyHUvLlKMF5jHtmIBNc1MCAAAAIgAg5wSPQMR6YTcIQdK6l0tBy4dX/kFQhX0kf/3YX36kJWgAAAAAAAEBKwDyBSoBAAAAIgAgktQ0vpFxo8PkueNijCiW0AfpJGdHl+atyFZs1eiTZJYiAgL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qEgwRQIhAPijKXgqeKv7OYgwCxGGg8ieYYdQQNnkpCTdhocwl4Y8AiB7AIQdWEnuxVCEEDTIrqIYkQJ9xHNQ+fshXxipYaPYvwEBBUdSIQL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qCEDipBRl3HrSI68KR5wVM2k+7WB7XqXvDw0Of/D8jKzNmdSriIGAvnaCbSKLvQFgQZT09JHPE4zYZglru/anzJrsaNMrPyoHEvaCWYwAACAAQAAgAAAAIACAACAAQAAAAAAAAAiBgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2ZxxTSkqCMAAAgAEAAIAAAACAAgAAgAEAAAAAAAAAAAEBKwDyBSoBAAAAIgAgktQ0vpFxo8PkueNijCiW0AfpJGdHl+atyFZs1eiTZJYiAgL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qEcwRAIgWFydclK0t8P/nv6p9BssIlO9YY8EV7DyHurVPwS2nzwCIF+64oaljF0w9D25d4lElX8LDRzCMvGabhemhK7+dVvlAQEFR1IhAvnaCbSKLvQFgQZT09JHPE4zYZglru/anzJrsaNMrPyoIQOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2Z1KuIgYC+doJtIou9AWBBlPT0kc8TjNhmCWu79qfMmuxo0ys/KgcS9oJZjAAAIABAACAAAAAgAIAAIABAAAAAAAAACIGA4qQUZdx60iOvCkecFTNpPu1ge16l7w8NDn/w/IyszZnHFNKSoIwAACAAQAAgAAAAIACAACAAQAAAAAAAAAAAAEBR1IhAhaclNqMdyE/iSgfJxhm92aBQxKEwCtbPAliQqrv1GkhIQOPItmysPBQ4Ou3j9i2KdvE1r2NKfR3he0ekfXr3iE0FlKuIgICFpyU2ox3IT+JKB8nGGb3ZoFDEoTAK1s8CWJCqu/UaSEcS9oJZjAAAIABAACAAAAAgAIAAIAAAAAAAwAAACICA48i2bKw8FDg67eP2LYp28TWvY0p9HeF7R6R9eveITQWHFNKSoIwAACAAQAAgAAAAIACAACAAAAAAAMAAAAA)";
  std::string psbt_2 =
      R"(cHNidP8BAKYCAAAAAgcbUNztmFpEWJgYQmJyBRj7mOqBCo/+qJbLiRz06K7YAAAAAAD/////B+6F6SmjUM2gM9I7bez98phSUf7riYEAv5/BQUdbWkcAAAAAAP////8CgJaYAAAAAAAWABRpaV51XHyRsmCyHUvLlKMF5jHtmIBNc1MCAAAAIgAg5wSPQMR6YTcIQdK6l0tBy4dX/kFQhX0kf/3YX36kJWgAAAAAAAEBKwDyBSoBAAAAIgAgktQ0vpFxo8PkueNijCiW0AfpJGdHl+atyFZs1eiTZJYiAgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2Z0gwRQIhAIp2a5dEr/OZPIqvd1KiR3S/IYMvL9wwn5N0+jZiSBpdAiBurRg0BW/eEVwp/JM5Q4WZMKxZBIw+Ka2atO7swm7rnwEBBUdSIQL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qCEDipBRl3HrSI68KR5wVM2k+7WB7XqXvDw0Of/D8jKzNmdSriIGAvnaCbSKLvQFgQZT09JHPE4zYZglru/anzJrsaNMrPyoHEvaCWYwAACAAQAAgAAAAIACAACAAQAAAAAAAAAiBgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2ZxxTSkqCMAAAgAEAAIAAAACAAgAAgAEAAAAAAAAAAAEBKwDyBSoBAAAAIgAgktQ0vpFxo8PkueNijCiW0AfpJGdHl+atyFZs1eiTZJYiAgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2Z0gwRQIhAMQCsnmHyXQ4DAbySllvZdEfLNbtWW4VZGWvee3AJISjAiBACQTl0CeEYT7QAHETKO1XRq6ixXI3cm8Qb2A4p57CUAEBBUdSIQL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qCEDipBRl3HrSI68KR5wVM2k+7WB7XqXvDw0Of/D8jKzNmdSriIGAvnaCbSKLvQFgQZT09JHPE4zYZglru/anzJrsaNMrPyoHEvaCWYwAACAAQAAgAAAAIACAACAAQAAAAAAAAAiBgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2ZxxTSkqCMAAAgAEAAIAAAACAAgAAgAEAAAAAAAAAAAABAUdSIQIWnJTajHchP4koHycYZvdmgUMShMArWzwJYkKq79RpISEDjyLZsrDwUODrt4/YtinbxNa9jSn0d4XtHpH1694hNBZSriICAhaclNqMdyE/iSgfJxhm92aBQxKEwCtbPAliQqrv1GkhHEvaCWYwAACAAQAAgAAAAIACAACAAAAAAAMAAAAiAgOPItmysPBQ4Ou3j9i2KdvE1r2NKfR3he0ekfXr3iE0FhxTSkqCMAAAgAEAAIAAAACAAgAAgAAAAAADAAAAAA==)";
  std::string combined_psbt =
      R"(cHNidP8BAKYCAAAAAgcbUNztmFpEWJgYQmJyBRj7mOqBCo/+qJbLiRz06K7YAAAAAAD/////B+6F6SmjUM2gM9I7bez98phSUf7riYEAv5/BQUdbWkcAAAAAAP////8CgJaYAAAAAAAWABRpaV51XHyRsmCyHUvLlKMF5jHtmIBNc1MCAAAAIgAg5wSPQMR6YTcIQdK6l0tBy4dX/kFQhX0kf/3YX36kJWgAAAAAAAEBKwDyBSoBAAAAIgAgktQ0vpFxo8PkueNijCiW0AfpJGdHl+atyFZs1eiTZJYiAgL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qEgwRQIhAPijKXgqeKv7OYgwCxGGg8ieYYdQQNnkpCTdhocwl4Y8AiB7AIQdWEnuxVCEEDTIrqIYkQJ9xHNQ+fshXxipYaPYvwEiAgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2Z0gwRQIhAIp2a5dEr/OZPIqvd1KiR3S/IYMvL9wwn5N0+jZiSBpdAiBurRg0BW/eEVwp/JM5Q4WZMKxZBIw+Ka2atO7swm7rnwEBBUdSIQL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qCEDipBRl3HrSI68KR5wVM2k+7WB7XqXvDw0Of/D8jKzNmdSriIGAvnaCbSKLvQFgQZT09JHPE4zYZglru/anzJrsaNMrPyoHEvaCWYwAACAAQAAgAAAAIACAACAAQAAAAAAAAAiBgOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2ZxxTSkqCMAAAgAEAAIAAAACAAgAAgAEAAAAAAAAAAAEBKwDyBSoBAAAAIgAgktQ0vpFxo8PkueNijCiW0AfpJGdHl+atyFZs1eiTZJYiAgL52gm0ii70BYEGU9PSRzxOM2GYJa7v2p8ya7GjTKz8qEcwRAIgWFydclK0t8P/nv6p9BssIlO9YY8EV7DyHurVPwS2nzwCIF+64oaljF0w9D25d4lElX8LDRzCMvGabhemhK7+dVvlASICA4qQUZdx60iOvCkecFTNpPu1ge16l7w8NDn/w/IyszZnSDBFAiEAxAKyeYfJdDgMBvJKWW9l0R8s1u1ZbhVkZa957cAkhKMCIEAJBOXQJ4RhPtAAcRMo7VdGrqLFcjdybxBvYDinnsJQAQEFR1IhAvnaCbSKLvQFgQZT09JHPE4zYZglru/anzJrsaNMrPyoIQOKkFGXcetIjrwpHnBUzaT7tYHtepe8PDQ5/8PyMrM2Z1KuIgYC+doJtIou9AWBBlPT0kc8TjNhmCWu79qfMmuxo0ys/KgcS9oJZjAAAIABAACAAAAAgAIAAIABAAAAAAAAACIGA4qQUZdx60iOvCkecFTNpPu1ge16l7w8NDn/w/IyszZnHFNKSoIwAACAAQAAgAAAAIACAACAAQAAAAAAAAAAAAEBR1IhAhaclNqMdyE/iSgfJxhm92aBQxKEwCtbPAliQqrv1GkhIQOPItmysPBQ4Ou3j9i2KdvE1r2NKfR3he0ekfXr3iE0FlKuIgICFpyU2ox3IT+JKB8nGGb3ZoFDEoTAK1s8CWJCqu/UaSEcS9oJZjAAAIABAACAAAAAgAIAAIAAAAAAAwAAACICA48i2bKw8FDg67eP2LYp28TWvY0p9HeF7R6R9eveITQWHFNKSoIwAACAAQAAgAAAAIACAACAAAAAAAMAAAAA)";
  std::string decoded_psbt =
      R"({"fee":0.0,"inputs":[{"bip32_derivs":[{"master_fingerprint":"4bda0966","path":"m/48'/1'/0'/2'/1/0","pubkey":"02f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca8"},{"master_fingerprint":"534a4a82","path":"m/48'/1'/0'/2'/1/0","pubkey":"038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b33667"}],"partial_signatures":{"02f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca8":"3045022100f8a329782a78abfb3988300b118683c89e61875040d9e4a424dd86873097863c02207b00841d5849eec550841034c8aea21891027dc47350f9fb215f18a961a3d8bf01","038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b33667":"30450221008a766b9744aff3993c8aaf7752a24774bf21832f2fdc309f9374fa3662481a5d02206ead1834056fde115c29fc933943859930ac59048c3e29ad9ab4eeecc26eeb9f01"},"witness_script":{"asm":"2 02f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca8 038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b33667 2 OP_CHECKMULTISIG","hex":"522102f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca821038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b3366752ae","type":"multisig"},"witness_utxo":{"amount":50.0,"scriptPubKey":{"address":"bcrt1qjt2rf053wx3u8e9eud3gc2yk6qr7jfr8g7t7dtwg2ekdt6ynvjtqvlt5wf","asm":"0 92d434be9171a3c3e4b9e3628c2896d007e924674797e6adc8566cd5e8936496","hex":"002092d434be9171a3c3e4b9e3628c2896d007e924674797e6adc8566cd5e8936496","type":"witness_v0_scripthash"}}},{"bip32_derivs":[{"master_fingerprint":"4bda0966","path":"m/48'/1'/0'/2'/1/0","pubkey":"02f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca8"},{"master_fingerprint":"534a4a82","path":"m/48'/1'/0'/2'/1/0","pubkey":"038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b33667"}],"partial_signatures":{"02f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca8":"30440220585c9d7252b4b7c3ff9efea9f41b2c2253bd618f0457b0f21eead53f04b69f3c02205fbae286a58c5d30f43db9778944957f0b0d1cc232f19a6e17a684aefe755be501","038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b33667":"3045022100c402b27987c974380c06f24a596f65d11f2cd6ed596e156465af79edc02484a30220400904e5d02784613ed000711328ed5746aea2c57237726f106f6038a79ec25001"},"witness_script":{"asm":"2 02f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca8 038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b33667 2 OP_CHECKMULTISIG","hex":"522102f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca821038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b3366752ae","type":"multisig"},"witness_utxo":{"amount":50.0,"scriptPubKey":{"address":"bcrt1qjt2rf053wx3u8e9eud3gc2yk6qr7jfr8g7t7dtwg2ekdt6ynvjtqvlt5wf","asm":"0 92d434be9171a3c3e4b9e3628c2896d007e924674797e6adc8566cd5e8936496","hex":"002092d434be9171a3c3e4b9e3628c2896d007e924674797e6adc8566cd5e8936496","type":"witness_v0_scripthash"}}}],"outputs":[{},{"bip32_derivs":[{"master_fingerprint":"4bda0966","path":"m/48'/1'/0'/2'/0/3","pubkey":"02169c94da8c77213f89281f271866f76681431284c02b5b3c096242aaefd46921"},{"master_fingerprint":"534a4a82","path":"m/48'/1'/0'/2'/0/3","pubkey":"038f22d9b2b0f050e0ebb78fd8b629dbc4d6bd8d29f47785ed1e91f5ebde213416"}],"witness_script":{"asm":"2 02169c94da8c77213f89281f271866f76681431284c02b5b3c096242aaefd46921 038f22d9b2b0f050e0ebb78fd8b629dbc4d6bd8d29f47785ed1e91f5ebde213416 2 OP_CHECKMULTISIG","hex":"522102169c94da8c77213f89281f271866f76681431284c02b5b3c096242aaefd4692121038f22d9b2b0f050e0ebb78fd8b629dbc4d6bd8d29f47785ed1e91f5ebde21341652ae","type":"multisig"}}],"tx":{"hash":"fb2ce3926112251c6ef615ecc06e52666245e8d51fc02968eec963e731bd3f43","locktime":0,"size":166,"txid":"fb2ce3926112251c6ef615ecc06e52666245e8d51fc02968eec963e731bd3f43","version":2,"vin":[{"scriptSig":{"asm":"","hex":""},"sequence":4294967295,"txid":"d8aee8f41c89cb96a8fe8f0a81ea98fb1805726242189858445a98eddc501b07","vout":0},{"scriptSig":{"asm":"","hex":""},"sequence":4294967295,"txid":"475a5b4741c19fbf008189ebfe515298f2fdec6d3bd233a0cd50a329e985ee07","vout":0}],"vout":[{"n":0,"scriptPubKey":{"addresses":["bcrt1qd954ua2u0jgmyc9jr49uh99rqhnrrmvck4qdvm"],"asm":"0 69695e755c7c91b260b21d4bcb94a305e631ed98","hex":"001469695e755c7c91b260b21d4bcb94a305e631ed98","reqSigs":1,"type":"witness_v0_keyhash"},"value":0.1},{"n":1,"scriptPubKey":{"addresses":["bcrt1quuzg7sxy0fsnwzzp62afwj6pewr40ljp2zzh6frllhv97l4yy45qy67pdj"],"asm":"0 e7048f40c47a61370841d2ba974b41cb8757fe4150857d247ffdd85f7ea42568","hex":"0020e7048f40c47a61370841d2ba974b41cb8757fe4150857d247ffdd85f7ea42568","reqSigs":1,"type":"witness_v0_scripthash"},"value":99.9}],"vsize":166,"weight":664},"unknown":{}})";
  std::string raw_tx =
      R"(02000000000102071b50dced985a445898184262720518fb98ea810a8ffea896cb891cf4e8aed80000000000ffffffff07ee85e929a350cda033d23b6decfdf2985251feeb898100bf9fc141475b5a470000000000ffffffff02809698000000000016001469695e755c7c91b260b21d4bcb94a305e631ed98804d735302000000220020e7048f40c47a61370841d2ba974b41cb8757fe4150857d247ffdd85f7ea425680400483045022100f8a329782a78abfb3988300b118683c89e61875040d9e4a424dd86873097863c02207b00841d5849eec550841034c8aea21891027dc47350f9fb215f18a961a3d8bf014830450221008a766b9744aff3993c8aaf7752a24774bf21832f2fdc309f9374fa3662481a5d02206ead1834056fde115c29fc933943859930ac59048c3e29ad9ab4eeecc26eeb9f0147522102f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca821038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b3366752ae04004730440220585c9d7252b4b7c3ff9efea9f41b2c2253bd618f0457b0f21eead53f04b69f3c02205fbae286a58c5d30f43db9778944957f0b0d1cc232f19a6e17a684aefe755be501483045022100c402b27987c974380c06f24a596f65d11f2cd6ed596e156465af79edc02484a30220400904e5d02784613ed000711328ed5746aea2c57237726f106f6038a79ec2500147522102f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca821038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b3366752ae00000000)";
  std::string tx =
      R"({"hash":"eab9bb494ee366f21289591426bb2b13cc77a7360c3867a8bbec1b59270807d0","locktime":0,"size":607,"txid":"fb2ce3926112251c6ef615ecc06e52666245e8d51fc02968eec963e731bd3f43","version":2,"vin":[{"scriptSig":{"asm":"","hex":""},"sequence":4294967295,"txid":"d8aee8f41c89cb96a8fe8f0a81ea98fb1805726242189858445a98eddc501b07","txinwitness":["","3045022100f8a329782a78abfb3988300b118683c89e61875040d9e4a424dd86873097863c02207b00841d5849eec550841034c8aea21891027dc47350f9fb215f18a961a3d8bf01","30450221008a766b9744aff3993c8aaf7752a24774bf21832f2fdc309f9374fa3662481a5d02206ead1834056fde115c29fc933943859930ac59048c3e29ad9ab4eeecc26eeb9f01","522102f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca821038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b3366752ae"],"vout":0},{"scriptSig":{"asm":"","hex":""},"sequence":4294967295,"txid":"475a5b4741c19fbf008189ebfe515298f2fdec6d3bd233a0cd50a329e985ee07","txinwitness":["","30440220585c9d7252b4b7c3ff9efea9f41b2c2253bd618f0457b0f21eead53f04b69f3c02205fbae286a58c5d30f43db9778944957f0b0d1cc232f19a6e17a684aefe755be501","3045022100c402b27987c974380c06f24a596f65d11f2cd6ed596e156465af79edc02484a30220400904e5d02784613ed000711328ed5746aea2c57237726f106f6038a79ec25001","522102f9da09b48a2ef405810653d3d2473c4e33619825aeefda9f326bb1a34cacfca821038a90519771eb488ebc291e7054cda4fbb581ed7a97bc3c3439ffc3f232b3366752ae"],"vout":0}],"vout":[{"n":0,"scriptPubKey":{"addresses":["bcrt1qd954ua2u0jgmyc9jr49uh99rqhnrrmvck4qdvm"],"asm":"0 69695e755c7c91b260b21d4bcb94a305e631ed98","hex":"001469695e755c7c91b260b21d4bcb94a305e631ed98","reqSigs":1,"type":"witness_v0_keyhash"},"value":0.1},{"n":1,"scriptPubKey":{"addresses":["bcrt1quuzg7sxy0fsnwzzp62afwj6pewr40ljp2zzh6frllhv97l4yy45qy67pdj"],"asm":"0 e7048f40c47a61370841d2ba974b41cb8757fe4150857d247ffdd85f7ea42568","hex":"0020e7048f40c47a61370841d2ba974b41cb8757fe4150857d247ffdd85f7ea42568","reqSigs":1,"type":"witness_v0_scripthash"},"value":99.9}],"vsize":277,"weight":1105})";
  CHECK(CoreUtils::getInstance().DeriveAddresses(desc, 1) == address);
  CHECK(CoreUtils::getInstance().CombinePsbt({psbt_1, psbt_2}) ==
        combined_psbt);
  CHECK(CoreUtils::getInstance().DecodePsbt(combined_psbt) == decoded_psbt);
  CHECK(CoreUtils::getInstance().FinalizePsbt(combined_psbt) == raw_tx);
  CHECK(CoreUtils::getInstance().DecodeRawTransaction(raw_tx) == tx);
}
