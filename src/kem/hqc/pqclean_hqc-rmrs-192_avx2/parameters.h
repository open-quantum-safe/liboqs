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
  #define PARAM_SECURITY                        Define the security level corresponding to the chosen parameters
  #define PARAM_DFR_EXP                         Define the decryption failure rate corresponding to the chosen parameters

  #define SECRET_KEY_BYTES                      Define the size of the secret key in bytes
  #define PUBLIC_KEY_BYTES                      Define the size of the public key in bytes
  #define SHARED_SECRET_BYTES                   Define the size of the shared secret in bytes
  #define CIPHERTEXT_BYTES                      Define the size of the ciphertext in bytes

  #define UTILS_REJECTION_THRESHOLD             Define the rejection threshold used to generate given weight vectors (see vector_set_random_fixed_weight function)
  #define VEC_N_SIZE_BYTES                      Define the size of the array used to store a PARAM_N sized vector in bytes
  #define VEC_K_SIZE_BYTES                      Define the size of the array used to store a PARAM_K sized vector in bytes
  #define VEC_N1Y_SIZE_BYTES                    Define the size of the array used to store a PARAM_N1 sized vector in bytes
  #define VEC_N1N2_SIZE_BYTES                   Define the size of the array used to store a PARAM_N1N2 sized vector in bytes

  #define VEC_N_SIZE_64                         Define the size of the array used to store a PARAM_N sized vector in 64 bits
  #define VEC_K_SIZE_64                         Define the size of the array used to store a PARAM_K sized vector in 64 bits
  #define VEC_N1_SIZE_64                        Define the size of the array used to store a PARAM_N1 sized vector in 64 bits
  #define VEC_N1N2_SIZE_64                      Define the size of the array used to store a PARAM_N1N2 sized vector in 64 bits

  #define VEC_N_256_SIZE_64                     Define the size of the array of 64 bits elements used to store an array of size PARAM_N considered as elements of 256 bits
  #define VEC_N1N2_256_SIZE_64                  Define the size of the array of 64 bits elements used to store an array of size PARAM_N1N2 considered as elements of 256 bits

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

  #define RED_MASK                              A mask fot the higher bits of a vector
  #define SHA512_BYTES                          Define the size of SHA512 output in bytes
  #define SEED_BYTES                            Define the size of the seed in bytes
  #define SEEDEXPANDER_MAX_LENGTH               Define the seed expander max length
*/

#define PARAM_N                                 35851
#define PARAM_N1                                56
#define PARAM_N2                                640
#define PARAM_N1N2                              35840
#define PARAM_OMEGA                             100
#define PARAM_OMEGA_E                           114
#define PARAM_OMEGA_R                           114
#define PARAM_SECURITY                          192
#define PARAM_DFR_EXP                           192

#define SECRET_KEY_BYTES                        PQCLEAN_HQCRMRS192_AVX2_CRYPTO_SECRETKEYBYTES
#define PUBLIC_KEY_BYTES                        PQCLEAN_HQCRMRS192_AVX2_CRYPTO_PUBLICKEYBYTES
#define SHARED_SECRET_BYTES                     PQCLEAN_HQCRMRS192_AVX2_CRYPTO_BYTES
#define CIPHERTEXT_BYTES                        PQCLEAN_HQCRMRS192_AVX2_CRYPTO_CIPHERTEXTBYTES

#define UTILS_REJECTION_THRESHOLD               16742417
#define VEC_N_SIZE_BYTES                        CEIL_DIVIDE(PARAM_N, 8)
#define VEC_K_SIZE_BYTES                        PARAM_K
#define VEC_N1_SIZE_BYTES                       PARAM_N1
#define VEC_N1N2_SIZE_BYTES                     CEIL_DIVIDE(PARAM_N1N2, 8)

#define VEC_N_SIZE_64                         CEIL_DIVIDE(PARAM_N, 64)
#define VEC_K_SIZE_64                           CEIL_DIVIDE(PARAM_K, 8)
#define VEC_N1_SIZE_64                          CEIL_DIVIDE(PARAM_N1, 8)
#define VEC_N1N2_SIZE_64                        CEIL_DIVIDE(PARAM_N1N2, 64)

#define PARAM_N_MULT                            (9*256*CEIL_DIVIDE(CEIL_DIVIDE(PARAM_N, 9), 256))
#define VEC_N_256_SIZE_64                       (PARAM_N_MULT / 64)
#define VEC_N1N2_256_SIZE_64                    (CEIL_DIVIDE(PARAM_N1N2, 256) << 2)

#define PARAM_DELTA                             16
#define PARAM_M                                 8
#define PARAM_GF_POLY                           0x11D
#define PARAM_GF_POLY_WT                        5
#define PARAM_GF_POLY_M2                        4
#define PARAM_GF_MUL_ORDER                      255
#define PARAM_K                                 24
#define PARAM_G                                 33
#define PARAM_FFT                               5
#define RS_POLY_COEFS 45,216,239,24,253,104,27,40,107,50,163,210,227,134,224,158,119,13,158,1,238,164,82,43,15,232,246,142,50,189,29,232,1

#define RED_MASK                                0x7ff
#define SHA512_BYTES                            64
#define SEED_BYTES                              40
#define SEEDEXPANDER_MAX_LENGTH                 4294967295

#endif
