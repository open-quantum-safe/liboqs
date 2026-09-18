#include <inttypes.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vole_private.h"

const gf256 GF256_ZERO = {.v64 = {0, 0, 0, 0}};
const gf256 GF256_ONE = {.v64 = {1, 0, 0, 0}};

__always_inline uint8_t gf256p_equals(const gf256* const a, const gf256* const b) {
  return memcmp(a->v, b->v, 32) == 0;
}

__always_inline void gf256p_sum(gf256* const res, const gf256* const a, const gf256* const b) {
  gf256_sum_ref(res, a, b);
}

__always_inline uint8_t gf256p_bitof(const gf256* const a, const uint64_t position) {
  CREQUIRE(position >= 0 && position < 256, "bad bit position %" PRIu64, position);
  const uint64_t q8 = position >> 3;
  const uint64_t r8 = position & 7;
  return (a->v[q8] >> r8) & 1;
}

// 64-bit limb shifts (little-endian v64[4]). 
// This function does not need to be CT (same as lsh).
void gf256p_lsh(gf256* const res, const gf256* const a, const uint64_t amount) {
  CREQUIRE(amount < 256, "bad left shift amount %" PRIu64, amount);
  const uint64_t ql = amount >> 6;
  const uint64_t rb = amount & 63;
  uint64_t o[4] = {0, 0, 0, 0};
  if (rb == 0) {
    for (uint64_t i = ql; i < 4; i++) o[i] = a->v64[i - ql];
  } else {
    for (uint64_t i = 4; i-- > ql;) {
      const uint64_t hi = a->v64[i - ql] << rb;
      const uint64_t lo = (i - ql >= 1) ? (a->v64[i - ql - 1] >> (64 - rb)) : 0;
      o[i] = hi | lo;
    }
  }
  res->v64[0] = o[0];
  res->v64[1] = o[1];
  res->v64[2] = o[2];
  res->v64[3] = o[3];
}

void gf256p_rsh(gf256* const res, const gf256* const a, const uint64_t amount) {
  CREQUIRE(amount < 256, "bad right shift amount %" PRIu64, amount);
  const uint64_t ql = amount >> 6;
  const uint64_t rb = amount & 63;
  uint64_t o[4] = {0, 0, 0, 0};
  if (rb == 0) {
    for (uint64_t i = 0; i + ql < 4; i++) o[i] = a->v64[i + ql];
  } else {
    for (uint64_t i = 0; i + ql < 4; i++) {
      const uint64_t lo = a->v64[i + ql] >> rb;
      const uint64_t hi = (i + ql + 1 < 4) ? (a->v64[i + ql + 1] << (64 - rb)) : 0;
      o[i] = lo | hi;
    }
  }
  res->v64[0] = o[0];
  res->v64[1] = o[1];
  res->v64[2] = o[2];
  res->v64[3] = o[3];
}

__always_inline void gf256p_mul(gf256* const res, const gf256* const a, const gf256* const b) {
  gf256_product_ref(res, a, b);
}

uint8_t gf256v_equals(const gf256 a, const gf256 b) { return gf256p_equals(&a, &b); }

gf256 gf256v_sum(const gf256 a, const gf256 b) {
  gf256 res;
  gf256p_sum(&res, &a, &b);
  return res;
}

uint8_t gf256v_bitof(const gf256 a, const uint64_t position) { return gf256p_bitof(&a, position); }

gf256 gf256v_lsh(const gf256 a, const uint64_t amount) {
  gf256 res;
  gf256p_lsh(&res, &a, amount);
  return res;
}

gf256 gf256v_rsh(const gf256 a, const uint64_t amount) {
  gf256 res;
  gf256p_rsh(&res, &a, amount);
  return res;
}

gf256 gf256v_mul(const gf256 a, const gf256 b) {
  gf256 res;
  gf256p_mul(&res, &a, &b);
  return res;
}

EXPORT void gf256_set_ref(gf256* res, const gf256* a) { *res = *a; }
EXPORT void gf256_flambda_set_ref(flambda_t* res, const flambda_t* a) {
  gf256_set_ref((gf256*) res, (const gf256*) a);
}


EXPORT void gf256_sum_ref(gf256* res, const gf256* a, const gf256* b) {
  for (uint64_t i = 0; i < 32; ++i) res->v[i] = a->v[i] ^ b->v[i];
}
EXPORT void gf256_flambda_sum_ref(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf256_sum_ref((gf256*) res, (const gf256*) a, (const gf256*) b);
}


