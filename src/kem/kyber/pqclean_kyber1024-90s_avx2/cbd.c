#include "cbd.h"
#include "params.h"

#include <immintrin.h>
#include <stdint.h>

/*************************************************
* Name:        cbd
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter KYBER_ETA
*
* Arguments:   - poly *r:                  pointer to output polynomial
*              - const uint8_t *buf: pointer to input byte array
**************************************************/
void PQCLEAN_KYBER102490S_AVX2_cbd(poly *r, const uint8_t *buf) {
    __m256i vec0, vec1, vec2, vec3, tmp;
    const __m256i mask55 = _mm256_set1_epi32(0x55555555);
    const __m256i mask33 = _mm256_set1_epi32(0x33333333);
    const __m256i mask03 = _mm256_set1_epi32(0x03030303);

    for (size_t i = 0; i < KYBER_N / 64; i++) {
        vec0 = _mm256_loadu_si256((__m256i *)&buf[32 * i]);

        vec1 = _mm256_srli_epi32(vec0, 1);
        vec0 = _mm256_and_si256(mask55, vec0);
        vec1 = _mm256_and_si256(mask55, vec1);
        vec0 = _mm256_add_epi32(vec0, vec1);

        vec1 = _mm256_srli_epi32(vec0, 2);
        vec0 = _mm256_and_si256(mask33, vec0);
        vec1 = _mm256_and_si256(mask33, vec1);

        vec2 = _mm256_srli_epi32(vec0, 4);
        vec3 = _mm256_srli_epi32(vec1, 4);
        vec0 = _mm256_and_si256(mask03, vec0);
        vec1 = _mm256_and_si256(mask03, vec1);
        vec2 = _mm256_and_si256(mask03, vec2);
        vec3 = _mm256_and_si256(mask03, vec3);

        vec1 = _mm256_sub_epi8(vec0, vec1);
        vec3 = _mm256_sub_epi8(vec2, vec3);

        vec0 = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(vec1));
        vec1 = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(vec1, 1));
        vec2 = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(vec3));
        vec3 = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(vec3, 1));

        tmp = _mm256_unpacklo_epi16(vec0, vec2);
        vec2 = _mm256_unpackhi_epi16(vec0, vec2);
        vec0 = tmp;
        tmp = _mm256_unpacklo_epi16(vec1, vec3);
        vec3 = _mm256_unpackhi_epi16(vec1, vec3);
        vec1 = tmp;

        tmp = _mm256_permute2x128_si256(vec0, vec2, 0x20);
        vec2 = _mm256_permute2x128_si256(vec0, vec2, 0x31);
        vec0 = tmp;
        tmp = _mm256_permute2x128_si256(vec1, vec3, 0x20);
        vec3 = _mm256_permute2x128_si256(vec1, vec3, 0x31);
        vec1 = tmp;

        _mm256_store_si256((__m256i *)&r->coeffs[64 * i + 0], vec0);
        _mm256_store_si256((__m256i *)&r->coeffs[64 * i + 16], vec2);
        _mm256_store_si256((__m256i *)&r->coeffs[64 * i + 32], vec1);
        _mm256_store_si256((__m256i *)&r->coeffs[64 * i + 48], vec3);
    }
}
