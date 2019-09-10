#ifndef PICNIC_L1_UR_API_H
#define PICNIC_L1_UR_API_H

#define CRYPTO_SECRETKEYBYTES (1 + 2 * 16 + 16)
#define CRYPTO_PUBLICKEYBYTES (1 + 2 * 16)
#define CRYPTO_BYTES (4 + 53961)
#define CRYPTO_ALGNAME "picnicl1ur"
#define CRYPTO_DETERMINISTIC 1

int crypto_sign_keypair(unsigned char* pk, unsigned char* sk);
int crypto_sign(unsigned char* sm, unsigned long long* smlen, const unsigned char* m,
                unsigned long long mlen, const unsigned char* sk);
int crypto_sign_open(unsigned char* m, unsigned long long* mlen, const unsigned char* sm,
                     unsigned long long smlen, const unsigned char* pk);

#endif
