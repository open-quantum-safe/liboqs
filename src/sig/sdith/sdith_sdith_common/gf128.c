#include "inttypes.h"
#include "vole_private.h"

const gf128 GF128_ZERO = {.v128 = 0};
const gf128 GF128_ONE = {.v128 = 1};

__always_inline uint8_t gf128p_equals(const gf128* const a, const gf128* const b) { return a->v128 == b->v128; }

void gf128p_sum(gf128* const res, const gf128* const a, const gf128* const b) { gf128_sum_ref(res, a, b); }

__always_inline uint8_t gf128p_bitof(const gf128* const a, const uint64_t position) {
  CREQUIRE(position >= 0 && position < 128, "bad bit position %" PRIu64, position);
  return (a->v128 >> position) & 1;
}

void gf128p_lsh(gf128* const res, const gf128* const a, const uint64_t amount) {
  CREQUIRE(amount < 128, "bad left shift amount %" PRIu64, amount);
  res->v128 = a->v128 << amount;
}

void gf128p_rsh(gf128* const res, const gf128* const a, const uint64_t amount) {
  CREQUIRE(amount < 128, "bad right shift amount %" PRIu64, amount);
  res->v128 = a->v128 >> amount;
}

void gf128p_mul(gf128* const res, const gf128* const a, const gf128* const b) { gf128_product_ref(res, a, b); }

uint8_t gf128v_equals(const gf128 a, const gf128 b) { return gf128p_equals(&a, &b); }

gf128 gf128v_sum(const gf128 a, const gf128 b) {
  gf128 res;
  gf128p_sum(&res, &a, &b);
  return res;
}

uint8_t gf128v_bitof(const gf128 a, const uint64_t position) { return gf128p_bitof(&a, position); }

gf128 gf128v_lsh(const gf128 a, const uint64_t amount) {
  gf128 res;
  gf128p_lsh(&res, &a, amount);
  return res;
}

gf128 gf128v_rsh(const gf128 a, const uint64_t amount) {
  gf128 res;
  gf128p_rsh(&res, &a, amount);
  return res;
}

gf128 gf128v_mul(const gf128 a, const gf128 b) {
  gf128 res;
  gf128p_mul(&res, &a, &b);
  return res;
}

EXPORT void gf128_set_ref(gf128* res, const gf128* a) { *res = *a; }
EXPORT void gf128_flambda_set_ref(flambda_t* res, const flambda_t* a) {
  gf128_set_ref((gf128*) res, (const gf128*) a);
};


void gf128_sum_ref(gf128* res, const gf128* a, const gf128* b) { res->v128 = a->v128 ^ b->v128; }
EXPORT void gf128_flambda_sum_ref(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf128_sum_ref((gf128*) res, (const gf128*) a, (const gf128*) b);
}


__uint128_t gfprod128(__uint128_t a, __uint128_t b) {
  __uint128_t tt = a;
  __uint128_t rr = 0;
  for (uint64_t i = 0; i < 128; ++i) {
    rr ^= (-((b >> i) & (__uint128_t)1)) & tt;
    tt = ((-(tt >> 127)) & GF128_P) ^ (tt << 1);
  }
  return rr;
}

void gf128_product_ref(gf128* res, const gf128* a, const gf128* b) { res->v128 = gfprod128(a->v128, b->v128); }
EXPORT void gf128_flambda_product_ref(flambda_t* res, const flambda_t* a, const flambda_t* b) {
  gf128_product_ref((gf128*) res, (const gf128*) a, (const gf128*) b);
}


__uint128_t gfprod128_f2(__uint128_t a, __uint128_t b) { return (-b) & a; }

void gf128_product_f2_ref(gf128* res, const gf128* a, const gf128* b) { res->v128 = gfprod128_f2(a->v128, b->v128); }
EXPORT void gf128_flambda_product_f2_ref(flambda_t* res, const flambda_t* a, const flambda_t* b_f2) {
  gf128_product_f2_ref((gf128*) res, (const gf128*) a, (const gf128*) b_f2);
}


EXPORT void gf128_dot_product_ref(gf128* res, const gf128* x, const gf128* y, const uint64_t size) {
  __uint128_t acc = 0;
  for (uint64_t i = 0; i < size; ++i) {
    acc ^= gfprod128(x[i].v128, y[i].v128);
  }
  res->v128 = acc;
}
EXPORT void gf128_flambda_dot_product_ref(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size) {
  gf128_dot_product_ref((gf128*) res, (const gf128*) x, (const gf128*) y, size);
}


EXPORT void gf128_dot_product_acc_ref(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y,
                                      uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf128) == 0, "byte slice not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf128);
  __uint128_t acc = res->v128;
  for (uint64_t i = 0; i < size; ++i) {
    acc ^= gfprod128(x[i * xbs].v128, y[i].v128);
  }
  res->v128 = acc;
}
EXPORT void gf128_flambda_dot_product_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                              const flambda_t* y, uint64_t size) {
  gf128_dot_product_acc_ref((gf128*) res, (const gf128*) x, x_byte_slice, (const gf128*) y, size);
}


