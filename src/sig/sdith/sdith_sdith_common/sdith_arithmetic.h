#ifndef SDITH_ARITHMETIC_H
#define SDITH_ARITHMETIC_H

// This file contains the declaration of all aritmetic functions over F_2^128, F_2^192, and F_2^256

#include "commons.h"

typedef union gf128_t gf128 __attribute((aligned(16)));
typedef union gf192_t gf192 __attribute((aligned(8)));
typedef union gf256_t gf256 __attribute((aligned(32)));

/* res = a [requires a flambda_t version] */
EXPORT void gf128_set_ref(gf128* res, const gf128* a);
EXPORT void gf192_set_ref(gf192* res, const gf192* a);
EXPORT void gf256_set_ref(gf256* res, const gf256* a);
EXPORT void gf128_flambda_set_ref(flambda_t* res, const flambda_t* a);
EXPORT void gf192_flambda_set_ref(flambda_t* res, const flambda_t* a);
EXPORT void gf256_flambda_set_ref(flambda_t* res, const flambda_t* a);

/* res = a + b [requires a flambda_t version] */
EXPORT void gf128_sum_ref(gf128* res, const gf128* a, const gf128* b);
EXPORT void gf128_sum_avx2(gf128* res, const gf128* a, const gf128* b);
EXPORT void gf192_sum_ref(gf192* res, const gf192* a, const gf192* b);
EXPORT void gf256_sum_ref(gf256* res, const gf256* a, const gf256* b);
EXPORT void gf256_sum_avx2(gf256* res, const gf256* a, const gf256* b);
EXPORT void gf128_flambda_sum_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf128_flambda_sum_avx2(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf192_flambda_sum_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf256_flambda_sum_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf256_flambda_sum_avx2(flambda_t* res, const flambda_t* a, const flambda_t* b);

/* res = a * b [requires a flambda_t version] */
EXPORT void gf128_product_ref(gf128* res, const gf128* a, const gf128* b);
EXPORT void gf128_product_f2_ref(gf128* res, const gf128* a, const gf128* b_f2);
EXPORT void gf128_product_pclmul(gf128* res, const gf128* a, const gf128* b);
EXPORT void gf128_product_pclmul_f2(gf128* res, const gf128* a, const gf128* b);
EXPORT void gf192_product_ref(gf192* res, const gf192* a, const gf192* b);
EXPORT void gf192_product_f2_ref(gf192* res, const gf192* a, const gf192* b);
EXPORT void gf192_product_pclmul(gf192* res, const gf192* a, const gf192* b);
EXPORT void gf256_product_ref(gf256* res, const gf256* a, const gf256* b);
EXPORT void gf256_product_f2_ref(gf256* res, const gf256* a, const gf256* b);
EXPORT void gf256_product_pclmul(gf256* res, const gf256* a, const gf256* b);
EXPORT void gf256_product_pclmul_f2(gf256* res, const gf256* a, const gf256* b);
EXPORT void gf128_flambda_product_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf128_flambda_product_f2_ref(flambda_t* res, const flambda_t* a, const flambda_t* b_f2);
EXPORT void gf128_flambda_product_pclmul(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf128_flambda_product_pclmul_f2(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf192_flambda_product_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf192_flambda_product_f2_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf192_flambda_product_pclmul(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf256_flambda_product_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf256_flambda_product_f2_ref(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf256_flambda_product_pclmul(flambda_t* res, const flambda_t* a, const flambda_t* b);
EXPORT void gf256_flambda_product_pclmul_f2(flambda_t* res, const flambda_t* a, const flambda_t* b);

/* res = sum_{i=0}^{size-1} x[i] * y[i] [requires a flambda_t version] */
EXPORT void gf128_dot_product_ref(gf128* res, const gf128* x, const gf128* y, const uint64_t size);
EXPORT void gf128_dot_product_pclmul(gf128* res, const gf128* x, const gf128* y, const uint64_t size);
EXPORT void gf192_dot_product_ref(gf192* res, const gf192* x, const gf192* y, const uint64_t size);
EXPORT void gf192_dot_product_pclmul(gf192* res, const gf192* x, const gf192* y, const uint64_t size);
EXPORT void gf256_dot_product_ref(gf256* res, const gf256* x, const gf256* y, const uint64_t size);
EXPORT void gf256_dot_product_pclmul(gf256* res, const gf256* x, const gf256* y, const uint64_t size);
EXPORT void gf128_flambda_dot_product_ref(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size);
EXPORT void gf128_flambda_dot_product_pclmul(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size);
EXPORT void gf192_flambda_dot_product_ref(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size);
EXPORT void gf192_flambda_dot_product_pclmul(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size);
EXPORT void gf256_flambda_dot_product_ref(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size);
EXPORT void gf256_flambda_dot_product_pclmul(flambda_t* res, const flambda_t* x, const flambda_t* y, const uint64_t size);

/* res += sum_{i=0}^{size-1} x[i . x_byte_slice] * y[i]     (x is strided, y is contiguous)
 * x_byte_slice is a byte stride, and must be a multiple of the element size. [requires a flambda_t version] */
EXPORT void gf128_dot_product_acc_ref(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y,
                                      uint64_t size);
EXPORT void gf128_dot_product_acc_pclmul(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y,
                                         uint64_t size);
EXPORT void gf192_dot_product_acc_ref(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y,
                                      uint64_t size);
EXPORT void gf192_dot_product_acc_pclmul(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y,
                                         uint64_t size);
EXPORT void gf256_dot_product_acc_ref(gf256* res, const gf256* x, uint64_t x_byte_slice, const gf256* y,
                                      uint64_t size);
EXPORT void gf256_dot_product_acc_pclmul(gf256* res, const gf256* x, uint64_t x_byte_slice, const gf256* y,
                                         uint64_t size);
EXPORT void gf128_flambda_dot_product_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                              const flambda_t* y, uint64_t size);
EXPORT void gf128_flambda_dot_product_acc_pclmul(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y, uint64_t size);
EXPORT void gf192_flambda_dot_product_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                              const flambda_t* y, uint64_t size);
EXPORT void gf192_flambda_dot_product_acc_pclmul(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y, uint64_t size);
EXPORT void gf256_flambda_dot_product_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                              const flambda_t* y, uint64_t size);
EXPORT void gf256_flambda_dot_product_acc_pclmul(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y, uint64_t size);

/* res += sum_{i=0}^{size-1} x[i . x_byte_slice] * y_f2[i], where every y_f2[i] is 0 or 1.
 * Same conventions as the *_acc dot products above. [requires a flambda_t version] */
EXPORT void gf128_dot_product_f2_acc_ref(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y_f2,
                                         uint64_t size);
EXPORT void gf128_dot_product_f2_acc_avx2(gf128* res, const gf128* x, uint64_t x_byte_slice, const gf128* y_f2,
                                          uint64_t size);
EXPORT void gf192_dot_product_f2_acc_ref(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y_f2,
                                         uint64_t size);
EXPORT void gf192_dot_product_f2_acc_avx2(gf192* res, const gf192* x, uint64_t x_byte_slice, const gf192* y_f2,
                                          uint64_t size);
EXPORT void gf256_dot_product_f2_acc_ref(gf256* res, const gf256* x, uint64_t x_byte_slice, const gf256* y_f2,
                                         uint64_t size);
EXPORT void gf256_dot_product_f2_acc_avx2(gf256* res, const gf256* x, uint64_t x_byte_slice, const gf256* y_f2,
                                          uint64_t size);
EXPORT void gf128_flambda_dot_product_f2_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y_f2, uint64_t size);
EXPORT void gf128_flambda_dot_product_f2_acc_avx2(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                  const flambda_t* y_f2, uint64_t size);
EXPORT void gf192_flambda_dot_product_f2_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y_f2, uint64_t size);
EXPORT void gf192_flambda_dot_product_f2_acc_avx2(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                  const flambda_t* y_f2, uint64_t size);
EXPORT void gf256_flambda_dot_product_f2_acc_ref(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                 const flambda_t* y_f2, uint64_t size);
EXPORT void gf256_flambda_dot_product_f2_acc_avx2(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                                  const flambda_t* y_f2, uint64_t size);

