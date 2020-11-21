#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
/**
 * @file parsing.c
 * @brief Functions to parse secret key, public key and ciphertext of the HQC scheme
 */


void PQCLEAN_HQCRMRS128_CLEAN_store8(unsigned char *out, uint64_t in) {
    out[0] = (in >> 0x00) & 0xFF;
    out[1] = (in >> 0x08) & 0xFF;
    out[2] = (in >> 0x10) & 0xFF;
    out[3] = (in >> 0x18) & 0xFF;
    out[4] = (in >> 0x20) & 0xFF;
    out[5] = (in >> 0x28) & 0xFF;
    out[6] = (in >> 0x30) & 0xFF;
    out[7] = (in >> 0x38) & 0xFF;
}


uint64_t PQCLEAN_HQCRMRS128_CLEAN_load8(const unsigned char *in) {
    uint64_t ret = in[7];

    for (int8_t i = 6; i >= 0; i--) {
        ret <<= 8;
        ret |= in[i];
    }

    return ret;
}

void PQCLEAN_HQCRMRS128_CLEAN_load8_arr(uint64_t *out64, size_t outlen, const uint8_t *in8, size_t inlen) {
    size_t index_in = 0;
    size_t index_out = 0;

    // first copy by 8 bytes
    if (inlen >= 8 && outlen >= 1) {
        while (index_out < outlen && index_in + 8 <= inlen) {
            out64[index_out] = PQCLEAN_HQCRMRS128_CLEAN_load8(in8 + index_in);

            index_in += 8;
            index_out += 1;
        }
    }

    // we now need to do the last 7 bytes if necessary
    if (index_in >= inlen || index_out >= outlen) {
        return;
    }
    out64[index_out] = in8[inlen - 1];
    for (int8_t i = (int8_t)(inlen - index_in) - 2; i >= 0; i--) {
        out64[index_out] <<= 8;
        out64[index_out] |= in8[index_in + i];
    }
}

void PQCLEAN_HQCRMRS128_CLEAN_store8_arr(uint8_t *out8, size_t outlen, const uint64_t *in64, size_t inlen) {
    for (size_t index_out = 0, index_in = 0; index_out < outlen && index_in < inlen;) {
        out8[index_out] = (in64[index_in] >> ((index_out % 8) * 8)) & 0xFF;
        index_out++;
        if (index_out % 8 == 0) {
            index_in++;
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
 * @param[in] pk String containing the public key
 */
void PQCLEAN_HQCRMRS128_CLEAN_hqc_secret_key_to_string(uint8_t *sk, const uint8_t *sk_seed, const uint8_t *pk) {
    memcpy(sk, sk_seed, SEED_BYTES);
    sk += SEED_BYTES;
    memcpy(sk, pk, PUBLIC_KEY_BYTES);
}

/**
 * @brief Parse a secret key from a string
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] x uint64_t representation of vector x
 * @param[out] y uint32_t representation of vector y
 * @param[out] pk String containing the public key
 * @param[in] sk String containing the secret key
 */
void PQCLEAN_HQCRMRS128_CLEAN_hqc_secret_key_from_string(uint64_t *x, uint32_t *y, uint8_t *pk, const uint8_t *sk) {
    AES_XOF_struct sk_seedexpander;
    uint8_t sk_seed[SEED_BYTES] = {0};

    memcpy(sk_seed, sk, SEED_BYTES);
    sk += SEED_BYTES;
    memcpy(pk, sk, PUBLIC_KEY_BYTES);

    seedexpander_init(&sk_seedexpander, sk_seed, sk_seed + 32, SEEDEXPANDER_MAX_LENGTH);
    PQCLEAN_HQCRMRS128_CLEAN_vect_set_random_fixed_weight(&sk_seedexpander, x, PARAM_OMEGA);
    PQCLEAN_HQCRMRS128_CLEAN_vect_set_random_fixed_weight_by_coordinates(&sk_seedexpander, y, PARAM_OMEGA);
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
void PQCLEAN_HQCRMRS128_CLEAN_hqc_public_key_to_string(uint8_t *pk, const uint8_t *pk_seed, const uint64_t *s) {
    memcpy(pk, pk_seed, SEED_BYTES);
    PQCLEAN_HQCRMRS128_CLEAN_store8_arr(pk + SEED_BYTES, VEC_N_SIZE_BYTES, s, VEC_N_SIZE_64);
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
void PQCLEAN_HQCRMRS128_CLEAN_hqc_public_key_from_string(uint64_t *h, uint64_t *s, const uint8_t *pk) {
    AES_XOF_struct pk_seedexpander;
    uint8_t pk_seed[SEED_BYTES] = {0};

    memcpy(pk_seed, pk, SEED_BYTES);
    pk += SEED_BYTES;
    PQCLEAN_HQCRMRS128_CLEAN_load8_arr(s, VEC_N_SIZE_64, pk, VEC_N_SIZE_BYTES);

    seedexpander_init(&pk_seedexpander, pk_seed, pk_seed + 32, SEEDEXPANDER_MAX_LENGTH);
    PQCLEAN_HQCRMRS128_CLEAN_vect_set_random(&pk_seedexpander, h);
}


/**
 * @brief Parse a ciphertext into a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and hash <b>d</b>.
 *
 * @param[out] ct String containing the ciphertext
 * @param[in] u uint8_t representation of vector u
 * @param[in] v uint8_t representation of vector v
 * @param[in] d String containing the hash d
 */
void PQCLEAN_HQCRMRS128_CLEAN_hqc_ciphertext_to_string(uint8_t *ct, const uint64_t *u, const uint64_t *v, const uint8_t *d) {
    PQCLEAN_HQCRMRS128_CLEAN_store8_arr(ct, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
    ct += VEC_N_SIZE_BYTES;
    PQCLEAN_HQCRMRS128_CLEAN_store8_arr(ct, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
    ct += VEC_N1N2_SIZE_BYTES;
    memcpy(ct, d, SHA512_BYTES);
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
void PQCLEAN_HQCRMRS128_CLEAN_hqc_ciphertext_from_string(uint64_t *u, uint64_t *v, uint8_t *d, const uint8_t *ct) {
    PQCLEAN_HQCRMRS128_CLEAN_load8_arr(u, VEC_N_SIZE_64, ct, VEC_N_SIZE_BYTES);
    ct += VEC_N_SIZE_BYTES;
    PQCLEAN_HQCRMRS128_CLEAN_load8_arr(v, VEC_N1N2_SIZE_64, ct, VEC_N1N2_SIZE_BYTES);
    ct += VEC_N1N2_SIZE_BYTES;
    memcpy(d, ct, SHA512_BYTES);
}
