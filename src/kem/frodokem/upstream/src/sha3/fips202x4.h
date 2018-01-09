#ifndef FIPS202X4_H
#define FIPS202X4_H

#include <immintrin.h>

void cshake128_simple_absorb4x(__m256i *s, uint16_t cstm0, uint16_t cstm1, uint16_t cstm2, uint16_t cstm3, const unsigned char *in, unsigned long long inlen);

void cshake128_simple_squeezeblocks4x(unsigned char *output0, unsigned char *output1, unsigned char *output2, unsigned char *output3, unsigned long long outlen, __m256i *s);

/* N is assumed to be empty; S is assumed to have at most 2 characters */
void cshake128_simple4x(unsigned char *output0, unsigned char *output1, unsigned char *output2, unsigned char *output3, unsigned long long outlen, 
                        uint16_t cstm0, uint16_t cstm1, uint16_t cstm2, uint16_t cstm3, const unsigned char *in, unsigned long long inlen);

void cshake256_simple_absorb4x(__m256i *s, uint16_t cstm0, uint16_t cstm1, uint16_t cstm2, uint16_t cstm3, const unsigned char *in, unsigned long long inlen);

void cshake256_simple_squeezeblocks4x(unsigned char *output0, unsigned char *output1, unsigned char *output2, unsigned char *output3, unsigned long long outlen, __m256i *s);

/* N is assumed to be empty; S is assumed to have at most 2 characters */
void cshake256_simple4x(unsigned char *output0, unsigned char *output1, unsigned char *output2, unsigned char *output3, unsigned long long outlen, 
                        uint16_t cstm0, uint16_t cstm1, uint16_t cstm2, uint16_t cstm3, const unsigned char *in, unsigned long long inlen);

#endif
