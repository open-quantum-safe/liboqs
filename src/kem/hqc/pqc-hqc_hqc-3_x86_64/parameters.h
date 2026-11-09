/**
 * @file parameters.h
 * @brief Parameters of the HQC-KEM IND-CCA2 scheme
 */

#ifndef HQC_PARAMETERS_H
#define HQC_PARAMETERS_H

#include "hqc-kem.h"

#define CEIL_DIVIDE(a, b) (((a) / (b)) + ((a) % (b) == 0 ? 0 : 1)) /*!< Divide a by b and ceil the result*/
#define BITMASK(a, size)  ((1UL << (a % size)) - 1)                /*!< Create a mask*/

#define PARAM_N                     35851       ///< Define the parameter n of the scheme
#define PARAM_N1                    56          ///< Define the parameter n1 of the scheme (length of Reed-Solomon code)
#define PARAM_N2                    640         ///< Define the parameter n2 of the scheme (length of Duplicated Reed-Muller code)
#define PARAM_N1N2                  35840       ///< Define the length in bits of the concatenated code
#define PARAM_OMEGA                 101         ///< Define the parameter omega of the scheme
#define PARAM_OMEGA_E               114         ///< Define the parameter omega_e of the scheme
#define PARAM_OMEGA_R               114         ///< Define the parameter omega_r of the scheme
#define PARAM_SECURITY              192         ///< Define the security level corresponding to the chosen parameters
#define PARAM_SECURITY_BYTES        24          ///< Define the security level in bytes
#define PARAM_DFR_EXP               192         ///< Define the decryption failure rate corresponding to the chosen parameters

#define SECRET_KEY_BYTES_KEM        CRYPTO_SECRETKEYBYTES   ///< Define the size of the KEM secret key in bytes
#define PUBLIC_KEY_BYTES_KEM        CRYPTO_PUBLICKEYBYTES   ///< Define the size of the KEM public key in bytes
#define SHARED_SECRET_BYTES_KEM     CRYPTO_BYTES            ///< Define the size of the shared secret in bytes
#define CIPHERTEXT_BYTES_KEM        CRYPTO_CIPHERTEXTBYTES  ///< Define the size of the ciphertext in bytes

#define SECRET_KEY_BYTES_PKE        SEED_BYTES                       ///< Define the size of the PKE secret key in bytes
#define PUBLIC_KEY_BYTES_PKE        (SEED_BYTES + VEC_N_SIZE_BYTES)  ///< Define the size of the PKE public key in bytes

#define VEC_N_SIZE_BYTES            CEIL_DIVIDE(PARAM_N, 8)     ///< Size of array to store PARAM_N bits in bytes
#define VEC_K_SIZE_BYTES            PARAM_K                     ///< Size of array to store PARAM_K bits in bytes
#define VEC_N1_SIZE_BYTES           PARAM_N1                    ///< Size of array to store PARAM_N1 bits in bytes
#define VEC_N1N2_SIZE_BYTES         CEIL_DIVIDE(PARAM_N1N2, 8)  ///< Size of array to store PARAM_N1N2 bits in bytes

#define VEC_N_SIZE_64               CEIL_DIVIDE(PARAM_N, 64)    ///< Size of array to store PARAM_N bits in 64-bit words
#define VEC_N1_SIZE_64              CEIL_DIVIDE(PARAM_N1, 8)    ///< Size of array to store PARAM_N1 bits in 64-bit words
#define VEC_N1N2_SIZE_64            CEIL_DIVIDE(PARAM_N1N2, 64) ///< Size of array to store PARAM_N1N2 bits in 64-bit words

#define PARAM_N_MULT                36480       ///< Define the size of the array of 64-bit elements to store PARAM_N as 256-bit blocks
#define VEC_N_256_SIZE_64           (CEIL_DIVIDE(PARAM_N_MULT, 256) << 2)   ///< 256-bit block count for PARAM_N
#define VEC_N1N2_256_SIZE_64        (CEIL_DIVIDE(PARAM_N1N2, 256) << 2)     ///< 256-bit block count for PARAM_N1N2
#define VEC_N_PKE_WORDS             VEC_N_256_SIZE_64                       ///< Size of PKE u in 64-bit words
#define VEC_N1N2_PKE_WORDS          VEC_N1N2_256_SIZE_64                    ///< Size of PKE v in 64-bit words

#define PARAM_T                 16          ///< Define the error-correcting capacity (t) of the Reed-Solomon code
#define PARAM_M                     8           ///< Define the degree m of the Galois field GF(2^m)
#define PARAM_GF_POLY               0x11B       ///< Generator polynomial of GF(2^PARAM_M) in hexadecimal form
#define PARAM_GF_MUL_ORDER          255         ///< Size of the multiplicative group of GF(2^PARAM_M) (i.e., 2^PARAM_M−1)
#define PARAM_K                     24          ///< Define the size of the information bits of the Reed-Solomon code
#define PARAM_G                     33          ///< Define the size of the generator polynomial of the Reed-Solomon code

#define RS_POLY_COEFS \
    56, 180, 145, 30, 131, 105, 28, 60, 107, 33, 206, 184, 155, 249, 153, 231, \
    112, 11, 231, 1, 144, 201, 71, 62, 8, 150, 142, 246, 33, 214, 26, 150, \
    1  ///< Coefficients of the Reed-Solomon generator polynomial

#define SEED_BYTES                  32          ///< Define the size of the seed in bytes
#define SALT_BYTES                  16          ///< Define the size of a salt in bytes

#define PARAM_N_MU 119800ULL   ///<  Define a precomputed multiplier for Barrett reduction mu = floor(2^32 / PARAM_N)
#define UTILS_REJECTION_THRESHOLD   16742417  ///< Rejection threshold for uniform sampling in [0, PARAM_N)

#endif // HQC_PARAMETERS_H
