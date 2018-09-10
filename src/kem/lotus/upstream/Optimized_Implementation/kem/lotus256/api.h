/**
 * @file api.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief API header for submission of NIST PQC standardization
 */

#ifndef _API_H
#define _API_H

#define CRYPTO_PUBLICKEYBYTES 1470976
#define CRYPTO_SECRETKEYBYTES 1630720
#define CRYPTO_BYTES 32
#define CRYPTO_CIPHERTEXTBYTES 1800

#define CRYPTO_ALGNAME "LOTUS256KEM"
#define CRYPTO_VERSION "1.0a"

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char*pk);
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
