#ifndef BENES_H
#define BENES_H
/*
  This file is for Benes network related functions
*/

#include "namespace.h"

#define benes CRYPTO_NAMESPACE(benes)
#define load_bits CRYPTO_NAMESPACE(load_bits)

#include "vec128.h"

void load_bits(vec128 bits_int[][32], const unsigned char *bits);
void benes(vec128 *r, vec128 b[][32], int rev);

#endif
