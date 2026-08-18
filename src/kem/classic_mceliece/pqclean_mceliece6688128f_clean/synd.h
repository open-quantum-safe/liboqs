#ifndef SYND_H
#define SYND_H
/*
  This file is for syndrome computation
*/

#include "namespace.h"

#define synd CRYPTO_NAMESPACE(synd)

#include "gf.h"

void synd(gf *out, gf *f, gf *L, const unsigned char *r);

#endif
