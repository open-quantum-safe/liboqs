#ifndef HQC192_PARAMETERS_H
#define HQC192_PARAMETERS_H

/**
 * @file parameters.h
 * @brief Parameters of the HQC_KEM IND-CCA2 scheme
 */

#include "api.h"

#define HQC192_CEIL_DIVIDE(a, b)  (((a)/(b)) + ((a) % (b) == 0 ? 0 : 1)) /*!< Divide a by b and ceil the result*/
#define HQC192_BITMASK(a, size) ((1UL << (a % size)) - 1) /*!< Create a mask*/

/*
  #define HQC192_PARAM_N                             	Define the parameter n of the scheme
  #define HQC192_PARAM_N1                            	Define the parameter n1 of the scheme (length of Reed-Solomon code)
  #define HQC192_PARAM_N2                            	Define the parameter n2 of the scheme (length of Duplicated Reed-Muller code)
  #define HQC192_PARAM_N1N2                          	Define the length in bits of the Concatenated code
  #define HQC192_PARAM_OMEGA                         	Define the parameter omega of the scheme
  #define HQC192_PARAM_OMEGA_E                       	Define the parameter omega_e of the scheme
  #define HQC192_PARAM_OMEGA_R                       	Define the parameter omega_r of the scheme
  #define HQC192_PARAM_SECURITY                      	Define the security level corresponding to the chosen parameters
  #define HQC192_PARAM_DFR_EXP                       	Define the decryption failure rate corresponding to the chosen parameters

  #define HQC192_SECRET_KEY_BYTES                    	Define the size of the secret key in bytes
  #define HQC192_PUBLIC_KEY_BYTES                    	Define the size of the public key in bytes
  #define HQC192_SHARED_SECRET_BYTES                 	Define the size of the shared secret in bytes
  #define HQC192_CIPHERTEXT_BYTES                    	Define the size of the ciphertext in bytes

  #define HQC192_UTILS_REJECTION_THRESHOLD           	Define the rejection threshold used to generate given weight vectors (see vector_set_random_fixed_weight function)
  #define HQC192_VEC_N_SIZE_BYTES                    	Define the size of the array used to store a HQC192_PARAM_N sized vector in bytes
  #define HQC192_VEC_K_SIZE_BYTES                    	Define the size of the array used to store a HQC192_PARAM_K sized vector in bytes
  #define HQC192_VEC_N1Y_SIZE_BYTES                  	Define the size of the array used to store a HQC192_PARAM_N1 sized vector in bytes
  #define HQC192_VEC_N1N2_SIZE_BYTES                 	Define the size of the array used to store a HQC192_PARAM_N1N2 sized vector in bytes

  #define HQC192_VEC_N_SIZE_64                       	Define the size of the array used to store a HQC192_PARAM_N sized vector in 64 bits
  #define HQC192_VEC_K_SIZE_64                       	Define the size of the array used to store a HQC192_PARAM_K sized vector in 64 bits
  #define HQC192_VEC_N1_SIZE_64                      	Define the size of the array used to store a HQC192_PARAM_N1 sized vector in 64 bits
  #define HQC192_VEC_N1N2_SIZE_64                    	Define the size of the array used to store a HQC192_PARAM_N1N2 sized vector in 64 bits

  #define HQC192_VEC_N_256_SIZE_64                   	Define the size of the array of 64 bits elements used to store an array of size HQC192_PARAM_N considered as elements of 256 bits
  #define HQC192_VEC_N1N2_256_SIZE_64                	Define the size of the array of 64 bits elements used to store an array of size HQC192_PARAM_N1N2 considered as elements of 256 bits

  #define HQC192_PARAM_DELTA                         	Define the parameter delta of the scheme (correcting capacity of the Reed-Solomon code)
  #define HQC192_PARAM_M                             	Define a positive integer
  #define HQC192_PARAM_GF_POLY                       	Generator polynomial of galois field GF(2^HQC192_PARAM_M), represented in hexadecimial form
  #define HQC192_PARAM_GF_POLY_WT                      Hamming weight of HQC192_PARAM_GF_POLY
  #define HQC192_PARAM_GF_POLY_M2                      Distance between the primitive polynomial first two set bits
  #define HQC192_PARAM_GF_MUL_ORDER                  	Define the size of the multiplicative group of GF(2^HQC192_PARAM_M),  i.e 2^HQC192_PARAM_M -1
  #define HQC192_PARAM_K                             	Define the size of the information bits of the Reed-Solomon code
  #define HQC192_PARAM_G                             	Define the size of the generator polynomial of Reed-Solomon code
  #define HQC192_PARAM_FFT                           	The additive FFT takes a 2^HQC192_PARAM_FFT polynomial as input
                                              	We use the FFT to compute the roots of sigma, whose degree if HQC192_PARAM_DELTA=24
                                              	The smallest power of 2 greater than 24+1 is 32=2^5
  #define HQC192_RS_POLY_COEFS                       	Coefficients of the generator polynomial of the Reed-Solomon code

  #define HQC192_RED_MASK                            	A mask fot the higher bits of a vector
  #define HQC192_SHAKE256_512_BYTES                    Define the size of SHAKE-256 output in bytes
  #define HQC192_SEED_BYTES                          	Define the size of the seed in bytes
  #define HQC192_SALT_SIZE_BYTES                       Define the size of a salt in bytes
  #define HQC192_SALT_SIZE_64                          Define the size of a salt in 64 bits words
*/

