#include <stdint.h>
#include <immintrin.h>
#include <string.h>
#include "align.h"
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "rounding.h"
#include "rejsample.h"
#include "consts.h"
#include "symmetric.h"
#include "fips202x4.h"

#ifdef DBENCH
#include "test/cpucycles.h"
extern const uint64_t timing_overhead;
extern uint64_t *tred, *tadd, *tmul, *tround, *tsample, *tpack;
#define DBENCH_START() uint64_t time = cpucycles()
#define DBENCH_STOP(t) t += cpucycles() - time - timing_overhead
#else
#define DBENCH_START()
#define DBENCH_STOP(t)
#endif

#define _mm256_blendv_epi32(a,b,mask) \
  _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(a), \
                                       _mm256_castsi256_ps(b), \
                                       _mm256_castsi256_ps(mask)))

/*************************************************
* Name:        poly_reduce
*
* Description: Inplace reduction of all coefficients of polynomial to
*              representative in [-6283009,6283008]. Assumes input
*              coefficients to be at most 2^31 - 2^22 - 1 in absolute value.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *a) {
  unsigned int i;
  __m256i f,g;
  const __m256i q = _mm256_load_si256(&qdata.vec[_8XQ/8]);
  const __m256i off = _mm256_set1_epi32(1<<22);
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_load_si256(&a->vec[i]);
    g = _mm256_add_epi32(f,off);
    g = _mm256_srai_epi32(g,23);
    g = _mm256_mullo_epi32(g,q);
    f = _mm256_sub_epi32(f,g);
    _mm256_store_si256(&a->vec[i],f);
  }

  DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_addq
*
* Description: For all coefficients of in/out polynomial add Q if
*              coefficient is negative.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_caddq(poly *a) {
  unsigned int i;
  __m256i f,g;
  const __m256i q = _mm256_load_si256(&qdata.vec[_8XQ/8]);
  const __m256i zero = _mm256_setzero_si256();
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_load_si256(&a->vec[i]);
    g = _mm256_blendv_epi32(zero,q,f);
    f = _mm256_add_epi32(f,g);
    _mm256_store_si256(&a->vec[i],f);
  }

  DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_add
*
* Description: Add polynomials. No modular reduction is performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first summand
*              - const poly *b: pointer to second summand
**************************************************/
void poly_add(poly *c, const poly *a, const poly *b)  {
  unsigned int i;
  __m256i f,g;
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_load_si256(&a->vec[i]);
    g = _mm256_load_si256(&b->vec[i]);
    f = _mm256_add_epi32(f,g);
    _mm256_store_si256(&c->vec[i],f);
  }

  DBENCH_STOP(*tadd);
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract polynomials. No modular reduction is
*              performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial to be
*                               subtraced from first input polynomial
**************************************************/
void poly_sub(poly *c, const poly *a, const poly *b) {
  unsigned int i;
  __m256i f,g;
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_load_si256(&a->vec[i]);
    g = _mm256_load_si256(&b->vec[i]);
    f = _mm256_sub_epi32(f,g);
    _mm256_store_si256(&c->vec[i],f);
  }

  DBENCH_STOP(*tadd);
}

/*************************************************
* Name:        poly_shiftl
*
* Description: Multiply polynomial by 2^D without modular reduction. Assumes
*              input coefficients to be less than 2^{31-D} in absolute value.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_shiftl(poly *a) {
  unsigned int i;
  __m256i f;
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_load_si256(&a->vec[i]);
    f = _mm256_slli_epi32(f,D);
    _mm256_store_si256(&a->vec[i],f);
  }

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_ntt
*
* Description: Inplace forward NTT. Coefficients can grow by up to
*              8*Q in absolute value.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_ntt(poly *a) {
  DBENCH_START();

  ntt_avx(a->vec, qdata.vec);

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_invntt_tomont
*
* Description: Inplace inverse NTT and multiplication by 2^{32}.
*              Input coefficients need to be less than Q in absolute
*              value and output coefficients are again bounded by Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_invntt_tomont(poly *a) {
  DBENCH_START();

  invntt_avx(a->vec, qdata.vec);

  DBENCH_STOP(*tmul);
}

void poly_nttunpack(poly *a) {
  DBENCH_START();

  nttunpack_avx(a->vec);

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_pointwise_montgomery
*
* Description: Pointwise multiplication of polynomials in NTT domain
*              representation and multiplication of resulting polynomial
*              by 2^{-32}.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_pointwise_montgomery(poly *c, const poly *a, const poly *b) {
  DBENCH_START();

  pointwise_avx(c->vec, a->vec, b->vec, qdata.vec);

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_power2round
*
* Description: For all coefficients c of the input polynomial,
*              compute c0, c1 such that c mod^+ Q = c1*2^D + c0
*              with -2^{D-1} < c0 <= 2^{D-1}. Assumes coefficients to be
*              positive standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients c0
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_power2round(poly *a1, poly *a0, const poly *a)
{
  DBENCH_START();

  power2round_avx(a1->vec, a0->vec, a->vec);

  DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_decompose
*
* Description: For all coefficients c of the input polynomial,
*              compute high and low bits c0, c1 such c mod^+ Q = c1*ALPHA + c0
*              with -ALPHA/2 < c0 <= ALPHA/2 except if c1 = (Q-1)/ALPHA where we
*              set c1 = 0 and -ALPHA/2 <= c0 = c mod Q - Q < 0.
*              Assumes coefficients to be positive standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients c0
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_decompose(poly *a1, poly *a0, const poly *a)
{
  DBENCH_START();

  decompose_avx(a1->vec, a0->vec, a->vec);

  DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_make_hint
*
* Description: Compute hint array. The coefficients of which are the
*              indices of the coefficients of the input polynomial
*              whose low bits overflow into the high bits.
*
* Arguments:   - uint8_t *h: pointer to output hint array (preallocated of length N)
*              - const poly *a0: pointer to low part of input polynomial
*              - const poly *a1: pointer to high part of input polynomial
*
* Returns number of hints, i.e. length of hint array.
**************************************************/
unsigned int poly_make_hint(uint8_t hint[N], const poly *a0, const poly *a1)
{
  unsigned int r;
  DBENCH_START();

  r = make_hint_avx(hint, a0->vec, a1->vec);

  DBENCH_STOP(*tround);
  return r;
}