/* res = a^-1 [requires a flambda_t version] */
EXPORT void gf128_inverse_ref(gf128* res, const gf128* a);
EXPORT void gf128_inverse_pclmul(gf128* res, const gf128* a);
EXPORT void gf192_inverse_ref(gf192* res, const gf192* a);
EXPORT void gf192_inverse_pclmul(gf192* res, const gf192* a);
EXPORT void gf256_inverse_ref(gf256* res, const gf256* a);
EXPORT void gf256_inverse_pclmul(gf256* res, const gf256* a);
EXPORT void gf128_flambda_inverse_ref(flambda_t* res, const flambda_t* a);
EXPORT void gf128_flambda_inverse_pclmul(flambda_t* res, const flambda_t* a);
EXPORT void gf192_flambda_inverse_ref(flambda_t* res, const flambda_t* a);
EXPORT void gf192_flambda_inverse_pclmul(flambda_t* res, const flambda_t* a);
EXPORT void gf256_flambda_inverse_ref(flambda_t* res, const flambda_t* a);
EXPORT void gf256_flambda_inverse_pclmul(flambda_t* res, const flambda_t* a);

/** Combine lambda bit Vole into a big field Vole */
/** res = sum 2^i.x_i [requires a flambda_t version] */
EXPORT void gf128_sum_pow2_naive(gf128* res, const gf128* x);
EXPORT void gf128_sum_pow2_ref(gf128* res, const gf128* x);
EXPORT void gf192_sum_pow2_naive(gf192* res, const gf192* x);
EXPORT void gf192_sum_pow2_ref(gf192* res, const gf192* x);
EXPORT void gf192_sum_pow2_avx2(gf192* res, const gf192* x);
EXPORT void gf256_sum_pow2_naive(gf256* res, const gf256* x);
EXPORT void gf256_sum_pow2_ref(gf256* res, const gf256* x);
EXPORT void gf128_flambda_sum_pow2_ref(flambda_t* res, const flambda_t* x);
// EXPORT void gf128_flambda_sum_pow2_avx2(flambda_t* res, const flambda_t* x);
EXPORT void gf192_flambda_sum_pow2_ref(flambda_t* res, const flambda_t* x);
// EXPORT void gf192_flambda_sum_pow2_avx2(flambda_t* res, const flambda_t* x);
EXPORT void gf256_flambda_sum_pow2_ref(flambda_t* res, const flambda_t* x);
// EXPORT void gf256_flambda_sum_pow2_avx2(flambda_t* res, const flambda_t* x);

