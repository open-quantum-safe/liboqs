#ifndef __FIELDS_AVX2_H__
#define __FIELDS_AVX2_H__

/* Check for AVX2 support */
#ifdef __AVX2__

#include "fields_common.h"
#include "fields_ref.h"
/* Needed for memcpy */
#include <string.h>
/* Needed for AVX2 assembly intrinsics */
#include <immintrin.h>

/* === GF(2) === */
/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint8_t gf2_mult_avx2(uint8_t a, uint8_t b) {
	return gf2_mult_ref(a, b);
}

#define AVX_MASK_SET 0x80000000

/* Lane selection mask shared by the "incomplete" load/store helpers below:
 * returns a mask whose (len / 4) first 32-bit lanes have their most significant
 * bit set, which is what _mm256_maskload_epi32 / _mm256_maskstore_epi32 look at.
 *
 * PRECONDITION: len <= 32. 
 *
 */
static inline __m256i lane_mask_m256(uint32_t len) {
	/* 8 all-ones 32-bit lanes followed by 8 all-zero ones. Reading 256 bits
	 * starting at element (8 - (len / 4)) therefore yields exactly (len / 4)
	 * selected lanes, in one unaligned load and with no branch at all. */
	static const uint32_t sliding[16] = {
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0, 0, 0, 0, 0, 0, 0, 0
	};
	uint32_t nb_lanes = len / 4;

	/* Clamp rather than trust the precondition */
	if (nb_lanes > 8) {
		nb_lanes = 8;
	}

	return _mm256_loadu_si256((const __m256i*)&sliding[8 - nb_lanes]);
}

static inline __m256i load_incomplete_m256_aligned32(const uint8_t *a, uint32_t len) {
	/* len == 0 yields an all-zero mask, hence an all-zero result, and masked
	 * off lanes are never accessed: no fault on the tail of the buffer. */
	return _mm256_maskload_epi32((int const*)a, lane_mask_m256(len));
}

static inline __m256i load_incomplete_m256_unaligned32(const uint8_t *a, uint32_t len) {
	/* Deal with the 32-bit leftover */
	__m256i res, expanded_leftover, lane_sel;
	uint32_t leftover;
	unsigned int i;
	const __m256i lane_idx = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);

	/* Extract the leftover: the (len % 4) bytes that do not fill a full
	 * 32-bit lane. Only these bytes are read, so we never read past a[len-1]. */
	leftover = 0;
	for (i = 0; i < (len % 4); i++) {
		leftover |= (a[(4 * (len / 4)) + i] << (8 * i));
	}
	expanded_leftover = _mm256_set1_epi32((int) leftover);

	/* Load the complete 32-bit lanes, then drop the partial lane (index
	 * len / 4) in place. Same branch-free rationale as above. */
	res = _mm256_maskload_epi32((int const*)a, lane_mask_m256(len & ~((uint32_t) 3)));
	lane_sel = _mm256_cmpeq_epi32(lane_idx, _mm256_set1_epi32((int)(len / 4)));
	res = _mm256_blendv_epi8(res, expanded_leftover, lane_sel);

	return res;
}

static inline void store_incomplete_m256_aligned32(__m256i in, uint8_t *a, uint32_t len) {
	/* len == 0 yields an all-zero mask, hence no store at all. */
	_mm256_maskstore_epi32((int*)a, lane_mask_m256(len), in);

	return;
}

static inline void store_incomplete_m256_unaligned32(__m256i in, uint8_t *a, uint32_t len) {
	uint8_t local_a[32];
	_mm256_storeu_si256((__m256i*)&local_a[0], in);
	memcpy(&a[0], &local_a[0], len);

	return;
}

/* This helper tries to efficiently copy len bytes from the ymm register */
static inline void store_incomplete_m256(__m256i in, uint8_t *a, uint32_t len) {
	if (len == 32) {
		_mm256_storeu_si256((__m256i*)a, in);
	} else if (len % 4 == 0) {
		store_incomplete_m256_aligned32(in, a, len);
	} else {
		store_incomplete_m256_unaligned32(in, a, len);
	}
	return;
}


/* This helper tries to efficiently copy len bytes in the ymm register */
static inline __m256i load_incomplete_m256(const uint8_t *a, uint32_t len) {
	if (len % 4 == 0) {
		return load_incomplete_m256_aligned32(a, len);
	} else {
		return load_incomplete_m256_unaligned32(a, len);
	}
}

/* Fold a ymm accumulator down to a single 128-bit register by XORing the two
 * lanes: shared prologue of all the horizontal XOR reductions below. */
static inline __m128i fold_lanes_avx2(__m256i v) {
	return _mm_xor_si128(_mm256_castsi256_si128(v), _mm256_extracti128_si256(v, 1));
}

static inline uint8_t parity_avx2(__m256i v) {
	__m128i t = fold_lanes_avx2(v);
#if defined(__x86_64__)
	uint64_t a = ((uint64_t)_mm_cvtsi128_si64(t)) ^ ((uint64_t)_mm_extract_epi64(t, 1));

	return (uint8_t)(_mm_popcnt_u64(a) & 1);
#else
	/* 32-bit x86: _mm_cvtsi128_si64 / _mm_extract_epi64 / _mm_popcnt_u64 need a
	 * 64-bit GPR and simply do not exist there. Fold to 32 bits instead - since
	 * popcount(a ^ b) and popcount(a) + popcount(b) agree modulo 2, folding by
	 * XOR preserves the parity of the whole register whatever the width.
	 * _mm_extract_epi32 (SSE4.1) and _mm_popcnt_u32 (POPCNT) are both implied by
	 * -mavx2 in 32-bit mode. */
	uint32_t a = ((uint32_t)_mm_cvtsi128_si32(t))
	           ^ ((uint32_t)_mm_extract_epi32(t, 1))
	           ^ ((uint32_t)_mm_extract_epi32(t, 2))
	           ^ ((uint32_t)_mm_extract_epi32(t, 3));

	return (uint8_t)(_mm_popcnt_u32(a) & 1);
#endif
}

/* Horizontal XOR of the 32 bytes of the accumulator.
 * NOTE: this is a pure binary tree of XORs done in the SIMD domain (lane fold,
 * then 64/32/16/8 bit folds), which is much cheaper than extracting the four
 * 64-bit words and unrolling a scalar byte loop. */
static inline uint8_t sum_uint8_avx2(__m256i accu) {
	__m128i t = fold_lanes_avx2(accu);

	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0x4E));
	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0xB1));
	t = _mm_xor_si128(t, _mm_srli_epi32(t, 16));
	t = _mm_xor_si128(t, _mm_srli_epi32(t, 8));

	return (uint8_t)_mm_cvtsi128_si32(t);
}

/* Horizontal XOR of the 16 16-bit words of the accumulator, same principle as
 * sum_uint8_avx2() but stopping the fold at the 16-bit granularity. */
static inline uint16_t sum_uint16_avx2(__m256i accu) {
	__m128i t = fold_lanes_avx2(accu);

	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0x4E));
	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0xB1));
	t = _mm_xor_si128(t, _mm_srli_epi32(t, 16));

	return (uint16_t)_mm_cvtsi128_si32(t);
}

/*
 * Vector multiplied by a constant in GF(2).
 */
static inline void gf2_constant_vect_mult_avx2(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	gf2_constant_vect_mult_ref(b, a, c, len);

	return;
}

static inline uint8_t gf2_vect_mult_avx2(const uint8_t *a, const uint8_t *b, uint32_t len_bits) {
	uint32_t i;
	__m256i accu, _a, _b;
	uint32_t len = (len_bits / 8);

	/* Set the accumulator to 0 */
	accu = _mm256_setzero_si256();
	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			/* Note: if we are here, we are sure that we are 32-bit aligned */
			_a = load_incomplete_m256(&a[i], len - i);
			_b = load_incomplete_m256(&b[i], len - i);
		} else {
			/* Obvious 256-bit */
			_a = _mm256_lddqu_si256((__m256i*)&a[i]);
			_b = _mm256_lddqu_si256((__m256i*)&b[i]);
		}
		/* Vectorized AND of inputs and then XOR with the accumulator */
		accu ^= (_a & _b);
	}

	/* Now, we have to compute the parity bit, do it 64 bits per 64 bits */
	return parity_avx2(accu);
}

