#include "cbd.h"
#include "params.h"
#include <immintrin.h>
#include <stdint.h>

/*************************************************
* Name:        cbd2
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const __m256i *buf: pointer to aligned input byte array
**************************************************/
static void cbd2(poly *restrict r, const __m256i buf[2 * KYBER_N / 128]) {
    unsigned int i;
    __m256i f0, f1, f2, f3;
    const __m256i mask55 = _mm256_set1_epi32(0x55555555);
    const __m256i mask33 = _mm256_set1_epi32(0x33333333);
    const __m256i mask03 = _mm256_set1_epi32(0x03030303);
    const __m256i mask0F = _mm256_set1_epi32(0x0F0F0F0F);

    for (i = 0; i < KYBER_N / 64; i++) {
        f0 = _mm256_load_si256(&buf[i]);

        f1 = _mm256_srli_epi16(f0, 1);
        f0 = _mm256_and_si256(mask55, f0);
        f1 = _mm256_and_si256(mask55, f1);
        f0 = _mm256_add_epi8(f0, f1);

        f1 = _mm256_srli_epi16(f0, 2);
        f0 = _mm256_and_si256(mask33, f0);
        f1 = _mm256_and_si256(mask33, f1);
        f0 = _mm256_add_epi8(f0, mask33);
        f0 = _mm256_sub_epi8(f0, f1);

        f1 = _mm256_srli_epi16(f0, 4);
        f0 = _mm256_and_si256(mask0F, f0);
        f1 = _mm256_and_si256(mask0F, f1);
        f0 = _mm256_sub_epi8(f0, mask03);
        f1 = _mm256_sub_epi8(f1, mask03);

        f2 = _mm256_unpacklo_epi8(f0, f1);
        f3 = _mm256_unpackhi_epi8(f0, f1);

        f0 = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(f2));
        f1 = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(f2, 1));
        f2 = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(f3));
        f3 = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(f3, 1));

        _mm256_store_si256(&r->vec[4 * i + 0], f0);
        _mm256_store_si256(&r->vec[4 * i + 1], f2);
        _mm256_store_si256(&r->vec[4 * i + 2], f1);
        _mm256_store_si256(&r->vec[4 * i + 3], f3);
    }
}


/* buf 32 bytes longer for cbd3 */
void PQCLEAN_KYBER1024_AVX2_poly_cbd_eta1(poly *r, const __m256i buf[KYBER_ETA1 * KYBER_N / 128 + 1]) {
    cbd2(r, buf);
}

void PQCLEAN_KYBER1024_AVX2_poly_cbd_eta2(poly *r, const __m256i buf[KYBER_ETA2 * KYBER_N / 128]) {
    cbd2(r, buf);
}