EXPORT void gf256_product_ref(gf256* res, const gf256* a, const gf256* b) {
  // deal with the in-place issue
  if (res == b) {
    gf256 tmp;
    gf256p_mul(&tmp, a, b);
    memcpy(res->v, tmp.v, 32);
    return;
  }
  // Out of place product, constant-time and word-level. The 4x4 carryless limb
  // products (Karatsuba gf_clmul64) are accumulated into a 512-bit result, then
  // reduced modulo the field polynomial x^256 + x^10 + x^5 + x^2 + 1 (0x425)
  // with sparse shift-xors. There is no data-dependent branch or memory access,
  // so the running time is independent of the secret bits of a and b (this runs
  // on prover witness values, where a data-dependent branch would be a timing
  // leak). The result is bit-identical to the schoolbook version, so all KATs
  // are unchanged. Operand limbs are read before res is written, so res may
  // alias a (res == b is handled by the guard above).
  const uint64_t* const av = a->v64;
  const uint64_t* const bv = b->v64;
  uint64_t t[8] = {0}, hi, lo;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) {
      gf_clmul64(av[i], bv[j], &hi, &lo);
      t[i + j] ^= lo;
      t[i + j + 1] ^= hi;
    }
  // fold the high 256 bits (t[4..7]) down: x^(256+n) = x^n * (x^10 + x^5 + x^2 + 1)
  uint64_t r0 = t[0], r1 = t[1], r2 = t[2], r3 = t[3], c;
#define GF256_FOLD(v, L, H)                             \
  do {                                                  \
    (L) = (v) ^ ((v) << 2) ^ ((v) << 5) ^ ((v) << 10); \
    (H) = ((v) >> 62) ^ ((v) >> 59) ^ ((v) >> 54);     \
  } while (0)
  GF256_FOLD(t[4], lo, hi); r0 ^= lo; r1 ^= hi;
  GF256_FOLD(t[5], lo, hi); r1 ^= lo; r2 ^= hi;
  GF256_FOLD(t[6], lo, hi); r2 ^= lo; r3 ^= hi;
  GF256_FOLD(t[7], lo, hi); r3 ^= lo; c = hi;  // c holds coefficients of bits 256..265
  r0 ^= c ^ (c << 2) ^ (c << 5) ^ (c << 10);   // second fold; c <= 10 bits, no further overflow
#undef GF256_FOLD
  res->v64[0] = r0;
  res->v64[1] = r1;
  res->v64[2] = r2;
  res->v64[3] = r3;
}
EXPORT void gf256_flambda_product_ref(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf256_product_ref((gf256*) res, (const gf256*) a, (const gf256*) b);
}


EXPORT void gf256_dot_product_ref(gf256* res, const gf256* x, const gf256* y, const uint64_t size) {
  gf256 acc = GF256_ZERO;
  gf256 tmp;
  for (uint64_t i = 0; i < size; ++i) {
    gf256_product_ref(&tmp, &x[i], &y[i]);
    gf256_sum_ref(&acc, &acc, &tmp);
  }
  *res = acc;
}
EXPORT void gf256_flambda_dot_product_ref(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size) {
  gf256_dot_product_ref((gf256*) res, (const gf256*) x, (const gf256*) y, size);
}


EXPORT void gf256_dot_product_acc_ref(gf256* res, const gf256* x, uint64_t x_byte_slice, const gf256* y,
                                      uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf256) == 0, "byte slice not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf256);
  gf256 acc = *res;
  gf256 tmp;
  for (uint64_t i = 0; i < size; ++i) {
    gf256_product_ref(&tmp, &x[i * xbs], &y[i]);
    gf256_sum_ref(&acc, &acc, &tmp);
  }
  *res = acc;
}
EXPORT void gf256_flambda_dot_product_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                              const flambda_t* y, uint64_t size) {
  gf256_dot_product_acc_ref((gf256*) res, (const gf256*) x, x_byte_slice, (const gf256*) y, size);
}


EXPORT void gf256_dot_product_f2_acc_ref(gf256* res, const gf256* x, uint64_t x_byte_slice, const gf256* y_f2,
                                         uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf256) == 0, "byte slice not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf256);
  gf256 acc = *res;
  for (uint64_t i = 0; i < size; ++i) {
    const uint64_t mask = -y_f2[i].v64[0];
    for (uint64_t k = 0; k < 4; ++k) acc.v64[k] ^= x[i * xbs].v64[k] & mask;
  }
  *res = acc;
}
EXPORT void gf256_flambda_dot_product_f2_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y_f2, uint64_t size) {
  gf256_dot_product_f2_acc_ref((gf256*) res, (const gf256*) x, x_byte_slice, (const gf256*) y_f2, size);
}


EXPORT void gf256_product_f2_ref(gf256* res, const gf256* a, const gf256* b_f2) {
  const uint64_t mask = -b_f2->v64[0];
  for (uint64_t i = 0; i < 4; ++i) res->v64[i] = a->v64[i] & mask;
}
EXPORT void gf256_flambda_product_f2_ref(flambda_t* res, const flambda_t* a, const flambda_t* b_f2) {
  gf256_product_f2_ref((gf256*) res, (const gf256*) a, (const gf256*) b_f2);
}


