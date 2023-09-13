#ifndef BM_H
#define BM_H
/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/

#include "namespace.h"

#define bm CRYPTO_NAMESPACE(bm)

#include "vec128.h"
#include "vec256.h"

void bm(vec128 *out, vec256 *in);

#endif
