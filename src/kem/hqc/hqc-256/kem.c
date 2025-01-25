/**
 * @file kem.c
 * @brief Implementation of api.h
 */

#include "api.h"
#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "shake_ds.h"
#include "fips202.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
#include <immintrin.h>
#ifdef VERBOSE
#include <stdio.h>
#endif


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
int HQC256_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    #ifdef VERBOSE
        printf("\n\n\n\n### KEYGEN ###");
    #endif

    HQC256_hqc_pke_keygen(pk, sk);
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
int HQC256_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    #ifdef VERBOSE
        printf("\n\n\n\n### ENCAPS ###");
    #endif

    uint8_t theta[HQC256_SHAKE256_512_BYTES] = {0};
    uint8_t m[HQC256_VEC_K_SIZE_BYTES] = {0};
    static uint64_t u[HQC256_VEC_N_256_SIZE_64] = {0};
    uint64_t v[HQC256_VEC_N1N2_256_SIZE_64] = {0};
    uint8_t mc[HQC256_VEC_K_SIZE_BYTES + HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES] = {0};
    uint64_t salt[HQC256_SALT_SIZE_64] = {0};
    uint8_t tmp[HQC256_VEC_K_SIZE_BYTES + (HQC256_SALT_SIZE_BYTES * 2) + HQC256_SALT_SIZE_BYTES] = {0};
    shake256incctx shake256state;

    // Computing m
    HQC256_vect_set_random_from_prng((uint64_t *)m, HQC256_VEC_K_SIZE_64);

    // Computing theta
    HQC256_vect_set_random_from_prng(salt, HQC256_SALT_SIZE_64);
    memcpy(tmp, m, HQC256_VEC_K_SIZE_BYTES);
    memcpy(tmp + HQC256_VEC_K_SIZE_BYTES, pk, (HQC256_SALT_SIZE_BYTES * 2));
    memcpy(tmp + HQC256_VEC_K_SIZE_BYTES + (HQC256_SALT_SIZE_BYTES * 2), salt, HQC256_SALT_SIZE_BYTES);
    HQC256_shake256_512_ds(&shake256state, theta, tmp, HQC256_VEC_K_SIZE_BYTES + (HQC256_SALT_SIZE_BYTES * 2) + HQC256_SALT_SIZE_BYTES, HQC256_G_FCT_DOMAIN);

    // Encrypting m
    HQC256_hqc_pke_encrypt(u, v, (uint64_t *)m, theta, pk);

    // Computing shared secret
    memcpy(mc, m, HQC256_VEC_K_SIZE_BYTES);
    memcpy(mc + HQC256_VEC_K_SIZE_BYTES, u, HQC256_VEC_N_SIZE_BYTES);
    memcpy(mc + HQC256_VEC_K_SIZE_BYTES + HQC256_VEC_N_SIZE_BYTES, v, HQC256_VEC_N1N2_SIZE_BYTES);
    HQC256_shake256_512_ds(&shake256state, ss, mc, HQC256_VEC_K_SIZE_BYTES + HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES, HQC256_K_FCT_DOMAIN);

    // Computing ciphertext
    HQC256_hqc_ciphertext_to_string(ct, u, v, salt);

    #ifdef VERBOSE
        printf("\n\npk: "); for(int i = 0 ; i < HQC256_PUBLIC_KEY_BYTES ; ++i) printf("%02x", pk[i]);
        printf("\n\nm: "); HQC256_vect_print((uint64_t *)m, HQC256_VEC_K_SIZE_BYTES);
        printf("\n\ntheta: "); for(int i = 0 ; i < HQC256_SHAKE256_512_BYTES ; ++i) printf("%02x", theta[i]);
        printf("\n\nciphertext: "); for(int i = 0 ; i < HQC256_CIPHERTEXT_BYTES ; ++i) printf("%02x", ct[i]);
        printf("\n\nsecret 1: "); for(int i = 0 ; i < HQC256_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss[i]);
    #endif

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
int HQC256_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    #ifdef VERBOSE
        printf("\n\n\n\n### DECAPS ###");
    #endif

    uint8_t result;
    __m256i u_256[HQC256_VEC_N_256_SIZE_64 >> 2] = {0};
    uint64_t v[HQC256_VEC_N1N2_256_SIZE_64] = {0};
    uint8_t pk[HQC256_PUBLIC_KEY_BYTES] = {0};
    uint8_t m[HQC256_VEC_K_SIZE_BYTES] = {0};
    uint8_t sigma[HQC256_VEC_K_SIZE_BYTES] = {0};
    uint8_t theta[HQC256_SHAKE256_512_BYTES] = {0};
    uint64_t u2[HQC256_VEC_N_256_SIZE_64] = {0};
    uint64_t v2[HQC256_VEC_N1N2_256_SIZE_64] = {0};
    uint8_t mc[HQC256_VEC_K_SIZE_BYTES + HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES] = {0};
    uint64_t salt[HQC256_SALT_SIZE_64] = {0};
    uint8_t tmp[HQC256_VEC_K_SIZE_BYTES + (HQC256_SALT_SIZE_BYTES * 2) + HQC256_SALT_SIZE_BYTES] = {0};
    shake256incctx shake256state;

    // Retrieving u, v and d from ciphertext
    HQC256_hqc_ciphertext_from_string((uint64_t *) u_256, v, salt, ct);

    // Retrieving pk from sk
    memcpy(pk, sk + HQC256_SEED_BYTES, HQC256_PUBLIC_KEY_BYTES);

    // Decrypting
    result = HQC256_hqc_pke_decrypt((uint64_t *)m, sigma, u_256, v, sk);

    // Computing theta
    memcpy(tmp, m, HQC256_VEC_K_SIZE_BYTES);
    memcpy(tmp + HQC256_VEC_K_SIZE_BYTES, pk, (HQC256_SALT_SIZE_BYTES * 2));
    memcpy(tmp + HQC256_VEC_K_SIZE_BYTES + (HQC256_SALT_SIZE_BYTES * 2), salt, HQC256_SALT_SIZE_BYTES);
    HQC256_shake256_512_ds(&shake256state, theta, tmp, HQC256_VEC_K_SIZE_BYTES + (HQC256_SALT_SIZE_BYTES * 2) + HQC256_SALT_SIZE_BYTES, HQC256_G_FCT_DOMAIN);

    // Encrypting m'
    HQC256_hqc_pke_encrypt(u2, v2, (uint64_t *)m, theta, pk);

    // Check if c != c'
    result |= HQC256_vect_compare((uint8_t *) u_256, (uint8_t *) u2, HQC256_VEC_N_SIZE_BYTES);
    result |= HQC256_vect_compare((uint8_t *) v, (uint8_t *) v2, HQC256_VEC_N1N2_SIZE_BYTES);

    result = (uint8_t) (-((int16_t) result) >> 15);
    
    for (size_t i = 0; i < HQC256_VEC_K_SIZE_BYTES; ++i) {
        mc[i] = (m[i] & result) ^ (sigma[i] & ~result);
    }

    // Computing shared secret
    memcpy(mc + HQC256_VEC_K_SIZE_BYTES, u_256, HQC256_VEC_N_SIZE_BYTES);
    memcpy(mc + HQC256_VEC_K_SIZE_BYTES + HQC256_VEC_N_SIZE_BYTES, v, HQC256_VEC_N1N2_SIZE_BYTES);
    HQC256_shake256_512_ds(&shake256state, ss, mc, HQC256_VEC_K_SIZE_BYTES + HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES, HQC256_K_FCT_DOMAIN);

    #ifdef VERBOSE
        printf("\n\npk: "); for(int i = 0 ; i < HQC256_PUBLIC_KEY_BYTES ; ++i) printf("%02x", pk[i]);
        printf("\n\nsk: "); for(int i = 0 ; i < HQC256_SECRET_KEY_BYTES ; ++i) printf("%02x", sk[i]);
        printf("\n\nciphertext: "); for(int i = 0 ; i < HQC256_CIPHERTEXT_BYTES ; ++i) printf("%02x", ct[i]);
        printf("\n\nm: "); HQC256_vect_print((uint64_t *)m, HQC256_VEC_K_SIZE_BYTES);
        printf("\n\ntheta: "); for(int i = 0 ; i < HQC256_SHAKE256_512_BYTES ; ++i) printf("%02x", theta[i]);
        printf("\n\n\n# Checking Ciphertext- Begin #");
        printf("\n\nu2: "); HQC256_vect_print(u2, HQC256_VEC_N_SIZE_BYTES);
        printf("\n\nv2: "); HQC256_vect_print(v2, HQC256_VEC_N1N2_SIZE_BYTES);
        printf("\n\n# Checking Ciphertext - End #\n");
    #endif

    return -(~result & 1);
}