EXPORT void gf256_inverse_ref(gf256* res, const gf256* a) {
  // this is the euclidian algorithm to compute the extended gcd
  gf256 aa = *a;
  CREQUIRE(!gf256p_equals(&aa, &GF256_ZERO), "inverse of 0");
  gf256 uu = GF256_ONE;
  uint64_t log2a = 255;
  while (gf256p_bitof(&aa, log2a) == 0) --log2a;
  gf256 bb;
  gf256p_lsh(&bb, &aa, 256 - log2a);
  bb.v64[0] ^= GF256_P;
  gf256 vv;
  gf256p_lsh(&vv, &uu, 256 - log2a);
  uint64_t log2b = 255;
  while (log2a != 0) {
    CASSERT(gf256v_equals(gf256v_mul(*a, uu), aa), "bug1");
    CASSERT(gf256v_equals(gf256v_mul(*a, vv), bb), "bug2");
    while (log2b >= log2a) {
      if (gf256p_bitof(&bb, log2b)) {
        gf256 tmp;
        gf256p_lsh(&tmp, &aa, log2b - log2a);
        gf256p_sum(&bb, &bb, &tmp);
        gf256p_lsh(&tmp, &uu, log2b - log2a);
        gf256p_sum(&vv, &vv, &tmp);
      }
      --log2b;
    }
    // swap aa <-> bb, uu <-> vv
    gf256 tmp = aa;
    aa = bb;
    bb = tmp;
    tmp = uu;
    uu = vv;
    vv = tmp;
    uint64_t tmp2 = log2a;
    log2a = log2b;
    log2b = tmp2;
    // update log2a
    CASSERT(!gf256p_equals(&aa, &GF256_ZERO), "bug3");
    while (gf256p_bitof(&aa, log2a) == 0) --log2a;
  }
  CASSERT(gf256p_equals(&aa, &GF256_ONE), "bug4");
  *res = uu;
}
EXPORT void gf256_flambda_inverse_ref(flambda_t* res, const flambda_t* a) {
  gf256_inverse_ref((gf256*) res, (const gf256*) a);
}


EXPORT void gf256_sum_pow2_naive(gf256* res, const gf256* x) {
  gf256 ONE = GF256_ONE;
  gf256 r = GF256_ZERO;
  gf256 tmp;
  for (uint64_t i = 0; i < 256; ++i) {
    gf256p_lsh(&tmp, &ONE, i);
    gf256p_mul(&tmp, &tmp, x + i);
    gf256p_sum(&r, &r, &tmp);
  }
  *res = r;
}

EXPORT void gf256_sum_pow2_ref(gf256* res, const gf256* x) {
  static const gf256 PP = {.v64 = {GF256_PV, 0, 0, 0}};
  gf256 left = x[0];
  gf256 right = GF256_ZERO;
  gf256 tmp;
  for (uint64_t i = 1; i < 256; ++i) {
    gf256p_lsh(&tmp, x + i, i);
    gf256p_sum(&left, &left, &tmp);
    gf256p_rsh(&tmp, x + i, 256 - i);
    gf256p_sum(&right, &right, &tmp);
  }
  gf256p_mul(&tmp, &right, &PP);
  gf256p_sum(res, &left, &tmp);
}
EXPORT void gf256_flambda_sum_pow2_ref(flambda_t* res, const flambda_t* x) {
  gf256_sum_pow2_ref((gf256*) res, (const gf256*) x);
}


void gf256_echelon_pow2_naive(uint64_t k, gf256* res, const gf256* x, uint64_t x_size, uint64_t x_byte_slice) {
  CREQUIRE(x_byte_slice % sizeof(gf256) == 0, "byte slice not supported");
  CREQUIRE(k > 0, "k not supported");
  CREQUIRE(k * x_size <= 256, "echelon width not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf256);
  gf256 ONE = GF256_ONE;
  gf256 r = GF256_ZERO;
  gf256 tmp;
  for (uint64_t i = 0; i < x_size; ++i) {
    gf256p_lsh(&tmp, &ONE, k * i);
    gf256p_mul(&tmp, &tmp, x + i * xbs);
    gf256p_sum(&r, &r, &tmp);
  }
  *res = r;
}

void gf256_echelon_pow2_ref(uint64_t k, gf256* res, const gf256* x, uint64_t x_size, uint64_t x_byte_slice) {
  CREQUIRE(x_byte_slice % sizeof(gf256) == 0, "byte slice not supported");
  CASSERT(k > 0, "k not supported");
  CASSERT(k * x_size <= 256, "echelon width not supported");
  if (x_size == 0) {
    *res = GF256_ZERO;
    return;
  }
  const uint64_t xbs = x_byte_slice / sizeof(gf256);
  static const gf256 PP = {.v64 = {GF256_PV, 0, 0, 0}};
  gf256 left = x[0];
  gf256 right = GF256_ZERO;
  gf256 tmp;
  for (uint64_t i = 1; i < x_size; ++i) {
    gf256p_lsh(&tmp, x + i * xbs, k * i);
    gf256p_sum(&left, &left, &tmp);
    gf256p_rsh(&tmp, x + i * xbs, 256 - k * i);
    gf256p_sum(&right, &right, &tmp);
  }
  gf256p_mul(&tmp, &right, &PP);
  gf256p_sum(res, &left, &tmp);
}
EXPORT void gf256_flambda_echelon_pow2_ref(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice) {
  gf256_echelon_pow2_ref(k, (gf256*) res, (const gf256*) x, x_size, x_byte_slice);
}

