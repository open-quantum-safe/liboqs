/**
 * @file hqc-kem.h
 * @brief A shimmed API header, replacing common/hqc-<1|3|5>/hqc-kem.h
 */

#ifndef HQC_KEM_H
#define HQC_KEM_H

#include <stdint.h>

#include "namespace.h"

#ifndef HQC_PARAMETER_SET
#define HQC_PARAMETER_SET 1
#endif /* !HQC_PARAMETER_SET */

#if (HQC_PARAMETER_SET != 1) && (HQC_PARAMETER_SET != 3) && (HQC_PARAMETER_SET != 5)
#error "Invalid HQC_PARAMETER_SET. Must be one of 1, 3, or 5"
#endif

#if (HQC_PARAMETER_SET == 1)
#define CRYPTO_ALGNAME "HQC-1"
#define CRYPTO_SECRETKEYBYTES  32U
#define CRYPTO_PUBLICKEYBYTES  2241U
#define CRYPTO_BYTES           32U
#define CRYPTO_CIPHERTEXTBYTES 4433U
#elif (HQC_PARAMETER_SET == 3)
#define CRYPTO_ALGNAME "HQC-3"
#define CRYPTO_SECRETKEYBYTES  32U
#define CRYPTO_PUBLICKEYBYTES  4514U
#define CRYPTO_BYTES           32U
#define CRYPTO_CIPHERTEXTBYTES 8978U
#else
#define CRYPTO_ALGNAME "HQC-5"
#define CRYPTO_SECRETKEYBYTES  32U
#define CRYPTO_PUBLICKEYBYTES  7237U
#define CRYPTO_BYTES           32U
#define CRYPTO_CIPHERTEXTBYTES 14421U
#endif /* HQC_PARAMETER_SET decides CRYPTO_ALGNAME and parameter sets */

#define hqc_kem_keygen HQC_NS(hqc_kem_keygen)
#define hqc_kem_encaps HQC_NS(hqc_kem_encaps)
#define hqc_kem_decaps HQC_NS(hqc_kem_decaps)

/**
 * @brief Generate a KEM keypair.
 *
 * @param[out] ek_kem Encapsulation key buffer (size CRYPTO_PUBLICKEYBYTES).
 * @param[out] dk_kem Decapsulation key buffer (size CRYPTO_SECRETKEYBYTES).
 *
 * @details The encapsulation key is the PKE public key.
 *          The decapsulation key is the KEM master seed `seed_kem`, from which
 *          `seed_pke` and `sigma` are re-derived via the HQC XOF whenever needed:
 *          - seed_pke: seed used to regenerate the PKE keypair (`ek_pke`, `dk_pke`)
 *          - sigma: implicit rejection value used during decapsulation
 *
 * @return 0 on success, -1 on randombytes failure.
 */
int8_t hqc_kem_keygen(uint8_t *ek_kem, uint8_t *dk_kem);

/**
 * @brief Encapsulate a shared secret using the public key.
 *
 * @param[out] K     Shared secret buffer (size CRYPTO_BYTES).
 * @param[out] c_kem Ciphertext buffer (size CRYPTO_CIPHERTEXTBYTES).
 * @param[in]  ek_kem Encapsulation key buffer (size CRYPTO_PUBLICKEYBYTES).
 *
 * @details The encapsulation key is the PKE public key.
 *
 * @return 0 on success, -1 on randombytes failure.
 */
int8_t hqc_kem_encaps(uint8_t *K, uint8_t *c_kem, const uint8_t *ek_kem);

/**
 * @brief Decapsulate a shared secret using the secret key.
 *
 * @param[out] K_prime Shared secret buffer (size CRYPTO_BYTES).
 * @param[in]  dk_kem  Decapsulation key buffer (size CRYPTO_SECRETKEYBYTES).
 * @param[in]  c_kem   Ciphertext buffer (size CRYPTO_CIPHERTEXTBYTES).
 *
 * @details The decapsulation key is the KEM master seed `seed_kem`, from which
 *          `seed_pke` and `sigma` are re-derived via the HQC XOF whenever needed:
 *          - seed_pke: seed used to regenerate the PKE keypair (`ek_pke`, `dk_pke`)
 *          - sigma: implicit rejection value used during decapsulation
 *
 * @return 0 on success.
 */
int8_t hqc_kem_decaps(uint8_t *K_prime, const uint8_t *dk_kem, const uint8_t *c_kem);

#endif  // HQC_KEM_H
