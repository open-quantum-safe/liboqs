/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: parameters and API for FrodoKEM-976
*********************************************************************************************/

#ifndef _API_Frodo976_H_
#define _API_Frodo976_H_


#define CRYPTO_SECRETKEYBYTES  31272     // sizeof(s) + CRYPTO_PUBLICKEYBYTES + 2*PARAMS_N*PARAMS_NBAR
#define CRYPTO_PUBLICKEYBYTES  15632     // sizeof(seed_A) + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8
#define CRYPTO_BYTES              24
#define CRYPTO_CIPHERTEXTBYTES 15768     // (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + CRYPTO_BYTES  

// Algorithm name
#define CRYPTO_ALGNAME "FrodoKEM-976"           


int crypto_kem_keypair_Frodo976(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc_Frodo976(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int crypto_kem_dec_Frodo976(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif

