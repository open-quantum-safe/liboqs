#ifndef BENES_H
#define BENES_H
/*
  This file is for Benes network related functions
*/

#include "namespace.h"

#define benes CRYPTO_NAMESPACE(benes)
#define load_bits CRYPTO_NAMESPACE(load_bits)

#include "gf.h"

void load_bits(uint64_t out[][32], const unsigned char *bits);
void benes(uint64_t *r, uint64_t cond[][32], int rev);

#endif
