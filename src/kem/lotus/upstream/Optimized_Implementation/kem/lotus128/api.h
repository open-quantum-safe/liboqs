/**
 * @file api.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief API header for submission of NIST PQC standardization
 */

#ifndef _API_H
#define _API_H

#define CRYPTO_PUBLICKEYBYTES 658944
#define CRYPTO_SECRETKEYBYTES 714240
#define CRYPTO_BYTES 16
#define CRYPTO_CIPHERTEXTBYTES 1160

#define CRYPTO_ALGNAME "LOTUS128KEM"
#define CRYPTO_VERSION "1.0a"

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char*pk);
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
