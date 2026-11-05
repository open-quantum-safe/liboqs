/**
 * @file hqc-kem.c
 * @brief Implementation of hqc-kem.h
 */

#include "hqc-kem.h"
#include <stdint.h>
#include <string.h>
#include "crypto_memset.h"
#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include "symmetric.h"
#include "vector.h"

#ifdef VERBOSE
#include <stdio.h>
#endif

/**
 * @brief Internal key generation: derives the KEM keypair from a master seed.
 *
 * This follows a KeyGen_internal flow, deriving the keypair from the single seed
 * `seed_kem`:
 *  - seed_pke          <- XOF(seed_kem)
 *  - (ek_pke, dk_pke)  <- HQC-PKE.KeyGen(seed_pke)
 *  - ek_kem            <- ek_pke
 *  - dk_kem            <- seed_kem
 *
 * Decapsulation performs the same derivation inline on `dk_kem`, and additionally
 * squeezes `sigma` for implicit rejection (which key generation does not need).
 *
 * @param[out] ek_kem   Pointer to the output buffer where the encapsulation key will be stored.
 * @param[out] dk_kem   Pointer to the output buffer where the decapsulation key will be stored.
 * @param[in]  seed_kem Pointer to the KEM master seed.
 *
 * @return 0 on success.
 */
static int8_t hqc_kem_keygen_internal(uint8_t *ek_kem, uint8_t *dk_kem, const uint8_t *seed_kem) {
    uint8_t seed_pke[SEED_BYTES] = {0};
    uint8_t ek_pke[PUBLIC_KEY_BYTES_PKE] = {0};
    uint8_t dk_pke[SECRET_KEY_BYTES_PKE] = {0};
    shake256_xof_ctx ctx_kem = {0};

    // Derive seed_pke from seed_kem
    xof_init_dom_sep(&ctx_kem, seed_kem, SEED_BYTES, PARAM_K);
    xof_get_bytes(&ctx_kem, seed_pke, SEED_BYTES);

    // Compute HQC-PKE keypair, then assemble the KEM keys
    hqc_pke_keygen(ek_pke, dk_pke, seed_pke);
    memcpy(ek_kem, ek_pke, PUBLIC_KEY_BYTES_PKE);
    memcpy(dk_kem, seed_kem, SEED_BYTES);

#ifdef VERBOSE
    printf("\n\nseed_kem: ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", seed_kem[i]);
    printf("\n\nseed_pke: ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", seed_pke[i]);
    printf("\n\nek_pke: ");
    for (size_t i = 0; i < PUBLIC_KEY_BYTES_PKE; ++i) printf("%02x", ek_pke[i]);
    printf("\n\ndk_pke: ");
    for (size_t i = 0; i < SECRET_KEY_BYTES_PKE; ++i) printf("%02x", dk_pke[i]);
#endif

    // Zeroize sensitive data
    memset_zero(seed_pke, sizeof seed_pke);
    memset_zero(dk_pke, sizeof dk_pke);
    xof_ctx_release(&ctx_kem);
    memset_zero(&ctx_kem, sizeof ctx_kem);

    return 0;
}

/**
 * @brief Generates a keypair for the KEM (Key Encapsulation Mechanism) scheme.
 *
 * This function generates a public/private keypair used for key encapsulation and decapsulation.
 * The encapsulation key (`ek`) is used to encapsulate a shared secret, while the decapsulation key (`dk`)
 * is used to recover it.
 *
 * @param[out] ek_kem Pointer to the output buffer where the encapsulation key will be stored.
 * @param[out] dk_kem Pointer to the output buffer where the decapsulation key will be stored.
 *
 * @return 0 on success, -1 if ::randombytes() fails.
 *
 * @note This function samples `seed_kem` via ::randombytes(); `seed_pke` is
 *       derived from it using the HQC XOF inside ::hqc_kem_keygen_internal().
 * @note The caller must provide a properly seeded ::randombytes() implementation.
 * @see randombytes
 */
int8_t hqc_kem_keygen(uint8_t *ek_kem, uint8_t *dk_kem) {
#ifdef VERBOSE
    printf("\n\n\n### KEYGEN ###");
#endif
    uint8_t seed_kem[SEED_BYTES] = {0};

    if (randombytes(seed_kem, SEED_BYTES) != 0) {
        memset_zero(seed_kem, sizeof seed_kem);
        return -1;
    }

    int8_t ret = hqc_kem_keygen_internal(ek_kem, dk_kem, seed_kem);

    // Zeroize sensitive data
    memset_zero(seed_kem, sizeof seed_kem);

    return ret;
}

