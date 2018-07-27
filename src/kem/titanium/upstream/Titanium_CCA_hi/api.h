/* ****************************** *
 * Titanium_CCA_hi                *
 * Implemented by Raymond K. ZHAO *
 * ****************************** */

#ifndef api_h
#define api_h

#define CRYPTO_SECRETKEYBYTES 20544
#define CRYPTO_PUBLICKEYBYTES 20512
#define CRYPTO_BYTES 32
#define CRYPTO_CIPHERTEXTBYTES 6048

#define CRYPTO_ALGNAME "Titanium CCA hi"

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
