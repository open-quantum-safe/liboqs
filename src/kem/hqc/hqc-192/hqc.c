/**
 * @file hqc.c
 * @brief Implementation of hqc.h
 */

#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "shake_prng.h"
#include "gf2x.h"
#include "code.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
#ifdef VERBOSE
#include <stdio.h>
#endif


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
void HQC192_hqc_pke_keygen(unsigned char* pk, unsigned char* sk) {
    seedexpander_state sk_seedexpander;
    seedexpander_state pk_seedexpander;
    uint8_t sk_seed[HQC192_SEED_BYTES] = {0};
    uint8_t sigma[HQC192_VEC_K_SIZE_BYTES] = {0};
    uint8_t pk_seed[HQC192_SEED_BYTES] = {0};
    static __m256i h_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static __m256i y_256[HQC192_VEC_N_256_SIZE_64 >> 2];   
    static __m256i x_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static uint64_t s[HQC192_VEC_N_256_SIZE_64];
    static __m256i tmp_256[HQC192_VEC_N_256_SIZE_64 >> 2];

    #ifdef __STDC_LIB_EXT1__
        memset_s(x_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset_s(y_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset_s(h_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    #else
        memset(x_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset(y_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset(h_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    #endif


    // Create seed_expanders for public key and secret key
    HQC192_shake_prng(sk_seed, HQC192_SEED_BYTES);
    HQC192_shake_prng(sigma, HQC192_VEC_K_SIZE_BYTES);
    HQC192_seedexpander_init(&sk_seedexpander, sk_seed, HQC192_SEED_BYTES);

    HQC192_shake_prng(pk_seed, HQC192_SEED_BYTES);
    HQC192_seedexpander_init(&pk_seedexpander, pk_seed, HQC192_SEED_BYTES);

    // Compute secret key
    HQC192_vect_set_random_fixed_weight(&sk_seedexpander, y_256, HQC192_PARAM_OMEGA);
    HQC192_vect_set_random_fixed_weight(&sk_seedexpander, x_256, HQC192_PARAM_OMEGA);

    // Compute public key
    HQC192_vect_set_random(&pk_seedexpander, (uint64_t *) h_256);
    HQC192_vect_mul(tmp_256, y_256, h_256);
    HQC192_vect_add(s, (uint64_t *) x_256, (uint64_t *) tmp_256, HQC192_VEC_N_256_SIZE_64);

    // Parse keys to string
    HQC192_hqc_public_key_to_string(pk, pk_seed, s);
    HQC192_hqc_secret_key_to_string(sk, sk_seed, sigma, pk);

    #ifdef VERBOSE
        printf("\n\nsk_seed: "); for(int i = 0 ; i < HQC192_SEED_BYTES ; ++i) printf("%02x", sk_seed[i]);
        printf("\n\nsigma: "); for(int i = 0 ; i < HQC192_VEC_K_SIZE_BYTES ; ++i) printf("%02x", sigma[i]);
        printf("\n\nx: "); HQC192_vect_print((uint64_t *) x_256, HQC192_VEC_N_SIZE_BYTES);
        printf("\n\ny: "); HQC192_vect_print((uint64_t *) y_256, HQC192_VEC_N_SIZE_BYTES);

        printf("\n\npk_seed: "); for(int i = 0 ; i < HQC192_SEED_BYTES ; ++i) printf("%02x", pk_seed[i]);
        printf("\n\nh: "); HQC192_vect_print((uint64_t *) h_256, HQC192_VEC_N_SIZE_BYTES);
        printf("\n\ns: "); HQC192_vect_print(s, HQC192_VEC_N_SIZE_BYTES);

        printf("\n\nsk: "); for(int i = 0 ; i < HQC192_SECRET_KEY_BYTES ; ++i) printf("%02x", sk[i]);
        printf("\n\npk: "); for(int i = 0 ; i < HQC192_PUBLIC_KEY_BYTES ; ++i) printf("%02x", pk[i]);
    #endif
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
void HQC192_hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk) {
    seedexpander_state seedexpander;
    static __m256i HQC192_h_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static __m256i HQC192_s_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static __m256i HQC192_r2_256[HQC192_VEC_N_256_SIZE_64 >> 2];

    static __m256i HQC192_r1_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static __m256i HQC192_e_256[HQC192_VEC_N_256_SIZE_64 >> 2];

    static __m256i HQC192_tmp1_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static __m256i HQC192_tmp2_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static __m256i HQC192_tmp3_256[HQC192_VEC_N_256_SIZE_64 >> 2];
    static uint64_t HQC192_tmp4[HQC192_VEC_N_256_SIZE_64];

    #ifdef __STDC_LIB_EXT1__
        memset_s(HQC192_r2_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset_s(HQC192_h_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset_s(HQC192_s_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset_s(HQC192_r1_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset_s(HQC192_e_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    #else
        memset(HQC192_r2_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset(HQC192_h_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset(HQC192_s_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset(HQC192_r1_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
        memset(HQC192_e_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    #endif

    // Create seed_expander from theta
    HQC192_seedexpander_init(&seedexpander, theta, HQC192_SEED_BYTES);

    // Retrieve h and s from public key
    HQC192_hqc_public_key_from_string((uint64_t *) HQC192_h_256, (uint64_t *) HQC192_s_256, pk);

    // Generate r1, r2 and e
    HQC192_vect_set_random_fixed_weight(&seedexpander, HQC192_r2_256, HQC192_PARAM_OMEGA_R);
    HQC192_vect_set_random_fixed_weight(&seedexpander, HQC192_e_256, HQC192_PARAM_OMEGA_E);
    HQC192_vect_set_random_fixed_weight(&seedexpander, HQC192_r1_256, HQC192_PARAM_OMEGA_R);

    // Compute u = r1 + r2.h
    HQC192_vect_mul(HQC192_tmp1_256, HQC192_r2_256, HQC192_h_256);
    HQC192_vect_add(u, (uint64_t *) HQC192_r1_256, (uint64_t *) HQC192_tmp1_256, HQC192_VEC_N_256_SIZE_64);

    // Compute v = m.G by encoding the message
    HQC192_code_encode(v, m);
    HQC192_vect_resize((uint64_t *) HQC192_tmp2_256, HQC192_PARAM_N, v, HQC192_PARAM_N1N2);

    // Compute v = m.G + s.r2 + e
    HQC192_vect_mul(HQC192_tmp3_256, HQC192_r2_256, HQC192_s_256);
    HQC192_vect_add(HQC192_tmp4, (uint64_t *) HQC192_e_256, (uint64_t *) HQC192_tmp3_256, HQC192_VEC_N_256_SIZE_64);
    HQC192_vect_add((uint64_t *) HQC192_tmp3_256, (uint64_t *) HQC192_tmp2_256, HQC192_tmp4, HQC192_VEC_N_256_SIZE_64);
    HQC192_vect_resize(v, HQC192_PARAM_N1N2, (uint64_t *) HQC192_tmp3_256, HQC192_PARAM_N);

    #ifdef VERBOSE
        printf("\n\nh: "); HQC192_vect_print((uint64_t *) HQC192_h_256, VEC_N_SIZE_BYTES);
        printf("\n\ns: "); HQC192_vect_print((uint64_t *) HQC192_s_256, VEC_N_SIZE_BYTES);
        printf("\n\nr1: "); HQC192_vect_print((uint64_t *) HQC192_r1_256, VEC_N_SIZE_BYTES);
        printf("\n\nr2: "); HQC192_vect_print((uint64_t *) HQC192_r2_256, VEC_N_SIZE_BYTES);
        printf("\n\ne: "); HQC192_vect_print((uint64_t *) HQC192_e_256, VEC_N_SIZE_BYTES);
        printf("\n\ntmp3_256: "); HQC192_vect_print((uint64_t *) HQC192_tmp3_256, VEC_N_SIZE_BYTES);

        printf("\n\nu: "); HQC192_vect_print(u, VEC_N_SIZE_BYTES);
        printf("\n\nv: "); HQC192_vect_print(v, VEC_N1N2_SIZE_BYTES);
    #endif
}



/**
 * @brief Decryption of the HQC_PKE IND_CPA scheme
 *
 * @param[out] m Vector representing the decrypted message
 * @param[in] u Vector u (first part of the ciphertext)
 * @param[in] v Vector v (second part of the ciphertext)
 * @param[in] sk String containing the secret key
 * @returns 0 
 */
uint8_t HQC192_hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const __m256i *u_256, const uint64_t *v, const uint8_t *sk) {
    static __m256i HQC192_y_256[HQC192_VEC_N_256_SIZE_64 >> 2] = {0};
    uint8_t HQC192_pk[HQC192_PUBLIC_KEY_BYTES] = {0};
    static uint64_t HQC192_tmp1[HQC192_VEC_N_256_SIZE_64] = {0};
    static uint64_t HQC192_tmp2[HQC192_VEC_N_256_SIZE_64] = {0};
    static __m256i HQC192_tmp3_256[HQC192_VEC_N_256_SIZE_64 >> 2];

    #ifdef __STDC_LIB_EXT1__
        memset_s(HQC192_y_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    #else
        memset(HQC192_y_256, 0, (HQC192_VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    #endif

    // Retrieve x, y, pk from secret key
    HQC192_hqc_secret_key_from_string(HQC192_y_256, sigma, HQC192_pk, sk);

    // Compute v - u.y
    HQC192_vect_resize(HQC192_tmp1, HQC192_PARAM_N, v, HQC192_PARAM_N1N2);
    HQC192_vect_mul(HQC192_tmp3_256, HQC192_y_256, u_256);
    HQC192_vect_add(HQC192_tmp2, HQC192_tmp1, (uint64_t *) HQC192_tmp3_256, HQC192_VEC_N_256_SIZE_64);

    #ifdef VERBOSE
        printf("\n\nu: "); HQC192_vect_print((uint64_t *) u_256, VEC_N_SIZE_BYTES);
        printf("\n\nv: "); HQC192_vect_print(v, VEC_N1N2_SIZE_BYTES);
        printf("\n\ny: "); HQC192_vect_print((uint64_t *) HQC192_y_256, VEC_N_SIZE_BYTES);
        printf("\n\nv - u.y: "); HQC192_vect_print(HQC192_tmp2, VEC_N_SIZE_BYTES);
    #endif

    // Compute m by decoding v - u.y
    HQC192_code_decode(m, HQC192_tmp2);
    
    return 0;
}
