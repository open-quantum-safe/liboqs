#ifndef PK_GEN_H
#define PK_GEN_H
/*
  This file is for public-key generation
*/

#include "namespace.h"

#define pk_gen CRYPTO_NAMESPACE(pk_gen)

#include "gf.h"

int pk_gen(unsigned char *pk, unsigned char *sk, const uint32_t *perm, int16_t *pi);

#endif
