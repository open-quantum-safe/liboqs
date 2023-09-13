#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "namespace.h"

#include "crypto_kem.h"

int crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif
