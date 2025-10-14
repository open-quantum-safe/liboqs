#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

extern const int16_t zetas[192];
extern const uint32_t zetas_plant[192];

void ntt(int16_t r[NTRUPLUS_N], const int16_t a[NTRUPLUS_N]);
void invntt(int16_t r[NTRUPLUS_N], const int16_t a[NTRUPLUS_N]);

int  baseinv(int16_t r[8], const int16_t a[8], uint32_t zeta);
void basemul(int16_t r[4], const int16_t a[4], const int16_t b[4], int16_t zeta);
void basemul_add(int16_t r[4], const int16_t a[4], const int16_t b[4], const int16_t c[4], int16_t zeta);

#endif
