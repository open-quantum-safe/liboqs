#define CRYPTO_SECRETKEYBYTES (1 + 2 * 24 + 24)
#define CRYPTO_PUBLICKEYBYTES (1 + 2 * 24)
#define CRYPTO_BYTES (4 + 76740)
#define CRYPTO_ALGNAME "picnicl3fs"

int crypto_sign_keypair(unsigned char* pk, unsigned char* sk);
int crypto_sign(unsigned char* sm, unsigned long long* smlen, const unsigned char* m,
                unsigned long long mlen, const unsigned char* sk);
int crypto_sign_open(unsigned char* m, unsigned long long* mlen, const unsigned char* sm,
                     unsigned long long smlen, const unsigned char* pk);
