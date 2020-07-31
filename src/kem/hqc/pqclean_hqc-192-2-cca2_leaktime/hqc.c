/**
 * @file hqc.c
 * @brief Implementation of hqc.h
 */

#include "gf2x.h"
#include "hqc.h"
#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include "tensor.h"
#include "vector.h"
#include <stdint.h>


/**
 * @brief Keygen of the HQC_PKE IND_CPA scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the <b>seed</b> used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the <b>seed</b> used to generate vectors <b>x</b> and  <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] pk String containing the public key
 * @param[out] sk String containing the secret key
 */
void PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_pke_keygen(uint8_t *pk, uint8_t *sk) {
    AES_XOF_struct sk_seedexpander;
    AES_XOF_struct pk_seedexpander;
    uint8_t sk_seed[SEED_BYTES] = {0};
    uint8_t pk_seed[SEED_BYTES] = {0};
    uint8_t x[VEC_N_SIZE_BYTES] = {0};
    uint32_t y[PARAM_OMEGA] = {0};
    uint8_t h[VEC_N_SIZE_BYTES] = {0};
    uint8_t s[VEC_N_SIZE_BYTES] = {0};

    // Create seed_expanders for public key and secret key
    randombytes(sk_seed, SEED_BYTES);
    seedexpander_init(&sk_seedexpander, sk_seed, sk_seed + 32, SEEDEXPANDER_MAX_LENGTH);

    randombytes(pk_seed, SEED_BYTES);
    seedexpander_init(&pk_seedexpander, pk_seed, pk_seed + 32, SEEDEXPANDER_MAX_LENGTH);

    // Compute secret key
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_set_random_fixed_weight(&sk_seedexpander, x, PARAM_OMEGA);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_set_random_fixed_weight_by_coordinates(&sk_seedexpander, y, PARAM_OMEGA);

    // Compute public key
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_set_random(&pk_seedexpander, h);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_mul(s, y, h, PARAM_OMEGA);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_add(s, x, s, VEC_N_SIZE_BYTES);

    // Parse keys to string
    PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_public_key_to_string(pk, pk_seed, s);
    PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_secret_key_to_string(sk, sk_seed, pk);
}



/**
 * @brief Encryption of the HQC_PKE IND_CPA scheme
 *
 * The cihertext is composed of vectors <b>u</b> and <b>v</b>.
 *
 * @param[out] u Vector u (first part of the ciphertext)
 * @param[out] v Vector v (second part of the ciphertext)
 * @param[in] m Vector representing the message to encrypt
 * @param[in] theta Seed used to derive randomness required for encryption
 * @param[in] pk String containing the public key
 */
void PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_pke_encrypt(uint8_t *u, uint8_t *v, const uint8_t *m, const uint8_t *theta, const uint8_t *pk) {
    AES_XOF_struct seedexpander;
    uint8_t h[VEC_N_SIZE_BYTES] = {0};
    uint8_t s[VEC_N_SIZE_BYTES] = {0};
    uint8_t r1[VEC_N_SIZE_BYTES] = {0};
    uint32_t r2[PARAM_OMEGA_R] = {0};
    uint8_t e[VEC_N_SIZE_BYTES] = {0};
    uint8_t tmp1[VEC_N_SIZE_BYTES] = {0};
    uint8_t tmp2[VEC_N_SIZE_BYTES] = {0};

    // Create seed_expander from theta
    seedexpander_init(&seedexpander, theta, theta + 32, SEEDEXPANDER_MAX_LENGTH);

    // Retrieve h and s from public key
    PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_public_key_from_string(h, s, pk);

    // Generate r1, r2 and e
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_set_random_fixed_weight(&seedexpander, r1, PARAM_OMEGA_R);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_set_random_fixed_weight_by_coordinates(&seedexpander, r2, PARAM_OMEGA_R);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_set_random_fixed_weight(&seedexpander, e, PARAM_OMEGA_E);

    // Compute u = r1 + r2.h
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_mul(u, r2, h, PARAM_OMEGA_R);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_add(u, r1, u, VEC_N_SIZE_BYTES);

    // Compute v = m.G by encoding the message
    PQCLEAN_HQC1922CCA2_LEAKTIME_tensor_code_encode(v, m);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);

    // Compute v = m.G + s.r2 + e
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_mul(tmp2, r2, s, PARAM_OMEGA_R);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_add(tmp2, e, tmp2, VEC_N_SIZE_BYTES);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_BYTES);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_resize(v, PARAM_N1N2, tmp2, PARAM_N);
}



/**
 * @brief Decryption of the HQC_PKE IND_CPA scheme
 *
 * @param[out] m Vector representing the decrypted message
 * @param[in] u Vector u (first part of the ciphertext)
 * @param[in] v Vector v (second part of the ciphertext)
 * @param[in] sk String containing the secret key
 */
void PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_pke_decrypt(uint8_t *m, const uint8_t *u, const uint8_t *v, const uint8_t *sk) {
    uint8_t x[VEC_N_SIZE_BYTES] = {0};
    uint32_t y[PARAM_OMEGA] = {0};
    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
    uint8_t tmp1[VEC_N_SIZE_BYTES] = {0};
    uint8_t tmp2[VEC_N_SIZE_BYTES] = {0};

    // Retrieve x, y, pk from secret key
    PQCLEAN_HQC1922CCA2_LEAKTIME_hqc_secret_key_from_string(x, y, pk, sk);

    // Compute v - u.y
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_mul(tmp2, y, u, PARAM_OMEGA);
    PQCLEAN_HQC1922CCA2_LEAKTIME_vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_BYTES);

    // Compute m by decoding v - u.y
    PQCLEAN_HQC1922CCA2_LEAKTIME_tensor_code_decode(m, tmp2);
}
