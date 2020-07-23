#include <stdint.h>
#include <immintrin.h>
#include "params.h"
#include "rounding.h"
#include "consts.h"

/*************************************************
* Name:        power2round
*
* Description: For finite field element a, compute a0, a1 such that
*              a mod Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}.
*              Assumes a to be standard representative.
*
* Arguments:   - uint32_t *a1:      output array of length N with high bits
*              - uint32_t *a0:      output array of length N with low bits Q + a0
*              - const uint32_t *a: input array of length N
*
**************************************************/
void power2round_avx(uint32_t * restrict a1, uint32_t * restrict a0, const uint32_t * restrict a)
{
  unsigned int i;
  __m256i f,f0,f1;
  const __m256i q = _mm256_load_si256((__m256i *)&qdata[_8XQ]);
  const __m256i mask = _mm256_set1_epi32(-(1U << D));
  const __m256i half = _mm256_set1_epi32((1U << (D-1)) - 1);

  for(i = 0; i < N/8; ++i) {
    f = _mm256_load_si256((__m256i *)&a[8*i]);
    f1 = _mm256_add_epi32(f,half);
    f0 = _mm256_and_si256(f1,mask);
    f1 = _mm256_srli_epi32(f1,D);
    f = _mm256_add_epi32(f,q);
    f0 = _mm256_sub_epi32(f,f0);
    _mm256_store_si256((__m256i *)&a1[8*i], f1);
    _mm256_store_si256((__m256i *)&a0[8*i], f0);
  }
}

/*************************************************
* Name:        decompose
*
* Description: For finite field element a, compute high and low parts a0, a1 such
*              that a mod Q = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except
*              if a1 = (Q-1)/ALPHA where we set a1 = 0 and
*              -ALPHA/2 <= a0 = a mod Q - Q < 0. Assumes a to be standard
*              representative.
*
* Arguments:   - uint32_t *a1:      output array of length N with high parts
*              - uint32_t *a0:      output array of length N with low parts Q + a0
*              - const uint32_t *a: input array of length N
*
**************************************************/
void decompose_avx(uint32_t * restrict a1, uint32_t * restrict a0, const uint32_t * restrict a)
{
  unsigned int i;
  __m256i f,f0,f1,t0,t1;
  const __m256i q = _mm256_load_si256((__m256i *)&qdata[_8XQ]);
  const __m256i half = _mm256_set1_epi32(1U << 18);
  const __m256i mask1 = _mm256_set1_epi32(-(1U << 19));
  const __m256i mask2 = _mm256_set1_epi32(15);

  for(i=0;i<N/8;i++) {
    f = _mm256_load_si256((__m256i *)&a[8*i]);
    t0 = _mm256_srli_epi32(f,10);
    t1 = _mm256_srli_epi32(f,20);
    t0 = _mm256_add_epi32(t0,f);
    t1 = _mm256_add_epi32(t1,half);
    t0 = _mm256_add_epi32(t0,t1);
    f1 = _mm256_srli_epi32(t0,19);

    t0 = _mm256_and_si256(t0,mask1);
    t1 = _mm256_slli_epi32(f1,9);
    f0 = _mm256_add_epi32(f,q);
    f0 = _mm256_sub_epi32(f0,t0);
    f0 = _mm256_add_epi32(f0,t1);

    t0 = _mm256_srli_epi32(f1,4);
    f1 = _mm256_and_si256(f1,mask2);
    f0 = _mm256_sub_epi32(f0,t0);
    _mm256_store_si256((__m256i *)&a1[8*i],f1);
    _mm256_store_si256((__m256i *)&a0[8*i],f0);
  }
}

/*************************************************
* Name:        make_hint
*
* Description: Compute hint bits indicating whether the low bits of the
*              input element overflow into the high bits. Inputs assumed to be
*              standard representatives.
*
* Arguments:   - uint32_t *a0: low bits of input elements
*              - uint32_t *a1: high bits of input elements
*
* Returns number of overflowing low bits
**************************************************/
unsigned int make_hint_avx(uint32_t * restrict h, const uint32_t * restrict a0, const uint32_t * restrict a1)
{
  unsigned int i, r = 0;
  __m256i f0, f1, g0, g1;
  const __m256i blo = _mm256_set1_epi32(GAMMA2 + 1);
  const __m256i bhi = _mm256_set1_epi32(Q - GAMMA2);
  const __m256i zeros = _mm256_setzero_si256();
  const __m256i ones = _mm256_set1_epi32(1);

  for(i = 0; i < N/8; ++i) {
    f0 = _mm256_load_si256((__m256i *)&a0[8*i]);
    f1 = _mm256_load_si256((__m256i *)&a1[8*i]);

    g0 = _mm256_cmpgt_epi32(blo, f0);
    g1 = _mm256_cmpgt_epi32(f0, bhi);
    g0 = _mm256_or_si256(g0, g1);
    g1 = _mm256_cmpeq_epi32(f0, bhi);
    f1 = _mm256_cmpeq_epi32(f1, zeros);
    g1 = _mm256_and_si256(g1, f1);
    g0 = _mm256_or_si256(g0, g1);

    r += _mm_popcnt_u32(_mm256_movemask_ps((__m256)g0));
    g0 = _mm256_add_epi32(g0, ones);
    _mm256_store_si256((__m256i *)&h[8*i], g0);
  }

  return N - r;
}

/*************************************************
* Name:        use_hint
*
* Description: Correct high parts according to hint.
*
* Arguments:   - uint32_t *b:       output array of length N with corrected high parts
*              - uint32_t *a:       input array of length N
*              - const uint32_t *a: input array of length N with hint bits
*
**************************************************/
void use_hint_avx(uint32_t * restrict b, const uint32_t * restrict a, const uint32_t * restrict hint) {
  unsigned int i;
  __attribute__((aligned(32)))
  uint32_t a0[N];
  __m256i f,g,h;
  const __m256i q = _mm256_load_si256((__m256i *)&qdata[_8XQ]);;
  const __m256i mask = _mm256_set1_epi32(15);

  decompose_avx(b, a0, a);
  for(i=0;i<N/8;i++) {
    f = _mm256_load_si256((__m256i *)&a0[8*i]);
    g = _mm256_load_si256((__m256i *)&b[8*i]);
    h = _mm256_load_si256((__m256i *)&hint[8*i]);
    f = _mm256_sub_epi32(f,q);
    h = _mm256_sign_epi32(h,f);
    g = _mm256_add_epi32(g,h);
    g = _mm256_and_si256(g,mask);
    _mm256_store_si256((__m256i *)&b[8*i],g);
  }
}
