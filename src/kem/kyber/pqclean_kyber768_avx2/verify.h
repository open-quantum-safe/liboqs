#ifndef VERIFY_H
#define VERIFY_H

#include <stddef.h>
#include <stdint.h>

uint8_t PQCLEAN_KYBER768_AVX2_verify(const uint8_t *a, const uint8_t *b, size_t len);

void PQCLEAN_KYBER768_AVX2_cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);

#endif
