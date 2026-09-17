#include <immintrin.h>

#include "vole_private.h"

#define PCLMUL_LO_X_LO 0
#define PCLMUL_LO_X_HI 16
#define PCLMUL_HI_X_LO 1
#define PCLMUL_HI_X_HI 17

static void gf128_square_pclmul(gf128* res, const gf128* a) {
  const __m128i p = _mm_set_epi64x(0, GF128_P);
  __m128i aa = _mm_loadu_si128((__m128i*)a);
  // square a
  __m128i u = _mm_clmulepi64_si128(aa, aa, PCLMUL_LO_X_LO);
  __m128i c = _mm_clmulepi64_si128(aa, aa, PCLMUL_HI_X_HI);
  // multiply upper half by p
  __m128i d = _mm_clmulepi64_si128(c, p, 1);
  u = _mm_xor_si128(u, _mm_clmulepi64_si128(c, p, 0));
  u = _mm_xor_si128(u, _mm_slli_si128(d, 8));
  c = _mm_srli_si128(d, 8);
  // multiply by c by p
  u = _mm_xor_si128(u, _mm_clmulepi64_si128(c, p, 0));
  _mm_storeu_si128((__m128i*)res, u);
}

void gf128_product_pclmul(gf128* res, const gf128* a, const gf128* b) {
  const __m128i p = _mm_set_epi64x(0, GF128_P);
  __m128i aa = _mm_loadu_si128((__m128i*)a);
  __m128i bb = _mm_loadu_si128((__m128i*)b);
  // multiply a and b
  __m128i u = _mm_clmulepi64_si128(aa, bb, 0);
  __m128i d1 = _mm_clmulepi64_si128(aa, bb, 1);
  __m128i d2 = _mm_clmulepi64_si128(aa, bb, 16);
  __m128i c = _mm_clmulepi64_si128(aa, bb, 17);
  __m128i d = _mm_xor_si128(d1, d2);
  u = _mm_xor_si128(u, _mm_slli_si128(d, 8));
  c = _mm_xor_si128(c, _mm_srli_si128(d, 8));
  // multiply by c by p
  d = _mm_clmulepi64_si128(c, p, 1);
  u = _mm_xor_si128(u, _mm_clmulepi64_si128(c, p, 0));
  u = _mm_xor_si128(u, _mm_slli_si128(d, 8));
  c = _mm_srli_si128(d, 8);
  // multiply by c by p
  u = _mm_xor_si128(u, _mm_clmulepi64_si128(c, p, 0));
  _mm_storeu_si128((__m128i*)res, u);
}
EXPORT void gf128_flambda_product_pclmul(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf128_product_pclmul((gf128*) res, (const gf128*) a, (const gf128*) b);
}


void gf128_product_pclmul_f2(gf128* res, const gf128* a, const gf128* b) {
  __m128i aa = _mm_loadu_si128((__m128i*)a);
  //__m128i bb = _mm_loadu_si128((__m128i*)b);

  __m128i u = _mm_set1_epi32(-b->v[0]);
  u = _mm_and_si128(u, aa);

  _mm_storeu_si128((__m128i*)res, u);
}
EXPORT void gf128_flambda_product_pclmul_f2(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf128_product_pclmul_f2((gf128*) res, (const gf128*) a, (const gf128*) b);
}