#define HQC192_PARAM_N                             	35851
#define HQC192_PARAM_N1                            	56
#define HQC192_PARAM_N2                            	640
#define HQC192_PARAM_N1N2                          	35840
#define HQC192_PARAM_OMEGA                         	100
#define HQC192_PARAM_OMEGA_E                       	114
#define HQC192_PARAM_OMEGA_R                       	114
#define HQC192_PARAM_SECURITY                      	192
#define HQC192_PARAM_DFR_EXP                       	192

#define HQC192_SECRET_KEY_BYTES                    	HQC192_CRYPTO_SECRETKEYBYTES
#define HQC192_PUBLIC_KEY_BYTES                    	HQC192_CRYPTO_PUBLICKEYBYTES
#define HQC192_SHARED_SECRET_BYTES                 	HQC192_CRYPTO_BYTES
#define HQC192_CIPHERTEXT_BYTES                    	HQC192_CRYPTO_CIPHERTEXTBYTES

#define HQC192_UTILS_REJECTION_THRESHOLD           	16742417
#define HQC192_VEC_N_SIZE_BYTES                    	HQC192_CEIL_DIVIDE(HQC192_PARAM_N, 8)
#define HQC192_VEC_K_SIZE_BYTES                    	HQC192_PARAM_K
#define HQC192_VEC_N1_SIZE_BYTES                   	HQC192_PARAM_N1
#define HQC192_VEC_N1N2_SIZE_BYTES                 	HQC192_CEIL_DIVIDE(HQC192_PARAM_N1N2, 8)

#define HQC192_VEC_N_SIZE_64                         	HQC192_CEIL_DIVIDE(HQC192_PARAM_N, 64)
#define HQC192_VEC_K_SIZE_64                       	HQC192_CEIL_DIVIDE(HQC192_PARAM_K, 8)
#define HQC192_VEC_N1_SIZE_64                      	HQC192_CEIL_DIVIDE(HQC192_PARAM_N1, 8)
#define HQC192_VEC_N1N2_SIZE_64                    	HQC192_CEIL_DIVIDE(HQC192_PARAM_N1N2, 64)

#define HQC192_PARAM_N_MULT                        	36480
#define HQC192_VEC_N_256_SIZE_64                     	(HQC192_CEIL_DIVIDE(HQC192_PARAM_N_MULT, 256) << 2)
#define HQC192_VEC_N1N2_256_SIZE_64                	(HQC192_CEIL_DIVIDE(HQC192_PARAM_N1N2, 256) << 2)

#define HQC192_PARAM_DELTA                         	16
#define HQC192_PARAM_M                             	8
#define HQC192_PARAM_GF_POLY                       	0x11D
#define HQC192_PARAM_GF_POLY_WT                      	5
#define HQC192_PARAM_GF_POLY_M2                      	4
#define HQC192_PARAM_GF_MUL_ORDER                  	255
#define HQC192_PARAM_K                             	24
#define HQC192_PARAM_G                             	33
#define HQC192_PARAM_FFT                           	5
#define HQC192_RS_POLY_COEFS 45,216,239,24,253,104,27,40,107,50,163,210,227,134,224,158,119,13,158,1,238,164,82,43,15,232,246,142,50,189,29,232,1
#define HQC192_SYND_SIZE_256							(HQC192_CEIL_DIVIDE(2*HQC192_PARAM_DELTA, 16))

#define HQC192_RED_MASK                            	HQC192_BITMASK(HQC192_PARAM_N, 64)
#define HQC192_SHAKE256_512_BYTES                    	64
#define HQC192_SEED_BYTES                          	40
#define HQC192_SALT_SIZE_BYTES                       16
#define HQC192_SALT_SIZE_64                          2

#endif
