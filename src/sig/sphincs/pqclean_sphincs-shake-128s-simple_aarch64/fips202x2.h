#ifndef SPX_FIPS202X2_H
#define SPX_FIPS202X2_H

#include <stdint.h>

uint64_t load64(const unsigned char *x);
void store64(uint8_t *x, uint64_t u);


void shake128x2(unsigned char *out0,
                unsigned char *out1,
                unsigned long long outlen,
                unsigned char *in0,
                unsigned char *in1,
                unsigned long long inlen);

void shake256x2(unsigned char *out0,
                unsigned char *out1,
                unsigned long long outlen,
                unsigned char *in0,
                unsigned char *in1,
                unsigned long long inlen);

#endif
