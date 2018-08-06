#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

extern uint32_t zetas[N];
extern uint32_t zetas_inv[N];

void ntt(uint32_t *phat, const uint32_t *p, const uint32_t *zetas) asm("ntt");
void invntt(uint32_t *p, const uint32_t *phat, const uint32_t *zetas_inv) asm("invntt");
void pointwise_mul(uint32_t *c, const uint32_t *a, const uint32_t *b) asm("pointwise_mul");

#endif
