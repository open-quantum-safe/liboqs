#ifndef __FIELDS_AVX512_H__
#define __FIELDS_AVX512_H__

/* Check if AVX512 is supported */
#if defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512VL__) && defined(__AVX512VPOPCNTDQ__) && defined(__AVX512VBMI__)

#include "fields_common.h"
#include "fields_ref.h"
/* Needed for memcpy */
#include <string.h>
/* Needed for AVX512 assembly intrinsics */
#include <immintrin.h>

/* This helper tries to efficiently copy len bytes in the ymm register */
static inline __mmask64 mask64_from_m128(__m128i v) {
	uint64_t r;
	_mm_storel_epi64((__m128i*)&r, v);
	return (__mmask64)r;
}

static inline __m512i load_incomplete_m512(const uint8_t *a, uint32_t len) {
	const __m512i zero = _mm512_setzero_epi32();

	/* A zmm register holds 64 bytes, so clamp instead of trusting callers */
	if (len > 64) {
		len = 64;
	}

	if (len % 4) {
		/* In this case, we switch to the slower _mm512_mask_loadu_epi8 */
		__mmask64 mask = ((__mmask64)1 << len) - 1;
		return _mm512_mask_loadu_epi8(zero, mask, (int const*)a);
	} else {
		/* Optimize with faster instruction */
		__mmask16 mask = (__mmask16)((1u << (len / 4)) - 1u);
		return _mm512_mask_loadu_epi32(zero, mask, (int const*)a);
	}
}

/* This helper tries to efficiently copy len bytes from the ymm register */
static inline void store_incomplete_m512(__m512i in, uint8_t *a, uint32_t len) {
	/* Same clamp as load_incomplete_m512 above */
	if (len > 64) {
		len = 64;
	}

	if (len % 4) {
		/* In this case, we switch to the slower _mm512_mask_storeu_epi8 */
		__mmask64 mask = ((__mmask64)1 << len) - 1;
		_mm512_mask_storeu_epi8((void*)a, mask, in);
	} else {
		/* Optimize with faster instruction */
		__mmask16 mask = (__mmask16)((1u << (len / 4)) - 1u);
		_mm512_mask_storeu_epi32((void*)a, mask, in);
	}
}

/* Fold a zmm accumulator down to a single 128-bit register by XORing the four
 * lanes: shared prologue of all the horizontal XOR reductions below.
 */
static inline __m128i fold_lanes_avx512(__m512i v) {
	__m256i t = _mm256_xor_si256(_mm512_castsi512_si256(v), _mm512_extracti64x4_epi64(v, 1));

	return _mm_xor_si128(_mm256_castsi256_si128(t), _mm256_extracti128_si256(t, 1));
}

static inline uint8_t parity_avx512(__m512i accu) {
	/* popcount(a ^ b) and popcount(a) + popcount(b) agree modulo 2, so the
	 * lane folding preserves the parity of the whole register. */
	__m128i t = fold_lanes_avx512(accu);
#if defined(__x86_64__)
	uint64_t a = ((uint64_t)_mm_cvtsi128_si64(t)) ^ ((uint64_t)_mm_extract_epi64(t, 1));

	return (uint8_t)(_mm_popcnt_u64(a) & 1);
#else
	/* 32-bit x86: same substitution as parity_avx2 in fields_avx2.h - the 64-bit
	 * extraction intrinsics need a 64-bit GPR, so fold to 32 bits instead. The
	 * XOR fold preserves parity whatever the width. */
	uint32_t a = ((uint32_t)_mm_cvtsi128_si32(t))
	           ^ ((uint32_t)_mm_extract_epi32(t, 1))
	           ^ ((uint32_t)_mm_extract_epi32(t, 2))
	           ^ ((uint32_t)_mm_extract_epi32(t, 3));

	return (uint8_t)(_mm_popcnt_u32(a) & 1);
#endif
}

static inline uint8_t sum_uint8_avx512(__m512i accu) {
	__m128i t = fold_lanes_avx512(accu);

	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0x4E));
	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0xB1));
	t = _mm_xor_si128(t, _mm_srli_epi32(t, 16));
	t = _mm_xor_si128(t, _mm_srli_epi32(t, 8));

	return (uint8_t)_mm_cvtsi128_si32(t);
}

static inline uint16_t sum_uint16_avx512(__m512i accu) {
	__m128i t = fold_lanes_avx512(accu);

	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0x4E));
	t = _mm_xor_si128(t, _mm_shuffle_epi32(t, 0xB1));
	t = _mm_xor_si128(t, _mm_srli_epi32(t, 16));

	return (uint16_t)_mm_cvtsi128_si32(t);
}

/* === GF(2) === */
/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint8_t gf2_mult_avx512(uint8_t a, uint8_t b) {
	return gf2_mult_ref(a, b);
}

/*
 * Vector multiplied by a constant in GF(2).
 */
static inline void gf2_constant_vect_mult_avx512(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	gf2_constant_vect_mult_ref(b, a, c, len);

	return;
}

/* XXX: TODO: This is not optimal, since we hardly fill our zmm register to its full capacity */
static inline uint8_t gf2_vect_mult_avx512(const uint8_t *a, const uint8_t *b, uint32_t len_bits) {
	uint32_t i;
	__m512i accu, _a, _b;
	uint32_t len = (len_bits / 8);

	/* Set the accumulator to 0 */
	accu = _mm512_setzero_epi32();

	for (i = 0; i < len; i += 64) {
		if ((len - i) < 64) {
			_a = load_incomplete_m512(&a[i], len - i);
			_b = load_incomplete_m512(&b[i], len - i);
		} else {
			/* Obvious 512-bit */
			_a = _mm512_loadu_epi64((__m512i*)&a[i]);
			_b = _mm512_loadu_epi64((__m512i*)&b[i]);
		}
		/* Vectorized AND of inputs and then XOR with the accumulator */
		accu ^= (_a & _b);
	}

	return parity_avx512(accu);
}

/* Matrix and vector multiplication over GF(2)
 * C = A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
/* XXX: TODO: this can be optimized by packing rows in zmm when n <= 256 */
static inline void gf2_mat_mult_avx512(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF2_MAT_MULT(A, X, Y, n, mtype, gf2_vect_mult_avx512);
}

/* GF(2) matrix transposition */
static inline void gf2_mat_transpose_avx512(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	gf2_mat_transpose_ref(A, B, n, mtype);
}

/* === GF(256) === */
/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint8_t gf256_mult_avx512(uint8_t x, uint8_t y) {
	return gf256_mult_ref(x, y);
}

