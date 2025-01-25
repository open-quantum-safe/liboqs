/**
 * @file parsing.c
 * @brief Functions to parse secret key, public key and ciphertext of the HQC scheme
 */

#include "shake_prng.h"
#include "parameters.h"
#include "parsing.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>


/**
 * @brief Parse a secret key into a string
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] sk String containing the secret key
 * @param[in] sk_seed Seed used to generate the secret key
 * @param[in] sigma String used in HHK transform
 * @param[in] pk String containing the public key
 */
void HQC256_hqc_secret_key_to_string(uint8_t *sk, const uint8_t *sk_seed, const uint8_t *sigma, const uint8_t *pk) {
    memcpy(sk, sk_seed, HQC256_SEED_BYTES);
    memcpy(sk + HQC256_SEED_BYTES, sigma, HQC256_VEC_K_SIZE_BYTES);
    memcpy(sk + HQC256_SEED_BYTES + HQC256_VEC_K_SIZE_BYTES, pk, HQC256_PUBLIC_KEY_BYTES);
}



/**
 * @brief Parse a secret key from a string
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] y uint64_t representation of vector y
 * @param[in] sigma String used in HHK transform
 * @param[out] pk String containing the public key
 * @param[in] sk String containing the secret key
 */
void HQC256_hqc_secret_key_from_string(__m256i *y256, uint8_t *sigma, uint8_t *pk, const uint8_t *sk) {
    seedexpander_state sk_seedexpander;
    uint8_t sk_seed[HQC256_SEED_BYTES] = {0};

    memcpy(sk_seed, sk, HQC256_SEED_BYTES);
    memcpy(sigma, sk + HQC256_SEED_BYTES, HQC256_VEC_K_SIZE_BYTES);
    HQC256_seedexpander_init(&sk_seedexpander, sk_seed, HQC256_SEED_BYTES);

    HQC256_vect_set_random_fixed_weight(&sk_seedexpander, y256, HQC256_PARAM_OMEGA);
    memcpy(pk, sk + HQC256_SEED_BYTES + HQC256_VEC_K_SIZE_BYTES, HQC256_PUBLIC_KEY_BYTES);
}



/**
 * @brief Parse a public key into a string
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>
 *
 * @param[out] pk String containing the public key
 * @param[in] pk_seed Seed used to generate the public key
 * @param[in] s uint8_t representation of vector s
 */
void HQC256_hqc_public_key_to_string(uint8_t *pk, const uint8_t *pk_seed, const uint64_t *s) {
    memcpy(pk, pk_seed, HQC256_SEED_BYTES);
    memcpy(pk + HQC256_SEED_BYTES, s, HQC256_VEC_N_SIZE_BYTES);
}



/**
 * @brief Parse a public key from a string
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>
 *
 * @param[out] h uint8_t representation of vector h
 * @param[out] s uint8_t representation of vector s
 * @param[in] pk String containing the public key
 */
void HQC256_hqc_public_key_from_string(uint64_t *h, uint64_t *s, const uint8_t *pk) {
    seedexpander_state pk_seedexpander;
    uint8_t pk_seed[HQC256_SEED_BYTES] = {0};

    memcpy(pk_seed, pk, HQC256_SEED_BYTES);
    HQC256_seedexpander_init(&pk_seedexpander, pk_seed, HQC256_SEED_BYTES);
    HQC256_vect_set_random(&pk_seedexpander, h);

    memcpy(s, pk + HQC256_SEED_BYTES, HQC256_VEC_N_SIZE_BYTES);
}



/**
 * @brief Parse a ciphertext into a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and hash <b>d</b>.
 *
 * @param[out] ct String containing the ciphertext
 * @param[in] u uint8_t representation of vector u
 * @param[in] v uint8_t representation of vector v
 * @param[in] salt String containing a salt
 */
void HQC256_hqc_ciphertext_to_string(uint8_t *ct, const uint64_t *u, const uint64_t *v, const uint64_t *salt) {
    memcpy(ct, u, HQC256_VEC_N_SIZE_BYTES);
    memcpy(ct + HQC256_VEC_N_SIZE_BYTES, v, HQC256_VEC_N1N2_SIZE_BYTES);
    memcpy(ct + HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES, salt, HQC256_SALT_SIZE_BYTES);
}



/**
 * @brief Parse a ciphertext from a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and hash <b>d</b>.
 *
 * @param[out] u uint8_t representation of vector u
 * @param[out] v uint8_t representation of vector v
 * @param[out] d String containing the hash d
 * @param[in] ct String containing the ciphertext
 */
void HQC256_hqc_ciphertext_from_string(uint64_t *u, uint64_t *v, uint64_t *salt, const uint8_t *ct) {
    memcpy(u, ct, HQC256_VEC_N_SIZE_BYTES);
    memcpy(v, ct + HQC256_VEC_N_SIZE_BYTES, HQC256_VEC_N1N2_SIZE_BYTES);
    memcpy(salt, ct + HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES, HQC256_SALT_SIZE_BYTES);
}