// res = sum_i x[i . x_byte_slice]*y[i] (accumulated on top of res if accumulate).
// The clmul products are aggregated per Karatsuba term across the whole vector
// (3 accumulators) and a single reduction is done at the end.
static __always_inline void gf128_dot_product_core_pclmul(gf128* res, const uint8_t* x, const uint64_t x_byte_slice,
                                                          const gf128* y, const uint64_t size,
                                                          const int accumulate) {
  const __m128i p = _mm_set_epi64x(0, GF128_P);
  // one accumulator per clmul product (3 = 2*(2+1)/2)
  __m128i accD0 = _mm_setzero_si128();  // sum x0*y0            (degree 0)
  __m128i accD1 = _mm_setzero_si128();  // sum x1*y1            (degree 2)
  __m128i accP = _mm_setzero_si128();   // sum (x0+x1)*(y0+y1)  (degree 1, Karatsuba)
  for (uint64_t i = 0; i < size; ++i) {
    __m128i xx = _mm_loadu_si128((__m128i*)(x + i * x_byte_slice));
    __m128i yy = _mm_loadu_si128((__m128i*)&y[i]);
    __m128i xs = _mm_xor_si128(xx, _mm_srli_si128(xx, 8));  // lane0 = x0^x1
    __m128i ys = _mm_xor_si128(yy, _mm_srli_si128(yy, 8));  // lane0 = y0^y1
    accD0 = _mm_xor_si128(accD0, _mm_clmulepi64_si128(xx, yy, PCLMUL_LO_X_LO));
    accD1 = _mm_xor_si128(accD1, _mm_clmulepi64_si128(xx, yy, PCLMUL_HI_X_HI));
    accP = _mm_xor_si128(accP, _mm_clmulepi64_si128(xs, ys, PCLMUL_LO_X_LO));
  }
  // degree-1 coefficient: (x0y1 + x1y0) = accP + accD0 + accD1
  __m128i d = _mm_xor_si128(accP, _mm_xor_si128(accD0, accD1));
  // condense to the 256-bit representation u (low) || c (high)
  __m128i u = _mm_xor_si128(accD0, _mm_slli_si128(d, 8));
  __m128i c = _mm_xor_si128(accD1, _mm_srli_si128(d, 8));
  // single reduction (same tail as gf128_product_pclmul)
  d = _mm_clmulepi64_si128(c, p, 1);
  u = _mm_xor_si128(u, _mm_clmulepi64_si128(c, p, 0));
  u = _mm_xor_si128(u, _mm_slli_si128(d, 8));
  c = _mm_srli_si128(d, 8);
  u = _mm_xor_si128(u, _mm_clmulepi64_si128(c, p, 0));
  if (accumulate) u = _mm_xor_si128(u, _mm_loadu_si128((__m128i*)res));
  _mm_storeu_si128((__m128i*)res, u);
}

void gf128_dot_product_pclmul(gf128* res, const gf128* x, const gf128* y, const uint64_t size) {
  gf128_dot_product_core_pclmul(res, (const uint8_t*)x, sizeof(gf128), y, size, 0);
}
EXPORT void gf128_flambda_dot_product_pclmul(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size) {
  gf128_dot_product_pclmul((gf128*) res, (const gf128*) x, (const gf128*) y, size);
}


EXPORT void gf128_dot_product_acc_pclmul(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y,
                                         uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf128) == 0, "byte slice not supported");
  gf128_dot_product_core_pclmul(res, (const uint8_t*)x, x_byte_slice, y, size, 1);
}
EXPORT void gf128_flambda_dot_product_acc_pclmul(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y, uint64_t size) {
  gf128_dot_product_acc_pclmul((gf128*) res, (const gf128*) x, x_byte_slice, (const gf128*) y, size);
}


EXPORT void gf128_dot_product_f2_acc_avx2(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y_f2,
                                          uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf128) == 0, "byte slice not supported");
  const uint8_t* xp = (const uint8_t*)x;
  __m128i acc = _mm_loadu_si128((__m128i*)res);
  for (uint64_t i = 0; i < size; ++i) {
    // y_f2[i] is 0 or 1: broadcasting its negation gives the 0/all-ones bit-select mask
    __m128i mask = _mm_set1_epi32(-y_f2[i].v[0]);
    acc = _mm_xor_si128(acc, _mm_and_si128(mask, _mm_loadu_si128((__m128i*)(xp + i * x_byte_slice))));
  }
  _mm_storeu_si128((__m128i*)res, acc);
}
EXPORT void gf128_flambda_dot_product_f2_acc_avx2(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                  const flambda_t* y_f2, uint64_t size) {
  gf128_dot_product_f2_acc_avx2((gf128*) res, (const gf128*) x, x_byte_slice, (const gf128*) y_f2, size);
}


