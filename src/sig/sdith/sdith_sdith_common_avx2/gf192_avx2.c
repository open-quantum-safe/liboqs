#include <immintrin.h>

#include "vole_private.h"

#define PCLMUL_LO_X_LO 0
#define PCLMUL_LO_X_HI 16
#define PCLMUL_HI_X_LO 1
#define PCLMUL_HI_X_HI 17

static void gf192_square_pclmul(gf192* res, const gf192* a) {
  const __m128i pa2 = _mm_set_epi64x(a->v64[2], GF192_P);
  __m128i a01 = _mm_loadu_si128((__m128i*)a);
  // multiply a and b
  // we have a = (a2X^128 + a1X^64 + a0)
  // then c = c2X^256 + c1X^128 + c0
  // with c2 = a2a2
  // and  c1 = a1a1
  // and  c0 = a0a0
  __m128i c0, c1, c2, c3, tmp[4];

  c0 = _mm_clmulepi64_si128(a01, a01, PCLMUL_LO_X_LO);  // c0 = a0a0
  c1 = _mm_clmulepi64_si128(a01, a01, PCLMUL_HI_X_HI);  // c1 = a1a1
  c2 = _mm_clmulepi64_si128(pa2, pa2, PCLMUL_HI_X_HI);  // c2 = a2a2

  // condense to a 4-register representation where two hold the low result and two hold the high result
  // currently we have
  // c0c0
  //     c1c1
  //         c2c2
  // and we want c0||c1 to hold the low result and c2||c3 to hold the high result
  tmp[0] = _mm_setzero_si128();
  c3 = _mm_srli_si128(c2, 8);                // upper half of c2 goes to c3 lower half
  c2 = _mm_slli_si128(c2, 8);                // lower half of c2 goes to c2 upper half
  tmp[1] = _mm_srli_si128(c1, 8);            // move upper half of c1 down to tmp1
  c2 = _mm_blend_epi32(c2, tmp[1], 0b0011);  // upper half stays in c2, lower half taken from tmp1

  // c1 still has its upper bits that are moved to c2!

  // reduction:
  // (1) multiply high result (in c2 and c3) by p, return 192 MSBs, then
  // (2) multiply that by p again, return 192 LSBs as remainder
  // (3) add remainder to 192 LSBs of multiplication result

  // Step 1: multiply high result (in c2 and c3) by p, return 192 MSBs
  // (c2L + c2HX^64 + c3LX^128) * (p + X^192) = p*c2L + p*c2HX^64 + p*c3LX^128 + c2LX^192 + c2HX^256 + c3LX^384
  // but we only care about the high result:
  // p*c2L + p*c2HX^64 are completely in the low half
  // the upper half of p*c3LX^128 is the first term of the high result
  // c2LX^192 + c2HX^256 + c3LX^384 is added to that
  tmp[1] = _mm_clmulepi64_si128(c3, pa2, PCLMUL_LO_X_LO);
  tmp[1] = _mm_bsrli_si128(tmp[1], 8);
  c2 = _mm_xor_si128(tmp[1], c2);

  // c2 and c3 now hold the high multiplication result, and now we multiply p again
  // but now we want the LSBs of (c2L + c2HX^64 + c3LX^128) * p
  tmp[0] = _mm_clmulepi64_si128(c3, pa2, PCLMUL_LO_X_LO);  // tmp0 = p * c3  (HIGH)
  tmp[2] = _mm_clmulepi64_si128(c2, pa2, PCLMUL_LO_X_LO);  // tmp1 = p * c2L (LOW)
  tmp[3] = _mm_clmulepi64_si128(c2, pa2, PCLMUL_HI_X_LO);  // tmp2 = p * c2H (MID)
  c1 = _mm_xor_si128(c1, tmp[0]);                          // reduce HIGH
  c0 = _mm_xor_si128(c0, tmp[2]);                          // reduce LOW
  tmp[0] = _mm_bsrli_si128(tmp[3], 8);                     // tmp0 = (p*c2H) >> 64
  tmp[2] = _mm_bslli_si128(tmp[3], 8);                     // tmp2 = (p*c2H) << 64
  c1 = _mm_xor_si128(c1, tmp[0]);                          // reduce MID
  c0 = _mm_xor_si128(c0, tmp[2]);                          // reduce MID

  _mm_storeu_si128((__m128i*)res, c0);
  res->v64[2] = _mm_extract_epi64(c1, 0);
}

