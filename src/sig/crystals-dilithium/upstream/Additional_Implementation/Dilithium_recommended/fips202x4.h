#ifndef FIPS202X4_H
#define FIPS202X4_H

#include <immintrin.h>
#include "params.h"

void shake128_absorb4x(__m256i *s,
                       const unsigned char *m0,
                       const unsigned char *m1,
                       const unsigned char *m2,
                       const unsigned char *m3,
                       unsigned long long mlen);

void shake128_squeezeblocks4x(unsigned char *h0,
                              unsigned char *h1,
                              unsigned char *h2,
                              unsigned char *h3,
                              unsigned long nblocks,
                              __m256i *s);

void shake256_absorb4x(__m256i *s,
                       const unsigned char *m0,
                       const unsigned char *m1,
                       const unsigned char *m2,
                       const unsigned char *m3,
                       unsigned long long mlen);

void shake256_squeezeblocks4x(unsigned char *h0,
                              unsigned char *h1,
                              unsigned char *h2,
                              unsigned char *h3,
                              unsigned long nblocks,
                              __m256i *s);

void shake128_4x(unsigned char *h0,
                 unsigned char *h1,
                 unsigned char *h2,
                 unsigned char *h3,
                 unsigned long long hlen, 
                 const unsigned char *m0,
                 const unsigned char *m1,
                 const unsigned char *m2,
                 const unsigned char *m3,
                 unsigned long long mlen);

void shake256_4x(unsigned char *h0,
                 unsigned char *h1,
                 unsigned char *h2,
                 unsigned char *h3,
                 unsigned long long hlen, 
                 const unsigned char *m0,
                 const unsigned char *m1,
                 const unsigned char *m2,
                 const unsigned char *m3,
                 unsigned long long mlen);

#endif