/*************************************************
* Name:        poly_use_hint
*
* Description: Use hint polynomial to correct the high bits of a polynomial.
*
* Arguments:   - poly *b: pointer to output polynomial with corrected high bits
*              - const poly *a: pointer to input polynomial
*              - const poly *h: pointer to input hint polynomial
**************************************************/
void poly_use_hint(poly *b, const poly *a, const poly *h)
{
  DBENCH_START();

  use_hint_avx(b->vec, a->vec, h->vec);

  DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_chknorm
*
* Description: Check infinity norm of polynomial against given bound.
*              Assumes input polynomial to be reduced by poly_reduce().
*
* Arguments:   - const poly *a: pointer to polynomial
*              - int32_t B: norm bound
*
* Returns 0 if norm is strictly smaller than B <= (Q-1)/8 and 1 otherwise.
**************************************************/
int poly_chknorm(const poly *a, int32_t B) {
  unsigned int i;
  int r;
  __m256i f,t;
  const __m256i bound = _mm256_set1_epi32(B-1);
  DBENCH_START();

  if(B > (Q-1)/8)
    return 1;

  t = _mm256_setzero_si256();
  for(i = 0; i < N/8; i++) {
    f = _mm256_load_si256(&a->vec[i]);
    f = _mm256_abs_epi32(f);
    f = _mm256_cmpgt_epi32(f,bound);
    t = _mm256_or_si256(t,f);
  }

  r = 1 - _mm256_testz_si256(t,t);
  DBENCH_STOP(*tsample);
  return r;
}

/*************************************************
* Name:        rej_uniform
*
* Description: Sample uniformly random coefficients in [0, Q-1] by
*              performing rejection sampling on array of random bytes.
*
* Arguments:   - int32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_uniform(int32_t *a,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint32_t t;
  DBENCH_START();

  ctr = pos = 0;
  while(ctr < len && pos + 3 <= buflen) {
    t  = buf[pos++];
    t |= (uint32_t)buf[pos++] << 8;
    t |= (uint32_t)buf[pos++] << 16;
    t &= 0x7FFFFF;

    if(t < Q)
      a[ctr++] = t;
  }

  DBENCH_STOP(*tsample);
  return ctr;
}

/*************************************************
* Name:        poly_uniform
*
* Description: Sample polynomial with uniformly random coefficients
*              in [0,Q-1] by performing rejection sampling on the
*              output stream of SHAKE256(seed|nonce)
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length SEEDBYTES
*              - uint16_t nonce: 2-byte nonce
**************************************************/
void poly_uniform_preinit(poly *a, stream128_state *state)
{
  unsigned int ctr;
  /* rej_uniform_avx reads up to 8 additional bytes */
  ALIGNED_UINT8(REJ_UNIFORM_BUFLEN+8) buf;

  stream128_squeezeblocks(buf.coeffs, REJ_UNIFORM_NBLOCKS, state);
  ctr = rej_uniform_avx(a->coeffs, buf.coeffs);

  while(ctr < N) {
    /* length of buf is always divisible by 3; hence, no bytes left */
    stream128_squeezeblocks(buf.coeffs, 1, state);
    ctr += rej_uniform(a->coeffs + ctr, N - ctr, buf.coeffs, STREAM128_BLOCKBYTES);
  }
}

void poly_uniform(poly *a, const uint8_t seed[SEEDBYTES], uint16_t nonce)
{
  stream128_state state;
  stream128_init(&state, seed, nonce);
  poly_uniform_preinit(a, &state);
}

void poly_uniform_4x(poly *a0,
                     poly *a1,
                     poly *a2,
                     poly *a3,
                     const uint8_t seed[32],
                     uint16_t nonce0,
                     uint16_t nonce1,
                     uint16_t nonce2,
                     uint16_t nonce3)
{
  unsigned int ctr0, ctr1, ctr2, ctr3;
  ALIGNED_UINT8(REJ_UNIFORM_BUFLEN+8) buf[4];
  keccakx4_state state;
  __m256i f;

  f = _mm256_loadu_si256((__m256i *)seed);
  _mm256_store_si256(buf[0].vec,f);
  _mm256_store_si256(buf[1].vec,f);
  _mm256_store_si256(buf[2].vec,f);
  _mm256_store_si256(buf[3].vec,f);

  buf[0].coeffs[SEEDBYTES+0] = nonce0;
  buf[0].coeffs[SEEDBYTES+1] = nonce0 >> 8;
  buf[1].coeffs[SEEDBYTES+0] = nonce1;
  buf[1].coeffs[SEEDBYTES+1] = nonce1 >> 8;
  buf[2].coeffs[SEEDBYTES+0] = nonce2;
  buf[2].coeffs[SEEDBYTES+1] = nonce2 >> 8;
  buf[3].coeffs[SEEDBYTES+0] = nonce3;
  buf[3].coeffs[SEEDBYTES+1] = nonce3 >> 8;

  shake128x4_absorb_once(&state, buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, SEEDBYTES + 2);
  shake128x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, REJ_UNIFORM_NBLOCKS, &state);

  ctr0 = rej_uniform_avx(a0->coeffs, buf[0].coeffs);
  ctr1 = rej_uniform_avx(a1->coeffs, buf[1].coeffs);
  ctr2 = rej_uniform_avx(a2->coeffs, buf[2].coeffs);
  ctr3 = rej_uniform_avx(a3->coeffs, buf[3].coeffs);

  while(ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
    shake128x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, 1, &state);

    ctr0 += rej_uniform(a0->coeffs + ctr0, N - ctr0, buf[0].coeffs, SHAKE128_RATE);
    ctr1 += rej_uniform(a1->coeffs + ctr1, N - ctr1, buf[1].coeffs, SHAKE128_RATE);
    ctr2 += rej_uniform(a2->coeffs + ctr2, N - ctr2, buf[2].coeffs, SHAKE128_RATE);
    ctr3 += rej_uniform(a3->coeffs + ctr3, N - ctr3, buf[3].coeffs, SHAKE128_RATE);
  }
}

