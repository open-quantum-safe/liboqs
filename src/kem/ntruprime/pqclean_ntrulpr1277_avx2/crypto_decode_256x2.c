#include "crypto_decode_256x2.h"
#include <immintrin.h>
#include <stdint.h>

#define COPY _mm256_set_epi64x(0x0303030303030303,0x0202020202020202,0x0101010101010101,0x0000000000000000)
#define MASK _mm256_set1_epi64x((int64_t) 0x8040201008040201)
#define MASK2 _mm256_set1_epi64x(0x0101010101010101)

void PQCLEAN_NTRULPR1277_AVX2_crypto_decode_256x2(void *v, const unsigned char *s) {
    __m256i *r = v;
    int i;

    for (i = 0; i < 8; ++i) {
        /* bytes s0 s1 s2 s3 */
        __m256i x = _mm256_set1_epi32(*(int32_t *) s);
        /* s0 s1 s2 s3 s0 s1 s2 s3 s0 s1 s2 s3 s0 s1 s2 s3 s0 s1 s2 s3 s0 s1 s2 s3 s0 s1 s2 s3 s0 s1 s2 s3 */
        x = _mm256_shuffle_epi8(x, COPY);
        /* s0 s0 s0 s0 s0 s0 s0 s0 s1 s1 s1 s1 s1 s1 s1 s1 s2 s2 s2 s2 s2 s2 s2 s2 s3 s3 s3 s3 s3 s3 s3 s3 */
        x = _mm256_andnot_si256(x, MASK);
        x = _mm256_cmpeq_epi8(x, _mm256_setzero_si256());
        x &= MASK2;
        _mm256_storeu_si256(r, x);

        s += 4;
        r += 1;
    }
}
