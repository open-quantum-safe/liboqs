#include "crypto_verify_1039.h"
#include <immintrin.h>

int PQCLEAN_SNTRUP761_AVX2_crypto_verify_1039(const unsigned char *x, const unsigned char *y) {
    __m256i diff = _mm256_set1_epi8(0);
    unsigned int differentbits = 0;
    int i = PQCLEAN_SNTRUP761_AVX2_crypto_verify_1039_BYTES;

    i -= 32;
    for (;;) {
        do {
            __m256i x0 = _mm256_loadu_si256((__m256i *) x);
            __m256i y0 = _mm256_loadu_si256((__m256i *) y);
            diff |= x0 ^ y0;
            i -= 32;
            x += 32;
            y += 32;
        } while (i >= 0);
        if (i <= -32) {
            break;
        }
        x += i;
        y += i;
    }

    diff |= _mm256_srli_epi16(diff, 8);
    diff |= _mm256_srli_epi32(diff, 16);
    diff |= _mm256_srli_epi64(diff, 32);

    differentbits = (unsigned int) _mm256_extract_epi8(diff, 0);
    differentbits |= (unsigned int) _mm256_extract_epi8(diff, 8);
    differentbits |= (unsigned int) _mm256_extract_epi8(diff, 16);
    differentbits |= (unsigned int) _mm256_extract_epi8(diff, 24);

    return (int) (1 & ((differentbits - 1) >> 8)) - 1;
}