/* Matrix and vector multiplication over GF(2)
 * C = A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
/* XXX: TODO: this can be optimized by packing rows in zmm when n <= 256 */
static inline void gf2_mat_mult_avx2(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF2_MAT_MULT(A, X, Y, n, mtype, gf2_vect_mult_avx2);
}

/* GF(2) matrix transposition */
static inline void gf2_mat_transpose_avx2(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	gf2_mat_transpose_ref(A, B, n, mtype);
}

/* === GF(256) === */
/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint8_t gf256_mult_avx2(uint8_t x, uint8_t y) {
	return gf256_mult_ref(x, y);
}

static inline __m256i gf256_mult_vectorized_avx2(__m256i _a, __m256i _b) {
	/* NOTE: when GFNI is detected, we use the accelerated GF(256) Rijndael instruction */
#if defined(__GFNI__) && !defined(NO_GFNI)
	return _mm256_gf2p8mul_epi8(_a, _b);
#else
	/* Fallback to the slower implementation without GFNI */
	/* Our reduction polynomial */
	const __m256i red_poly = _mm256_set_epi64x(0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B);
	const __m256i zero     = _mm256_setzero_si256();
	__m256i accu = _mm256_setzero_si256();

	uint32_t j;
	__m256i mask_lsb, tmp;

	/* Compute the vectorized multiplication in GF(256) */
	for (j = 0; j < 8; j++) {
		mask_lsb = _mm256_slli_epi64(_b, 7 - j);
		accu ^= _mm256_blendv_epi8(zero, _a, mask_lsb);
		tmp = _mm256_add_epi8(_a, _a);
		_a = _mm256_blendv_epi8(zero, red_poly, _a) ^ tmp;
	}
	return accu;
#endif
}

/* ============================================================================
 * Matrix by vector products: shared "precomputed multiples".
 *
 * In A * X the vector X is common to every row, so instead of paying a full
 * generic GF(256) vectorized multiplication (8 rounds of double-and-add) for
 * each 32-byte chunk of each row, we precompute once the 8 multiples
 * X * 2^k (k = 0..7) of every 32-byte block of X. A row chunk then only costs
 * one bit-select (blendv) plus one XOR per bit of the matrix coefficients,
 * i.e. roughly half of the generic path, and the doubling chain is paid once
 * per matrix instead of once per row.
 *
 * ============================================================================
 */

/* gf_prefix_mask_table + (32 - k) is a 32-byte mask made of k leading 0xFF
 * bytes followed by zeroes, for any k in [0, 32]. */
static const uint8_t gf_prefix_mask_table[64] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static inline __m256i gf_prefix_mask_avx2(uint32_t k) {
	return _mm256_loadu_si256((const __m256i*)(const void*)&gf_prefix_mask_table[32 - k]);
}

/* Number of 32-byte blocks of the common vector whose multiples are kept
 * live at once. This bounds the scratch space (8 * 32 bytes per block for
 * GF(256), 16 * 32 bytes per block for GF(256^2)) while still covering the
 * whole vector in a single pass for every parameter set where the vector fits
 * in GF_MULT_PANEL_BLOCKS * 32 bytes. */
#ifndef GF_MULT_PANEL_BLOCKS
#define GF_MULT_PANEL_BLOCKS 4
#endif
/* Overridable from the command line, so pin the one value the code cannot survive:
 * the panel loops step by GF_MULT_PANEL_BLOCKS and size mul[] with it, so 0 gives a
 * zero-length array and a loop that never advances. */
#if GF_MULT_PANEL_BLOCKS < 1
#error "GF_MULT_PANEL_BLOCKS must be at least 1"
#endif

/* Doubling in GF(256) (multiplication by the element 2) of every byte. */
static inline __m256i gf256_xtime_avx2(__m256i v) {
	const __m256i red_poly = _mm256_set_epi64x(0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B);
	const __m256i zero     = _mm256_setzero_si256();

	return _mm256_blendv_epi8(zero, red_poly, v) ^ _mm256_add_epi8(v, v);
}

/* Compute the 8 multiples v * 2^k (k = 0..7) of a 32-byte GF(256) block.
 * NOTE: with GFNI a full GF(256) product is a single instruction, so the
 * multiples are useless there and only the block itself is kept. */
static inline void gf256_precompute_multiples_avx2(__m256i v, __m256i out[8]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	out[0] = v;
#else
	uint32_t k;

	out[0] = v;
	for (k = 1; k < 8; k++) {
		v = gf256_xtime_avx2(v);
		out[k] = v;
	}
#endif

	return;
}

/* Multiply and accumulate: acc ^= a * X, where mul[] holds the precomputed
 * multiples of the X block. Two partial accumulators are used to shorten the
 * dependency chain. */
static inline __m256i gf256_mac_block_avx2(__m256i acc, __m256i a, const __m256i mul[8]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	return acc ^ _mm256_gf2p8mul_epi8(a, mul[0]);
#else
	const __m256i zero = _mm256_setzero_si256();
	__m256i t0, t1;

	t0  = _mm256_blendv_epi8(zero, mul[0], _mm256_slli_epi64(a, 7));
	t1  = _mm256_blendv_epi8(zero, mul[1], _mm256_slli_epi64(a, 6));
	t0 ^= _mm256_blendv_epi8(zero, mul[2], _mm256_slli_epi64(a, 5));
	t1 ^= _mm256_blendv_epi8(zero, mul[3], _mm256_slli_epi64(a, 4));
	t0 ^= _mm256_blendv_epi8(zero, mul[4], _mm256_slli_epi64(a, 3));
	t1 ^= _mm256_blendv_epi8(zero, mul[5], _mm256_slli_epi64(a, 2));
	t0 ^= _mm256_blendv_epi8(zero, mul[6], _mm256_slli_epi64(a, 1));
	t1 ^= _mm256_blendv_epi8(zero, mul[7], a);

	return acc ^ t0 ^ t1;
#endif
}

/*
 * Vector multiplied by a constant in GF(256).
 */

static inline void gf256_constant_vect_mult_avx2(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	uint32_t i;

#if defined(__GFNI__) && !defined(NO_GFNI)
	/* GFNI: single _mm256_gf2p8mul_epi8 instruction per 32-byte block */
	__m256i _b = _mm256_set1_epi8((int8_t)b);
	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			store_incomplete_m256(
				gf256_mult_vectorized_avx2(load_incomplete_m256(&a[i], len - i), _b),
				&c[i], len - i);
		} else {
			_mm256_storeu_si256((void*)&c[i],
				gf256_mult_vectorized_avx2(_mm256_lddqu_si256((__m256i*)&a[i]), _b));
		}
	}
#else
	__m256i _a, _b;
	/* Load the constant byte b broadcasted in _b */
	_b = _mm256_set1_epi8(b);
	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			_a = load_incomplete_m256(&a[i], len - i);
			/* Vectorized multiplication in GF(256) */
			store_incomplete_m256(gf256_mult_vectorized_avx2(_a, _b), &c[i], len - i);
		} else {
			/* Obvious 512-bit */
			_a = _mm256_lddqu_si256((__m256i*)&a[i]);
			/* Vectorized multiplication in GF(256) */
			_mm256_storeu_si256((void*)&c[i], gf256_mult_vectorized_avx2(_a, _b));
		}
	}
#endif

	return;
}

/*
 * Vector to vector multiplication in GF(256).
 * Takes two vectors of length 'len', and returns a byte (element in GF(256))
 */
static inline uint8_t gf256_vect_mult_avx2(const uint8_t *a, const uint8_t *b, uint32_t len) {
	uint32_t i;
	__m256i accu, _a, _b;

	/* Set the accumulator to 0 */
	accu = _mm256_setzero_si256();

	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			_a = load_incomplete_m256(&a[i], len - i);
			_b = load_incomplete_m256(&b[i], len - i);
		} else {
			/* Obvious 256-bit */
			_a = _mm256_lddqu_si256((__m256i*)&a[i]);
			_b = _mm256_lddqu_si256((__m256i*)&b[i]);
		}
		accu ^= gf256_mult_vectorized_avx2(_a, _b);
	}

	return sum_uint8_avx2(accu);
}

