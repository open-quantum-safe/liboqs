#ifndef PQCLEAN_KYBER512_AVX2_CBD_H
#define PQCLEAN_KYBER512_AVX2_CBD_H
#include "params.h"
#include "poly.h"
#include <immintrin.h>
#include <stdint.h>

void PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(poly *r, const __m256i buf[KYBER_ETA1 * KYBER_N / 128 + 1]);

void PQCLEAN_KYBER512_AVX2_poly_cbd_eta2(poly *r, const __m256i buf[KYBER_ETA2 * KYBER_N / 128]);

#endif
