#include <inttypes.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vole_private.h"

const gf192 GF192_ZERO = {.v64 = {0, 0, 0}};
const gf192 GF192_ONE = {.v64 = {1, 0, 0}};

__always_inline uint8_t gf192p_equals(const gf192* const a, const gf192* const b) {
  return memcmp(a->v, b->v, 24) == 0;
}

__always_inline void gf192p_sum(gf192* const res, const gf192* const a, const gf192* const b) {
  gf192_sum_ref(res, a, b);
}

__always_inline uint8_t gf192p_bitof(const gf192* const a, const uint64_t position) {
  CREQUIRE(position >= 0 && position < 192, "bad bit position %" PRIu64, position);
  const uint64_t q8 = position >> 3;
  const uint64_t r8 = position & 7;
  return (a->v[q8] >> r8) & 1;
}

// 64-bit limb shifts (little-endian v64[3]). 
// This function does not need to be CT: amount is always public in the hot callers (sum_pow2/echelon loop index); 
// It is also used on delta by the ref_inverse 
// which is also public since delta is part of the signature.
void gf192p_lsh(gf192* const res, const gf192* const a, const uint64_t amount) {
  CREQUIRE(amount < 192, "bad left shift amount %" PRIu64, amount);
  const uint64_t ql = amount >> 6;
  const uint64_t rb = amount & 63;
  uint64_t o[3] = {0, 0, 0};
  if (rb == 0) {
    for (uint64_t i = ql; i < 3; i++) o[i] = a->v64[i - ql];
  } else {
    for (uint64_t i = 3; i-- > ql;) {
      const uint64_t hi = a->v64[i - ql] << rb;
      const uint64_t lo = (i - ql >= 1) ? (a->v64[i - ql - 1] >> (64 - rb)) : 0;
      o[i] = hi | lo;
    }
  }
  res->v64[0] = o[0];
  res->v64[1] = o[1];
  res->v64[2] = o[2];
}

void gf192p_rsh(gf192* const res, const gf192* const a, const uint64_t amount) {
  CREQUIRE(amount < 192, "bad right shift amount %" PRIu64, amount);
  const uint64_t ql = amount >> 6;
  const uint64_t rb = amount & 63;
  uint64_t o[3] = {0, 0, 0};
  if (rb == 0) {
    for (uint64_t i = 0; i + ql < 3; i++) o[i] = a->v64[i + ql];
  } else {
    for (uint64_t i = 0; i + ql < 3; i++) {
      const uint64_t lo = a->v64[i + ql] >> rb;
      const uint64_t hi = (i + ql + 1 < 3) ? (a->v64[i + ql + 1] << (64 - rb)) : 0;
      o[i] = lo | hi;
    }
  }
  res->v64[0] = o[0];
  res->v64[1] = o[1];
  res->v64[2] = o[2];
}

__always_inline void gf192p_mul(gf192* const res, const gf192* const a, const gf192* const b) {
  gf192_product_ref(res, a, b);
}

uint8_t gf192v_equals(const gf192 a, const gf192 b) { return gf192p_equals(&a, &b); }

gf192 gf192v_sum(const gf192 a, const gf192 b) {
  gf192 res;
  gf192p_sum(&res, &a, &b);
  return res;
}

uint8_t gf192v_bitof(const gf192 a, const uint64_t position) { return gf192p_bitof(&a, position); }

gf192 gf192v_lsh(const gf192 a, const uint64_t amount) {
  gf192 res;
  gf192p_lsh(&res, &a, amount);
  return res;
}

gf192 gf192v_rsh(const gf192 a, const uint64_t amount) {
  gf192 res;
  gf192p_rsh(&res, &a, amount);
  return res;
}

gf192 gf192v_mul(const gf192 a, const gf192 b) {
  gf192 res;
  gf192p_mul(&res, &a, &b);
  return res;
}

EXPORT void gf192_set_ref(gf192* res, const gf192* a) { *res = *a; }
EXPORT void gf192_flambda_set_ref(flambda_t* res, const flambda_t* a) {
  gf192_set_ref((gf192*) res, (const gf192*) a);
}

EXPORT void gf192_sum_ref(gf192* res, const gf192* a, const gf192* b) {
  for (uint64_t i = 0; i < 3; ++i) res->v64[i] = a->v64[i] ^ b->v64[i];
}
EXPORT void gf192_flambda_sum_ref(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf192_sum_ref((gf192*) res, (const gf192*) a, (const gf192*) b);
}