/*
 * Vector to vector batched multiplication in GF(256).
 * Takes n+1 vectors a, b[0], ..., b[n-1] of length 'len'
 * Returns n bytes c[0], ..., c[n-1] (elements in GF(256))
 *   where c[i] is the vector to vector multiplication between a and b[i]
 * Assume that a is a non-sensitive vector
 */
static inline void gf256_vect_mult_multiple_public_avx2(uint8_t* const* c, const uint8_t *a, const uint8_t* const* b, uint32_t len, uint32_t n) {
	__m256i mul[GF_MULT_PANEL_BLOCKS][8];
	const uint32_t nblocks = (len + 31) / 32;
	uint32_t i, k;

	/* a is common to the n products, so its multiples are precomputed once and
	 * each product then costs a bit selection chain instead of a full generic
	 * multiplication. Vectors too long for the scratch area fall back to the
	 * generic path (this test folds away when len is a compile time constant). */
	if (nblocks > GF_MULT_PANEL_BLOCKS) {
		for (i = 0; i < n; i++) {
			*c[i] = gf256_vect_mult_avx2(a, b[i], len);
		}
		return;
	}

	for (k = 0; k < nblocks; k++) {
		uint32_t off = 32 * k;
		__m256i ab;
		if ((off + 32) <= len) {
			ab = _mm256_lddqu_si256((const __m256i*)(const void*)&a[off]);
		} else {
			/* Zero padded, which also zeroes the tail of the products */
			ab = load_incomplete_m256(&a[off], len - off);
		}
		gf256_precompute_multiples_avx2(ab, mul[k]);
	}

	for (i = 0; i < n; i++) {
		__m256i acc = _mm256_setzero_si256();
		for (k = 0; k < nblocks; k++) {
			uint32_t off = 32 * k;
			__m256i bb;
			if ((off + 32) <= len) {
				bb = _mm256_lddqu_si256((const __m256i*)(const void*)&b[i][off]);
			} else {
				bb = load_incomplete_m256(&b[i][off], len - off);
			}
			acc = gf256_mac_block_avx2(acc, bb, mul[k]);
		}
		*c[i] = sum_uint8_avx2(acc);
	}

	return;
}

/* Matrix and vector multiplication over GF(256)
 * C += A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
/* One panel pass of a GF(256) matrix by vector product: XOR into Y the
 * contribution of the columns [32 * p, 32 * p + 32 * np) of A, whose
 * multiples of X are already in mul[][]. When 'first' is set the result
 * overwrites Y instead of being accumulated into it. */
static inline void gf256_mat_mult_panel_avx2(const uint8_t *A, uint8_t *Y, uint32_t n,
                uint32_t p, uint32_t np, const __m256i mul[][8], int tri_inf, int first) {
	const uint32_t lo = 32 * p;
	uint32_t i, b;

	for (i = 0; i < n; i++) {
		uint32_t len = tri_inf ? (i + 1) : n;
		uint32_t hi, nfull, tail;
		__m256i acc0, acc1;
		uint8_t s;

		if (len <= lo) {
			/* TRI_INF: this row does not reach the current panel */
			continue;
		}
		hi = len;
		if (hi > (lo + (32 * np))) {
			hi = lo + (32 * np);
		}
		nfull = (hi - lo) / 32;
		tail  = (hi - lo) % 32;

		acc0 = _mm256_setzero_si256();
		acc1 = _mm256_setzero_si256();
		for (b = 0; b < nfull; b++) {
			size_t aoff = ((size_t)n * i) + lo + (32 * b);
			acc0 = gf256_mac_block_avx2(acc0, _mm256_lddqu_si256((const __m256i*)(const void*)&A[aoff]), mul[b]);
		}
		if (tail != 0) {
			size_t aoff = ((size_t)n * i) + lo + (32 * nfull);
			__m256i a;
			/* Reading the tail whole and masking it is only allowed while it
			 * stays inside the n x n matrix; the very last row falls back to
			 * the (slower) masked load. */
			if ((aoff + 32) <= ((size_t)n * n)) {
				a = _mm256_lddqu_si256((const __m256i*)(const void*)&A[aoff]) & gf_prefix_mask_avx2(tail);
			} else {
				a = load_incomplete_m256(&A[aoff], tail);
			}
			acc1 = gf256_mac_block_avx2(acc1, a, mul[nfull]);
		}

		s = sum_uint8_avx2(acc0 ^ acc1);
		if (first) {
			Y[i] = s;
		} else {
			Y[i] ^= s;
		}
	}

	return;
}

static inline void gf256_mat_mult_avx2(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	__m256i mul[GF_MULT_PANEL_BLOCKS][8];
	const uint32_t nblocks = (n + 31) / 32;
	uint32_t b, p;

	/* TRI_SUP starts each row at a different, non block aligned column, which
	 * does not fit the scheme below; it is unused by MQOM and is left on the
	 * generic path. */
	if (mtype == TRI_SUP) {
		GF256_MAT_MULT(A, X, Y, n, mtype, gf256_vect_mult_avx2);
		return;
	}

	/* X is walked by panels of at most GF_MULT_PANEL_BLOCKS blocks so that the
	 * precomputed multiples stay in a bounded scratch area. Every parameter set
	 * that reaches THIS function uses a single panel: only the gf16-fast sets have
	 * MQOM3_PARAM_EXT_FIELD == 8, hence FIELD_EXT_PREFIX == gf256, and their
	 * MQ_N of 56 / 84 / 116 gives nblocks = 2 / 3 / 4 <= GF_MULT_PANEL_BLOCKS.
	 * This says nothing about the gf256to2 matmuls further down, where an element
	 * is 2 bytes: there nblocks = ceil(2 * MQ_N / 32) reaches 20, and the panel
	 * loop really does iterate (10 of the 18 parameter sets). */
	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			uint32_t off = 32 * (p + b);
			__m256i xb;
			if ((off + 32) <= n) {
				xb = _mm256_lddqu_si256((const __m256i*)(const void*)&X[off]);
			} else {
				/* Zero padded: the multiples of the padding are zero too, so
				 * the trailing block can then be consumed whole. */
				xb = load_incomplete_m256(&X[off], n - off);
			}
			gf256_precompute_multiples_avx2(xb, mul[b]);
		}

		gf256_mat_mult_panel_avx2(A, Y, n, p, np, (const __m256i (*)[8])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256)
 */
static inline uint8_t gf2_gf256_vect_mult_avx2(const uint8_t *a_gf2, const uint8_t *b_gf256, uint32_t len) {
	uint32_t i;
	__m256i _a, _b;

	/* Set the accumulator to 0 */
	__m256i accu = _mm256_setzero_si256();

	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			uint32_t ceil_len = ((len - i) % 8 == 0) ? ((len - i) / 8) : (((len - i) / 8) + 1);
			_a = load_incomplete_m256(&a_gf2[i / 8], ceil_len);
			_b = load_incomplete_m256(&b_gf256[i], len - i);
		} else {
			/* Obvious 256-bit */
			_a = load_incomplete_m256(&a_gf2[i / 8], 4);
			_b = _mm256_lddqu_si256((__m256i*)&b_gf256[i]);
		}
		/* Create a selection mask from the bits in _a */
		const __m256i shuff_msk = _mm256_set_epi8(3, 3, 3,  3,  3, 3, 3, 3, 2, 2, 2,  2,  2, 2, 2, 2,
		                          1, 1, 1,  1,  1, 1, 1, 1, 0, 0, 0,  0,  0, 0, 0, 0);
		const __m256i and_msk = _mm256_set_epi8(-128, 64, 32, 16, 8, 4, 2, 1,
		                                        -128, 64, 32, 16, 8, 4, 2, 1,
		                                        -128, 64, 32, 16, 8, 4, 2, 1,
		                                        -128, 64, 32, 16, 8, 4, 2, 1);

		/* Copy in the two lanes */
		_a = _mm256_permute4x64_epi64(_a, 68);
		/* Only keep the selection bits */
		_a = _mm256_shuffle_epi8(_a, shuff_msk) & and_msk;
		/* Transform these bits to either 0 or 0xFF */
		_a = _mm256_cmpeq_epi8(_a, and_msk);
		/* Bytes selection */
		accu ^= (_a & _b);
	}
	return sum_uint8_avx2(accu);
}

