#include "parameters.h"
#include "parsing.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
/**
 * @file parsing.c
 * @brief Functions to parse secret key, public key and ciphertext of the HQC scheme
 */

static uint64_t load8(const uint8_t *in) {
    uint64_t ret = in[7];

    for (int8_t i = 6; i >= 0; --i) {
        ret <<= 8;
        ret |= in[i];
    }

    return ret;
}

void PQCLEAN_HQC128_CLEAN_load8_arr(uint64_t *out64, size_t outlen, const uint8_t *in8, size_t inlen) {
    size_t index_in = 0;
    size_t index_out = 0;

    // first copy by 8 bytes
    if (inlen >= 8 && outlen >= 1) {
        while (index_out < outlen && index_in + 8 <= inlen) {
            out64[index_out] = load8(in8 + index_in);

            index_in += 8;
            index_out += 1;
        }
    }

    // we now need to do the last 7 bytes if necessary
    if (index_in >= inlen || index_out >= outlen) {
        return;
    }
    out64[index_out] = in8[inlen - 1];
    for (int8_t i = (int8_t)(inlen - index_in) - 2; i >= 0; --i) {
        out64[index_out] <<= 8;
        out64[index_out] |= in8[index_in + i];
    }
}

void PQCLEAN_HQC128_CLEAN_store8_arr(uint8_t *out8, size_t outlen, const uint64_t *in64, size_t inlen) {
    for (size_t index_out = 0, index_in = 0; index_out < outlen && index_in < inlen;) {
        out8[index_out] = (in64[index_in] >> ((index_out % 8) * 8)) & 0xFF;
        ++index_out;
        if (index_out % 8 == 0) {
            ++index_in;
        }
    }
}

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
void PQCLEAN_HQC128_CLEAN_hqc_secret_key_to_string(uint8_t *sk, const uint8_t *sk_seed, const uint8_t *sigma, const uint8_t *pk) {
    memcpy(sk, sk_seed, SEED_BYTES);
    memcpy(sk + SEED_BYTES, sigma, VEC_K_SIZE_BYTES);
    memcpy(sk + SEED_BYTES + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
}

/**
 * @brief Parse a secret key from a string
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] x uint64_t representation of vector x
 * @param[out] y uint64_t representation of vector y
 * @param[out] pk String containing the public key
 * @param[in] sk String containing the secret key
 */
void PQCLEAN_HQC128_CLEAN_hqc_secret_key_from_string(uint64_t *x, uint64_t *y, uint8_t *sigma, uint8_t *pk, const uint8_t *sk) {
    seedexpander_state sk_seedexpander;

    memcpy(sigma, sk + SEED_BYTES, VEC_K_SIZE_BYTES);
    PQCLEAN_HQC128_CLEAN_seedexpander_init(&sk_seedexpander, sk, SEED_BYTES);

    PQCLEAN_HQC128_CLEAN_vect_set_random_fixed_weight(&sk_seedexpander, x, PARAM_OMEGA);
    PQCLEAN_HQC128_CLEAN_vect_set_random_fixed_weight(&sk_seedexpander, y, PARAM_OMEGA);
    memcpy(pk, sk + SEED_BYTES + VEC_K_SIZE_BYTES, PUBLIC_KEY_BYTES);

    PQCLEAN_HQC128_CLEAN_seedexpander_release(&sk_seedexpander);
}

/**
 * @brief Parse a public key into a string
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>
 *
 * @param[out] pk String containing the public key
 * @param[in] pk_seed Seed used to generate the public key
 * @param[in] s uint64_t representation of vector s
 */
void PQCLEAN_HQC128_CLEAN_hqc_public_key_to_string(uint8_t *pk, const uint8_t *pk_seed, const uint64_t *s) {
    memcpy(pk, pk_seed, SEED_BYTES);
    PQCLEAN_HQC128_CLEAN_store8_arr(pk + SEED_BYTES, VEC_N_SIZE_BYTES, s, VEC_N_SIZE_64);
}

/**
 * @brief Parse a public key from a string
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>
 *
 * @param[out] h uint64_t representation of vector h
 * @param[out] s uint64_t representation of vector s
 * @param[in] pk String containing the public key
 */
void PQCLEAN_HQC128_CLEAN_hqc_public_key_from_string(uint64_t *h, uint64_t *s, const uint8_t *pk) {
    seedexpander_state pk_seedexpander;

    PQCLEAN_HQC128_CLEAN_seedexpander_init(&pk_seedexpander, pk, SEED_BYTES);
    PQCLEAN_HQC128_CLEAN_vect_set_random(&pk_seedexpander, h);

    PQCLEAN_HQC128_CLEAN_load8_arr(s, VEC_N_SIZE_64, pk + SEED_BYTES, VEC_N_SIZE_BYTES);

    PQCLEAN_HQC128_CLEAN_seedexpander_release(&pk_seedexpander);
}

/**
 * @brief Parse a ciphertext into a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and salt.
 *
 * @param[out] ct String containing the ciphertext
 * @param[in] u uint64_t representation of vector u
 * @param[in] v uint64_t representation of vector v
 * @param[in] salt String containing a salt
 */
void PQCLEAN_HQC128_CLEAN_hqc_ciphertext_to_string(uint8_t *ct, const uint64_t *u, const uint64_t *v, const uint8_t *salt) {
    PQCLEAN_HQC128_CLEAN_store8_arr(ct, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
    PQCLEAN_HQC128_CLEAN_store8_arr(ct + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
    memcpy(ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, salt, SALT_SIZE_BYTES);
}

/**
 * @brief Parse a ciphertext from a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and salt.
 *
 * @param[out] u uint64_t representation of vector u
 * @param[out] v uint64_t representation of vector v
 * @param[out] d String containing the hash d
 * @param[in] ct String containing the ciphertext
 */
void PQCLEAN_HQC128_CLEAN_hqc_ciphertext_from_string(uint64_t *u, uint64_t *v, uint8_t *salt, const uint8_t *ct) {
    PQCLEAN_HQC128_CLEAN_load8_arr(u, VEC_N_SIZE_64, ct, VEC_N_SIZE_BYTES);
    PQCLEAN_HQC128_CLEAN_load8_arr(v, VEC_N1N2_SIZE_64, ct + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES);
    memcpy(salt, ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, SALT_SIZE_BYTES);
}