/**
 * @brief Internal encapsulation using provided randomness.
 *
 * This follows an Encaps_internal flow with external randomness:
 *  - (K, theta) <- G(H(ek_kem) || m || salt)
 *  - c <- HQC-PKE.Encrypt(ek_kem, m, theta)
 *  - c_kem <- (c, salt)
 *
 * @param[out] K      Pointer to the output buffer where the shared secret will be stored.
 * @param[out] c_kem  Pointer to the output buffer where the KEM ciphertext will be stored.
 * @param[in]  ek_kem Pointer to the encapsulation key.
 * @param[in]  m      Pointer to PARAM_K bytes of randomness for encapsulation.
 * @param[in]  salt   Pointer to SALT_BYTES of randomness for encapsulation.
 *
 * @return 0 on success.
 */
static int8_t hqc_kem_encaps_internal(uint8_t *K, uint8_t *c_kem, const uint8_t *ek_kem, const uint8_t *m,
                                      const uint8_t *salt) {
#ifdef VERBOSE
    printf("\n\n\n\n### ENCAPS ###");
#endif

    uint8_t K_theta[SHARED_SECRET_BYTES_KEM + SEED_BYTES] = {0};
    uint8_t hash_ek_kem[SEED_BYTES] = {0};
    uint64_t c_pke_u[VEC_N_PKE_WORDS] = {0};
    uint64_t c_pke_v[VEC_N1N2_PKE_WORDS] = {0};

    // Compute shared key K and ciphertext c_kem
    hash_h(hash_ek_kem, ek_kem);
    hash_g(K_theta, hash_ek_kem, m, salt);
    hqc_pke_encrypt(c_pke_u, c_pke_v, ek_kem, (uint64_t *)m, K_theta + SHARED_SECRET_BYTES_KEM);

    hqc_c_kem_to_string(c_kem, c_pke_u, c_pke_v, salt);
    memcpy(K, K_theta, SHARED_SECRET_BYTES_KEM);

#ifdef VERBOSE
    printf("\n\nek_kem: ");
    for (size_t i = 0; i < PUBLIC_KEY_BYTES_KEM; ++i) printf("%02x", ek_kem[i]);
    printf("\n\nm: ");
    vect_print((uint64_t *)m, PARAM_K);
    printf("\n\nsalt: ");
    for (size_t i = 0; i < SALT_BYTES; ++i) printf("%02x", salt[i]);
    printf("\n\nH(ek_kem): ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", hash_ek_kem[i]);
    printf("\n\ntheta: ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", K_theta[SHARED_SECRET_BYTES_KEM + i]);
    printf("\n\nc_kem: ");
    for (size_t i = 0; i < CIPHERTEXT_BYTES_KEM; ++i) printf("%02x", c_kem[i]);
    printf("\n\nK: ");
    for (size_t i = 0; i < SHARED_SECRET_BYTES_KEM; ++i) printf("%02x", K[i]);
#endif

    // Zeroize sensitive data
    memset_zero(K_theta, sizeof K_theta);

    return 0;
}

/**
 * @brief Performs key encapsulation using the KEM scheme.
 *
 * This function uses the encapsulation key (`ek`) to generate a ciphertext (`c_kem`) and a shared secret (`K`).
 *
 * @param[out] K       Pointer to the output buffer where the shared secret will be stored.
 * @param[out] c_kem   Pointer to the output buffer where the KEM ciphertext will be stored.
 * @param[in]  ek_kem  Pointer to the encapsulation key.
 *
 * @return 0 on success, -1 if ::randombytes() fails.
 *
 * @note This function samples `m` and `salt` via ::randombytes().
 * @note The caller must provide a properly seeded ::randombytes() implementation.
 * @see randombytes
 */
int8_t hqc_kem_encaps(uint8_t *K, uint8_t *c_kem, const uint8_t *ek_kem) {
    uint8_t m[PARAM_K] = {0};
    uint8_t salt[SALT_BYTES] = {0};

    // Sample message m and salt
    if (randombytes(m, PARAM_K) != 0 || randombytes(salt, SALT_BYTES) != 0) {
        memset_zero(m, sizeof m);
        memset_zero(salt, sizeof salt);
        return -1;
    }

    int8_t ret = hqc_kem_encaps_internal(K, c_kem, ek_kem, m, salt);

    // Zeroize sensitive data
    memset_zero(m, sizeof m);

    return ret;
}

