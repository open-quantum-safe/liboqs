/**
 * @file api.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief API header for submission of NIST PQC standardization
 */

#ifndef _API_H
#define _API_H

#define CRYPTO_PUBLICKEYBYTES 1025024
#define CRYPTO_SECRETKEYBYTES 1126400
#define CRYPTO_BYTES 24
#define CRYPTO_CIPHERTEXTBYTES 1480

#define CRYPTO_ALGNAME "LOTUS192KEM"
#define CRYPTO_VERSION "1.0a"

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char*pk);
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
