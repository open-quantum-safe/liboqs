#include <stdint.h>
#include <oqs/rand.h>
#include "icicle/runtime.h"
#include <icicle/pqc/ml_kem.h>

extern "C" {
int icicle_ml_kem_1024_keypair(uint8_t *pk, uint8_t *sk) {
    uint8_t coins[icicle::pqc::ml_kem::ENTROPY_BYTES];
    OQS_randombytes(coins, icicle::pqc::ml_kem::ENTROPY_BYTES);
#if defined(OQS_ENABLE_KEM_ml_kem_1024_icicle_cuda)
    icicle::Device dev = {"CUDA-PQC", 0};
#endif
    icicle_set_device(dev);

    icicle::pqc::ml_kem::MlKemConfig config;

    return (int)icicle::pqc::ml_kem::keygen<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)coins, config, (std::byte*)pk, (std::byte*)sk);
}

int icicle_ml_kem_1024_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    uint8_t coins[icicle::pqc::ml_kem::MESSAGE_BYTES];
    OQS_randombytes(coins, icicle::pqc::ml_kem::MESSAGE_BYTES);
#if defined(OQS_ENABLE_KEM_ml_kem_1024_icicle_cuda)
    icicle::Device dev = {"CUDA-PQC", 0};
#endif
    icicle_set_device(dev);

    icicle::pqc::ml_kem::MlKemConfig config;
    return (int)icicle::pqc::ml_kem::encapsulate<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)coins, (std::byte*)pk, config, (std::byte*)ct, (std::byte*)ss);
}

int icicle_ml_kem_1024_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
#if defined(OQS_ENABLE_KEM_ml_kem_1024_icicle_cuda)
    icicle::Device dev = {"CUDA-PQC", 0};
#endif
    icicle_set_device(dev);

    icicle::pqc::ml_kem::MlKemConfig config;
    return (int)icicle::pqc::ml_kem::decapsulate<icicle::pqc::ml_kem::Kyber1024Params>((std::byte*)sk, (std::byte*)ct, config, (std::byte*)ss);
}
}