/*************************************************
* Name:        rej_eta
*
* Description: Sample uniformly random coefficients in [-ETA, ETA] by
*              performing rejection sampling on array of random bytes.
*
* Arguments:   - int32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_eta(int32_t *a,
                            unsigned int len,
                            const uint8_t *buf,
                            unsigned int buflen)
{
  unsigned int ctr, pos;
  uint32_t t0, t1;
  DBENCH_START();

  ctr = pos = 0;
  while(ctr < len && pos < buflen) {
    t0 = buf[pos] & 0x0F;
    t1 = buf[pos++] >> 4;

#if ETA == 2
    if(t0 < 15) {
      t0 = t0 - (205*t0 >> 10)*5;
      a[ctr++] = 2 - t0;
    }
    if(t1 < 15 && ctr < len) {
      t1 = t1 - (205*t1 >> 10)*5;
      a[ctr++] = 2 - t1;
    }
#elif ETA == 4
    if(t0 < 9)
      a[ctr++] = 4 - t0;
    if(t1 < 9 && ctr < len)
      a[ctr++] = 4 - t1;
#endif
  }

  DBENCH_STOP(*tsample);
  return ctr;
}

/*************************************************
* Name:        poly_uniform_eta
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-ETA,ETA] by performing rejection sampling using the
*              output stream of SHAKE256(seed|nonce)
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length CRHBYTES
*              - uint16_t nonce: 2-byte nonce
**************************************************/
void poly_uniform_eta_preinit(poly *a, stream256_state *state)
{
  unsigned int ctr;
  ALIGNED_UINT8(REJ_UNIFORM_ETA_BUFLEN) buf;

  stream256_squeezeblocks(buf.coeffs, REJ_UNIFORM_ETA_NBLOCKS, state);
  ctr = rej_eta_avx(a->coeffs, buf.coeffs);

  while(ctr < N) {
    stream256_squeezeblocks(buf.coeffs, 1, state);
    ctr += rej_eta(a->coeffs + ctr, N - ctr, buf.coeffs, STREAM256_BLOCKBYTES);
  }
}

void poly_uniform_eta(poly *a, const uint8_t seed[CRHBYTES], uint16_t nonce)
{
  stream256_state state;
  stream256_init(&state, seed, nonce);
  poly_uniform_eta_preinit(a, &state);
}