void gf128_sum_avx2(gf128* res, const gf128* a, const gf128* b) {
  __m128i aa = _mm_loadu_si128((__m128i*)a);
  __m128i bb = _mm_loadu_si128((__m128i*)b);
  aa = _mm_xor_si128(aa, bb);
  _mm_storeu_si128((__m128i*)res, aa);
}
EXPORT void gf128_flambda_sum_avx2(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf128_sum_avx2((gf128*) res, (const gf128*) a, (const gf128*) b);
}


void gf128_inverse_pclmul(gf128* res, const gf128* a) {
  gf128 tmp, pow_a[6];  // pow_a[i] = a**(2**(2**(i+1))-1) --> a**3, a**15, a**255, a**65536, ...
  gf128* pow_a_ptr = &pow_a[0];

  gf128_square_pclmul(&tmp, a);
  gf128_product_pclmul(&pow_a[0], &tmp, a);  // pow_a[0] = a^3

  for (size_t i = 2; i < 64; i <<= 1) {
    gf128_square_pclmul(&tmp, pow_a_ptr);
    for (size_t j = 1; j < i; j++) {
      gf128_square_pclmul(&tmp, &tmp);
    }
    gf128_product_pclmul(pow_a_ptr + 1, &tmp, pow_a_ptr);
    pow_a_ptr++;
  }

  pow_a_ptr = &pow_a[4];
  for (size_t i = 32; i > 1; i >>= 1) {
    for (size_t j = 0; j < i; j++) {
      gf128_square_pclmul(&pow_a[5], &pow_a[5]);
    }
    gf128_product_pclmul(&pow_a[5], &pow_a[5], pow_a_ptr);
    pow_a_ptr--;
  }

  // pow_a[5] = a**(2**126-1)

  gf128_square_pclmul(&pow_a[5], &pow_a[5]);
  gf128_product_pclmul(&pow_a[5], &pow_a[5], a);

  // pow_a[5] = a**(2**127-1)

  gf128_square_pclmul(res, &pow_a[5]);  // res = (a**(2**127-1))**2 = a**(2*2**127-2) = a**(2**128-2) = a**-1
}
EXPORT void gf128_flambda_inverse_pclmul(flambda_t* res, const flambda_t* a) {
  gf128_inverse_pclmul((gf128*) res, (const gf128*) a);
}


void gf128_echelon_pow2_avx(uint64_t k, gf128* res, const gf128* x, uint64_t x_size, uint64_t x_byte_slice) {
  CREQUIRE(x_byte_slice % sizeof(gf128) == 0, "byte slice not supported");
  CASSERT(k > 0, "k not supported");
  CASSERT(k * x_size <= 128, "echelon width not supported");
  if (x_size == 0) {
    *res = GF128_ZERO;
    return;
  }
  const uint64_t xbs = x_byte_slice / sizeof(gf128);
  static const gf128 PP = {.v128 = GF128_PV};
  gf128 left = x[0];
  gf128 right = GF128_ZERO;
  gf128 tmp;
  for (uint64_t i = 1; i < x_size; ++i) {
    gf128p_lsh(&tmp, x + i * xbs, k * i);
    gf128_sum_avx2(&left, &left, &tmp);
    gf128p_rsh(&tmp, x + i * xbs, 128 - k * i);
    gf128_sum_avx2(&right, &right, &tmp);
  }
  gf128_product_pclmul(&tmp, &right, &PP);
  gf128_sum_avx2(res, &left, &tmp);
}
EXPORT void gf128_flambda_echelon_pow2_avx(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice) {
  gf128_echelon_pow2_avx(k, (gf128*) res, (const gf128*) x, x_size, x_byte_slice);
}

