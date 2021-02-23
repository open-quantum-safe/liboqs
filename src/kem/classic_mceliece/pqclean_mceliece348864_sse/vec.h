#ifndef PQCLEAN_MCELIECE348864_SSE_VEC_H
#define PQCLEAN_MCELIECE348864_SSE_VEC_H

#include <stdint.h>

extern void PQCLEAN_MCELIECE348864_SSE_vec_mul_asm(uint64_t *, const uint64_t *, const uint64_t *, int);

void PQCLEAN_MCELIECE348864_SSE_vec_mul(uint64_t *h, const uint64_t *f, const uint64_t *g);
void PQCLEAN_MCELIECE348864_SSE_vec_add(uint64_t *h, const uint64_t *f, const uint64_t *g);

#endif