void poly_uniform_eta_4x(poly *a0,
                         poly *a1,
                         poly *a2,
                         poly *a3,
                         const uint8_t seed[64],
                         uint16_t nonce0,
                         uint16_t nonce1,
                         uint16_t nonce2,
                         uint16_t nonce3)
{
  unsigned int ctr0, ctr1, ctr2, ctr3;
  ALIGNED_UINT8(REJ_UNIFORM_ETA_BUFLEN) buf[4];

  __m256i f;
  keccakx4_state state;

  f = _mm256_loadu_si256((__m256i *)&seed[0]);
  _mm256_store_si256(&buf[0].vec[0],f);
  _mm256_store_si256(&buf[1].vec[0],f);
  _mm256_store_si256(&buf[2].vec[0],f);
  _mm256_store_si256(&buf[3].vec[0],f);
  f = _mm256_loadu_si256((__m256i *)&seed[32]);
  _mm256_store_si256(&buf[0].vec[1],f);
  _mm256_store_si256(&buf[1].vec[1],f);
  _mm256_store_si256(&buf[2].vec[1],f);
  _mm256_store_si256(&buf[3].vec[1],f);

  buf[0].coeffs[64] = nonce0;
  buf[0].coeffs[65] = nonce0 >> 8;
  buf[1].coeffs[64] = nonce1;
  buf[1].coeffs[65] = nonce1 >> 8;
  buf[2].coeffs[64] = nonce2;
  buf[2].coeffs[65] = nonce2 >> 8;
  buf[3].coeffs[64] = nonce3;
  buf[3].coeffs[65] = nonce3 >> 8;

  shake256x4_absorb_once(&state, buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, 66);
  shake256x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, REJ_UNIFORM_ETA_NBLOCKS, &state);

  ctr0 = rej_eta_avx(a0->coeffs, buf[0].coeffs);
  ctr1 = rej_eta_avx(a1->coeffs, buf[1].coeffs);
  ctr2 = rej_eta_avx(a2->coeffs, buf[2].coeffs);
  ctr3 = rej_eta_avx(a3->coeffs, buf[3].coeffs);

  while(ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
    shake256x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, 1, &state);

    ctr0 += rej_eta(a0->coeffs + ctr0, N - ctr0, buf[0].coeffs, SHAKE256_RATE);
    ctr1 += rej_eta(a1->coeffs + ctr1, N - ctr1, buf[1].coeffs, SHAKE256_RATE);
    ctr2 += rej_eta(a2->coeffs + ctr2, N - ctr2, buf[2].coeffs, SHAKE256_RATE);
    ctr3 += rej_eta(a3->coeffs + ctr3, N - ctr3, buf[3].coeffs, SHAKE256_RATE);
  }
}

/*************************************************
* Name:        poly_uniform_gamma1
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1] by unpacking output stream
*              of SHAKE256(seed|nonce)
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length CRHBYTES
*              - uint16_t nonce: 16-bit nonce
**************************************************/
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((POLYZ_PACKEDBYTES+STREAM256_BLOCKBYTES-1)/STREAM256_BLOCKBYTES)
void poly_uniform_gamma1_preinit(poly *a, stream256_state *state)
{
  /* polyz_unpack reads 14 additional bytes */
  ALIGNED_UINT8(POLY_UNIFORM_GAMMA1_NBLOCKS*STREAM256_BLOCKBYTES+14) buf;
  stream256_squeezeblocks(buf.coeffs, POLY_UNIFORM_GAMMA1_NBLOCKS, state);
  polyz_unpack(a, buf.coeffs);
}

void poly_uniform_gamma1(poly *a, const uint8_t seed[CRHBYTES], uint16_t nonce)
{
  stream256_state state;
  stream256_init(&state, seed, nonce);
  poly_uniform_gamma1_preinit(a, &state);
}

void poly_uniform_gamma1_4x(poly *a0,
                            poly *a1,
                            poly *a2,
                            poly *a3,
                            const uint8_t seed[64],
                            uint16_t nonce0,
                            uint16_t nonce1,
                            uint16_t nonce2,
                            uint16_t nonce3)
{
  ALIGNED_UINT8(POLY_UNIFORM_GAMMA1_NBLOCKS*STREAM256_BLOCKBYTES+14) buf[4];
  keccakx4_state state;
  __m256i f;

  f = _mm256_loadu_si256((__m256i *)&seed[0]);
  _mm256_store_si256(&buf[0].vec[0],f);
  _mm256_store_si256(&buf[1].vec[0],f);
  _mm256_store_si256(&buf[2].vec[0],f);
  _mm256_store_si256(&buf[3].vec[0],f);
  f = _mm256_loadu_si256((__m256i *)&seed[32]);
  _mm256_store_si256(&buf[0].vec[1],f);
  _mm256_store_si256(&buf[1].vec[1],f);
  _mm256_store_si256(&buf[2].vec[1],f);
  _mm256_store_si256(&buf[3].vec[1],f);

  buf[0].coeffs[64] = nonce0;
  buf[0].coeffs[65] = nonce0 >> 8;
  buf[1].coeffs[64] = nonce1;
  buf[1].coeffs[65] = nonce1 >> 8;
  buf[2].coeffs[64] = nonce2;
  buf[2].coeffs[65] = nonce2 >> 8;
  buf[3].coeffs[64] = nonce3;
  buf[3].coeffs[65] = nonce3 >> 8;

  shake256x4_absorb_once(&state, buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, 66);
  shake256x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, POLY_UNIFORM_GAMMA1_NBLOCKS, &state);

  polyz_unpack(a0, buf[0].coeffs);
  polyz_unpack(a1, buf[1].coeffs);
  polyz_unpack(a2, buf[2].coeffs);
  polyz_unpack(a3, buf[3].coeffs);
}

