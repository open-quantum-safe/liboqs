#define CRYPTO_SECRETKEYBYTES 192
#define CRYPTO_PUBLICKEYBYTES 64
#define CRYPTO_BYTES 32
#define CRYPTO_CIPHERTEXTBYTES 128
#define CRYPTO_ALGNAME "DUMMY"

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
