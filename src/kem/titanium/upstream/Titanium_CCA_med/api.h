/* ****************************** *
 * Titanium_CCA_med               *
 * Implemented by Raymond K. ZHAO *
 * ****************************** */

#ifndef api_h
#define api_h

#define CRYPTO_SECRETKEYBYTES 18304
#define CRYPTO_PUBLICKEYBYTES 18272
#define CRYPTO_BYTES 32
#define CRYPTO_CIPHERTEXTBYTES 4544

#define CRYPTO_ALGNAME "Titanium CCA med"

int crypto_kem_keypair(
unsigned char *pk,
unsigned char *sk
);

int crypto_kem_enc(
unsigned char *ct,
unsigned char *ss,
const unsigned char *pk
);

int crypto_kem_dec(
unsigned char *ss,
const unsigned char *ct,
const unsigned char *sk
);

#endif /* api_h */
