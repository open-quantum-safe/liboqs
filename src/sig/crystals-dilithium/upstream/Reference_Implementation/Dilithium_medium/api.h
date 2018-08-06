#ifndef API_H
#define API_H

#define CRYPTO_PUBLICKEYBYTES 1184U
#define CRYPTO_SECRETKEYBYTES 2800U
#define CRYPTO_BYTES 2044U

#define CRYPTO_ALGNAME "Dilithium_medium"

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *msg, unsigned long long len, 
                const unsigned char *sk);

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk);

#endif