/**
 * @brief Internal decapsulation using provided inputs.
 *
 * This follows a Decaps_internal flow. `dk_kem` is the KEM master seed `seed_kem`:
 *  - (seed_pke, sigma) <- XOF(dk_kem)
 *  - (ek_pke, dk_pke)  <- HQC-PKE.KeyGen(seed_pke)
 *  - parse c
 *  - m' <- HQC-PKE.Decrypt(dk_pke, c)
 *  - (K', theta') <- G(H(ek_pke) || m' || salt)
 *  - K_bar <- J(H(ek_pke), sigma, c)
 *  - c' <- HQC-PKE.Encrypt(ek_pke, m', theta')
 *  - select K' or K_bar depending on ciphertext match
 *
 * @param[out] K_prime Pointer to the output buffer where the recovered shared secret will be stored.
 * @param[in]  dk_kem  Pointer to the decapsulation key, i.e. the KEM master seed `seed_kem`.
 * @param[in]  c_kem   Pointer to the input KEM ciphertext.
 *
 * @return 0 on success.
 */
static int8_t hqc_kem_decaps_internal(uint8_t *K_prime, const uint8_t *dk_kem, const uint8_t *c_kem) {
#ifdef VERBOSE
    printf("\n\n\n\n### DECAPS ###");
#endif

    uint8_t seed_pke[SEED_BYTES] = {0};
    shake256_xof_ctx ctx_kem = {0};
    uint8_t ek_pke[PUBLIC_KEY_BYTES_PKE] = {0};
    uint8_t dk_pke[SECRET_KEY_BYTES_PKE] = {0};
    uint8_t sigma[PARAM_K] = {0};
    uint8_t m_prime[PARAM_K] = {0};
    uint8_t hash_ek_pke[SEED_BYTES] = {0};
    uint8_t K_theta_prime[SHARED_SECRET_BYTES_KEM + SEED_BYTES] = {0};
    uint8_t K_bar[SHARED_SECRET_BYTES_KEM] = {0};
    uint64_t c_pke_u[VEC_N_PKE_WORDS] = {0};
    uint64_t c_pke_v[VEC_N1N2_PKE_WORDS] = {0};
    uint8_t salt[SALT_BYTES] = {0};
    uint64_t c_pke_prime_u[VEC_N_PKE_WORDS] = {0};
    uint64_t c_pke_prime_v[VEC_N1N2_PKE_WORDS] = {0};
    uint8_t salt_prime[SALT_BYTES] = {0};
    uint8_t result;

    // Re-derive seed_pke and sigma from dk_kem (the KEM master seed), then
    // regenerate the PKE keypair (ek_pke, dk_pke)
    xof_init_dom_sep(&ctx_kem, dk_kem, SEED_BYTES, PARAM_K);
    xof_get_bytes(&ctx_kem, seed_pke, SEED_BYTES);
    xof_get_bytes(&ctx_kem, sigma, PARAM_K);
    hqc_pke_keygen(ek_pke, dk_pke, seed_pke);

#ifdef VERBOSE
    printf("\n\nseed_kem: ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", dk_kem[i]);
    printf("\n\nseed_pke: ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", seed_pke[i]);
    printf("\n\nsigma: ");
    for (size_t i = 0; i < PARAM_K; ++i) printf("%02x", sigma[i]);
    printf("\n\nek_pke: ");
    for (size_t i = 0; i < PUBLIC_KEY_BYTES_PKE; ++i) printf("%02x", ek_pke[i]);
    printf("\n\ndk_pke: ");
    for (size_t i = 0; i < SECRET_KEY_BYTES_PKE; ++i) printf("%02x", dk_pke[i]);
#endif

    // Parse ciphertext c_kem
    hqc_c_kem_from_string(c_pke_u, c_pke_v, salt, c_kem);

    // Compute message m_prime
    result = hqc_pke_decrypt((uint64_t *)m_prime, dk_pke, c_pke_u, c_pke_v);

    // Compute shared key K_prime and ciphertext c_kem_prime
    hash_h(hash_ek_pke, ek_pke);
    hash_g(K_theta_prime, hash_ek_pke, m_prime, salt);

    hqc_pke_encrypt(c_pke_prime_u, c_pke_prime_v, ek_pke, (uint64_t *)m_prime, K_theta_prime + SHARED_SECRET_BYTES_KEM);
    memcpy(salt_prime, salt, SALT_BYTES);

    // Compute rejection key K_bar
    hash_j(K_bar, hash_ek_pke, sigma, c_pke_u, c_pke_v, salt);
    result |= vect_compare((uint8_t *)c_pke_u, (uint8_t *)c_pke_prime_u, VEC_N_SIZE_BYTES);
    result |= vect_compare((uint8_t *)c_pke_v, (uint8_t *)c_pke_prime_v, VEC_N1N2_SIZE_BYTES);
    result |= vect_compare(salt, salt_prime, SALT_BYTES);
    result -= 1;
    for (size_t i = 0; i < SHARED_SECRET_BYTES_KEM; ++i) {
        K_prime[i] = (K_theta_prime[i] & result) ^ (K_bar[i] & ~result);
    }

#ifdef VERBOSE
    printf("\n\nc_kem: ");
    for (size_t i = 0; i < CIPHERTEXT_BYTES_KEM; ++i) printf("%02x", c_kem[i]);
    printf("\n\nm_prime: ");
    vect_print((uint64_t *)m_prime, PARAM_K);
    printf("\n\nH(ek_pke): ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", hash_ek_pke[i]);
    printf("\n\nK_prime_candidate: ");
    for (size_t i = 0; i < SHARED_SECRET_BYTES_KEM; ++i) printf("%02x", K_theta_prime[i]);
    printf("\n\ntheta_prime: ");
    for (size_t i = 0; i < SEED_BYTES; ++i) printf("%02x", K_theta_prime[SHARED_SECRET_BYTES_KEM + i]);
    printf("\n\n\n# Checking Ciphertext - Begin #");
    printf("\n\nc_prime.u: ");
    vect_print(c_pke_prime_u, VEC_N_SIZE_BYTES);
    printf("\n\nc_prime.v: ");
    vect_print(c_pke_prime_v, VEC_N1N2_SIZE_BYTES);
    printf("\n\nsalt_prime: ");
    for (size_t i = 0; i < SALT_BYTES; ++i) printf("%02x", salt_prime[i]);
    printf("\n\n# Checking Ciphertext - End #\n");
    printf("\n\nK_bar: ");
    for (size_t i = 0; i < SHARED_SECRET_BYTES_KEM; ++i) printf("%02x", K_bar[i]);
    printf("\n\nK_prime: ");
    for (size_t i = 0; i < SHARED_SECRET_BYTES_KEM; ++i) printf("%02x", K_prime[i]);
#endif

    // Zeroize sensitive data
    memset_zero(seed_pke, sizeof seed_pke);
    xof_ctx_release(&ctx_kem);
    memset_zero(&ctx_kem, sizeof ctx_kem);
    memset_zero(dk_pke, sizeof dk_pke);
    memset_zero(sigma, sizeof sigma);
    memset_zero(m_prime, sizeof m_prime);
    memset_zero(K_theta_prime, sizeof K_theta_prime);
    memset_zero(K_bar, sizeof K_bar);

    return 0;
}

