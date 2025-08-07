// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>
#include <oqs/sig_ml_dsa.h>
#include <string.h>

// Forward declarations for reference implementations
#if defined(OQS_ENABLE_SIG_ml_dsa_44)
extern int pqcrystals_ml_dsa_44_ref_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[32]);
#endif
#if defined(OQS_ENABLE_SIG_ml_dsa_65)
extern int pqcrystals_ml_dsa_65_ref_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[32]);
#endif
#if defined(OQS_ENABLE_SIG_ml_dsa_87)
extern int pqcrystals_ml_dsa_87_ref_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[32]);
#endif

// Forward declarations for AVX2 implementations
#if defined(OQS_ENABLE_SIG_ml_dsa_44_avx2)
extern int pqcrystals_ml_dsa_44_avx2_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[32]);
#endif
#if defined(OQS_ENABLE_SIG_ml_dsa_65_avx2)
extern int pqcrystals_ml_dsa_65_avx2_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[32]);
#endif
#if defined(OQS_ENABLE_SIG_ml_dsa_87_avx2)
extern int pqcrystals_ml_dsa_87_avx2_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[32]);
#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_44)

OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed) {
    int ret;
#if defined(OQS_ENABLE_SIG_ml_dsa_44_avx2)
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
        ret = pqcrystals_ml_dsa_44_avx2_keypair_derand(public_key, secret_key, seed);
    } else
#endif
    {
        ret = pqcrystals_ml_dsa_44_ref_keypair_derand(public_key, secret_key, seed);
    }
    return (ret == 0) ? OQS_SUCCESS : OQS_ERROR;
}

#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_65)

OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed) {
    int ret;
#if defined(OQS_ENABLE_SIG_ml_dsa_65_avx2)
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
        ret = pqcrystals_ml_dsa_65_avx2_keypair_derand(public_key, secret_key, seed);
    } else
#endif
    {
        ret = pqcrystals_ml_dsa_65_ref_keypair_derand(public_key, secret_key, seed);
    }
    return (ret == 0) ? OQS_SUCCESS : OQS_ERROR;
}

#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_87)

OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed) {
    int ret;
#if defined(OQS_ENABLE_SIG_ml_dsa_87_avx2)
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
        ret = pqcrystals_ml_dsa_87_avx2_keypair_derand(public_key, secret_key, seed);
    } else
#endif
    {
        ret = pqcrystals_ml_dsa_87_ref_keypair_derand(public_key, secret_key, seed);
    }
    return (ret == 0) ? OQS_SUCCESS : OQS_ERROR;
}

#endif