#ifndef BM_H
#define BM_H
/*
  This file is for the Berlekamp-Massey algorithm
  see http://crypto.stanford.edu/~mironov/cs359/massey.pdf
*/

#include "gf.h"
#include "namespace.h"

#define bm CRYPTO_NAMESPACE(bm)

void bm(gf *out, gf *s);

#endif