/**
 * @brief Performs key decapsulation using the KEM scheme.
 *
 * This function uses the decapsulation key (`dk`) to recover the shared secret (`K_prime`)
 * from the given KEM ciphertext (`c_kem`), which was generated during encapsulation.
 *
 * @param[out] K_prime   Pointer to the output buffer where the recovered shared secret will be stored.
 * @param[in]  dk_kem    Pointer to the decapsulation key.
 * @param[in]  c_kem     Pointer to the input KEM ciphertext.
 *
 * @return Returns 0 on success.
 */
int8_t hqc_kem_decaps(uint8_t *K_prime, const uint8_t *dk_kem, const uint8_t *c_kem) {
    return hqc_kem_decaps_internal(K_prime, dk_kem, c_kem);
}

/*
 * liboqs integration: NIST-style wrappers.
 * liboqs calls KEMs as (ct, ss, pk) / (ss, ct, sk) with an int return value,
 * whereas the HQC API is (K, c_kem, ek_kem) / (K', dk_kem, c_kem) returning int8_t.
 */
int crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    return hqc_kem_keygen(pk, sk) == 0 ? 0 : -1;
}

/* seed: SEED_BYTES bytes (seed_kem) */
int crypto_kem_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
    return hqc_kem_keygen_internal(pk, sk, seed) == 0 ? 0 : -1;
}

int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    return hqc_kem_encaps(ss, ct, pk) == 0 ? 0 : -1;
}

/* seed: PARAM_K bytes (m) || SALT_BYTES bytes (salt) */
int crypto_kem_enc_derand(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *seed) {
    uint64_t m[(PARAM_K + 7) / 8] = {0}; /* m is used as uint64_t* downstream: keep it aligned */
    uint8_t salt[SALT_BYTES] = {0};
    memcpy(m, seed, PARAM_K);
    memcpy(salt, seed + PARAM_K, SALT_BYTES);
    int8_t ret = hqc_kem_encaps_internal(ss, ct, pk, (const uint8_t *)m, salt);
    memset_zero(m, sizeof m);
    return ret == 0 ? 0 : -1;
}

int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    return hqc_kem_decaps(ss, sk, ct) == 0 ? 0 : -1;
}