static inline __m512i gf256_mult_vectorized_avx512(__m512i _a, __m512i _b) {
	/* NOTE: when GFNI is detected, we use the accelerated GF(256) Rijndael instruction */
#if defined(__GFNI__) && !defined(NO_GFNI)
	return _mm512_gf2p8mul_epi8(_a, _b);
#else
	/* NOTE: because we do not have a blending based on AVX512, and given the fact that
	 * 64 bits mask extraction is a bit slow, and also given the fact that when using triangular matrices
	 * we hit more alignment issues with AVX512, we prefer to fallback to AVX2 on each half of the AVX512
	 * The "pure" AVX512 implementation is left in comment below for information, but it is less efficient
	 * than the AVX2 fallback (at least on the tested AMD Zen 4 platform) */
	__m256i a_low  = _mm512_castsi512_si256(_a);
	__m256i b_low  = _mm512_castsi512_si256(_b);
	__m256i a_high = _mm512_extracti64x4_epi64(_a, 1);
	__m256i b_high = _mm512_extracti64x4_epi64(_b, 1);

	uint32_t j;
	__m256i mask_lsb_low, mask_lsb_high, tmp_low, tmp_high;
	const __m256i red_poly = _mm256_set_epi64x(0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B);
	const __m256i zero     = _mm256_setzero_si256();
	__m256i accu_low = _mm256_setzero_si256();
	__m256i accu_high = _mm256_setzero_si256();

	/* Compute the vectorized multiplication in GF(256) */
	for (j = 0; j < 8; j++) {
		mask_lsb_low  = _mm256_slli_epi64(b_low, 7 - j);
		mask_lsb_high = _mm256_slli_epi64(b_high, 7 - j);
		accu_low  ^= _mm256_blendv_epi8(zero, a_low, mask_lsb_low);
		accu_high ^= _mm256_blendv_epi8(zero, a_high, mask_lsb_high);
		tmp_low  = _mm256_add_epi8(a_low, a_low);
		tmp_high = _mm256_add_epi8(a_high, a_high);
		a_low  = _mm256_blendv_epi8(zero, red_poly, a_low)  ^ tmp_low;
		a_high = _mm256_blendv_epi8(zero, red_poly, a_high) ^ tmp_high;
	}

	__m512i _accu_low = _mm512_castsi256_si512(accu_low);
	__m512i accu = _mm512_inserti64x4(_accu_low, accu_high, 1);

	return accu;
#if 0
	/* Our reduction polynomial */
	const __m512i red_poly = _mm512_set_epi64(0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B,
	                         0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B, 0x1B1B1B1B1B1B1B1B);
	const __m512i zero     = _mm512_setzero_epi32();
	__m512i accu = _mm512_setzero_epi32();

	uint32_t j;
	__mmask64 mask;
	__m512i tmp;

	/* Compute the vectorized multiplication in GF(256) */
	for (j = 0; j < 8; j++) {
		mask = _mm512_movepi8_mask(_mm512_slli_epi64(_b, 7 - j));
		accu ^= _mm512_mask_blend_epi8(mask, zero, _a);
		tmp  = _mm512_add_epi8(_a, _a);
		mask = _mm512_movepi8_mask(_a);
		_a = _mm512_mask_blend_epi8(mask, zero, red_poly) ^ tmp;
	}
	return accu;
#endif
#endif
}

/* ============================================================================
 * Matrix by vector products: shared "precomputed multiples" machinery, same
 * scheme as the AVX2 backend. In A * X the vector X is common to every row, so
 * the 8 multiples X * 2^k (k = 0..7) of each 64-byte block of X are computed
 * once and a row chunk then only costs one masked select plus one XOR per bit
 * of the matrix coefficients, instead of a full double-and-add per chunk.
 * ============================================================================
 */

#ifndef GF_MULT_PANEL_BLOCKS
#define GF_MULT_PANEL_BLOCKS 4
#endif
/* See the same guard in fields_avx2.h: 0 means a zero-length mul[] and a panel
 * loop that never advances. */
#if GF_MULT_PANEL_BLOCKS < 1
#error "GF_MULT_PANEL_BLOCKS must be at least 1"
#endif

/* Doubling in GF(256) (multiplication by the element 2) of every byte. */
static inline __m512i gf256_xtime_avx512(__m512i v) {
	const __m512i red_poly = _mm512_set1_epi8(0x1B);
	const __m512i zero     = _mm512_setzero_epi32();

	return _mm512_mask_blend_epi8(_mm512_movepi8_mask(v), zero, red_poly) ^ _mm512_add_epi8(v, v);
}

/* Keep only the k first bytes of v, zeroing the rest. k must be < 64. */
static inline __m512i gf_keep_prefix_avx512(__m512i v, uint32_t k) {
	return _mm512_maskz_mov_epi8(((__mmask64)1 << k) - 1, v);
}

/* Compute the 8 multiples v * 2^k (k = 0..7) of a 64-byte GF(256) block.
 * NOTE: with GFNI a full GF(256) product is a single instruction, so the
 * multiples are useless there and only the block itself is kept. */
static inline void gf256_precompute_multiples_avx512(__m512i v, __m512i out[8]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	out[0] = v;
#else
	uint32_t k;

	out[0] = v;
	for (k = 1; k < 8; k++) {
		v = gf256_xtime_avx512(v);
		out[k] = v;
	}
#endif

	return;
}

/* Multiply and accumulate: acc ^= a * X, where mul[] holds the precomputed
 * multiples of the X block. Two partial accumulators are used to shorten the
 * dependency chain. */
static inline __m512i gf256_mac_block_avx512(__m512i acc, __m512i a, const __m512i mul[8]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	return acc ^ _mm512_gf2p8mul_epi8(a, mul[0]);
#else
	__m512i t0, t1;

	t0  = _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 7)), mul[0]);
	t1  = _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 6)), mul[1]);
	t0 ^= _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 5)), mul[2]);
	t1 ^= _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 4)), mul[3]);
	t0 ^= _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 3)), mul[4]);
	t1 ^= _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 2)), mul[5]);
	t0 ^= _mm512_maskz_mov_epi8(_mm512_movepi8_mask(_mm512_slli_epi64(a, 1)), mul[6]);
	t1 ^= _mm512_maskz_mov_epi8(_mm512_movepi8_mask(a), mul[7]);

	return acc ^ t0 ^ t1;
#endif
}

/* One panel pass of a GF(256) matrix by vector product: XOR into Y the
 * contribution of the columns [64 * p, 64 * p + 64 * np) of A, whose multiples
 * of X are already in mul[][]. When 'first' is set the result overwrites Y
 * instead of being accumulated into it.
 * NOTE: the tail of a row is fetched with a masked load, which never faults on
 * the masked off bytes, so no in-bounds special case is needed here. */
static inline void gf256_mat_mult_panel_avx512(const uint8_t *A, uint8_t *Y, uint32_t n,
                uint32_t p, uint32_t np, const __m512i mul[][8], int tri_inf, int first) {
	const uint32_t lo = 64 * p;
	uint32_t i, b;

	for (i = 0; i < n; i++) {
		uint32_t len = tri_inf ? (i + 1) : n;
		uint32_t hi, nfull, tail;
		__m512i acc0, acc1;
		uint8_t s;

		if (len <= lo) {
			/* TRI_INF: this row does not reach the current panel */
			continue;
		}
		hi = len;
		if (hi > (lo + (64 * np))) {
			hi = lo + (64 * np);
		}
		nfull = (hi - lo) / 64;
		tail  = (hi - lo) % 64;

		acc0 = _mm512_setzero_epi32();
		acc1 = _mm512_setzero_epi32();
		for (b = 0; b < nfull; b++) {
			size_t aoff = ((size_t)n * i) + lo + (64 * b);
			acc0 = gf256_mac_block_avx512(acc0, _mm512_loadu_epi64((const __m512i*)(const void*)&A[aoff]), mul[b]);
		}
		if (tail != 0) {
			size_t aoff = ((size_t)n * i) + lo + (64 * nfull);
			acc1 = gf256_mac_block_avx512(acc1, load_incomplete_m512(&A[aoff], tail), mul[nfull]);
		}

		s = sum_uint8_avx512(acc0 ^ acc1);
		if (first) {
			Y[i] = s;
		} else {
			Y[i] ^= s;
		}
	}

	return;
}