EXPORT void gf192_product_pclmul(gf192* res, const gf192* a, const gf192* b) {
  const __m128i p = _mm_set_epi64x(0, GF192_P);
  __m128i a01 = _mm_loadu_si128((__m128i*)a);
  __m128i b01 = _mm_loadu_si128((__m128i*)b);
  __m128i a2b2 = _mm_set_epi64x(a->v64[2], b->v64[2]);
  // multiply a and b
  // we have a = (a2X^128 + a1X^64 + a0) and b analogous
  // then c = c4X^256 + c3X^192 + c2X^128 + c1X^64 + c0
  // with c4 = a2b2
  // and  c3 = a2b1 + a1b2
  // and  c2 = a2b0 + a1b1 + a0b2
  // and  c1 = a1b0 + a0b1
  // and  c0 = a0b0
  __m128i c0, c1, c2, c3, c4, tmp[4];

  c0 = _mm_clmulepi64_si128(a01, b01, PCLMUL_LO_X_LO);       // c0 = a0b0
  c1 = _mm_clmulepi64_si128(a01, b01, PCLMUL_HI_X_LO);       // c1 = a1b0
  c2 = _mm_clmulepi64_si128(a2b2, b01, PCLMUL_HI_X_LO);      // c2 = a2b0
  c3 = _mm_clmulepi64_si128(a2b2, b01, PCLMUL_HI_X_HI);      // c3 = a2b1
  c4 = _mm_clmulepi64_si128(a2b2, a2b2, PCLMUL_HI_X_LO);     // c4 = a2b2
  tmp[0] = _mm_clmulepi64_si128(a01, b01, PCLMUL_LO_X_HI);   // tmp0 = a0b1
  tmp[1] = _mm_clmulepi64_si128(a01, b01, PCLMUL_HI_X_HI);   // tmp1 = a1b1
  tmp[2] = _mm_clmulepi64_si128(a01, a2b2, PCLMUL_LO_X_LO);  // tmp2 = a0b2
  tmp[3] = _mm_clmulepi64_si128(a01, a2b2, PCLMUL_HI_X_LO);  // tmp3 = a1b2

  c1 = _mm_xor_si128(c1, tmp[0]);  // c1 = a1b0 + a0b1
  c2 = _mm_xor_si128(c2, tmp[1]);  // c2 = a2b0 + a1b1
  c3 = _mm_xor_si128(c3, tmp[3]);  // c3 = a2b1 + a1b2
  c2 = _mm_xor_si128(c2, tmp[2]);  // c2 = a2b0 + a1b1 + a0b2

  // condense to a 4-register representation where two hold the low result and two hold the high result
  // currently we have
  // c0c0
  //   c1c1
  //     c2c2
  //       c3c3
  //         c4c4
  // and we want c0||c1 to hold the low result and c2||c3 to hold the high result
  // so we need to do:
  // - c0 += c1L<<64
  // - c1 := c1H + c2L
  // - c2 := c3 + c2H + (c4L<<64)
  // - c3 := c4H
  tmp[0] = _mm_setzero_si128();
  c1 = _mm_shuffle_epi32(c1, 0b01001110);        // c1 := c1 with swapped 64-bit words
  c4 = _mm_shuffle_epi32(c4, 0b01001110);        // c4 := c4 with swapped 64-bit words
  tmp[2] = _mm_shuffle_epi32(c2, 0b01001110);    // tmp2 := c2 with swapped 64-bit words
  tmp[3] = _mm_blend_epi32(c1, tmp[0], 0b0011);  // tmp3 only has the high half of c1, which is now c1L
  tmp[1] = _mm_blend_epi32(tmp[2], c4, 0b1100);  // tmp1 has now c2H + (c4L<<64)
  c1 = _mm_xor_si128(c1, c2);                    // c1 := c1H + c2L (plus nonsense higher 64 bits)
  c0 = _mm_xor_si128(c0, tmp[3]);                // c0 := c0 + (c1L<<64)
  c2 = _mm_xor_si128(c3, tmp[1]);                // c2 := c3 + c2H + (c4L<<64)
  c3 = _mm_blend_epi32(c4, tmp[0], 0b1100);      // c3 := c4H
  c1 = _mm_blend_epi32(c1, tmp[0], 0b1100);      // c1 := c1H + c2L

  // c4 is now free to be used as tmp registers

  // reduction:
  // (1) multiply high result (in c2 and c3) by p, return 192 MSBs, then
  // (2) multiply that by p again, return 192 LSBs as remainder
  // (3) add remainder to 192 LSBs of multiplication result

  // Step 1: multiply high result (in c2 and c3) by p, return 192 MSBs
  // (c2L + c2HX^64 + c3LX^128) * (p + X^192) = p*c2L + p*c2HX^64 + p*c3LX^128 + c2LX^192 + c2HX^256 + c3LX^384
  // but we only care about the high result:
  // p*c2L + p*c2HX^64 are completely in the low half
  // the upper half of p*c3LX^128 is the first term of the high result
  // c2LX^192 + c2HX^256 + c3LX^384 is added to that
  tmp[1] = _mm_clmulepi64_si128(c3, p, PCLMUL_LO_X_LO);
  tmp[1] = _mm_bsrli_si128(tmp[1], 8);
  c2 = _mm_xor_si128(tmp[1], c2);

  // c2 and c3 now hold the high multiplication result, and now we multiply p again
  // but now we want the LSBs of (c2L + c2HX^64 + c3LX^128) * p
  tmp[0] = _mm_clmulepi64_si128(c3, p, PCLMUL_LO_X_LO);  // tmp0 = p * c3  (HIGH)
  tmp[2] = _mm_clmulepi64_si128(c2, p, PCLMUL_LO_X_LO);  // tmp1 = p * c2L (LOW)
  tmp[3] = _mm_clmulepi64_si128(c2, p, PCLMUL_HI_X_LO);  // tmp2 = p * c2H (MID)
  c1 = _mm_xor_si128(c1, tmp[0]);                        // reduce HIGH
  c0 = _mm_xor_si128(c0, tmp[2]);                        // reduce LOW
  tmp[0] = _mm_bsrli_si128(tmp[3], 8);                   // tmp0 = (p*c2H) >> 64
  tmp[2] = _mm_bslli_si128(tmp[3], 8);                   // tmp2 = (p*c2H) << 64
  c1 = _mm_xor_si128(c1, tmp[0]);                        // reduce MID
  c0 = _mm_xor_si128(c0, tmp[2]);                        // reduce MID

  _mm_storeu_si128((__m128i*)res, c0);
  res->v64[2] = _mm_extract_epi64(c1, 0);
}
EXPORT void gf192_flambda_product_pclmul(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf192_product_pclmul((gf192*) res, (const gf192*) a, (const gf192*) b);
}


