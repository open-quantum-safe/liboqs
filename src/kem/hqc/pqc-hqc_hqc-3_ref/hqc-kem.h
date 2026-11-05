/**
 * @file hqc-kem.h
 * @brief NIST KEM API used by the HQC-KEM IND-CCA2 scheme
 */

#ifndef HQC_KEM_H
#define HQC_KEM_H

#include <stdint.h>
#include "namespace.h"

#define hqc_kem_keygen PQCHQC_NAMESPACE(hqc_kem_keygen)
#define hqc_kem_encaps PQCHQC_NAMESPACE(hqc_kem_encaps)
#define hqc_kem_decaps PQCHQC_NAMESPACE(hqc_kem_decaps)
#define crypto_kem_keypair PQCHQC_NAMESPACE(crypto_kem_keypair)
#define crypto_kem_enc PQCHQC_NAMESPACE(crypto_kem_enc)
#define crypto_kem_dec PQCHQC_NAMESPACE(crypto_kem_dec)

#define CRYPTO_ALGNAME "HQC-3"

#define CRYPTO_SECRETKEYBYTES  32U
#define CRYPTO_PUBLICKEYBYTES  4514U
#define CRYPTO_BYTES           32U
#define CRYPTO_CIPHERTEXTBYTES 8978U

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

/* liboqs: NIST-style (int-returning, ct/ss/pk-ordered) wrappers */
int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif  // HQC_KEM_H
