#ifndef PQCLEAN_MCELIECE8192128F_VEC_UTIL_H
#define PQCLEAN_MCELIECE8192128F_VEC_UTIL_H
/*
  This file is for loading/storing data in a little-endian fashion
*/


#include "vec.h"

#include <stdint.h>

void PQCLEAN_MCELIECE8192128F_VEC_store2(unsigned char *dest, uint16_t a);

uint16_t PQCLEAN_MCELIECE8192128F_VEC_load2(const unsigned char *src);

uint32_t PQCLEAN_MCELIECE8192128F_VEC_load4(const unsigned char *src);

void PQCLEAN_MCELIECE8192128F_VEC_irr_load(vec out[][GFBITS], const unsigned char *in);

void PQCLEAN_MCELIECE8192128F_VEC_store8(unsigned char *out, uint64_t in);

uint64_t PQCLEAN_MCELIECE8192128F_VEC_load8(const unsigned char *in);

#endif