/*
 * "Hybrid" multiplication of a constant in GF(2) and a vector in GF(256)
 */
static inline void gf2_gf256_constant_vect_mult_avx2(uint8_t a_gf2, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	gf2_gf256_constant_vect_mult_ref(a_gf2, b_gf256, c_gf256, n);

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(2)
 */
static inline void gf256_gf2_constant_vect_mult_avx2(uint8_t a_gf256, const uint8_t *b_gf2, uint8_t *c_gf256, uint32_t len) {
	uint32_t i;
	__m256i _a, _b;

	/* Broadcast the constant value */
	_a = _mm256_set1_epi8(a_gf256);

	for (i = 0; i < len; i += 32) {
		uint32_t ceil_len;
		/* Number of output GF(256) bytes for this iteration: bounded by the
		 * actual remaining length, not by the byte-rounded-up read size below
		 * (ceil_len is only valid for sizing the packed GF(2) read). */
		uint32_t out_len = ((len - i) < 32) ? (len - i) : 32;
		if ((len - i) < 32) {
			ceil_len = ((len - i) % 8 == 0) ? ((len - i) / 8) : (((len - i) / 8) + 1);
			_b = load_incomplete_m256(&b_gf2[i / 8], ceil_len);
		} else {
			ceil_len = 4;
			/* Obvious 256-bit */
			_b = load_incomplete_m256(&b_gf2[i / 8], ceil_len);
		}
		/* Create a selection mask from the bits in _a */
		const __m256i shuff_msk = _mm256_set_epi8(3, 3, 3,  3,  3, 3, 3, 3, 2, 2, 2,  2,  2, 2, 2, 2,
		                          1, 1, 1,  1,  1, 1, 1, 1, 0, 0, 0,  0,  0, 0, 0, 0);
		const __m256i and_msk = _mm256_set_epi8(-128, 64, 32, 16, 8, 4, 2, 1,
		                                        -128, 64, 32, 16, 8, 4, 2, 1,
		                                        -128, 64, 32, 16, 8, 4, 2, 1,
		                                        -128, 64, 32, 16, 8, 4, 2, 1);

		/* Copy in the two lanes */
		_b = _mm256_permute4x64_epi64(_b, 68);
		/* Only keep the selection bits */
		_b = _mm256_shuffle_epi8(_b, shuff_msk) & and_msk;
		/* Transform these bits to either 0 or 0xFF */
		_b = _mm256_cmpeq_epi8(_b, and_msk);
		/* Bytes selection */
		__m256i _c = (_a & _b);
		/* Store the result */
		store_incomplete_m256(_c, &c_gf256[i], out_len);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(2)
 */
static inline uint8_t gf256_gf2_vect_mult_avx2(const uint8_t *a_gf256, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256_vect_mult_avx2(b_gf2, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256), resulting
 *  in a vector in GF(256)
 */
static inline void gf2_gf256_mat_mult_avx2(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* NOTE: XXX: we force a REG here as it allows for better performance */
	(void)mtype;
	GF2_GF256_MAT_MULT(A, X, Y, n, REG, gf2_gf256_vect_mult_avx2);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(2), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf2_mat_mult_avx2(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* NOTE: XXX: we force a REG here as it allows for better performance */
	(void)mtype;
	GF256_GF2_MAT_MULT(A, X, Y, n, REG, gf256_gf2_vect_mult_avx2);
}

/* GF(256) matrix transposition */
static inline void gf256_mat_transpose_avx2(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	gf256_mat_transpose_ref(A, B, n, mtype);
}

/*
 * "Hybrid" multiplication of a constant in GF(16) and a vector in GF(256)
 */
static inline void gf16_gf256_constant_vect_mult_avx2(uint8_t a_gf16, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint8_t a_gf256;
	gf256_vect_lift_from_gf16_ref(&a_gf16, &a_gf256, 1);
	gf256_constant_vect_mult_avx2(a_gf256, b_gf256, c_gf256, n);
	return;
}

/* Nibble to byte lookup table implementing the GF(16) -> GF(256) lifting,
 * replicated in both 128-bit lanes so that it can be fed to a vpshufb. */
#define GF256_LIFT_FROM_GF16_LOOKUP_AVX2() \
	_mm256_set_epi8(12, 13, -20, -19, 81, 80, -79, -80, -68, -67, 92, 93, -31, -32, 1, 0, \
	                12, 13, -20, -19, 81, 80, -79, -80, -68, -67, 92, 93, -31, -32, 1, 0)

/* Expand the 32 packed nibbles held in the low 128 bits of 'packed' into 32
 * bytes, mapping each nibble through the byte lookup 'lut'.
 * With the plain lifting table this is the GF(16) -> GF(256) lifting; with a
 * table premultiplied by a constant this fuses the lifting and the constant
 * multiplication into a single pass. */
static inline __m256i gf256_expand_nibbles_avx2(__m256i packed, __m256i lut) {
	const __m256i shuff_msk_even = _mm256_set_epi8(-1, 15, -1,  14,  -1, 13, -1, 12, -1, 11, -1,  10,  -1, 9, -1, 8,
	                               -1, 7, -1,  6,  -1, 5, -1, 4, -1, 3, -1,  2,  -1, 1, -1, 0);
	const __m256i shuff_msk_odd  = _mm256_set_epi8(15, -1, 14, -1,  13,  -1, 12, -1, 11, -1, 10, -1,  9,  -1, 8, -1,
	                               7, -1, 6, -1,  5,  -1, 4, -1, 3, -1, 2, -1,  1,  -1, 0, -1);
	const __m256i nib_mask = _mm256_set1_epi8(0x0f);
	__m256i _a, _b, _c;

	/* Duplicate lanes */
	packed = _mm256_permute4x64_epi64(packed, 68);
	/* Isolate the nibbles in packed */
	_a = packed & nib_mask;
	_b = _mm256_srli_epi64(packed, 4) & nib_mask;
	/* Create the nibbles mix */
	_c = _mm256_shuffle_epi8(_a, shuff_msk_even) | _mm256_shuffle_epi8(_b, shuff_msk_odd);
	/* Lift: since we are on 16 bits, we can perform a vperm lookup inside the register */
	return _mm256_shuffle_epi8(lut, _c);
}

/* Vectorized lifting from GF(16) to GF(256) */
static inline void gf256_vect_lift_from_gf16_avx2(const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t len) {
	const __m256i lifting_lookup = GF256_LIFT_FROM_GF16_LOOKUP_AVX2();
	uint32_t i;
	__m256i _a_gf16, _c;

	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			_a_gf16 = load_incomplete_m256((const uint8_t*)&b_gf16[i / 2], (len - i + 1) / 2);
		} else {
			/* Obvious 256-bit */
			_a_gf16 = load_incomplete_m256((const uint8_t*)&b_gf16[i / 2], 16);
		}

		_c = gf256_expand_nibbles_avx2(_a_gf16, lifting_lookup);
		/* Store the result */
		if ((len - i) < 32) {
			store_incomplete_m256(_c, &c_gf256[i], (len - i));
		} else {
			_mm256_storeu_si256((__m256i*)&c_gf256[i], _c);
		}
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(16)
 */
static inline void gf256_gf16_constant_vect_mult_avx2(uint8_t a_gf256, const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t n) {
	/* The lifting and the multiplication by the constant are fused: the 16
	 * entry lifting table is premultiplied by a_gf256 once (a single generic
	 * vectorized multiplication), then one vpshufb per output block produces
	 * the lifted and multiplied bytes directly. This replaces one full
	 * double-and-add multiplication per 32 output bytes, plus a full extra
	 * pass over the output buffer.
	 * NOTE: the table content depends on the (possibly secret) constant, but
	 * the lookup indices are register operands of vpshufb, whose timing is
	 * data independent: no secret indexed memory access is introduced. */
	const __m256i lut = gf256_mult_vectorized_avx2(_mm256_set1_epi8((int8_t)a_gf256), GF256_LIFT_FROM_GF16_LOOKUP_AVX2());
	uint32_t i;
	__m256i _a_gf16, _c;

	for (i = 0; i < n; i += 32) {
		if ((n - i) < 32) {
			_a_gf16 = load_incomplete_m256((const uint8_t*)&b_gf16[i / 2], (n - i + 1) / 2);
		} else {
			/* Obvious 256-bit */
			_a_gf16 = load_incomplete_m256((const uint8_t*)&b_gf16[i / 2], 16);
		}

		_c = gf256_expand_nibbles_avx2(_a_gf16, lut);
		/* Store the result */
		if ((n - i) < 32) {
			store_incomplete_m256(_c, &c_gf256[i], (n - i));
		} else {
			_mm256_storeu_si256((__m256i*)&c_gf256[i], _c);
		}
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256)
 */
static inline uint8_t gf16_gf256_vect_mult_avx2(const uint8_t *a_gf16, const uint8_t *b_gf256, uint32_t len) {
	/* NOTE: the lifting is done fully in registers here. The previous version
	 * round-tripped the lifted block through the stack (the ymm locals were
	 * passed as byte pointers to the buffer based lifting), which forced a
	 * spill plus a store-to-load forwarding on every single block. */
	const __m256i lifting_lookup = GF256_LIFT_FROM_GF16_LOOKUP_AVX2();
	uint32_t i;
	__m256i accu, _a, _b;
	__m256i _a_gf16;

	/* Set the accumulator to 0 */
	accu = _mm256_setzero_si256();

	for (i = 0; i < len; i += 32) {
		if ((len - i) < 32) {
			_a_gf16 = load_incomplete_m256((const uint8_t*)&a_gf16[i / 2], (len - i + 1) / 2);
			/* Zero the lifted elements beyond the vector length: the last
			 * loaded byte may carry a nibble past the end of the vector. */
			_a = gf256_expand_nibbles_avx2(_a_gf16, lifting_lookup) & gf_prefix_mask_avx2(len - i);
			_b = load_incomplete_m256((const uint8_t*)&b_gf256[i], len - i);
		} else {
			/* Obvious 256-bit */
			_a_gf16 = load_incomplete_m256((const uint8_t*)&a_gf16[i / 2], 16);
			_a = gf256_expand_nibbles_avx2(_a_gf16, lifting_lookup);
			_b = _mm256_lddqu_si256((__m256i*)&b_gf256[i]);
		}
		/* Multiply in GF(256) */
		accu ^= gf256_mult_vectorized_avx2(_a, _b);
	}

	return sum_uint8_avx2(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(16)
 */
static inline uint8_t gf256_gf16_vect_mult_avx2(const uint8_t *a_gf256, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256_vect_mult_avx2(b_gf16, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(16), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf16_mat_mult_avx2(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* Same scheme as gf256_mat_mult_avx2(), except that the common vector X is
	 * lifted from GF(16) once for the whole matrix instead of once per row.
	 * NOTE: like GF256_GF16_MAT_MULT (i.e. GF256_GF2_MAT_MULT), TRI_SUP is
	 * handled as REG here. */
	__m256i mul[GF_MULT_PANEL_BLOCKS][8];
	const __m256i lifting_lookup = GF256_LIFT_FROM_GF16_LOOKUP_AVX2();
	const uint32_t nblocks = (n + 31) / 32;
	uint32_t b, p;

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			uint32_t off = 32 * (p + b);
			__m256i xb;
			if ((off + 32) <= n) {
				xb = gf256_expand_nibbles_avx2(load_incomplete_m256(&X[off / 2], 16), lifting_lookup);
			} else {
				xb = gf256_expand_nibbles_avx2(load_incomplete_m256(&X[off / 2], (n - off + 1) / 2), lifting_lookup);
				/* Zero pad past the end of the vector */
				xb &= gf_prefix_mask_avx2(n - off);
			}
			gf256_precompute_multiples_avx2(xb, mul[b]);
		}

		gf256_mat_mult_panel_avx2(A, Y, n, p, np, (const __m256i (*)[8])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}


/* === GF(256^2) === */
/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint16_t gf256to2_mult_avx2(uint16_t x, uint16_t y) {
	return gf256to2_mult_ref(x, y);
}

/* Vectorize multiplication of _a and _b in GF(256^2): the elements in the field are made of
 * 16 bits each in the lanes of the ymm */
static inline __m256i gf256to2_mult_vectorized_avx2(__m256i _a, __m256i _b) {
	const __m256i shuff_msk_crossed = _mm256_set_epi8(30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17,
	                                  14, 15, 12, 13, 10, 11, 8,  9,  6,  7,  4,  5,  2,  3,  0,  1);
	const __m256i shuff_msk1 = _mm256_set_epi8(30, 30, 28, 28, 26, 26, 24, 24, 22, 22, 20, 20, 18, 18, 16, 16,
	                           14, 14, 12, 12, 10, 10, 8,  8,  6,  6,  4,  4,  2,  2,  0,  0);
	const __m256i shuff_msk2 = _mm256_set_epi8(31, 31, 29, 29, 27, 27, 25, 25, 23, 23, 21, 21, 19, 19, 17, 17,
	                           15, 15, 13, 13, 11, 11, 9,  9,  7,  7,  5,  5,  3,  3,  1,  1);

	const __m256i const32 = _mm256_set_epi64x(0x0020002000200020, 0x0020002000200020, 0x0020002000200020, 0x0020002000200020);

	const __m256i mask_c1 = _mm256_set_epi64x(0x8000800080008000, 0x8000800080008000, 0x8000800080008000, 0x8000800080008000);
	const __m256i zero = _mm256_setzero_si256();

	__m256i ab = gf256_mult_vectorized_avx2(_a, _b);
	__m256i a0b0 = _mm256_shuffle_epi8(ab, shuff_msk1);
	__m256i a1b1 = _mm256_shuffle_epi8(ab, shuff_msk2);
	__m256i a1b1_32 = gf256_mult_vectorized_avx2(a1b1, const32);
	/* */
	__m256i a0_xor_a1 = _a ^ _mm256_shuffle_epi8(_a, shuff_msk_crossed);
	__m256i b0_xor_b1 = _mm256_blendv_epi8(zero, _b ^ _mm256_shuffle_epi8(_b, shuff_msk_crossed), mask_c1);
	__m256i mult_ab_xor = gf256_mult_vectorized_avx2(a0_xor_a1, b0_xor_b1);

	/* Compute the result */
	__m256i res = a0b0 ^ a1b1_32 ^ mult_ab_xor;

	return res;
}

/* Byte shuffle masks used to split a block of GF(256^2) elements into its two
 * GF(256) coordinates, each duplicated over the whole 16-bit lane. */
#define GF256TO2_DUP_COORD0_AVX2() \
	_mm256_set_epi8(30, 30, 28, 28, 26, 26, 24, 24, 22, 22, 20, 20, 18, 18, 16, 16, \
	                14, 14, 12, 12, 10, 10, 8,  8,  6,  6,  4,  4,  2,  2,  0,  0)
#define GF256TO2_DUP_COORD1_AVX2() \
	_mm256_set_epi8(31, 31, 29, 29, 27, 27, 25, 25, 23, 23, 21, 21, 19, 19, 17, 17, \
	                15, 15, 13, 13, 11, 11, 9,  9,  7,  7,  5,  5,  3,  3,  1,  1)

/* Multiply every GF(256^2) element of the block by the generator t (the root of
 * X^2 + X + 32). With b = b0 + b1 * t one has t * b = (32 * b1) + (b0 + b1) * t,
 * which is directly read off the reference c0 = a0b0 + 32 * a1b1 and
 * c1 = a0b0 + (a0 + a1)(b0 + b1) with a0 = 0 and a1 = 1. */
static inline __m256i gf256to2_mult_by_t_avx2(__m256i b) {
	const __m256i swap_msk = _mm256_set_epi8(30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17,
	                         14, 15, 12, 13, 10, 11, 8,  9,  6,  7,  4,  5,  2,  3,  0,  1);
	const __m256i dup1 = GF256TO2_DUP_COORD1_AVX2();
	const __m256i odd_byte = _mm256_set1_epi16((int16_t)0xFF00);
	__m256i sum, m32;
	uint32_t k;

	/* Both bytes of each lane hold b0 ^ b1 */
	sum = b ^ _mm256_shuffle_epi8(b, swap_msk);
	/* Both bytes of each lane hold b1, then 32 * b1 */
	m32 = _mm256_shuffle_epi8(b, dup1);
	for (k = 0; k < 5; k++) {
		m32 = gf256_xtime_avx2(m32);
	}

	/* Coordinate 0 (even byte) <- 32 * b1, coordinate 1 (odd byte) <- b0 ^ b1 */
	return _mm256_blendv_epi8(m32, sum, odd_byte);
}

/* Precompute the 16 multiples of a GF(256^2) block needed to turn a product by
 * an arbitrary element a = a0 + a1 * t into pure bit selections:
 *   a * X = a0 * X + a1 * (t * X)
 * where a0 and a1 are GF(256) scalars, so each half is a plain bytewise
 * GF(256) product handled by the 8 multiples of X (resp. of t * X).
 * NOTE: with GFNI the generic product is cheap enough that only the block
 * itself is kept. */
static inline void gf256to2_precompute_multiples_avx2(__m256i v, __m256i out[16]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	/* With GFNI only X and t * X are needed: a * X = a0 * X + a1 * (t * X)
	 * turns into two single instruction bytewise GF(256) products. */
	out[0] = v;
	out[1] = gf256to2_mult_by_t_avx2(v);
#else
	gf256_precompute_multiples_avx2(v, &out[0]);
	gf256_precompute_multiples_avx2(gf256to2_mult_by_t_avx2(v), &out[8]);
#endif

	return;
}

/* Multiply and accumulate in GF(256^2): acc ^= a * X, with mul[] holding the
 * precomputed multiples of the X block. */
static inline __m256i gf256to2_mac_block_avx2(__m256i acc, __m256i a, const __m256i mul[16]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	__m256i t0 = _mm256_gf2p8mul_epi8(_mm256_shuffle_epi8(a, GF256TO2_DUP_COORD0_AVX2()), mul[0]);
	__m256i t1 = _mm256_gf2p8mul_epi8(_mm256_shuffle_epi8(a, GF256TO2_DUP_COORD1_AVX2()), mul[1]);

	return acc ^ t0 ^ t1;
#else
	const __m256i zero = _mm256_setzero_si256();
	__m256i t0, t1;

	t0 = gf256_mac_block_avx2(zero, _mm256_shuffle_epi8(a, GF256TO2_DUP_COORD0_AVX2()), &mul[0]);
	t1 = gf256_mac_block_avx2(zero, _mm256_shuffle_epi8(a, GF256TO2_DUP_COORD1_AVX2()), &mul[8]);

	return acc ^ t0 ^ t1;
#endif
}

/* One panel pass of a GF(256^2) matrix by vector product, see
 * gf256_mat_mult_panel_avx2() for the scheme. Everything is counted in bytes
 * here, an element being 2 bytes wide. */
static inline void gf256to2_mat_mult_panel_avx2(const uint8_t *A, uint16_t *Y, uint32_t n,
                uint32_t p, uint32_t np, const __m256i mul[][16], int tri_inf, int first) {
	const uint32_t lo = 32 * p;
	const size_t nbytes = (size_t)2 * n;
	uint32_t i, b;

	for (i = 0; i < n; i++) {
		uint32_t len = 2 * (tri_inf ? (i + 1) : n);
		uint32_t hi, nfull, tail;
		__m256i acc0, acc1;
		uint16_t s;

		if (len <= lo) {
			/* TRI_INF: this row does not reach the current panel */
			continue;
		}
		hi = len;
		if (hi > (lo + (32 * np))) {
			hi = lo + (32 * np);
		}
		nfull = (hi - lo) / 32;
		tail  = (hi - lo) % 32;

		acc0 = _mm256_setzero_si256();
		acc1 = _mm256_setzero_si256();
		for (b = 0; b < nfull; b++) {
			size_t aoff = (nbytes * i) + lo + (32 * b);
			acc0 = gf256to2_mac_block_avx2(acc0, _mm256_lddqu_si256((const __m256i*)(const void*)&A[aoff]), mul[b]);
		}
		if (tail != 0) {
			size_t aoff = (nbytes * i) + lo + (32 * nfull);
			__m256i a;
			if ((aoff + 32) <= (nbytes * n)) {
				a = _mm256_lddqu_si256((const __m256i*)(const void*)&A[aoff]) & gf_prefix_mask_avx2(tail);
			} else {
				a = load_incomplete_m256(&A[aoff], tail);
			}
			acc1 = gf256to2_mac_block_avx2(acc1, a, mul[nfull]);
		}

		s = sum_uint16_avx2(acc0 ^ acc1);
		if (first) {
			Y[i] = s;
		} else {
			Y[i] ^= s;
		}
	}

	return;
}

/* Same panel pass as gf256to2_mat_mult_panel_avx2(), but for the case where
 * the common vector lies in the GF(256) subfield (each of its elements being
 * duplicated over both bytes of a 16-bit lane): the per element product then
 * degenerates into a plain bytewise GF(256) product driven by 8 multiples. */
static inline void gf256_mat_mult_panel_gf256to2_avx2(const uint8_t *A, uint16_t *Y, uint32_t n,
                uint32_t p, uint32_t np, const __m256i mul[][8], int tri_inf, int first) {
	const uint32_t lo = 32 * p;
	const size_t nbytes = (size_t)2 * n;
	uint32_t i, b;

	for (i = 0; i < n; i++) {
		uint32_t len = 2 * (tri_inf ? (i + 1) : n);
		uint32_t hi, nfull, tail;
		__m256i acc0, acc1;
		uint16_t s;

		if (len <= lo) {
			continue;
		}
		hi = len;
		if (hi > (lo + (32 * np))) {
			hi = lo + (32 * np);
		}
		nfull = (hi - lo) / 32;
		tail  = (hi - lo) % 32;

		acc0 = _mm256_setzero_si256();
		acc1 = _mm256_setzero_si256();
		for (b = 0; b < nfull; b++) {
			size_t aoff = (nbytes * i) + lo + (32 * b);
			acc0 = gf256_mac_block_avx2(acc0, _mm256_lddqu_si256((const __m256i*)(const void*)&A[aoff]), mul[b]);
		}
		if (tail != 0) {
			size_t aoff = (nbytes * i) + lo + (32 * nfull);
			__m256i a;
			if ((aoff + 32) <= (nbytes * n)) {
				a = _mm256_lddqu_si256((const __m256i*)(const void*)&A[aoff]) & gf_prefix_mask_avx2(tail);
			} else {
				a = load_incomplete_m256(&A[aoff], tail);
			}
			acc1 = gf256_mac_block_avx2(acc1, a, mul[nfull]);
		}

		s = sum_uint16_avx2(acc0 ^ acc1);
		if (first) {
			Y[i] = s;
		} else {
			Y[i] ^= s;
		}
	}

	return;
}

/*
 * Vector multiplied by a constant in GF(256^2).
 */
static inline void gf256to2_constant_vect_mult_avx2(uint16_t b, const uint16_t *a, uint16_t *c, uint32_t len) {
	/* The constant is the same for every element, so its multiples are
	 * precomputed once and each block then only costs two bit selection
	 * chains instead of three full generic GF(256) multiplications. */
	__m256i mul[16];
	uint32_t i;
	__m256i _a;

	gf256to2_precompute_multiples_avx2(_mm256_set1_epi16((int16_t)b), mul);

	for (i = 0; i < (2 * len); i += 32) {
		if (((2 * len) - i) < 32) {
			_a = load_incomplete_m256((const uint8_t*)&a[i / 2], ((2 * len) - i));
			/* Vectorized multiplication in GF(256^2) */
			store_incomplete_m256(gf256to2_mac_block_avx2(_mm256_setzero_si256(), _a, mul), (uint8_t*)&c[i / 2], (2 * len) - i);
		} else {
			/* Obvious 512-bit */
			_a = _mm256_lddqu_si256((__m256i*)&a[i / 2]);
			/* Vectorized multiplication in GF(256^2) */
			_mm256_storeu_si256((__m256i*)&c[i / 2], gf256to2_mac_block_avx2(_mm256_setzero_si256(), _a, mul));
		}
	}

	return;
}

/* Perform a multiplication in GF(256^2) of elements in vectors a an b */
static inline uint16_t gf256to2_vect_mult_avx2(const uint16_t *a, const uint16_t *b, uint32_t len) {
	uint32_t i;
	__m256i accu, _a, _b;

	/* Set the accumulator to 0 */
	accu = _mm256_setzero_si256();

	for (i = 0; i < (2 * len); i += 32) {
		if (((2 * len) - i) < 32) {
			_a = load_incomplete_m256((const uint8_t*)&a[i / 2], ((2 * len) - i));
			_b = load_incomplete_m256((const uint8_t*)&b[i / 2], ((2 * len) - i));
		} else {
			/* Obvious 256-bit */
			_a = _mm256_lddqu_si256((__m256i*)&a[i / 2]);
			_b = _mm256_lddqu_si256((__m256i*)&b[i / 2]);
		}
		accu ^= gf256to2_mult_vectorized_avx2(_a, _b);
	}

	return sum_uint16_avx2(accu);
}

/*
 * Vector to vector batched multiplication in GF(256^2).
 * Takes n+1 vectors a, b[0], ..., b[n-1] of length 'len'
 * Returns n double-bytes c[0], ..., c[n-1] (elements in GF(256^2))
 *   where c[i] is the vector to vector multiplication between a and b[i]
 * Assume that a is a non-sensitive vector
 */
static inline void gf256to2_vect_mult_multiple_public_avx2(uint16_t* const* c, const uint16_t *a, const uint16_t* const* b, uint32_t len, uint32_t n) {
	__m256i mul[GF_MULT_PANEL_BLOCKS][16];
	const uint8_t *ab_ptr = (const uint8_t*)(const void*)a;
	const uint32_t nbytes = 2 * len;
	const uint32_t nblocks = (nbytes + 31) / 32;
	uint32_t i, k;

	/* Same idea as gf256_vect_mult_multiple_public_avx2() */
	if (nblocks > GF_MULT_PANEL_BLOCKS) {
		for (i = 0; i < n; i++) {
			*c[i] = gf256to2_vect_mult_avx2(a, b[i], len);
		}
		return;
	}

	for (k = 0; k < nblocks; k++) {
		uint32_t off = 32 * k;
		__m256i av;
		if ((off + 32) <= nbytes) {
			av = _mm256_lddqu_si256((const __m256i*)(const void*)&ab_ptr[off]);
		} else {
			av = load_incomplete_m256(&ab_ptr[off], nbytes - off);
		}
		gf256to2_precompute_multiples_avx2(av, mul[k]);
	}

	for (i = 0; i < n; i++) {
		const uint8_t *bb_ptr = (const uint8_t*)(const void*)b[i];
		__m256i acc = _mm256_setzero_si256();
		for (k = 0; k < nblocks; k++) {
			uint32_t off = 32 * k;
			__m256i bv;
			if ((off + 32) <= nbytes) {
				bv = _mm256_lddqu_si256((const __m256i*)(const void*)&bb_ptr[off]);
			} else {
				bv = load_incomplete_m256(&bb_ptr[off], nbytes - off);
			}
			acc = gf256to2_mac_block_avx2(acc, bv, mul[k]);
		}
		*c[i] = sum_uint16_avx2(acc);
	}

	return;
}

/*
 * GF(2^16) matrix multiplication
 */
static inline void gf256to2_mat_mult_avx2(const uint16_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	__m256i mul[GF_MULT_PANEL_BLOCKS][16];
	const uint8_t *Ab = (const uint8_t*)(const void*)A;
	const uint8_t *Xb = (const uint8_t*)(const void*)X;
	const uint32_t nbytes = 2 * n;
	const uint32_t nblocks = (nbytes + 31) / 32;
	uint32_t b, p;

	/* TRI_SUP starts each row at a different, non block aligned column, which
	 * does not fit the scheme below; it is unused by MQOM and is left on the
	 * generic path. */
	if (mtype == TRI_SUP) {
		GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256to2_vect_mult_avx2);
		return;
	}

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			uint32_t off = 32 * (p + b);
			__m256i xb;
			if ((off + 32) <= nbytes) {
				xb = _mm256_lddqu_si256((const __m256i*)(const void*)&Xb[off]);
			} else {
				/* Zero padded: the multiples of the padding are zero too */
				xb = load_incomplete_m256(&Xb[off], nbytes - off);
			}
			gf256to2_precompute_multiples_avx2(xb, mul[b]);
		}

		gf256to2_mat_mult_panel_avx2(Ab, Y, n, p, np, (const __m256i (*)[16])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(2) and a vector in GF(256^2)
 */
static inline void gf2_gf256to2_constant_vect_mult_avx2(uint8_t a_gf2, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	gf2_gf256to2_constant_vect_mult_ref(a_gf2, b_gf256to2, c_gf256to2, n);

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(2)
 */
static inline void gf256to2_gf2_constant_vect_mult_avx2(uint16_t a_gf256to2, const uint8_t *b_gf2, uint16_t *c_gf256to2, uint32_t len) {
	uint32_t i;
	__m256i _a, _b;

	/* Broadcast the constant value */
	_a = _mm256_set1_epi16(a_gf256to2);

	for (i = 0; i < (2 * len); i += 32) {
		uint32_t ceil_len;
		/* Number of output bytes for this iteration: bounded by the actual
		 * remaining length, not by the byte-rounded-up read size below
		 * (ceil_len is only valid for sizing the packed GF(2) read). */
		uint32_t out_len = (((2 * len) - i) < 32) ? ((2 * len) - i) : 32;
		if (((2 * len) - i) < 32) {
			ceil_len = (((2 * len) - i) % 16 == 0) ? (((2 * len) - i) / 16) : ((((2 * len) - i) / 16) + 1);
			_b = load_incomplete_m256(&b_gf2[i / 16], ceil_len);
		} else {
			/* Obvious 256-bit */
			ceil_len = 2;
			_b = load_incomplete_m256(&b_gf2[i / 16], ceil_len);
		}
		/* Create a selection mask from the bits in _a */
		const __m256i shuff_msk = _mm256_set_epi8(1, 1, 1,  1,  1, 1, 1, 1, 1, 1, 1,  1,  1, 1, 1, 1,
		                          0, 0, 0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0, 0, 0, 0);
		const __m256i and_msk = _mm256_set_epi8(-128, -128, 64, 64, 32, 32, 16, 16,
		                                        8, 8, 4, 4, 2, 2, 1, 1,
		                                        -128, -128, 64, 64, 32, 32, 16, 16,
		                                        8, 8, 4, 4, 2, 2, 1, 1);

		/* Only keep the selection bits */
		_b = _mm256_permute4x64_epi64(_b, 68);
		_b = _mm256_shuffle_epi8(_b, shuff_msk);
		_b = _b & and_msk;
		/* Transform these bits to either 0 or 0xFF */
		_b = _mm256_cmpeq_epi8(_b, and_msk);
		/* Bytes selection */
		__m256i _c = (_a & _b);
		/* Store the result */
		store_incomplete_m256(_c, (uint8_t*)&c_gf256to2[i / 2], out_len);
	}
	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256) and a vector in GF(256^2)
 * Helper for gf16_gf256to2_constant_vect_mult.
 */
static inline void gf256_gf256to2_constant_vect_mult_avx2(uint8_t a_gf256, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	gf256_gf256to2_constant_vect_mult_ref(a_gf256, b_gf256to2, c_gf256to2, n);
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(256)
 * Helper for gf256to2_gf16_constant_vect_mult.
 */
static inline void gf256to2_gf256_constant_vect_mult_avx2(uint16_t a_gf256to2, const uint8_t *b_gf256, uint16_t *c_gf256to2, uint32_t len) {
	/* Every element of b_gf256 is a GF(256) scalar, so this is a plain
	 * bytewise GF(256) product against the broadcast constant: its multiples
	 * are precomputed once. */
	__m256i mul[8];
	uint32_t i;
	__m256i _b;
	const __m256i shuff_msk = _mm256_set_epi8(15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,
	                          7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2,  2,  1, 1, 0, 0);

	gf256_precompute_multiples_avx2(_mm256_set1_epi16((int16_t)a_gf256to2), mul);
	for (i = 0; i < len; i += 16) {
		uint32_t to_load = (len - i) < 16 ? (len - i) : 16;
		_b = load_incomplete_m256((const uint8_t*)&b_gf256[i], to_load);
		/* Copy in the two lanes */
		_b = _mm256_permute4x64_epi64(_b, 68);
		/* Duplicate elements in the register */
		_b = _mm256_shuffle_epi8(_b, shuff_msk);
		/* Vectorized multiplication in GF(256) */
		store_incomplete_m256(gf256_mac_block_avx2(_mm256_setzero_si256(), _b, mul), (uint8_t*)&c_gf256to2[i], 2 * to_load);
	}
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256^2)
 */
static inline uint16_t gf2_gf256to2_vect_mult_avx2(const uint8_t *a_gf2, const uint16_t *b_gf256to2, uint32_t len) {
	uint32_t i;
	__m256i _a, _b;

	/* Set the accumulator to 0 */
	__m256i accu = _mm256_setzero_si256();

	for (i = 0; i < (2 * len); i += 32) {
		if (((2 * len) - i) < 32) {
			uint32_t ceil_len = (((2 * len) - i) % 16 == 0) ? (((2 * len) - i) / 16) : ((((2 * len) - i) / 16) + 1);
			_a = load_incomplete_m256(&a_gf2[i / 16], ceil_len);
			_b = load_incomplete_m256((const uint8_t*)&b_gf256to2[i / 2], (2 * len) - i);
		} else {
			/* Obvious 256-bit */
			_a = load_incomplete_m256(&a_gf2[i / 16], 2);
			_b = _mm256_lddqu_si256((__m256i*)&b_gf256to2[i / 2]);
		}
		/* Create a selection mask from the bits in _a */
		const __m256i shuff_msk = _mm256_set_epi8(1, 1, 1,  1,  1, 1, 1, 1, 1, 1, 1,  1,  1, 1, 1, 1,
		                          0, 0, 0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0, 0, 0, 0);
		const __m256i and_msk = _mm256_set_epi8(-128, -128, 64, 64, 32, 32, 16, 16,
		                                        8, 8, 4, 4, 2, 2, 1, 1,
		                                        -128, -128, 64, 64, 32, 32, 16, 16,
		                                        8, 8, 4, 4, 2, 2, 1, 1);
		/* Copy in the two lanes */
		_a = _mm256_permute4x64_epi64(_a, 68);
		/* Only keep the selection bits */
		_a = _mm256_shuffle_epi8(_a, shuff_msk) & and_msk;
		/* Transform these bits to either 0 or 0xFF */
		_a = _mm256_cmpeq_epi8(_a, and_msk);
		/* Bytes selection */
		accu ^= (_a & _b);
	}
	return sum_uint16_avx2(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(256)
 */
static inline uint16_t gf256to2_gf2_vect_mult_avx2(const uint16_t *a_gf256to2, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256to2_vect_mult_avx2(b_gf2, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256^2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf2_gf256to2_mat_mult_avx2(const uint8_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF2_GF256to2_MAT_MULT(A, X, Y, n, mtype, gf2_gf256to2_vect_mult_avx2);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf2_mat_mult_avx2(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_GF2_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf2_vect_mult_avx2);
}

/* GF(256^2) matrix transposition */
static inline void gf256to2_mat_transpose_avx2(const uint16_t *A, uint16_t *B, uint32_t n, matrix_type mtype) {
	gf256to2_mat_transpose_ref(A, B, n, mtype);
}

/*
 * "Hybrid" constant multiplication of a constant in GF(16) and a vector in GF(256^2)
 */
static inline void gf16_gf256to2_constant_vect_mult_avx2(uint8_t a_gf16, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint8_t a_gf256;
	gf256_vect_lift_from_gf16_ref(&a_gf16, &a_gf256, 1);
	gf256_gf256to2_constant_vect_mult_avx2(a_gf256, b_gf256to2, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(16)
 */
static inline void gf256to2_gf16_constant_vect_mult_avx2(uint16_t a_gf256to2, const uint8_t *b_gf16, uint16_t *c_gf256to2, uint32_t n) {
	uint8_t *buf = ((uint8_t*) c_gf256to2) + n;
	gf256_vect_lift_from_gf16_avx2(b_gf16, buf, n);
	gf256to2_gf256_constant_vect_mult_avx2(a_gf256to2, buf, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256^2)
 */
static inline uint16_t gf16_gf256to2_vect_mult_avx2(const uint8_t *a_gf16, const uint16_t *b_gf256to2, uint32_t len) {
	uint32_t i;
	__m256i accu, _a, _b;
	__m256i _a_gf16;

	/* Set the accumulator to 0 */
	accu = _mm256_setzero_si256();

	for (i = 0; i < (2 * len); i += 32) {
		const __m256i shuff_msk = _mm256_set_epi8(15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,
		                          7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2,  2,  1, 1, 0, 0);
		/* NOTE: the lifting is kept fully in registers here; the previous
		 * version round-tripped it through the stack on every block. */
		if (((2 * len) - i) < 32) {
			_a_gf16 = load_incomplete_m256((const uint8_t*)&a_gf16[i / 4], (((2 * len) - i) + 3) / 4);
			_a = gf256_expand_nibbles_avx2(_a_gf16, GF256_LIFT_FROM_GF16_LOOKUP_AVX2());
			/* Zero the lifted elements beyond the vector length */
			_a &= gf_prefix_mask_avx2(((2 * len) - i + 1) / 2);
			_b = load_incomplete_m256((const uint8_t*)&b_gf256to2[i / 2], (2 * len) - i);
		} else {
			/* Obvious 256-bit */
			_a_gf16 = load_incomplete_m256((const uint8_t*)&a_gf16[i / 4], 8);
			_a = gf256_expand_nibbles_avx2(_a_gf16, GF256_LIFT_FROM_GF16_LOOKUP_AVX2());
			_b = _mm256_lddqu_si256((__m256i*)&b_gf256to2[i / 2]);
		}
		/* Duplicate the values in _a */
		_a = _mm256_permute4x64_epi64(_a, 68);
		_a = _mm256_shuffle_epi8(_a, shuff_msk);
		/* Multiply in GF(256) */
		accu ^= gf256_mult_vectorized_avx2(_a, _b);
	}

	return sum_uint16_avx2(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(16)
 */
static inline uint16_t gf256to2_gf16_vect_mult_avx2(const uint16_t *a_gf256to2, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256to2_vect_mult_avx2(b_gf16, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(16), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf16_mat_mult_avx2(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	/* Each element of X is a GF(16) value lifted into the GF(256) subfield, so
	 * the product against a GF(256^2) row element is a plain bytewise GF(256)
	 * product: only the 8 multiples of the lifted-and-duplicated X block are
	 * needed here (versus 16 for a full GF(256^2) common vector).
	 * NOTE: like GF256to2_GF16_MAT_MULT (i.e. GF256_GF2_MAT_MULT), TRI_SUP is
	 * handled as REG here. */
	__m256i mul[GF_MULT_PANEL_BLOCKS][8];
	const __m256i dup_msk = _mm256_set_epi8(15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,
	                                        7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2,  2,  1, 1, 0, 0);
	const uint8_t *Ab = (const uint8_t*)(const void*)A;
	const uint32_t nbytes = 2 * n;
	const uint32_t nblocks = (nbytes + 31) / 32;
	uint32_t b, p;

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			/* A 32-byte block covers 16 GF(256^2) elements, hence 16 GF(16)
			 * elements of X packed into 8 bytes. */
			uint32_t off = 32 * (p + b);
			uint32_t nelts = (nbytes - off) / 2;
			__m256i xb;
			if (nelts > 16) {
				nelts = 16;
			}
			xb = gf256_expand_nibbles_avx2(load_incomplete_m256(&X[off / 4], (nelts + 1) / 2),
			                               GF256_LIFT_FROM_GF16_LOOKUP_AVX2());
			if (nelts < 16) {
				/* Zero pad past the end of the vector */
				xb &= gf_prefix_mask_avx2(nelts);
			}
			/* Duplicate each lifted byte over its 16-bit lane */
			xb = _mm256_shuffle_epi8(_mm256_permute4x64_epi64(xb, 68), dup_msk);
			gf256_precompute_multiples_avx2(xb, mul[b]);
		}

		gf256_mat_mult_panel_gf256to2_avx2(Ab, Y, n, p, np, (const __m256i (*)[8])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}

#endif /* __AVX2__ */

#endif /* __FIELDS_AVX2_H__ */