/*
 * Vector multiplied by a constant in GF(256).
 */
static inline void gf256_constant_vect_mult_avx512(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	uint32_t i;
	__m512i _a, _b;

	/* Create a vector _b with the duplicated constant everywhere */
	_b = _mm512_set1_epi8(b);

	for (i = 0; i < len; i += 64) {
		if ((len - i) < 64) {
			_a = load_incomplete_m512(&a[i], len - i);
			/* Vectorized multiplication in GF(256) */
			store_incomplete_m512(gf256_mult_vectorized_avx512(_a, _b), &c[i], len - i);
		} else {
			/* Obvious 512-bit */
			_a = _mm512_loadu_epi64((__m512i*)&a[i]);
			/* Vectorized multiplication in GF(256) */
			_mm512_storeu_epi64(&c[i], gf256_mult_vectorized_avx512(_a, _b));
		}
	}

	return;
}

/*
 * Vector to vector multiplication in GF(256).
 * Takes two vectors of length 'len', and returns a byte (element in GF(256))
 */
static inline uint8_t gf256_vect_mult_avx512(const uint8_t *a, const uint8_t *b, uint32_t len) {
	uint32_t i;
	__m512i accu, _a, _b;

	/* Set the accumulator to 0 */
	accu = _mm512_setzero_epi32();

	for (i = 0; i < len; i += 64) {
		if ((len - i) < 64) {
			_a = load_incomplete_m512(&a[i], len - i);
			_b = load_incomplete_m512(&b[i], len - i);
		} else {
			/* Obvious 512-bit */
			_a = _mm512_loadu_epi64((__m512i*)&a[i]);
			_b = _mm512_loadu_epi64((__m512i*)&b[i]);
		}
		/* Vectorized multiplication in GF(256) */
		accu ^= gf256_mult_vectorized_avx512(_a, _b);
	}

	return sum_uint8_avx512(accu);
}

/*
 * Vector to vector batched multiplication in GF(256).
 * Takes n+1 vectors a, b[0], ..., b[n-1] of length 'len'
 * Returns n bytes c[0], ..., c[n-1] (elements in GF(256))
 *   where c[i] is the vector to vector multiplication between a and b[i]
 * Assume that a is a non-sensitive vector
 */
static inline void gf256_vect_mult_multiple_public_avx512(uint8_t* const* c, const uint8_t *a, const uint8_t* const* b, uint32_t len, uint32_t n) {
	__m512i mul[GF_MULT_PANEL_BLOCKS][8];
	const uint32_t nblocks = (len + 63) / 64;
	uint32_t i, k;

	/* a is common to the n products, so its multiples are precomputed once */
	if (nblocks > GF_MULT_PANEL_BLOCKS) {
		for (i = 0; i < n; i++) {
			*c[i] = gf256_vect_mult_avx512(a, b[i], len);
		}
		return;
	}

	for (k = 0; k < nblocks; k++) {
		uint32_t off = 64 * k;
		__m512i ab;
		if ((off + 64) <= len) {
			ab = _mm512_loadu_epi64((const __m512i*)(const void*)&a[off]);
		} else {
			ab = load_incomplete_m512(&a[off], len - off);
		}
		gf256_precompute_multiples_avx512(ab, mul[k]);
	}

	for (i = 0; i < n; i++) {
		__m512i acc = _mm512_setzero_epi32();
		for (k = 0; k < nblocks; k++) {
			uint32_t off = 64 * k;
			__m512i bb;
			if ((off + 64) <= len) {
				bb = _mm512_loadu_epi64((const __m512i*)(const void*)&b[i][off]);
			} else {
				bb = load_incomplete_m512(&b[i][off], len - off);
			}
			acc = gf256_mac_block_avx512(acc, bb, mul[k]);
		}
		*c[i] = sum_uint8_avx512(acc);
	}

	return;
}

/* Matrix and vector multiplication over GF(256)
 * C += A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
static inline void gf256_mat_mult_avx512(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	__m512i mul[GF_MULT_PANEL_BLOCKS][8];
	const uint32_t nblocks = (n + 63) / 64;
	uint32_t b, p;

	/* TRI_SUP starts each row at a different, non block aligned column, which
	 * does not fit the scheme below; it is unused by MQOM and is left on the
	 * generic path. */
	if (mtype == TRI_SUP) {
		GF256_MAT_MULT(A, X, Y, n, mtype, gf256_vect_mult_avx512);
		return;
	}

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			uint32_t off = 64 * (p + b);
			__m512i xb;
			if ((off + 64) <= n) {
				xb = _mm512_loadu_epi64((const __m512i*)(const void*)&X[off]);
			} else {
				/* Zero padded: the multiples of the padding are zero too */
				xb = load_incomplete_m512(&X[off], n - off);
			}
			gf256_precompute_multiples_avx512(xb, mul[b]);
		}

		gf256_mat_mult_panel_avx512(A, Y, n, p, np, (const __m512i (*)[8])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}


/*
 * "Hybrid" multiplication of a constant in GF(2) and a vector in GF(256)
 */
