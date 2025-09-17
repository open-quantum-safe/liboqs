#ifndef VERIFY_H
#define VERIFY_H

#include "params.h"

#include <stddef.h>

void PQCLEAN_NTRUHPS4096821_AVX2_cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b);

#endif