EXPORT void gf128_dot_product_f2_acc_ref(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y_f2,
                                         uint64_t size) {
  CASSERT(x_byte_slice % sizeof(gf128) == 0, "byte slice not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf128);
  __uint128_t acc = res->v128;
  for (uint64_t i = 0; i < size; ++i) {
    acc ^= gfprod128_f2(x[i * xbs].v128, y_f2[i].v128);
  }
  res->v128 = acc;
}
EXPORT void gf128_flambda_dot_product_f2_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y_f2, uint64_t size) {
  gf128_dot_product_f2_acc_ref((gf128*) res, (const gf128*) x, x_byte_slice, (const gf128*) y_f2, size);
}


EXPORT void gf128_inverse_ref(gf128* res, const gf128* a) {
  // this is the euclidian algorithm to compute the extended gcd
  __uint128_t aa = a->v128;
  CREQUIRE(aa != 0, "inverse of 0");
  __uint128_t uu = 1;
  uint64_t log2a = 127;
  while (((aa >> log2a) & 1) == 0) --log2a;
  if (log2a == 0) {  // a == 1: inverse is 1. Early-out also avoids the aa << 128 UB below.
    res->v128 = 1;
    return;
  }
  __uint128_t bb = GF128_P ^ (aa << (128 - log2a));
  __uint128_t vv = uu << (128 - log2a);
  uint64_t log2b = 127;
  while (log2a != 0) {
    CASSERT(gfprod128(a->v128, uu) == aa, "bug1");
    CASSERT(gfprod128(a->v128, vv) == bb, "bug2");
    while (log2b >= log2a) {
      if ((bb >> log2b) & 1) {
        bb ^= (aa << (log2b - log2a));
        vv ^= (uu << (log2b - log2a));
      }
      --log2b;
    }
    // swap aa <-> bb, uu <-> vv
    __uint128_t tmp = aa;
    aa = bb;
    bb = tmp;
    tmp = uu;
    uu = vv;
    vv = tmp;
    uint64_t tmp2 = log2a;
    log2a = log2b;
    log2b = tmp2;
    // update log2a
    CASSERT(aa != 0, "bug3");
    while (((aa >> log2a) & 1) == 0) --log2a;
  }
  CASSERT(aa == 1, "bug4");
  res->v128 = uu;
}
EXPORT void gf128_flambda_inverse_ref(flambda_t* res, const flambda_t* a) {
  gf128_inverse_ref((gf128*) res, (const gf128*) a);
}


EXPORT void gf128_sum_pow2_naive(gf128* res, const gf128* x) {
  static const __uint128_t ONE = 1;
  __uint128_t r = 0;
  for (uint64_t i = 0; i < 128; ++i) {
    r ^= gfprod128(ONE << i, x[i].v128);
  }
  res->v128 = r;
}

EXPORT void gf128_sum_pow2_ref(gf128* res, const gf128* x) {
  __uint128_t left = x[0].v128;
  __uint128_t right = 0;
  for (uint64_t i = 1; i < 128; ++i) {
    __uint128_t xi = x[i].v128;
    left ^= xi << i;
    right ^= xi >> (128 - i);
  }
  res->v128 = left ^ gfprod128(GF128_P, right);
}
EXPORT void gf128_flambda_sum_pow2_ref(flambda_t* res, const flambda_t* x) {
  gf128_sum_pow2_ref((gf128*) res, (const gf128*) x);
}


void gf128_echelon_pow2_naive(uint64_t k, gf128* res, const gf128* x, uint64_t x_size, uint64_t x_byte_slice) {
  CREQUIRE(x_byte_slice % sizeof(gf128) == 0, "byte slice not supported");
  CREQUIRE(k > 0, "k not supported");
  CREQUIRE(k * x_size <= 128, "echelon width not supported");
  const uint64_t xbs = x_byte_slice / sizeof(gf128);
  gf128 ONE = GF128_ONE;
  gf128 r = GF128_ZERO;
  gf128 tmp;
  for (uint64_t i = 0; i < x_size; ++i) {
    gf128p_lsh(&tmp, &ONE, k * i);
    gf128p_mul(&tmp, &tmp, x + i * xbs);
    gf128p_sum(&r, &r, &tmp);
  }
  *res = r;
}

void gf128_echelon_pow2_ref(uint64_t k, gf128* res, const gf128* x, uint64_t x_size, uint64_t x_byte_slice) {
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
    gf128p_sum(&left, &left, &tmp);
    gf128p_rsh(&tmp, x + i * xbs, 128 - k * i);
    gf128p_sum(&right, &right, &tmp);
  }
  gf128p_mul(&tmp, &right, &PP);
  gf128p_sum(res, &left, &tmp);
}
EXPORT void gf128_flambda_echelon_pow2_ref(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice) {
  gf128_echelon_pow2_ref(k, (gf128*) res, (const gf128*) x, x_size, x_byte_slice);
}

