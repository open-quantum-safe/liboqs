#ifndef MZD_ADDITIONAL_H
#define MZD_ADDITIONAL_H

#include "macros.h"

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint64_t word;

/* Inspired by m4ri's mzd, but completely re-written for our use-case. */

typedef struct {
  uint32_t nrows, ncols, width, rowstride;
  uint32_t padding[4];
  uint64_t rows[];
} mzd_local_t ATTR_ALIGNED(32);

mzd_local_t* mzd_local_init_ex(uint32_t r, uint32_t c, bool clear) ATTR_ASSUME_ALIGNED(32);

#define mzd_local_init(r, c) mzd_local_init_ex(r, c, true)

void mzd_local_free(mzd_local_t* v);

void mzd_local_init_multiple_ex(mzd_local_t** dst, size_t n, uint32_t r, uint32_t c, bool clear)
    ATTR_NONNULL_ARG(1);

#define mzd_local_init_multiple(dst, n, r, c) mzd_local_init_multiple_ex(dst, n, r, c, true)

/**
 * mzd_free for mzd_local_init_multiple.
 */
void mzd_local_free_multiple(mzd_local_t** vs);
/**
 * Improved mzd_copy for specific memory layouts.
 */
mzd_local_t* mzd_local_copy(mzd_local_t* dst, mzd_local_t const* src) ATTR_NONNULL_ARG(2);

void mzd_local_clear(mzd_local_t* c) ATTR_NONNULL;

void mzd_shift_right(mzd_local_t* res, mzd_local_t const* val, unsigned count) ATTR_NONNULL;

void mzd_shift_left(mzd_local_t* res, mzd_local_t const* val, unsigned count) ATTR_NONNULL;

mzd_local_t* mzd_and(mzd_local_t* res, mzd_local_t const* first,
                     mzd_local_t const* second) ATTR_NONNULL;

mzd_local_t* mzd_xor(mzd_local_t* res, mzd_local_t const* first,
                     mzd_local_t const* second) ATTR_NONNULL;
mzd_local_t* mzd_xor_sse(mzd_local_t* res, mzd_local_t const* first,
                         mzd_local_t const* second) ATTR_NONNULL;
mzd_local_t* mzd_xor_avx(mzd_local_t* res, mzd_local_t const* first,
                         mzd_local_t const* second) ATTR_NONNULL;
mzd_local_t* mzd_xor_general(mzd_local_t* res, mzd_local_t const* first,
                             mzd_local_t const* second) ATTR_NONNULL;
mzd_local_t* mzd_xor_neon(mzd_local_t* res, mzd_local_t const* first,
                          mzd_local_t const* second) ATTR_NONNULL;

/**
 * Compare two vectors for equality. Note that this version is optimized for
 * vectors with a multiple of sizeof(word) * 8 columns.
 *
 * \param first
 *          first vector
 * \param second
 *          second vector
 * \returns true if both vectors are equal, false otherwise.
 */
bool mzd_local_equal(mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;

/**
 * Compute v * A optimized for v being a vector.
 */
mzd_local_t* mzd_mul_v(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
mzd_local_t* mzd_mul_v_general(mzd_local_t* c, mzd_local_t const* v,
                               mzd_local_t const* At) ATTR_NONNULL;
mzd_local_t* mzd_mul_v_sse(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_v_avx(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_v_neon(mzd_local_t* c, mzd_local_t const* v,
                            mzd_local_t const* A) ATTR_NONNULL;

/**
 * Compute c + v * A optimized for c and v being vectors.
 */
mzd_local_t* mzd_addmul_v(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
mzd_local_t* mzd_addmul_v_general(mzd_local_t* c, mzd_local_t const* v,
                                  mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_v_sse(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A);
mzd_local_t* mzd_addmul_v_avx(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A);
mzd_local_t* mzd_addmul_v_neon(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A);

/**
 * Compute v * A optimized for v being a vector.
 */
mzd_local_t* mzd_mul_vl(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
mzd_local_t* mzd_mul_vl_general(mzd_local_t* c, mzd_local_t const* v,
                                mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_vl_sse_128(mzd_local_t* c, mzd_local_t const* v,
                                mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_vl_sse(mzd_local_t* c, mzd_local_t const* v,
                            mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_vl_avx_256(mzd_local_t* c, mzd_local_t const* v,
                                mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_vl_avx(mzd_local_t* c, mzd_local_t const* v,
                            mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_mul_vl_neon_128(mzd_local_t* c, mzd_local_t const* v,
                                 mzd_local_t const* A) ATTR_NONNULL;

mzd_local_t* mzd_mul_vl_neon_multiple_of_128(mzd_local_t* c, mzd_local_t const* v,
                                             mzd_local_t const* A) ATTR_NONNULL;
/**
 * Compute c + v * A optimized for c and v being vectors.
 */
mzd_local_t* mzd_addmul_vl(mzd_local_t* c, mzd_local_t const* v,
                           mzd_local_t const* At) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_sse_128(mzd_local_t* c, mzd_local_t const* v,
                                   mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_avx_256(mzd_local_t* c, mzd_local_t const* v,
                                   mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_sse(mzd_local_t* c, mzd_local_t const* v,
                               mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_avx(mzd_local_t* c, mzd_local_t const* v,
                               mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_general(mzd_local_t* c, mzd_local_t const* v,
                                   mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_neon_128(mzd_local_t* c, mzd_local_t const* v,
                                    mzd_local_t const* A) ATTR_NONNULL;
mzd_local_t* mzd_addmul_vl_neon(mzd_local_t* c, mzd_local_t const* v,
                                mzd_local_t const* A) ATTR_NONNULL;

/**
 * Compute v * A optimized for v being a vector.
 */
void mzd_mul_vlm(mzd_local_t** c, mzd_local_t const* const* v, mzd_local_t const* At,
                 unsigned int sc) ATTR_NONNULL;

/**
 * Compute c + v * A optimized for c and v being vectors.
 */
void mzd_addmul_vlm(mzd_local_t** c, mzd_local_t const* const* v, mzd_local_t const* At,
                    unsigned int sc) ATTR_NONNULL;

/**
 * Pre-compute matrices for faster mzd_addmul_v computions.
 *
 */
mzd_local_t* mzd_precompute_matrix_lookup(mzd_local_t const* A) ATTR_NONNULL;

#define ROW(v, r) ((word*)(((void*)(v)) + 32 + (v)->rowstride * (r) * sizeof(word)))
#define CONST_ROW(v, r)                                                                            \
  ((word const*)(((void const*)(v)) + 32 + (v)->rowstride * (r) * sizeof(word)))

#define FIRST_ROW(v) ROW(v, 0)
#define CONST_FIRST_ROW(v) CONST_ROW(v, 0)

#define WRITE_BIT(w, spot, value)                                                                  \
  ((w) = (((w) & ~(UINT64_C(1) << (spot))) | (-(word)(value) & (UINT64_C(1) << (spot)))))

#define mzd_local_write_bit(v, r, c, b)                                                            \
  WRITE_BIT(ROW(v, r)[c / (sizeof(word) * 8)], c % (sizeof(word) * 8), b)

#endif
