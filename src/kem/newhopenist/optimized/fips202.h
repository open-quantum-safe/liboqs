#ifndef FIPS202_H
#define FIPS202_H

#include <stdint.h>

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136

void shake128_absorb(uint64_t *s, const unsigned char *input, unsigned long long inputByteLen);
void shake128_squeezeblocks(unsigned char *output, unsigned long long nblocks, uint64_t *s);
void shake256(unsigned char *output, unsigned long long outputByteLen, const unsigned char *input, unsigned long long inputByteLen);

#endif