/** sum_{i=0 to k-1} x_i . 2^{k.i} [requires flambda version] */
EXPORT void gf128_echelon_pow2_naive(uint64_t k, gf128* res, const gf128* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf128_echelon_pow2_ref(uint64_t k, gf128* res, const gf128* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf128_echelon_pow2_avx(uint64_t k, gf128* res, const gf128* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf192_echelon_pow2_naive(uint64_t k, gf192* res, const gf192* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf192_echelon_pow2_ref(uint64_t k, gf192* res, const gf192* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf192_echelon_pow2_avx(uint64_t k, gf192* res, const gf192* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf256_echelon_pow2_naive(uint64_t k, gf256* res, const gf256* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf256_echelon_pow2_ref(uint64_t k, gf256* res, const gf256* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf256_echelon_pow2_avx(uint64_t k, gf256* res, const gf256* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf128_flambda_echelon_pow2_ref(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf128_flambda_echelon_pow2_avx(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf192_flambda_echelon_pow2_ref(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf192_flambda_echelon_pow2_avx(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf256_flambda_echelon_pow2_ref(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice);
EXPORT void gf256_flambda_echelon_pow2_avx(uint64_t k, flambda_t* res, const flambda_t* x, uint64_t x_size, uint64_t x_byte_slice);

/** This function takes a matrix of B x B bits, and transposes it */
EXPORT void transpose_128_128_naive(void* out, const void* in);
EXPORT void transpose_192_192_naive(void* out, const void* in);
EXPORT void transpose_256_256_naive(void* out, const void* in);
EXPORT void transpose_128_128_ref(void* x);
EXPORT void transpose_192_192_ref(void* x);
EXPORT void transpose_256_256_ref(void* x);
EXPORT void transpose_128_L_naive(void* out, const void* in, uint64_t L, uint64_t Lslice);
EXPORT void transpose_192_L_naive(void* out, const void* in, uint64_t L, uint64_t Lslice);
EXPORT void transpose_256_L_naive(void* out, const void* in, uint64_t L, uint64_t Lslice);
EXPORT void transpose_128_L_ref(void* out, const void* in, uint64_t L, uint64_t Lslice);
EXPORT void transpose_192_L_ref(void* out, const void* in, uint64_t L, uint64_t Lslice);
EXPORT void transpose_256_L_ref(void* out, const void* in, uint64_t L, uint64_t Lslice);

#endif  // SDITH_ARITHMETIC_H