// res = sum_i x[i . x_byte_slice]*y[i] (accumulated on top of res if accumulate).
// The clmul products are aggregated per Karatsuba term across the whole vector
// (6 accumulators) and a single reduction is done at the end.
static __always_inline void gf192_dot_product_core_pclmul(gf192* res, const uint8_t* x, const uint64_t x_byte_slice,
                                                          const gf192* y, const uint64_t size,
                                                          const int accumulate) {
  const __m128i p = _mm_set_epi64x(0, GF192_P);
  // one accumulator per clmul product (6 = 3*(3+1)/2)
  __m128i accD0 = _mm_setzero_si128();   // x0*y0 (deg 0)
  __m128i accD1 = _mm_setzero_si128();   // x1*y1 (deg 2)
  __m128i accD2 = _mm_setzero_si128();   // x2*y2 (deg 4)
  __m128i accP01 = _mm_setzero_si128();  // (x0+x1)*(y0+y1) (deg 1)
  __m128i accP02 = _mm_setzero_si128();  // (x0+x2)*(y0+y2) (deg 2)
  __m128i accP12 = _mm_setzero_si128();  // (x1+x2)*(y1+y2) (deg 3)
  for (uint64_t i = 0; i < size; ++i) {
    const gf192* xi = (const gf192*)(x + i * x_byte_slice);
    __m128i a01 = _mm_loadu_si128((__m128i*)xi);               // (x0,x1)
    __m128i b01 = _mm_loadu_si128((__m128i*)&y[i]);            // (y0,y1)
    __m128i a2 = _mm_loadl_epi64((__m128i*)&xi->v64[2]);       // (x2,0)
    __m128i b2 = _mm_loadl_epi64((__m128i*)&y[i].v64[2]);      // (y2,0)
    __m128i a1 = _mm_srli_si128(a01, 8);                       // (x1,0)
    __m128i b1 = _mm_srli_si128(b01, 8);                       // (y1,0)
    // pair sums (each in lane 0)
    __m128i sx01 = _mm_xor_si128(a01, a1);   // x0^x1
    __m128i sy01 = _mm_xor_si128(b01, b1);   // y0^y1
    __m128i sx02 = _mm_xor_si128(a01, a2);   // x0^x2
    __m128i sy02 = _mm_xor_si128(b01, b2);   // y0^y2
    __m128i sx12 = _mm_xor_si128(a1, a2);    // x1^x2
    __m128i sy12 = _mm_xor_si128(b1, b2);    // y1^y2
    accD0 = _mm_xor_si128(accD0, _mm_clmulepi64_si128(a01, b01, PCLMUL_LO_X_LO));
    accD1 = _mm_xor_si128(accD1, _mm_clmulepi64_si128(a01, b01, PCLMUL_HI_X_HI));
    accD2 = _mm_xor_si128(accD2, _mm_clmulepi64_si128(a2, b2, PCLMUL_LO_X_LO));
    accP01 = _mm_xor_si128(accP01, _mm_clmulepi64_si128(sx01, sy01, PCLMUL_LO_X_LO));
    accP02 = _mm_xor_si128(accP02, _mm_clmulepi64_si128(sx02, sy02, PCLMUL_LO_X_LO));
    accP12 = _mm_xor_si128(accP12, _mm_clmulepi64_si128(sx12, sy12, PCLMUL_LO_X_LO));
  }
  // degree coefficients c0..c4 (same layout as gf192_product_pclmul before condensing)
  __m128i c0 = accD0;                                                        // deg 0
  __m128i c1 = _mm_xor_si128(accP01, _mm_xor_si128(accD0, accD1));           // deg 1
  __m128i c2 = _mm_xor_si128(accD1, _mm_xor_si128(accP02, _mm_xor_si128(accD0, accD2)));  // deg 2
  __m128i c3 = _mm_xor_si128(accP12, _mm_xor_si128(accD1, accD2));           // deg 3
  __m128i c4 = accD2;                                                        // deg 4
  __m128i tmp[4];

  // condense to the 4-register (low||high) representation (from gf192_product_pclmul)
  tmp[0] = _mm_setzero_si128();
  c1 = _mm_shuffle_epi32(c1, 0b01001110);        // c1 := c1 with swapped 64-bit words
  c4 = _mm_shuffle_epi32(c4, 0b01001110);        // c4 := c4 with swapped 64-bit words
  tmp[2] = _mm_shuffle_epi32(c2, 0b01001110);    // tmp2 := c2 with swapped 64-bit words
  tmp[3] = _mm_blend_epi32(c1, tmp[0], 0b0011);  // tmp3 only has the high half of c1, which is now c1L
  tmp[1] = _mm_blend_epi32(tmp[2], c4, 0b1100);  // tmp1 has now c2H + (c4L<<64)
  c1 = _mm_xor_si128(c1, c2);                    // c1 := c1H + c2L (plus nonsense higher 64 bits)
  c0 = _mm_xor_si128(c0, tmp[3]);                // c0 := c0 + (c1L<<64)
  c2 = _mm_xor_si128(c3, tmp[1]);                // c2 := c3 + c2H + (c4L<<64)
  c3 = _mm_blend_epi32(c4, tmp[0], 0b1100);      // c3 := c4H
  c1 = _mm_blend_epi32(c1, tmp[0], 0b1100);      // c1 := c1H + c2L

  // single reduction (from gf192_product_pclmul)
  tmp[1] = _mm_clmulepi64_si128(c3, p, PCLMUL_LO_X_LO);
  tmp[1] = _mm_bsrli_si128(tmp[1], 8);
  c2 = _mm_xor_si128(tmp[1], c2);

  tmp[0] = _mm_clmulepi64_si128(c3, p, PCLMUL_LO_X_LO);  // tmp0 = p * c3  (HIGH)
  tmp[2] = _mm_clmulepi64_si128(c2, p, PCLMUL_LO_X_LO);  // tmp1 = p * c2L (LOW)
  tmp[3] = _mm_clmulepi64_si128(c2, p, PCLMUL_HI_X_LO);  // tmp2 = p * c2H (MID)
  c1 = _mm_xor_si128(c1, tmp[0]);                        // reduce HIGH
  c0 = _mm_xor_si128(c0, tmp[2]);                        // reduce LOW
  tmp[0] = _mm_bsrli_si128(tmp[3], 8);                   // tmp0 = (p*c2H) >> 64
  tmp[2] = _mm_bslli_si128(tmp[3], 8);                   // tmp2 = (p*c2H) << 64
  c1 = _mm_xor_si128(c1, tmp[0]);                        // reduce MID
  c0 = _mm_xor_si128(c0, tmp[2]);                        // reduce MID

  if (accumulate) {
    c0 = _mm_xor_si128(c0, _mm_loadu_si128((__m128i*)res));
    c1 = _mm_xor_si128(c1, _mm_loadl_epi64((__m128i*)&res->v64[2]));
  }
  _mm_storeu_si128((__m128i*)res, c0);
  res->v64[2] = _mm_extract_epi64(c1, 0);
}

