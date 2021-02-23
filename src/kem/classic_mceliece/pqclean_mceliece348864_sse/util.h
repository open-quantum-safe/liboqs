#ifndef PQCLEAN_MCELIECE348864_SSE_UTIL_H
#define PQCLEAN_MCELIECE348864_SSE_UTIL_H
/*
  This file is for loading/storing data in a little-endian fashion
*/


#include "gf.h"
#include "vec128.h"

#include <stdint.h>

void PQCLEAN_MCELIECE348864_SSE_store_i(unsigned char *out, uint64_t in, int i);
void PQCLEAN_MCELIECE348864_SSE_store2(unsigned char *dest, gf a);

uint16_t PQCLEAN_MCELIECE348864_SSE_load2(const unsigned char *src);

uint32_t PQCLEAN_MCELIECE348864_SSE_load4(const unsigned char *src);

void PQCLEAN_MCELIECE348864_SSE_irr_load(uint64_t *out, const unsigned char *in);

void PQCLEAN_MCELIECE348864_SSE_store8(unsigned char *out, uint64_t in);

uint64_t PQCLEAN_MCELIECE348864_SSE_load8(const unsigned char *in);

gf PQCLEAN_MCELIECE348864_SSE_bitrev(gf a);

vec128 PQCLEAN_MCELIECE348864_SSE_load16(const unsigned char *in);

void PQCLEAN_MCELIECE348864_SSE_store16(unsigned char *out, vec128 in);

#endif

