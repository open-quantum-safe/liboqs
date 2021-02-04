#include <stdint.h>
#include <immintrin.h>
#include <string.h>
#include "params.h"
#include "rounding.h"
#include "rejsample.h"
#include "consts.h"

#define _mm256_blendv_epi32(a,b,mask) \
  _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(a), \
                                       _mm256_castsi256_ps(b), \
                                       _mm256_castsi256_ps(mask)))

/*************************************************
* Name:        power2round
*
* Description: For finite field elements a, compute a0, a1 such that
*              a mod^+ Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}.
*              Assumes a to be positive standard representative.
*
* Arguments:   - __m256i *a1: output array of length N/8 with high bits
*              - __m256i *a0: output array of length N/8 with low bits a0
*              - const __m256i *a: input array of length N/8
*
**************************************************/
void power2round_avx(__m256i *a1, __m256i *a0, const __m256i *a)
{
  unsigned int i;
  __m256i f,f0,f1;
  const __m256i mask = _mm256_set1_epi32(-(1 << D));
  const __m256i half = _mm256_set1_epi32((1 << (D-1)) - 1);

  for(i = 0; i < N/8; ++i) {
    f = _mm256_load_si256(&a[i]);
    f1 = _mm256_add_epi32(f,half);
    f0 = _mm256_and_si256(f1,mask);
    f1 = _mm256_srli_epi32(f1,D);
    f0 = _mm256_sub_epi32(f,f0);
    _mm256_store_si256(&a1[i],f1);
    _mm256_store_si256(&a0[i],f0);
  }
}

/*************************************************
* Name:        decompose
*
* Description: For finite field element a, compute high and low parts a0, a1 such
*              that a mod^+ Q = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except
*              if a1 = (Q-1)/ALPHA where we set a1 = 0 and
*              -ALPHA/2 <= a0 = a mod Q - Q < 0. Assumes a to be positive standard
*              representative.
*
* Arguments:   - __m256i *a1: output array of length N/8 with high parts
*              - __m256i *a0: output array of length N/8 with low parts a0
*              - const __m256i *a: input array of length N/8
*
**************************************************/
#if GAMMA2 == (Q-1)/32
void decompose_avx(__m256i *a1, __m256i *a0, const __m256i *a)
{
  unsigned int i;
  __m256i f,f0,f1;
  const __m256i q = _mm256_load_si256(&qdata.vec[_8XQ/8]);
  const __m256i hq = _mm256_srli_epi32(q,1);
  const __m256i v = _mm256_set1_epi32(1025);
  const __m256i alpha = _mm256_set1_epi32(2*GAMMA2);
  const __m256i off = _mm256_set1_epi32(127);
  const __m256i shift = _mm256_set1_epi32(512);
  const __m256i mask = _mm256_set1_epi32(15);

  for(i=0;i<N/8;i++) {
    f = _mm256_load_si256(&a[i]);
    f1 = _mm256_add_epi32(f,off);
    f1 = _mm256_srli_epi32(f1,7);
    f1 = _mm256_mulhi_epu16(f1,v);
    f1 = _mm256_mulhrs_epi16(f1,shift);
    f1 = _mm256_and_si256(f1,mask);
    f0 = _mm256_mullo_epi32(f1,alpha);
    f0 = _mm256_sub_epi32(f,f0);
    f = _mm256_cmpgt_epi32(f0,hq);
    f = _mm256_and_si256(f,q);
    f0 = _mm256_sub_epi32(f0,f);
    _mm256_store_si256(&a1[i],f1);
    _mm256_store_si256(&a0[i],f0);
  }
}