EXPORT void gf192_product_ref(gf192* res, const gf192* a, const gf192* b) {
  // deal with the in-place issue
  if (res == b) {
    gf192 tmp;
    gf192p_mul(&tmp, a, b);
    memcpy(res->v, tmp.v, 24);
    return;
  }
  // Out of place product, constant-time and word-level. The 3x3 carryless limb
  // products (Karatsuba gf_clmul64) are accumulated into a 384-bit result, then
  // reduced modulo the field polynomial x^192 + x^7 + x^2 + x + 1 (0x87) with
  // sparse shift-xors. There is no data-dependent branch or memory access, so
  // the running time is independent of the secret bits of a and b (this runs on
  // prover witness values, where a data-dependent branch would be a timing
  // leak). The result is bit-identical to the schoolbook version, so all KATs
  // are unchanged. Operand limbs are read before res is written, so res may
  // alias a (res == b is handled by the guard above).
  const uint64_t* const av = a->v64;
  const uint64_t* const bv = b->v64;
  uint64_t t[6] = {0}, hi, lo;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      gf_clmul64(av[i], bv[j], &hi, &lo);
      t[i + j] ^= lo;
      t[i + j + 1] ^= hi;
    }
  // fold the high 192 bits (t[3..5]) down: x^(192+n) = x^n * (x^7 + x^2 + x + 1)
  uint64_t r0 = t[0], r1 = t[1], r2 = t[2], c;
#define GF192_FOLD(v, L, H)                            \
  do {                                                 \
    (L) = (v) ^ ((v) << 1) ^ ((v) << 2) ^ ((v) << 7); \
    (H) = ((v) >> 63) ^ ((v) >> 62) ^ ((v) >> 57);    \
  } while (0)
  GF192_FOLD(t[3], lo, hi); r0 ^= lo; r1 ^= hi;
  GF192_FOLD(t[4], lo, hi); r1 ^= lo; r2 ^= hi;
  GF192_FOLD(t[5], lo, hi); r2 ^= lo; c = hi;  // c holds coefficients of bits 192..198
  r0 ^= c ^ (c << 1) ^ (c << 2) ^ (c << 7);    // second fold; c <= 7 bits, no further overflow
#undef GF192_FOLD
  res->v64[0] = r0;
  res->v64[1] = r1;
  res->v64[2] = r2;
}
EXPORT void gf192_flambda_product_ref(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf192_product_ref((gf192*) res, (const gf192*) a, (const gf192*) b);
}


EXPORT void gf192_dot_product_ref(gf192* res, const gf192* x, const gf192* y, const uint64_t size) {
  gf192 acc = GF192_ZERO;
  gf192 tmp;
  for (uint64_t i = 0; i < size; ++i) {
    gf192_product_ref(&tmp, &x[i], &y[i]);
    gf192_sum_ref(&acc, &acc, &tmp);
  }
  *res = acc;
}
EXPORT void gf192_flambda_dot_product_ref(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size) {
  gf192_dot_product_ref((gf192*) res, (const gf192*) x, (const gf192*) y, size);
}


EXPORT void gf192_dot_product_acc_ref(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y,
                                      uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf192) == 0, "byte slice not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf192);
  gf192 acc = *res;
  gf192 tmp;
  for (uint64_t i = 0; i < size; ++i) {
    gf192_product_ref(&tmp, &x[i * xbs], &y[i]);
    gf192_sum_ref(&acc, &acc, &tmp);
  }
  *res = acc;
}
EXPORT void gf192_flambda_dot_product_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                              const flambda_t* y, uint64_t size) {
  gf192_dot_product_acc_ref((gf192*) res, (const gf192*) x, x_byte_slice, (const gf192*) y, size);
}


EXPORT void gf192_dot_product_f2_acc_ref(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y_f2,
                                         uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf192) == 0, "byte slice not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf192);
  uint64_t acc0 = res->v64[0], acc1 = res->v64[1], acc2 = res->v64[2];
  for (uint64_t i = 0; i < size; ++i) {
    const uint64_t mask = -y_f2[i].v64[0];
    acc0 ^= x[i * xbs].v64[0] & mask;
    acc1 ^= x[i * xbs].v64[1] & mask;
    acc2 ^= x[i * xbs].v64[2] & mask;
  }
  res->v64[0] = acc0;
  res->v64[1] = acc1;
  res->v64[2] = acc2;
}
EXPORT void gf192_flambda_dot_product_f2_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y_f2, uint64_t size) {
  gf192_dot_product_f2_acc_ref((gf192*) res, (const gf192*) x, x_byte_slice, (const gf192*) y_f2, size);
}


EXPORT void gf192_product_f2_ref(gf192* res, const gf192* a, const gf192* b_f2) {
  const uint64_t mask = -b_f2->v64[0];
  for (uint64_t i = 0; i < 3; ++i) res->v64[i] = a->v64[i] & mask;
}
EXPORT void gf192_flambda_product_f2_ref(flambda_t* res, const flambda_t* a, const flambda_t* b_f2) {
  gf192_product_f2_ref((gf192*) res, (const gf192*) a, (const gf192*) b_f2);
}