void gf192_dot_product_pclmul(gf192* res, const gf192* x, const gf192* y, const uint64_t size) {
  gf192_dot_product_core_pclmul(res, (const uint8_t*)x, sizeof(gf192), y, size, 0);
}
EXPORT void gf192_flambda_dot_product_pclmul(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size) {
  gf192_dot_product_pclmul((gf192*) res, (const gf192*) x, (const gf192*) y, size);
}


EXPORT void gf192_dot_product_acc_pclmul(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y,
                                         uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf192) == 0, "byte slice not supported");
  gf192_dot_product_core_pclmul(res, (const uint8_t*)x, x_byte_slice, y, size, 1);
}
EXPORT void gf192_flambda_dot_product_acc_pclmul(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y, uint64_t size) {
  gf192_dot_product_acc_pclmul((gf192*) res, (const gf192*) x, x_byte_slice, (const gf192*) y, size);
}


// res += sum_i x[i . x_byte_slice] * y_f2[i], where every y_f2[i] is 0 or 1 (a bit-select).
// gf192 is 24 bytes: the low two limbs go through a 128-bit lane, the top limb through a 64-bit
// one, so nothing is written past res.
EXPORT void gf192_dot_product_f2_acc_avx2(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y_f2,
                                          uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf192) == 0, "byte slice not supported");
  const uint8_t* xp = (const uint8_t*)x;
  __m128i acc01 = _mm_loadu_si128((__m128i*)res);         // res.v64[0..1]
  __m128i acc2 = _mm_loadl_epi64((__m128i*)&res->v64[2]);  // res.v64[2]
  for (uint64_t i = 0; i < size; ++i) {
    const gf192* xi = (const gf192*)(xp + i * x_byte_slice);
    // y_f2[i] is 0 or 1: broadcasting its negation gives the 0/all-ones bit-select mask
    __m128i mask = _mm_set1_epi64x(-(int64_t)y_f2[i].v64[0]);
    acc01 = _mm_xor_si128(acc01, _mm_and_si128(mask, _mm_loadu_si128((__m128i*)xi)));
    acc2 = _mm_xor_si128(acc2, _mm_and_si128(mask, _mm_loadl_epi64((__m128i*)&xi->v64[2])));
  }
  _mm_storeu_si128((__m128i*)res, acc01);
  res->v64[2] = _mm_extract_epi64(acc2, 0);
}
EXPORT void gf192_flambda_dot_product_f2_acc_avx2(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                  const flambda_t* y_f2, uint64_t size) {
  gf192_dot_product_f2_acc_avx2((gf192*) res, (const gf192*) x, x_byte_slice, (const gf192*) y_f2, size);
}