#elif GAMMA2 == (Q-1)/88
void decompose_avx(__m256i *a1, __m256i *a0, const __m256i *a)
{
  unsigned int i;
  __m256i f,f0,f1,t;
  const __m256i q = _mm256_load_si256(&qdata.vec[_8XQ/8]);
  const __m256i hq = _mm256_srli_epi32(q,1);
  const __m256i v = _mm256_set1_epi32(11275);
  const __m256i alpha = _mm256_set1_epi32(2*GAMMA2);
  const __m256i off = _mm256_set1_epi32(127);
  const __m256i shift = _mm256_set1_epi32(128);
  const __m256i max = _mm256_set1_epi32(43);
  const __m256i zero = _mm256_setzero_si256();

  for(i=0;i<N/8;i++) {
    f = _mm256_load_si256(&a[i]);
    f1 = _mm256_add_epi32(f,off);
    f1 = _mm256_srli_epi32(f1,7);
    f1 = _mm256_mulhi_epu16(f1,v);
    f1 = _mm256_mulhrs_epi16(f1,shift);
    t = _mm256_sub_epi32(max,f1);
    f1 = _mm256_blendv_epi32(f1,zero,t);
    f0 = _mm256_mullo_epi32(f1,alpha);
    f0 = _mm256_sub_epi32(f,f0);
    f = _mm256_cmpgt_epi32(f0,hq);
    f = _mm256_and_si256(f,q);
    f0 = _mm256_sub_epi32(f0,f);
    _mm256_store_si256(&a1[i],f1);
    _mm256_store_si256(&a0[i],f0);
  }
}
#endif

/*************************************************
* Name:        make_hint
*
* Description: Compute indices of polynomial coefficients whose low bits
*              overflow into the high bits.
*
* Arguments:   - uint8_t *hint: hint array
*              - const __m256i *a0: low bits of input elements
*              - const __m256i *a1: high bits of input elements
*
* Returns number of overflowing low bits
**************************************************/
unsigned int make_hint_avx(uint8_t hint[N], const __m256i * restrict a0, const __m256i * restrict a1)
{
  unsigned int i, n = 0;
  __m256i f0, f1, g0, g1;
  uint32_t bad;
  uint64_t idx;
  const __m256i low = _mm256_set1_epi32(-GAMMA2);
  const __m256i high = _mm256_set1_epi32(GAMMA2);

  for(i = 0; i < N/8; ++i) {
    f0 = _mm256_load_si256(&a0[i]);
    f1 = _mm256_load_si256(&a1[i]);
    g0 = _mm256_abs_epi32(f0);
    g0 = _mm256_cmpgt_epi32(g0,high);
    g1 = _mm256_cmpeq_epi32(f0,low);
    g1 = _mm256_sign_epi32(g1,f1);
    g0 = _mm256_or_si256(g0,g1);

    bad = _mm256_movemask_ps((__m256)g0);
    memcpy(&idx,idxlut[bad],8);
    idx += (uint64_t)0x0808080808080808*i;
    memcpy(&hint[n],&idx,8);
    n += _mm_popcnt_u32(bad);
  }

  return n;
}

/*************************************************
* Name:        use_hint
*
* Description: Correct high parts according to hint.
*
* Arguments:   - __m256i *b: output array of length N/8 with corrected high parts
*              - const __m256i *a: input array of length N/8
*              - const __m256i *a: input array of length N/8 with hint bits
*
**************************************************/
void use_hint_avx(__m256i *b, const __m256i *a, const __m256i * restrict hint) {
  unsigned int i;
  __m256i a0[N/8];
  __m256i f,g,h,t;
  const __m256i zero = _mm256_setzero_si256();
#if GAMMA2 == (Q-1)/32
  const __m256i mask = _mm256_set1_epi32(15);
#elif GAMMA2 == (Q-1)/88
  const __m256i max = _mm256_set1_epi32(43);
#endif

  decompose_avx(b, a0, a);
  for(i=0;i<N/8;i++) {
    f = _mm256_load_si256(&a0[i]);
    g = _mm256_load_si256(&b[i]);
    h = _mm256_load_si256(&hint[i]);
    t = _mm256_blendv_epi32(zero,h,f);
    t = _mm256_slli_epi32(t,1);
    h = _mm256_sub_epi32(h,t);
    g = _mm256_add_epi32(g,h);
#if GAMMA2 == (Q-1)/32
    g = _mm256_and_si256(g,mask);
#elif GAMMA2 == (Q-1)/88
    g = _mm256_blendv_epi32(g,max,g);
    f = _mm256_cmpgt_epi32(g,max);
    g = _mm256_blendv_epi32(g,zero,f);
#endif
    _mm256_store_si256(&b[i],g);
  }
}
