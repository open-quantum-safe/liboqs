#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>
#include "params.h"
#include "poly.h"

void sample_y(int64_t *y, const unsigned char *seed, int nonce);
void sample_gauss_poly(int64_t *x, const unsigned char *seed, int nonce);
void encode_c(uint32_t *pos_list, int16_t *sign_list, unsigned char *c_bin);

#endif
