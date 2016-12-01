#include "external/api.h"

int crypto_encrypt(
    unsigned char *c, size_t *clen,
    const unsigned char *m, unsigned long long mlen,
    const unsigned char *pk,
    OQS_RAND *r
);

int crypto_encrypt_open(
    unsigned char *m, size_t *mlen,
    const unsigned char *c, unsigned long long clen,
    const unsigned char *sk
);

int crypto_encrypt_keypair
(
    unsigned char *pk,
    unsigned char *sk,
    OQS_RAND *r
);
