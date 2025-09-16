// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <oqs/rand.h>
#include "icicle/pqc/ml_kem.h"
#include "icicle/runtime.h"
#include "icicle/device.h"
#include "icicle/errors.h"

extern "C" {
#if defined(OQS_ENABLE_KEM_ml_kem_1024_icicle_cuda)
OQS_STATUS icicle_ml_kem_1024_keypair(uint8_t *pk, uint8_t *sk) {
    uint8_t coins[icicle::pqc::ml_kem::ENTROPY_BYTES];
    OQS_randombytes(coins, icicle::pqc::ml_kem::ENTROPY_BYTES);

    icicle::pqc::ml_kem::MlKemConfig config;
    return icicle::pqc::ml_kem::keygen<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)coins, config, (std::byte*)pk, (std::byte*)sk) == icicle::eIcicleError::SUCCESS ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS icicle_ml_kem_1024_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    uint8_t coins[icicle::pqc::ml_kem::MESSAGE_BYTES];
    OQS_randombytes(coins, icicle::pqc::ml_kem::MESSAGE_BYTES);

    icicle::pqc::ml_kem::MlKemConfig config;
    return icicle::pqc::ml_kem::encapsulate<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)coins, (std::byte*)pk, config, (std::byte*)ct, (std::byte*)ss) == icicle::eIcicleError::SUCCESS ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS icicle_ml_kem_1024_enc_derand(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *coins) {
    icicle::pqc::ml_kem::MlKemConfig config;
    return icicle::pqc::ml_kem::encapsulate<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)coins, (std::byte*)pk, config, (std::byte*)ct, (std::byte*)ss) == icicle::eIcicleError::SUCCESS ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS icicle_ml_kem_1024_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    icicle::pqc::ml_kem::MlKemConfig config;
    return icicle::pqc::ml_kem::decapsulate<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)sk, (std::byte*)ct, config, (std::byte*)ss) == icicle::eIcicleError::SUCCESS ? OQS_SUCCESS : OQS_ERROR;
}
#endif
}
