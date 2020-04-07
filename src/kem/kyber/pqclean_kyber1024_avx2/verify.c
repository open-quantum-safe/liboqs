#include "verify.h"

#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>

/*************************************************
* Name:        verify
*
* Description: Compare two arrays for equality in constant time.
*
* Arguments:   const uint8_t *a: pointer to first byte array
*              const uint8_t *b: pointer to second byte array
*              size_t len:             length of the byte arrays
*
* Returns 0 if the byte arrays are equal, 1 otherwise
**************************************************/
uint8_t PQCLEAN_KYBER1024_AVX2_verify(const uint8_t *a, const uint8_t *b, size_t len) {
    size_t pos;
    uint64_t r;
    __m256i avec, bvec, cvec;

    cvec = _mm256_setzero_si256();
    for (pos = 0; pos + 32 <= len; pos += 32) {
        avec = _mm256_loadu_si256((__m256i *)&a[pos]);
        bvec = _mm256_loadu_si256((__m256i *)&b[pos]);
        avec = _mm256_xor_si256(avec, bvec);
        cvec = _mm256_or_si256(cvec, avec);
    }

    cvec = _mm256_cmpeq_epi8(cvec, _mm256_setzero_si256());
    r = (uint32_t)(_mm256_movemask_epi8(cvec) ^ -1);

    while (pos < len) {
        r |= a[pos] ^ b[pos];
        pos += 1;
    }

    r = (-r) >> 63;
    return (uint8_t)r;
}

/*************************************************
* Name:        cmov
*
* Description: Copy len bytes from x to r if b is 1;
*              don't modify x if b is 0. Requires b to be in {0,1};
*              assumes two's complement representation of negative integers.
*              Runs in constant time.
*
* Arguments:   uint8_t *r:       pointer to output byte array
*              const uint8_t *x: pointer to input byte array
*              size_t len:             Amount of bytes to be copied
*              uint8_t b:        Condition bit; has to be in {0,1}
**************************************************/
void PQCLEAN_KYBER1024_AVX2_cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b) {
    size_t pos;
    __m256i xvec, rvec, bvec;

    b = -b;
    bvec = _mm256_set1_epi8((char)b);

    for (pos = 0; pos + 32 <= len; pos += 32) {
        rvec = _mm256_loadu_si256((__m256i *)&r[pos]);
        xvec = _mm256_loadu_si256((__m256i *)&x[pos]);
        xvec = _mm256_xor_si256(xvec, rvec);
        xvec = _mm256_and_si256(xvec, bvec);
        rvec = _mm256_xor_si256(rvec, xvec);
        _mm256_storeu_si256((__m256i *)&r[pos], rvec);
    }

    while (pos < len) {
        r[pos] ^= b & (x[pos] ^ r[pos]);
        pos += 1;
    }
}