EXPORT void gf192_inverse_ref(gf192* res, const gf192* a) {
  // this is the euclidian algorithm to compute the extended gcd
  gf192 aa = *a;
  CREQUIRE(!gf192p_equals(&aa, &GF192_ZERO), "inverse of 0");
  gf192 uu = GF192_ONE;
  uint64_t log2a = 191;
  while (gf192p_bitof(&aa, log2a) == 0) --log2a;
  gf192 bb;
  gf192p_lsh(&bb, &aa, 192 - log2a);
  bb.v64[0] ^= GF192_P;
  gf192 vv;
  gf192p_lsh(&vv, &uu, 192 - log2a);
  uint64_t log2b = 191;
  while (log2a != 0) {
    CASSERT(gf192v_equals(gf192v_mul(*a, uu), aa), "bug1");
    CASSERT(gf192v_equals(gf192v_mul(*a, vv), bb), "bug2");
    while (log2b >= log2a) {
      if (gf192p_bitof(&bb, log2b)) {
        gf192 tmp;
        gf192p_lsh(&tmp, &aa, log2b - log2a);
        gf192p_sum(&bb, &bb, &tmp);
        gf192p_lsh(&tmp, &uu, log2b - log2a);
        gf192p_sum(&vv, &vv, &tmp);
      }
      --log2b;
    }
    // swap aa <-> bb, uu <-> vv
    gf192 tmp = aa;
    aa = bb;
    bb = tmp;
    tmp = uu;
    uu = vv;
    vv = tmp;
    uint64_t tmp2 = log2a;
    log2a = log2b;
    log2b = tmp2;
    // update log2a
    CASSERT(!gf192p_equals(&aa, &GF192_ZERO), "bug3");
    while (gf192p_bitof(&aa, log2a) == 0) --log2a;
  }
  CASSERT(gf192p_equals(&aa, &GF192_ONE), "bug4");
  *res = uu;
}
EXPORT void gf192_flambda_inverse_ref(flambda_t* res, const flambda_t* a) {
  gf192_inverse_ref((gf192*) res, (const gf192*) a);
}


EXPORT void gf192_sum_pow2_naive(gf192* res, const gf192* x) {
  gf192 ONE = GF192_ONE;
  gf192 r = GF192_ZERO;
  gf192 tmp;
  for (uint64_t i = 0; i < 192; ++i) {
    gf192p_lsh(&tmp, &ONE, i);
    gf192p_mul(&tmp, &tmp, x + i);
    gf192p_sum(&r, &r, &tmp);
  }
  *res = r;
}

EXPORT void gf192_sum_pow2_ref(gf192* res, const gf192* x) {
  static const gf192 PP = {.v64 = {GF192_PV, 0, 0}};
  gf192 left = x[0];
  gf192 right = GF192_ZERO;
  gf192 tmp;
  for (uint64_t i = 1; i < 192; ++i) {
    gf192p_lsh(&tmp, x + i, i);
    gf192p_sum(&left, &left, &tmp);
    gf192p_rsh(&tmp, x + i, 192 - i);
    gf192p_sum(&right, &right, &tmp);
  }
  gf192p_mul(&tmp, &right, &PP);
  gf192p_sum(res, &left, &tmp);
}
EXPORT void gf192_flambda_sum_pow2_ref(flambda_t* res, const flambda_t* x) {
  gf192_sum_pow2_ref((gf192*) res, (const gf192*) x);
}


void gf192_echelon_pow2_naive(uint64_t k, gf192* res, const gf192* x, uint64_t x_size, uint64_t x_byte_slice) {
  CREQUIRE(x_byte_slice % sizeof(gf192) == 0, "byte slice not supported");
  CREQUIRE(k > 0, "k not supported");
  CREQUIRE(k * x_size <= 192, "echelon width not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf192);
  gf192 ONE = GF192_ONE;
  gf192 r = GF192_ZERO;
  gf192 tmp;
  for (uint64_t i = 0; i < x_size; ++i) {
    gf192p_lsh(&tmp, &ONE, k * i);
    gf192p_mul(&tmp, &tmp, x + i * xbs);
    gf192p_sum(&r, &r, &tmp);
  }
  *res = r;
}

void gf192_echelon_pow2_ref(uint64_t k, gf192* res, const gf192* x, uint64_t x_size, uint64_t x_byte_slice) {
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
    gf192p_sum(&left, &left, &tmp);
    gf192p_rsh(&tmp, x + i * xbs, 192 - k * i);
    gf192p_sum(&right, &right, &tmp);
  }
  gf192p_mul(&tmp, &right, &PP);
  gf192p_sum(res, &left, &tmp);
}
EXPORT void gf192_flambda_echelon_pow2_ref(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice) {
  gf192_echelon_pow2_ref(k, (gf192*) res, (const gf192*) x, x_size, x_byte_slice);
}

