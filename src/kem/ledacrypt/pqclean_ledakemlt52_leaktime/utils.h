#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

int PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_verify(const DIGIT *a, const DIGIT *b, size_t len);
void PQCLEAN_LEDAKEMLT52_LEAKTIME_cmov(uint8_t *r, const uint8_t *a, size_t len, int cond);

#endif