/*************************************************
* Name:        challenge
*
* Description: Implementation of H. Samples polynomial with TAU nonzero
*              coefficients in {-1,1} using the output stream of
*              SHAKE256(seed).
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const uint8_t mu[]: byte array containing seed of length CTILDEBYTES
**************************************************/
void poly_challenge(poly * restrict c, const uint8_t seed[CTILDEBYTES]) {
  unsigned int i, b, pos;
  uint64_t signs;
  ALIGNED_UINT8(SHAKE256_RATE) buf;
  keccak_state state;

  shake256_init(&state);
  shake256_absorb(&state, seed, CTILDEBYTES);
  shake256_finalize(&state);
  shake256_squeezeblocks(buf.coeffs, 1, &state);

  memcpy(&signs, buf.coeffs, 8);
  pos = 8;

  memset(c->vec, 0, sizeof(poly));
  for(i = N-TAU; i < N; ++i) {
    do {
      if(pos >= SHAKE256_RATE) {
        shake256_squeezeblocks(buf.coeffs, 1, &state);
        pos = 0;
      }

      b = buf.coeffs[pos++];
    } while(b > i);

    c->coeffs[i] = c->coeffs[b];
    c->coeffs[b] = 1 - 2*(signs & 1);
    signs >>= 1;
  }
}