EXPORT void gf192_sum_pow2_avx2(gf192* res, const gf192* x) { gf192_sum_pow2_ref(res, x); }

void gf192_inverse_pclmul(gf192* res, const gf192* a) {
  gf192 tmp, pow_a[7];  // pow_a[i] = a**(2**(2**(i+1))-1) --> a**3, a**15, a**255, a**65536, ...
  gf192* pow_a_ptr = &pow_a[0];

  gf192_square_pclmul(&tmp, a);
  gf192_product_pclmul(&pow_a[0], &tmp, a);  // pow_a[0] = a^3

  for (size_t i = 2; i < 128; i <<= 1) {
    gf192_square_pclmul(&tmp, pow_a_ptr);
    for (size_t j = 1; j < i; j++) {
      gf192_square_pclmul(&tmp, &tmp);
    }
    gf192_product_pclmul(pow_a_ptr + 1, &tmp, pow_a_ptr);
    pow_a_ptr++;
  }

  pow_a_ptr = &pow_a[4];
  for (size_t i = 32; i > 1; i >>= 1) {
    for (size_t j = 0; j < i; j++) {
      gf192_square_pclmul(&pow_a[6], &pow_a[6]);
    }
    gf192_product_pclmul(&pow_a[6], &pow_a[6], pow_a_ptr);
    pow_a_ptr--;
  }

  // pow_a[6] = a**(2**190-1)

  gf192_square_pclmul(&pow_a[6], &pow_a[6]);
  gf192_product_pclmul(&pow_a[6], &pow_a[6], a);

  // pow_a[6] = a**(2**191-1)

  gf192_square_pclmul(res, &pow_a[6]);  // res = (a**(2**191-1))**2 = a**(2*2**191-2) = a**(2**192-2) = a**-1
}
EXPORT void gf192_flambda_inverse_pclmul(flambda_t* res, const flambda_t* a) {
  gf192_inverse_pclmul((gf192*) res, (const gf192*) a);
}


