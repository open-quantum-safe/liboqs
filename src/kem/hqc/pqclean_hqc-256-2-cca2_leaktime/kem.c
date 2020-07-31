/**
 * @file kem.c
 * @brief Implementation of api.h
 */

#include "api.h"
#include "hqc.h"
#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "sha2.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>


/**
 * @brief Keygen of the HQC_KEM IND_CAA2 scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] pk String containing the public key
 * @param[out] sk String containing the secret key
 * @returns 0 if keygen is successful
 */
int PQCLEAN_HQC2562CCA2_LEAKTIME_crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    PQCLEAN_HQC2562CCA2_LEAKTIME_hqc_pke_keygen(pk, sk);
    return 0;
}



/**
 * @brief Encapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ct String containing the ciphertext
 * @param[out] ss String containing the shared secret
 * @param[in] pk String containing the public key
 * @returns 0 if encapsulation is successful
 */
int PQCLEAN_HQC2562CCA2_LEAKTIME_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    AES_XOF_struct G_seedexpander;
    uint8_t seed_G[VEC_K_SIZE_BYTES];
    uint8_t diversifier_bytes[8] = {0};
    uint8_t m[VEC_K_SIZE_BYTES] = {0};
    uint8_t theta[SEED_BYTES] = {0};
    uint8_t u[VEC_N_SIZE_BYTES] = {0};
    uint8_t v[VEC_N1N2_SIZE_BYTES] = {0};
    uint8_t d[SHA512_BYTES] = {0};
    uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};

    // Computing m
    PQCLEAN_HQC2562CCA2_LEAKTIME_vect_set_random_from_randombytes(m);

    // Generating G function
    memcpy(seed_G, m, VEC_K_SIZE_BYTES);
    seedexpander_init(&G_seedexpander, seed_G, diversifier_bytes, SEEDEXPANDER_MAX_LENGTH);

    // Computing theta
    seedexpander(&G_seedexpander, theta, SEED_BYTES);

    // Encrypting m
    PQCLEAN_HQC2562CCA2_LEAKTIME_hqc_pke_encrypt(u, v, m, theta, pk);

    // Computing d
    sha512(d, m, VEC_K_SIZE_BYTES);

    // Computing shared secret
    memcpy(mc, m, VEC_K_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES, u, VEC_N_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
    sha512(ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES);

    // Computing ciphertext
    PQCLEAN_HQC2562CCA2_LEAKTIME_hqc_ciphertext_to_string(ct, u, v, d);

    return 0;
}



/**
 * @brief Decapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ss String containing the shared secret
 * @param[in] ct String containing the cipĥertext
 * @param[in] sk String containing the secret key
 * @returns 0 if decapsulation is successful, -1 otherwise
 */
int PQCLEAN_HQC2562CCA2_LEAKTIME_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    AES_XOF_struct G_seedexpander;
    uint8_t seed_G[VEC_K_SIZE_BYTES] = {0};
    uint8_t diversifier_bytes[8] = {0};
    uint8_t u[VEC_N_SIZE_BYTES] = {0};
    uint8_t v[VEC_N1N2_SIZE_BYTES] = {0};
    uint8_t d[SHA512_BYTES] = {0};
    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
    uint8_t m[VEC_K_SIZE_BYTES] = {0};
    uint8_t theta[SEED_BYTES] = {0};
    uint8_t u2[VEC_N_SIZE_BYTES] = {0};
    uint8_t v2[VEC_N1N2_SIZE_BYTES] = {0};
    uint8_t d2[SHA512_BYTES] = {0};
    uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
    int8_t abort = 0;

    // Retrieving u, v and d from ciphertext
    PQCLEAN_HQC2562CCA2_LEAKTIME_hqc_ciphertext_from_string(u, v, d, ct);

    // Retrieving pk from sk
    memcpy(pk, sk + SEED_BYTES, PUBLIC_KEY_BYTES);

    // Decryting
    PQCLEAN_HQC2562CCA2_LEAKTIME_hqc_pke_decrypt(m, u, v, sk);

    // Generating G function
    memcpy(seed_G, m, VEC_K_SIZE_BYTES);
    seedexpander_init(&G_seedexpander, seed_G, diversifier_bytes, SEEDEXPANDER_MAX_LENGTH);

    // Computing theta
    seedexpander(&G_seedexpander, theta, SEED_BYTES);

    // Encrypting m'
    PQCLEAN_HQC2562CCA2_LEAKTIME_hqc_pke_encrypt(u2, v2, m, theta, pk);

    // Checking that c = c' and abort otherwise
    if (PQCLEAN_HQC2562CCA2_LEAKTIME_vect_compare(u, u2, VEC_N_SIZE_BYTES) != 0 ||
            PQCLEAN_HQC2562CCA2_LEAKTIME_vect_compare(v, v2, VEC_N1N2_SIZE_BYTES) != 0) {
        abort = 1;
    }

    // Computing d'
    sha512(d2, m, VEC_K_SIZE_BYTES);

    // Checking that d = d' and abort otherwise
    if (memcmp(d, d2, SHA512_BYTES) != 0) {
        abort = 1;
    }

    if (abort == 1) {
        memset(ss, 0, SHARED_SECRET_BYTES);
        return -1;
    }

    // Computing shared secret
    memcpy(mc, m, VEC_K_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES, u, VEC_N_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
    sha512(ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES);

    return 0;
}
