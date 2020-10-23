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

  #define PARAM_DELTA                           Define the parameter delta of the scheme (correcting capacity of the Reed-Solomon code)
  #define PARAM_M                               Define a positive integer
  #define PARAM_GF_POLY                         Generator polynomial of galois field GF(2^PARAM_M), represented in hexadecimial form
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

#define PARAM_N                 20533
#define PARAM_N1                                80
#define PARAM_N2                                256
#define PARAM_N1N2                              20480
#define PARAM_OMEGA                             67
#define PARAM_OMEGA_E                           77
#define PARAM_OMEGA_R                           77
#define PARAM_SECURITY                          128
#define PARAM_DFR_EXP                           128

#define SECRET_KEY_BYTES                        PQCLEAN_HQCRMRS128_CLEAN_CRYPTO_SECRETKEYBYTES
#define PUBLIC_KEY_BYTES                        PQCLEAN_HQCRMRS128_CLEAN_CRYPTO_PUBLICKEYBYTES
#define SHARED_SECRET_BYTES                     PQCLEAN_HQCRMRS128_CLEAN_CRYPTO_BYTES
#define CIPHERTEXT_BYTES                        PQCLEAN_HQCRMRS128_CLEAN_CRYPTO_CIPHERTEXTBYTES

#define UTILS_REJECTION_THRESHOLD               16775461
#define VEC_N_SIZE_BYTES                        CEIL_DIVIDE(PARAM_N, 8)
#define VEC_K_SIZE_BYTES                        PARAM_K
#define VEC_N1_SIZE_BYTES                       PARAM_N1
#define VEC_N1N2_SIZE_BYTES                     CEIL_DIVIDE(PARAM_N1N2, 8)

#define VEC_N_SIZE_64                           CEIL_DIVIDE(PARAM_N, 64)
#define VEC_K_SIZE_64                           CEIL_DIVIDE(PARAM_K, 8)
#define VEC_N1_SIZE_64                          CEIL_DIVIDE(PARAM_N1, 8)
#define VEC_N1N2_SIZE_64                        CEIL_DIVIDE(PARAM_N1N2, 64)

#define PARAM_DELTA                             24
#define PARAM_M                             8
#define PARAM_GF_POLY                           0x11D
#define PARAM_GF_MUL_ORDER                      255
#define PARAM_K                                 32
#define PARAM_G                                 49
#define PARAM_FFT                               5
#define RS_POLY_COEFS               228,231,214,81,113,204,19,169,10,244,117,219,130,12,160,151,195,170,150,151,251,218,245,166,149,183,109,176,148,218,21,161,240,25,15,71,62,5,17,32,157,194,73,195,218,14,12,122,1

#define RED_MASK                                0x1fffffffffffffUL
#define SHA512_BYTES                            64
#define SEED_BYTES                              40
#define SEEDEXPANDER_MAX_LENGTH                 4294967295

#endif
