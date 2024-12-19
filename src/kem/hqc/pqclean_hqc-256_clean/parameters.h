#ifndef HQC_PARAMETERS_H
#define HQC_PARAMETERS_H


/**
 * @file parameters.h
 * @brief Parameters of the HQC_KEM IND-CCA2 scheme
 */
#include "api.h"


#define CEIL_DIVIDE(a, b)  (((a)+(b)-1)/(b)) /*!< Divide a by b and ceil the result*/

/*
  #define PARAM_N                               Define the parameter n of the scheme
  #define PARAM_N1                              Define the parameter n1 of the scheme (length of Reed-Solomon code)
  #define PARAM_N2                              Define the parameter n2 of the scheme (length of Duplicated Reed-Muller code)
  #define PARAM_N1N2                            Define the length in bits of the Concatenated code
  #define PARAM_OMEGA                           Define the parameter omega of the scheme
  #define PARAM_OMEGA_E                         Define the parameter omega_e of the scheme
  #define PARAM_OMEGA_R                         Define the parameter omega_r of the scheme

  #define SECRET_KEY_BYTES                      Define the size of the secret key in bytes
  #define PUBLIC_KEY_BYTES                      Define the size of the public key in bytes
  #define SHARED_SECRET_BYTES                   Define the size of the shared secret in bytes
  #define CIPHERTEXT_BYTES                      Define the size of the ciphertext in bytes

  #define VEC_N_SIZE_BYTES                      Define the size of the array used to store a PARAM_N sized vector in bytes
  #define VEC_K_SIZE_BYTES                      Define the size of the array used to store a PARAM_K sized vector in bytes
  #define VEC_N1Y_SIZE_BYTES                    Define the size of the array used to store a PARAM_N1 sized vector in bytes
  #define VEC_N1N2_SIZE_BYTES                   Define the size of the array used to store a PARAM_N1N2 sized vector in bytes

  #define VEC_N_SIZE_64                         Define the size of the array used to store a PARAM_N sized vector in 64 bits
  #define VEC_K_SIZE_64                         Define the size of the array used to store a PARAM_K sized vector in 64 bits
  #define VEC_N1_SIZE_64                        Define the size of the array used to store a PARAM_N1 sized vector in 64 bits
  #define VEC_N1N2_SIZE_64                      Define the size of the array used to store a PARAM_N1N2 sized vector in 64 bits

  #define PARAM_DELTA                           Define the parameter delta of the scheme (correcting capacity of the Reed-Solomon code)
  #define PARAM_M                               Define a positive integer
  #define PARAM_GF_POLY                         Generator polynomial of galois field GF(2^PARAM_M), represented in hexadecimial form
  #define PARAM_GF_POLY_WT                      Hamming weight of PARAM_GF_POLY
  #define PARAM_GF_POLY_M2                      Distance between the primitive polynomial first two set bits
  #define PARAM_GF_MUL_ORDER                    Define the size of the multiplicative group of GF(2^PARAM_M),  i.e 2^PARAM_M -1
  #define PARAM_K                               Define the size of the information bits of the Reed-Solomon code
  #define PARAM_G                               Define the size of the generator polynomial of Reed-Solomon code
  #define PARAM_FFT                             The additive FFT takes a 2^PARAM_FFT polynomial as input
                                                We use the FFT to compute the roots of sigma, whose degree if PARAM_DELTA=24
                                                The smallest power of 2 greater than 24+1 is 32=2^5
  #define RS_POLY_COEFS                         Coefficients of the generator polynomial of the Reed-Solomon code

  #define RED_MASK                              A mask for the higher bits of a vector
  #define SHAKE256_512_BYTES                    Define the size of SHAKE-256 output in bytes
  #define SEED_BYTES                            Define the size of the seed in bytes
  #define SALT_SIZE_BYTES                       Define the size of a salt in bytes
*/

#define PARAM_N                                                             57637
#define PARAM_N1                                90
#define PARAM_N2                                640
#define PARAM_N1N2                              57600
#define PARAM_OMEGA                             131
#define PARAM_OMEGA_E                           149
#define PARAM_OMEGA_R                           149

#define SECRET_KEY_BYTES                        PQCLEAN_HQC256_CLEAN_CRYPTO_SECRETKEYBYTES
#define PUBLIC_KEY_BYTES                        PQCLEAN_HQC256_CLEAN_CRYPTO_PUBLICKEYBYTES
#define SHARED_SECRET_BYTES                     PQCLEAN_HQC256_CLEAN_CRYPTO_BYTES
#define CIPHERTEXT_BYTES                        PQCLEAN_HQC256_CLEAN_CRYPTO_CIPHERTEXTBYTES

#define VEC_N_SIZE_BYTES                        CEIL_DIVIDE(PARAM_N, 8)
#define VEC_K_SIZE_BYTES                        PARAM_K
#define VEC_N1_SIZE_BYTES                       PARAM_N1
#define VEC_N1N2_SIZE_BYTES                     CEIL_DIVIDE(PARAM_N1N2, 8)

#define VEC_N_SIZE_64                           CEIL_DIVIDE(PARAM_N, 64)
#define VEC_K_SIZE_64                           CEIL_DIVIDE(PARAM_K, 8)
#define VEC_N1_SIZE_64                          CEIL_DIVIDE(PARAM_N1, 8)
#define VEC_N1N2_SIZE_64                        CEIL_DIVIDE(PARAM_N1N2, 64)

#define PARAM_DELTA                             29
#define PARAM_M                                 8
#define PARAM_GF_POLY                           0x11D
#define PARAM_GF_POLY_WT                      5
#define PARAM_GF_POLY_M2                        4
#define PARAM_GF_MUL_ORDER                      255
#define PARAM_K                                 32
#define PARAM_G                                 59
#define PARAM_FFT                               5
#define RS_POLY_COEFS 49,167,49,39,200,121,124,91,240,63,148,71,150,123,87,101,32,215,159,71,201,115,97,210,186,183,141,217,123,12,31,243,180,219,152,239,99,141,4,246,191,144,8,232,47,27,141,178,130,64,124,47,39,188,216,48,199,187,1

#define RED_MASK                                0x1fffffffff
#define SHAKE256_512_BYTES                    64
#define SEED_BYTES                              40
#define SALT_SIZE_BYTES                       16

#endif