static inline void gf2_gf256_constant_vect_mult_avx512(uint8_t a_gf2, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	gf2_gf256_constant_vect_mult_ref(a_gf2, b_gf256, c_gf256, n);

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(2)
 */
static inline void gf256_gf2_constant_vect_mult_avx512(uint8_t a_gf256, const uint8_t *b_gf2, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;
	const __m512i zero = _mm512_setzero_epi32();
	const __m128i zero_128 = _mm_setzero_si128();

	/* Load and broadcast the constant */
	__m512i _a = _mm512_set1_epi8(a_gf256);

	for (i = 0; i < n; i += 64) {
		/* We use a mask load depending on the mask value in b_gf2 */
		uint32_t len = (n - i) < 64 ? (n - i) : 64;
		uint32_t ceil_len_bits = (len + 7) / 8;
		__m128i mask_128 = _mm_mask_loadu_epi8(zero_128, ((__mmask16)1 << ceil_len_bits) - 1, &b_gf2[(i / 8)]);
		/* Transfer to our 64 bits mask */
		__mmask64 mask = mask64_from_m128(mask_128);
		__m512i _c = _mm512_mask_mov_epi8(zero, mask, _a);
		store_incomplete_m512(_c, &c_gf256[i], len);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256)
 */
static inline uint8_t gf2_gf256_vect_mult_avx512(const uint8_t *a_gf2, const uint8_t *b_gf256, uint32_t n) {
	uint32_t i;
	const __m512i zero = _mm512_setzero_epi32();
	const __m128i zero_128 = _mm_setzero_si128();

	/* Set the accumulator to 0 */
	__m512i accu = _mm512_setzero_epi32();

	for (i = 0; i < n; i += 64) {
		/* We use a mask load depending on the mask value in a_gf2 */
		uint32_t len = (n - i) < 64 ? (n - i) : 64;
		uint32_t ceil_len_bits = (len + 7) / 8;
		__m128i mask_128 = _mm_mask_loadu_epi8(zero_128, ((__mmask16)1 << ceil_len_bits) - 1, &a_gf2[(i / 8)]);
		/* Transfer to our 64 bits mask */
		__mmask64 mask = mask64_from_m128(mask_128) & ((len == 64) ? (__mmask64) -1 : (((__mmask64)1 << len) - 1));
		accu ^= _mm512_mask_loadu_epi8(zero, mask, (int const*)&b_gf256[i]);
	}

	return sum_uint8_avx512(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(2)
 */
static inline uint8_t gf256_gf2_vect_mult_avx512(const uint8_t *a_gf256, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256_vect_mult_avx512(b_gf2, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256), resulting
 *  in a vector in GF(256)
 */
static inline void gf2_gf256_mat_mult_avx512(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF2_GF256_MAT_MULT(A, X, Y, n, mtype, gf2_gf256_vect_mult_avx512);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(2), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf2_mat_mult_avx512(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF256_GF2_MAT_MULT(A, X, Y, n, mtype, gf256_gf2_vect_mult_avx512);
}

/* GF(256) matrix transposition */
static inline void gf256_mat_transpose_avx512(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	gf256_mat_transpose_ref(A, B, n, mtype);
}

/*
 * "Hybrid" multiplication of a constant in GF(16) and a vector in GF(256)
 */
static inline void gf16_gf256_constant_vect_mult_avx512(uint8_t a_gf16, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint8_t a_gf256;
	gf256_vect_lift_from_gf16_ref(&a_gf16, &a_gf256, 1);
	gf256_constant_vect_mult_avx512(a_gf256, b_gf256, c_gf256, n);
	return;
}

/* Nibble to byte lookup table implementing the GF(16) -> GF(256) lifting,
 * replicated in each 128-bit lane so that it can be fed to a vpshufb. */
#define GF256_LIFT_FROM_GF16_LOOKUP_AVX512() \
	_mm512_set_epi8(12, 13, -20, -19, 81, 80, -79, -80, -68, -67, 92, 93, -31, -32, 1, 0, \
	                12, 13, -20, -19, 81, 80, -79, -80, -68, -67, 92, 93, -31, -32, 1, 0, \
	                12, 13, -20, -19, 81, 80, -79, -80, -68, -67, 92, 93, -31, -32, 1, 0, \
	                12, 13, -20, -19, 81, 80, -79, -80, -68, -67, 92, 93, -31, -32, 1, 0)

/* Expand the 64 packed nibbles held in the low 256 bits of 'packed' into 64
 * bytes, mapping each nibble through the byte lookup 'lut'.
 * With the plain lifting table this is the GF(16) -> GF(256) lifting; with a
 * table premultiplied by a constant this fuses the lifting and the constant
 * multiplication into a single pass. */
static inline __m512i gf256_expand_nibbles_avx512(__m512i packed, __m512i lut) {
	const __m512i shuff_msk_even = _mm512_set_epi8(-1, 31, -1,  30,  -1, 29, -1, 28, -1, 27, -1,  26,  -1, 25, -1, 24,
	                               -1, 23, -1, 22, -1,  21,  -1, 20, -1, 19, -1, 18, -1,  17,  -1, 16,
	                               -1, 15, -1,  14,  -1, 13, -1, 12, -1, 11, -1,  10,  -1, 9, -1, 8,
	                               -1, 7, -1,  6,  -1, 5, -1, 4, -1, 3, -1,  2,  -1, 1, -1, 0);
	const __m512i shuff_msk_odd  = _mm512_set_epi8(31, -1,  30,  -1, 29, -1, 28, -1, 27, -1,  26,  -1, 25, -1, 24, -1,
	                               23, -1, 22, -1,  21,  -1, 20, -1, 19, -1, 18, -1,  17,  -1, 16, -1,
	                               15, -1,  14,  -1, 13, -1, 12, -1, 11, -1,  10,  -1, 9, -1, 8, -1,
	                               7, -1,  6,  -1, 5, -1, 4, -1, 3, -1,  2,  -1, 1, -1, 0, -1);
	const __m512i nib_mask = _mm512_set1_epi8(0x0f);
	const __m512i zero = _mm512_setzero_epi32();
	__m512i _a, _b, _c;

	/* Duplicate lanes */
	packed = _mm512_inserti64x4(packed, _mm512_castsi512_si256(packed), 1);
	/* Isolate the nibbles in packed */
	_a = packed & nib_mask;
	_b = _mm512_srli_epi64(packed, 4) & nib_mask;
	/* Create the nibbles mix using a cross-lane shuffling */
	_a = _mm512_permutex2var_epi8(_a, shuff_msk_even, zero);
	_b = _mm512_permutex2var_epi8(_b, shuff_msk_odd, zero);
	_c = _a | _b;

	/* Lift: since we are on 16 bits, we can perform a vperm lookup inside the register */
	return _mm512_shuffle_epi8(lut, _c);
}

/* Vectorized lifting from GF(16) to GF(256) */
static inline void gf256_vect_lift_from_gf16_avx512(const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t len) {
	const __m512i lifting_lookup = GF256_LIFT_FROM_GF16_LOOKUP_AVX512();
	uint32_t i;
	__m512i _a_gf16, _c;

	for (i = 0; i < len; i += 64) {
		if ((len - i) < 64) {
			_a_gf16 = load_incomplete_m512((const uint8_t*)&b_gf16[i / 2], (len - i + 1) / 2);
		} else {
			/* Obvious 256-bit */
			_a_gf16 = load_incomplete_m512((const uint8_t*)&b_gf16[i / 2], 32);
		}

		_c = gf256_expand_nibbles_avx512(_a_gf16, lifting_lookup);
		/* Store the result */
		if ((len - i) < 64) {
			store_incomplete_m512(_c, &c_gf256[i], (len - i));
		} else {
			_mm512_storeu_epi64((__m512i*)&c_gf256[i], _c);
		}
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(16)
 */
static inline void gf256_gf16_constant_vect_mult_avx512(uint8_t a_gf256, const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t n) {
	/* The lifting and the multiplication by the constant are fused: the 16
	 * entry lifting table is premultiplied by a_gf256 once, then one vpshufb
	 * per output block produces the lifted and multiplied bytes directly.
	 * NOTE: the table content depends on the (possibly secret) constant, but
	 * the lookup indices are register operands of vpshufb, whose timing is
	 * data independent: no secret indexed memory access is introduced. */
	const __m512i lut = gf256_mult_vectorized_avx512(_mm512_set1_epi8((char)a_gf256), GF256_LIFT_FROM_GF16_LOOKUP_AVX512());
	uint32_t i;
	__m512i _a_gf16, _c;

	for (i = 0; i < n; i += 64) {
		if ((n - i) < 64) {
			_a_gf16 = load_incomplete_m512((const uint8_t*)&b_gf16[i / 2], (n - i + 1) / 2);
		} else {
			_a_gf16 = load_incomplete_m512((const uint8_t*)&b_gf16[i / 2], 32);
		}

		_c = gf256_expand_nibbles_avx512(_a_gf16, lut);
		/* Store the result */
		if ((n - i) < 64) {
			store_incomplete_m512(_c, &c_gf256[i], (n - i));
		} else {
			_mm512_storeu_epi64((__m512i*)&c_gf256[i], _c);
		}
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256)
 */
static inline uint8_t gf16_gf256_vect_mult_avx512(const uint8_t *a_gf16, const uint8_t *b_gf256, uint32_t len) {
	uint32_t i;
	__m512i accu, _a, _b;
	__m512i _a_gf16;

	/* Set the accumulator to 0 */
	accu = _mm512_setzero_epi32();

	/* NOTE: the lifting is kept fully in registers here; the previous version
	 * round-tripped the lifted block through the stack on every block. */
	for (i = 0; i < len; i += 64) {
		if ((len - i) < 64) {
			_a_gf16 = load_incomplete_m512((const uint8_t*)&a_gf16[i / 2], (len - i + 1) / 2);
			/* Zero the lifted elements beyond the vector length: the last
			 * loaded byte may carry a nibble past the end of the vector. */
			_a = gf_keep_prefix_avx512(gf256_expand_nibbles_avx512(_a_gf16, GF256_LIFT_FROM_GF16_LOOKUP_AVX512()), len - i);
			_b = load_incomplete_m512(&b_gf256[i], len - i);
		} else {
			/* Obvious 512-bit */
			_a_gf16 = load_incomplete_m512((const uint8_t*)&a_gf16[i / 2], 32);
			_a = gf256_expand_nibbles_avx512(_a_gf16, GF256_LIFT_FROM_GF16_LOOKUP_AVX512());
			_b = _mm512_loadu_epi64((__m512i*)&b_gf256[i]);
		}
		/* Vectorized multiplication in GF(256) */
		accu ^= gf256_mult_vectorized_avx512(_a, _b);
	}

	return sum_uint8_avx512(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(16)
 */
static inline uint8_t gf256_gf16_vect_mult_avx512(const uint8_t *a_gf256, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256_vect_mult_avx512(b_gf16, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(16), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf16_mat_mult_avx512(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* Same scheme as gf256_mat_mult_avx512(), except that the common vector X
	 * is lifted from GF(16) once for the whole matrix instead of once per row.
	 * NOTE: like GF256_GF16_MAT_MULT (i.e. GF256_GF2_MAT_MULT), TRI_SUP is
	 * handled as REG here. */
	__m512i mul[GF_MULT_PANEL_BLOCKS][8];
	const __m512i lifting_lookup = GF256_LIFT_FROM_GF16_LOOKUP_AVX512();
	const uint32_t nblocks = (n + 63) / 64;
	uint32_t b, p;

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			uint32_t off = 64 * (p + b);
			__m512i xb;
			if ((off + 64) <= n) {
				xb = gf256_expand_nibbles_avx512(load_incomplete_m512(&X[off / 2], 32), lifting_lookup);
			} else {
				xb = gf256_expand_nibbles_avx512(load_incomplete_m512(&X[off / 2], (n - off + 1) / 2), lifting_lookup);
				/* Zero pad past the end of the vector */
				xb = gf_keep_prefix_avx512(xb, n - off);
			}
			gf256_precompute_multiples_avx512(xb, mul[b]);
		}

		gf256_mat_mult_panel_avx512(A, Y, n, p, np, (const __m512i (*)[8])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}

/* === GF(256^2) === */
/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint16_t gf256to2_mult_avx512(uint16_t x, uint16_t y) {
	return gf256to2_mult_ref(x, y);
}

/* Vectorize multiplication of _a and _b in GF(256^2): the elements in the field are made of
 * 16 bits each in the lanes of the zmm */
static inline __m512i gf256to2_mult_vectorized_avx512(__m512i _a, __m512i _b) {
	const __m512i shuff_msk_crossed = _mm512_set_epi8(62, 63, 60, 61, 58, 59, 56, 57, 54, 55, 52, 53,
	                                  50, 51, 48, 49, 46, 47, 44, 45, 42, 43, 40, 41,
	                                  38, 39, 36, 37, 34, 35, 32, 33, 30, 31, 28, 29,
	                                  26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17,
	                                  14, 15, 12, 13, 10, 11, 8,  9,  6,  7,  4,  5,
	                                  2,  3,  0,  1);
	const __m512i shuff_msk1 = _mm512_set_epi8(62, 62, 60, 60, 58, 58, 56, 56, 54, 54, 52, 52,
	                           50, 50, 48, 48, 46, 46, 44, 44, 42, 42, 40, 40,
	                           38, 38, 36, 36, 34, 34, 32, 32, 30, 30, 28, 28,
	                           26, 26, 24, 24, 22, 22, 20, 20, 18, 18, 16, 16,
	                           14, 14, 12, 12, 10, 10, 8,  8,  6,  6,  4,  4,
	                           2,  2,  0,  0);
	const __m512i shuff_msk2 = _mm512_set_epi8(63, 63, 61, 61, 59, 59, 57, 57, 55, 55, 53, 53,
	                           51, 51, 49, 49, 47, 47, 45, 45, 43, 43, 41, 41,
	                           39, 39, 37, 37, 35, 35, 33, 33, 31, 31, 29, 29,
	                           27, 27, 25, 25, 23, 23, 21, 21, 19, 19, 17, 17,
	                           15, 15, 13, 13, 11, 11, 9,  9,  7,  7,  5,  5,
	                           3,  3,  1,  1);
	const __m512i const32 = _mm512_set_epi64(0x0020002000200020, 0x0020002000200020, 0x0020002000200020, 0x0020002000200020,
	                        0x0020002000200020, 0x0020002000200020, 0x0020002000200020, 0x0020002000200020);
	const __m512i zero = _mm512_setzero_epi32();

	__m512i ab = gf256_mult_vectorized_avx512(_a, _b);
	__m512i a0b0 = _mm512_shuffle_epi8(ab, shuff_msk1);
	__m512i a1b1 = _mm512_shuffle_epi8(ab, shuff_msk2);
	__m512i a1b1_32 = gf256_mult_vectorized_avx512(a1b1, const32);
	/* */
	__m512i a0_xor_a1 = _a ^ _mm512_shuffle_epi8(_a, shuff_msk_crossed);
	__m512i b0_xor_b1 = _mm512_mask_blend_epi8((__mmask64)0xaaaaaaaaaaaaaaaa, zero, _b ^ _mm512_shuffle_epi8(_b, shuff_msk_crossed));
	__m512i mult_ab_xor = gf256_mult_vectorized_avx512(a0_xor_a1, b0_xor_b1);

	/* Compute the result */
	__m512i res = a0b0 ^ a1b1_32 ^ mult_ab_xor;

	return res;
}

/* Byte shuffle masks used to split a block of GF(256^2) elements into its two
 * GF(256) coordinates, each duplicated over the whole 16-bit lane. */
#define GF256TO2_DUP_COORD0_AVX512() \
	_mm512_set_epi8(62, 62, 60, 60, 58, 58, 56, 56, 54, 54, 52, 52, 50, 50, 48, 48, \
	                46, 46, 44, 44, 42, 42, 40, 40, 38, 38, 36, 36, 34, 34, 32, 32, \
	                30, 30, 28, 28, 26, 26, 24, 24, 22, 22, 20, 20, 18, 18, 16, 16, \
	                14, 14, 12, 12, 10, 10, 8,  8,  6,  6,  4,  4,  2,  2,  0,  0)
#define GF256TO2_DUP_COORD1_AVX512() \
	_mm512_set_epi8(63, 63, 61, 61, 59, 59, 57, 57, 55, 55, 53, 53, 51, 51, 49, 49, \
	                47, 47, 45, 45, 43, 43, 41, 41, 39, 39, 37, 37, 35, 35, 33, 33, \
	                31, 31, 29, 29, 27, 27, 25, 25, 23, 23, 21, 21, 19, 19, 17, 17, \
	                15, 15, 13, 13, 11, 11, 9,  9,  7,  7,  5,  5,  3,  3,  1,  1)

/* Multiply every GF(256^2) element of the block by the generator t (the root of
 * X^2 + X + 32). With b = b0 + b1 * t one has t * b = (32 * b1) + (b0 + b1) * t. */
static inline __m512i gf256to2_mult_by_t_avx512(__m512i b) {
	const __m512i swap_msk = _mm512_set_epi8(62, 63, 60, 61, 58, 59, 56, 57, 54, 55, 52, 53, 50, 51, 48, 49,
	                         46, 47, 44, 45, 42, 43, 40, 41, 38, 39, 36, 37, 34, 35, 32, 33,
	                         30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17,
	                         14, 15, 12, 13, 10, 11, 8,  9,  6,  7,  4,  5,  2,  3,  0,  1);
	const __m512i dup1 = GF256TO2_DUP_COORD1_AVX512();
	__m512i sum, m32;
	uint32_t k;

	/* Both bytes of each lane hold b0 ^ b1 */
	sum = b ^ _mm512_shuffle_epi8(b, swap_msk);
	/* Both bytes of each lane hold b1, then 32 * b1 */
	m32 = _mm512_shuffle_epi8(b, dup1);
	for (k = 0; k < 5; k++) {
		m32 = gf256_xtime_avx512(m32);
	}

	/* Coordinate 0 (even byte) <- 32 * b1, coordinate 1 (odd byte) <- b0 ^ b1 */
	return _mm512_mask_blend_epi8((__mmask64)0xaaaaaaaaaaaaaaaaULL, m32, sum);
}

/* Precompute the 16 multiples of a GF(256^2) block needed to turn a product by
 * an arbitrary element a = a0 + a1 * t into pure bit selections:
 *   a * X = a0 * X + a1 * (t * X)
 * NOTE: with GFNI only the block itself is kept. */
static inline void gf256to2_precompute_multiples_avx512(__m512i v, __m512i out[16]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	/* With GFNI only X and t * X are needed: a * X = a0 * X + a1 * (t * X)
	 * turns into two single instruction bytewise GF(256) products. */
	out[0] = v;
	out[1] = gf256to2_mult_by_t_avx512(v);
#else
	gf256_precompute_multiples_avx512(v, &out[0]);
	gf256_precompute_multiples_avx512(gf256to2_mult_by_t_avx512(v), &out[8]);
#endif

	return;
}

/* Multiply and accumulate in GF(256^2): acc ^= a * X. */
static inline __m512i gf256to2_mac_block_avx512(__m512i acc, __m512i a, const __m512i mul[16]) {
#if defined(__GFNI__) && !defined(NO_GFNI)
	__m512i t0 = _mm512_gf2p8mul_epi8(_mm512_shuffle_epi8(a, GF256TO2_DUP_COORD0_AVX512()), mul[0]);
	__m512i t1 = _mm512_gf2p8mul_epi8(_mm512_shuffle_epi8(a, GF256TO2_DUP_COORD1_AVX512()), mul[1]);

	return acc ^ t0 ^ t1;
#else
	const __m512i zero = _mm512_setzero_epi32();
	__m512i t0, t1;

	t0 = gf256_mac_block_avx512(zero, _mm512_shuffle_epi8(a, GF256TO2_DUP_COORD0_AVX512()), &mul[0]);
	t1 = gf256_mac_block_avx512(zero, _mm512_shuffle_epi8(a, GF256TO2_DUP_COORD1_AVX512()), &mul[8]);

	return acc ^ t0 ^ t1;
#endif
}

/* One panel pass of a GF(256^2) matrix by vector product, counted in bytes. */
static inline void gf256to2_mat_mult_panel_avx512(const uint8_t *A, uint16_t *Y, uint32_t n,
                uint32_t p, uint32_t np, const __m512i mul[][16], int tri_inf, int first) {
	const uint32_t lo = 64 * p;
	const size_t nbytes = (size_t)2 * n;
	uint32_t i, b;

	for (i = 0; i < n; i++) {
		uint32_t len = 2 * (tri_inf ? (i + 1) : n);
		uint32_t hi, nfull, tail;
		__m512i acc0, acc1;
		uint16_t s;

		if (len <= lo) {
			continue;
		}
		hi = len;
		if (hi > (lo + (64 * np))) {
			hi = lo + (64 * np);
		}
		nfull = (hi - lo) / 64;
		tail  = (hi - lo) % 64;

		acc0 = _mm512_setzero_epi32();
		acc1 = _mm512_setzero_epi32();
		for (b = 0; b < nfull; b++) {
			size_t aoff = (nbytes * i) + lo + (64 * b);
			acc0 = gf256to2_mac_block_avx512(acc0, _mm512_loadu_epi64((const __m512i*)(const void*)&A[aoff]), mul[b]);
		}
		if (tail != 0) {
			size_t aoff = (nbytes * i) + lo + (64 * nfull);
			acc1 = gf256to2_mac_block_avx512(acc1, load_incomplete_m512(&A[aoff], tail), mul[nfull]);
		}

		s = sum_uint16_avx512(acc0 ^ acc1);
		if (first) {
			Y[i] = s;
		} else {
			Y[i] ^= s;
		}
	}

	return;
}

/* Same as above but for a common vector lying in the GF(256) subfield, each of
 * its elements being duplicated over both bytes of a 16-bit lane: the per
 * element product then degenerates into a plain bytewise GF(256) product. */
static inline void gf256_mat_mult_panel_gf256to2_avx512(const uint8_t *A, uint16_t *Y, uint32_t n,
                uint32_t p, uint32_t np, const __m512i mul[][8], int tri_inf, int first) {
	const uint32_t lo = 64 * p;
	const size_t nbytes = (size_t)2 * n;
	uint32_t i, b;

	for (i = 0; i < n; i++) {
		uint32_t len = 2 * (tri_inf ? (i + 1) : n);
		uint32_t hi, nfull, tail;
		__m512i acc0, acc1;
		uint16_t s;

		if (len <= lo) {
			continue;
		}
		hi = len;
		if (hi > (lo + (64 * np))) {
			hi = lo + (64 * np);
		}
		nfull = (hi - lo) / 64;
		tail  = (hi - lo) % 64;

		acc0 = _mm512_setzero_epi32();
		acc1 = _mm512_setzero_epi32();
		for (b = 0; b < nfull; b++) {
			size_t aoff = (nbytes * i) + lo + (64 * b);
			acc0 = gf256_mac_block_avx512(acc0, _mm512_loadu_epi64((const __m512i*)(const void*)&A[aoff]), mul[b]);
		}
		if (tail != 0) {
			size_t aoff = (nbytes * i) + lo + (64 * nfull);
			acc1 = gf256_mac_block_avx512(acc1, load_incomplete_m512(&A[aoff], tail), mul[nfull]);
		}

		s = sum_uint16_avx512(acc0 ^ acc1);
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
static inline void gf256to2_constant_vect_mult_avx512(uint16_t b, const uint16_t *a, uint16_t *c, uint32_t len) {
	/* The constant is the same for every element, so its multiples are
	 * precomputed once and each block then only costs two bit selection
	 * chains instead of three full generic GF(256) multiplications. */
	__m512i mul[16];
	uint32_t i;
	__m512i _a;

	gf256to2_precompute_multiples_avx512(_mm512_set1_epi16((short)b), mul);

	for (i = 0; i < (2 * len); i += 64) {
		if (((2 * len) - i) < 64) {
			_a = load_incomplete_m512((const uint8_t*)&a[i / 2], (2 * len) - i);
			/* Vectorized multiplication in GF(256^2) */
			store_incomplete_m512(gf256to2_mac_block_avx512(_mm512_setzero_epi32(), _a, mul), (uint8_t*)&c[i / 2], (2 * len) - i);
		} else {
			/* Obvious 512-bit */
			_a = _mm512_loadu_epi64((__m512i*)&a[i / 2]);
			/* Vectorized multiplication in GF(256^2) */
			_mm512_storeu_epi64(&c[i / 2], gf256to2_mac_block_avx512(_mm512_setzero_epi32(), _a, mul));
		}
	}

	return;
}

/* Perform a multiplication in GF(256^2) of elements in vectors a an b */
static inline uint16_t gf256to2_vect_mult_avx512(const uint16_t *a, const uint16_t *b, uint32_t len) {
	uint32_t i;
	__m512i accu, _a, _b;

	/* Set the accumulator to 0 */
	accu = _mm512_setzero_epi32();

	for (i = 0; i < (2 * len); i += 64) {
		if (((2 * len) - i) < 64) {
			_a = load_incomplete_m512((const uint8_t*)&a[i / 2], ((2 * len) - i));
			_b = load_incomplete_m512((const uint8_t*)&b[i / 2], ((2 * len) - i));
		} else {
			/* Obvious 512-bit */
			_a = _mm512_loadu_epi64((__m512i*)&a[i / 2]);
			_b = _mm512_loadu_epi64((__m512i*)&b[i / 2]);
		}
		accu ^= gf256to2_mult_vectorized_avx512(_a, _b);
	}

	return sum_uint16_avx512(accu);
}

/*
 * Vector to vector batched multiplication in GF(256^2).
 * Takes n+1 vectors a, b[0], ..., b[n-1] of length 'len'
 * Returns n double-bytes c[0], ..., c[n-1] (elements in GF(256^2))
 *   where c[i] is the vector to vector multiplication between a and b[i]
 * Assume that a is a non-sensitive vector
 */
static inline void gf256to2_vect_mult_multiple_public_avx512(uint16_t* const* c, const uint16_t *a, const uint16_t* const* b, uint32_t len, uint32_t n) {
	__m512i mul[GF_MULT_PANEL_BLOCKS][16];
	const uint8_t *ab_ptr = (const uint8_t*)(const void*)a;
	const uint32_t nbytes = 2 * len;
	const uint32_t nblocks = (nbytes + 63) / 64;
	uint32_t i, k;

	/* a is common to the n products, so its multiples are precomputed once */
	if (nblocks > GF_MULT_PANEL_BLOCKS) {
		for (i = 0; i < n; i++) {
			*c[i] = gf256to2_vect_mult_avx512(a, b[i], len);
		}
		return;
	}

	for (k = 0; k < nblocks; k++) {
		uint32_t off = 64 * k;
		__m512i av;
		if ((off + 64) <= nbytes) {
			av = _mm512_loadu_epi64((const __m512i*)(const void*)&ab_ptr[off]);
		} else {
			av = load_incomplete_m512(&ab_ptr[off], nbytes - off);
		}
		gf256to2_precompute_multiples_avx512(av, mul[k]);
	}

	for (i = 0; i < n; i++) {
		const uint8_t *bb_ptr = (const uint8_t*)(const void*)b[i];
		__m512i acc = _mm512_setzero_epi32();
		for (k = 0; k < nblocks; k++) {
			uint32_t off = 64 * k;
			__m512i bv;
			if ((off + 64) <= nbytes) {
				bv = _mm512_loadu_epi64((const __m512i*)(const void*)&bb_ptr[off]);
			} else {
				bv = load_incomplete_m512(&bb_ptr[off], nbytes - off);
			}
			acc = gf256to2_mac_block_avx512(acc, bv, mul[k]);
		}
		*c[i] = sum_uint16_avx512(acc);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(2) and a vector in GF(256^2)
 */
static inline void gf2_gf256to2_constant_vect_mult_avx512(uint8_t a_gf2, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	gf2_gf256to2_constant_vect_mult_ref(a_gf2, b_gf256to2, c_gf256to2, n);

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(2)
 */
static inline void gf256to2_gf2_constant_vect_mult_avx512(uint16_t a_gf256to2, const uint8_t *b_gf2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;
	const __m512i zero = _mm512_setzero_epi32();
	const __m128i zero_128 = _mm_setzero_si128();

	/* Load and broadcast the constant */
	__m512i _a = _mm512_set1_epi16(a_gf256to2);

	for (i = 0; i < n; i += 32) {
		/* We use a mask load depending on the mask value in b_gf2 */
		uint32_t len = (n - i) < 32 ? (n - i) : 32;
		uint32_t ceil_len_bits = (len + 7) / 8;
		__m128i mask_128 = _mm_mask_loadu_epi8(zero_128, ((__mmask16)1 << ceil_len_bits) - 1, &b_gf2[(i / 8)]);
		/* Transfer to our 64 bits mask */
		__mmask64 mask64 = mask64_from_m128(mask_128);
		__mmask32 mask = (__mmask32)mask64;
		__m512i _c = _mm512_mask_mov_epi16(zero, mask, _a);
		store_incomplete_m512(_c, (uint8_t*)&c_gf256to2[i], 2 * len);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(256)
 */
static inline void gf256_gf256to2_constant_vect_mult_avx512(uint8_t a_gf256, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	gf256_gf256to2_constant_vect_mult_ref(a_gf256, b_gf256to2, c_gf256to2, n);

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(256)
 */
static inline void gf256to2_gf256_constant_vect_mult_avx512(uint16_t a_gf256to2, const uint8_t *b_gf256, uint16_t *c_gf256to2, uint32_t len) {
	__m512i mul[8];
	uint32_t i;
	__m512i _b;
	const __m512i shuff_msk = _mm512_set_epi8(31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24,
	                          23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16,
	                          15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,
	                          7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2,  2,  1, 1, 0, 0);

	/* Every element of b_gf256 is a GF(256) scalar, so this is a plain
	 * bytewise GF(256) product against the broadcast constant: its multiples
	 * are precomputed once. */
	gf256_precompute_multiples_avx512(_mm512_set1_epi16((short)a_gf256to2), mul);

	for (i = 0; i < len; i += 32) {
		uint32_t to_load = (len - i) < 32 ? (len - i) : 32;
		_b = load_incomplete_m512((const uint8_t*)&b_gf256[i], to_load);
		/* Duplicate the values in _b */
		_b = _mm512_permutex2var_epi8(_b, shuff_msk, _b);
		/* Vectorized multiplication in GF(256^2) */
		store_incomplete_m512(gf256_mac_block_avx512(_mm512_setzero_epi32(), _b, mul), (uint8_t*)&c_gf256to2[i], 2 * to_load);
	}
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256^2)
 */
static inline uint16_t gf2_gf256to2_vect_mult_avx512(const uint8_t *a_gf2, const uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;
	const __m512i zero = _mm512_setzero_epi32();
	const __m128i zero_128 = _mm_setzero_si128();

	/* Set the accumulator to 0 */
	__m512i accu = _mm512_setzero_epi32();

	for (i = 0; i < n; i += 32) {
		/* We use a mask load depending on the mask value in a_gf2 */
		uint32_t len = (n - i) < 32 ? (n - i) : 32;
		uint32_t ceil_len_bits = (len + 7) / 8;
		/* Load 32 bits max */
		__m128i mask_128 = _mm_mask_loadu_epi8(zero_128, ((__mmask16)1 << ceil_len_bits) - 1, &a_gf2[(i / 8)]);
		/* Transfer to our 32 bits mask */
		__mmask64 mask64 = mask64_from_m128(mask_128) & (((__mmask64)1 << len) - 1);
		__mmask32 mask = (__mmask32)mask64;
		accu ^= _mm512_mask_loadu_epi16(zero, mask, (int const*)&b_gf256to2[i]);
	}

	return sum_uint16_avx512(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(256)
 */
static inline uint16_t gf256to2_gf2_vect_mult_avx512(const uint16_t *a_gf256to2, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256to2_vect_mult_avx512(b_gf2, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256^2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf2_gf256to2_mat_mult_avx512(const uint8_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF2_GF256to2_MAT_MULT(A, X, Y, n, mtype, gf2_gf256to2_vect_mult_avx512);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf2_mat_mult_avx512(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_GF2_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf2_vect_mult_avx512);
}

/*
 * GF(2^16) matrix multiplication
 */
static inline void gf256to2_mat_mult_avx512(const uint16_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	__m512i mul[GF_MULT_PANEL_BLOCKS][16];
	const uint8_t *Ab = (const uint8_t*)(const void*)A;
	const uint8_t *Xb = (const uint8_t*)(const void*)X;
	const uint32_t nbytes = 2 * n;
	const uint32_t nblocks = (nbytes + 63) / 64;
	uint32_t b, p;

	/* TRI_SUP starts each row at a different, non block aligned column, which
	 * does not fit the scheme below; it is unused by MQOM and is left on the
	 * generic path. */
	if (mtype == TRI_SUP) {
		GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256to2_vect_mult_avx512);
		return;
	}

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			uint32_t off = 64 * (p + b);
			__m512i xb;
			if ((off + 64) <= nbytes) {
				xb = _mm512_loadu_epi64((const __m512i*)(const void*)&Xb[off]);
			} else {
				xb = load_incomplete_m512(&Xb[off], nbytes - off);
			}
			gf256to2_precompute_multiples_avx512(xb, mul[b]);
		}

		gf256to2_mat_mult_panel_avx512(Ab, Y, n, p, np, (const __m512i (*)[16])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}


/* GF(256^2) matrix transposition */
static inline void gf256to2_mat_transpose_avx512(const uint16_t *A, uint16_t *B, uint32_t n, matrix_type mtype) {
	gf256to2_mat_transpose_ref(A, B, n, mtype);
}

/*
 * "Hybrid" constant multiplication of a constant in GF(16) and a vector in GF(256^2)
 */
static inline void gf16_gf256to2_constant_vect_mult_avx512(uint8_t a_gf16, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint8_t a_gf256;
	gf256_vect_lift_from_gf16_ref(&a_gf16, &a_gf256, 1);
	gf256_gf256to2_constant_vect_mult_avx512(a_gf256, b_gf256to2, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(16)
 */
static inline void gf256to2_gf16_constant_vect_mult_avx512(uint16_t a_gf256to2, const uint8_t *b_gf16, uint16_t *c_gf256to2, uint32_t n) {
	uint8_t *buf = ((uint8_t*) c_gf256to2) + n;
	gf256_vect_lift_from_gf16_avx512(b_gf16, buf, n);
	gf256to2_gf256_constant_vect_mult_avx512(a_gf256to2, buf, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256^2)
 */
static inline uint16_t gf16_gf256to2_vect_mult_avx512(const uint8_t *a_gf16, const uint16_t *b_gf256to2, uint32_t len) {
	uint32_t i;
	__m512i accu, _a, _b;
	__m512i _a_gf16;

	/* Set the accumulator to 0 */
	accu = _mm512_setzero_epi32();

	for (i = 0; i < (2 * len); i += 64) {
		const __m512i shuff_msk = _mm512_set_epi8(31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24,
		                          23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16,
		                          15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,
		                          7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2,  2,  1, 1, 0, 0);

		/* NOTE: the lifting is kept fully in registers here; the previous
		 * version round-tripped it through the stack on every block. */
		if (((2 * len) - i) < 64) {
			_a_gf16 = load_incomplete_m512((const uint8_t*)&a_gf16[i / 4], (((2 * len) - i) + 3) / 4);
			_a = gf256_expand_nibbles_avx512(_a_gf16, GF256_LIFT_FROM_GF16_LOOKUP_AVX512());
			/* Zero the lifted elements beyond the vector length */
			_a = gf_keep_prefix_avx512(_a, ((2 * len) - i + 1) / 2);
			_b = load_incomplete_m512((const uint8_t*)&b_gf256to2[i / 2], ((2 * len) - i));
		} else {
			/* Obvious 512-bit */
			_a_gf16 = load_incomplete_m512((const uint8_t*)&a_gf16[i / 4], 16);
			_a = gf256_expand_nibbles_avx512(_a_gf16, GF256_LIFT_FROM_GF16_LOOKUP_AVX512());
			_b = _mm512_loadu_epi64((__m512i*)&b_gf256to2[i / 2]);
		}
		/* Duplicate the values in _a */
		_a = _mm512_permutex2var_epi8(_a, shuff_msk, _a);
		/* Multiply in GF(256) */
		accu ^= gf256_mult_vectorized_avx512(_a, _b);
	}

	return sum_uint16_avx512(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(16)
 */
static inline uint16_t gf256to2_gf16_vect_mult_avx512(const uint16_t *a_gf256to2, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256to2_vect_mult_avx512(b_gf16, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(16), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf16_mat_mult_avx512(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	/* Each element of X is a GF(16) value lifted into the GF(256) subfield, so
	 * only the 8 multiples of the lifted-and-duplicated X block are needed.
	 * NOTE: like GF256to2_GF16_MAT_MULT (i.e. GF256_GF2_MAT_MULT), TRI_SUP is
	 * handled as REG here. */
	__m512i mul[GF_MULT_PANEL_BLOCKS][8];
	const __m512i dup_msk = _mm512_set_epi8(31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24,
	                                        23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16,
	                                        15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,
	                                        7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2,  2,  1, 1, 0, 0);
	const uint8_t *Ab = (const uint8_t*)(const void*)A;
	const uint32_t nbytes = 2 * n;
	const uint32_t nblocks = (nbytes + 63) / 64;
	uint32_t b, p;

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS) {
			np = GF_MULT_PANEL_BLOCKS;
		}

		for (b = 0; b < np; b++) {
			/* A 64-byte block covers 32 GF(256^2) elements, hence 32 GF(16)
			 * elements of X packed into 16 bytes. */
			uint32_t off = 64 * (p + b);
			uint32_t nelts = (nbytes - off) / 2;
			__m512i xb;
			if (nelts > 32) {
				nelts = 32;
			}
			xb = gf256_expand_nibbles_avx512(load_incomplete_m512(&X[off / 4], (nelts + 1) / 2),
			                                 GF256_LIFT_FROM_GF16_LOOKUP_AVX512());
			if (nelts < 32) {
				/* Zero pad past the end of the vector */
				xb = gf_keep_prefix_avx512(xb, nelts);
			}
			/* Duplicate each lifted byte over its 16-bit lane */
			xb = _mm512_permutex2var_epi8(xb, dup_msk, xb);
			gf256_precompute_multiples_avx512(xb, mul[b]);
		}

		gf256_mat_mult_panel_gf256to2_avx512(Ab, Y, n, p, np, (const __m512i (*)[8])mul, (mtype == TRI_INF), (p == 0));
	}

	return;
}

#endif /* defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512VL__) && defined(__AVX512VPOPCNTDQ__) */

#endif /* __FIELDS_AVX512_H__ */