void gf192_echelon_pow2_avx(uint64_t k, gf192* res, const gf192* x, uint64_t x_size, uint64_t x_byte_slice) {
  CREQUIRE(x_byte_slice % sizeof(gf192) == 0, "byte slice not supported");
  CASSERT(k > 0, "k not supported");
  CASSERT(k * x_size <= 192, "echelon width not supported");
  if (x_size == 0) {
    *res = GF192_ZERO;
    return;
  }
  const uint64_t xbs = x_byte_slice / sizeof(gf192);
  static const gf192 PP = {.v64 = {GF192_PV, 0, 0}};
  gf192 left = x[0];
  gf192 right = GF192_ZERO;
  gf192 tmp;
  for (uint64_t i = 1; i < x_size; ++i) {
    gf192p_lsh(&tmp, x + i * xbs, k * i);
    gf192_sum_ref(&left, &left, &tmp);
    gf192p_rsh(&tmp, x + i * xbs, 192 - k * i);
    gf192_sum_ref(&right, &right, &tmp);
  }
  gf192_product_pclmul(&tmp, &right, &PP);
  gf192_sum_ref(res, &left, &tmp);
}
EXPORT void gf192_flambda_echelon_pow2_avx(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice) {
  gf192_echelon_pow2_avx(k, (gf192*) res, (const gf192*) x, x_size, x_byte_slice);
}

