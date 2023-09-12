#ifndef UTIL_H
#define UTIL_H
/*
  This file is for loading/storing data in a little-endian fashion
*/

#include "namespace.h"

#define bitrev CRYPTO_NAMESPACE(bitrev)
#define load4 CRYPTO_NAMESPACE(load4)
#define load8 CRYPTO_NAMESPACE(load8)
#define load_gf CRYPTO_NAMESPACE(load_gf)
#define store8 CRYPTO_NAMESPACE(store8)
#define store_gf CRYPTO_NAMESPACE(store_gf)

#include "gf.h"
#include <inttypes.h>

void store_gf(unsigned char *dest, gf a);
uint16_t load_gf(const unsigned char *src);

uint32_t load4(const unsigned char *in);

void store8(unsigned char *out, uint64_t in);
uint64_t load8(const unsigned char *in);

gf bitrev(gf a);

#endif