/*************************************************
* Name:        polyeta_pack
*
* Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYETA_PACKEDBYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyeta_pack(uint8_t r[POLYETA_PACKEDBYTES], const poly * restrict a) {
  unsigned int i;
  uint8_t t[8];
  DBENCH_START();

#if ETA == 2
  for(i = 0; i < N/8; ++i) {
    t[0] = ETA - a->coeffs[8*i+0];
    t[1] = ETA - a->coeffs[8*i+1];
    t[2] = ETA - a->coeffs[8*i+2];
    t[3] = ETA - a->coeffs[8*i+3];
    t[4] = ETA - a->coeffs[8*i+4];
    t[5] = ETA - a->coeffs[8*i+5];
    t[6] = ETA - a->coeffs[8*i+6];
    t[7] = ETA - a->coeffs[8*i+7];

    r[3*i+0]  = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
    r[3*i+1]  = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[3*i+2]  = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
  }
#elif ETA == 4
  for(i = 0; i < N/2; ++i) {
    t[0] = ETA - a->coeffs[2*i+0];
    t[1] = ETA - a->coeffs[2*i+1];
    r[i] = t[0] | (t[1] << 4);
  }
#endif

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyeta_unpack
*
* Description: Unpack polynomial with coefficients in [-ETA,ETA].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyeta_unpack(poly * restrict r, const uint8_t a[POLYETA_PACKEDBYTES]) {
  unsigned int i;
  DBENCH_START();

#if ETA == 2
  for(i = 0; i < N/8; ++i) {
    r->coeffs[8*i+0] =  (a[3*i+0] >> 0) & 7;
    r->coeffs[8*i+1] =  (a[3*i+0] >> 3) & 7;
    r->coeffs[8*i+2] = ((a[3*i+0] >> 6) | (a[3*i+1] << 2)) & 7;
    r->coeffs[8*i+3] =  (a[3*i+1] >> 1) & 7;
    r->coeffs[8*i+4] =  (a[3*i+1] >> 4) & 7;
    r->coeffs[8*i+5] = ((a[3*i+1] >> 7) | (a[3*i+2] << 1)) & 7;
    r->coeffs[8*i+6] =  (a[3*i+2] >> 2) & 7;
    r->coeffs[8*i+7] =  (a[3*i+2] >> 5) & 7;

    r->coeffs[8*i+0] = ETA - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = ETA - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = ETA - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = ETA - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = ETA - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = ETA - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = ETA - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = ETA - r->coeffs[8*i+7];
  }
#elif ETA == 4
  for(i = 0; i < N/2; ++i) {
    r->coeffs[2*i+0] = a[i] & 0x0F;
    r->coeffs[2*i+1] = a[i] >> 4;
    r->coeffs[2*i+0] = ETA - r->coeffs[2*i+0];
    r->coeffs[2*i+1] = ETA - r->coeffs[2*i+1];
  }
#endif

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt1_pack
*
* Description: Bit-pack polynomial t1 with coefficients fitting in 10 bits.
*              Input coefficients are assumed to be positive standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYT1_PACKEDBYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt1_pack(uint8_t r[POLYT1_PACKEDBYTES], const poly * restrict a) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < N/4; ++i) {
    r[5*i+0] = (a->coeffs[4*i+0] >> 0);
    r[5*i+1] = (a->coeffs[4*i+0] >> 8) | (a->coeffs[4*i+1] << 2);
    r[5*i+2] = (a->coeffs[4*i+1] >> 6) | (a->coeffs[4*i+2] << 4);
    r[5*i+3] = (a->coeffs[4*i+2] >> 4) | (a->coeffs[4*i+3] << 6);
    r[5*i+4] = (a->coeffs[4*i+3] >> 2);
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt1_unpack
*
* Description: Unpack polynomial t1 with 10-bit coefficients.
*              Output coefficients are positive standard representatives.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyt1_unpack(poly * restrict r, const uint8_t a[POLYT1_PACKEDBYTES]) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < N/4; ++i) {
    r->coeffs[4*i+0] = ((a[5*i+0] >> 0) | ((uint32_t)a[5*i+1] << 8)) & 0x3FF;
    r->coeffs[4*i+1] = ((a[5*i+1] >> 2) | ((uint32_t)a[5*i+2] << 6)) & 0x3FF;
    r->coeffs[4*i+2] = ((a[5*i+2] >> 4) | ((uint32_t)a[5*i+3] << 4)) & 0x3FF;
    r->coeffs[4*i+3] = ((a[5*i+3] >> 6) | ((uint32_t)a[5*i+4] << 2)) & 0x3FF;
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt0_pack
*
* Description: Bit-pack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYT0_PACKEDBYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt0_pack(uint8_t r[POLYT0_PACKEDBYTES], const poly * restrict a) {
  unsigned int i;
  uint32_t t[8];
  DBENCH_START();

  for(i = 0; i < N/8; ++i) {
    t[0] = (1 << (D-1)) - a->coeffs[8*i+0];
    t[1] = (1 << (D-1)) - a->coeffs[8*i+1];
    t[2] = (1 << (D-1)) - a->coeffs[8*i+2];
    t[3] = (1 << (D-1)) - a->coeffs[8*i+3];
    t[4] = (1 << (D-1)) - a->coeffs[8*i+4];
    t[5] = (1 << (D-1)) - a->coeffs[8*i+5];
    t[6] = (1 << (D-1)) - a->coeffs[8*i+6];
    t[7] = (1 << (D-1)) - a->coeffs[8*i+7];

    r[13*i+ 0]  =  t[0];
    r[13*i+ 1]  =  t[0] >>  8;
    r[13*i+ 1] |=  t[1] <<  5;
    r[13*i+ 2]  =  t[1] >>  3;
    r[13*i+ 3]  =  t[1] >> 11;
    r[13*i+ 3] |=  t[2] <<  2;
    r[13*i+ 4]  =  t[2] >>  6;
    r[13*i+ 4] |=  t[3] <<  7;
    r[13*i+ 5]  =  t[3] >>  1;
    r[13*i+ 6]  =  t[3] >>  9;
    r[13*i+ 6] |=  t[4] <<  4;
    r[13*i+ 7]  =  t[4] >>  4;
    r[13*i+ 8]  =  t[4] >> 12;
    r[13*i+ 8] |=  t[5] <<  1;
    r[13*i+ 9]  =  t[5] >>  7;
    r[13*i+ 9] |=  t[6] <<  6;
    r[13*i+10]  =  t[6] >>  2;
    r[13*i+11]  =  t[6] >> 10;
    r[13*i+11] |=  t[7] <<  3;
    r[13*i+12]  =  t[7] >>  5;
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt0_unpack
*
* Description: Unpack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyt0_unpack(poly * restrict r, const uint8_t a[POLYT0_PACKEDBYTES]) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < N/8; ++i) {
    r->coeffs[8*i+0]  = a[13*i+0];
    r->coeffs[8*i+0] |= (uint32_t)a[13*i+1] << 8;
    r->coeffs[8*i+0] &= 0x1FFF;

    r->coeffs[8*i+1]  = a[13*i+1] >> 5;
    r->coeffs[8*i+1] |= (uint32_t)a[13*i+2] << 3;
    r->coeffs[8*i+1] |= (uint32_t)a[13*i+3] << 11;
    r->coeffs[8*i+1] &= 0x1FFF;

    r->coeffs[8*i+2]  = a[13*i+3] >> 2;
    r->coeffs[8*i+2] |= (uint32_t)a[13*i+4] << 6;
    r->coeffs[8*i+2] &= 0x1FFF;

    r->coeffs[8*i+3]  = a[13*i+4] >> 7;
    r->coeffs[8*i+3] |= (uint32_t)a[13*i+5] << 1;
    r->coeffs[8*i+3] |= (uint32_t)a[13*i+6] << 9;
    r->coeffs[8*i+3] &= 0x1FFF;

    r->coeffs[8*i+4]  = a[13*i+6] >> 4;
    r->coeffs[8*i+4] |= (uint32_t)a[13*i+7] << 4;
    r->coeffs[8*i+4] |= (uint32_t)a[13*i+8] << 12;
    r->coeffs[8*i+4] &= 0x1FFF;

    r->coeffs[8*i+5]  = a[13*i+8] >> 1;
    r->coeffs[8*i+5] |= (uint32_t)a[13*i+9] << 7;
    r->coeffs[8*i+5] &= 0x1FFF;

    r->coeffs[8*i+6]  = a[13*i+9] >> 6;
    r->coeffs[8*i+6] |= (uint32_t)a[13*i+10] << 2;
    r->coeffs[8*i+6] |= (uint32_t)a[13*i+11] << 10;
    r->coeffs[8*i+6] &= 0x1FFF;

    r->coeffs[8*i+7]  = a[13*i+11] >> 3;
    r->coeffs[8*i+7] |= (uint32_t)a[13*i+12] << 5;
    r->coeffs[8*i+7] &= 0x1FFF;

    r->coeffs[8*i+0] = (1 << (D-1)) - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = (1 << (D-1)) - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = (1 << (D-1)) - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = (1 << (D-1)) - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = (1 << (D-1)) - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = (1 << (D-1)) - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = (1 << (D-1)) - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = (1 << (D-1)) - r->coeffs[8*i+7];
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyz_pack
*
* Description: Bit-pack polynomial with coefficients
*              in [-(GAMMA1 - 1), GAMMA1].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYZ_PACKEDBYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyz_pack(uint8_t r[POLYZ_PACKEDBYTES], const poly * restrict a) {
  unsigned int i;
  uint32_t t[4];
  DBENCH_START();

#if GAMMA1 == (1 << 17)
  for(i = 0; i < N/4; ++i) {
    t[0] = GAMMA1 - a->coeffs[4*i+0];
    t[1] = GAMMA1 - a->coeffs[4*i+1];
    t[2] = GAMMA1 - a->coeffs[4*i+2];
    t[3] = GAMMA1 - a->coeffs[4*i+3];

    r[9*i+0]  = t[0];
    r[9*i+1]  = t[0] >> 8;
    r[9*i+2]  = t[0] >> 16;
    r[9*i+2] |= t[1] << 2;
    r[9*i+3]  = t[1] >> 6;
    r[9*i+4]  = t[1] >> 14;
    r[9*i+4] |= t[2] << 4;
    r[9*i+5]  = t[2] >> 4;
    r[9*i+6]  = t[2] >> 12;
    r[9*i+6] |= t[3] << 6;
    r[9*i+7]  = t[3] >> 2;
    r[9*i+8]  = t[3] >> 10;
  }
#elif GAMMA1 == (1 << 19)
  for(i = 0; i < N/2; ++i) {
    t[0] = GAMMA1 - a->coeffs[2*i+0];
    t[1] = GAMMA1 - a->coeffs[2*i+1];

    r[5*i+0]  = t[0];
    r[5*i+1]  = t[0] >> 8;
    r[5*i+2]  = t[0] >> 16;
    r[5*i+2] |= t[1] << 4;
    r[5*i+3]  = t[1] >> 4;
    r[5*i+4]  = t[1] >> 12;
  }
#endif

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyz_unpack
*
* Description: Unpack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
#if GAMMA1 == (1 << 17)
void polyz_unpack(poly * restrict r, const uint8_t *a) {
  unsigned int i;
  __m256i f;
  const __m256i shufbidx = _mm256_set_epi8(-1, 9, 8, 7,-1, 7, 6, 5,-1, 5, 4, 3,-1, 3, 2, 1,
                                           -1, 8, 7, 6,-1, 6, 5, 4,-1, 4, 3, 2,-1, 2, 1, 0);
  const __m256i srlvdidx = _mm256_set_epi32(6,4,2,0,6,4,2,0);
  const __m256i mask = _mm256_set1_epi32(0x3FFFF);
  const __m256i gamma1 = _mm256_set1_epi32(GAMMA1);
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_loadu_si256((__m256i *)&a[18*i]);
    f = _mm256_permute4x64_epi64(f,0x94);
    f = _mm256_shuffle_epi8(f,shufbidx);
    f = _mm256_srlv_epi32(f,srlvdidx);
    f = _mm256_and_si256(f,mask);
    f = _mm256_sub_epi32(gamma1,f);
    _mm256_store_si256(&r->vec[i],f);
  }

  DBENCH_STOP(*tpack);
}

#elif GAMMA1 == (1 << 19)
void polyz_unpack(poly * restrict r, const uint8_t *a) {
  unsigned int i;
  __m256i f;
  const __m256i shufbidx = _mm256_set_epi8(-1,11,10, 9,-1, 9, 8, 7,-1, 6, 5, 4,-1, 4, 3, 2,
                                           -1, 9, 8, 7,-1, 7, 6, 5,-1, 4, 3, 2,-1, 2, 1, 0);
  const __m256i srlvdidx = _mm256_set1_epi64x((uint64_t)4 << 32);
  const __m256i mask = _mm256_set1_epi32(0xFFFFF);
  const __m256i gamma1 = _mm256_set1_epi32(GAMMA1);
  DBENCH_START();

  for(i = 0; i < N/8; i++) {
    f = _mm256_loadu_si256((__m256i *)&a[20*i]);
    f = _mm256_permute4x64_epi64(f,0x94);
    f = _mm256_shuffle_epi8(f,shufbidx);
    f = _mm256_srlv_epi32(f,srlvdidx);
    f = _mm256_and_si256(f,mask);
    f = _mm256_sub_epi32(gamma1,f);
    _mm256_store_si256(&r->vec[i],f);
  }

  DBENCH_STOP(*tpack);
}
#endif

/*************************************************
* Name:        polyw1_pack
*
* Description: Bit-pack polynomial w1 with coefficients in [0,15] or [0,43].
*              Input coefficients are assumed to be positive standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYW1_PACKEDBYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
#if GAMMA2 == (Q-1)/88
void polyw1_pack(uint8_t *r, const poly * restrict a) {
  unsigned int i;
  __m256i f0,f1,f2,f3;
  const __m256i shift1 = _mm256_set1_epi16((64 << 8) + 1);
  const __m256i shift2 = _mm256_set1_epi32((4096 << 16) + 1);
  const __m256i shufdidx1 = _mm256_set_epi32(7,3,6,2,5,1,4,0);
  const __m256i shufdidx2 = _mm256_set_epi32(-1,-1,6,5,4,2,1,0);
  const __m256i shufbidx = _mm256_set_epi8(-1,-1,-1,-1,14,13,12,10, 9, 8, 6, 5, 4, 2, 1, 0,
                                           -1,-1,-1,-1,14,13,12,10, 9, 8, 6, 5, 4, 2, 1, 0);
  DBENCH_START();

  for(i = 0; i < N/32; i++) {
    f0 = _mm256_load_si256(&a->vec[4*i+0]);
    f1 = _mm256_load_si256(&a->vec[4*i+1]);
    f2 = _mm256_load_si256(&a->vec[4*i+2]);
    f3 = _mm256_load_si256(&a->vec[4*i+3]);
    f0 = _mm256_packus_epi32(f0,f1);
    f1 = _mm256_packus_epi32(f2,f3);
    f0 = _mm256_packus_epi16(f0,f1);
    f0 = _mm256_maddubs_epi16(f0,shift1);
    f0 = _mm256_madd_epi16(f0,shift2);
    f0 = _mm256_permutevar8x32_epi32(f0,shufdidx1);
    f0 = _mm256_shuffle_epi8(f0,shufbidx);
    f0 = _mm256_permutevar8x32_epi32(f0,shufdidx2);
    _mm256_storeu_si256((__m256i *)&r[24*i],f0);
  }

  DBENCH_STOP(*tpack);
}

#elif GAMMA2 == (Q-1)/32
void polyw1_pack(uint8_t *r, const poly * restrict a) {
  unsigned int i;
  __m256i f0, f1, f2, f3, f4, f5, f6, f7;
  const __m256i shift = _mm256_set1_epi16((16 << 8) + 1);
  const __m256i shufbidx = _mm256_set_epi8(15,14, 7, 6,13,12, 5, 4,11,10, 3, 2, 9, 8, 1, 0,
                                           15,14, 7, 6,13,12, 5, 4,11,10, 3, 2, 9, 8, 1, 0);
  DBENCH_START();

  for(i = 0; i < N/64; ++i) {
    f0 = _mm256_load_si256(&a->vec[8*i+0]);
    f1 = _mm256_load_si256(&a->vec[8*i+1]);
    f2 = _mm256_load_si256(&a->vec[8*i+2]);
    f3 = _mm256_load_si256(&a->vec[8*i+3]);
    f4 = _mm256_load_si256(&a->vec[8*i+4]);
    f5 = _mm256_load_si256(&a->vec[8*i+5]);
    f6 = _mm256_load_si256(&a->vec[8*i+6]);
    f7 = _mm256_load_si256(&a->vec[8*i+7]);
    f0 = _mm256_packus_epi32(f0,f1);
    f1 = _mm256_packus_epi32(f2,f3);
    f2 = _mm256_packus_epi32(f4,f5);
    f3 = _mm256_packus_epi32(f6,f7);
    f0 = _mm256_packus_epi16(f0,f1);
    f1 = _mm256_packus_epi16(f2,f3);
    f0 = _mm256_maddubs_epi16(f0,shift);
    f1 = _mm256_maddubs_epi16(f1,shift);
    f0 = _mm256_packus_epi16(f0,f1);
    f0 = _mm256_permute4x64_epi64(f0,0xD8);
    f0 = _mm256_shuffle_epi8(f0,shufbidx);
    _mm256_storeu_si256((__m256i *)&r[32*i], f0);
  }

  DBENCH_STOP(*tpack);
}
#endif
