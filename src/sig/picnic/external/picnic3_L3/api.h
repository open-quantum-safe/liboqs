#ifndef PICNIC3_L3_FS_API_H
#define PICNIC3_L3_FS_API_H

#define CRYPTO_SECRETKEYBYTES (1 + 2 * 24 + 24)
#define CRYPTO_PUBLICKEYBYTES (1 + 2 * 24)
#define CRYPTO_BYTES (4 + 35024)
#define CRYPTO_ALGNAME "picnic3l3"
#define CRYPTO_DETERMINISTIC 1

int crypto_sign_keypair(unsigned char* pk, unsigned char* sk);
int crypto_sign(unsigned char* sm, unsigned long long* smlen, const unsigned char* m,
                unsigned long long mlen, const unsigned char* sk);
int crypto_sign_open(unsigned char* m, unsigned long long* mlen, const unsigned char* sm,
                     unsigned long long smlen, const unsigned char* pk);

#endif
