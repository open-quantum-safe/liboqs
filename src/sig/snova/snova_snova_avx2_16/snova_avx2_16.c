// SPDX-License-Identifier: MIT

/**
 * AVX2 and GFNI optimized q=16 implementation.
 *
 * This is a single file version of the source in ../core.
 *
 * Copyright (c) 2026 SNOVA TEAM
 */

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "snova.h"

#ifndef USE_GFNI
#define USE_GFNI __GFNI__
#endif

#define SNOVA_V SNOVA_v
#define SNOVA_O SNOVA_o
#define SNOVA_Q SNOVA_q
#define SNOVA_L SNOVA_l
#define SNOVA_R SNOVA_r
#define SNOVA_M1 SNOVA_m1
#define SNOVA_ARCH_X86_AVX2 1

/**
 * @file snova_params.h
 */

#ifndef SNOVA_V
#define SNOVA_V 24
#endif
#ifndef SNOVA_O
#define SNOVA_O 5
#endif
#ifndef SNOVA_L
#define SNOVA_L 4
#endif

#ifndef SNOVA_Q
#define SNOVA_Q 16
#endif

#if SNOVA_Q == 11
#define SNOVA_Q_A 0
#define SNOVA_Q_B 3
#define SNOVA_Q_C 6
#define SNOVA_PACK_GF 16
#define SNOVA_PACK_BYTES 7
#elif SNOVA_Q == 13
#define SNOVA_Q_A 2
#define SNOVA_Q_B 11
#define SNOVA_Q_C 3
#define SNOVA_PACK_GF 15
#define SNOVA_PACK_BYTES 7
#elif SNOVA_Q == 16
#define SNOVA_PACK_GF 2
#define SNOVA_PACK_BYTES 1
#elif SNOVA_Q == 19
#define SNOVA_Q_A 1
#define SNOVA_Q_B 3
#define SNOVA_Q_C 15
#define SNOVA_PACK_GF 15
#define SNOVA_PACK_BYTES 8
#else
#error "Unsupported SNOVA_Q (supported: 11, 13, 16, 19)"
#endif
#define SNOVA_BYTES_GF(x) ((SNOVA_PACK_BYTES * (x) + SNOVA_PACK_GF - 1) / SNOVA_PACK_GF)
#define SNOVA_REJECTION_LIMIT ((256 / SNOVA_Q) * SNOVA_Q)
#ifndef SNOVA_R
#define SNOVA_R SNOVA_L
#endif
#ifndef SNOVA_M1
#define SNOVA_M1 ((SNOVA_O * SNOVA_R) / SNOVA_L)
#endif
#define SNOVA_M2 (SNOVA_O * SNOVA_L * SNOVA_R)
#ifdef SNOVA_M2_ASSERT
_Static_assert(SNOVA_M2_ASSERT == (SNOVA_O * SNOVA_L * SNOVA_R),
               "SNOVA_M2 supplied by the build disagrees with the derived o*l*r "
               "(build configuration vs snova_params.h formula divergence)");
#endif

#ifndef FIXED_ABQ
#define FIXED_ABQ 1
#endif
#ifndef HASH_PK
#define HASH_PK (SNOVA_l > 2)
#endif
#ifndef SNOVA_BYTES_PK_HASH
#define SNOVA_BYTES_PK_HASH 48
#endif
#ifndef ROUND2_T12
#define ROUND2_T12 0
#endif
#ifndef ABQ_ALG2
#define ABQ_ALG2 1
#endif

#define SNOVA_SK_IS_SEED 1
#ifndef SNOVA_PK_EXPAND_SHAKE
#define SNOVA_PK_EXPAND_SHAKE 0
#endif

#define SNOVA_N      (SNOVA_V + SNOVA_O)
#define SNOVA_M      (SNOVA_O)
#define SNOVA_L2     (SNOVA_L * SNOVA_L)
#ifndef SNOVA_ALPHA
#define SNOVA_ALPHA  (SNOVA_L * SNOVA_R + 2 * SNOVA_R)
#endif
#define SNOVA_RANK   (SNOVA_L)
#define SNOVA_SQ_RANK (SNOVA_RANK * SNOVA_RANK)

#define SNOVA_SEED_PUB   16
#define SNOVA_SEED_PRIV  32
#define SNOVA_SEED_LEN   (SNOVA_SEED_PUB + SNOVA_SEED_PRIV)
#define SNOVA_SALT_BYTES 16

#define SNOVA_GF16S_HASH   (SNOVA_O * SNOVA_L * SNOVA_R)
#define SNOVA_GF16S_SIG    (SNOVA_N * SNOVA_L * SNOVA_R)
#if SNOVA_Q != 16
#define SNOVA_BYTES_HASH   (SNOVA_BYTES_GF(SNOVA_GF16S_HASH))
#define SNOVA_BYTES_SIG    (SNOVA_BYTES_GF(SNOVA_GF16S_SIG))
#else
#define SNOVA_BYTES_HASH   ((SNOVA_GF16S_HASH + 1) >> 1)
#define SNOVA_BYTES_SIG    ((SNOVA_GF16S_SIG + 1) >> 1)
#endif
#define SNOVA_BYTES_SIG_SALT (SNOVA_BYTES_SIG + SNOVA_SALT_BYTES)

// #define SNOVA__STR2(x) #x
// #define SNOVA__STR(x) SNOVA__STR2(x)
// #define SNOVA_NAME ("SNOVA_" SNOVA__STR(SNOVA_V) "_" SNOVA__STR(SNOVA_O) "_" SNOVA__STR(SNOVA_L))

#if SNOVA_R == SNOVA_L
_Static_assert((SNOVA_O * SNOVA_R) % SNOVA_L == 0,
               "seven-param: m1 = (o*r)/l must divide evenly for square cells");
_Static_assert(SNOVA_M1 == SNOVA_O,
               "seven-param: m1 must collapse to o for square cells");
_Static_assert(SNOVA_M2 == SNOVA_M * SNOVA_L2,
               "seven-param: m2 must equal o*l^2 (GAUSS_ROW invariant)");
#endif


// #include "gf16_core/gf16m.h"

#if SNOVA_Q != 16
#define SNOVA_PK_BYTES (SNOVA_SEED_PUB + SNOVA_BYTES_GF(SNOVA_M1*SNOVA_O*SNOVA_O*SNOVA_L2))
#else
#define SNOVA_PK_BYTES (SNOVA_SEED_PUB + ((SNOVA_M1*SNOVA_O*SNOVA_O*SNOVA_L2 + 1) >> 1))
#endif
#define SNOVA_SK_BYTES (SNOVA_SEED_LEN + SNOVA_BYTES_PK_HASH)
#define SNOVA_SM_OVERHEAD (SNOVA_BYTES_SIG + SNOVA_SALT_BYTES)


// #include "rng.h"
#include "symmetric.h"

// SNOVA_PK_EXPAND_SHAKE=0

#define SNOVA_WRAPPER_STACK 1
#define RCT_QRP16 1
#define RCT_MULLO 1
#define RCT_COEF_MULLO 1
#define RCT_SQ_CM_LEFT 1
#define RCT_SQ_CM_TIGHT 1
#define RCT_SQ_CM_S2 1
#define RCT_SQ_RIGHT_DUAL 1
#define RCT_SQ_CM_S3 1
#define RCT_T12_MULLO 1
#define RCT_KG_MULLO 1
#define RCT_OQ_WHIPVEC 1
#define RCT_OQ_DEVFLOW 1

// #include "snova_core/ct_poison.h"
#define SNOVA_CT_POISON(p, n) do { } while (0)
#define SNOVA_CT_DECLASSIFY(p, n) do { } while (0)
#define SNOVA_CT_ASSERT_PUBLIC_MEM(p, n) do { } while (0)

// #include "snova_core/secure_clear.h"
#define SNOVA_CLEAR(p, n) do { } while (0)
#define SNOVA_CLEAR_OBJ(x) do { } while (0)

#define SNOVA_DUDECT_RETRY_RESET() ((void)0)
#define SNOVA_DUDECT_RETRY_TICK()  ((void)0)

// #include "gf16_core/gf16.c"

//#include "gf16.h"
/**
 * @file gf16.h
 */
#ifndef M0_GF16_H
#define M0_GF16_H

typedef uint8_t gf16_t;

static inline gf16_t gf16_add(gf16_t a, gf16_t b) {
	return (gf16_t)(a ^ b);
}

#endif

// #include "xgf16.h"
/**
 * @file xgf16.h
 */
#ifndef M1_XGF16_H
#define M1_XGF16_H

#define XGF16_LANE_MASK 0x49249249u
#define XGF16_NIBBLE_MASK 0x249u

static inline uint32_t xgf16_spread(uint8_t a) {
	uint32_t m = (uint32_t)(a & 0x0F) | ((uint32_t)(a & 0x0F) << 4);
	return (m & 0x41u) | ((m << 2) & 0x208u);
}

static inline uint32_t xgf16_reduce(uint32_t v) {
	uint32_t r = v & XGF16_LANE_MASK;
	uint32_t u = r >> 12;
	r = r ^ u ^ (u << 3);
	u = r >> 12;
	r = r ^ u ^ (u << 3);
	u = r >> 12;
	r = r ^ u ^ (u << 3);
	return r & XGF16_NIBBLE_MASK;
}

static inline uint8_t xgf16_unspread(uint32_t v) {
	uint32_t r = v | (v >> 4);
	return (uint8_t)((r & 0x5u) | ((r >> 2) & 0xAu));
}

#endif

static gf16_t gf16_mul(gf16_t a, gf16_t b) {
	uint32_t p = xgf16_spread(a) * xgf16_spread(b);
	return xgf16_unspread(xgf16_reduce(p));
}

//#include "gf16_core/gf16m.c"
/**
 * @file gf16m.h
 */
#ifndef M0_GF16M_H
#define M0_GF16M_H

typedef gf16_t gf16m_t[SNOVA_SQ_RANK];

#define GF16M_AT(m, i, j) ((m)[(i) * SNOVA_RANK + (j)])

#endif
/**
 * @file gf16m.c
//  */
// #include "gf16m.h"
// #include "../gf16_spec.h"

#ifndef GF16_SPEC_H
#define GF16_SPEC_H

#define GF16_REDUCTION_POLY 0x13u

#define GF16_GENERATOR 0x2u

#define GF16_ORDER 16
#define GF16_MULORDER 15

#define GF16_S_ENTRY(i, j) ((unsigned char)(8 - ((i) + (j))))

#endif

__attribute__((unused))
static gf16_t det_cofactor(const gf16_t *M, int n) {
	if (n == 1) {
		return M[0];
	}
	if (n == 2) {
		return gf16_add(gf16_mul(M[0], M[3]), gf16_mul(M[1], M[2]));
	}
	gf16_t acc = 0;
	gf16_t minor[SNOVA_SQ_RANK];
	for (int col = 0; col < n; ++col) {
		int mi = 0;
		for (int i = 1; i < n; ++i)
			for (int j = 0; j < n; ++j) {
				if (j == col) {
					continue;
				}
				minor[mi++] = M[i * n + j];
			}
		acc = gf16_add(acc, gf16_mul(M[col], det_cofactor(minor, n - 1)));
	}
	return acc;
}

// #include "xgf16.h"

#if 0
static uint8_t gf16_mul_tab[256];

static inline gf16_t det_gauss_mul(gf16_t a, gf16_t b) {
	return gf16_mul_tab[((a & 0xF) << 4) | (b & 0xF)];
}

static const gf16_t gf16_inv_tab[16] = {
	0,  1,  9, 14, 13, 11,  7,  6,
	15, 2, 12,  5, 10,  4,  3,  8
};
static inline gf16_t det_gauss_inv(gf16_t a) {
	return gf16_inv_tab[a & 0x0F];
}
__attribute__((unused))
static gf16_t det_gauss(const gf16_t *M, int n) {
	gf16_t a[SNOVA_SQ_RANK];
	for (int i = 0; i < n * n; ++i) {
		a[i] = M[i];
	}
	gf16_t det = 1;
	for (int i = 0; i < n; ++i) {
		if (a[i * n + i] == 0) {
			int j;
			for (j = i + 1; j < n; ++j) if (a[j * n + i] != 0) {
					break;
				}
			if (j == n) {
				return 0;
			}
			for (int k = 0; k < n; ++k) {
				gf16_t t = a[i * n + k];
				a[i * n + k] = a[j * n + k];
				a[j * n + k] = t;
			}
		}
		gf16_t piv = a[i * n + i];
		det = det_gauss_mul(det, piv);
		gf16_t inv = det_gauss_inv(piv);
		for (int j = i + 1; j < n; ++j) {
			gf16_t f = det_gauss_mul(a[j * n + i], inv);
			if (f == 0) {
				continue;
			}
			for (int k = i; k < n; ++k) {
				a[j * n + k] = gf16_add(a[j * n + k], det_gauss_mul(f, a[i * n + k]));
			}
		}
	}
	return det;
}
#endif
// #include "xgf16.h"

#if SNOVA_RANK >= 2 && SNOVA_RANK <= 4 && 0
static inline gf16_t gf16_mul_inline(gf16_t a, gf16_t b) {
	return gf16_mul_tab[((a & 0xF) << 4) | (b & 0xF)];
}
#endif

#if SNOVA_RANK == 2
static inline gf16_t det_inline(const gf16m_t a) {
	return gf16_add(
	           gf16_mul_inline(GF16M_AT(a, 0, 0), GF16M_AT(a, 1, 1)),
	           gf16_mul_inline(GF16M_AT(a, 0, 1), GF16M_AT(a, 1, 0)));
}
#elif SNOVA_RANK == 3
static inline gf16_t det_inline(const gf16m_t a) {
	gf16_t d0 = gf16_mul_inline(GF16M_AT(a, 0, 0),
	                            gf16_add(gf16_mul_inline(GF16M_AT(a, 1, 1), GF16M_AT(a, 2, 2)),
	                                     gf16_mul_inline(GF16M_AT(a, 1, 2), GF16M_AT(a, 2, 1))));
	gf16_t d1 = gf16_mul_inline(GF16M_AT(a, 0, 1),
	                            gf16_add(gf16_mul_inline(GF16M_AT(a, 1, 0), GF16M_AT(a, 2, 2)),
	                                     gf16_mul_inline(GF16M_AT(a, 1, 2), GF16M_AT(a, 2, 0))));
	gf16_t d2 = gf16_mul_inline(GF16M_AT(a, 0, 2),
	                            gf16_add(gf16_mul_inline(GF16M_AT(a, 1, 0), GF16M_AT(a, 2, 1)),
	                                     gf16_mul_inline(GF16M_AT(a, 1, 1), GF16M_AT(a, 2, 0))));
	return gf16_add(gf16_add(d0, d1), d2);
}
#elif SNOVA_RANK == 4
#define POD4(a, i0, j0, i1, j1, i2, j2, i3, j3, i4, j4) \
    gf16_mul_inline(GF16M_AT(a, i0, j0), \
        gf16_add(gf16_mul_inline(GF16M_AT(a, i1, j1), GF16M_AT(a, i2, j2)), \
                 gf16_mul_inline(GF16M_AT(a, i3, j3), GF16M_AT(a, i4, j4))))
#if 0
static inline gf16_t det_inline(const gf16m_t a) {
	gf16_t d0 = gf16_mul_inline(GF16M_AT(a, 0, 0),
	                            gf16_add(gf16_add(POD4(a, 1, 1, 2, 2, 3, 3, 2, 3, 3, 2),
	                                POD4(a, 1, 2, 2, 1, 3, 3, 2, 3, 3, 1)),
	                                     POD4(a, 1, 3, 2, 1, 3, 2, 2, 2, 3, 1)));
	gf16_t d1 = gf16_mul_inline(GF16M_AT(a, 0, 1),
	                            gf16_add(gf16_add(POD4(a, 1, 0, 2, 2, 3, 3, 2, 3, 3, 2),
	                                POD4(a, 1, 2, 2, 0, 3, 3, 2, 3, 3, 0)),
	                                     POD4(a, 1, 3, 2, 0, 3, 2, 2, 2, 3, 0)));
	gf16_t d2 = gf16_mul_inline(GF16M_AT(a, 0, 2),
	                            gf16_add(gf16_add(POD4(a, 1, 0, 2, 1, 3, 3, 2, 3, 3, 1),
	                                POD4(a, 1, 1, 2, 0, 3, 3, 2, 3, 3, 0)),
	                                     POD4(a, 1, 3, 2, 0, 3, 1, 2, 1, 3, 0)));
	gf16_t d3 = gf16_mul_inline(GF16M_AT(a, 0, 3),
	                            gf16_add(gf16_add(POD4(a, 1, 0, 2, 1, 3, 2, 2, 2, 3, 1),
	                                POD4(a, 1, 1, 2, 0, 3, 2, 2, 2, 3, 0)),
	                                     POD4(a, 1, 2, 2, 0, 3, 1, 2, 1, 3, 0)));
	return gf16_add(gf16_add(d0, d1), gf16_add(d2, d3));
}
#elif SNOVA_RANK == 5 && 0
static inline gf16_t det3_n5(const gf16m_t a, int j0, int j1, int j2) {
	gf16_t t0 = gf16_mul_inline(GF16M_AT(a, 0, j0),
	                            gf16_add(gf16_mul_inline(GF16M_AT(a, 1, j1), GF16M_AT(a, 2, j2)),
	                                     gf16_mul_inline(GF16M_AT(a, 1, j2), GF16M_AT(a, 2, j1))));
	gf16_t t1 = gf16_mul_inline(GF16M_AT(a, 0, j1),
	                            gf16_add(gf16_mul_inline(GF16M_AT(a, 1, j0), GF16M_AT(a, 2, j2)),
	                                     gf16_mul_inline(GF16M_AT(a, 1, j2), GF16M_AT(a, 2, j0))));
	gf16_t t2 = gf16_mul_inline(GF16M_AT(a, 0, j2),
	                            gf16_add(gf16_mul_inline(GF16M_AT(a, 1, j0), GF16M_AT(a, 2, j1)),
	                                     gf16_mul_inline(GF16M_AT(a, 1, j1), GF16M_AT(a, 2, j0))));
	return gf16_add(gf16_add(t0, t1), t2);
}

static inline gf16_t det2_n5(const gf16m_t a, int j0, int j1) {
	return gf16_add(gf16_mul_inline(GF16M_AT(a, 3, j0), GF16M_AT(a, 4, j1)),
	                gf16_mul_inline(GF16M_AT(a, 3, j1), GF16M_AT(a, 4, j0)));
}

static inline gf16_t det_inline(const gf16m_t a) {
	gf16_t d012 = gf16_mul_inline(det3_n5(a, 0, 1, 2), det2_n5(a, 3, 4));
	gf16_t d013 = gf16_mul_inline(det3_n5(a, 0, 1, 3), det2_n5(a, 2, 4));
	gf16_t d014 = gf16_mul_inline(det3_n5(a, 0, 1, 4), det2_n5(a, 2, 3));
	gf16_t d023 = gf16_mul_inline(det3_n5(a, 0, 2, 3), det2_n5(a, 1, 4));
	gf16_t d024 = gf16_mul_inline(det3_n5(a, 0, 2, 4), det2_n5(a, 1, 3));
	gf16_t d034 = gf16_mul_inline(det3_n5(a, 0, 3, 4), det2_n5(a, 1, 2));
	gf16_t d123 = gf16_mul_inline(det3_n5(a, 1, 2, 3), det2_n5(a, 0, 4));
	gf16_t d124 = gf16_mul_inline(det3_n5(a, 1, 2, 4), det2_n5(a, 0, 3));
	gf16_t d134 = gf16_mul_inline(det3_n5(a, 1, 3, 4), det2_n5(a, 0, 2));
	gf16_t d234 = gf16_mul_inline(det3_n5(a, 2, 3, 4), det2_n5(a, 0, 1));
	return gf16_add(
	           gf16_add(gf16_add(gf16_add(d012, d013), gf16_add(d014, d023)),
	                    gf16_add(gf16_add(d024, d034), gf16_add(d123, d124))),
	           gf16_add(d134, d234));
}
#endif
#endif

/**
 * @file gf16_core/gf16_qrp16.h
 */
#ifndef SNOVA_GF16_QRP16_H
#define SNOVA_GF16_QRP16_H
#if 0
static inline uint64_t gf16_u64_mul(uint64_t a, uint64_t b) {
	uint64_t t = 0, mask;
	for (int i = 0; i < 4; i++) {
		mask = (b & 0x1111111111111111ull) * 0xf;
		t ^= (a & mask);
		mask = ((a >> 3) & 0x1111111111111111ull);
		a = ((a ^ (mask * 0x9)) << 1) ^ mask;
		b >>= 1;
	}
	return t;
}
#endif
#if defined(__AVX2__)
#include <immintrin.h>

#if 0
static inline __m256i gf16_qrp16_256_mul_(__m256i a, __m256i b, __m256i unit) {
	__m256i t = _mm256_setzero_si256();
	for (int i = 0; i < 4; i++) {
		__m256i lb = _mm256_and_si256(b, unit);
		__m256i mask = _mm256_or_si256(
		                   _mm256_or_si256(lb, _mm256_slli_epi64(lb, 1)),
		                   _mm256_or_si256(_mm256_slli_epi64(lb, 2), _mm256_slli_epi64(lb, 3)));
		t = _mm256_xor_si256(t, _mm256_and_si256(a, mask));
		__m256i m3 = _mm256_and_si256(_mm256_srli_epi64(a, 3), unit);
		a = _mm256_xor_si256(
		        _mm256_slli_epi64(
		            _mm256_xor_si256(a, _mm256_or_si256(m3, _mm256_slli_epi64(m3, 3))), 1),
		        m3);
		b = _mm256_srli_epi64(b, 1);
	}
	return t;
}

static inline __m256i gf16_qrp16_256_nib_mul(__m256i a, __m256i b) {
	return gf16_qrp16_256_mul_(a, b, _mm256_set1_epi64x(0x1111111111111111ll));
}

static inline __m128i gf16_qrp16_256_nib_outer4_pack(
    __m128i a0, __m128i a1, __m128i a2, __m128i a3,
    __m128i b0, __m128i b1, __m128i b2, __m128i b3) {
	const __m128i A_lo = _mm_or_si128(a0, _mm_slli_epi16(a1, 4));
	const __m128i A_hi = _mm_or_si128(a2, _mm_slli_epi16(a3, 4));
	const __m128i B_lo = _mm_or_si128(b0, _mm_slli_epi16(b1, 4));
	const __m128i B_hi = _mm_or_si128(b2, _mm_slli_epi16(b3, 4));
	const __m256i A = _mm256_set_m128i(A_hi, A_lo);
	const __m256i B = _mm256_set_m128i(B_hi, B_lo);
	const __m256i P = gf16_qrp16_256_nib_mul(A, B);
	return _mm_xor_si128(_mm256_castsi256_si128(P),
	                     _mm256_extracti128_si256(P, 1));
}
static inline __m128i gf16_nibpack_fold128(__m128i t) {
	const __m128i m = _mm_set1_epi8(0x0f);
	return _mm_xor_si128(_mm_and_si128(t, m),
	                     _mm_and_si128(_mm_srli_epi16(t, 4), m));
}
#endif
#if !USE_GFNI
static inline __m256i gf16_qrp16_256_byte_mul(__m256i a, __m256i b) {
	const __m256i unit = _mm256_set1_epi8(0x01);
	const __m256i zero = _mm256_setzero_si256();
	__m256i t = zero;
	for (int i = 0; i < 4; i++) {
		__m256i lb = _mm256_and_si256(b, unit);
		__m256i mask = _mm256_sub_epi8(zero, lb);
		t = _mm256_xor_si256(t, _mm256_and_si256(a, mask));
		__m256i m3 = _mm256_and_si256(_mm256_srli_epi64(a, 3), unit);
		a = _mm256_xor_si256(
		        _mm256_slli_epi64(
		            _mm256_xor_si256(a, _mm256_or_si256(m3, _mm256_slli_epi64(m3, 3))), 1),
		        m3);
		b = _mm256_srli_epi64(b, 1);
	}
	return t;
}
#endif
#if SNOVA_l == 4
#if 0
static inline __m128i gf16_qrp16_128_mul_(__m128i a, __m128i b, __m128i unit) {
	__m128i t = _mm_setzero_si128();
	for (int i = 0; i < 4; i++) {
		__m128i lb = _mm_and_si128(b, unit);
		__m128i mask = _mm_or_si128(
		                   _mm_or_si128(lb, _mm_slli_epi64(lb, 1)),
		                   _mm_or_si128(_mm_slli_epi64(lb, 2), _mm_slli_epi64(lb, 3)));
		t = _mm_xor_si128(t, _mm_and_si128(a, mask));
		__m128i m3 = _mm_and_si128(_mm_srli_epi64(a, 3), unit);
		a = _mm_xor_si128(
		        _mm_slli_epi64(
		            _mm_xor_si128(a, _mm_or_si128(m3, _mm_slli_epi64(m3, 3))), 1),
		        m3);
		b = _mm_srli_epi64(b, 1);
	}
	return t;
}
#endif
#if !USE_GFNI
static inline __m128i gf16_qrp16_128_byte_mul(__m128i a, __m128i b) {
	const __m128i unit = _mm_set1_epi8(0x01);
	const __m128i zero = _mm_setzero_si128();
	__m128i t = zero;
	for (int i = 0; i < 4; i++) {
		__m128i lb = _mm_and_si128(b, unit);
		__m128i mask = _mm_sub_epi8(zero, lb);
		t = _mm_xor_si128(t, _mm_and_si128(a, mask));
		__m128i m3 = _mm_and_si128(_mm_srli_epi64(a, 3), unit);
		a = _mm_xor_si128(
		        _mm_slli_epi64(
		            _mm_xor_si128(a, _mm_or_si128(m3, _mm_slli_epi64(m3, 3))), 1),
		        m3);
		b = _mm_srli_epi64(b, 1);
	}
	return t;
}
#endif
#endif

#endif

#endif

//#include "snova_core/snova_rect.h"
/**
 * @file snova_rect.h
 */
#ifndef SNOVA_RECT_H
#define SNOVA_RECT_H

#if !FIXED_ABQ
#error "rct core requires FIXED_ABQ=1 (R3): expand_public never writes the ABQ tail of P_matrix"
#endif

#if SNOVA_WRAPPER_STACK
#define RCT_SCRATCH
#else
#define RCT_SCRATCH static
#endif

typedef uint8_t gf_t;

#define CRYPTO_BYTES_R  (BYTES_SIGNATURE)

static gf_t rct_multtab[SNOVA_q * SNOVA_q];
static gf_t rct_addtab[SNOVA_q * SNOVA_q];
static gf_t rct_S[SNOVA_l * SNOVA_l2];

static inline gf_t gf_mult(const gf_t a, const gf_t b) {
	return rct_multtab[a * SNOVA_q + b];
}

static inline gf_t gf_add(const gf_t a, const gf_t b) {
#if SNOVA_q != 16
	unsigned t = (unsigned)a + b;
	return (gf_t)(t - (SNOVA_q & (0u - (unsigned)(t >= SNOVA_q))));
#else
	return (gf_t)(a ^ b);
#endif
}
static inline void gf_set_add(gf_t *a, const gf_t b) {
	*a = gf_add(*a, b);
}
static inline gf_t gf_sub(const gf_t a, const gf_t b) {
#if SNOVA_q != 16
	unsigned t = (unsigned)a + SNOVA_q - b;
	return (gf_t)(t - (SNOVA_q & (0u - (unsigned)(t >= SNOVA_q))));
#else
	return (gf_t)(a ^ b);
#endif
}

#if SNOVA_q == 16
//#include "../gf16_core/xgf16.h"
#endif
static inline gf_t gf_mult_sec(const gf_t a, const gf_t b) {
#if SNOVA_q != 16
	return (gf_t)(((unsigned)a * b) % SNOVA_q);
#else
	return (gf_t)xgf16_unspread(xgf16_reduce(xgf16_spread(a) * xgf16_spread(b)));
#endif
}
static inline gf_t gf_inv_sec(const gf_t a) {
#if SNOVA_q == 16
	uint32_t a0 = a & 1u, a1 = (a >> 1) & 1u, a2 = (a >> 2) & 1u, a3 = (a >> 3) & 1u;
	gf_t s2  = (gf_t)((a0 ^ a2) | (a2 << 1) | ((a1 ^ a3) << 2) | (a3 << 3));
	gf_t s3  = gf_mult_sec(s2, a);
	uint32_t b0 = s3 & 1u, b1 = (s3 >> 1) & 1u, b2 = (s3 >> 2) & 1u, b3 = (s3 >> 3) & 1u;
	gf_t s6  = (gf_t)((b0 ^ b2) | (b2 << 1) | ((b1 ^ b3) << 2) | (b3 << 3));
	uint32_t c0 = s6 & 1u, c1 = (s6 >> 1) & 1u, c2 = (s6 >> 2) & 1u, c3 = (s6 >> 3) & 1u;
	gf_t s12 = (gf_t)((c0 ^ c2) | (c2 << 1) | ((c1 ^ c3) << 2) | (c3 << 3));
	return gf_mult_sec(s12, s2);
#else
	gf_t val = a;
	for (int j1 = 3; j1 < SNOVA_q; j1++) {
		val = gf_mult_sec(val, a);
	}
	return val;
#endif
}
static inline uint32_t ct_gf_nz(const uint32_t v) {
	return (0u - v) >> 31;
}
#if 0
static inline gf_t ct_gf_sel(const uint32_t cond, const gf_t a, const gf_t b) {
	uint32_t m = 0u - cond;
	return (gf_t)((a & m) | (b & ~m));
}
#endif

#if defined(__AVX2__) && (SNOVA_Q == 16) && !defined(RCT_FORCE_SCALAR)
#define RCT_AVX2_Q16 1
#include <immintrin.h>
//#include "gf16_core/gf16_qrp16.h"
#define SNOVA_lr16 (((SNOVA_lr) + 31) / 32)
#define SNOVA_lr32 (SNOVA_lr16 * 32)
#if USE_GFNI && !defined(RCT_FORCE_PSHUFB)
#define RCT_HAVE_GFNI 1
#else
#define RCT_HAVE_GFNI 0
#endif
#else
#define RCT_AVX2_Q16 0
#define RCT_HAVE_GFNI 0
#endif

#if RCT_AVX2_Q16 && (SNOVA_l == 4)
#if RCT_HAVE_GFNI
#define RCT_USE_GFNI 1
#define RCT_USE_PSHUFB 0
#else
#define RCT_USE_GFNI 0
#define RCT_USE_PSHUFB 1
#endif
#else
#define RCT_USE_GFNI 0
#define RCT_USE_PSHUFB 0
#endif
#define RCT_USE_SIMD (RCT_USE_GFNI || RCT_USE_PSHUFB)

#if RCT_AVX2_Q16 && ((SNOVA_l != 4) || defined(RCT_FORCE_JOG))
#define RCT_VF_JOG 1
#else
#define RCT_VF_JOG 0
#endif

#if RCT_AVX2_Q16 && (SNOVA_l != 4)
#define RCT_SIGN_JOG 1
#else
#define RCT_SIGN_JOG 0
#endif


//#include "platforms/x86_avx2/rct_sign_engine.h"
#ifndef RCT_SIGN_ENGINE_H
#define RCT_SIGN_ENGINE_H
#if RCT_SIGN_JOG
#if USE_GFNI
static inline __m128i rct_sj_cleanup128(__m128i v) {
	const __m128i m0f = _mm_set1_epi8(0x0f);
	__m128i vhi = _mm_and_si128(v, _mm_set1_epi8((char)0xf0));
	__m128i a = _mm_srli_epi16(vhi, 3);
	__m128i b = _mm_srli_epi16(v, 4);
	return _mm_and_si128(_mm_xor_si128(_mm_xor_si128(v, a), b), m0f);
}
#endif
static inline __m256i rct_sj_cleanup256(__m256i v) {
	const __m256i m0f = _mm256_set1_epi8(0x0f);
	__m256i vhi = _mm256_and_si256(v, _mm256_set1_epi8((char)0xf0));
	__m256i a = _mm256_srli_epi16(vhi, 3);
	__m256i b = _mm256_srli_epi16(v, 4);
	return _mm256_and_si256(_mm256_xor_si256(_mm256_xor_si256(v, a), b), m0f);
}

#if RCT_HAVE_GFNI
static inline __m128i rct_sj_bc128(gf_t s) {
	return _mm_set1_epi8((char)s);
}
static inline __m128i rct_sj_sv128(__m128i bc, __m128i v) {
	return _mm_gf2p8mul_epi8(bc, v);
}
#define RCT_SJ_CLEAN(v) rct_sj_cleanup128(v)
static inline __m256i rct_sj_bc256(gf_t s) {
	return _mm256_set1_epi8((char)s);
}
static inline __m256i rct_sj_sv256(__m256i bc, __m256i v) {
	return _mm256_gf2p8mul_epi8(bc, v);
}
#define RCT_SJ_CLEAN256(v) rct_sj_cleanup256(v)
static inline void rct_sj_ensure(void) {}
#else
static __m128i rct_sj_vtl[16];
static int rct_sj_vtl_done = 0;
static inline void rct_sj_ensure(void) {
	if (rct_sj_vtl_done) {
		return;
	}
	for (int k = 0; k < 16; ++k) {
		_Alignas(16) uint8_t t[16];
		for (int x = 0; x < 16; ++x) {
			t[x] = gf_mult((gf_t)k, (gf_t)x);
		}
		rct_sj_vtl[k] = _mm_load_si128((const __m128i *)t);
	}
	rct_sj_vtl_done = 1;
}
static inline __m128i rct_sj_bc128(gf_t s) {
	const __m128i sv = _mm_set1_epi8((char)(s & 0x0F));
	const __m128i b1 = _mm_set1_epi8(1), b2 = _mm_set1_epi8(2),
	              b4 = _mm_set1_epi8(4), b8 = _mm_set1_epi8(8);
	__m128i r = _mm_and_si128(rct_sj_vtl[1], _mm_cmpeq_epi8(_mm_and_si128(sv, b1), b1));
	r = _mm_xor_si128(r, _mm_and_si128(rct_sj_vtl[2], _mm_cmpeq_epi8(_mm_and_si128(sv, b2), b2)));
	r = _mm_xor_si128(r, _mm_and_si128(rct_sj_vtl[4], _mm_cmpeq_epi8(_mm_and_si128(sv, b4), b4)));
	r = _mm_xor_si128(r, _mm_and_si128(rct_sj_vtl[8], _mm_cmpeq_epi8(_mm_and_si128(sv, b8), b8)));
	return r;
}
static inline __m128i rct_sj_sv128(__m128i bc, __m128i v) {
	return _mm_shuffle_epi8(bc, v);
}
#define RCT_SJ_CLEAN(v) (v)
static inline __m256i rct_sj_bc256(gf_t s) {
	__m128i b = rct_sj_bc128(s);
	return _mm256_set_m128i(b, b);
}
static inline __m256i rct_sj_sv256(__m256i bc, __m256i v) {
	return _mm256_shuffle_epi8(bc, v);
}
#define RCT_SJ_CLEAN256(v) (v)
#endif

static inline __m128i rct_sj_bc128_pub(gf_t s) {
	SNOVA_CT_ASSERT_PUBLIC_MEM(&s, sizeof s);
#if RCT_HAVE_GFNI
	return _mm_set1_epi8((char)s);
#else
	return rct_sj_vtl[s & 0x0F];
#endif
}
#if SNOVA_r < 8
static inline __m256i rct_sj_bc256_pub(gf_t s) {
	SNOVA_CT_ASSERT_PUBLIC_MEM(&s, sizeof s);
#if RCT_HAVE_GFNI
	return _mm256_set1_epi8((char)s);
#else
	return _mm256_broadcastsi128_si256(rct_sj_vtl[s & 0x0F]);
#endif
}
#endif
static inline void rct_sj_store_r(gf_t *dst, __m128i acc) {
	_Alignas(16) uint8_t tmp[16];
	_mm_store_si128((__m128i *)tmp, RCT_SJ_CLEAN(acc));
	memcpy(dst, tmp, SNOVA_r);
}

static inline void rct_sj_mm_add(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd, int cd) {
	for (int i = 0; i < ad; ++i) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; ++k)
			acc = _mm_xor_si128(acc, rct_sj_sv128(rct_sj_bc128(A[i * bd + k]),
			                                      _mm_loadu_si128((const __m128i *)&B[k * cd])));
		_Alignas(16) uint8_t pb[16];
		_mm_store_si128((__m128i *)pb, RCT_SJ_CLEAN(acc));
		for (int j = 0; j < cd; ++j) {
			C[i * cd + j] = (gf_t)(C[i * cd + j] ^ pb[j]);
		}
	}
}

static inline void rct_sj_mm_add_pub(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd, int cd) {
	for (int i = 0; i < ad; ++i) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; ++k)
			acc = _mm_xor_si128(acc, rct_sj_sv128(rct_sj_bc128_pub(A[i * bd + k]),
			                                      _mm_loadu_si128((const __m128i *)&B[k * cd])));
		_Alignas(16) uint8_t pb[16];
		_mm_store_si128((__m128i *)pb, RCT_SJ_CLEAN(acc));
		for (int j = 0; j < cd; ++j) {
			C[i * cd + j] = (gf_t)(C[i * cd + j] ^ pb[j]);
		}
	}
}

#if RCT_HAVE_GFNI && SNOVA_l == 5 && (SNOVA_r == 6 || SNOVA_r == 8)
#define RCT_SJ_A4 1
#if SNOVA_r == 8
typedef struct {
	__m256i bv;
	__m128i bt;
} rct_a4_bc_t;
typedef struct {
	__m256i a;
	__m128i t;
} rct_a4_acc_t;
static inline rct_a4_acc_t rct_a4_zero(void) {
	rct_a4_acc_t z;
	z.a = _mm256_setzero_si256();
	z.t = _mm_setzero_si128();
	return z;
}
static inline rct_a4_bc_t rct_a4_bc(const gf_t *sc) {
	const __m256i PAT = _mm256_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	                                     2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3);
	rct_a4_bc_t b;
	b.bv = _mm256_shuffle_epi8(_mm256_broadcastsi128_si256(
	                               _mm_loadu_si128((const __m128i *)sc)), PAT);
	b.bt = _mm_set1_epi8((char)sc[4]);
	return b;
}
static inline void rct_a4_mac(rct_a4_acc_t *A, rct_a4_bc_t b, const gf_t *w) {
	A->a = _mm256_xor_si256(A->a, _mm256_gf2p8mul_epi8(b.bv,
	    _mm256_loadu_si256((const __m256i *)w)));
	A->t = _mm_xor_si128(A->t, _mm_gf2p8mul_epi8(b.bt,
	    _mm_loadu_si128((const __m128i *)&w[32])));
}
static inline __m128i rct_a4_fold(rct_a4_acc_t A) {
	__m128i f = _mm_xor_si128(_mm256_castsi256_si128(A.a),
	                          _mm256_extracti128_si256(A.a, 1));
	f = _mm_xor_si128(f, _mm_unpackhi_epi64(f, f));
	return _mm_xor_si128(f, A.t);
}
#elif SNOVA_r == 6
typedef struct {
	__m128i b01, b23, bt;
} rct_a4_bc_t;
typedef struct {
	__m128i a01, a23, t;
} rct_a4_acc_t;
#if SNOVA_l == SNOVA_r
static inline rct_a4_acc_t rct_a4_zero(void) {
	rct_a4_acc_t z;
	z.a01 = z.a23 = z.t = _mm_setzero_si128();
	return z;
}
static inline rct_a4_bc_t rct_a4_bc(const gf_t *sc) {
	const __m128i P01 = _mm_setr_epi8(0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1);
	const __m128i P23 = _mm_setr_epi8(2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, -1, -1, -1, -1);
	__m128i v = _mm_loadu_si128((const __m128i *)sc);
	rct_a4_bc_t b;
	b.b01 = _mm_shuffle_epi8(v, P01);
	b.b23 = _mm_shuffle_epi8(v, P23);
	b.bt  = _mm_set1_epi8((char)sc[4]);
	return b;
}
static inline void rct_a4_mac(rct_a4_acc_t *A, rct_a4_bc_t b, const gf_t *w) {
	A->a01 = _mm_xor_si128(A->a01, _mm_gf2p8mul_epi8(b.b01,
	    _mm_loadu_si128((const __m128i *)w)));
	A->a23 = _mm_xor_si128(A->a23, _mm_gf2p8mul_epi8(b.b23,
	    _mm_loadu_si128((const __m128i *)&w[12])));
	A->t   = _mm_xor_si128(A->t, _mm_gf2p8mul_epi8(b.bt,
	    _mm_loadu_si128((const __m128i *)&w[24])));
}
static inline __m128i rct_a4_fold(rct_a4_acc_t A) {
	__m128i f = _mm_xor_si128(A.a01, A.a23);
	f = _mm_xor_si128(f, _mm_srli_si128(f, 6));
	return _mm_xor_si128(f, A.t);
}
#endif
#else
typedef struct {
	__m128i b012, b34;
} rct_a4_bc_t;
typedef struct {
	__m128i a, b;
} rct_a4_acc_t;
static inline rct_a4_acc_t rct_a4_zero(void) {
	rct_a4_acc_t z;
	z.a = z.b = _mm_setzero_si128();
	return z;
}
static inline rct_a4_bc_t rct_a4_bc(const gf_t *sc) {
	const __m128i P012 = _mm_setr_epi8(0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, -1);
	const __m128i P34  = _mm_setr_epi8(3, 3, 3, 3, 3, 4, 4, 4, 4, 4, -1, -1, -1, -1, -1, -1);
	__m128i v = _mm_loadu_si128((const __m128i *)sc);
	rct_a4_bc_t b;
	b.b012 = _mm_shuffle_epi8(v, P012);
	b.b34  = _mm_shuffle_epi8(v, P34);
	return b;
}
static inline void rct_a4_mac(rct_a4_acc_t *A, rct_a4_bc_t b, const gf_t *w) {
	A->a = _mm_xor_si128(A->a, _mm_gf2p8mul_epi8(b.b012,
	    _mm_loadu_si128((const __m128i *)w)));
	A->b = _mm_xor_si128(A->b, _mm_gf2p8mul_epi8(b.b34,
	    _mm_loadu_si128((const __m128i *)&w[15])));
}
static inline __m128i rct_a4_fold(rct_a4_acc_t A) {
	__m128i f = _mm_xor_si128(A.a, _mm_srli_si128(A.a, 5));
	f = _mm_xor_si128(f, _mm_srli_si128(A.a, 10));
	f = _mm_xor_si128(f, A.b);
	return _mm_xor_si128(f, _mm_srli_si128(A.b, 5));
}
#endif
#else
#define RCT_SJ_A4 0
#endif

#if RCT_SIGN_JOG && !RCT_HAVE_GFNI && SNOVA_l == 5 && (SNOVA_r == 5 || SNOVA_r == 6 || SNOVA_r == 8)
#define RCT_SJ_M4 1
// #include "gf16_core/gf16_mullo16.h"
/**
 * @file gf16_core/gf16_mullo16.h
 */
#ifndef SNOVA_GF16_CORE_GF16_MULLO16_H
#define SNOVA_GF16_CORE_GF16_MULLO16_H

#include <stdint.h>
#include <immintrin.h>

static inline __m256i cl_gf16_expand_u16x16(__m256i x) {
	__m256i v = _mm256_or_si256(_mm256_or_si256(x, _mm256_slli_epi16(x, 3)),
	                            _mm256_or_si256(_mm256_slli_epi16(x, 6), _mm256_slli_epi16(x, 9)));
	return _mm256_and_si256(v, _mm256_set1_epi16(0x1111));
}
static inline __m256i cl_gf16_compress_u16x16(__m256i a) {
	const __m256i m0f = _mm256_set1_epi16(0x000f);
	__m256i val = _mm256_xor_si256(
	                  _mm256_xor_si256(_mm256_and_si256(a, m0f),
	                                   _mm256_srli_epi16(_mm256_and_si256(a, _mm256_set1_epi16(0x00f0)), 3)),
	                  _mm256_xor_si256(_mm256_srli_epi16(_mm256_and_si256(a, _mm256_set1_epi16(0x0f00)), 6),
	                                   _mm256_srli_epi16(_mm256_and_si256(a, _mm256_set1_epi16((short)0xf000)), 9)));
	val = _mm256_xor_si256(_mm256_xor_si256(val,
	                                        _mm256_srli_epi16(_mm256_and_si256(val, _mm256_set1_epi16(0x00f0)), 3)),
	                       _mm256_srli_epi16(val, 4));
	return _mm256_and_si256(val, m0f);
}
static inline __m128i cl_gf16_pack_u16_to_bytes(__m256i c) {
	return _mm_packus_epi16(_mm256_castsi256_si128(c), _mm256_extracti128_si256(c, 1));
}

static inline uint16_t cl_expand_scalar16(uint8_t x) {
	return (uint16_t)((x & 1) | ((x & 2) << 3) | ((x & 4) << 6) | ((x & 8) << 9));
}

#endif


#if SNOVA_r == 5
#define RCT_M4_PAT0 _mm256_setr_epi8(0,1,0,1,0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,4,5,4,5,4,5,4,5,4,5,6,7)
#define RCT_M4_PAT1 _mm256_setr_epi8(6,7,6,7,6,7,6,7,8,9,8,9,8,9,8,9,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1)
#endif
#if SNOVA_r == 6
#define RCT_M4_PAT0 _mm256_setr_epi8(0,1,0,1,0,1,0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,2,3,4,5,4,5,4,5,4,5)
#define RCT_M4_PAT1 _mm256_setr_epi8(4,5,4,5,6,7,6,7,6,7,6,7,6,7,6,7,8,9,8,9,8,9,8,9,8,9,8,9,-1,-1,-1,-1)
#endif
#if SNOVA_r == 8
#define RCT_M4_PAT0 _mm256_setr_epi8(0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3)
#define RCT_M4_PAT1 _mm256_setr_epi8(4,5,4,5,4,5,4,5,4,5,4,5,4,5,4,5,6,7,6,7,6,7,6,7,6,7,6,7,6,7,6,7)
#endif

typedef struct {
	__m256i b0, b1;
#if SNOVA_r == 8
	__m256i b2;
#endif
} rct_m4_bc_t;
typedef struct {
	__m256i a0, a1;
#if SNOVA_r == 8
	__m256i a2;
#endif
} rct_m4_acc_t;
static inline rct_m4_acc_t rct_m4_zero(void) {
	rct_m4_acc_t z;
	z.a0 = z.a1 = _mm256_setzero_si256();
#if SNOVA_r == 8
	z.a2 = _mm256_setzero_si256();
#endif
	return z;
}
#if 0
static inline rct_m4_bc_t rct_m4_bc(const gf_t *sc) {
	_Alignas(16) uint16_t ev[8];
	for (int k = 0; k < 5; ++k) {
		ev[k] = cl_expand_scalar16(sc[k]);
	}
	__m256i evb = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i *)ev));
	rct_m4_bc_t b;
	b.b0 = _mm256_shuffle_epi8(evb, RCT_M4_PAT0);
	b.b1 = _mm256_shuffle_epi8(evb, RCT_M4_PAT1);
#if SNOVA_r == 8
	b.b2 = _mm256_set1_epi16((short)ev[4]);
#endif
	return b;
}
#endif
static inline void rct_m4_mac(rct_m4_acc_t *A, rct_m4_bc_t b, const gf_t *w) {
	A->a0 = _mm256_xor_si256(A->a0, _mm256_mullo_epi16(b.b0,
	    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)w))));
	A->a1 = _mm256_xor_si256(A->a1, _mm256_mullo_epi16(b.b1,
	    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)&w[16]))));
#if SNOVA_r == 8
	A->a2 = _mm256_xor_si256(A->a2, _mm256_mullo_epi16(b.b2,
	    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)&w[32]))));
#endif
}
#if 0
static inline __m128i rct_m4_fold(rct_m4_acc_t A) {
	_Alignas(32) uint16_t tb[48];
	_mm256_store_si256((__m256i *)tb, A.a0);
	_mm256_store_si256((__m256i *)&tb[16], A.a1);
#if SNOVA_r == 8
	_mm256_store_si256((__m256i *)&tb[32], A.a2);
#else
	_mm256_store_si256((__m256i *)&tb[32], _mm256_setzero_si256());
#endif
	__m128i f = _mm_loadu_si128((const __m128i *)tb);
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[SNOVA_r]));
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[2 * SNOVA_r]));
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[3 * SNOVA_r]));
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[4 * SNOVA_r]));
	__m256i c = cl_gf16_compress_u16x16(_mm256_castsi128_si256(f));
	return cl_gf16_pack_u16_to_bytes(c);
}
#endif
#else
#define RCT_SJ_M4 0
#endif

#if RCT_SIGN_JOG && !RCT_HAVE_GFNI && SNOVA_l == 5
// #include "gf16_core/gf16_mullo16.h"
static inline void rct_m4_expand_buf(uint16_t *dst, const gf_t *src, int n) {
	for (int i = 0; i < n; i += 16)
		_mm256_storeu_si256((__m256i *)&dst[i], cl_gf16_expand_u16x16(
		                        _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)&src[i]))));
}
static inline void rct_m4_raw_buf(uint16_t *dst, const gf_t *src, int n) {
	for (int i = 0; i < n; i += 16)
		_mm256_storeu_si256((__m256i *)&dst[i],
		                    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)&src[i])));
}
static inline __m128i rct_m4_compress128(__m128i a) {
	const __m128i m0f = _mm_set1_epi16(0x000f);
	__m128i val = _mm_xor_si128(
	                  _mm_xor_si128(_mm_and_si128(a, m0f),
	                                _mm_srli_epi16(_mm_and_si128(a, _mm_set1_epi16(0x00f0)), 3)),
	                  _mm_xor_si128(_mm_srli_epi16(_mm_and_si128(a, _mm_set1_epi16(0x0f00)), 6),
	                                _mm_srli_epi16(_mm_and_si128(a, _mm_set1_epi16((short)0xf000)), 9)));
	val = _mm_xor_si128(_mm_xor_si128(val,
	                                  _mm_srli_epi16(_mm_and_si128(val, _mm_set1_epi16(0x00f0)), 3)),
	                    _mm_srli_epi16(val, 4));
	return _mm_and_si128(val, m0f);
}
#endif

#if RCT_SJ_M4
static inline void rct_m4_mm_add(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd, int cd) {
	_Alignas(32) uint16_t xe[64];
	rct_m4_expand_buf(xe, A, ad * bd);
	for (int i = 0; i < ad; ++i) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; ++k)
			acc = _mm_xor_si128(acc, _mm_mullo_epi16(_mm_set1_epi16((short)xe[i * bd + k]),
			    _mm256_castsi256_si128(_mm256_cvtepu8_epi16(
			                               _mm_loadu_si128((const __m128i *)&B[k * cd])))));
		_Alignas(16) uint8_t pb[16];
		_mm_store_si128((__m128i *)pb, _mm_packus_epi16(rct_m4_compress128(acc), _mm_setzero_si128()));
		for (int j = 0; j < cd; ++j) {
			C[i * cd + j] = (gf_t)(C[i * cd + j] ^ pb[j]);
		}
	}
}
#endif

_Static_assert(SNOVA_r <= 16, "RCT_SIGN_JOG engine requires r <= 16 (16-byte slot/lane)");
_Static_assert(SNOVA_l <= 16, "RCT_SIGN_JOG engine requires l <= 16 (16-byte slot/lane)");

static inline void rct_gauss_row_scale_sec(gf_t *row, gf_t s, int k0, int kend) {
	__m256i bs = rct_sj_bc256(s);
	for (int k = k0; k < kend; k += 32)
		_mm256_storeu_si256((__m256i *)&row[k],
		                    RCT_SJ_CLEAN256(rct_sj_sv256(bs, _mm256_loadu_si256((const __m256i *)&row[k]))));
}
static inline void rct_gauss_row_axpy_sec(gf_t *dst, const gf_t *src, gf_t s, int k0, int kend) {
	__m256i bs = rct_sj_bc256(s);
	for (int k = k0; k < kend; k += 32)
		_mm256_storeu_si256((__m256i *)&dst[k], _mm256_xor_si256(
		                        _mm256_loadu_si256((const __m256i *)&dst[k]),
		                        RCT_SJ_CLEAN256(rct_sj_sv256(bs, _mm256_loadu_si256((const __m256i *)&src[k])))));
}
#endif
#endif


#if RCT_VF_JOG
#define RCT_JOG_NL (SNOVA_n * SNOVA_l)
#define RCT_JOG_VTL ((RCT_JOG_NL + 31) / 32)
#define RCT_JOG_L32 (RCT_JOG_VTL * 32)
#define RCT_JOG_RP ((SNOVA_r + 1) / 2)
#if SNOVA_l != 4
#define RCT_JOG_PKXJOG 1
#else
#define RCT_JOG_PKXJOG 0
#endif
#else
#define RCT_JOG_PKXJOG 0
#endif

#if !defined(RCT_TILE4)
#if RCT_VF_JOG && RCT_HAVE_GFNI && (SNOVA_l == 5) && (SNOVA_q == 16) && (SNOVA_lr <= 32)
#define RCT_TILE4 1
#else
#define RCT_TILE4 0
#endif
#endif
#if (RCT_TILE4 + 0) && RCT_VF_JOG && RCT_HAVE_GFNI && (SNOVA_l == 5) && (SNOVA_q == 16)
#define RCT_VF_TILE4 1
#else
#define RCT_VF_TILE4 0
#endif

#if defined(RCT_OQ_WHIPVEC) && (RCT_OQ_WHIPVEC + 0) && (SNOVA_q != 16)
#define RCT_OQWV 1
#else
#define RCT_OQWV 0
#endif

#if RCT_USE_PSHUFB && defined(RCT_QRP16) && (RCT_QRP16 + 0) && (SNOVA_l == 4)
#define RCT_HOT_QRP16 1
#else
#define RCT_HOT_QRP16 0
#endif
#if RCT_USE_GFNI
#define RCT_GFMUL256(a, b) _mm256_gf2p8mul_epi8((a), (b))
#define RCT_GFMUL128(a, b) _mm_gf2p8mul_epi8((a), (b))
#elif RCT_HOT_QRP16
#define RCT_GFMUL256(a, b) gf16_qrp16_256_byte_mul((a), (b))
#define RCT_GFMUL128(a, b) gf16_qrp16_128_byte_mul((a), (b))
#elif RCT_SIGN_JOG && RCT_HAVE_GFNI
#define RCT_GFMUL256(a, b) _mm256_gf2p8mul_epi8((a), (b))
#define RCT_GFMUL128(a, b) _mm_gf2p8mul_epi8((a), (b))
#elif RCT_SIGN_JOG && defined(RCT_QRP16) && (RCT_QRP16 + 0)
#define RCT_GFMUL256(a, b) gf16_qrp16_256_byte_mul((a), (b))
#define RCT_GFMUL128(a, b) gf16_qrp16_128_byte_mul((a), (b))
#endif
#ifdef RCT_GFMUL256
#define RCT_GFMUL_ANY 1
#else
#define RCT_GFMUL_ANY 0
#endif

#if defined(SNOVA_SIGN_STREAM) && (SNOVA_SIGN_STREAM + 0)
#if SNOVA_q != 16
#error "SNOVA_SIGN_STREAM is q16-only: the T12 block-transpose fold identities assume S-symmetry (q16); odd-q needs a cell-transpose re-verify first — build without SIGN_STREAM=1"
#endif
#if !SNOVA_PK_EXPAND_SHAKE && !defined(SNOVA_ARCH_X86_AVX2)
#error "SNOVA_SIGN_STREAM AES path needs aes128_ctr_zero_at (AVX2); build with AES=0 (SHAKE XOF) or ARCH=x86_avx2"
#endif
#define RCT_SIGN_STREAM 0
#else
#define RCT_SIGN_STREAM 0
#endif

#if defined(SNOVA_KEYGEN_STREAM) && (SNOVA_KEYGEN_STREAM + 0)
#if !SNOVA_PK_EXPAND_SHAKE && !defined(SNOVA_ARCH_X86_AVX2)
#error "SNOVA_KEYGEN_STREAM AES path needs aes128_ctr_zero_at (AVX2); build with AES=0 (SHAKE XOF) or ARCH=x86_avx2"
#endif
#define RCT_KG_STREAM 1
#else
#define RCT_KG_STREAM 0
#endif

#ifdef RCT_BASELINE_SCALAR_HOT
#define RCT_HOT_SIMD RCT_USE_GFNI
#else
#define RCT_HOT_SIMD RCT_USE_SIMD
#endif

//#include "platforms/x86_avx2/rct_gf16_cell_avx2.h"
#ifndef RCT_GF16_CELL_AVX2_H
#define RCT_GF16_CELL_AVX2_H

#if RCT_USE_SIMD
#if !USE_GFNI || (SNOVA_l != SNOVA_r)
static inline uint8_t rct_gfni_cleanup(uint8_t v) {
	return (uint8_t)((v ^ ((v & 0xf0) >> 3) ^ (v >> 4)) & 0x0f);
}
#endif
static inline __m256i rct_gfni_cleanup256(__m256i v) {
	const __m256i m0f = _mm256_set1_epi8(0x0f);
	__m256i vhi = _mm256_and_si256(v, _mm256_set1_epi8((char)0xf0));
	__m256i a = _mm256_srli_epi16(vhi, 3);
	__m256i b = _mm256_srli_epi16(v, 4);
	return _mm256_and_si256(_mm256_xor_si256(_mm256_xor_si256(v, a), b), m0f);
}
#if SNOVA_l == 4
static inline __m128i rct_gfni_cleanup128(__m128i v) {
	const __m128i m0f = _mm_set1_epi8(0x0f);
	__m128i vhi = _mm_and_si128(v, _mm_set1_epi8((char)0xf0));
	__m128i a = _mm_srli_epi16(vhi, 3);
	__m128i b = _mm_srli_epi16(v, 4);
	return _mm_and_si128(_mm_xor_si128(_mm_xor_si128(v, a), b), m0f);
}
#if RCT_USE_GFNI && (SNOVA_r == 4)
static inline __m256i rct_cm_cellmm256(__m256i A, __m256i B,
                                       const __m256i *SAY, const __m256i *SBY) {
	__m256i acc = _mm256_setzero_si256();
	for (int k = 0; k < SNOVA_l; ++k)
		acc = _mm256_xor_si256(acc, RCT_GFMUL256(
		                           _mm256_shuffle_epi8(A, SAY[k]), _mm256_shuffle_epi8(B, SBY[k])));
	return rct_gfni_cleanup256(acc);
}
#endif

static inline __m256i gf16_expand_u16x16(__m256i x) {
	__m256i v = _mm256_or_si256(_mm256_or_si256(x, _mm256_slli_epi16(x, 3)),
	                            _mm256_or_si256(_mm256_slli_epi16(x, 6), _mm256_slli_epi16(x, 9)));
	return _mm256_and_si256(v, _mm256_set1_epi16(0x1111));
}
static inline __m256i gf16_compress_u16x16(__m256i a) {
	const __m256i m0f = _mm256_set1_epi16(0x000f);
	__m256i val = _mm256_xor_si256(
	                  _mm256_xor_si256(_mm256_and_si256(a, m0f),
	                                   _mm256_srli_epi16(_mm256_and_si256(a, _mm256_set1_epi16(0x00f0)), 3)),
	                  _mm256_xor_si256(_mm256_srli_epi16(_mm256_and_si256(a, _mm256_set1_epi16(0x0f00)), 6),
	                                   _mm256_srli_epi16(_mm256_and_si256(a, _mm256_set1_epi16((short)0xf000)), 9)));
	val = _mm256_xor_si256(_mm256_xor_si256(val,
	                                        _mm256_srli_epi16(_mm256_and_si256(val, _mm256_set1_epi16(0x00f0)), 3)),
	                       _mm256_srli_epi16(val, 4));
	return _mm256_and_si256(val, m0f);
}
static inline __m128i gf16_pack_u16_to_bytes(__m256i c) {
	return _mm_packus_epi16(_mm256_castsi256_si128(c), _mm256_extracti128_si256(c, 1));
}
#define RCT_GF16_MM0 _mm256_setr_epi8(0,1,0,1,0,1,0,1,8,9,8,9,8,9,8,9, 0,1,0,1,0,1,0,1,8,9,8,9,8,9,8,9)
#define RCT_GF16_MM1 _mm256_setr_epi8(2,3,2,3,2,3,2,3,10,11,10,11,10,11,10,11, 2,3,2,3,2,3,2,3,10,11,10,11,10,11,10,11)
#define RCT_GF16_MM2 _mm256_setr_epi8(4,5,4,5,4,5,4,5,12,13,12,13,12,13,12,13, 4,5,4,5,4,5,4,5,12,13,12,13,12,13,12,13)
#define RCT_GF16_MM3 _mm256_setr_epi8(6,7,6,7,6,7,6,7,14,15,14,15,14,15,14,15, 6,7,6,7,6,7,6,7,14,15,14,15,14,15,14,15)
#if !USE_GFNI
static inline void rct_gf16_cperm_exp(__m256i cw_raw, __m256i *cperm) {
	__m256i e = gf16_expand_u16x16(cw_raw);
	cperm[0] = _mm256_permute4x64_epi64(e, 0x00);
	cperm[1] = _mm256_permute4x64_epi64(e, 0x55);
	cperm[2] = _mm256_permute4x64_epi64(e, 0xAA);
	cperm[3] = _mm256_permute4x64_epi64(e, 0xFF);
}
static inline __m256i rct_gf16_mm4_bc(__m256i bw_raw, const __m256i *cperm_exp) {
	const __m256i m0 = RCT_GF16_MM0, m1 = RCT_GF16_MM1, m2 = RCT_GF16_MM2, m3 = RCT_GF16_MM3;
	__m256i a = _mm256_mullo_epi16(_mm256_shuffle_epi8(bw_raw, m0), cperm_exp[0]);
	a = _mm256_xor_si256(a, _mm256_mullo_epi16(_mm256_shuffle_epi8(bw_raw, m1), cperm_exp[1]));
	a = _mm256_xor_si256(a, _mm256_mullo_epi16(_mm256_shuffle_epi8(bw_raw, m2), cperm_exp[2]));
	a = _mm256_xor_si256(a, _mm256_mullo_epi16(_mm256_shuffle_epi8(bw_raw, m3), cperm_exp[3]));
	return a;
}
static inline void rct_gf16_bshuf_exp(__m256i bw_raw, __m256i *bsh) {
	const __m256i m0 = RCT_GF16_MM0, m1 = RCT_GF16_MM1, m2 = RCT_GF16_MM2, m3 = RCT_GF16_MM3;
	__m256i e = gf16_expand_u16x16(bw_raw);
	bsh[0] = _mm256_shuffle_epi8(e, m0);
	bsh[1] = _mm256_shuffle_epi8(e, m1);
	bsh[2] = _mm256_shuffle_epi8(e, m2);
	bsh[3] = _mm256_shuffle_epi8(e, m3);
}
static inline __m256i rct_gf16_mm4_bs(const __m256i *bsh_exp, __m256i cw_raw) {
	__m256i a = _mm256_mullo_epi16(bsh_exp[0], _mm256_permute4x64_epi64(cw_raw, 0x00));
	a = _mm256_xor_si256(a, _mm256_mullo_epi16(bsh_exp[1], _mm256_permute4x64_epi64(cw_raw, 0x55)));
	a = _mm256_xor_si256(a, _mm256_mullo_epi16(bsh_exp[2], _mm256_permute4x64_epi64(cw_raw, 0xAA)));
	a = _mm256_xor_si256(a, _mm256_mullo_epi16(bsh_exp[3], _mm256_permute4x64_epi64(cw_raw, 0xFF)));
	return a;
}
#endif
#if defined(RCT_COEF_MULLO) && (RCT_COEF_MULLO + 0) \
    && (SNOVA_q == 16) && (SNOVA_r != SNOVA_l)
#define RCT_CM_ACTIVE 1
#endif
#if !defined(RCT_CM_ACTIVE) && defined(RCT_SQ_CM_S3) && (RCT_SQ_CM_S3 + 0) \
    && RCT_USE_SIMD && (SNOVA_q == 16) && (SNOVA_r == SNOVA_l) \
    && (SNOVA_L == 4)
#define RCT_CMS3_ONLY 1
#endif
#if defined(RCT_CM_ACTIVE) || defined(RCT_CMS3_ONLY)
static inline uint16_t rct_cm_exp(uint8_t a) {
	return (uint16_t)((a | ((uint16_t)a << 3) | ((uint16_t)a << 6) | ((uint16_t)a << 9)) & 0x1111);
}
#if !USE_GFNI || SNOVA_r != SNOVA_l
static inline uint16_t rct_cm_cmp(uint16_t a) {
	uint16_t v = (uint16_t)((a & 0xf) ^ ((a & 0xf0) >> 3) ^ ((a & 0xf00) >> 6) ^ ((a & 0xf000) >> 9));
	return (uint16_t)((v ^ ((v & 0xf0) >> 3) ^ (v >> 4)) & 0xf);
}
#endif
static inline void rct_cm_expand_arr(uint16_t *dst, const uint8_t *src, int n) {
	int i = 0;
	for (; i + 16 <= n; i += 16)
		_mm256_storeu_si256((__m256i *)(dst + i),
		                    gf16_expand_u16x16(_mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)(src + i)))));
	for (; i < n; ++i) {
		dst[i] = rct_cm_exp(src[i]);
	}
}
#endif
#endif
#endif

#endif

#if !defined(RCT_CM_ACTIVE) && defined(RCT_SQ_CM_LEFT) && (RCT_SQ_CM_LEFT + 0) \
    && RCT_USE_SIMD && (SNOVA_q == 16) && (SNOVA_r == SNOVA_l) \
    && (SNOVA_L == 4)
#define RCT_CML_ONLY 1
#endif

#if defined(RCT_SQ_CM_S2) && (RCT_SQ_CM_S2 + 0) && RCT_USE_SIMD \
    && (SNOVA_q == 16) && (SNOVA_r == SNOVA_l) && (SNOVA_L == 4)
#define RCT_CMS2_ONLY 1
static inline __m128i rct_s2_pcol(int k) {
	return _mm_add_epi8(_mm_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12),
	                    _mm_set1_epi8((char)k));
}
static inline __m128i rct_s2_prow(int k) {
	return _mm_add_epi8(_mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3),
	                    _mm_set1_epi8((char)(4 * k)));
}
#endif

#if defined(RCT_SQ_RIGHT_DUAL) && (RCT_SQ_RIGHT_DUAL + 0) && RCT_USE_SIMD \
    && (SNOVA_q == 16) && (SNOVA_r == SNOVA_l) && (SNOVA_L == 4)
#define RCT_RD_ACTIVE 1
#endif

#if defined(RCT_KG_MULLO) && (RCT_KG_MULLO + 0) && RCT_USE_PSHUFB \
    && (SNOVA_q == 16) && (SNOVA_l == 4)
#define RCT_KGM_ACTIVE 1
#else
#define RCT_KGM_ACTIVE 0
#endif

#if RCT_USE_SIMD && (SNOVA_l == 4) && (SNOVA_lr16 == 1)
#define RCT_VF_MTK2 1
#else
#define RCT_VF_MTK2 0
#endif

#if RCT_USE_GFNI && RCT_VF_MTK2 && (SNOVA_r2 <= 64) && (SNOVA_r <= 7) && !RCT_VF_JOG
#define RCT_VF_EMM 1
#else
#define RCT_VF_EMM 0
#endif
#if RCT_USE_GFNI && RCT_VF_MTK2 && !defined(RCT_AQ_OFF) && !RCT_VF_JOG
#define RCT_VF_AQ 1
#else
#define RCT_VF_AQ 0
#endif

#if RCT_USE_GFNI
// #include "platforms/x86_avx2/rct_gf16_gfni_avx2.h"
#ifndef RCT_GF16_GFNI_AVX2_H
#define RCT_GF16_GFNI_AVX2_H

#if RCT_USE_GFNI
#define RCT_BC(s)    _mm256_set1_epi8((char)(s))
#define RCT_SV(bc, v) RCT_GFMUL256((bc), (v))
#define RCT_BC128(s)     _mm_set1_epi8((char)(s))
#define RCT_SV128(bc, v) RCT_GFMUL128((bc), (v))
#define RCT_BC_SEC(s)    RCT_BC(s)
#define RCT_BC128_SEC(s) RCT_BC128(s)

#if USE_GFNI
static inline void rct_gauss_row_axpy(gf_t *dst, const gf_t *src, gf_t s, int k0, int kend) {
	__m256i sv = _mm256_set1_epi8((char)s);
	for (int k = k0; k < kend; k += 32) {
		__m256i p = RCT_GFMUL256(sv, _mm256_loadu_si256((const __m256i *)(src + k)));
		__m256i d = _mm256_loadu_si256((const __m256i *)(dst + k));
		_mm256_storeu_si256((__m256i *)(dst + k), _mm256_xor_si256(d, rct_gfni_cleanup256(p)));
	}
}
static inline void rct_gauss_row_scale(gf_t *row, gf_t s, int k0, int kend) {
	__m256i sv = _mm256_set1_epi8((char)s);
	for (int k = k0; k < kend; k += 32) {
		__m256i p = RCT_GFMUL256(sv, _mm256_loadu_si256((const __m256i *)(row + k)));
		_mm256_storeu_si256((__m256i *)(row + k), rct_gfni_cleanup256(p));
	}
}
#endif

#if SNOVA_l == 4
#if !USE_GFNI
static inline void rct_gf4_matmul_add(gf_t *acc, const gf_t *A, const gf_t *B) {
	static const _Alignas(16) uint8_t AMASK[4][16] = {
		{0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12},
		{1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13},
		{2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14},
		{3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15}
	};
	static const _Alignas(16) uint8_t BMASK[4][16] = {
		{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3},
		{4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7},
		{8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11},
		{12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15}
	};
	__m128i av = _mm_loadu_si128((const __m128i *)A);
	__m128i bv = _mm_loadu_si128((const __m128i *)B);
	__m128i prod = _mm_setzero_si128();
	for (int k = 0; k < 4; ++k) {
		__m128i ak = _mm_shuffle_epi8(av, _mm_load_si128((const __m128i *)AMASK[k]));
		__m128i bk = _mm_shuffle_epi8(bv, _mm_load_si128((const __m128i *)BMASK[k]));
		prod = _mm_xor_si128(prod, RCT_GFMUL128(ak, bk));
	}
	__m128i r = rct_gfni_cleanup128(prod);
	__m128i cur = _mm_loadu_si128((const __m128i *)acc);
	_mm_storeu_si128((__m128i *)acc, _mm_xor_si128(cur, r));
}
#endif
#if !USE_GFNI || SNOVA_r != 4
static inline void rct_matmul_l4rows(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd) {
	for (int i = 0; i < ad; i++) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; k++)
			acc = _mm_xor_si128(acc, RCT_GFMUL128(
			                        _mm_set1_epi8((char)A[i * bd + k]),
			                        _mm_loadu_si128((const __m128i *)&B[k * 4])));
		int32_t v = _mm_cvtsi128_si32(rct_gfni_cleanup128(acc));
		memcpy(&C[i * 4], &v, 4);
	}
}

static inline void rct_matmul_l4rows_add(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd) {
	for (int i = 0; i < ad; i++) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; k++)
			acc = _mm_xor_si128(acc, RCT_GFMUL128(
			                        _mm_set1_epi8((char)A[i * bd + k]),
			                        _mm_loadu_si128((const __m128i *)&B[k * 4])));
		int32_t v = _mm_cvtsi128_si32(rct_gfni_cleanup128(acc));
		int32_t c;
		memcpy(&c, &C[i * 4], 4);
		c ^= v;
		memcpy(&C[i * 4], &c, 4);
	}
}
#endif
static __m256i rct_vtl[16];
static void rct_build_vtl(void) {
	_Alignas(32) uint8_t buf[32];
	for (int k = 0; k < 16; ++k) {
		for (int j = 0; j < 16; ++j) {
			buf[j] = rct_multtab[k * SNOVA_q + j];
			buf[16 + j] = buf[j];
		}
		rct_vtl[k] = _mm256_loadu_si256((const __m256i *)buf);
	}
}
#if SNOVA_l == SNOVA_r
static inline __m128i rct_vtl128(uint8_t k) {
	return _mm256_castsi256_si128(rct_vtl[k & 0x0F]);
}
#endif
#endif

#define rct_gauss_row_scale_sec rct_gauss_row_scale
#define rct_gauss_row_axpy_sec  rct_gauss_row_axpy
#if SNOVA_l == 4
#define rct_gf4_matmul_add_sec  rct_gf4_matmul_add
#define rct_matmul_l4rows_sec   rct_matmul_l4rows
#endif
#endif

#endif

#elif RCT_USE_PSHUFB
//#include "platforms/x86_avx2/rct_gf16_pshufb_avx2.h"
#ifndef RCT_GF16_PSHUFB_AVX2_H
#define RCT_GF16_PSHUFB_AVX2_H

#if RCT_USE_PSHUFB
static __m256i rct_vtl[16];
static void rct_build_vtl(void) {
	_Alignas(32) uint8_t buf[32];
	for (int k = 0; k < 16; ++k) {
		for (int j = 0; j < 16; ++j) {
			buf[j] = rct_multtab[k * SNOVA_q + j];
			buf[16 + j] = buf[j];
		}
		rct_vtl[k] = _mm256_loadu_si256((const __m256i *)buf);
	}
}
static inline __m128i rct_vtl128(uint8_t k) {
	return _mm256_castsi256_si128(rct_vtl[k & 0x0F]);
}

#define RCT_BC(s)    rct_vtl[(uint8_t)(s) & 0x0F]
#define RCT_SV(bc, v) _mm256_shuffle_epi8((bc), (v))
#define RCT_BC128(s)     rct_vtl128((uint8_t)(s))
#define RCT_SV128(bc, v) _mm_shuffle_epi8((bc), (v))

static inline __m256i rct_bc_sec(uint8_t s) {
	const __m256i sv = _mm256_set1_epi8((char)(s & 0x0F));
	const __m256i b1 = _mm256_set1_epi8(1), b2 = _mm256_set1_epi8(2),
	              b4 = _mm256_set1_epi8(4), b8 = _mm256_set1_epi8(8);
	__m256i r;
	r = _mm256_and_si256(rct_vtl[1], _mm256_cmpeq_epi8(_mm256_and_si256(sv, b1), b1));
	r = _mm256_xor_si256(r, _mm256_and_si256(rct_vtl[2], _mm256_cmpeq_epi8(_mm256_and_si256(sv, b2), b2)));
	r = _mm256_xor_si256(r, _mm256_and_si256(rct_vtl[4], _mm256_cmpeq_epi8(_mm256_and_si256(sv, b4), b4)));
	r = _mm256_xor_si256(r, _mm256_and_si256(rct_vtl[8], _mm256_cmpeq_epi8(_mm256_and_si256(sv, b8), b8)));
	return r;
}
#if 0
static inline __m128i rct_bc128_sec(uint8_t s) {
	return _mm256_castsi256_si128(rct_bc_sec(s));
}
#endif
#define RCT_BC_SEC(s)    rct_bc_sec((uint8_t)(s))
#define RCT_BC128_SEC(s) rct_bc128_sec((uint8_t)(s))

#if SNOVA_l != SNOVA_r
static inline __m128i rct_gf16_mul128_sec(__m128i av, __m128i bv) {
	const __m128i b1 = _mm_set1_epi8(1), b2 = _mm_set1_epi8(2),
	              b4 = _mm_set1_epi8(4), b8 = _mm_set1_epi8(8);
	__m128i r;
	r = _mm_and_si128(_mm_shuffle_epi8(rct_vtl128(1), bv), _mm_cmpeq_epi8(_mm_and_si128(av, b1), b1));
	r = _mm_xor_si128(r, _mm_and_si128(_mm_shuffle_epi8(rct_vtl128(2), bv), _mm_cmpeq_epi8(_mm_and_si128(av, b2), b2)));
	r = _mm_xor_si128(r, _mm_and_si128(_mm_shuffle_epi8(rct_vtl128(4), bv), _mm_cmpeq_epi8(_mm_and_si128(av, b4), b4)));
	r = _mm_xor_si128(r, _mm_and_si128(_mm_shuffle_epi8(rct_vtl128(8), bv), _mm_cmpeq_epi8(_mm_and_si128(av, b8), b8)));
	return r;
}

#if 0
static inline void rct_gauss_row_scale(gf_t *row, gf_t s, int k0, int kend) {
	__m256i t = rct_vtl[s & 0x0F];
	for (int k = k0; k < kend; k += 32) {
		__m256i v = _mm256_loadu_si256((const __m256i *)(row + k));
		_mm256_storeu_si256((__m256i *)(row + k), _mm256_shuffle_epi8(t, v));
	}
}
static inline void rct_gauss_row_axpy(gf_t *dst, const gf_t *src, gf_t s, int k0, int kend) {
	__m256i t = rct_vtl[s & 0x0F];
	for (int k = k0; k < kend; k += 32) {
		__m256i p = _mm256_shuffle_epi8(t, _mm256_loadu_si256((const __m256i *)(src + k)));
		__m256i d = _mm256_loadu_si256((const __m256i *)(dst + k));
		_mm256_storeu_si256((__m256i *)(dst + k), _mm256_xor_si256(d, p));
	}
}
#endif
#endif
#if SNOVA_l == 4
#if 0
static inline void rct_gf4_matmul_add(gf_t *acc, const gf_t *A, const gf_t *B) {
	static const _Alignas(16) int8_t BM[4][16] = {
		{0, 1, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	};
	__m128i bv = _mm_loadu_si128((const __m128i *)B);
	__m128i brow[4];
	for (int k = 0; k < 4; ++k) {
		brow[k] = _mm_shuffle_epi8(bv, _mm_load_si128((const __m128i *)BM[k]));
	}
	for (int i = 0; i < 4; ++i) {
		__m128i a = _mm_setzero_si128();
		for (int k = 0; k < 4; ++k) {
			a = _mm_xor_si128(a, _mm_shuffle_epi8(rct_vtl128(A[i * 4 + k]), brow[k]));
		}
		uint32_t cur;
		memcpy(&cur, &acc[i * 4], 4);
		cur ^= (uint32_t)_mm_cvtsi128_si32(a);
		memcpy(&acc[i * 4], &cur, 4);
	}
}
#endif
#if !USE_GFNI
static inline void rct_matmul_l4rows(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd) {
	for (int i = 0; i < ad; ++i) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; ++k)
			acc = _mm_xor_si128(acc, _mm_shuffle_epi8(rct_vtl128(A[i * bd + k]),
			    _mm_loadu_si128((const __m128i *)&B[k * 4])));
		uint32_t r = (uint32_t)_mm_cvtsi128_si32(acc);
		memcpy(&C[i * 4], &r, 4);
	}
}
#endif
static inline void rct_matmul_l4rows_add(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd) {
	for (int i = 0; i < ad; ++i) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; ++k)
			acc = _mm_xor_si128(acc, _mm_shuffle_epi8(rct_vtl128(A[i * bd + k]),
			    _mm_loadu_si128((const __m128i *)&B[k * 4])));
		uint32_t cur;
		memcpy(&cur, &C[i * 4], 4);
		cur ^= (uint32_t)_mm_cvtsi128_si32(acc);
		memcpy(&C[i * 4], &cur, 4);
	}
}

#if 0
static inline void rct_gf4_matmul_add_sec(gf_t *acc, const gf_t *A, const gf_t *B) {
	static const _Alignas(16) int8_t BM[4][16] = {
		{0, 1, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	};
	__m128i bv = _mm_loadu_si128((const __m128i *)B);
	__m128i brow[4];
	for (int k = 0; k < 4; ++k) {
		brow[k] = _mm_shuffle_epi8(bv, _mm_load_si128((const __m128i *)BM[k]));
	}
	for (int i = 0; i < 4; ++i) {
		__m128i a = _mm_setzero_si128();
		for (int k = 0; k < 4; ++k) {
			a = _mm_xor_si128(a, rct_gf16_mul128_sec(_mm_set1_epi8((char)A[i * 4 + k]), brow[k]));
		}
		uint32_t cur;
		memcpy(&cur, &acc[i * 4], 4);
		cur ^= (uint32_t)_mm_cvtsi128_si32(a);
		memcpy(&acc[i * 4], &cur, 4);
	}
}
#endif
#if SNOVA_l != SNOVA_r
static inline void rct_matmul_l4rows_sec(gf_t *C, const gf_t *A, const gf_t *B, int ad, int bd) {
	for (int i = 0; i < ad; ++i) {
		__m128i acc = _mm_setzero_si128();
		for (int k = 0; k < bd; ++k)
			acc = _mm_xor_si128(acc, rct_gf16_mul128_sec(_mm_set1_epi8((char)A[i * bd + k]),
			    _mm_loadu_si128((const __m128i *)&B[k * 4])));
		uint32_t r = (uint32_t)_mm_cvtsi128_si32(acc);
		memcpy(&C[i * 4], &r, 4);
	}
}
#endif
#endif

static inline void rct_gauss_row_scale_sec(gf_t *row, gf_t s, int k0, int kend) {
	__m256i t = rct_bc_sec(s);
	for (int k = k0; k < kend; k += 32) {
		__m256i v = _mm256_loadu_si256((const __m256i *)(row + k));
		_mm256_storeu_si256((__m256i *)(row + k), _mm256_shuffle_epi8(t, v));
	}
}
static inline void rct_gauss_row_axpy_sec(gf_t *dst, const gf_t *src, gf_t s, int k0, int kend) {
	__m256i t = rct_bc_sec(s);
	for (int k = k0; k < kend; k += 32) {
		__m256i p = _mm256_shuffle_epi8(t, _mm256_loadu_si256((const __m256i *)(src + k)));
		__m256i d = _mm256_loadu_si256((const __m256i *)(dst + k));
		_mm256_storeu_si256((__m256i *)(dst + k), _mm256_xor_si256(d, p));
	}
}
#endif

#endif

#endif

#if RCT_SIGN_JOG
#define RCT_MATMUL_ADD(a, b, c)      rct_sj_mm_add((a), (b), (c), SNOVA_l, SNOVA_l, SNOVA_l)
#define RCT_MATMUL_ADD_ASEC(a, b, c) rct_sj_mm_add((a), (b), (c), SNOVA_l, SNOVA_l, SNOVA_l)
#define RCT_MATMUL_ADD_BSEC(a, b, c) rct_sj_mm_add_pub((a), (b), (c), SNOVA_l, SNOVA_l, SNOVA_l)
#elif RCT_USE_SIMD && SNOVA_l == 4
#define RCT_MATMUL_ADD(a, b, c) rct_gf4_matmul_add((a), (b), (c))
#define RCT_MATMUL_ADD_ASEC(a, b, c) rct_gf4_matmul_add_sec((a), (b), (c))
#define RCT_MATMUL_ADD_BSEC(a, b, c) rct_gf4_matmul_add((a), (b), (c))
#else
#define RCT_MATMUL_ADD(a, b, c) gf_mat_mul_add((a), (b), (c))
#define RCT_MATMUL_ADD_ASEC(a, b, c) gf_mat_mul_add_sec((a), (b), (c))
#define RCT_MATMUL_ADD_BSEC(a, b, c) gf_mat_mul_add_sec((a), (b), (c))
#endif

// #include "platforms/x86_avx2/rct_oddq_avx2.h"

// #include "platforms/x86_avx2/rct_verify_kernels_avx2.h"
#ifndef RCT_VERIFY_KERNELS_AVX2_H
#define RCT_VERIFY_KERNELS_AVX2_H

#if RCT_VF_MTK2 || RCT_VF_JOG
static _Alignas(64) uint8_t rct_mtk2[256][16];
static void rct_build_mtk2(void) {
	for (int idx = 0; idx < 256; ++idx)
		for (int x = 0; x < 16; ++x)
			rct_mtk2[idx][x] = (uint8_t)(rct_multtab[(idx & 0x0F) * SNOVA_q + x] |
			                             (rct_multtab[(idx >> 4) * SNOVA_q + x] << 4));
}
#if SNOVA_l == 5
static inline __m256i rct_mtk2t(uint8_t idx) {
	return _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i *)rct_mtk2[idx]));
}
#endif
#if !USE_GFNI || (SNOVA_l != 4)
static inline __m256i rct_mtk2t16(uint16_t idx16) {
	return _mm256_broadcastsi128_si256(
	           _mm_load_si128((const __m128i *)((const uint8_t *)rct_mtk2 + idx16)));
}
#endif
static inline __m256i rct_nib_lo(__m256i v) {
	return _mm256_and_si256(v, _mm256_set1_epi8(0x0f));
}
static inline __m256i rct_nib_hi(__m256i v) {
	return _mm256_and_si256(_mm256_srli_epi16(v, 4), _mm256_set1_epi8(0x0f));
}
#if SNOVA_l == 4
static inline void rct_vf_expand_sig(gf_t *out, const uint8_t *in, size_t num) {
	const __m128i m0f = _mm_set1_epi8(0x0f);
	size_t nb = num / 2;
	uint32_t i = 0;
	for (; i + 16 <= nb; i += 16) {
		__m128i b = _mm_loadu_si128((const __m128i *)(in + i));
		__m128i lo = _mm_and_si128(b, m0f);
		__m128i hi = _mm_and_si128(_mm_srli_epi16(b, 4), m0f);
		_mm_storeu_si128((__m128i *)(out + 2 * i), _mm_unpacklo_epi8(lo, hi));
		_mm_storeu_si128((__m128i *)(out + 2 * i + 16), _mm_unpackhi_epi8(lo, hi));
	}
	for (; i < nb; ++i) {
		out[2 * i] = (gf_t)(in[i] & 0x0F);
		out[2 * i + 1] = (gf_t)(in[i] >> 4);
	}
}
#endif
#endif

#if RCT_VF_MTK2
#if !USE_GFNI
static inline __m128i rct_vf_pack_pair32(__m256i v, __m256i pl, __m256i ph) {
	__m256i lo = _mm256_shuffle_epi8(v, pl);
	__m256i hi = _mm256_shuffle_epi8(v, ph);
	__m256i pk = _mm256_or_si256(lo, _mm256_slli_epi16(hi, 4));
	return _mm256_castsi256_si128(_mm256_permute4x64_epi64(pk, 0x08));
}
#endif
#if !RCT_USE_GFNI
static const _Alignas(32) uint8_t RCT_VF_PPL[32] = {
	0, 8, 1, 9, 2, 10, 3, 11, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0, 8, 1, 9, 2, 10, 3, 11, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};
static const _Alignas(32) uint8_t RCT_VF_PPH[32] = {
	4, 12, 5, 13, 6, 14, 7, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	4, 12, 5, 13, 6, 14, 7, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};
static const _Alignas(32) uint8_t RCT_VF_WPL[32] = {
	0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};
static const _Alignas(32) uint8_t RCT_VF_WPH[32] = {
	1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};
#endif

#if RCT_USE_GFNI
static _Alignas(32) uint8_t rct_whipM[SNOVA_l2][32];
static _Alignas(32) uint8_t rct_whipR[SNOVA_l][32];
static _Alignas(32) uint8_t rct_vf_dwexp[32];
static void rct_build_vf_gfni(void) {
	memset(rct_whipM, 0, sizeof(rct_whipM));
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int k1 = 0; k1 < SNOVA_l; k1++)
			for (int ab = 0; ab < SNOVA_l; ab++)
				for (int j = 0; j < SNOVA_r; j++)
					rct_whipM[i1 * SNOVA_l + k1][ab * SNOVA_r + j] =
					    rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1];
	for (int k1 = 0; k1 < SNOVA_l; k1++)
		for (int b = 0; b < 32; b++) {
			rct_whipR[k1][b] = (b < SNOVA_lr) ? (uint8_t)((k1 & 1) * SNOVA_r + (b % SNOVA_r)) : 0x80;
		}
	for (int b = 0; b < 32; b++) {
		rct_vf_dwexp[b] = (b / 4 < SNOVA_r) ? (uint8_t)(b / 4) : 0x80;
	}
}
#if SNOVA_l == SNOVA_r || SNOVA_r < 8
static inline __m256i rct_vf_mm_dw(const uint8_t *Acm, const gf_t *B) {
	const __m256i pat = _mm256_load_si256((const __m256i *)rct_vf_dwexp);
	__m256i acc = _mm256_setzero_si256();
	for (int k = 0; k < SNOVA_r; k++) {
		int64_t aw;
		memcpy(&aw, Acm + k * 8, 8);
		__m256i acol = _mm256_shuffle_epi8(_mm256_set1_epi64x(aw), pat);
		int32_t bw;
		memcpy(&bw, B + k * SNOVA_l, 4);
		acc = _mm256_xor_si256(acc, _mm256_gf2p8mul_epi8(acol, _mm256_set1_epi32(bw)));
	}
	return acc;
}
static inline void rct_vf_tr8(uint8_t *dst, const uint8_t *src, int rstride) {
	__m128i l0 = _mm_loadl_epi64((const __m128i *)(src + 0 * rstride));
	__m128i l1 = _mm_loadl_epi64((const __m128i *)(src + 1 * rstride));
	__m128i l2 = _mm_loadl_epi64((const __m128i *)(src + 2 * rstride));
	__m128i l3 = _mm_loadl_epi64((const __m128i *)(src + 3 * rstride));
	__m128i l4 = _mm_loadl_epi64((const __m128i *)(src + 4 * rstride));
	__m128i l5 = _mm_loadl_epi64((const __m128i *)(src + 5 * rstride));
	__m128i l6 = _mm_loadl_epi64((const __m128i *)(src + 6 * rstride));
	__m128i x0 = _mm_unpacklo_epi64(l0, l1);
	__m128i x1 = _mm_unpacklo_epi64(l2, l3);
	__m128i x2 = _mm_unpacklo_epi64(l4, l5);
	__m128i x3 = _mm_unpacklo_epi64(l6, _mm_setzero_si128());
	__m128i u0 = _mm_unpacklo_epi8(x0, x1);
	__m128i u1 = _mm_unpackhi_epi8(x0, x1);
	__m128i u2 = _mm_unpacklo_epi8(x2, x3);
	__m128i u3 = _mm_unpackhi_epi8(x2, x3);
	__m128i v0 = _mm_unpacklo_epi8(u0, u1);
	__m128i v1 = _mm_unpackhi_epi8(u0, u1);
	__m128i v2 = _mm_unpacklo_epi8(u2, u3);
	__m128i v3 = _mm_unpackhi_epi8(u2, u3);
	_mm_store_si128((__m128i *)(dst + 0), _mm_unpacklo_epi32(v0, v2));
	_mm_store_si128((__m128i *)(dst + 16), _mm_unpackhi_epi32(v0, v2));
	_mm_store_si128((__m128i *)(dst + 32), _mm_unpacklo_epi32(v1, v3));
	_mm_store_si128((__m128i *)(dst + 48), _mm_unpackhi_epi32(v1, v3));
}
#endif

static __m256i rct_vf_thv[4];
static __m256i rct_vf_aqko;
static __m256i rct_vf_aqlo;
static __m256i rct_vf_madp;
__attribute__((unused)) static void rct_build_vf_aq(void) {
	for (int t = 0; t < 4; ++t) {
		uint8_t colb[8];
		for (int j = 0; j < 8; ++j) {
			uint8_t v = (uint8_t)(1u << (j & 3));
			uint8_t rh = 0;
			for (int k = 0; k < 4; ++k) {
				rh |= (uint8_t)(((rct_multtab[v * SNOVA_q + (1u << k)] >> t) & 1) << k);
			}
			colb[j] = (j < 4) ? rh : (uint8_t)(rh << 4);
		}
		uint64_t qw = 0;
		for (int i = 0; i < 8; ++i) {
			uint8_t row = 0;
			for (int j = 0; j < 8; ++j) {
				row |= (uint8_t)(((colb[j] >> i) & 1) << j);
			}
			qw |= (uint64_t)row << (8 * (7 - i));
		}
		rct_vf_thv[t] = _mm256_set1_epi64x((long long)qw);
	}
	_Alignas(32) uint8_t ko[32], lo[32];
	static const uint8_t ord8[8] = {2, 0, 6, 4, 3, 1, 7, 5};
	for (int b = 0; b < 32; ++b) {
		ko[b] = (uint8_t)((b & 8) | ord8[b & 7]);
		lo[b] = (uint8_t)((b & 16) | ((b & 15) ^ 1));
	}
	rct_vf_aqko = _mm256_load_si256((const __m256i *)ko);
	rct_vf_aqlo = _mm256_load_si256((const __m256i *)lo);
	rct_vf_madp = _mm256_set1_epi16(0x1001);
}
static inline void rct_vf_aq_tree(__m256i keys, __m256i out[4]) {
	__m256i R0 = _mm256_gf2p8affine_epi64_epi8(keys, rct_vf_thv[0], 0);
	__m256i R1 = _mm256_gf2p8affine_epi64_epi8(keys, rct_vf_thv[1], 0);
	__m256i R2 = _mm256_gf2p8affine_epi64_epi8(keys, rct_vf_thv[2], 0);
	__m256i R3 = _mm256_gf2p8affine_epi64_epi8(keys, rct_vf_thv[3], 0);
	__m256i u32lo = _mm256_unpacklo_epi8(R3, R2);
	__m256i u10lo = _mm256_unpacklo_epi8(R1, R0);
	__m256i u32hi = _mm256_unpackhi_epi8(R3, R2);
	__m256i u10hi = _mm256_unpackhi_epi8(R1, R0);
	out[0] = _mm256_unpacklo_epi16(u32lo, u10lo);
	out[1] = _mm256_unpackhi_epi16(u32lo, u10lo);
	out[2] = _mm256_unpacklo_epi16(u32hi, u10hi);
	out[3] = _mm256_unpackhi_epi16(u32hi, u10hi);
}
static inline void rct_vf_aq_quad(const uint8_t *pc, uint8_t *dst) {
	__m256i c01 = _mm256_loadu_si256((const __m256i *)pc);
	__m256i c23 = _mm256_loadu_si256((const __m256i *)(pc + 32));
	__m256i k01 = _mm256_maddubs_epi16(c01, rct_vf_madp);
	__m256i k23 = _mm256_maddubs_epi16(c23, rct_vf_madp);
	__m256i keys = _mm256_shuffle_epi8(_mm256_packus_epi16(k01, k23), rct_vf_aqko);
	__m256i q[4];
	rct_vf_aq_tree(keys, q);
	_mm256_store_si256((__m256i *)(dst + 0), q[0]);
	_mm256_store_si256((__m256i *)(dst + 32), q[1]);
	_mm256_store_si256((__m256i *)(dst + 64), q[2]);
	_mm256_store_si256((__m256i *)(dst + 96), q[3]);
}
#define RCT_VF_AQ_LOFF(p) (32 * (((p) & 7) >> 1) + 16 * ((p) >> 3) + 8 * ((p) & 1))
static inline __m256i rct_vf_aq_bq(const uint8_t *base, int off) {
	int64_t w;
	memcpy(&w, base + off, 8);
	return _mm256_set1_epi64x(w);
}
#endif

#endif

#endif


//#include "platforms/generic/rct_gf16_scalar.h"

static inline void gf_mat_mul(gf_t *a, const gf_t *b, const gf_t *c) {
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_t sum = 0;
			for (int k1 = 0; k1 < SNOVA_l; k1++) {
				gf_set_add(&sum, gf_mult(b[i1 * SNOVA_l + k1], c[k1 * SNOVA_l + j1]));
			}
			a[i1 * SNOVA_l + j1] = sum;
		}
}

#if SNOVA_l == 5
#if 0
static inline void gf_mat_mul_add(gf_t *a, const gf_t *b, const gf_t *c) {
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_t sum = 0;
			for (int k1 = 0; k1 < SNOVA_l; k1++) {
				gf_set_add(&sum, gf_mult(b[i1 * SNOVA_l + k1], c[k1 * SNOVA_l + j1]));
			}
			gf_set_add(&a[i1 * SNOVA_l + j1], sum);
		}
}
#endif
static inline void gf_mat_mul_add_lr(gf_t *a, const gf_t *b, const gf_t *c, int ad, int bd, int cd) {
	for (int i1 = 0; i1 < ad; i1++)
		for (int j1 = 0; j1 < cd; j1++) {
			gf_t sum = 0;
			for (int k1 = 0; k1 < bd; k1++) {
				gf_set_add(&sum, gf_mult(b[i1 * bd + k1], c[k1 * cd + j1]));
			}
			gf_set_add(&a[i1 * cd + j1], sum);
		}
}

#if 0
static inline void gf_mat_mul_add_sec(gf_t *a, const gf_t *b, const gf_t *c) {
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_t sum = 0;
			for (int k1 = 0; k1 < SNOVA_l; k1++) {
				gf_set_add(&sum, gf_mult_sec(b[i1 * SNOVA_l + k1], c[k1 * SNOVA_l + j1]));
			}
			gf_set_add(&a[i1 * SNOVA_l + j1], sum);
		}
}
static inline void gf_mat_mul_add_lr_sec(gf_t *a, const gf_t *b, const gf_t *c, int ad, int bd, int cd) {
	for (int i1 = 0; i1 < ad; i1++)
		for (int j1 = 0; j1 < cd; j1++) {
			gf_t sum = 0;
			for (int k1 = 0; k1 < bd; k1++) {
				gf_set_add(&sum, gf_mult_sec(b[i1 * bd + k1], c[k1 * cd + j1]));
			}
			gf_set_add(&a[i1 * cd + j1], sum);
		}
}
#endif
#endif



#if defined(SNOVA_VERIFY_STREAM) && (SNOVA_VERIFY_STREAM + 0) && !RCT_VF_JOG && \
    (((SNOVA_q == 16) && RCT_VF_AQ) || ((SNOVA_q != 16) && RCT_Q_SIMD && RCT_Q_MADD))
#if !SNOVA_PK_EXPAND_SHAKE && !defined(SNOVA_ARCH_X86_AVX2)
#error "SNOVA_VERIFY_STREAM (rct) AES path needs aes128_ctr_zero_at (AVX2); build with AES=0 (SHAKE XOF) or ARCH=x86_avx2"
#endif
#define RCT_VERIFY_STREAM 1
#else
#define RCT_VERIFY_STREAM 0
#endif

#if defined(SNOVA_PKX_PGEN) && (SNOVA_PKX_PGEN + 0)
#if !SNOVA_PK_EXPAND_SHAKE && !defined(SNOVA_ARCH_X86_AVX2)
#error "SNOVA_PKX_PGEN AES path needs aes128_ctr_zero_at (AVX2); build with AES=0 (SHAKE XOF) or ARCH=x86_avx2"
#endif
#if ((SNOVA_q == 16) && ((SNOVA_l2 % 2) != 0)) || RCT_JOG_PKXJOG
#error "SNOVA_PKX_PGEN requires the fused pk_expand gate (q16 needs even l2; JOG l!=4 pkx uses the jogress-layout legacy path) - build without PKX_PGEN=1"
#endif
#define RCT_PKX_PGEN 1
#else
#define RCT_PKX_PGEN 0
#endif

#if RCT_VERIFY_STREAM && (SNOVA_q != 16)
static inline void rct_vf_qs_rowseg(uint16_t *accm, const gf_t *cells, int col0, int ncols) {
	__m256i acc[SNOVA_l][RCT_Q_LR16];
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int g = 0; g < RCT_Q_LR16; g++) {
			acc[i1][g] = _mm256_load_si256((const __m256i *)&accm[(i1 * RCT_Q_LR16 + g) * 16]);
		}
	for (int j = 0; j < ncols; ++j) {
		const uint8_t *wb = &rct_qv_wpair[(size_t)(col0 + j) * 2 * 2 * RCT_Q_LRP];
		const gf_t *pcell = cells + (size_t)j * SNOVA_l2;
		for (int h = 0; h < 2; ++h) {
			__m256i w0 = _mm256_load_si256((const __m256i *)(wb + h * 2 * RCT_Q_LRP));
#if RCT_Q_LR16 == 2
			__m256i w1 = _mm256_load_si256((const __m256i *)(wb + h * 2 * RCT_Q_LRP + 32));
#endif
			for (int i1 = 0; i1 < SNOVA_l; i1++) {
				uint16_t pw;
				memcpy(&pw, pcell + i1 * SNOVA_l + 2 * h, 2);
				__m256i pb = _mm256_set1_epi16((short)pw);
				acc[i1][0] = _mm256_add_epi16(acc[i1][0], _mm256_maddubs_epi16(w0, pb));
#if RCT_Q_LR16 == 2
				acc[i1][1] = _mm256_add_epi16(acc[i1][1], _mm256_maddubs_epi16(w1, pb));
#endif
			}
		}
	}
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int g = 0; g < RCT_Q_LR16; g++) {
			_mm256_store_si256((__m256i *)&accm[(i1 * RCT_Q_LR16 + g) * 16], acc[i1][g]);
		}
}
#endif

static inline gf_t gf_mat_det(gf_t *a) {
	gf_t det = 0;
#if SNOVA_l == 1
	det = a[0];
#elif SNOVA_l == 2
	det = gf_sub(gf_mult(a[0], a[3]), gf_mult(a[1], a[2]));
#elif SNOVA_l == 3
	det = gf_mult(a[0], gf_sub(gf_mult(a[4], a[8]), gf_mult(a[5], a[7])));
	gf_set_add(&det, gf_mult(a[1], gf_sub(gf_mult(a[5], a[6]), gf_mult(a[3], a[8]))));
	gf_set_add(&det, gf_mult(a[2], gf_sub(gf_mult(a[3], a[7]), gf_mult(a[4], a[6]))));
#elif SNOVA_l == 4
	gf_t det_l, det_r;
#define DET_L(x, y) det_l = gf_sub(gf_mult(a[x], a[4 + y]), gf_mult(a[y], a[4 + x]))
#define DET_R(x, y) det_r = gf_sub(gf_mult(a[8 + x], a[12 + y]), gf_mult(a[8 + y], a[12 + x]))
#define DET22(x1, y1, x2, y2) DET_L(x1, y1); DET_R(x2, y2); gf_set_add(&det, gf_mult(det_l, det_r))
	DET22(0, 1, 2, 3);
	DET22(0, 2, 3, 1);
	DET22(0, 3, 1, 2);
	DET22(1, 2, 0, 3);
	DET22(1, 3, 2, 0);
	DET22(2, 3, 0, 1);
#undef DET_R
#undef DET22
#undef DET_L
#elif SNOVA_l == 5
	gf_t det_l, det_r;
#define DET_L(x, y) det_l = gf_sub(gf_mult(a[x], a[5 + y]), gf_mult(a[y], a[5 + x]))
#define DET_R2(x, y, z) gf_mult(gf_sub(gf_mult(a[10 + x], a[15 + y]), gf_mult(a[10 + y], a[15 + x])), a[20 + z])
#define DET_R3(x, y, z) det_r = gf_add(DET_R2(x, y, z), gf_add(DET_R2(y, z, x), DET_R2(z, x, y)))
#define DET23(x1, y1, x2, y2, z2) DET_L(x1, y1); DET_R3(x2, y2, z2); gf_set_add(&det, gf_mult(det_l, det_r))
	DET23(0, 1, 2, 3, 4);
	DET23(0, 2, 3, 1, 4);
	DET23(0, 3, 1, 2, 4);
	DET23(0, 4, 1, 3, 2);
	DET23(1, 2, 0, 3, 4);
	DET23(1, 3, 2, 0, 4);
	DET23(1, 4, 2, 3, 0);
	DET23(2, 3, 0, 1, 4);
	DET23(2, 4, 0, 3, 1);
	DET23(3, 4, 2, 0, 1);
#undef DET_R2
#undef DET_R3
#undef DET23
#undef DET_L
#else
#error "Unsupported rank"
#endif
	return det;
}

static void rct_init_gf_tables(void) {
#if SNOVA_q == 16
	uint8_t F_star[15] = {1, 2, 4, 8, 3, 6, 12, 11, 5, 10, 7, 14, 15, 13, 9};
	for (int i1 = 0; i1 < 16; i1++) {
		rct_multtab[i1] = 0;
		rct_multtab[i1 * SNOVA_q] = 0;
	}
	for (int i1 = 0; i1 < SNOVA_q - 1; i1++)
		for (int j1 = 0; j1 < SNOVA_q - 1; j1++) {
			rct_multtab[F_star[i1] * SNOVA_q + F_star[j1]] = F_star[(i1 + j1) % (SNOVA_q - 1)];
		}
	for (int i1 = 0; i1 < SNOVA_q; i1++)
		for (int j1 = 0; j1 < SNOVA_q; j1++) {
			rct_addtab[i1 * SNOVA_q + j1] = (i1 ^ j1);
		}
#else
	for (int i1 = 0; i1 < SNOVA_q; i1++)
		for (int j1 = 0; j1 < SNOVA_q; j1++) {
			rct_multtab[i1 * SNOVA_q + j1] = (i1 * j1) % SNOVA_q;
			rct_addtab[i1 * SNOVA_q + j1] = (i1 + j1) % SNOVA_q;
		}
#endif
}

static void rct_set_S(gf_t *gf_S1) {
#if SNOVA_q == 16
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_S1[i1 * SNOVA_l + j1] = 8 - (i1 + j1);
		}
#if SNOVA_l == 5
	gf_S1[SNOVA_l2 - 1] = 9;
#endif
#else
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_S1[i1 * SNOVA_l + j1] = (SNOVA_Q_A + i1 + j1) & SNOVA_Q_B;
		}
	gf_S1[SNOVA_l2 - 1] = SNOVA_Q_C;
#endif
}

static void rct_gen_S_array(void) {
	memset(rct_S, 0, sizeof(rct_S));
	for (int i1 = 0; i1 < SNOVA_l; i1++) {
		rct_S[i1 * SNOVA_l + i1] = 1;
	}
#if SNOVA_l > 1
	rct_set_S(&rct_S[1 * SNOVA_l2]);
	for (int i1 = 2; i1 < SNOVA_l; i1++) {
		gf_mat_mul(&rct_S[i1 * SNOVA_l2], &rct_S[1 * SNOVA_l2], &rct_S[(i1 - 1) * SNOVA_l2]);
	}
#endif
}

static void convert_bytes_to_GF(gf_t *gf_array, const uint8_t *byte_array, size_t num) {
#if SNOVA_q != 16
	for (size_t idx = 0; idx < num; idx++) {
		gf_array[idx] = byte_array[idx] % SNOVA_q;
	}
#else
	for (size_t idx = 0; idx < num / 2; idx++) {
		gf_array[2 * idx] = (byte_array[idx] & 0xf) % SNOVA_q;
		gf_array[2 * idx + 1] = (byte_array[idx] >> 4) % SNOVA_q;
	}
	if (num & 1) {
		gf_array[num - 1] = (byte_array[num / 2] & 0xf) % SNOVA_q;
	}
#endif
}

#if SNOVA_q == 16
#if SNOVA_l == 4
static inline void rct_unpack_nib_seg(gf_t *dst, const uint8_t *src, size_t nb) {
	size_t i = 0;
#if RCT_USE_SIMD
	const __m256i m0f = _mm256_set1_epi8(0x0f);
	for (; i + 32 <= nb; i += 32) {
		__m256i b = _mm256_loadu_si256((const __m256i *)(src + i));
		__m256i lo = _mm256_and_si256(b, m0f);
		__m256i hi = _mm256_and_si256(_mm256_srli_epi16(b, 4), m0f);
		__m256i u0 = _mm256_unpacklo_epi8(lo, hi);
		__m256i u1 = _mm256_unpackhi_epi8(lo, hi);
		_mm256_storeu_si256((__m256i *)(dst + 2 * i), _mm256_permute2x128_si256(u0, u1, 0x20));
		_mm256_storeu_si256((__m256i *)(dst + 2 * i + 32), _mm256_permute2x128_si256(u0, u1, 0x31));
	}
	if (i + 16 <= nb) {
		__m128i b = _mm_loadu_si128((const __m128i *)(src + i));
		__m128i lo = _mm_and_si128(b, _mm256_castsi256_si128(m0f));
		__m128i hi = _mm_and_si128(_mm_srli_epi16(b, 4), _mm256_castsi256_si128(m0f));
		_mm_storeu_si128((__m128i *)(dst + 2 * i), _mm_unpacklo_epi8(lo, hi));
		_mm_storeu_si128((__m128i *)(dst + 2 * i + 16), _mm_unpackhi_epi8(lo, hi));
		i += 16;
	}
#endif
	for (; i < nb; ++i) {
		dst[2 * i] = (gf_t)(src[i] & 0x0f);
		dst[2 * i + 1] = (gf_t)(src[i] >> 4);
	}
}
#endif
#else
static inline void rct_unpack_modq_seg(gf_t *dst, const uint8_t *src, size_t ngf) {
	for (size_t i = 0; i < ngf; ++i) {
		dst[i] = src[i] % SNOVA_q;
	}
}
#endif

static void compress_gf(uint8_t *byte_array, const gf_t *gf_array, size_t num) {
	size_t idx = 0, out_idx = 0;
	size_t num_bytes = BYTES_GF(num);
	do {
		uint64_t val = 0, fact = 1;
		int i1 = 0;
		while (i1 < PACK_GF && idx < num) {
			val += fact * (gf_array[idx] % SNOVA_q);
			idx++;
			i1++;
			fact *= SNOVA_q;
		}
		// i1 = (i1 + 1) / 2;
		int j1 = 0;
		while (j1 < PACK_BYTES && out_idx < num_bytes) {
			byte_array[out_idx] = val & 0xff;
			out_idx++;
			val = val >> 8;
			j1++;
		}
	} while (idx < num);
}

static int expand_gf(gf_t *gf_array, const uint8_t *byte_array, size_t num) {
	size_t num_bytes = BYTES_GF(num);
	size_t idx = 0, out_idx = 0;
	uint64_t val;
	uint64_t res = 0;
	do {
		val = 0;
		int i1 = 0;
		while (i1 < PACK_BYTES && idx < num_bytes) {
			val = val ^ ((uint64_t)(byte_array[idx]) << (8 * i1));
			idx++;
			i1++;
		}
		int j1 = 0;
		while (j1 < PACK_GF && out_idx < num) {
			gf_array[out_idx] = val % SNOVA_q;
			val = val / SNOVA_q;
			out_idx++;
			j1++;
		}
		res |= val;
	} while (out_idx < num);
#if SNOVA_q == 16
	if (num & 1) {
		return byte_array[num / 2] & 0xF0;
	}
#endif
	return res != 0;
}

static void compress_pk(uint8_t *pk, gf_t *P22) {
	compress_gf(pk, P22, NUMGF_PK);
}
static int expand_pk(gf_t *P22, const uint8_t *pk) {
	return expand_gf(P22, pk, NUMGF_PK);
}

static void rct_public_xof(const uint8_t seed[16], uint8_t *out, size_t outlen) {
	size_t padded = (outlen + 7u) & ~(size_t)7u;
	snova_pk_expand(out, padded, seed, 16);
}

static void expand_public(gf_t *P_matrix, const uint8_t *seed) {
#if SNOVA_WRAPPER_STACK
	_Alignas(32) uint8_t pk_bytes[((NUM_GEN_PUB_BYTES + 15) & ~(size_t)7u)];
#else
	_Alignas(8) static uint8_t pk_bytes[((NUM_GEN_PUB_BYTES + 15) & ~(size_t)7u)];
#endif
	rct_public_xof(seed, pk_bytes, NUM_GEN_PUB_BYTES);
	convert_bytes_to_GF(P_matrix, (uint8_t *)pk_bytes, NUM_GEN_PUB_GF);
}

#if RCT_SIGN_STREAM || RCT_KG_STREAM || RCT_VERIFY_STREAM || RCT_PKX_PGEN
// #include "pgen.h"
#endif

static void hash_combined(uint8_t *hash_out, const uint8_t *digest, const size_t len_digest,
                          const uint8_t *pk_seed, const uint8_t *salt) {
	shake_t state;
	shake256_init(&state);
#if HASH_PK
	shake_absorb(&state, pk_seed, BYTES_PK_HASH);
#else
	shake_absorb(&state, pk_seed, SEED_LENGTH_PUBLIC);
#endif
	shake_absorb(&state, digest, len_digest);
	shake_absorb(&state, salt, BYTES_SALT);
	shake_finalize(&state);
	shake_squeeze(hash_out, BYTES_HASH, &state);
}

static inline void gen_a_FqS(gf_t *Qm, gf_t *q) {
#if ROUND2_T12
	if (!q[SNOVA_l - 1]) {
		q[SNOVA_l - 1] = SNOVA_q - (q[0] + (q[0] == 0));
	}
#endif
	for (int i1 = 0; i1 < SNOVA_l2; i1++) {
		gf_t sum = 0;
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_set_add(&sum, gf_mult(q[j1], rct_S[j1 * SNOVA_l2 + i1]));
		}
		Qm[i1] = sum;
	}
}

static inline void gen_a_FqS_sec(gf_t *Qm, gf_t *q) {
#if ROUND2_T12
	uint32_t nz_last = ct_gf_nz(q[SNOVA_l - 1]);
	gf_t fallback = (gf_t)(SNOVA_q - (q[0] + (1u - ct_gf_nz(q[0]))));
	q[SNOVA_l - 1] = ct_gf_sel(nz_last, q[SNOVA_l - 1], fallback);
#endif
#if (RCT_USE_GFNI || RCT_HOT_QRP16) && (SNOVA_l == 4)
	__m128i acc = RCT_GFMUL128(_mm_set1_epi8((char)q[0]), _mm_loadu_si128((const __m128i *)&rct_S[0]));
	acc = _mm_xor_si128(acc, RCT_GFMUL128(_mm_set1_epi8((char)q[1]), _mm_loadu_si128((const __m128i *)&rct_S[SNOVA_l2])));
	acc = _mm_xor_si128(acc, RCT_GFMUL128(_mm_set1_epi8((char)q[2]), _mm_loadu_si128((const __m128i *)&rct_S[2 * SNOVA_l2])));
	acc = _mm_xor_si128(acc, RCT_GFMUL128(_mm_set1_epi8((char)q[3]), _mm_loadu_si128((const __m128i *)&rct_S[3 * SNOVA_l2])));
	_mm_storeu_si128((__m128i *)Qm, rct_gfni_cleanup128(acc));
#elif RCT_USE_PSHUFB && (SNOVA_l == 4)
	__m128i acc = rct_gf16_mul128_sec(_mm_set1_epi8((char)q[0]), _mm_loadu_si128((const __m128i *)&rct_S[0]));
	acc = _mm_xor_si128(acc, rct_gf16_mul128_sec(_mm_set1_epi8((char)q[1]), _mm_loadu_si128((const __m128i *)&rct_S[SNOVA_l2])));
	acc = _mm_xor_si128(acc, rct_gf16_mul128_sec(_mm_set1_epi8((char)q[2]), _mm_loadu_si128((const __m128i *)&rct_S[2 * SNOVA_l2])));
	acc = _mm_xor_si128(acc, rct_gf16_mul128_sec(_mm_set1_epi8((char)q[3]), _mm_loadu_si128((const __m128i *)&rct_S[3 * SNOVA_l2])));
	_mm_storeu_si128((__m128i *)Qm, acc);
#else
	for (int i1 = 0; i1 < SNOVA_l2; i1++) {
		gf_t sum = 0;
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_set_add(&sum, gf_mult_sec(q[j1], rct_S[j1 * SNOVA_l2 + i1]));
		}
		Qm[i1] = sum;
	}
#endif
}

#define SK_BLOCK_SIZE ((SNOVA_o * SNOVA_v * SNOVA_l + 1) / 2)
static void expand_T12(gf_t *T12, const uint8_t *seed) {
	gf_t T12coef[SNOVA_o * SNOVA_v * SNOVA_l];
	gf_t sk_data[SK_BLOCK_SIZE];
	shake_t state;
	shake256_init(&state);
	shake_absorb(&state, seed, SEED_LENGTH_PRIVATE);
	shake_finalize(&state);

	size_t idx = SK_BLOCK_SIZE, t_idx = 0;
	while (t_idx < (size_t)SNOVA_o * SNOVA_v * SNOVA_l) {
		if (idx >= SK_BLOCK_SIZE) {
			shake_squeeze(sk_data, SK_BLOCK_SIZE, &state);
			idx = 0;
		}
#if SNOVA_q != 16
		{
			int accept = (sk_data[idx] < SNOVA_REJECTION_LIMIT);
			SNOVA_CT_DECLASSIFY(&accept, sizeof accept);
			if (accept) {
				T12coef[t_idx] = sk_data[idx] % SNOVA_q;
				t_idx++;
			}
		}
#else
		T12coef[t_idx] = sk_data[idx] & 0xf;
		t_idx++;
		T12coef[t_idx] = sk_data[idx] >> 4;
		t_idx++;
#endif
		idx++;
	}
	for (size_t i1 = 0; i1 < (size_t)SNOVA_o * SNOVA_v; i1++) {
		gen_a_FqS_sec(&T12[i1 * SNOVA_l2], &T12coef[i1 * SNOVA_l]);
	}
	SNOVA_CLEAR_OBJ(T12coef);
	SNOVA_CLEAR_OBJ(sk_data);
}

static inline void be_invertible_by_add_aS(gf_t *mat, const gf_t *orig, const int l1, const int l2) {
	memcpy(mat, orig, l1 * l2);
#if ABQ_ALG2
	if ((l1 == SNOVA_l) && (l2 == SNOVA_l))
		if (gf_mat_det(mat) == 0)
			for (gf_t f1 = 1; f1 < SNOVA_q; f1++) {
#if SNOVA_l > 1
				for (int i1 = 0; i1 < SNOVA_l2; i1++) {
					gf_set_add(&mat[i1], gf_mult(f1, rct_S[SNOVA_l2 + i1]));
				}
#else
				mat[0] = 1;
#endif
				if (gf_mat_det(mat) != 0) {
					break;
				}
			}
#endif
}

static void gen_ABQ(gf_t *A, gf_t *Am, gf_t *Bm, gf_t *Q1m, gf_t *Q2m) {
	gf_t *B = A + SNOVA_o * SNOVA_alpha * SNOVA_r2;
	gf_t *q1 = B + SNOVA_o * SNOVA_alpha * SNOVA_lr;
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;
	for (size_t idx = 0; idx < (size_t)SNOVA_o * SNOVA_alpha; idx++) {
		be_invertible_by_add_aS(&Am[idx * SNOVA_r2], &A[idx * SNOVA_r2], SNOVA_r, SNOVA_r);
		be_invertible_by_add_aS(&Bm[idx * SNOVA_lr], &B[idx * SNOVA_lr], SNOVA_r, SNOVA_l);
		gen_a_FqS(&Q1m[idx * SNOVA_l2], &q1[idx * SNOVA_l]);
		gen_a_FqS(&Q2m[idx * SNOVA_l2], &q2[idx * SNOVA_l]);
	}
}

#define ABQ_RAW_N (SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + 2 * SNOVA_l))
static gf_t rct_fixed_abq[ABQ_RAW_N];
#if FIXED_ABQ
static gf_t rct_fixed_Am[SNOVA_o * SNOVA_alpha * SNOVA_r2];
static gf_t rct_fixed_Bm[SNOVA_o * SNOVA_alpha * SNOVA_lr];
static gf_t rct_fixed_Q1[SNOVA_o * SNOVA_alpha * SNOVA_l2];
static gf_t rct_fixed_Q2[SNOVA_o * SNOVA_alpha * SNOVA_l2];
#endif
#if !SNOVA_WRAPPER_STACK && !(RCT_KG_STREAM && !RCT_JOG_PKXJOG)
static gf_t rct_pub_Pmatrix[NUM_PUB_GF];
#endif
static int rct_inited = 0;

static void gen_fixed_ABQ(const char *abq_seed) {
	uint8_t rng_out[ABQ_RAW_N];
	shake256(rng_out, ABQ_RAW_N, (const uint8_t *)abq_seed, (size_t)strlen(abq_seed));
	convert_bytes_to_GF(rct_fixed_abq, rng_out, ABQ_RAW_N);
}

#if RCT_VF_TILE4
static void rct_build_t4(void);
#endif
static void rct_init(void) {
	if (rct_inited) {
		return;
	}
	rct_init_gf_tables();
#if RCT_USE_PSHUFB || RCT_USE_GFNI
	rct_build_vtl();
#endif
	rct_gen_S_array();
#if RCT_VF_MTK2 || RCT_VF_JOG
	rct_build_mtk2();
#endif
#if RCT_VF_MTK2 && RCT_USE_GFNI
	rct_build_vf_gfni();
#endif
#if RCT_VF_AQ
	rct_build_vf_aq();
#endif
#if RCT_VF_TILE4
	rct_build_t4();
#endif
	gen_fixed_ABQ("SNOVA_ABQ");
#if FIXED_ABQ
	gen_ABQ(rct_fixed_abq, rct_fixed_Am, rct_fixed_Bm, rct_fixed_Q1, rct_fixed_Q2);
#endif
	rct_inited = 1;
}

// #include "platforms/x86_avx2/rct_fold_engine.h"
#ifndef SNOVA_RCT_FOLD_ENGINE_H
#define SNOVA_RCT_FOLD_ENGINE_H

#ifndef RCT_L4G_FOLD
#define RCT_L4G_FOLD (!RCT_Q_SIMD && RCT_USE_GFNI && (SNOVA_l == 4))
#endif
#if RCT_L4G_FOLD

#define RCT_L4G_PATS \
    const __m128i sA0 = _mm_setr_epi8(0,0,0,0,4,4,4,4,8,8,8,8,12,12,12,12); \
    const __m128i sA1 = _mm_setr_epi8(1,1,1,1,5,5,5,5,9,9,9,9,13,13,13,13); \
    const __m128i sA2 = _mm_setr_epi8(2,2,2,2,6,6,6,6,10,10,10,10,14,14,14,14); \
    const __m128i sA3 = _mm_setr_epi8(3,3,3,3,7,7,7,7,11,11,11,11,15,15,15,15); \
    const __m128i sB0 = _mm_setr_epi8(0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3); \
    const __m128i sB1 = _mm_setr_epi8(4,5,6,7,4,5,6,7,4,5,6,7,4,5,6,7); \
    const __m128i sB2 = _mm_setr_epi8(8,9,10,11,8,9,10,11,8,9,10,11,8,9,10,11); \
    const __m128i sB3 = _mm_setr_epi8(12,13,14,15,12,13,14,15,12,13,14,15,12,13,14,15)

static void rct_l4g_fold_bsec(gf_t *C, const gf_t *A, const gf_t *T12, int nrows, int rmw) {
	RCT_L4G_PATS;
	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int j1 = 0; j1 < nrows; j1++) {
			__m128i acc[SNOVA_o];
			for (int k1 = 0; k1 < SNOVA_o; k1++) {
				acc[k1] = _mm_setzero_si128();
			}
			for (int j2 = 0; j2 < SNOVA_v; j2++) {
				__m128i av = _mm_loadu_si128((const __m128i *)&A[((i1 * nrows + j1) * SNOVA_v + j2) * SNOVA_l2]);
				__m128i ak0 = _mm_shuffle_epi8(av, sA0), ak1 = _mm_shuffle_epi8(av, sA1),
				        ak2 = _mm_shuffle_epi8(av, sA2), ak3 = _mm_shuffle_epi8(av, sA3);
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					__m128i bv = _mm_loadu_si128((const __m128i *)&T12[(j2 * SNOVA_o + k1) * SNOVA_l2]);
					__m128i p = RCT_GFMUL128(ak0, _mm_shuffle_epi8(bv, sB0));
					p = _mm_xor_si128(p, RCT_GFMUL128(ak1, _mm_shuffle_epi8(bv, sB1)));
					p = _mm_xor_si128(p, RCT_GFMUL128(ak2, _mm_shuffle_epi8(bv, sB2)));
					p = _mm_xor_si128(p, RCT_GFMUL128(ak3, _mm_shuffle_epi8(bv, sB3)));
					acc[k1] = _mm_xor_si128(acc[k1], p);
				}
			}
			for (int k1 = 0; k1 < SNOVA_o; k1++) {
				gf_t *c = &C[((i1 * nrows + j1) * SNOVA_o + k1) * SNOVA_l2];
				__m128i r = rct_gfni_cleanup128(acc[k1]);
				if (rmw) {
					r = _mm_xor_si128(r, _mm_loadu_si128((const __m128i *)c));
				}
				_mm_storeu_si128((__m128i *)c, r);
			}
		}
}

static void rct_l4g_fold_asec(gf_t *P22, const gf_t *T12, const gf_t *F12) {
	RCT_L4G_PATS;
	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int j1 = 0; j1 < SNOVA_o; j1++) {
			__m128i acc[SNOVA_o];
			for (int k1 = 0; k1 < SNOVA_o; k1++) {
				acc[k1] = _mm_setzero_si128();
			}
			for (int idx = 0; idx < SNOVA_v; idx++) {
				__m128i av = _mm_loadu_si128((const __m128i *)&T12[(idx * SNOVA_o + j1) * SNOVA_l2]);
				__m128i ak0 = _mm_shuffle_epi8(av, sA0), ak1 = _mm_shuffle_epi8(av, sA1),
				        ak2 = _mm_shuffle_epi8(av, sA2), ak3 = _mm_shuffle_epi8(av, sA3);
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					__m128i bv = _mm_loadu_si128((const __m128i *)&F12[((i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]);
					__m128i p = RCT_GFMUL128(ak0, _mm_shuffle_epi8(bv, sB0));
					p = _mm_xor_si128(p, RCT_GFMUL128(ak1, _mm_shuffle_epi8(bv, sB1)));
					p = _mm_xor_si128(p, RCT_GFMUL128(ak2, _mm_shuffle_epi8(bv, sB2)));
					p = _mm_xor_si128(p, RCT_GFMUL128(ak3, _mm_shuffle_epi8(bv, sB3)));
					acc[k1] = _mm_xor_si128(acc[k1], p);
				}
			}
			for (int k1 = 0; k1 < SNOVA_o; k1++) {
				gf_t *c = &P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2];
				_mm_storeu_si128((__m128i *)c,
				                 _mm_xor_si128(rct_gfni_cleanup128(acc[k1]),
				                               _mm_loadu_si128((const __m128i *)c)));
			}
		}
}

#endif

#if RCT_SIGN_JOG
#ifndef RCT_FOLD_SEG
#define RCT_FOLD_SEG 1
#endif
#ifndef RCT_FOLD_SEG_M4
#define RCT_FOLD_SEG_M4 1
#endif
#define RCT_F5_A4 (RCT_SIGN_JOG && RCT_HAVE_GFNI  && SNOVA_l == 5 && RCT_FOLD_SEG)
#define RCT_F5_M4 (RCT_SIGN_JOG && !RCT_HAVE_GFNI && SNOVA_l == 5 && RCT_FOLD_SEG && RCT_FOLD_SEG_M4)
#ifndef RCT_FOLD_WIDE
#define RCT_FOLD_WIDE SNOVA_WRAPPER_STACK
#endif
#define RCT_F5_WIDE (RCT_SIGN_JOG && !RCT_HAVE_GFNI && SNOVA_l == 5 && RCT_FOLD_WIDE)

#if RCT_F5_WIDE
#define RCT_WIDE_MVL       (SNOVA_m1 * SNOVA_v * SNOVA_l)
#define RCT_WIDE_P11AW_LEN (RCT_WIDE_MVL * SNOVA_v * SNOVA_l)
#define RCT_WIDE_FW_LEN    (RCT_WIDE_MVL * SNOVA_o * SNOVA_l + 16)
#if SNOVA_WRAPPER_STACK
#define RCT_WIDE_SCRATCH_DECL \
    _Alignas(32) uint16_t P11aw[RCT_WIDE_P11AW_LEN]; \
    _Alignas(32) uint16_t Fw[RCT_WIDE_FW_LEN]
#else
static _Alignas(32) uint16_t rct_wide_p11aw_s[RCT_WIDE_P11AW_LEN];
static _Alignas(32) uint16_t rct_wide_fw_s[RCT_WIDE_FW_LEN];
#define RCT_WIDE_SCRATCH_DECL \
    uint16_t *const P11aw = rct_wide_p11aw_s; \
    uint16_t *const Fw = rct_wide_fw_s
#endif
#endif

#if RCT_F5_WIDE
// #include "gf16_core/gf16_mullo16.h"
static inline __attribute__((always_inline)) void rct_wide_fold_bsec(
    gf_t *C, const gf_t *A, const gf_t *T12, int nrows, int rmw,
    uint16_t *const P11aw, uint16_t *const Fw) {
	const int mvl = SNOVA_m1 * nrows * SNOVA_l;
	memset(Fw, 0, ((size_t)mvl * SNOVA_o * SNOVA_l + 16) * sizeof(uint16_t));
	for (int ni = 0; ni < SNOVA_v; ++ni)
		for (int k1 = 0; k1 < SNOVA_l; ++k1)
			for (int mi = 0; mi < SNOVA_m1; ++mi)
				for (int nj = 0; nj < nrows; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; ++i1)
						P11aw[(ni * SNOVA_l + k1) * mvl + (mi * nrows + nj) * SNOVA_l + i1] =
						    A[((mi * nrows + nj) * SNOVA_v + ni) * SNOVA_l2 + i1 * SNOVA_l + k1];
	for (int nk = 0; nk < SNOVA_o; ++nk)
		for (int j1 = 0; j1 < SNOVA_l; ++j1)
			for (int ni = 0; ni < SNOVA_v; ++ni)
				for (int k1 = 0; k1 < SNOVA_l; ++k1) {
					uint16_t s = cl_expand_scalar16(T12[(ni * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + j1]);
					uint16_t *Fr = &Fw[(nk * SNOVA_l + j1) * mvl];
					const uint16_t *Pr = &P11aw[(ni * SNOVA_l + k1) * mvl];
					for (int mi = 0; mi < mvl; ++mi) {
						Fr[mi] ^= (uint16_t)(s * Pr[mi]);
					}
				}
	for (int i = 0; i < mvl * SNOVA_o * SNOVA_l; i += 16) {
		_mm256_storeu_si256((__m256i *)&Fw[i], cl_gf16_compress_u16x16(_mm256_loadu_si256((const __m256i *)&Fw[i])));
	}
	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int nj = 0; nj < nrows; ++nj)
			for (int nk = 0; nk < SNOVA_o; ++nk)
				for (int i1 = 0; i1 < SNOVA_l; ++i1)
					for (int j1 = 0; j1 < SNOVA_l; ++j1) {
						gf_t r = (gf_t)Fw[(nk * SNOVA_l + j1) * mvl + (mi * nrows + nj) * SNOVA_l + i1];
						gf_t *c = &C[((mi * nrows + nj) * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + j1];
						*c = rmw ? (gf_t)(*c ^ r) : r;
					}
	SNOVA_CLEAR(Fw, ((size_t)mvl * SNOVA_o * SNOVA_l + 16) * sizeof(uint16_t));
}
static void rct_wide_fold_F12(gf_t *F12, const gf_t *P11, const gf_t *T12,
                              uint16_t *const P11aw, uint16_t *const Fw) {
	rct_wide_fold_bsec(F12, P11, T12, SNOVA_v, 0, P11aw, Fw);
}
#endif

#if RCT_F5_A4
_Static_assert(SNOVA_l == 5, "RCT_F5_A4 fold f-clone requires l == 5");
typedef struct {
	__m128i b012, b34;
} rct_a4f_bc_t;
typedef struct {
	__m128i a, b;
} rct_a4f_acc_t;
static inline rct_a4f_acc_t rct_a4f_zero(void) {
	rct_a4f_acc_t z;
	z.a = z.b = _mm_setzero_si128();
	return z;
}
static inline rct_a4f_bc_t rct_a4f_bc(const gf_t *sc) {
	const __m128i P012 = _mm_setr_epi8(0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, -1);
	const __m128i P34  = _mm_setr_epi8(3, 3, 3, 3, 3, 4, 4, 4, 4, 4, -1, -1, -1, -1, -1, -1);
	__m128i v = _mm_loadu_si128((const __m128i *)sc);
	rct_a4f_bc_t b;
	b.b012 = _mm_shuffle_epi8(v, P012);
	b.b34  = _mm_shuffle_epi8(v, P34);
	return b;
}
static inline void rct_a4f_mac(rct_a4f_acc_t *A, rct_a4f_bc_t b, const gf_t *w) {
	A->a = _mm_xor_si128(A->a, _mm_gf2p8mul_epi8(b.b012,
	    _mm_loadu_si128((const __m128i *)w)));
	A->b = _mm_xor_si128(A->b, _mm_gf2p8mul_epi8(b.b34,
	    _mm_loadu_si128((const __m128i *)&w[15])));
}
static inline __m128i rct_a4f_fold(rct_a4f_acc_t A) {
	__m128i f = _mm_xor_si128(A.a, _mm_srli_si128(A.a, 5));
	f = _mm_xor_si128(f, _mm_srli_si128(A.a, 10));
	f = _mm_xor_si128(f, A.b);
	return _mm_xor_si128(f, _mm_srli_si128(A.b, 5));
}
static inline void rct_a4f_xor5(gf_t *C, __m128i acc_folded) {
	_Alignas(16) uint8_t pb[16];
	_mm_store_si128((__m128i *)pb, RCT_SJ_CLEAN(acc_folded));
	for (int j = 0; j < 5; ++j) {
		C[j] = (gf_t)(C[j] ^ pb[j]);
	}
}
#if SNOVA_l == SNOVA_r && SNOVA_l != 5
static inline void rct_a4f_cell_add(gf_t *C, const rct_a4f_bc_t bc[5], const gf_t *B) {
	for (int i = 0; i < 5; ++i) {
		rct_a4f_acc_t a = rct_a4f_zero();
		rct_a4f_mac(&a, bc[i], B);
		rct_a4f_xor5(&C[i * 5], rct_a4f_fold(a));
	}
}
#endif
#endif

#if RCT_F5_M4
_Static_assert(SNOVA_l == 5, "RCT_F5_M4 fold f-clone requires l == 5");
// #include "gf16_core/gf16_mullo16.h"
#define RCT_M4F_PAT0 _mm256_setr_epi8(0,1,0,1,0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,4,5,4,5,4,5,4,5,4,5,6,7)
#define RCT_M4F_PAT1 _mm256_setr_epi8(6,7,6,7,6,7,6,7,8,9,8,9,8,9,8,9,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1)
typedef struct {
	__m256i b0, b1;
} rct_m4f_bc_t;
typedef struct {
	__m256i a0, a1;
} rct_m4f_acc_t;
#if 0
static inline rct_m4f_acc_t rct_m4f_zero(void) {
	rct_m4f_acc_t z;
	z.a0 = z.a1 = _mm256_setzero_si256();
	return z;
}
static inline rct_m4f_bc_t rct_m4f_bc(const gf_t *sc) {
	_Alignas(16) uint16_t ev[8] = {0};
	for (int k = 0; k < 5; ++k) {
		ev[k] = cl_expand_scalar16(sc[k]);
	}
	__m256i evb = _mm256_broadcastsi128_si256(_mm_load_si128((const __m128i *)ev));
	rct_m4f_bc_t b;
	b.b0 = _mm256_shuffle_epi8(evb, RCT_M4F_PAT0);
	b.b1 = _mm256_shuffle_epi8(evb, RCT_M4F_PAT1);
	return b;
}
static inline void rct_m4f_mac(rct_m4f_acc_t *A, rct_m4f_bc_t b, const gf_t *w) {
	A->a0 = _mm256_xor_si256(A->a0, _mm256_mullo_epi16(b.b0,
	    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)w))));
	A->a1 = _mm256_xor_si256(A->a1, _mm256_mullo_epi16(b.b1,
	    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)&w[16]))));
}
static inline __m128i rct_m4f_fold(rct_m4f_acc_t A) {
	_Alignas(32) uint16_t tb[48];
	_mm256_store_si256((__m256i *)tb, A.a0);
	_mm256_store_si256((__m256i *)&tb[16], A.a1);
	_mm256_store_si256((__m256i *)&tb[32], _mm256_setzero_si256());
	__m128i f = _mm_loadu_si128((const __m128i *)tb);
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[5]));
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[10]));
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[15]));
	f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[20]));
	__m256i c = cl_gf16_compress_u16x16(_mm256_castsi128_si256(f));
	return cl_gf16_pack_u16_to_bytes(c);
}
static inline void rct_m4f_xor5(gf_t *C, __m128i acc_folded) {
	_Alignas(16) uint8_t pb[16];
	_mm_store_si128((__m128i *)pb, acc_folded);
	for (int j = 0; j < 5; ++j) {
		C[j] = (gf_t)(C[j] ^ pb[j]);
	}
}
static inline void rct_m4f_cell_add(gf_t *C, const rct_m4f_bc_t bc[5], const gf_t *B) {
	for (int i = 0; i < 5; ++i) {
		rct_m4f_acc_t a = rct_m4f_zero();
		rct_m4f_mac(&a, bc[i], B);
		rct_m4f_xor5(&C[i * 5], rct_m4f_fold(a));
	}
}
#endif
static inline rct_m4f_bc_t rct_m4f2_bc_mirror(const uint16_t *ev5) {
	__m256i evb = _mm256_broadcastsi128_si256(_mm_loadu_si128((const __m128i *)ev5));
	rct_m4f_bc_t b;
	b.b0 = _mm256_shuffle_epi8(evb, RCT_M4F_PAT0);
	b.b1 = _mm256_shuffle_epi8(evb, RCT_M4F_PAT1);
	return b;
}
#if 0
static inline void rct_m4f2_build_bc(rct_m4f_bc_t (*bc)[5], const gf_t *src, int n_cells) {
	for (int c = 0; c < n_cells; ++c) {
		_Alignas(32) uint16_t ev[32];
		rct_m4_expand_buf(ev, &src[c * SNOVA_l2], SNOVA_l2);
		for (int i = 0; i < 5; ++i) {
			bc[c][i] = rct_m4f2_bc_mirror(&ev[i * 5]);
		}
	}
}
#endif
static inline void rct_m4f2_mac5(__m256i *a0, __m256i *a1, const rct_m4f_bc_t *bc, const gf_t *w) {
	const __m256i w0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)w));
	const __m256i w1 = _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)&w[16]));
	for (int i = 0; i < 5; ++i) {
		a0[i] = _mm256_xor_si256(a0[i], _mm256_mullo_epi16(bc[i].b0, w0));
		a1[i] = _mm256_xor_si256(a1[i], _mm256_mullo_epi16(bc[i].b1, w1));
	}
}
static inline void rct_m4f2_fold5_xor(gf_t *C, const __m256i *a0, const __m256i *a1) {
	_Alignas(32) uint16_t tb[5][48];
	for (int i = 0; i < 5; ++i) {
		_mm256_store_si256((__m256i *)tb[i], a0[i]);
		_mm256_store_si256((__m256i *)&tb[i][16], a1[i]);
		_mm256_store_si256((__m256i *)&tb[i][32], _mm256_setzero_si256());
	}
	for (int i = 0; i < 5; ++i) {
		__m128i f = _mm_loadu_si128((const __m128i *)tb[i]);
		f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[i][5]));
		f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[i][10]));
		f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[i][15]));
		f = _mm_xor_si128(f, _mm_loadu_si128((const __m128i *)&tb[i][20]));
		__m256i c = cl_gf16_compress_u16x16(_mm256_castsi128_si256(f));
		_Alignas(16) uint8_t pb[16];
		_mm_store_si128((__m128i *)pb, cl_gf16_pack_u16_to_bytes(c));
		for (int j = 0; j < 5; ++j) {
			C[i * 5 + j] = (gf_t)(C[i * 5 + j] ^ pb[j]);
		}
	}
}
#if 0
static inline void rct_m4f2_fold_F12(gf_t *F12, const gf_t *P11, const gf_t *T12) {
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j1 = 0; j1 < SNOVA_v; ++j1) {
			rct_m4f_bc_t bcp[SNOVA_v][5];
			rct_m4f2_build_bc(bcp, &P11[(i1 * SNOVA_v + j1) * SNOVA_v * SNOVA_l2], SNOVA_v);
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				gf_t *C = &F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2];
				__m256i a0[5], a1[5];
				for (int i = 0; i < 5; ++i) {
					a0[i] = _mm256_setzero_si256();
					a1[i] = _mm256_setzero_si256();
				}
				for (int j2 = 0; j2 < SNOVA_v; ++j2) {
					rct_m4f2_mac5(a0, a1, bcp[j2], &T12[(j2 * SNOVA_o + k1) * SNOVA_l2]);
				}
				rct_m4f2_fold5_xor(C, a0, a1);
			}
		}
}
#endif
static inline void rct_m4f2_fold_P22_pass1(gf_t *P22, const gf_t *T12, const gf_t *F12) {
	for (int j1 = 0; j1 < SNOVA_o; ++j1) {
		rct_m4f_bc_t bc[SNOVA_v][5];
		for (int idx = 0; idx < SNOVA_v; ++idx) {
			_Alignas(32) uint16_t ev[32];
			rct_m4_expand_buf(ev, &T12[(idx * SNOVA_o + j1) * SNOVA_l2], SNOVA_l2);
			for (int i = 0; i < 5; ++i) {
				bc[idx][i] = rct_m4f2_bc_mirror(&ev[i * 5]);
			}
		}
		for (int i1 = 0; i1 < SNOVA_m1; ++i1)
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				gf_t *C = &P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2];
				__m256i a0[5], a1[5];
				for (int i = 0; i < 5; ++i) {
					a0[i] = _mm256_setzero_si256();
					a1[i] = _mm256_setzero_si256();
				}
				for (int idx = 0; idx < SNOVA_v; ++idx) {
					rct_m4f2_mac5(a0, a1, bc[idx], &F12[((i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]);
				}
				rct_m4f2_fold5_xor(C, a0, a1);
			}
		SNOVA_CLEAR_OBJ(bc);
	}
}
#if 0
static inline void rct_m4f2_fold_P22_pass2(gf_t *P22, const gf_t *P21, const gf_t *T12) {
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j1 = 0; j1 < SNOVA_o; ++j1) {
			rct_m4f_bc_t bcp[SNOVA_v][5];
			rct_m4f2_build_bc(bcp, &P21[(i1 * SNOVA_o + j1) * SNOVA_v * SNOVA_l2], SNOVA_v);
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				gf_t *C = &P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2];
				__m256i a0[5], a1[5];
				for (int i = 0; i < 5; ++i) {
					a0[i] = _mm256_setzero_si256();
					a1[i] = _mm256_setzero_si256();
				}
				for (int idx = 0; idx < SNOVA_v; ++idx) {
					rct_m4f2_mac5(a0, a1, bcp[idx], &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
				}
				rct_m4f2_fold5_xor(C, a0, a1);
			}
		}
}
static inline void rct_m4f2_fold_row_bsec(gf_t *Crow, const gf_t *Arow, const gf_t *T12) {
	rct_m4f_bc_t bcp[SNOVA_v][5];
	rct_m4f2_build_bc(bcp, Arow, SNOVA_v);
	for (int k1 = 0; k1 < SNOVA_o; ++k1) {
		gf_t *C = &Crow[k1 * SNOVA_l2];
		__m256i a0[5], a1[5];
		for (int i = 0; i < 5; ++i) {
			a0[i] = _mm256_setzero_si256();
			a1[i] = _mm256_setzero_si256();
		}
		for (int idx = 0; idx < SNOVA_v; ++idx) {
			rct_m4f2_mac5(a0, a1, bcp[idx], &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
		}
		rct_m4f2_fold5_xor(C, a0, a1);
	}
}
#endif
#endif

#if RCT_F5_A4 || RCT_F5_M4 || RCT_F5_WIDE
#if RCT_F5_A4 || RCT_F5_M4
#if RCT_F5_A4
#define RCT_F5_BC_T     rct_a4f_bc_t
#define RCT_F5_ACC_T    rct_a4f_acc_t
#define RCT_F5_ZERO     rct_a4f_zero
#define RCT_F5_BC       rct_a4f_bc
#define RCT_F5_MAC      rct_a4f_mac
#define RCT_F5_FOLD     rct_a4f_fold
#define RCT_F5_XOR5     rct_a4f_xor5
#define RCT_F5_CELL_ADD rct_a4f_cell_add
#else
#define RCT_F5_BC_T     rct_m4f_bc_t
#define RCT_F5_ACC_T    rct_m4f_acc_t
#define RCT_F5_ZERO     rct_m4f_zero
#define RCT_F5_BC       rct_m4f_bc
#define RCT_F5_MAC      rct_m4f_mac
#define RCT_F5_FOLD     rct_m4f_fold
#define RCT_F5_XOR5     rct_m4f_xor5
#define RCT_F5_CELL_ADD rct_m4f_cell_add
#endif
#endif

static inline void rct_f5_fold_F12(gf_t *F12, const gf_t *P11, const gf_t *T12) {
#if RCT_F5_WIDE
	{ RCT_WIDE_SCRATCH_DECL; rct_wide_fold_F12(F12, P11, T12, P11aw, Fw); }
#elif RCT_F5_M4
	rct_m4f2_fold_F12(F12, P11, T12);
#else
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j1 = 0; j1 < SNOVA_v; ++j1) {
			RCT_F5_BC_T bcp[SNOVA_v][5];
			for (int j2 = 0; j2 < SNOVA_v; ++j2)
				for (int i = 0; i < 5; ++i) {
					bcp[j2][i] = RCT_F5_BC(&P11[((i1 * SNOVA_v + j1) * SNOVA_v + j2) * SNOVA_l2 + i * 5]);
				}
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				gf_t *C = &F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2];
				RCT_F5_ACC_T a[5];
				for (int i = 0; i < 5; ++i) {
					a[i] = RCT_F5_ZERO();
				}
				for (int j2 = 0; j2 < SNOVA_v; ++j2) {
					const gf_t *w = &T12[(j2 * SNOVA_o + k1) * SNOVA_l2];
					for (int i = 0; i < 5; ++i) {
						RCT_F5_MAC(&a[i], bcp[j2][i], w);
					}
				}
				for (int i = 0; i < 5; ++i) {
					RCT_F5_XOR5(&C[i * 5], RCT_F5_FOLD(a[i]));
				}
			}
		}
#endif
}

#if RCT_F5_A4 || RCT_F5_M4
static inline void rct_f5_fold_P22_pass1(gf_t *P22, const gf_t *T12, const gf_t *F12) {
#if RCT_F5_M4
	rct_m4f2_fold_P22_pass1(P22, T12, F12);
#else
	for (int j1 = 0; j1 < SNOVA_o; ++j1) {
		RCT_F5_BC_T bc[SNOVA_v][5];
		for (int idx = 0; idx < SNOVA_v; ++idx)
			for (int i = 0; i < 5; ++i) {
				bc[idx][i] = RCT_F5_BC(&T12[(idx * SNOVA_o + j1) * SNOVA_l2 + i * 5]);
			}
		for (int i1 = 0; i1 < SNOVA_m1; ++i1)
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				gf_t *C = &P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2];
				RCT_F5_ACC_T a[5];
				for (int i = 0; i < 5; ++i) {
					a[i] = RCT_F5_ZERO();
				}
				for (int idx = 0; idx < SNOVA_v; ++idx) {
					const gf_t *w = &F12[((i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2];
					for (int i = 0; i < 5; ++i) {
						RCT_F5_MAC(&a[i], bc[idx][i], w);
					}
				}
				for (int i = 0; i < 5; ++i) {
					RCT_F5_XOR5(&C[i * 5], RCT_F5_FOLD(a[i]));
				}
			}
		SNOVA_CLEAR_OBJ(bc);
	}
#endif
}

static inline void rct_f5_fold_P22_pass2(gf_t *P22, const gf_t *P21, const gf_t *T12) {
#if RCT_F5_WIDE
	{ RCT_WIDE_SCRATCH_DECL; rct_wide_fold_bsec(P22, P21, T12, SNOVA_o, 1, P11aw, Fw); }
#elif RCT_F5_M4
	rct_m4f2_fold_P22_pass2(P22, P21, T12);
#else
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j1 = 0; j1 < SNOVA_o; ++j1) {
			RCT_F5_BC_T bcp[SNOVA_v][5];
			for (int idx = 0; idx < SNOVA_v; ++idx)
				for (int i = 0; i < 5; ++i) {
					bcp[idx][i] = RCT_F5_BC(&P21[((i1 * SNOVA_o + j1) * SNOVA_v + idx) * SNOVA_l2 + i * 5]);
				}
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				gf_t *C = &P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2];
				RCT_F5_ACC_T a[5];
				for (int i = 0; i < 5; ++i) {
					a[i] = RCT_F5_ZERO();
				}
				for (int idx = 0; idx < SNOVA_v; ++idx) {
					const gf_t *w = &T12[(idx * SNOVA_o + k1) * SNOVA_l2];
					for (int i = 0; i < 5; ++i) {
						RCT_F5_MAC(&a[i], bcp[idx][i], w);
					}
				}
				for (int i = 0; i < 5; ++i) {
					RCT_F5_XOR5(&C[i * 5], RCT_F5_FOLD(a[i]));
				}
			}
		}
#endif
}
#if 0
static inline void rct_f5_fold_row_bsec(gf_t *Crow, const gf_t *Arow, const gf_t *T12) {
#if RCT_F5_M4
	rct_m4f2_fold_row_bsec(Crow, Arow, T12);
#else
	RCT_F5_BC_T bcp[SNOVA_v][5];
	for (int idx = 0; idx < SNOVA_v; ++idx)
		for (int i = 0; i < 5; ++i) {
			bcp[idx][i] = RCT_F5_BC(&Arow[idx * SNOVA_l2 + i * 5]);
		}
	for (int k1 = 0; k1 < SNOVA_o; ++k1) {
		gf_t *C = &Crow[k1 * SNOVA_l2];
		RCT_F5_ACC_T a[5];
		for (int i = 0; i < 5; ++i) {
			a[i] = RCT_F5_ZERO();
		}
		for (int idx = 0; idx < SNOVA_v; ++idx) {
			const gf_t *w = &T12[(idx * SNOVA_o + k1) * SNOVA_l2];
			for (int i = 0; i < 5; ++i) {
				RCT_F5_MAC(&a[i], bcp[idx][i], w);
			}
		}
		for (int i = 0; i < 5; ++i) {
			RCT_F5_XOR5(&C[i * 5], RCT_F5_FOLD(a[i]));
		}
	}
#endif
}
#endif
#endif
#endif
#endif
#endif


// #include "snova_rect_keygen.h"

#ifdef RCT_PROFILE
#include <x86intrin.h>
#include <stdio.h>
#define RCT_PT(x) do{ x = __rdtsc(); }while(0)
static uint64_t rct_pf[8];
#define RCT_PACC(i,a,b) rct_pf[i]+=(b)-(a)
static uint64_t rct_pf6[3];
#define RCT_PACC6(i,a,b) rct_pf6[i]+=(b)-(a)
static uint64_t rct_pf6b[3];
#define RCT_PACC6B(i,a,b) rct_pf6b[i]+=(b)-(a)
#else
#define RCT_PT(x) do{ (void)(x); }while(0)
#define RCT_PACC(i,a,b)
#define RCT_PACC6(i,a,b)
#define RCT_PACC6B(i,a,b)
#endif

#ifdef RCT_VPROF
#include <x86intrin.h>
#include <stdio.h>
#define VPT(x) do{ x = __rdtsc(); }while(0)
#define VPA(i,a,b) vpf[i]+=(b)-(a)
#ifndef VPROF_AT
#define VPROF_AT 2000
#endif
#else
#define VPT(x)
#define VPA(i,a,b)
#endif

static int rct_genkeys(uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
	rct_init();
#if RCT_SIGN_JOG
	rct_sj_ensure();
#endif
	uint64_t _pa, _pb;

#if SNOVA_WRAPPER_STACK
	_Alignas(32) gf_t T12[SNOVA_o * SNOVA_v * SNOVA_l2 + 16];
	_Alignas(32) gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#else
	static gf_t T12[SNOVA_o * SNOVA_v * SNOVA_l2 + 16];
	static gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#endif
	memset(T12 + SNOVA_o * SNOVA_v * SNOVA_l2, 0, 16);
	memset(P22, 0, sizeof(P22));

	expand_T12(T12, seed + SEED_LENGTH_PUBLIC);

#if RCT_KG_STREAM
	(void)_pa;
	(void)_pb;
#if SNOVA_WRAPPER_STACK
	_Alignas(32) gf_t F12[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2 + 16];
#else
	static gf_t F12[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2 + 16];
#endif
	memset(F12, 0, sizeof(F12));
	{
		snova_pgen_t pg;
		snova_pgen_init(&pg, seed);
		snova_prow_t rw;
#if RCT_Q_SIMD
		_Static_assert((uint32_t)SNOVA_v * (SNOVA_q - 1) * (SNOVA_q - 1)
		               + (SNOVA_q - 1) < 65536u,
		               "u16 accumulation overflow guard");
		_Static_assert(2u * (uint32_t)SNOVA_v * (SNOVA_q - 1) * (SNOVA_q - 1) < 65536u,
		               "u16 accumulation overflow guard");
		_Alignas(32) uint16_t rowacc[SNOVA_o * SNOVA_l2];
		while (snova_pgen_next_row(&pg, &rw)) {
			const int i1 = rw.mi, j1 = rw.ni;
			if (rw.block == 0) {
				memset(rowacc, 0, sizeof(rowacc));
				for (int idx = 0; idx < SNOVA_v; idx++)
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						rct_q_matmul4_add(&rowacc[k1 * SNOVA_l2],
						                  &rw.cells[(size_t)idx * SNOVA_l2],
						                  &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
				gf_t *frow = &F12[((size_t)(i1 * SNOVA_v + j1) * SNOVA_o) * SNOVA_l2];
				for (int t = 0; t < SNOVA_o * SNOVA_l2; t++) {
					frow[t] = (gf_t)(rowacc[t] % SNOVA_q);
				}
			} else if (rw.block == 1) {
				gf_t *frow = &F12[((size_t)(i1 * SNOVA_v + j1) * SNOVA_o) * SNOVA_l2];
				for (int t = 0; t < SNOVA_o * SNOVA_l2; t++) {
					frow[t] = (gf_t)((frow[t] + rw.cells[t]) % SNOVA_q);
				}
			} else {
				memset(rowacc, 0, sizeof(rowacc));
				for (int idx = 0; idx < SNOVA_v; idx++)
					for (int k1 = 0; k1 < SNOVA_o; k1++) {
						rct_q_matmul4_add(&rowacc[k1 * SNOVA_l2],
						                  &T12[(idx * SNOVA_o + j1) * SNOVA_l2],
						                  &F12[((size_t)(i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]);
						rct_q_matmul4_add(&rowacc[k1 * SNOVA_l2],
						                  &rw.cells[(size_t)idx * SNOVA_l2],
						                  &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
					}
				gf_t *prow = &P22[((size_t)(i1 * SNOVA_o + j1) * SNOVA_o) * SNOVA_l2];
				for (int t = 0; t < SNOVA_o * SNOVA_l2; t++) {
					prow[t] = (gf_t)((SNOVA_q - (rowacc[t] % SNOVA_q)) % SNOVA_q);
				}
			}
		}
		SNOVA_CLEAR_OBJ(rowacc);
#elif RCT_KGM_ACTIVE
		while (snova_pgen_next_row(&pg, &rw)) {
			const int i1 = rw.mi, j1 = rw.ni;
			if (rw.block == 0) {
				__m256i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm256_setzero_si256();
				}
				for (int idx = 0; idx < SNOVA_v; idx++) {
					__m256i bsh[4];
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&rw.cells[(size_t)idx * SNOVA_l2])), bsh);
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bsh,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&T12[(idx * SNOVA_o + k1) * SNOVA_l2]))));
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F12[((size_t)(i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[k1])));
			} else if (rw.block == 1) {
				gf_t *frow = &F12[((size_t)(i1 * SNOVA_v + j1) * SNOVA_o) * SNOVA_l2];
				for (int t = 0; t < SNOVA_o * SNOVA_l2; t++) {
					gf_set_add(&frow[t], rw.cells[t]);
				}
			} else {
				__m256i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm256_setzero_si256();
				}
				for (int idx = 0; idx < SNOVA_v; idx++) {
					__m256i bshT[4], bshP[4];
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&T12[(idx * SNOVA_o + j1) * SNOVA_l2])), bshT);
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&rw.cells[(size_t)idx * SNOVA_l2])), bshP);
					for (int k1 = 0; k1 < SNOVA_o; k1++) {
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bshT,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&F12[((size_t)(i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]))));
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bshP,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&T12[(idx * SNOVA_o + k1) * SNOVA_l2]))));
					}
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&P22[((size_t)(i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
					                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[k1])));
			}
		}
#else
		while (snova_pgen_next_row(&pg, &rw)) {
			const int i1 = rw.mi, j1 = rw.ni;
			if (rw.block == 0) {
				gf_t *frow = &F12[((size_t)(i1 * SNOVA_v + j1) * SNOVA_o) * SNOVA_l2];
#if RCT_F5_A4 || RCT_F5_M4
				rct_f5_fold_row_bsec(frow, rw.cells, T12);
#else
				for (int idx = 0; idx < SNOVA_v; idx++)
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						RCT_MATMUL_ADD_BSEC(&frow[k1 * SNOVA_l2],
						                    &rw.cells[(size_t)idx * SNOVA_l2],
						                    &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
#endif
			} else if (rw.block == 1) {
				gf_t *frow = &F12[((size_t)(i1 * SNOVA_v + j1) * SNOVA_o) * SNOVA_l2];
				for (int t = 0; t < SNOVA_o * SNOVA_l2; t++) {
					gf_set_add(&frow[t], rw.cells[t]);
				}
			} else {
				gf_t *prow = &P22[((size_t)(i1 * SNOVA_o + j1) * SNOVA_o) * SNOVA_l2];
#if RCT_F5_A4 || RCT_F5_M4
				rct_f5_fold_row_bsec(prow, rw.cells, T12);
#else
				for (int idx = 0; idx < SNOVA_v; idx++)
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						RCT_MATMUL_ADD_BSEC(&prow[k1 * SNOVA_l2],
						                    &rw.cells[(size_t)idx * SNOVA_l2],
						                    &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
#endif
			}
		}
#if RCT_F5_A4 || RCT_F5_M4
		rct_f5_fold_P22_pass1(P22, T12, F12);
#else
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int idx = 0; idx < SNOVA_v; idx++)
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					for (int j1 = 0; j1 < SNOVA_o; j1++)
						RCT_MATMUL_ADD_ASEC(&P22[((size_t)(i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
						                    &T12[(idx * SNOVA_o + j1) * SNOVA_l2],
						                    &F12[((size_t)(i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]);
#endif
#if SNOVA_q != 16
		for (int t = 0; t < SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2; t++) {
			P22[t] = (SNOVA_q - P22[t]) % SNOVA_q;
		}
#endif
#endif
	}
#else
#if SNOVA_WRAPPER_STACK
	_Alignas(32) gf_t P_matrix[NUM_PUB_GF];
#else
	gf_t *P_matrix = rct_pub_Pmatrix;
#endif
	RCT_PT(_pa);
	expand_public(P_matrix, seed);
	RCT_PT(_pb);
	RCT_PACC(0, _pa, _pb);

	gf_t *P11 = P_matrix;
	gf_t *P12 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;
#if SNOVA_WRAPPER_STACK
	_Alignas(32) gf_t F12[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2 + 16];
#else
	static gf_t F12[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2 + 16];
#endif
	memset(F12, 0, sizeof(F12));

	RCT_PT(_pa);
#if RCT_Q_SIMD
	_Static_assert((uint32_t)SNOVA_v * (SNOVA_q - 1) * (SNOVA_q - 1)
	               + (SNOVA_q - 1) < 65536u,
	               "u16 accumulation overflow guard");
	_Static_assert(2u * (uint32_t)SNOVA_v * (SNOVA_q - 1) * (SNOVA_q - 1) < 65536u,
	               "u16 accumulation overflow guard");
	{
		static uint16_t F12u[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2];
		memset(F12u, 0, sizeof(F12u));
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++)
				for (int idx = 0; idx < SNOVA_v; idx++)
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						rct_q_matmul4_add(&F12u[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
						                  &P11[((i1 * SNOVA_v + j1) * SNOVA_v + idx) * SNOVA_l2],
						                  &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
		for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2; i1++) {
			F12[i1] = (gf_t)((F12u[i1] + P12[i1]) % SNOVA_q);
		}
		SNOVA_CLEAR_OBJ(F12u);
	}
	RCT_PT(_pb);
	RCT_PACC(1, _pa, _pb);

	RCT_PT(_pa);
	{
		static uint16_t P22u[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
		memset(P22u, 0, sizeof(P22u));
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int idx = 0; idx < SNOVA_v; idx++)
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					for (int j1 = 0; j1 < SNOVA_o; j1++)
						rct_q_matmul4_add(&P22u[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
						                  &T12[(idx * SNOVA_o + j1) * SNOVA_l2],
						                  &F12[((i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]);
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_o; j1++)
				for (int idx = 0; idx < SNOVA_v; idx++)
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						rct_q_matmul4_add(&P22u[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
						                  &P21[((i1 * SNOVA_o + j1) * SNOVA_v + idx) * SNOVA_l2],
						                  &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
		for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2; i1++) {
			P22[i1] = (gf_t)((SNOVA_q - (P22u[i1] % SNOVA_q)) % SNOVA_q);
		}
		SNOVA_CLEAR_OBJ(P22u);
	}
	RCT_PT(_pb);
	RCT_PACC(2, _pa, _pb);
#elif RCT_KGM_ACTIVE
	{
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++) {
				__m256i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm256_setzero_si256();
				}
				for (int idx = 0; idx < SNOVA_v; idx++) {
					__m256i bsh[4];
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&P11[((i1 * SNOVA_v + j1) * SNOVA_v + idx) * SNOVA_l2])), bsh);
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bsh,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&T12[(idx * SNOVA_o + k1) * SNOVA_l2]))));
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[k1])));
			}
	}
	RCT_PT(_pb);
	RCT_PACC(1, _pa, _pb);

	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2; i1++) {
		gf_set_add(&F12[i1], P12[i1]);
	}

	RCT_PT(_pa);
	{
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_o; j1++) {
				__m256i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm256_setzero_si256();
				}
				for (int idx = 0; idx < SNOVA_v; idx++) {
					__m256i bshT[4], bshP[4];
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&T12[(idx * SNOVA_o + j1) * SNOVA_l2])), bshT);
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&P21[((i1 * SNOVA_o + j1) * SNOVA_v + idx) * SNOVA_l2])), bshP);
					for (int k1 = 0; k1 < SNOVA_o; k1++) {
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bshT,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&F12[((i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]))));
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bshP,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&T12[(idx * SNOVA_o + k1) * SNOVA_l2]))));
					}
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
					                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[k1])));
			}
	}
	RCT_PT(_pb);
	RCT_PACC(2, _pa, _pb);
#else
#if RCT_F5_A4 || RCT_F5_M4 || RCT_F5_WIDE
	rct_f5_fold_F12(F12, P11, T12);
#elif RCT_L4G_FOLD
	rct_l4g_fold_bsec(F12, P11, T12, SNOVA_v, 0);
#else
	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int j1 = 0; j1 < SNOVA_v; j1++)
			for (int idx = 0; idx < SNOVA_v; idx++)
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					RCT_MATMUL_ADD_BSEC(&F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                    &P11[((i1 * SNOVA_v + j1) * SNOVA_v + idx) * SNOVA_l2],
					                    &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
#endif
	RCT_PT(_pb);
	RCT_PACC(1, _pa, _pb);

	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2; i1++) {
		gf_set_add(&F12[i1], P12[i1]);
	}

	RCT_PT(_pa);
#if RCT_F5_A4 || RCT_F5_M4
	rct_f5_fold_P22_pass1(P22, T12, F12);
	rct_f5_fold_P22_pass2(P22, P21, T12);
#elif RCT_L4G_FOLD
	rct_l4g_fold_asec(P22, T12, F12);
	rct_l4g_fold_bsec(P22, P21, T12, SNOVA_o, 1);
#else
	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int idx = 0; idx < SNOVA_v; idx++)
			for (int k1 = 0; k1 < SNOVA_o; k1++)
				for (int j1 = 0; j1 < SNOVA_o; j1++)
					RCT_MATMUL_ADD_ASEC(&P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
					                    &T12[(idx * SNOVA_o + j1) * SNOVA_l2],
					                    &F12[((i1 * SNOVA_v + idx) * SNOVA_o + k1) * SNOVA_l2]);

	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int j1 = 0; j1 < SNOVA_o; j1++)
			for (int idx = 0; idx < SNOVA_v; idx++)
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					RCT_MATMUL_ADD_BSEC(&P22[((i1 * SNOVA_o + j1) * SNOVA_o + k1) * SNOVA_l2],
					                    &P21[((i1 * SNOVA_o + j1) * SNOVA_v + idx) * SNOVA_l2],
					                    &T12[(idx * SNOVA_o + k1) * SNOVA_l2]);
#endif

#if SNOVA_q != 16
	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2; i1++) {
		P22[i1] = (SNOVA_q - P22[i1]) % SNOVA_q;
	}
#endif
	RCT_PT(_pb);
	RCT_PACC(2, _pa, _pb);
#endif
#endif
#ifdef RCT_PROFILE
	fprintf(stderr, "[KG] expand_pub=%lu F12=%lu P22=%lu\n", (unsigned long)rct_pf[0], (unsigned long)rct_pf[1], (unsigned long)rct_pf[2]);
	rct_pf[0] = rct_pf[1] = rct_pf[2] = 0;
#endif

	memcpy(pk, seed, SEED_LENGTH_PUBLIC);
	compress_pk(pk + SEED_LENGTH_PUBLIC, P22);
	memcpy(sk, seed, SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE);
#if HASH_PK
	shake256(sk + SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE, BYTES_PK_HASH, pk, BYTES_PK);
#endif
	SNOVA_CLEAR_OBJ(T12);
	SNOVA_CLEAR_OBJ(F12);
	return 0;
}

//#include "snova_rect_sign.h"
#ifndef SNOVA_RECT_SIGN_H
#define SNOVA_RECT_SIGN_H

#ifndef RCT_SKX_SLIM
#define RCT_SKX_SLIM 0
#endif

typedef struct {
	uint8_t sk[BYTES_SK];
	gf_t T12[SNOVA_o * SNOVA_v * SNOVA_l2 + 16];
#if RCT_SIGN_STREAM
	gf_t abq[ABQ_RAW_N];
#elif RCT_SKX_SLIM
	gf_t P11[SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2 + 32];
	gf_t abq[ABQ_RAW_N];
#else
	gf_t P_matrix[NUM_PUB_GF];
#endif
	gf_t F21[SNOVA_m1 * SNOVA_o * SNOVA_v * SNOVA_l2];
	gf_t F12[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2];
	gf_t Am[SNOVA_o * SNOVA_alpha * SNOVA_r2 + 16];
	gf_t Bm[SNOVA_o * SNOVA_alpha * SNOVA_lr + 16];
	gf_t Q1[SNOVA_o * SNOVA_alpha * SNOVA_l2];
	gf_t Q2[SNOVA_o * SNOVA_alpha * SNOVA_l2 + 16];
} rct_skx_t;

#if defined(RCT_OQ_DEVFLOW) && (RCT_OQ_DEVFLOW + 0) && defined(RCT_Q_SIMD) && \
    (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC
#define RCT_OQDF 1
#else
#define RCT_OQDF 0
#endif
#if RCT_OQDF
static _Alignas(32) uint16_t rct_oq_whip_w[SNOVA_l * SNOVA_v * RCT_Q_LRP];
static _Alignas(32) uint16_t rct_oq_sum_t1u[SNOVA_m1 * SNOVA_l2 * SNOVA_r2 + 64];
static _Alignas(32) uint16_t rct_oq_P11u[SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2];
static _Alignas(32) uint16_t rct_oq_F21u[SNOVA_m1 * SNOVA_o * SNOVA_v * SNOVA_l2];
static _Alignas(32) uint16_t rct_oq_F12u[SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2];
static inline void rct_oq_expand_u16(uint16_t *dst, const gf_t *src, int n) {
	int i = 0;
	for (; i + 16 <= n; i += 16)
		_mm256_storeu_si256((__m256i *)(dst + i),
		                    _mm256_cvtepu8_epi16(_mm_loadu_si128((const __m128i *)(src + i))));
	for (; i < n; ++i) {
		dst[i] = src[i];
	}
}
#endif

#if RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC
enum { RCT_GN = SNOVA_o * SNOVA_lr, RCT_GPAD = (SNOVA_o * SNOVA_lr / 16 + 1) * 16 };
#endif
#if SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR)
enum { RCT_SNB = (SNOVA_o * SNOVA_lr + 1 + 31) / 32 };
#endif
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
enum { RCT_CM_LR16 = SNOVA_lr32 / 16,
       RCT_CM_OLR16 = (SNOVA_o * SNOVA_lr + 15) / 16,
       RCT_CM_OLR = RCT_CM_OLR16 * 16,
       RCT_CM_OLR32 = (RCT_CM_OLR + 31) / 32 * 32,
       RCT_CM_OLR32N = RCT_CM_OLR32 / 32
     };
#if (SNOVA_r == SNOVA_l) && defined(RCT_SQ_CM_TIGHT) && (RCT_SQ_CM_TIGHT + 0)
enum { RCT_CMW = SNOVA_lr, RCT_CMW16 = SNOVA_lr / 16 };
#else
enum { RCT_CMW = SNOVA_lr32, RCT_CMW16 = SNOVA_lr32 / 16 };
#endif
#endif

typedef struct rct_cm_ctx {
	uint16_t *cm_whip;
	uint8_t *cm_whipb;
	const uint16_t *Amx, *Bmx, *Q1x, *Q2x, *q1x, *q2x;
} rct_cm_ctx;

typedef struct rct_sign_ctx {
	rct_skx_t *skx;
	const gf_t *T12, *P11, *aptr, *F21, *F12, *Am, *Bm, *Q1, *Q2, *q1, *q2;
	gf_t *signature_in_GF;
	gf_t *hash_in_GF16;
	gf_t *Fvv;
	gf_t (*gauss)[SNOVA_o * SNOVA_lr + 1 + 64];
	gf_t *sum_t1;
	gf_t *whipped_sig;
	gf_t *whipped_F21, *whipped_F12;
	rct_cm_ctx *cm;
	uint8_t num_sign;
	int flag_redo;
	uint64_t _pa, _pb;
} rct_sign_ctx;

// #include "platforms/x86_avx2/rct_sign_gfni.h"
#ifndef RCT_SIGN_GFNI_H
#define RCT_SIGN_GFNI_H

#if SNOVA_Q == 16 && (RCT_USE_GFNI || RCT_HOT_QRP16) && !defined(RCT_GAUSS_SCALAR) && SNOVA_L == 4 \
    && !(RCT_HOT_QRP16 && !RCT_USE_GFNI && defined(RCT_T12_MULLO) && (RCT_T12_MULLO + 0))
static void rct_sign_apply_t12_gfni(rct_sign_ctx *c, const gf_t *solpad) {
	gf_t *signature_in_GF = c->signature_in_GF;
	const gf_t *T12 = c->T12;
	for (int index = 0; index < SNOVA_v; ++index) {
		gf_t *sigrow = &signature_in_GF[index * SNOVA_lr];
		for (int i1 = 0; i1 < SNOVA_l; ++i1) {
			__m128i acc = _mm_setzero_si128();
			const gf_t *tb = &T12[(index * SNOVA_o) * SNOVA_l2 + i1 * SNOVA_l];
			for (int mi = 0; mi < SNOVA_o; ++mi, tb += SNOVA_l2)
				for (int k1 = 0; k1 < SNOVA_l; ++k1)
					acc = _mm_xor_si128(acc, RCT_GFMUL128(
					                        _mm_set1_epi8((char)tb[k1]),
					                        _mm_loadu_si128((const __m128i *)&solpad[mi * SNOVA_lr + k1 * SNOVA_r])));
			acc = rct_gfni_cleanup128(acc);
			uint8_t out16[16];
			_mm_storeu_si128((__m128i *)out16, acc);
			for (int j1 = 0; j1 < SNOVA_r; ++j1) {
				sigrow[i1 * SNOVA_r + j1] ^= out16[j1];
			}
		}
	}
}
#endif

#if SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR)
static void rct_sign_backsub_gfni(rct_sign_ctx *c, gf_t *solpad, gf_t *solution) {
	gf_t (*gauss)[SNOVA_o * SNOVA_lr + 1 + 64] = c->gauss;
	memset(solpad, 0, (size_t)(RCT_SNB * 32 + 32) * sizeof(gf_t));
	for (int i = SNOVA_o * SNOVA_lr - 1; i >= 0; --i) {
		__m256i acc = _mm256_setzero_si256();
		for (int b = (i + 1) / 32; b < RCT_SNB; ++b)
			acc = _mm256_xor_si256(acc, RCT_GFMUL256(
			                           _mm256_loadu_si256((const __m256i *)&gauss[i][b * 32]),
			                           _mm256_load_si256((const __m256i *)&solpad[b * 32])));
#if RCT_USE_GFNI || RCT_HOT_QRP16
		acc = rct_gfni_cleanup256(acc);
#else
		acc = rct_sj_cleanup256(acc);
#endif
		__m128i x = _mm_xor_si128(_mm256_castsi256_si128(acc),
		                          _mm256_extracti128_si256(acc, 1));
		x = _mm_xor_si128(x, _mm_srli_si128(x, 8));
		x = _mm_xor_si128(x, _mm_srli_si128(x, 4));
		uint32_t w = (uint32_t)_mm_cvtsi128_si32(x);
		w ^= w >> 16;
		w ^= w >> 8;
		solpad[i] = (gf_t)(gauss[i][SNOVA_o * SNOVA_lr] ^ (w & 0x0fu));
	}
	for (int cc = 0; cc < SNOVA_o * SNOVA_lr; ++cc) {
		solution[cc] = solpad[cc];
	}
}
#endif

#if !(RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR))
static int rct_sign_gauss_q16(rct_sign_ctx *c) {
	gf_t (*gauss)[SNOVA_o * SNOVA_lr + 1 + 64] = c->gauss;
	int flag_redo;
	{
		enum { RCT_OLR = SNOVA_o * SNOVA_lr,
		       RCT_W = SNOVA_o * SNOVA_lr + 1,
		       RCT_WNB = (SNOVA_o * SNOVA_lr + 1 + 31) / 32
		     };
		uint32_t redo_acc = 0;
		for (int i = 0; i < RCT_OLR; ++i) {
#if (RCT_USE_SIMD || RCT_SIGN_JOG) && !defined(RCT_GAUSS_SCALAR)
			const int b0 = i / 32;
			__m256i rowi[RCT_WNB];
			for (int b = b0; b < RCT_WNB; ++b) {
				rowi[b] = _mm256_loadu_si256((const __m256i *)&gauss[i][b * 32]);
			}
			uint8_t ii = gauss[i][i];
			for (int j = i + 1; j < RCT_OLR; ++j) {
				uint32_t need = (1u - ct_gf_nz((uint32_t)ii)) & ct_gf_nz((uint32_t)gauss[j][i]);
				__m256i mv = _mm256_set1_epi8((char)(0u - need));
				for (int b = b0; b < RCT_WNB; ++b)
					rowi[b] = _mm256_xor_si256(rowi[b],
					                           _mm256_and_si256(_mm256_loadu_si256((const __m256i *)&gauss[j][b * 32]), mv));
				ii ^= (uint8_t)((0u - need) & (uint32_t)gauss[j][i]);
			}
			for (int b = b0; b < RCT_WNB; ++b) {
				_mm256_storeu_si256((__m256i *)&gauss[i][b * 32], rowi[b]);
			}
			redo_acc |= (1u - ct_gf_nz((uint32_t)ii));
			gf_t t_GF16 = gf_inv_sec(ii);
			rct_gauss_row_scale_sec(gauss[i], t_GF16, i, RCT_W);
			for (int j = i + 1; j < RCT_OLR; ++j) {
				rct_gauss_row_axpy_sec(gauss[j], gauss[i], gauss[j][i], i, RCT_W);
			}
#else
			for (int j = i + 1; j < RCT_OLR; ++j) {
				uint32_t need = (1u - ct_gf_nz((uint32_t)gauss[i][i])) & ct_gf_nz((uint32_t)gauss[j][i]);
				gf_t m = (gf_t)(0u - need);
				for (int k = i; k < RCT_W; ++k) {
					gauss[i][k] = gf_add(gauss[i][k], (gf_t)(gauss[j][k] & m));
				}
			}
			redo_acc |= (1u - ct_gf_nz((uint32_t)gauss[i][i]));
			gf_t t_GF16 = gf_inv_sec(gauss[i][i]);
			for (int k = i; k < RCT_W; ++k) {
				gauss[i][k] = gf_mult_sec(gauss[i][k], t_GF16);
			}
			for (int j = i + 1; j < RCT_OLR; ++j) {
				gf_t gji = gauss[j][i];
				for (int k = i; k < RCT_W; ++k) {
					gauss[j][k] = gf_sub(gauss[j][k], gf_mult_sec(gauss[i][k], gji));
				}
			}
#endif
		}
		flag_redo = (int)redo_acc;
		SNOVA_CT_DECLASSIFY(&flag_redo, sizeof flag_redo);
	}
	return flag_redo;
}
#endif

#if !RCT_SIGN_JOG \
    && !(RCT_Q_SIMD && (SNOVA_r == SNOVA_l) && RCT_Q_HAVE_MAGIC && !defined(RCT_FVV_SCALAR)) \
    && !(RCT_Q_SIMD && (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC) \
    && !defined(RCT_CMS2_ONLY)
static void rct_sign_fvv_std(rct_sign_ctx *c) {
	const gf_t *q1 = c->q1, *q2 = c->q2, *Am = c->Am, *Bm = c->Bm, *sum_t1 = c->sum_t1;
	gf_t *Fvv_in_GF16Matrix = c->Fvv;
#if RCT_USE_GFNI && (SNOVA_r == SNOVA_l) && (SNOVA_L == 4)
	const __m128i sD0 = _mm_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12);
	const __m128i sD1 = _mm_setr_epi8(1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13);
	const __m128i sD2 = _mm_setr_epi8(2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14);
	const __m128i sD3 = _mm_setr_epi8(3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15);
	const __m128i sE0 = _mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3);
	const __m128i sE1 = _mm_setr_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7);
	const __m128i sE2 = _mm_setr_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11);
	const __m128i sE3 = _mm_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15);
	for (int mi = 0; mi < SNOVA_o; ++mi) {
		__m128i Facc = _mm_setzero_si128();
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			int mi_prime = i_prime(mi, alpha);
			__m128i t1 = _mm_setzero_si128();
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				__m128i t0 = _mm_setzero_si128();
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					t0 = _mm_xor_si128(t0, RCT_SV128(
					                       RCT_BC128(q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
					                       _mm_loadu_si128((const __m128i *)
					                                       &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2])));
				t0 = rct_gfni_cleanup128(t0);
				t1 = _mm_xor_si128(t1, RCT_SV128(
				                       RCT_BC128(q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]), t0));
			}
			t1 = rct_gfni_cleanup128(t1);
			__m128i Bc = _mm_loadu_si128((const __m128i *)&Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr]);
			__m128i acc = RCT_GFMUL128(_mm_shuffle_epi8(t1, sD0), _mm_shuffle_epi8(Bc, sE0));
			acc = _mm_xor_si128(acc, RCT_GFMUL128(_mm_shuffle_epi8(t1, sD1), _mm_shuffle_epi8(Bc, sE1)));
			acc = _mm_xor_si128(acc, RCT_GFMUL128(_mm_shuffle_epi8(t1, sD2), _mm_shuffle_epi8(Bc, sE2)));
			acc = _mm_xor_si128(acc, RCT_GFMUL128(_mm_shuffle_epi8(t1, sD3), _mm_shuffle_epi8(Bc, sE3)));
			__m128i t2 = rct_gfni_cleanup128(acc);
			__m128i Ac = _mm_loadu_si128((const __m128i *)&Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2]);
			Facc = _mm_xor_si128(Facc, RCT_GFMUL128(_mm_shuffle_epi8(Ac, sD0), _mm_shuffle_epi8(t2, sE0)));
			Facc = _mm_xor_si128(Facc, RCT_GFMUL128(_mm_shuffle_epi8(Ac, sD1), _mm_shuffle_epi8(t2, sE1)));
			Facc = _mm_xor_si128(Facc, RCT_GFMUL128(_mm_shuffle_epi8(Ac, sD2), _mm_shuffle_epi8(t2, sE2)));
			Facc = _mm_xor_si128(Facc, RCT_GFMUL128(_mm_shuffle_epi8(Ac, sD3), _mm_shuffle_epi8(t2, sE3)));
		}
		_mm_storeu_si128((__m128i *)&Fvv_in_GF16Matrix[mi * SNOVA_lr],
		                 rct_gfni_cleanup128(Facc));
	}
#else
	for (int mi = 0; mi < SNOVA_o; ++mi)
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			int mi_prime = i_prime(mi, alpha);
			gf_t gf16m_temp1[SNOVA_r2] = {0};
			gf_t gf16m_temp2[SNOVA_lr + 16] = {0};
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r2 <= 64
			{
				__m256i t1lo = _mm256_setzero_si256(), t1hi = _mm256_setzero_si256();
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					__m256i slo = _mm256_setzero_si256(), shi = _mm256_setzero_si256();
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						const gf_t *base = &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2];
						__m256i qv = RCT_BC(q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]);
						slo = _mm256_xor_si256(slo, RCT_SV(qv, _mm256_loadu_si256((const __m256i *)base)));
						shi = _mm256_xor_si256(shi, RCT_SV(qv, _mm256_loadu_si256((const __m256i *)(base + 32))));
					}
					_Alignas(32) uint8_t sb[64];
					_mm256_store_si256((__m256i *)sb, slo);
					_mm256_store_si256((__m256i *)(sb + 32), shi);
					for (int k = 0; k < SNOVA_r2; ++k) {
						sb[k] = rct_gfni_cleanup(sb[k]);
					}
					__m256i qa = RCT_BC(q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]);
					t1lo = _mm256_xor_si256(t1lo, RCT_SV(qa, _mm256_loadu_si256((const __m256i *)sb)));
					t1hi = _mm256_xor_si256(t1hi, RCT_SV(qa, _mm256_loadu_si256((const __m256i *)(sb + 32))));
				}
				_Alignas(32) uint8_t t1b[64];
				_mm256_store_si256((__m256i *)t1b, t1lo);
				_mm256_store_si256((__m256i *)(t1b + 32), t1hi);
				for (int k = 0; k < SNOVA_r2; ++k) {
					gf16m_temp1[k] = rct_gfni_cleanup(t1b[k]);
				}
			}
#else
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				gf_t sumb[SNOVA_r2] = {0};
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							gf_set_add(&sumb[i1 * SNOVA_r + j1],
							           gf_mult_sec(sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1],
							                       q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]));
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						gf_set_add(&gf16m_temp1[i1 * SNOVA_r + j1],
						           gf_mult_sec(sumb[i1 * SNOVA_r + j1], q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]));
			}
#endif
#if RCT_USE_SIMD && SNOVA_l == 4
			rct_matmul_l4rows_sec(gf16m_temp2, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_r, SNOVA_r);
			rct_matmul_l4rows_add(&Fvv_in_GF16Matrix[mi * SNOVA_lr], &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp2, SNOVA_r, SNOVA_r);
#else
			gf_mat_mul_add_lr_sec(gf16m_temp2, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_r, SNOVA_r, SNOVA_l);
			gf_mat_mul_add_lr_sec(&Fvv_in_GF16Matrix[mi * SNOVA_lr], &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp2,
			                      SNOVA_r, SNOVA_r, SNOVA_l);
#endif
		}
#endif
}
#endif

#if !RCT_OQDF && RCT_USE_SIMD && SNOVA_r <= 16
static void rct_sign_whipbuild_q16(rct_sign_ctx *c) {
	const gf_t *signature_in_GF = c->signature_in_GF;
	gf_t *whipped_sig = c->whipped_sig;
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int i1 = 0; i1 < SNOVA_l; i1++) {
				__m128i acc = _mm_setzero_si128();
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					acc = _mm_xor_si128(acc, RCT_SV128(
					                        RCT_BC128(rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1]),
					                        _mm_loadu_si128((const __m128i *)&signature_in_GF[ni * SNOVA_lr + k1 * SNOVA_r])));
				acc = rct_gfni_cleanup128(acc);
				_Alignas(16) uint8_t tmpws[16];
				_mm_store_si128((__m128i *)tmpws, acc);
				memcpy(&whipped_sig[(ab * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r], tmpws, SNOVA_r);
			}
}
#endif

#if RCT_USE_SIMD
static void rct_sign_s1whip_simd(rct_sign_ctx *c) {
	const gf_t *whipped_sig = c->whipped_sig;
	const gf_t *P11 = c->P11;
	gf_t *sum_t1 = c->sum_t1;
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
	uint16_t *rct_cm_whip = c->cm->cm_whip;
#if RCT_USE_GFNI
	uint8_t *rct_cm_whipb = c->cm->cm_whipb;
#endif
#endif
	{
		RCT_SCRATCH _Alignas(32) uint8_t whipped_sig2[SNOVA_l * SNOVA_v * SNOVA_lr32];
		memset(whipped_sig2, 0, sizeof(whipped_sig2));
#if RCT_USE_SIMD && SNOVA_r <= 16 && !RCT_OQDF
		for (int idx = 0; idx < SNOVA_v; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int ab = 0; ab < SNOVA_l; ++ab)
					memcpy(&whipped_sig2[idx * SNOVA_l * SNOVA_lr32 + i1 * SNOVA_lr32 + ab * SNOVA_r],
					       &whipped_sig[(ab * SNOVA_v + idx) * SNOVA_lr + i1 * SNOVA_r], SNOVA_r);
#else
		const gf_t *signature_in_GF = c->signature_in_GF;
		for (int idx = 0; idx < SNOVA_v; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int ab = 0; ab < SNOVA_l; ++ab) {
					__m256i acc = _mm256_setzero_si256();
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						acc = _mm256_xor_si256(acc, RCT_SV(
						                           RCT_BC(rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1]),
						                           _mm256_loadu_si256((const __m256i *)&signature_in_GF[idx * SNOVA_lr + k1 * SNOVA_r])));
					_Alignas(32) uint8_t tmpws[32];
					_mm256_store_si256((__m256i *)tmpws, rct_gfni_cleanup256(acc));
					for (int j1 = 0; j1 < SNOVA_r; j1++) {
						whipped_sig2[idx * SNOVA_l * SNOVA_lr32 + i1 * SNOVA_lr32 + ab * SNOVA_r + j1] = tmpws[j1];
					}
				}
#endif
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
		for (int idx = 0; idx < SNOVA_v; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int c1 = 0; c1 < RCT_CMW; c1 += 16)
					_mm256_store_si256(
					    (__m256i *)&rct_cm_whip[(i1 * SNOVA_v + idx) * RCT_CMW + c1],
					    gf16_expand_u16x16(_mm256_cvtepu8_epi16(_mm_load_si128((const __m128i *)
					        &whipped_sig2[idx * SNOVA_l * SNOVA_lr32 + i1 * SNOVA_lr32 + c1]))));
#if RCT_USE_GFNI
		for (int idx = 0; idx < SNOVA_v; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				memcpy(&rct_cm_whipb[(i1 * SNOVA_v + idx) * RCT_CMW],
				       &whipped_sig2[idx * SNOVA_l * SNOVA_lr32 + i1 * SNOVA_lr32], RCT_CMW);
#endif
#endif

#ifdef RCT_RD_ACTIVE
		enum { RCT_RD_VG = SNOVA_v / 2 };
		RCT_SCRATCH _Alignas(32) uint8_t whipped_sig2d[RCT_RD_VG * SNOVA_l * 32];
		for (int g = 0; g < RCT_RD_VG; ++g)
			for (int k1 = 0; k1 < SNOVA_l; ++k1) {
				__m128i a2a = _mm_load_si128((const __m128i *)
				                             &whipped_sig2[(2 * g) * SNOVA_l * SNOVA_lr32 + k1 * SNOVA_lr32]);
				__m128i a2b = _mm_load_si128((const __m128i *)
				                             &whipped_sig2[(2 * g + 1) * SNOVA_l * SNOVA_lr32 + k1 * SNOVA_lr32]);
				_mm256_store_si256((__m256i *)&whipped_sig2d[(g * SNOVA_l + k1) * 32],
				                   _mm256_inserti128_si256(_mm256_castsi128_si256(a2a), a2b, 1));
			}
#endif

#if RCT_USE_GFNI && (SNOVA_r == SNOVA_l) && (SNOVA_L == 4)
		{
			const __m256i s1_Pcol_y[4] = {
				_mm256_broadcastsi128_si256(_mm_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12)),
				_mm256_broadcastsi128_si256(_mm_setr_epi8(1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13)),
				_mm256_broadcastsi128_si256(_mm_setr_epi8(2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14)),
				_mm256_broadcastsi128_si256(_mm_setr_epi8(3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15)),
			};
			const __m256i s1_Prow_y[4] = {
				_mm256_broadcastsi128_si256(_mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3)),
				_mm256_broadcastsi128_si256(_mm_setr_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7)),
				_mm256_broadcastsi128_si256(_mm_setr_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11)),
				_mm256_broadcastsi128_si256(_mm_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15)),
			};
			const __m128i s1_Prow[4] = {
				_mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3),
				_mm_setr_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7),
				_mm_setr_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11),
				_mm_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15),
			};
			const __m128i s1_Pcolt[4] = {
				_mm_setr_epi8(0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3),
				_mm_setr_epi8(4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7),
				_mm_setr_epi8(8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11),
				_mm_setr_epi8(12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15),
			};
			RCT_SCRATCH __m128i s1_wsig[SNOVA_l][SNOVA_v];
			RCT_SCRATCH __m256i s1_Brow2[SNOVA_l / 2][SNOVA_v][SNOVA_l];
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int g = 0; g < SNOVA_l / 2; ++g) {
					__m128i c0 = _mm_loadu_si128((const __m128i *)&whipped_sig[((2 * g) * SNOVA_v + nj) * SNOVA_lr]);
					__m128i c1 = _mm_loadu_si128((const __m128i *)&whipped_sig[((2 * g + 1) * SNOVA_v + nj) * SNOVA_lr]);
					s1_wsig[2 * g][nj]     = c0;
					s1_wsig[2 * g + 1][nj] = c1;
					__m256i cc = _mm256_inserti128_si256(_mm256_castsi128_si256(c0), c1, 1);
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						s1_Brow2[g][nj][k1] = _mm256_shuffle_epi8(cc, s1_Prow_y[k1]);
					}
				}
			RCT_SCRATCH __m128i s1_sum_t0[SNOVA_l][SNOVA_v];
			for (int mi = 0; mi < SNOVA_m1; ++mi) {
				for (int ni = 0; ni < SNOVA_v; ++ni) {
					__m256i acc[SNOVA_l / 2];
					for (int g = 0; g < SNOVA_l / 2; ++g) {
						acc[g] = _mm256_setzero_si256();
					}
					for (int nj = 0; nj < SNOVA_v; ++nj) {
						__m256i fy = _mm256_broadcastsi128_si256(_mm_loadu_si128(
						        (const __m128i *)&P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2]));
						__m256i Acol[SNOVA_l];
						for (int k1 = 0; k1 < SNOVA_l; ++k1) {
							Acol[k1] = _mm256_shuffle_epi8(fy, s1_Pcol_y[k1]);
						}
						for (int g = 0; g < SNOVA_l / 2; ++g)
							for (int k1 = 0; k1 < SNOVA_l; ++k1)
								acc[g] = _mm256_xor_si256(acc[g],
								                          RCT_GFMUL256(Acol[k1], s1_Brow2[g][nj][k1]));
					}
					for (int g = 0; g < SNOVA_l / 2; ++g) {
						__m256i cy = rct_gfni_cleanup256(acc[g]);
						s1_sum_t0[2 * g][ni]     = _mm256_castsi256_si128(cy);
						s1_sum_t0[2 * g + 1][ni] = _mm256_extracti128_si256(cy, 1);
					}
				}
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						__m128i acc = _mm_setzero_si128();
						for (int ni = 0; ni < SNOVA_v; ++ni) {
							__m128i w = s1_wsig[a1][ni], s = s1_sum_t0[b1][ni];
							for (int k1 = 0; k1 < SNOVA_l; ++k1)
								acc = _mm_xor_si128(acc, RCT_GFMUL128(
								                        _mm_shuffle_epi8(w, s1_Pcolt[k1]),
								                        _mm_shuffle_epi8(s, s1_Prow[k1])));
						}
						_mm_storeu_si128((__m128i *)&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2],
						                 rct_gfni_cleanup128(acc));
					}
			}
			SNOVA_CLEAR_OBJ(s1_wsig);
			SNOVA_CLEAR_OBJ(s1_Brow2);
			SNOVA_CLEAR_OBJ(s1_sum_t0);
		}
#else
		RCT_SCRATCH _Alignas(32) uint8_t sum_t1p[SNOVA_m1 * SNOVA_l * SNOVA_r * SNOVA_lr32];
		memset(sum_t1p, 0, sizeof(sum_t1p));
		for (int mi = 0; mi < SNOVA_m1; ++mi) {
			RCT_SCRATCH _Alignas(32) uint8_t sum_t0[SNOVA_v * SNOVA_l * SNOVA_lr32];
#ifndef RCT_RD_ACTIVE
			memset(sum_t0, 0, sizeof(sum_t0));
#endif
#ifdef RCT_RD_ACTIVE
			for (int ni = 0; ni < SNOVA_v; ++ni) {
				const gf_t *pbase = &P11[((size_t)(mi * SNOVA_v + ni) * SNOVA_v) * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_l; i1++) {
					__m256i acc = _mm256_setzero_si256();
					for (int g = 0; g < RCT_RD_VG; ++g) {
						const gf_t *p0 = pbase + (size_t)(2 * g) * SNOVA_l2 + i1 * SNOVA_l;
						const gf_t *p1 = p0 + SNOVA_l2;
						const __m256i *wd = (const __m256i *)&whipped_sig2d[(g * SNOVA_l) * 32];
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							__m256i tp = _mm256_inserti128_si256(
							                 _mm256_castsi128_si256(rct_vtl128(p0[k1])), rct_vtl128(p1[k1]), 1);
							acc = _mm256_xor_si256(acc, _mm256_shuffle_epi8(tp, wd[k1]));
						}
					}
#if (SNOVA_v & 1)
					{
						const gf_t *pt = pbase + (size_t)(SNOVA_v - 1) * SNOVA_l2 + i1 * SNOVA_l;
						const __m256i *wp = (const __m256i *)
						                    &whipped_sig2[(SNOVA_v - 1) * SNOVA_l * SNOVA_lr32];
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							acc = _mm256_xor_si256(acc, RCT_SV(RCT_BC(pt[k1]), wp[k1]));
						}
					}
#endif
					_mm256_store_si256((__m256i *)&sum_t0[(ni * SNOVA_l + i1) * SNOVA_lr32],
					                   _mm256_zextsi128_si256(_mm_xor_si128(_mm256_castsi256_si128(acc),
					                           _mm256_extracti128_si256(acc, 1))));
				}
			}
#else
			for (int ni = 0; ni < SNOVA_v; ++ni)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int k1 = 0; k1 < SNOVA_l; k1++)
							for (int b1 = 0; b1 < SNOVA_lr16; ++b1) {
								__m256i *s0 = (__m256i *)&sum_t0[(ni * SNOVA_l + i1) * SNOVA_lr32];
								__m256i q1v = RCT_BC(
								                  P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + k1]);
								__m256i *wp = (__m256i *)&whipped_sig2[nj * SNOVA_l * SNOVA_lr32 + k1 * SNOVA_lr32];
								s0[b1] = _mm256_xor_si256(s0[b1], RCT_SV(q1v, wp[b1]));
							}
#endif
			for (int i = 0; i < SNOVA_v * SNOVA_l * SNOVA_lr32; ++i) {
				sum_t0[i] = rct_gfni_cleanup(sum_t0[i]);
			}
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
			RCT_SCRATCH _Alignas(32) uint16_t sum_t0u[SNOVA_v * SNOVA_l * RCT_CMW];
#if !((SNOVA_r == SNOVA_l) && defined(RCT_SQ_CM_TIGHT) && (RCT_SQ_CM_TIGHT + 0))
			for (int i = 0; i < SNOVA_v * SNOVA_l * SNOVA_lr32; i += 32) {
				__m256i b = _mm256_load_si256((const __m256i *)&sum_t0[i]);
				_mm256_store_si256((__m256i *)&sum_t0u[i],
				                   _mm256_cvtepu8_epi16(_mm256_castsi256_si128(b)));
				_mm256_store_si256((__m256i *)&sum_t0u[i + 16],
				                   _mm256_cvtepu8_epi16(_mm256_extracti128_si256(b, 1)));
			}
#else
			for (int row = 0; row < SNOVA_v * SNOVA_l; ++row)
				_mm256_store_si256((__m256i *)&sum_t0u[row * RCT_CMW],
				                   _mm256_cvtepu8_epi16(_mm_load_si128((const __m128i *)&sum_t0[row * SNOVA_lr32])));
#endif
			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int i1 = 0; i1 < SNOVA_r; i1++) {
					__m256i acc[RCT_CMW16];
					for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
						acc[c1] = _mm256_setzero_si256();
					}
					for (int ni = 0; ni < SNOVA_v; ++ni)
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							__m256i wv = _mm256_set1_epi16((short)
							                               rct_cm_whip[(k1 * SNOVA_v + ni) * RCT_CMW + a1 * SNOVA_r + i1]);
							const __m256i *s0 = (const __m256i *)&sum_t0u[(ni * SNOVA_l + k1) * RCT_CMW];
							for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
								acc[c1] = _mm256_xor_si256(acc[c1], _mm256_mullo_epi16(wv, s0[c1]));
							}
						}
					uint8_t *dst = &sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + i1 * SNOVA_lr32];
					for (int c1 = 0; c1 < RCT_CMW16; ++c1)
						_mm_store_si128((__m128i *)(dst + c1 * 16),
						                gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[c1])));
				}
#else
			for (int ni = 0; ni < SNOVA_v; ++ni)
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_r; i1++) {
							__m256i wp = RCT_BC_SEC(
							                 whipped_sig2[ni * SNOVA_l * SNOVA_lr32 + k1 * SNOVA_lr32 + a1 * SNOVA_r + i1]);
							for (int b1 = 0; b1 < SNOVA_lr16; ++b1) {
								__m256i *s1 = (__m256i *)&sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + i1 * SNOVA_lr32];
								__m256i *s0 = (__m256i *)&sum_t0[(ni * SNOVA_l + k1) * SNOVA_lr32];
								s1[b1] = _mm256_xor_si256(s1[b1], RCT_SV(wp, s0[b1]));
							}
						}
#endif
			SNOVA_CLEAR_OBJ(sum_t0);
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
			SNOVA_CLEAR_OBJ(sum_t0u);
#endif
		}
#if !defined(RCT_CM_ACTIVE) && !defined(RCT_CML_ONLY)
		for (int i = 0; i < SNOVA_m1 * SNOVA_l * SNOVA_r * SNOVA_lr32; ++i) {
			sum_t1p[i] = rct_gfni_cleanup(sum_t1p[i]);
		}
#endif
		for (int mi = 0; mi < SNOVA_m1; ++mi)
			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1] =
							    sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + i1 * SNOVA_lr32 + b1 * SNOVA_r + j1];
		SNOVA_CLEAR_OBJ(sum_t1p);
#endif
		SNOVA_CLEAR_OBJ(whipped_sig2);
#ifdef RCT_RD_ACTIVE
		SNOVA_CLEAR_OBJ(whipped_sig2d);
#endif
	}
}
#endif

#if !RCT_Q_SIMD && RCT_USE_GFNI && (SNOVA_l == 4)
static void rct_skx_fold_F_gfni(gf_t *F21, gf_t *F12, const gf_t *T12, const gf_t *P11) {
	{
		const __m128i sA0 = _mm_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12);
		const __m128i sA1 = _mm_setr_epi8(1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13);
		const __m128i sA2 = _mm_setr_epi8(2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14);
		const __m128i sA3 = _mm_setr_epi8(3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15);
		const __m128i sB0 = _mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3);
		const __m128i sB1 = _mm_setr_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7);
		const __m128i sB2 = _mm_setr_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11);
		const __m128i sB3 = _mm_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15);
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++) {
				__m128i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm_setzero_si128();
				}
				for (int j2 = 0; j2 < SNOVA_v; j2++) {
					__m128i bv = _mm_loadu_si128((const __m128i *)&P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2]);
					__m128i bk0 = _mm_shuffle_epi8(bv, sB0), bk1 = _mm_shuffle_epi8(bv, sB1),
					        bk2 = _mm_shuffle_epi8(bv, sB2), bk3 = _mm_shuffle_epi8(bv, sB3);
					for (int k1 = 0; k1 < SNOVA_o; k1++) {
						__m128i av = _mm_loadu_si128((const __m128i *)&T12[(j2 * SNOVA_o + k1) * SNOVA_l2]);
						__m128i p = RCT_GFMUL128(_mm_shuffle_epi8(av, sA0), bk0);
						p = _mm_xor_si128(p, RCT_GFMUL128(_mm_shuffle_epi8(av, sA1), bk1));
						p = _mm_xor_si128(p, RCT_GFMUL128(_mm_shuffle_epi8(av, sA2), bk2));
						p = _mm_xor_si128(p, RCT_GFMUL128(_mm_shuffle_epi8(av, sA3), bk3));
						acc[k1] = _mm_xor_si128(acc[k1], p);
					}
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2],
					                 rct_gfni_cleanup128(acc[k1]));
			}
		rct_l4g_fold_bsec(F12, P11, T12, SNOVA_v, 0);
	}
}
#endif

#endif

// #include "platforms/x86_avx2/rct_sign_pshufb.h"
// #include "platforms/x86_avx2/rct_sign_mullo.h"
#ifndef RCT_SIGN_MULLO_H
#define RCT_SIGN_MULLO_H

#if SNOVA_Q == 16 && RCT_HOT_QRP16 && !RCT_USE_GFNI && defined(RCT_T12_MULLO) \
    && (RCT_T12_MULLO + 0) && !defined(RCT_GAUSS_SCALAR) && SNOVA_L == 4
static void rct_sign_apply_t12_mullo(rct_sign_ctx *c, const gf_t *solpad) {
	gf_t *signature_in_GF = c->signature_in_GF;
	const gf_t *T12 = c->T12;
	for (int index = 0; index < SNOVA_v; ++index) {
		gf_t *sigrow = &signature_in_GF[index * SNOVA_lr];
		for (int i1 = 0; i1 < SNOVA_l; ++i1) {
			__m256i acc = _mm256_setzero_si256();
			const gf_t *tb = &T12[(index * SNOVA_o) * SNOVA_l2 + i1 * SNOVA_l];
			for (int mi = 0; mi < SNOVA_o; ++mi, tb += SNOVA_l2)
				for (int k1 = 0; k1 < SNOVA_l; ++k1) {
					uint16_t te = (uint16_t)((tb[k1] | ((uint16_t)tb[k1] << 3)
					                          | ((uint16_t)tb[k1] << 6) | ((uint16_t)tb[k1] << 9)) & 0x1111);
					__m256i cv = _mm256_cvtepu8_epi16(_mm_loadu_si128(
					                                      (const __m128i *)&solpad[mi * SNOVA_lr + k1 * SNOVA_r]));
					acc = _mm256_xor_si256(acc,
					                       _mm256_mullo_epi16(_mm256_set1_epi16((short)te), cv));
				}
			__m128i out = gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc));
			uint8_t out16[16];
			_mm_storeu_si128((__m128i *)out16, out);
			for (int j1 = 0; j1 < SNOVA_r; ++j1) {
				sigrow[i1 * SNOVA_r + j1] ^= out16[j1];
			}
		}
	}
}
#endif

#if defined(RCT_CMS2_ONLY)
static void rct_sign_fvv_cms2(rct_sign_ctx *c) {
	const gf_t *q1 = c->q1, *q2 = c->q2, *Am = c->Am, *Bm = c->Bm, *sum_t1 = c->sum_t1;
	gf_t *Fvv_in_GF16Matrix = c->Fvv;
	for (int mi = 0; mi < SNOVA_o; ++mi) {
		__m256i Facc = _mm256_setzero_si256();
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			int mi_prime = i_prime(mi, alpha);
			__m128i t1 = _mm_setzero_si128();
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				__m128i t0 = _mm_setzero_si128();
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					t0 = _mm_xor_si128(t0, _mm_shuffle_epi8(
					                       rct_vtl128(q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
					                       _mm_loadu_si128((const __m128i *)
					                                       &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2])));
				t1 = _mm_xor_si128(t1, _mm_shuffle_epi8(
				                       rct_vtl128(q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]), t0));
			}
			__m128i Bc = _mm_loadu_si128((const __m128i *)&Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr]);
			__m256i acc = _mm256_setzero_si256();
			for (int k1 = 0; k1 < SNOVA_l; ++k1)
				acc = _mm256_xor_si256(acc, _mm256_mullo_epi16(
				                           _mm256_cvtepu8_epi16(_mm_shuffle_epi8(t1, rct_s2_pcol(k1))),
				                           gf16_expand_u16x16(_mm256_cvtepu8_epi16(_mm_shuffle_epi8(Bc, rct_s2_prow(k1))))));
			__m128i t2 = gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc));
			__m128i Ac = _mm_loadu_si128((const __m128i *)&Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2]);
			for (int k1 = 0; k1 < SNOVA_l; ++k1)
				Facc = _mm256_xor_si256(Facc, _mm256_mullo_epi16(
				                            _mm256_cvtepu8_epi16(_mm_shuffle_epi8(Ac, rct_s2_pcol(k1))),
				                            gf16_expand_u16x16(_mm256_cvtepu8_epi16(_mm_shuffle_epi8(t2, rct_s2_prow(k1))))));
		}
		_mm_storeu_si128((__m128i *)&Fvv_in_GF16Matrix[mi * SNOVA_lr],
		                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(Facc)));
	}
}
#endif

#if !RCT_Q_SIMD && !(RCT_USE_GFNI && (SNOVA_l == 4)) && RCT_USE_PSHUFB && (SNOVA_l == 4) && defined(RCT_MULLO) && (RCT_MULLO + 0)
static void rct_skx_fold_F_mullo(gf_t *F21, gf_t *F12, const gf_t *T12, const gf_t *P11) {
	{
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++) {
				__m256i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm256_setzero_si256();
				}
				for (int j2 = 0; j2 < SNOVA_v; j2++) {
					__m256i cperm[4];
					rct_gf16_cperm_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2])), cperm);
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bc(
						                               _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                                       (const __m128i *)&T12[(j2 * SNOVA_o + k1) * SNOVA_l2])), cperm));
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2],
					                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[k1])));
			}
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++) {
				__m256i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm256_setzero_si256();
				}
				for (int j2 = 0; j2 < SNOVA_v; j2++) {
					__m256i bsh[4];
					rct_gf16_bshuf_exp(_mm256_cvtepu8_epi16(_mm_loadu_si128(
					        (const __m128i *)&P11[((i1 * SNOVA_v + j1) * SNOVA_v + j2) * SNOVA_l2])), bsh);
					for (int k1 = 0; k1 < SNOVA_o; k1++)
						acc[k1] = _mm256_xor_si256(acc[k1], rct_gf16_mm4_bs(bsh,
						    _mm256_cvtepu8_epi16(_mm_loadu_si128(
						                             (const __m128i *)&T12[(j2 * SNOVA_o + k1) * SNOVA_l2]))));
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                 gf16_pack_u16_to_bytes(gf16_compress_u16x16(acc[k1])));
			}
	}
}
#endif

#if !RCT_Q_SIMD && !(RCT_USE_GFNI && (SNOVA_l == 4)) && RCT_HOT_QRP16 && (SNOVA_l == 4) && !(defined(RCT_MULLO) && (RCT_MULLO + 0))
static void rct_skx_fold_F_qrp16(gf_t *F21, gf_t *F12, const gf_t *T12, const gf_t *P11) {
	{
		const __m128i sA0 = _mm_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12);
		const __m128i sA1 = _mm_setr_epi8(1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13);
		const __m128i sA2 = _mm_setr_epi8(2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14);
		const __m128i sA3 = _mm_setr_epi8(3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15);
		const __m128i sB0 = _mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3);
		const __m128i sB1 = _mm_setr_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7);
		const __m128i sB2 = _mm_setr_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11);
		const __m128i sB3 = _mm_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15);
		const __m256i SA01 = _mm256_set_m128i(sA1, sA0), SA23 = _mm256_set_m128i(sA3, sA2);
		const __m256i SB01 = _mm256_set_m128i(sB1, sB0), SB23 = _mm256_set_m128i(sB3, sB2);
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++) {
				__m128i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm_setzero_si128();
				}
				for (int j2 = 0; j2 < SNOVA_v; j2++) {
					__m256i bv = _mm256_broadcastsi128_si256(_mm_loadu_si128(
					        (const __m128i *)&P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2]));
					__m256i B01 = _mm256_shuffle_epi8(bv, SB01), B23 = _mm256_shuffle_epi8(bv, SB23);
					for (int k1 = 0; k1 < SNOVA_o; k1++) {
						__m256i av = _mm256_broadcastsi128_si256(_mm_loadu_si128(
						        (const __m128i *)&T12[(j2 * SNOVA_o + k1) * SNOVA_l2]));
						__m256i P = _mm256_xor_si256(
						                gf16_qrp16_256_byte_mul(_mm256_shuffle_epi8(av, SA01), B01),
						                gf16_qrp16_256_byte_mul(_mm256_shuffle_epi8(av, SA23), B23));
						acc[k1] = _mm_xor_si128(acc[k1], _mm_xor_si128(
						                            _mm256_castsi256_si128(P), _mm256_extracti128_si256(P, 1)));
					}
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2],
					                 rct_gfni_cleanup128(acc[k1]));
			}
		for (int i1 = 0; i1 < SNOVA_m1; i1++)
			for (int j1 = 0; j1 < SNOVA_v; j1++) {
				__m128i acc[SNOVA_o];
				for (int k1 = 0; k1 < SNOVA_o; k1++) {
					acc[k1] = _mm_setzero_si128();
				}
				for (int j2 = 0; j2 < SNOVA_v; j2++) {
					__m256i av = _mm256_broadcastsi128_si256(_mm_loadu_si128(
					        (const __m128i *)&P11[((i1 * SNOVA_v + j1) * SNOVA_v + j2) * SNOVA_l2]));
					__m256i A01 = _mm256_shuffle_epi8(av, SA01), A23 = _mm256_shuffle_epi8(av, SA23);
					for (int k1 = 0; k1 < SNOVA_o; k1++) {
						__m256i bv = _mm256_broadcastsi128_si256(_mm_loadu_si128(
						        (const __m128i *)&T12[(j2 * SNOVA_o + k1) * SNOVA_l2]));
						__m256i P = _mm256_xor_si256(
						                gf16_qrp16_256_byte_mul(A01, _mm256_shuffle_epi8(bv, SB01)),
						                gf16_qrp16_256_byte_mul(A23, _mm256_shuffle_epi8(bv, SB23)));
						acc[k1] = _mm_xor_si128(acc[k1], _mm_xor_si128(
						                            _mm256_castsi256_si128(P), _mm256_extracti128_si256(P, 1)));
					}
				}
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					_mm_storeu_si128((__m128i *)&F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                 rct_gfni_cleanup128(acc[k1]));
			}
	}
}
#endif

#endif

// #include "platforms/x86_avx2/rct_sign_cm.h"
#ifndef RCT_SIGN_CM_H
#define RCT_SIGN_CM_H

#if defined(RCT_CM_ACTIVE) || defined(RCT_CMS3_ONLY)
_Static_assert(SNOVA_R >= SNOVA_L, "rct_sign_cm: requires r >= l (Bm scatter grid)");
static void rct_sign_wF_gauss_cm(rct_sign_ctx *c) {
	const gf_t *F21 = c->F21, *F12 = c->F12;
	const gf_t *q1 = c->q1, *q2 = c->q2, *Am = c->Am, *Bm = c->Bm, *Q1 = c->Q1, *Q2 = c->Q2;
	gf_t (*gauss)[SNOVA_o * SNOVA_lr + 1 + 64] = c->gauss;
	uint16_t *rct_cm_whip = c->cm->cm_whip;
#if RCT_USE_GFNI
	uint8_t *rct_cm_whipb = c->cm->cm_whipb;
	(void)rct_cm_whipb;
#endif
	const uint16_t *rct_cm_Amx = c->cm->Amx, *rct_cm_Bmx = c->cm->Bmx,
	                *rct_cm_Q1x = c->cm->Q1x, *rct_cm_Q2x = c->cm->Q2x,
	                 *rct_cm_q1x = c->cm->q1x, *rct_cm_q2x = c->cm->q2x;
	(void)F21;
	(void)F12;
	(void)q1;
	(void)q2;
	(void)Am;
	(void)Bm;
	(void)Q1;
	(void)Q2;
	(void)rct_cm_whip;
	(void)rct_cm_Amx;
	(void)rct_cm_Bmx;
	(void)rct_cm_Q1x;
	(void)rct_cm_Q2x;
	(void)rct_cm_q1x;
	(void)rct_cm_q2x;
	{
		RCT_SCRATCH _Alignas(32) uint16_t wF21w[SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr32];
		RCT_SCRATCH _Alignas(32) uint16_t wF12w[SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr32];
		for (int mi = 0; mi < SNOVA_m1; ++mi)
			for (int idx = 0; idx < SNOVA_o; ++idx)
				for (int i1 = 0; i1 < SNOVA_l; ++i1) {
#if RCT_USE_GFNI
					__m128i a21[RCT_CMW16], a12[RCT_CMW16];
					for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
						a21[c1] = _mm_setzero_si128();
						a12[c1] = _mm_setzero_si128();
					}
					for (int nj = 0; nj < SNOVA_v; ++nj)
						for (int k1 = 0; k1 < SNOVA_l; ++k1) {
							const uint8_t *wb = &rct_cm_whipb[(k1 * SNOVA_v + nj) * RCT_CMW];
							__m128i f21 = _mm_set1_epi8((char)
							                            F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + k1]);
							__m128i f12 = _mm_set1_epi8((char)
							                            F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + k1 * SNOVA_l + i1]);
							for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
								__m128i wv = _mm_loadu_si128((const __m128i *)(wb + c1 * 16));
								a21[c1] = _mm_xor_si128(a21[c1], RCT_GFMUL128(f21, wv));
								a12[c1] = _mm_xor_si128(a12[c1], RCT_GFMUL128(f12, wv));
							}
						}
					uint16_t *d21 = &wF21w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32];
					uint16_t *d12 = &wF12w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32];
					for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
						_mm256_store_si256((__m256i *)(d21 + c1 * 16),
						                   _mm256_cvtepu8_epi16(rct_gfni_cleanup128(a21[c1])));
						_mm256_store_si256((__m256i *)(d12 + c1 * 16),
						                   _mm256_cvtepu8_epi16(rct_gfni_cleanup128(a12[c1])));
					}
#else
					__m256i a21[RCT_CMW16], a12[RCT_CMW16];
					for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
						a21[c1] = _mm256_setzero_si256();
						a12[c1] = _mm256_setzero_si256();
					}
					for (int nj = 0; nj < SNOVA_v; ++nj)
						for (int k1 = 0; k1 < SNOVA_l; ++k1) {
							const __m256i *w = (const __m256i *)&rct_cm_whip[(k1 * SNOVA_v + nj) * RCT_CMW];
							__m256i f21 = _mm256_set1_epi16((short)
							                                F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + k1]);
							__m256i f12 = _mm256_set1_epi16((short)
							                                F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + k1 * SNOVA_l + i1]);
							for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
								a21[c1] = _mm256_xor_si256(a21[c1], _mm256_mullo_epi16(f21, w[c1]));
								a12[c1] = _mm256_xor_si256(a12[c1], _mm256_mullo_epi16(f12, w[c1]));
							}
						}
					uint16_t *d21 = &wF21w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32];
					uint16_t *d12 = &wF12w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32];
					for (int c1 = 0; c1 < RCT_CMW16; ++c1) {
						_mm256_store_si256((__m256i *)(d21 + c1 * 16), gf16_compress_u16x16(a21[c1]));
						_mm256_store_si256((__m256i *)(d12 + c1 * 16), gf16_compress_u16x16(a12[c1]));
					}
#endif
				}
#if RCT_USE_GFNI
		RCT_SCRATCH _Alignas(32) uint8_t wF21sb[SNOVA_m1 * SNOVA_l * RCT_CM_OLR32];
		RCT_SCRATCH _Alignas(32) uint8_t wF12sb[SNOVA_m1 * SNOVA_l * RCT_CM_OLR32];
		memset(wF21sb, 0, sizeof(wF21sb));
		memset(wF12sb, 0, sizeof(wF12sb));
		for (int mi = 0; mi < SNOVA_m1; ++mi) {
			for (int i1 = 0; i1 < SNOVA_l; ++i1)
				for (int idx = 0; idx < SNOVA_o; ++idx)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int j1 = 0; j1 < SNOVA_r; ++j1) {
							wF21sb[(mi * SNOVA_l + b1) * RCT_CM_OLR32 + idx * SNOVA_lr + i1 * SNOVA_r + j1] =
							    (uint8_t)wF21w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32 + b1 * SNOVA_r + j1];
							wF12sb[(mi * SNOVA_l + b1) * RCT_CM_OLR32 + idx * SNOVA_lr + i1 * SNOVA_r + j1] =
							    (uint8_t)wF12w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32 + b1 * SNOVA_r + j1];
						}
		}
		RCT_SCRATCH _Alignas(32) uint8_t gausstmp1b[SNOVA_o * SNOVA_r2 * RCT_CM_OLR32];
		RCT_SCRATCH _Alignas(32) uint8_t gausstmp2b[SNOVA_o * SNOVA_r2 * RCT_CM_OLR32];
		memset(gausstmp1b, 0, sizeof(gausstmp1b));
		memset(gausstmp2b, 0, sizeof(gausstmp2b));
#if (SNOVA_l == 4) && (SNOVA_r == 4)
		const __m256i RCT_SAY[4] = {
			_mm256_broadcastsi128_si256(_mm_setr_epi8(0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12)),
			_mm256_broadcastsi128_si256(_mm_setr_epi8(1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13)),
			_mm256_broadcastsi128_si256(_mm_setr_epi8(2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14)),
			_mm256_broadcastsi128_si256(_mm_setr_epi8(3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15)),
		};
		const __m256i RCT_SBY[4] = {
			_mm256_broadcastsi128_si256(_mm_setr_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3)),
			_mm256_broadcastsi128_si256(_mm_setr_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7)),
			_mm256_broadcastsi128_si256(_mm_setr_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11)),
			_mm256_broadcastsi128_si256(_mm_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15)),
		};
		const __m256i RCT_STY = _mm256_broadcastsi128_si256(
		                            _mm_setr_epi8(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15));
#endif
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
				int mp = i_prime(mi, alpha);
				const gf_t *q1raw = &q1[(mi * SNOVA_alpha + alpha) * SNOVA_l];
				const gf_t *q2raw = &q2[(mi * SNOVA_alpha + alpha) * SNOVA_l];
				const gf_t *Amraw = &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2];
				const gf_t *Bmraw = &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr];
#if (SNOVA_l == 4) && (SNOVA_r == 4)
				const __m256i Bm256 = _mm256_broadcastsi128_si256(_mm_loadu_si128((const __m128i *)Bmraw));
				const __m256i Am256 = _mm256_broadcastsi128_si256(_mm_loadu_si128((const __m128i *)Amraw));
				const __m256i Q1_256 = _mm256_broadcastsi128_si256(
				                           _mm_loadu_si128((const __m128i *)&Q1[(mi * SNOVA_alpha + alpha) * SNOVA_l2]));
				const __m256i Q2_256 = _mm256_broadcastsi128_si256(
				                           _mm_loadu_si128((const __m128i *)&Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2]));
#else
				const uint16_t *Amx = &rct_cm_Amx[(mi * SNOVA_alpha + alpha) * SNOVA_r2];
				const uint16_t *Bmx = &rct_cm_Bmx[(mi * SNOVA_alpha + alpha) * SNOVA_lr];
				const uint16_t *Q1x = &rct_cm_Q1x[(mi * SNOVA_alpha + alpha) * SNOVA_l2];
				const uint16_t *Q2x = &rct_cm_Q2x[(mi * SNOVA_alpha + alpha) * SNOVA_l2];
#endif
				{
#if (SNOVA_l == 4) && (SNOVA_r == 4)
					_Alignas(32) uint8_t t2b[RCT_CM_OLR32];
					{
						__m256i acc[RCT_CM_OLR32N];
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							acc[c1] = _mm256_setzero_si256();
						}
						for (int b1 = 0; b1 < SNOVA_l; ++b1) {
							__m256i qv = _mm256_set1_epi8((char)q2raw[b1]);
							const uint8_t *wf = &wF21sb[(mp * SNOVA_l + b1) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								acc[c1] = _mm256_xor_si256(acc[c1], RCT_GFMUL256(qv,
								    _mm256_loadu_si256((const __m256i *)(wf + c1 * 32))));
						}
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							__m256i T0 = rct_gfni_cleanup256(acc[c1]);
							__m256i t1c = rct_cm_cellmm256(T0, Bm256, RCT_SAY, RCT_SBY);
							__m256i t2c = rct_cm_cellmm256(Q1_256, t1c, RCT_SAY, RCT_SBY);
							_mm256_store_si256((__m256i *)&t2b[c1 * 32], t2c);
						}
					}
					for (int ti1 = 0; ti1 < SNOVA_r; ++ti1)
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							__m256i av = _mm256_set1_epi8((char)Amraw[ti1 * SNOVA_r + tj2]);
							uint8_t *g = &gausstmp1b[((mi * SNOVA_r + ti1) * SNOVA_r + tj2) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								_mm256_storeu_si256((__m256i *)(g + c1 * 32),
								                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)(g + c1 * 32)),
								                                     RCT_GFMUL256(av, _mm256_loadu_si256((const __m256i *)(t2b + c1 * 32)))));
						}
#else
					_Alignas(32) uint16_t t0[RCT_CM_OLR32];
					{
						__m256i acc[RCT_CM_OLR32N];
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							acc[c1] = _mm256_setzero_si256();
						}
						for (int b1 = 0; b1 < SNOVA_l; ++b1) {
							__m256i qv = _mm256_set1_epi8((char)q2raw[b1]);
							const uint8_t *wf = &wF21sb[(mp * SNOVA_l + b1) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								acc[c1] = _mm256_xor_si256(acc[c1], RCT_GFMUL256(qv,
								    _mm256_loadu_si256((const __m256i *)(wf + c1 * 32))));
						}
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							__m256i r = rct_gfni_cleanup256(acc[c1]);
							_mm256_store_si256((__m256i *)&t0[c1 * 32],
							                   _mm256_cvtepu8_epi16(_mm256_castsi256_si128(r)));
							_mm256_store_si256((__m256i *)&t0[c1 * 32 + 16],
							                   _mm256_cvtepu8_epi16(_mm256_extracti128_si256(r, 1)));
						}
					}
					uint16_t t1[SNOVA_o * SNOVA_l2] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_l; ++i1)
							for (int k1 = 0; k1 < SNOVA_r; ++k1) {
								uint16_t tv = t0[idx * SNOVA_lr + i1 * SNOVA_r + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t1[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(tv * Bmx[k1 * SNOVA_l + j1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_l2; ++i) {
						t1[i] = rct_cm_cmp(t1[i]);
					}
					_Alignas(32) uint16_t t2[RCT_CM_OLR32] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_l; ++i1)
							for (int k1 = 0; k1 < SNOVA_l; ++k1) {
								uint16_t qv = Q1x[i1 * SNOVA_l + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t2[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(qv * t1[idx * SNOVA_l2 + k1 * SNOVA_l + j1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_l2; ++i) {
						t2[i] = rct_cm_cmp(t2[i]);
					}
					_Alignas(32) uint8_t t2b[RCT_CM_OLR32];
					for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
						_mm_store_si128((__m128i *)&t2b[c1 * 32],
						                gf16_pack_u16_to_bytes(_mm256_load_si256((const __m256i *)&t2[c1 * 32])));
						_mm_store_si128((__m128i *)&t2b[c1 * 32 + 16],
						                gf16_pack_u16_to_bytes(_mm256_load_si256((const __m256i *)&t2[c1 * 32 + 16])));
					}
					for (int ti1 = 0; ti1 < SNOVA_r; ++ti1)
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							__m256i av = _mm256_set1_epi8((char)Amraw[ti1 * SNOVA_r + tj2]);
							uint8_t *g = &gausstmp1b[((mi * SNOVA_r + ti1) * SNOVA_r + tj2) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								_mm256_storeu_si256((__m256i *)(g + c1 * 32),
								                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)(g + c1 * 32)),
								                                     RCT_GFMUL256(av, _mm256_loadu_si256((const __m256i *)(t2b + c1 * 32)))));
						}
#endif
				}
				{
#if (SNOVA_l == 4) && (SNOVA_r == 4)
					_Alignas(32) uint8_t t2b[RCT_CM_OLR32];
					{
						__m256i acc[RCT_CM_OLR32N];
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							acc[c1] = _mm256_setzero_si256();
						}
						for (int b1 = 0; b1 < SNOVA_l; ++b1) {
							__m256i qv = _mm256_set1_epi8((char)q1raw[b1]);
							const uint8_t *wf = &wF12sb[(mp * SNOVA_l + b1) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								acc[c1] = _mm256_xor_si256(acc[c1], RCT_GFMUL256(qv,
								    _mm256_loadu_si256((const __m256i *)(wf + c1 * 32))));
						}
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							__m256i T0 = rct_gfni_cleanup256(acc[c1]);
							__m256i T0t = _mm256_shuffle_epi8(T0, RCT_STY);
							__m256i t1c = rct_cm_cellmm256(Am256, T0t, RCT_SAY, RCT_SBY);
							__m256i t2c = rct_cm_cellmm256(t1c, Q2_256, RCT_SAY, RCT_SBY);
							_mm256_store_si256((__m256i *)&t2b[c1 * 32], t2c);
						}
					}
					for (int ti2 = 0; ti2 < SNOVA_l; ++ti2)
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							__m256i bv = _mm256_set1_epi8((char)Bmraw[tj2 * SNOVA_l + ti2]);
							uint8_t *g = &gausstmp2b[((mi * SNOVA_r + ti2) * SNOVA_r + tj2) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								_mm256_storeu_si256((__m256i *)(g + c1 * 32),
								                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)(g + c1 * 32)),
								                                     RCT_GFMUL256(bv, _mm256_loadu_si256((const __m256i *)(t2b + c1 * 32)))));
						}
#else
					_Alignas(32) uint16_t t0[RCT_CM_OLR32];
					{
						__m256i acc[RCT_CM_OLR32N];
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							acc[c1] = _mm256_setzero_si256();
						}
						for (int b1 = 0; b1 < SNOVA_l; ++b1) {
							__m256i qv = _mm256_set1_epi8((char)q1raw[b1]);
							const uint8_t *wf = &wF12sb[(mp * SNOVA_l + b1) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								acc[c1] = _mm256_xor_si256(acc[c1], RCT_GFMUL256(qv,
								    _mm256_loadu_si256((const __m256i *)(wf + c1 * 32))));
						}
						for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
							__m256i r = rct_gfni_cleanup256(acc[c1]);
							_mm256_store_si256((__m256i *)&t0[c1 * 32],
							                   _mm256_cvtepu8_epi16(_mm256_castsi256_si128(r)));
							_mm256_store_si256((__m256i *)&t0[c1 * 32 + 16],
							                   _mm256_cvtepu8_epi16(_mm256_extracti128_si256(r, 1)));
						}
					}
					_Alignas(32) uint16_t t1[RCT_CM_OLR32] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_r; ++i1)
							for (int k1 = 0; k1 < SNOVA_r; ++k1) {
								uint16_t av = Amx[i1 * SNOVA_r + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t1[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(av * t0[idx * SNOVA_lr + j1 * SNOVA_r + k1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_lr; ++i) {
						t1[i] = rct_cm_cmp(t1[i]);
					}
					_Alignas(32) uint16_t t2[RCT_CM_OLR32] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_r; ++i1)
							for (int k1 = 0; k1 < SNOVA_l; ++k1) {
								uint16_t tv = t1[idx * SNOVA_lr + i1 * SNOVA_l + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t2[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(tv * Q2x[k1 * SNOVA_l + j1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_lr; ++i) {
						t2[i] = rct_cm_cmp(t2[i]);
					}
					_Alignas(32) uint8_t t2b[RCT_CM_OLR32];
					for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1) {
						_mm_store_si128((__m128i *)&t2b[c1 * 32],
						                gf16_pack_u16_to_bytes(_mm256_load_si256((const __m256i *)&t2[c1 * 32])));
						_mm_store_si128((__m128i *)&t2b[c1 * 32 + 16],
						                gf16_pack_u16_to_bytes(_mm256_load_si256((const __m256i *)&t2[c1 * 32 + 16])));
					}
					for (int ti2 = 0; ti2 < SNOVA_l; ++ti2)
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							__m256i bv = _mm256_set1_epi8((char)Bmraw[tj2 * SNOVA_l + ti2]);
							uint8_t *g = &gausstmp2b[((mi * SNOVA_r + ti2) * SNOVA_r + tj2) * RCT_CM_OLR32];
							for (int c1 = 0; c1 < RCT_CM_OLR32N; ++c1)
								_mm256_storeu_si256((__m256i *)(g + c1 * 32),
								                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)(g + c1 * 32)),
								                                     RCT_GFMUL256(bv, _mm256_loadu_si256((const __m256i *)(t2b + c1 * 32)))));
						}
#endif
				}
			}
		for (int i = 0; i < SNOVA_o * SNOVA_r2 * RCT_CM_OLR32; i += 32) {
			_mm256_store_si256((__m256i *)&gausstmp1b[i],
			                   rct_gfni_cleanup256(_mm256_load_si256((const __m256i *)&gausstmp1b[i])));
			_mm256_store_si256((__m256i *)&gausstmp2b[i],
			                   rct_gfni_cleanup256(_mm256_load_si256((const __m256i *)&gausstmp2b[i])));
		}
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int ti1 = 0; ti1 < SNOVA_r; ++ti1)
				for (int ti2 = 0; ti2 < SNOVA_l; ++ti2) {
					gf_t *grow = gauss[mi * SNOVA_lr + ti1 * SNOVA_l + ti2];
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int tj1 = 0; tj1 < SNOVA_l; ++tj1)
							for (int tj2 = 0; tj2 < SNOVA_r; ++tj2)
								grow[idx * SNOVA_lr + tj1 * SNOVA_r + tj2] ^= (gf_t)(
								        gausstmp1b[((mi * SNOVA_r + ti1) * SNOVA_r + tj2) * RCT_CM_OLR32 +
								                   idx * SNOVA_l2 + tj1 * SNOVA_l + ti2] ^
								        gausstmp2b[((mi * SNOVA_r + ti2) * SNOVA_r + tj2) * RCT_CM_OLR32 +
								                   idx * SNOVA_lr + ti1 * SNOVA_l + tj1]);
				}
		SNOVA_CLEAR_OBJ(wF21sb);
		SNOVA_CLEAR_OBJ(wF12sb);
		SNOVA_CLEAR_OBJ(gausstmp1b);
		SNOVA_CLEAR_OBJ(gausstmp2b);
#else
		RCT_SCRATCH _Alignas(32) uint16_t wF21s[SNOVA_m1 * SNOVA_l * RCT_CM_OLR];
		RCT_SCRATCH _Alignas(32) uint16_t wF12s[SNOVA_m1 * SNOVA_l * RCT_CM_OLR];
		memset(wF21s, 0, sizeof(wF21s));
		memset(wF12s, 0, sizeof(wF12s));
		for (int mi = 0; mi < SNOVA_m1; ++mi)
			for (int i1 = 0; i1 < SNOVA_l; ++i1)
				for (int idx = 0; idx < SNOVA_o; ++idx) {
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int j1 = 0; j1 < SNOVA_r; ++j1) {
							wF21s[(mi * SNOVA_l + b1) * RCT_CM_OLR + idx * SNOVA_lr + i1 * SNOVA_r + j1] =
							    wF21w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32 + b1 * SNOVA_r + j1];
							wF12s[(mi * SNOVA_l + b1) * RCT_CM_OLR + idx * SNOVA_lr + i1 * SNOVA_r + j1] =
							    wF12w[((mi * SNOVA_l + i1) * SNOVA_o + idx) * SNOVA_lr32 + b1 * SNOVA_r + j1];
						}
				}
		RCT_SCRATCH _Alignas(32) uint16_t gausstmp1[SNOVA_o * SNOVA_r2 * RCT_CM_OLR];
		RCT_SCRATCH _Alignas(32) uint16_t gausstmp2[SNOVA_o * SNOVA_r2 * RCT_CM_OLR];
		memset(gausstmp1, 0, sizeof(gausstmp1));
		memset(gausstmp2, 0, sizeof(gausstmp2));
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
				int mp = i_prime(mi, alpha);
				const uint16_t *q1r = &rct_cm_q1x[(mi * SNOVA_alpha + alpha) * SNOVA_l];
				const uint16_t *q2r = &rct_cm_q2x[(mi * SNOVA_alpha + alpha) * SNOVA_l];
				const uint16_t *Amr = &rct_cm_Amx[(mi * SNOVA_alpha + alpha) * SNOVA_r2];
				const uint16_t *Bmr = &rct_cm_Bmx[(mi * SNOVA_alpha + alpha) * SNOVA_lr];
				const uint16_t *Q1r = &rct_cm_Q1x[(mi * SNOVA_alpha + alpha) * SNOVA_l2];
				const uint16_t *Q2r = &rct_cm_Q2x[(mi * SNOVA_alpha + alpha) * SNOVA_l2];
				{
					_Alignas(32) uint16_t t0[RCT_CM_OLR];
					{
						__m256i acc[RCT_CM_OLR16];
						for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
							acc[c1] = _mm256_setzero_si256();
						}
						for (int b1 = 0; b1 < SNOVA_l; ++b1) {
							__m256i qv = _mm256_set1_epi16((short)q2r[b1]);
							const __m256i *wf = (const __m256i *)&wF21s[(mp * SNOVA_l + b1) * RCT_CM_OLR];
							for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
								acc[c1] = _mm256_xor_si256(acc[c1], _mm256_mullo_epi16(qv, wf[c1]));
							}
						}
						for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
							_mm256_store_si256((__m256i *)&t0[c1 * 16], gf16_compress_u16x16(acc[c1]));
						}
					}
					uint16_t t1[SNOVA_o * SNOVA_l2] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_l; ++i1)
							for (int k1 = 0; k1 < SNOVA_r; ++k1) {
								uint16_t tv = t0[idx * SNOVA_lr + i1 * SNOVA_r + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t1[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(tv * Bmr[k1 * SNOVA_l + j1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_l2; ++i) {
						t1[i] = rct_cm_cmp(t1[i]);
					}
					_Alignas(32) uint16_t t2[RCT_CM_OLR] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_l; ++i1)
							for (int k1 = 0; k1 < SNOVA_l; ++k1) {
								uint16_t qv = Q1r[i1 * SNOVA_l + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t2[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(qv * t1[idx * SNOVA_l2 + k1 * SNOVA_l + j1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_l2; ++i) {
						t2[i] = rct_cm_cmp(t2[i]);
					}
					for (int ti1 = 0; ti1 < SNOVA_r; ++ti1)
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							__m256i av = _mm256_set1_epi16((short)Amr[ti1 * SNOVA_r + tj2]);
							__m256i *g = (__m256i *)&gausstmp1[((mi * SNOVA_r + ti1) * SNOVA_r + tj2) * RCT_CM_OLR];
							const __m256i *tv = (const __m256i *)t2;
							for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
								g[c1] = _mm256_xor_si256(g[c1], _mm256_mullo_epi16(av, tv[c1]));
							}
						}
				}
				{
					_Alignas(32) uint16_t t0[RCT_CM_OLR];
					{
						__m256i acc[RCT_CM_OLR16];
						for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
							acc[c1] = _mm256_setzero_si256();
						}
						for (int b1 = 0; b1 < SNOVA_l; ++b1) {
							__m256i qv = _mm256_set1_epi16((short)q1r[b1]);
							const __m256i *wf = (const __m256i *)&wF12s[(mp * SNOVA_l + b1) * RCT_CM_OLR];
							for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
								acc[c1] = _mm256_xor_si256(acc[c1], _mm256_mullo_epi16(qv, wf[c1]));
							}
						}
						for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
							_mm256_store_si256((__m256i *)&t0[c1 * 16], gf16_compress_u16x16(acc[c1]));
						}
					}
					_Alignas(32) uint16_t t1[RCT_CM_OLR] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_r; ++i1)
							for (int k1 = 0; k1 < SNOVA_r; ++k1) {
								uint16_t av = Amr[i1 * SNOVA_r + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t1[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(av * t0[idx * SNOVA_lr + j1 * SNOVA_r + k1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_lr; ++i) {
						t1[i] = rct_cm_cmp(t1[i]);
					}
					_Alignas(32) uint16_t t2[RCT_CM_OLR] = {0};
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int i1 = 0; i1 < SNOVA_r; ++i1)
							for (int k1 = 0; k1 < SNOVA_l; ++k1) {
								uint16_t tv = t1[idx * SNOVA_lr + i1 * SNOVA_l + k1];
								for (int j1 = 0; j1 < SNOVA_l; ++j1)
									t2[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
									    (uint16_t)(tv * Q2r[k1 * SNOVA_l + j1]);
							}
					for (int i = 0; i < SNOVA_o * SNOVA_lr; ++i) {
						t2[i] = rct_cm_cmp(t2[i]);
					}
					for (int ti2 = 0; ti2 < SNOVA_l; ++ti2)
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							__m256i bv = _mm256_set1_epi16((short)Bmr[tj2 * SNOVA_l + ti2]);
							__m256i *g = (__m256i *)&gausstmp2[((mi * SNOVA_r + ti2) * SNOVA_r + tj2) * RCT_CM_OLR];
							const __m256i *tv = (const __m256i *)t2;
							for (int c1 = 0; c1 < RCT_CM_OLR16; ++c1) {
								g[c1] = _mm256_xor_si256(g[c1], _mm256_mullo_epi16(bv, tv[c1]));
							}
						}
				}
			}
		for (int i = 0; i < SNOVA_o * SNOVA_r2 * RCT_CM_OLR; i += 16) {
			_mm256_store_si256((__m256i *)&gausstmp1[i],
			                   gf16_compress_u16x16(_mm256_load_si256((const __m256i *)&gausstmp1[i])));
			_mm256_store_si256((__m256i *)&gausstmp2[i],
			                   gf16_compress_u16x16(_mm256_load_si256((const __m256i *)&gausstmp2[i])));
		}
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int ti1 = 0; ti1 < SNOVA_r; ++ti1)
				for (int ti2 = 0; ti2 < SNOVA_l; ++ti2) {
					gf_t *grow = gauss[mi * SNOVA_lr + ti1 * SNOVA_l + ti2];
					for (int idx = 0; idx < SNOVA_o; ++idx)
						for (int tj1 = 0; tj1 < SNOVA_l; ++tj1)
							for (int tj2 = 0; tj2 < SNOVA_r; ++tj2)
								grow[idx * SNOVA_lr + tj1 * SNOVA_r + tj2] ^= (gf_t)(
								        gausstmp1[((mi * SNOVA_r + ti1) * SNOVA_r + tj2) * RCT_CM_OLR +
								                  idx * SNOVA_l2 + tj1 * SNOVA_l + ti2] ^
								        gausstmp2[((mi * SNOVA_r + ti2) * SNOVA_r + tj2) * RCT_CM_OLR +
								                  idx * SNOVA_lr + ti1 * SNOVA_l + tj1]);
				}
		SNOVA_CLEAR_OBJ(wF21s);
		SNOVA_CLEAR_OBJ(wF12s);
		SNOVA_CLEAR_OBJ(gausstmp1);
		SNOVA_CLEAR_OBJ(gausstmp2);
#endif
		SNOVA_CLEAR_OBJ(wF21w);
		SNOVA_CLEAR_OBJ(wF12w);
	}
}
#endif

#endif

// #include "platforms/x86_avx2/rct_sign_oddq.h"
// #include "platforms/x86_avx2/rct_sign_jog.h"
#ifndef RCT_SIGN_JOG_H
#define RCT_SIGN_JOG_H

#if RCT_SIGN_JOG

#ifndef RCT_SIGN_P2_B1LANE
#define RCT_SIGN_P2_B1LANE 1
#endif
#if RCT_SIGN_P2_B1LANE && (RCT_SJ_M4 || (RCT_SJ_A4 && SNOVA_r != 8))
#define RCT_SJ_B1L 1
#else
#define RCT_SJ_B1L 0
#endif
#if RCT_SJ_M4 && RCT_SJ_B1L
enum { RCT_B1L_WN = SNOVA_l * SNOVA_v * SNOVA_lr,
       RCT_B1L_WCH = (RCT_B1L_WN + 15) / 16 * 16,
       RCT_B1L_FN = SNOVA_m1 * SNOVA_o * SNOVA_v * SNOVA_l2,
       RCT_B1L_FCH = (RCT_B1L_FN + 15) / 16 * 16
     };
#endif

static void rct_sign_whipbuild_jog(rct_sign_ctx *c) {
	rct_sj_ensure();
	const gf_t *sig = c->signature_in_GF;
	gf_t *whipped_sig = c->whipped_sig;
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				__m128i acc = _mm_setzero_si128();
				const gf_t *Srow = &rct_S[ab * SNOVA_l2 + i1 * SNOVA_l];
				for (int k1 = 0; k1 < SNOVA_l; ++k1)
					acc = _mm_xor_si128(acc, rct_sj_sv128(rct_sj_bc128_pub(Srow[k1]),
					                                      _mm_loadu_si128((const __m128i *)&sig[ni * SNOVA_lr + k1 * SNOVA_r])));
#if RCT_SJ_B1L
				rct_sj_store_r(&whipped_sig[(ni * SNOVA_l + i1) * SNOVA_lr + ab * SNOVA_r], acc);
#else
				rct_sj_store_r(&whipped_sig[(ab * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r], acc);
#endif
			}
}

static void rct_sign_sumt_jog(rct_sign_ctx *c) {
	rct_sj_ensure();
	const gf_t *P11 = c->P11, *whipped_sig = c->whipped_sig;
	gf_t *sum_t1 = c->sum_t1;
	RCT_SCRATCH _Alignas(32) gf_t sum_t0[SNOVA_m1 * SNOVA_l * SNOVA_v * SNOVA_lr + 16];
#if (RCT_SJ_A4 || RCT_SJ_M4) && !RCT_SJ_B1L
	RCT_SCRATCH _Alignas(32) gf_t rct_a4_wsigT[SNOVA_l * SNOVA_v * SNOVA_lr + 16];
#endif
	memset(sum_t0 + SNOVA_m1 * SNOVA_l * SNOVA_v * SNOVA_lr, 0, 16);
#if (RCT_SJ_A4 || RCT_SJ_M4) && !RCT_SJ_B1L
	memset(rct_a4_wsigT + SNOVA_l * SNOVA_v * SNOVA_lr, 0, 16);
#endif
#if RCT_SJ_B1L
#if RCT_SJ_M4
	RCT_SCRATCH _Alignas(32) uint16_t whipe16[RCT_B1L_WCH + 16];
	RCT_SCRATCH _Alignas(32) uint16_t st0r16[RCT_B1L_WCH + 16];
	memset(&whipe16[RCT_B1L_WCH], 0, 16 * sizeof(uint16_t));
	memset(&st0r16[RCT_B1L_WCH], 0, 16 * sizeof(uint16_t));
	rct_m4_expand_buf(whipe16, whipped_sig, RCT_B1L_WN);
#endif
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				const gf_t *pr = &P11[((mi * SNOVA_v + ni) * SNOVA_v) * SNOVA_l2 + i1 * SNOVA_l];
#if SNOVA_r == 8
				__m256i ae = _mm256_setzero_si256(), ao = _mm256_setzero_si256();
				__m128i te = _mm_setzero_si128(), to = _mm_setzero_si128();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const gf_t *wb = &whipped_sig[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m128i tb = rct_sj_bc128_pub(pr[nj * SNOVA_l2 + k1]);
						__m256i pd = rct_sj_sv256(_mm256_broadcastsi128_si256(tb),
						                          _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						__m128i pt = rct_sj_sv128(tb,
						                          _mm_loadu_si128((const __m128i *)&wb[k1 * SNOVA_lr + 32]));
						if (k1 & 1) {
							ao = _mm256_xor_si256(ao, pd);
							to = _mm_xor_si128(to, pt);
						} else        {
							ae = _mm256_xor_si256(ae, pd);
							te = _mm_xor_si128(te, pt);
						}
					}
				}
				_Alignas(32) uint8_t tp[48];
				_mm256_store_si256((__m256i *)tp, RCT_SJ_CLEAN256(_mm256_xor_si256(ae, ao)));
				_mm_store_si128((__m128i *)&tp[32], RCT_SJ_CLEAN(_mm_xor_si128(te, to)));
				memcpy(&sum_t0[((mi * SNOVA_v + ni) * SNOVA_l + i1) * SNOVA_lr], tp, SNOVA_lr);
#else
				__m256i ae = _mm256_setzero_si256(), ao = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const gf_t *wb = &whipped_sig[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i pd = rct_sj_sv256(rct_sj_bc256_pub(pr[nj * SNOVA_l2 + k1]),
						                          _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						if (k1 & 1) {
							ao = _mm256_xor_si256(ao, pd);
						} else {
							ae = _mm256_xor_si256(ae, pd);
						}
					}
				}
				_Alignas(32) uint8_t tp[32];
				_mm256_store_si256((__m256i *)tp, RCT_SJ_CLEAN256(_mm256_xor_si256(ae, ao)));
				memcpy(&sum_t0[((mi * SNOVA_v + ni) * SNOVA_l + i1) * SNOVA_lr], tp, SNOVA_lr);
#endif
			}
#if RCT_SJ_M4
		rct_m4_raw_buf(st0r16, &sum_t0[mi * SNOVA_v * SNOVA_l * SNOVA_lr], RCT_B1L_WN);
#endif
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int i1 = 0; i1 < SNOVA_r; ++i1) {
#if RCT_SJ_M4
#if SNOVA_r == 8
				__m256i ae0 = _mm256_setzero_si256(), ae1 = _mm256_setzero_si256(), ae2 = _mm256_setzero_si256();
				__m256i ao0 = _mm256_setzero_si256(), ao1 = _mm256_setzero_si256(), ao2 = _mm256_setzero_si256();
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i s = _mm256_set1_epi16((short)whipe16[(ni * SNOVA_l + k1) * SNOVA_lr + a1 * SNOVA_r + i1]);
						const uint16_t *sb = &st0r16[(ni * SNOVA_l + k1) * SNOVA_lr];
						__m256i p0 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)sb));
						__m256i p1 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&sb[16]));
						__m256i p2 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&sb[32]));
						if (k1 & 1) {
							ao0 = _mm256_xor_si256(ao0, p0);
							ao1 = _mm256_xor_si256(ao1, p1);
							ao2 = _mm256_xor_si256(ao2, p2);
						} else        {
							ae0 = _mm256_xor_si256(ae0, p0);
							ae1 = _mm256_xor_si256(ae1, p1);
							ae2 = _mm256_xor_si256(ae2, p2);
						}
					}
				_Alignas(32) uint8_t tp[48];
				_mm_store_si128((__m128i *)tp,
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae0, ao0))));
				_mm_store_si128((__m128i *)&tp[16],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae1, ao1))));
				_mm_store_si128((__m128i *)&tp[32],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae2, ao2))));
#else
				__m256i ae0 = _mm256_setzero_si256(), ae1 = _mm256_setzero_si256();
				__m256i ao0 = _mm256_setzero_si256(), ao1 = _mm256_setzero_si256();
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i s = _mm256_set1_epi16((short)whipe16[(ni * SNOVA_l + k1) * SNOVA_lr + a1 * SNOVA_r + i1]);
						const uint16_t *sb = &st0r16[(ni * SNOVA_l + k1) * SNOVA_lr];
						__m256i p0 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)sb));
						__m256i p1 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&sb[16]));
						if (k1 & 1) {
							ao0 = _mm256_xor_si256(ao0, p0);
							ao1 = _mm256_xor_si256(ao1, p1);
						} else        {
							ae0 = _mm256_xor_si256(ae0, p0);
							ae1 = _mm256_xor_si256(ae1, p1);
						}
					}
				_Alignas(32) uint8_t tp[32];
				_mm_store_si128((__m128i *)tp,
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae0, ao0))));
				_mm_store_si128((__m128i *)&tp[16],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae1, ao1))));
#endif
#else
				__m256i ae = _mm256_setzero_si256(), ao = _mm256_setzero_si256();
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i bc = _mm256_set1_epi8((char)whipped_sig[(ni * SNOVA_l + k1) * SNOVA_lr + a1 * SNOVA_r + i1]);
						__m256i pd = _mm256_gf2p8mul_epi8(bc,
						                                  _mm256_loadu_si256((const __m256i *)&sum_t0[((mi * SNOVA_v + ni) * SNOVA_l + k1) * SNOVA_lr]));
						if (k1 & 1) {
							ao = _mm256_xor_si256(ao, pd);
						} else {
							ae = _mm256_xor_si256(ae, pd);
						}
					}
				_Alignas(32) uint8_t tp[32];
				_mm256_store_si256((__m256i *)tp, RCT_SJ_CLEAN256(_mm256_xor_si256(ae, ao)));
#endif
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					memcpy(&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r],
					       &tp[b1 * SNOVA_r], SNOVA_r);
			}
	}
#if RCT_SJ_M4
	SNOVA_CLEAR_OBJ(whipe16);
	SNOVA_CLEAR_OBJ(st0r16);
#endif
#else
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
#if RCT_SJ_A4
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				rct_a4_acc_t acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = rct_a4_zero();
				}
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					rct_a4_bc_t bv = rct_a4_bc(&P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l]);
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						rct_a4_mac(&acc[b1], bv, &whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr]);
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&sum_t0[((mi * SNOVA_l + b1) * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r], rct_a4_fold(acc[b1]));
				}
			}
#else
		for (int b1 = 0; b1 < SNOVA_l; ++b1)
			for (int ni = 0; ni < SNOVA_v; ++ni)
				for (int i1 = 0; i1 < SNOVA_l; ++i1) {
					__m128i acc = _mm_setzero_si128();
					for (int nj = 0; nj < SNOVA_v; ++nj) {
						const gf_t *pc = &P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l];
						const gf_t *w = &whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr];
						for (int k1 = 0; k1 < SNOVA_l; ++k1)
							acc = _mm_xor_si128(acc, rct_sj_sv128(rct_sj_bc128_pub(pc[k1]),
							                                      _mm_loadu_si128((const __m128i *)&w[k1 * SNOVA_r])));
					}
					rct_sj_store_r(&sum_t0[((mi * SNOVA_l + b1) * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r], acc);
				}
#endif
#if RCT_SJ_A4
		if (mi == 0) {
			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int i1 = 0; i1 < SNOVA_r; ++i1)
						for (int k1 = 0; k1 < SNOVA_l; ++k1)
							rct_a4_wsigT[(a1 * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_l + k1] =
							    whipped_sig[(a1 * SNOVA_v + ni) * SNOVA_lr + k1 * SNOVA_r + i1];
		}
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int i1 = 0; i1 < SNOVA_r; ++i1) {
				rct_a4_acc_t acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = rct_a4_zero();
				}
				for (int ni = 0; ni < SNOVA_v; ++ni) {
					rct_a4_bc_t bv = rct_a4_bc(&rct_a4_wsigT[(a1 * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_l]);
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						rct_a4_mac(&acc[b1], bv, &sum_t0[((mi * SNOVA_l + b1) * SNOVA_v + ni) * SNOVA_lr]);
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r], rct_a4_fold(acc[b1]));
				}
			}
#elif RCT_SJ_M4
		if (mi == 0) {
			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int i1 = 0; i1 < SNOVA_r; ++i1)
						for (int k1 = 0; k1 < SNOVA_l; ++k1)
							rct_a4_wsigT[(a1 * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_l + k1] =
							    whipped_sig[(a1 * SNOVA_v + ni) * SNOVA_lr + k1 * SNOVA_r + i1];
		}
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int i1 = 0; i1 < SNOVA_r; ++i1) {
				rct_m4_acc_t acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = rct_m4_zero();
				}
				for (int ni = 0; ni < SNOVA_v; ++ni) {
					rct_m4_bc_t bv = rct_m4_bc(&rct_a4_wsigT[(a1 * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_l]);
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						rct_m4_mac(&acc[b1], bv, &sum_t0[((mi * SNOVA_l + b1) * SNOVA_v + ni) * SNOVA_lr]);
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r], rct_m4_fold(acc[b1]));
				}
			}
#else
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int i1 = 0; i1 < SNOVA_r; ++i1) {
				__m128i acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = _mm_setzero_si128();
				}
				for (int ni = 0; ni < SNOVA_v; ++ni) {
					const gf_t *w = &whipped_sig[(a1 * SNOVA_v + ni) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m128i bc = rct_sj_bc128(w[k1 * SNOVA_r + i1]);
						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							acc[b1] = _mm_xor_si128(acc[b1], rct_sj_sv128(bc,
							    _mm_loadu_si128((const __m128i *)&sum_t0[(((mi * SNOVA_l + b1) * SNOVA_v + ni)) * SNOVA_lr + k1 * SNOVA_r])));
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r], acc[b1]);
				}
			}
#endif
	}
#endif
	SNOVA_CLEAR_OBJ(sum_t0);
#if (RCT_SJ_A4 || RCT_SJ_M4) && !RCT_SJ_B1L
	SNOVA_CLEAR_OBJ(rct_a4_wsigT);
#endif
}

static void rct_sign_wf_jog(rct_sign_ctx *c) {
	rct_sj_ensure();
	const gf_t *F21 = c->F21, *F12 = c->F12, *whipped_sig = c->whipped_sig;
	gf_t *whipped_F21 = c->whipped_F21, *whipped_F12 = c->whipped_F12;
#if RCT_SJ_B1L
#if RCT_SJ_M4
	RCT_SCRATCH _Alignas(32) uint16_t whipr16[RCT_B1L_WCH + 16];
	RCT_SCRATCH _Alignas(32) uint16_t f21e16[RCT_B1L_FCH + 16];
	RCT_SCRATCH _Alignas(32) uint16_t f12e16[RCT_B1L_FCH + 16];
	memset(&whipr16[RCT_B1L_WCH], 0, 16 * sizeof(uint16_t));
	rct_m4_raw_buf(whipr16, whipped_sig, RCT_B1L_WN);
	rct_m4_expand_buf(f21e16, F21, RCT_B1L_FN);
	rct_m4_expand_buf(f12e16, F12, RCT_B1L_FN);
#endif
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		for (int idx = 0; idx < SNOVA_o; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
#if RCT_SJ_M4
#if SNOVA_r == 8
				__m256i ae0 = _mm256_setzero_si256(), ae1 = _mm256_setzero_si256(), ae2 = _mm256_setzero_si256();
				__m256i ao0 = _mm256_setzero_si256(), ao1 = _mm256_setzero_si256(), ao2 = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const uint16_t *fr = &f21e16[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l];
					const uint16_t *wb = &whipr16[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i s = _mm256_set1_epi16((short)fr[k1]);
						__m256i p0 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						__m256i p1 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr + 16]));
						__m256i p2 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr + 32]));
						if (k1 & 1) {
							ao0 = _mm256_xor_si256(ao0, p0);
							ao1 = _mm256_xor_si256(ao1, p1);
							ao2 = _mm256_xor_si256(ao2, p2);
						} else        {
							ae0 = _mm256_xor_si256(ae0, p0);
							ae1 = _mm256_xor_si256(ae1, p1);
							ae2 = _mm256_xor_si256(ae2, p2);
						}
					}
				}
				_Alignas(32) uint8_t tp[48];
				_mm_store_si128((__m128i *)tp,
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae0, ao0))));
				_mm_store_si128((__m128i *)&tp[16],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae1, ao1))));
				_mm_store_si128((__m128i *)&tp[32],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae2, ao2))));
#else
				__m256i ae0 = _mm256_setzero_si256(), ae1 = _mm256_setzero_si256();
				__m256i ao0 = _mm256_setzero_si256(), ao1 = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const uint16_t *fr = &f21e16[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l];
					const uint16_t *wb = &whipr16[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i s = _mm256_set1_epi16((short)fr[k1]);
						__m256i p0 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						__m256i p1 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr + 16]));
						if (k1 & 1) {
							ao0 = _mm256_xor_si256(ao0, p0);
							ao1 = _mm256_xor_si256(ao1, p1);
						} else        {
							ae0 = _mm256_xor_si256(ae0, p0);
							ae1 = _mm256_xor_si256(ae1, p1);
						}
					}
				}
				_Alignas(32) uint8_t tp[32];
				_mm_store_si128((__m128i *)tp,
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae0, ao0))));
				_mm_store_si128((__m128i *)&tp[16],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae1, ao1))));
#endif
#else
				__m256i ae = _mm256_setzero_si256(), ao = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const gf_t *fr = &F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l];
					const gf_t *wb = &whipped_sig[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i pd = _mm256_gf2p8mul_epi8(_mm256_set1_epi8((char)fr[k1]),
						                                  _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						if (k1 & 1) {
							ao = _mm256_xor_si256(ao, pd);
						} else {
							ae = _mm256_xor_si256(ae, pd);
						}
					}
				}
				_Alignas(32) uint8_t tp[32];
				_mm256_store_si256((__m256i *)tp, RCT_SJ_CLEAN256(_mm256_xor_si256(ae, ao)));
#endif
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					memcpy(&whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r],
					       &tp[b1 * SNOVA_r], SNOVA_r);
			}
		for (int idx = 0; idx < SNOVA_o; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
#if RCT_SJ_M4
#if SNOVA_r == 8
				__m256i ae0 = _mm256_setzero_si256(), ae1 = _mm256_setzero_si256(), ae2 = _mm256_setzero_si256();
				__m256i ao0 = _mm256_setzero_si256(), ao1 = _mm256_setzero_si256(), ao2 = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const uint16_t *fr = &f12e16[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + i1];
					const uint16_t *wb = &whipr16[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i s = _mm256_set1_epi16((short)fr[k1 * SNOVA_l]);
						__m256i p0 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						__m256i p1 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr + 16]));
						__m256i p2 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr + 32]));
						if (k1 & 1) {
							ao0 = _mm256_xor_si256(ao0, p0);
							ao1 = _mm256_xor_si256(ao1, p1);
							ao2 = _mm256_xor_si256(ao2, p2);
						} else        {
							ae0 = _mm256_xor_si256(ae0, p0);
							ae1 = _mm256_xor_si256(ae1, p1);
							ae2 = _mm256_xor_si256(ae2, p2);
						}
					}
				}
				_Alignas(32) uint8_t tp[48];
				_mm_store_si128((__m128i *)tp,
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae0, ao0))));
				_mm_store_si128((__m128i *)&tp[16],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae1, ao1))));
				_mm_store_si128((__m128i *)&tp[32],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae2, ao2))));
#else
				__m256i ae0 = _mm256_setzero_si256(), ae1 = _mm256_setzero_si256();
				__m256i ao0 = _mm256_setzero_si256(), ao1 = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const uint16_t *fr = &f12e16[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + i1];
					const uint16_t *wb = &whipr16[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i s = _mm256_set1_epi16((short)fr[k1 * SNOVA_l]);
						__m256i p0 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						__m256i p1 = _mm256_mullo_epi16(s, _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr + 16]));
						if (k1 & 1) {
							ao0 = _mm256_xor_si256(ao0, p0);
							ao1 = _mm256_xor_si256(ao1, p1);
						} else        {
							ae0 = _mm256_xor_si256(ae0, p0);
							ae1 = _mm256_xor_si256(ae1, p1);
						}
					}
				}
				_Alignas(32) uint8_t tp[32];
				_mm_store_si128((__m128i *)tp,
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae0, ao0))));
				_mm_store_si128((__m128i *)&tp[16],
				                cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(_mm256_xor_si256(ae1, ao1))));
#endif
#else
				__m256i ae = _mm256_setzero_si256(), ao = _mm256_setzero_si256();
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const gf_t *fr = &F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + i1];
					const gf_t *wb = &whipped_sig[(nj * SNOVA_l) * SNOVA_lr];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m256i pd = _mm256_gf2p8mul_epi8(_mm256_set1_epi8((char)fr[k1 * SNOVA_l]),
						                                  _mm256_loadu_si256((const __m256i *)&wb[k1 * SNOVA_lr]));
						if (k1 & 1) {
							ao = _mm256_xor_si256(ao, pd);
						} else {
							ae = _mm256_xor_si256(ae, pd);
						}
					}
				}
				_Alignas(32) uint8_t tp[32];
				_mm256_store_si256((__m256i *)tp, RCT_SJ_CLEAN256(_mm256_xor_si256(ae, ao)));
#endif
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					memcpy(&whipped_F12[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r],
					       &tp[b1 * SNOVA_r], SNOVA_r);
			}
	}
#if RCT_SJ_M4
	SNOVA_CLEAR_OBJ(whipr16);
	SNOVA_CLEAR_OBJ(f21e16);
	SNOVA_CLEAR_OBJ(f12e16);
#endif
#else
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
#if RCT_SJ_A4
		for (int idx = 0; idx < SNOVA_o; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				rct_a4_acc_t acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = rct_a4_zero();
				}
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					rct_a4_bc_t bv = rct_a4_bc(&F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l]);
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						rct_a4_mac(&acc[b1], bv, &whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr]);
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r], rct_a4_fold(acc[b1]));
				}
			}
#elif RCT_SJ_M4
		for (int idx = 0; idx < SNOVA_o; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				rct_m4_acc_t acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = rct_m4_zero();
				}
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					rct_m4_bc_t bv = rct_m4_bc(&F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l]);
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						rct_m4_mac(&acc[b1], bv, &whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr]);
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r], rct_m4_fold(acc[b1]));
				}
			}
#else
		for (int idx = 0; idx < SNOVA_o; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				__m128i acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = _mm_setzero_si128();
				}
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					const gf_t *fr = &F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l];
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m128i bc = rct_sj_bc128(fr[k1]);
						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							acc[b1] = _mm_xor_si128(acc[b1], rct_sj_sv128(bc,
							    _mm_loadu_si128((const __m128i *)&whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r])));
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r], acc[b1]);
				}
			}
#endif
		for (int idx = 0; idx < SNOVA_o; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				__m128i acc[SNOVA_l];
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					acc[b1] = _mm_setzero_si128();
				}
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						__m128i bc = rct_sj_bc128(F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + k1 * SNOVA_l + i1]);
						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							acc[b1] = _mm_xor_si128(acc[b1], rct_sj_sv128(bc,
							    _mm_loadu_si128((const __m128i *)&whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r])));
					}
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					rct_sj_store_r(&whipped_F12[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r], acc[b1]);
				}
			}
	}
#endif
}

static void rct_sign_fvv_jog(rct_sign_ctx *c) {
	rct_sj_ensure();
	const gf_t *q1 = c->q1, *q2 = c->q2, *Am = c->Am, *Bm = c->Bm, *sum_t1 = c->sum_t1;
	gf_t *Fvv = c->Fvv;
	for (int mi = 0; mi < SNOVA_o; ++mi)
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			int mi_prime = i_prime(mi, alpha);
			gf_t temp1[SNOVA_r2 + 16] = {0};
			gf_t temp2[SNOVA_lr + 16] = {0};
			const gf_t *q1r = &q1[(mi * SNOVA_alpha + alpha) * SNOVA_l];
			const gf_t *q2r = &q2[(mi * SNOVA_alpha + alpha) * SNOVA_l];
			for (int cc = 0; cc < SNOVA_r2; cc += 16) {
				__m128i t1 = _mm_setzero_si128();
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					__m128i t0 = _mm_setzero_si128();
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						t0 = _mm_xor_si128(t0, rct_sj_sv128(rct_sj_bc128_pub(q2r[b1]),
						                                    _mm_loadu_si128((const __m128i *)
						                                        &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + cc])));
					t0 = RCT_SJ_CLEAN(t0);
					t1 = _mm_xor_si128(t1, rct_sj_sv128(rct_sj_bc128_pub(q1r[a1]), t0)    );
				}
				_mm_storeu_si128((__m128i *)&temp1[cc], RCT_SJ_CLEAN(t1));
			}
			rct_sj_mm_add(temp2, temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_r, SNOVA_r, SNOVA_l);
			rct_sj_mm_add_pub(&Fvv[mi * SNOVA_lr], &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], temp2, SNOVA_r, SNOVA_r, SNOVA_l);
		}
}

#if SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR)
static void rct_sign_apply_t12_jog(rct_sign_ctx *c, const gf_t *solpad) {
	rct_sj_ensure();
	gf_t *signature_in_GF = c->signature_in_GF;
	const gf_t *T12 = c->T12;
	for (int index = 0; index < SNOVA_v; ++index)
		for (int mi = 0; mi < SNOVA_o; ++mi)
			rct_sj_mm_add(&signature_in_GF[index * SNOVA_lr],
			              &T12[(index * SNOVA_o + mi) * SNOVA_l2],
			              &solpad[mi * SNOVA_lr], SNOVA_l, SNOVA_l, SNOVA_r);
}
#endif

static void rct_skx_fold_F_jog(gf_t *F21, gf_t *F12, const gf_t *T12, const gf_t *P11) {
	rct_sj_ensure();
#if RCT_F5_A4
	for (int k1 = 0; k1 < SNOVA_o; ++k1) {
		rct_a4f_bc_t bct[SNOVA_v][5];
		for (int j2 = 0; j2 < SNOVA_v; ++j2)
			for (int i = 0; i < 5; ++i) {
				bct[j2][i] = rct_a4f_bc(&T12[(j2 * SNOVA_o + k1) * SNOVA_l2 + i * 5]);
			}
		for (int i1 = 0; i1 < SNOVA_m1; ++i1)
			for (int j1 = 0; j1 < SNOVA_v; ++j1) {
				gf_t *C = &F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2];
				rct_a4f_acc_t a[5];
				for (int i = 0; i < 5; ++i) {
					a[i] = rct_a4f_zero();
				}
				for (int j2 = 0; j2 < SNOVA_v; ++j2) {
					const gf_t *w = &P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2];
					for (int i = 0; i < 5; ++i) {
						rct_a4f_mac(&a[i], bct[j2][i], w);
					}
				}
				for (int i = 0; i < 5; ++i) {
					rct_a4f_xor5(&C[i * 5], rct_a4f_fold(a[i]));
				}
			}
		SNOVA_CLEAR_OBJ(bct);
	}
#elif RCT_F5_M4 && !RCT_FOLD_WIDE
	for (int k1 = 0; k1 < SNOVA_o; ++k1) {
		rct_m4f_bc_t bct[SNOVA_v][5];
		for (int j2 = 0; j2 < SNOVA_v; ++j2) {
			_Alignas(32) uint16_t ev[32];
			rct_m4_expand_buf(ev, &T12[(j2 * SNOVA_o + k1) * SNOVA_l2], SNOVA_l2);
			for (int i = 0; i < 5; ++i) {
				bct[j2][i] = rct_m4f2_bc_mirror(&ev[i * 5]);
			}
		}
		for (int i1 = 0; i1 < SNOVA_m1; ++i1)
			for (int j1 = 0; j1 < SNOVA_v; ++j1) {
				gf_t *C = &F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2];
				__m256i a0[5], a1[5];
				for (int i = 0; i < 5; ++i) {
					a0[i] = _mm256_setzero_si256();
					a1[i] = _mm256_setzero_si256();
				}
				for (int j2 = 0; j2 < SNOVA_v; ++j2)
					rct_m4f2_mac5(a0, a1, bct[j2],
					              &P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2]);
				rct_m4f2_fold5_xor(C, a0, a1);
			}
		SNOVA_CLEAR_OBJ(bct);
	}
#elif RCT_F5_WIDE
	{
		enum { RCT_MVL = RCT_WIDE_MVL };
		RCT_WIDE_SCRATCH_DECL;
		memset(Fw, 0, RCT_WIDE_FW_LEN * sizeof(uint16_t));
		for (int nk = 0; nk < SNOVA_v; ++nk)
			for (int k1 = 0; k1 < SNOVA_l; ++k1)
				for (int mi = 0; mi < SNOVA_m1; ++mi)
					for (int nj = 0; nj < SNOVA_v; ++nj)
						for (int j1 = 0; j1 < SNOVA_l; ++j1)
							P11aw[(nk * SNOVA_l + k1) * RCT_MVL + (mi * SNOVA_v + nj) * SNOVA_l + j1] =
							    P11[((mi * SNOVA_v + nk) * SNOVA_v + nj) * SNOVA_l2 + k1 * SNOVA_l + j1];
		for (int ni = 0; ni < SNOVA_o; ++ni)
			for (int i1 = 0; i1 < SNOVA_l; ++i1)
				for (int nk = 0; nk < SNOVA_v; ++nk)
					for (int k1 = 0; k1 < SNOVA_l; ++k1) {
						uint16_t s = cl_expand_scalar16(T12[(nk * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + k1]);
						uint16_t *Fr = &Fw[(ni * SNOVA_l + i1) * RCT_MVL];
						const uint16_t *Pr = &P11aw[(nk * SNOVA_l + k1) * RCT_MVL];
						for (int mi = 0; mi < RCT_MVL; ++mi) {
							Fr[mi] ^= (uint16_t)(s * Pr[mi]);
						}
					}
		for (int i = 0; i < RCT_MVL * SNOVA_o * SNOVA_l; i += 16) {
			_mm256_storeu_si256((__m256i *)&Fw[i], cl_gf16_compress_u16x16(_mm256_loadu_si256((const __m256i *)&Fw[i])));
		}
		for (int mi = 0; mi < SNOVA_m1; ++mi)
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int ni = 0; ni < SNOVA_o; ++ni)
					for (int i1 = 0; i1 < SNOVA_l; ++i1)
						for (int j1 = 0; j1 < SNOVA_l; ++j1)
							F21[((mi * SNOVA_o + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + j1] =
							    (gf_t)Fw[(ni * SNOVA_l + i1) * RCT_MVL + (mi * SNOVA_v + nj) * SNOVA_l + j1];
		SNOVA_CLEAR(Fw, RCT_WIDE_FW_LEN * sizeof(uint16_t));
		rct_wide_fold_F12(F12, P11, T12, P11aw, Fw);
	}
#elif RCT_SIGN_JOG && !RCT_HAVE_GFNI
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j2 = 0; j2 < SNOVA_v; ++j2)
			for (int k1 = 0; k1 < SNOVA_o; ++k1) {
				__m128i bcx[SNOVA_l2];
				for (int t = 0; t < SNOVA_l2; ++t) {
					bcx[t] = rct_sj_bc128(T12[(j2 * SNOVA_o + k1) * SNOVA_l2 + t]);
				}
				for (int j1 = 0; j1 < SNOVA_v; ++j1) {
					gf_t *C = &F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2];
					const gf_t *B = &P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2];
					for (int i = 0; i < SNOVA_l; ++i) {
						__m128i acc = _mm_setzero_si128();
						for (int k = 0; k < SNOVA_l; ++k)
							acc = _mm_xor_si128(acc, rct_sj_sv128(bcx[i * SNOVA_l + k],
							                                      _mm_loadu_si128((const __m128i *)&B[k * SNOVA_l])));
						_Alignas(16) uint8_t pb[16];
						_mm_store_si128((__m128i *)pb, RCT_SJ_CLEAN(acc));
						for (int j = 0; j < SNOVA_l; ++j) {
							C[i * SNOVA_l + j] = (gf_t)(C[i * SNOVA_l + j] ^ pb[j]);
						}
					}
				}
			}
#else
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j1 = 0; j1 < SNOVA_v; ++j1)
			for (int j2 = 0; j2 < SNOVA_v; ++j2)
				for (int k1 = 0; k1 < SNOVA_o; ++k1)
					rct_sj_mm_add(&F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2],
					              &T12[(j2 * SNOVA_o + k1) * SNOVA_l2],
					              &P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2],
					              SNOVA_l, SNOVA_l, SNOVA_l);
#endif
#if RCT_F5_WIDE
#elif RCT_F5_A4 || RCT_F5_M4
	rct_f5_fold_F12(F12, P11, T12);
#else
	for (int i1 = 0; i1 < SNOVA_m1; ++i1)
		for (int j1 = 0; j1 < SNOVA_v; ++j1)
			for (int j2 = 0; j2 < SNOVA_v; ++j2)
				for (int k1 = 0; k1 < SNOVA_o; ++k1)
					rct_sj_mm_add_pub(&F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                  &P11[((i1 * SNOVA_v + j1) * SNOVA_v + j2) * SNOVA_l2],
					                  &T12[(j2 * SNOVA_o + k1) * SNOVA_l2],
					                  SNOVA_l, SNOVA_l, SNOVA_l);
#endif
}

#endif

#endif

// #include "platforms/generic/rct_sign_scalar.h"
#ifndef RCT_SIGN_SCALAR_H
#define RCT_SIGN_SCALAR_H

#if !(RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC) \
    && !(SNOVA_Q == 16 && (RCT_USE_GFNI || RCT_HOT_QRP16) && !defined(RCT_GAUSS_SCALAR) && SNOVA_L == 4) \
    && !(RCT_SIGN_JOG && SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR))
static void rct_sign_apply_t12_scalar(rct_sign_ctx *c, const gf_t *solution) {
	gf_t *signature_in_GF = c->signature_in_GF;
	const gf_t *T12 = c->T12;
	for (int index = 0; index < SNOVA_v; ++index)
		for (int mi = 0; mi < SNOVA_o; ++mi)
			gf_mat_mul_add_lr_sec(&signature_in_GF[index * SNOVA_lr], &T12[(index * SNOVA_o + mi) * SNOVA_l2],
			                      &solution[mi * SNOVA_lr], SNOVA_l, SNOVA_l, SNOVA_r);
}
#endif

#if !(RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC) \
    && !(SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR))
static void rct_sign_backsub_scalar(rct_sign_ctx *c, gf_t *solution) {
	gf_t (*gauss)[SNOVA_o * SNOVA_lr + 1 + 64] = c->gauss;
	memset(solution, 0, (size_t)(SNOVA_o * SNOVA_lr) * sizeof(gf_t));
	for (int i = SNOVA_o * SNOVA_lr - 1; i >= 0; --i) {
		gf_t sum = 0;
		for (int k = i + 1; k < SNOVA_o * SNOVA_lr; ++k) {
			gf_set_add(&sum, gf_mult_sec(gauss[i][k], solution[k]));
		}
		solution[i] = gf_sub(gauss[i][SNOVA_o * SNOVA_lr], sum);
	}
}
#endif

#if !RCT_SIGN_JOG && !defined(RCT_CM_ACTIVE) && !defined(RCT_CMS3_ONLY) \
    && !(RCT_Q_SIMD && (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC) \
    && !(RCT_Q_SIMD && (SNOVA_r == SNOVA_l))
static void rct_sign_wf_std(rct_sign_ctx *c) {
	const gf_t *F21 = c->F21, *F12 = c->F12, *whipped_sig = c->whipped_sig;
	gf_t *whipped_F21 = c->whipped_F21, *whipped_F12 = c->whipped_F12;
	for (int mi = 0; mi < SNOVA_m1; mi++) {
#if RCT_HOT_SIMD && SNOVA_l == 4
		for (int idx = 0; idx < SNOVA_o; idx++)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int i1 = 0; i1 < SNOVA_l; i1++) {
					__m128i acc = _mm_setzero_si128();
					for (int nj = 0; nj < SNOVA_v; ++nj)
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							__m128i s = RCT_BC128_SEC(
							                F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + k1]);
							__m128i w = _mm_loadu_si128(
							                (const __m128i *)&whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r]);
							acc = _mm_xor_si128(acc, RCT_SV128(s, w));
						}
					acc = rct_gfni_cleanup128(acc);
					_Alignas(16) uint8_t tmp[16];
					_mm_store_si128((__m128i *)tmp, acc);
					memcpy(&whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r],
					       tmp, SNOVA_r);
				}
#else
		for (int idx = 0; idx < SNOVA_o; idx++)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					gf_mat_mul_add_lr_sec(&whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr],
					                      &F21[((mi * SNOVA_o + idx) * SNOVA_v + nj) * SNOVA_l2],
					                      &whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr], SNOVA_l, SNOVA_l, SNOVA_r);
#endif

#if RCT_HOT_SIMD && SNOVA_l == 4
		for (int idx = 0; idx < SNOVA_o; idx++)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int i1 = 0; i1 < SNOVA_l; i1++) {
					__m128i acc = _mm_setzero_si128();
					for (int nj = 0; nj < SNOVA_v; ++nj)
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							__m128i s = RCT_BC128_SEC(
							                F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + k1 * SNOVA_l + i1]);
							__m128i w = _mm_loadu_si128(
							                (const __m128i *)&whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r]);
							acc = _mm_xor_si128(acc, RCT_SV128(s, w));
						}
					acc = rct_gfni_cleanup128(acc);
					_Alignas(16) uint8_t tmp[16];
					_mm_store_si128((__m128i *)tmp, acc);
					memcpy(&whipped_F12[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r],
					       tmp, SNOVA_r);
				}
#else
		for (int idx = 0; idx < SNOVA_o; idx++)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								gf_set_add(&whipped_F12[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1],
								           gf_mult_sec(F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + k1 * SNOVA_l + i1],
								                       whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r + j1]));
#endif
	}
}
#endif

#if !defined(RCT_CM_ACTIVE) && !defined(RCT_CMS3_ONLY) \
    && !(RCT_Q_SIMD && (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC) \
    && !(RCT_Q_SIMD && (SNOVA_r == SNOVA_l))
#ifndef RCT_SIGN_P2_RSWAP
#define RCT_SIGN_P2_RSWAP 1
#endif
#if RCT_SIGN_P2_RSWAP && (RCT_SJ_A4 || RCT_SJ_M4)
#define RCT_SJ_RSWAP 1
#else
#define RCT_SJ_RSWAP 0
#endif

#if RCT_SJ_RSWAP
#if RCT_SJ_M4
#define RCT_RS_BC_T rct_m4_bc_t
#if SNOVA_r == 8
#define RCT_RS_OVW 48
#else
#define RCT_RS_OVW 32
#endif
#define RCT_RS_G16W ((SNOVA_o * SNOVA_lr + RCT_RS_OVW + 15) / 16 * 16)
static inline rct_m4_bc_t rct_rs_m4_bc16(const uint16_t *ev5) {
	__m256i evb = _mm256_broadcastsi128_si256(_mm_loadu_si128((const __m128i *)ev5));
	rct_m4_bc_t b;
	b.b0 = _mm256_shuffle_epi8(evb, RCT_M4_PAT0);
	b.b1 = _mm256_shuffle_epi8(evb, RCT_M4_PAT1);
#if SNOVA_r == 8
	b.b2 = _mm256_set1_epi16((short)ev5[4]);
#endif
	return b;
}
static inline void rct_rs_axpy16(uint16_t *g16, rct_m4_bc_t b, const gf_t *tile) {
	rct_m4_acc_t A = rct_m4_zero();
	rct_m4_mac(&A, b, tile);
	_mm256_storeu_si256((__m256i *)g16,
	                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)g16), A.a0));
	_mm256_storeu_si256((__m256i *)&g16[16],
	                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)&g16[16]), A.a1));
#if SNOVA_r == 8
	_mm256_storeu_si256((__m256i *)&g16[32],
	                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)&g16[32]), A.a2));
#endif
}
#else
#if SNOVA_r == 8
#define RCT_RS_BC_T rct_a4_bc_t
#define RCT_RS_BC_FN(sc) rct_a4_bc(sc)
#define RCT_RS_OVW 48
static inline void rct_rs_axpy(gf_t *g, rct_a4_bc_t b, const gf_t *tile) {
	rct_a4_acc_t A = rct_a4_zero();
	rct_a4_mac(&A, b, tile);
	_mm256_storeu_si256((__m256i *)g,
	                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)g), A.a));
	_mm_storeu_si128((__m128i *)&g[32],
	                 _mm_xor_si128(_mm_loadu_si128((const __m128i *)&g[32]), A.t));
}
#else
#define RCT_RS_BC_T __m256i
#define RCT_RS_BC_FN(sc) rct_rs_bc(sc)
#define RCT_RS_OVW 32
#define RCT_RS_BL(p) (char)((p) < SNOVA_lr ? (p) / SNOVA_r : -1)
static inline __m256i rct_rs_bc(const gf_t *sc) {
	const __m256i P = _mm256_setr_epi8(
	                      RCT_RS_BL(0), RCT_RS_BL(1), RCT_RS_BL(2), RCT_RS_BL(3),
	                      RCT_RS_BL(4), RCT_RS_BL(5), RCT_RS_BL(6), RCT_RS_BL(7),
	                      RCT_RS_BL(8), RCT_RS_BL(9), RCT_RS_BL(10), RCT_RS_BL(11),
	                      RCT_RS_BL(12), RCT_RS_BL(13), RCT_RS_BL(14), RCT_RS_BL(15),
	                      RCT_RS_BL(16), RCT_RS_BL(17), RCT_RS_BL(18), RCT_RS_BL(19),
	                      RCT_RS_BL(20), RCT_RS_BL(21), RCT_RS_BL(22), RCT_RS_BL(23),
	                      RCT_RS_BL(24), RCT_RS_BL(25), RCT_RS_BL(26), RCT_RS_BL(27),
	                      RCT_RS_BL(28), RCT_RS_BL(29), RCT_RS_BL(30), RCT_RS_BL(31));
	return _mm256_shuffle_epi8(_mm256_broadcastsi128_si256(
	                               _mm_loadu_si128((const __m128i *)sc)), P);
}
static inline void rct_rs_axpy(gf_t *g, __m256i bc, const gf_t *tile) {
	__m256i prod = _mm256_gf2p8mul_epi8(bc, _mm256_loadu_si256((const __m256i *)tile));
	_mm256_storeu_si256((__m256i *)g,
	                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)g), prod));
}
#endif
#endif
_Static_assert((SNOVA_o - 1) * SNOVA_lr + RCT_RS_OVW <= SNOVA_o * SNOVA_lr + 1 + 64,
               "rswap: overlapped storeu must stay within gauss row (OLR+1+guard)");
#define RCT_RS_PL(C, p) (char)((16 * (C) + (p)) < SNOVA_lr ? ((16 * (C) + (p)) % SNOVA_r) : -1)
#define RCT_RS_PAT(C) _mm_setr_epi8( \
    RCT_RS_PL(C, 0), RCT_RS_PL(C, 1), RCT_RS_PL(C, 2), RCT_RS_PL(C, 3), \
    RCT_RS_PL(C, 4), RCT_RS_PL(C, 5), RCT_RS_PL(C, 6), RCT_RS_PL(C, 7), \
    RCT_RS_PL(C, 8), RCT_RS_PL(C, 9), RCT_RS_PL(C, 10), RCT_RS_PL(C, 11), \
    RCT_RS_PL(C, 12), RCT_RS_PL(C, 13), RCT_RS_PL(C, 14), RCT_RS_PL(C, 15))
#define RCT_RS_NCH ((SNOVA_lr + 16 + 15) / 16)
static inline void rct_rs_tile_build(uint8_t *tile, __m128i src) {
	_mm_store_si128((__m128i *)tile, _mm_shuffle_epi8(src, RCT_RS_PAT(0)));
	_mm_store_si128((__m128i *)&tile[16], _mm_shuffle_epi8(src, RCT_RS_PAT(1)));
	_mm_store_si128((__m128i *)&tile[32], _mm_shuffle_epi8(src, RCT_RS_PAT(2)));
#if RCT_RS_NCH > 3
	_mm_store_si128((__m128i *)&tile[48], _mm_shuffle_epi8(src, RCT_RS_PAT(3)));
#endif
}
#endif
static void rct_sign_gauss_scatter_std(rct_sign_ctx *c) {
	const gf_t *whipped_F21 = c->whipped_F21, *whipped_F12 = c->whipped_F12;
	const gf_t *q1 = c->q1, *q2 = c->q2, *Am = c->Am, *Bm = c->Bm, *Q1 = c->Q1, *Q2 = c->Q2;
	gf_t (*gauss)[SNOVA_o * SNOVA_lr + 1 + 64] = c->gauss;
#if RCT_SJ_RSWAP
	_Alignas(16) gf_t rs_xT[SNOVA_l2 + 16];
	memset(rs_xT, 0, sizeof(rs_xT));
#if RCT_SJ_M4
	RCT_SCRATCH _Alignas(32) uint16_t rs_g16[SNOVA_o * SNOVA_lr][RCT_RS_G16W];
	memset(rs_g16, 0, sizeof(rs_g16));
	_Alignas(32) uint16_t rs_xe16[(SNOVA_lr + 16 + 15) / 16 * 16];
	memset(rs_xe16, 0, sizeof(rs_xe16));
#define RCT_RS_AXPY_ROW(row, ix, bcv, tile) rct_rs_axpy16(&rs_g16[row][(ix) * SNOVA_lr], bcv, tile)
#else
#define RCT_RS_AXPY_ROW(row, ix, bcv, tile) rct_rs_axpy(&gauss[row][(ix) * SNOVA_lr], bcv, tile)
#endif

	for (int mi = 0; mi < SNOVA_o; mi++)
		for (int alpha = 0; alpha < SNOVA_alpha; alpha++) {
			int mi_prime = i_prime(mi, alpha);
			_Alignas(16) uint8_t amtile[SNOVA_r][RCT_RS_NCH * 16];
			for (int ti1 = 0; ti1 < SNOVA_r; ++ti1)
				rct_rs_tile_build(amtile[ti1], _mm_loadu_si128(
				                      (const __m128i *)&Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r]));
			for (int idx = 0; idx < SNOVA_o; idx++) {
				gf_t gf16m_temp0[SNOVA_l2 + 16] = {0};
				gf_t gf16m_temp1[SNOVA_lr + 16] = {0};
				gf_t X_tmp[SNOVA_l2 + 16] = {0};
#ifdef RCT_PROFILE
				uint64_t _t0 = __rdtsc(), _t1, _t2;
#endif
				for (int cc = 0; cc < SNOVA_lr; cc += 16) {
					__m128i acc = _mm_setzero_si128();
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						acc = _mm_xor_si128(acc, rct_sj_sv128(
						                        rct_sj_bc128_pub(q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
						                        _mm_loadu_si128((const __m128i *)&whipped_F21[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + cc])));
					_Alignas(16) uint8_t pb[16];
					_mm_store_si128((__m128i *)pb, RCT_SJ_CLEAN(acc));
					int nn = SNOVA_lr - cc;
					if (nn > 16) {
						nn = 16;
					}
					for (int j = 0; j < nn; ++j) {
						gf16m_temp1[cc + j] = pb[j];
					}
				}
#if RCT_SJ_M4
				rct_m4_mm_add(gf16m_temp0, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_l, SNOVA_r, SNOVA_l);
#else
				rct_sj_mm_add(gf16m_temp0, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_l, SNOVA_r, SNOVA_l);
#endif
				rct_sj_mm_add_pub(X_tmp, &Q1[(mi * SNOVA_alpha + alpha) * SNOVA_l2], gf16m_temp0, SNOVA_l, SNOVA_l, SNOVA_l);
#ifdef RCT_PROFILE
				_t1 = __rdtsc();
				RCT_PACC6B(0, _t0, _t1);
#endif
				for (int ti2 = 0; ti2 < SNOVA_l; ++ti2)
					for (int tj1 = 0; tj1 < SNOVA_l; ++tj1) {
						rs_xT[ti2 * SNOVA_l + tj1] = X_tmp[tj1 * SNOVA_l + ti2];
					}
#if RCT_SJ_M4
				rct_m4_expand_buf(rs_xe16, rs_xT, SNOVA_l2);
#endif
				for (int ti2 = 0; ti2 < SNOVA_l; ++ti2) {
#if RCT_SJ_M4
					RCT_RS_BC_T bcv = rct_rs_m4_bc16(&rs_xe16[ti2 * SNOVA_l]);
#else
					RCT_RS_BC_T bcv = RCT_RS_BC_FN(&rs_xT[ti2 * SNOVA_l]);
#endif
					for (int ti1 = 0; ti1 < SNOVA_r; ++ti1) {
						RCT_RS_AXPY_ROW(mi * SNOVA_lr + ti1 * SNOVA_l + ti2, idx, bcv, amtile[ti1]);
					}
				}
#ifdef RCT_PROFILE
				_t2 = __rdtsc();
				RCT_PACC6B(1, _t1, _t2);
#endif
			}
		}

	for (int mi = 0; mi < SNOVA_o; mi++)
		for (int alpha = 0; alpha < SNOVA_alpha; alpha++) {
			int mi_prime = i_prime(mi, alpha);
			_Alignas(16) uint8_t bmtile[SNOVA_l][RCT_RS_NCH * 16];
			{
				_Alignas(16) uint8_t bmT[SNOVA_l * SNOVA_r + 16];
				memset(&bmT[SNOVA_l * SNOVA_r], 0, 16);
				for (int tj2 = 0; tj2 < SNOVA_r; ++tj2)
					for (int ti2 = 0; ti2 < SNOVA_l; ++ti2) {
						bmT[ti2 * SNOVA_r + tj2] = Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2];
					}
				for (int ti2 = 0; ti2 < SNOVA_l; ++ti2) {
					rct_rs_tile_build(bmtile[ti2], _mm_loadu_si128((const __m128i *)&bmT[ti2 * SNOVA_r]));
				}
			}
			for (int idx = 0; idx < SNOVA_o; idx++) {
				gf_t gf16m_temp0[SNOVA_lr + 16] = {0};
				gf_t gf16m_temp1[SNOVA_lr + 16] = {0};
				gf_t X_tmp[SNOVA_lr + 16] = {0};
#ifdef RCT_PROFILE
				uint64_t _t0 = __rdtsc(), _t1, _t2;
#endif
				{
					_Alignas(16) uint8_t fbuf[SNOVA_lr + 16];
					for (int cc = 0; cc < SNOVA_lr; cc += 16) {
						__m128i acc = _mm_setzero_si128();
						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							acc = _mm_xor_si128(acc, rct_sj_sv128(
							                        rct_sj_bc128_pub(q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
							                        _mm_loadu_si128((const __m128i *)&whipped_F12[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + cc])));
						_mm_store_si128((__m128i *)&fbuf[cc], RCT_SJ_CLEAN(acc));
					}
					for (int i1 = 0; i1 < SNOVA_l; ++i1)
						for (int j1 = 0; j1 < SNOVA_r; ++j1) {
							gf16m_temp1[j1 * SNOVA_l + i1] = fbuf[i1 * SNOVA_r + j1];
						}
				}
				rct_sj_mm_add_pub(gf16m_temp0, &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp1, SNOVA_r, SNOVA_r, SNOVA_l);
#if RCT_SJ_M4
				rct_m4_mm_add(X_tmp, gf16m_temp0, &Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2], SNOVA_r, SNOVA_l, SNOVA_l);
#else
				rct_sj_mm_add(X_tmp, gf16m_temp0, &Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2], SNOVA_r, SNOVA_l, SNOVA_l);
#endif
#ifdef RCT_PROFILE
				_t1 = __rdtsc();
				RCT_PACC6B(0, _t0, _t1);
#endif
#if RCT_SJ_M4
				rct_m4_expand_buf(rs_xe16, X_tmp, SNOVA_lr);
#endif
				for (int ti1 = 0; ti1 < SNOVA_r; ++ti1) {
#if RCT_SJ_M4
					RCT_RS_BC_T bcv = rct_rs_m4_bc16(&rs_xe16[ti1 * SNOVA_l]);
#else
					RCT_RS_BC_T bcv = RCT_RS_BC_FN(&X_tmp[ti1 * SNOVA_l]);
#endif
					for (int ti2 = 0; ti2 < SNOVA_l; ++ti2) {
						RCT_RS_AXPY_ROW(mi * SNOVA_lr + ti1 * SNOVA_l + ti2, idx, bcv, bmtile[ti2]);
					}
				}
#ifdef RCT_PROFILE
				_t2 = __rdtsc();
				RCT_PACC6B(1, _t1, _t2);
#endif
			}
		}

#ifdef RCT_PROFILE
	uint64_t _tc0 = __rdtsc(), _tc1;
#endif
#if !RCT_SJ_M4
	for (int rr = 0; rr < SNOVA_o * SNOVA_lr; ++rr)
		for (int cc = 0; cc < SNOVA_o * SNOVA_lr; cc += 32)
			_mm256_storeu_si256((__m256i *)&gauss[rr][cc],
			                    rct_sj_cleanup256(_mm256_loadu_si256((const __m256i *)&gauss[rr][cc])));
#else
	for (int rr = 0; rr < SNOVA_o * SNOVA_lr; ++rr)
		for (int cc = 0; cc < SNOVA_o * SNOVA_lr; cc += 16) {
			__m128i pb = cl_gf16_pack_u16_to_bytes(cl_gf16_compress_u16x16(
			        _mm256_loadu_si256((const __m256i *)&rs_g16[rr][cc])));
			_mm_storeu_si128((__m128i *)&gauss[rr][cc],
			                 _mm_xor_si128(_mm_loadu_si128((const __m128i *)&gauss[rr][cc]), pb));
		}
#endif
#ifdef RCT_PROFILE
	_tc1 = __rdtsc();
	RCT_PACC6B(2, _tc0, _tc1);
#endif
	SNOVA_CLEAR_OBJ(rs_xT);
#if RCT_SJ_M4
	SNOVA_CLEAR_OBJ(rs_g16);
	SNOVA_CLEAR_OBJ(rs_xe16);
#endif
#undef RCT_RS_AXPY_ROW
#else
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
#define RCT_GPW 32
	RCT_SCRATCH _Alignas(32) uint8_t gaussp[SNOVA_o * SNOVA_lr][SNOVA_o][RCT_GPW];
	memset(gaussp, 0, sizeof(gaussp));
#elif RCT_SJ_M4
#define RCT_GPW (SNOVA_l * 16)
	RCT_SCRATCH _Alignas(32) uint16_t gaussp16[SNOVA_o * SNOVA_lr][SNOVA_o][SNOVA_l * 8];
	memset(gaussp16, 0, sizeof(gaussp16));
#elif RCT_SIGN_JOG
#define RCT_GPW (SNOVA_l * 16)
	RCT_SCRATCH _Alignas(32) uint8_t gaussp[SNOVA_o * SNOVA_lr][SNOVA_o][RCT_GPW];
	memset(gaussp, 0, sizeof(gaussp));
#endif

	for (int mi = 0; mi < SNOVA_o; mi++)
		for (int idx = 0; idx < SNOVA_o; idx++)
			for (int alpha = 0; alpha < SNOVA_alpha; alpha++) {
				gf_t gf16m_temp0[SNOVA_l2 + 16] = {0};
				gf_t gf16m_temp1[SNOVA_lr + 16] = {0};
				gf_t X_tmp[SNOVA_l2 + 16] = {0};
				int mi_prime = i_prime(mi, alpha);
#ifdef RCT_PROFILE
				uint64_t _t0 = __rdtsc(), _t1, _t2;
#endif
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
				{
					__m256i acc = _mm256_setzero_si256();
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						__m256i qv = RCT_BC(q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]);
						__m256i wv = _mm256_loadu_si256(
						                 (const __m256i *)&whipped_F21[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr]);
						acc = _mm256_xor_si256(acc, RCT_SV(qv, wv));
					}
					_Alignas(32) uint8_t accbuf[32];
					_mm256_store_si256((__m256i *)accbuf, acc);
					for (int i1 = 0; i1 < SNOVA_lr; i1++) {
						gf16m_temp1[i1] = rct_gfni_cleanup(accbuf[i1]);
					}
				}
#elif RCT_SIGN_JOG
				for (int cc = 0; cc < SNOVA_lr; cc += 16) {
					__m128i acc = _mm_setzero_si128();
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						acc = _mm_xor_si128(acc, rct_sj_sv128(
						                        rct_sj_bc128_pub(q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
						                        _mm_loadu_si128((const __m128i *)&whipped_F21[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + cc])));
					_Alignas(16) uint8_t pb[16];
					_mm_store_si128((__m128i *)pb, RCT_SJ_CLEAN(acc));
					int nn = SNOVA_lr - cc;
					if (nn > 16) {
						nn = 16;
					}
					for (int j = 0; j < nn; ++j) {
						gf16m_temp1[cc + j] = pb[j];
					}
				}
#else
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int i1 = 0; i1 < SNOVA_lr; i1++)
						gf_set_add(&gf16m_temp1[i1],
						           gf_mult_sec(whipped_F21[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1],
						                       q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]));
#endif
#if RCT_USE_SIMD && SNOVA_l == 4
				rct_matmul_l4rows_sec(gf16m_temp0, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_l, SNOVA_r);
#elif RCT_SJ_M4
				rct_m4_mm_add(gf16m_temp0, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_l, SNOVA_r, SNOVA_l);
#elif RCT_SIGN_JOG
				rct_sj_mm_add(gf16m_temp0, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_l, SNOVA_r, SNOVA_l);
#else
				gf_mat_mul_add_lr_sec(gf16m_temp0, gf16m_temp1, &Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_l, SNOVA_r, SNOVA_l);
#endif
#if RCT_SIGN_JOG
				rct_sj_mm_add_pub(X_tmp, &Q1[(mi * SNOVA_alpha + alpha) * SNOVA_l2], gf16m_temp0, SNOVA_l, SNOVA_l, SNOVA_l);
#else
				RCT_MATMUL_ADD_BSEC(X_tmp, &Q1[(mi * SNOVA_alpha + alpha) * SNOVA_l2], gf16m_temp0);
#endif
#ifdef RCT_PROFILE
				_t1 = __rdtsc();
				RCT_PACC6B(0, _t0, _t1);
#endif
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
				const __m128i _m7 = _mm_set_epi64x(0LL, 0x00FFFFFFFFFFFFFFLL);
				for (int ti1 = 0; ti1 < SNOVA_r; ti1++) {
					__m128i amv = _mm_and_si128(_mm_loadu_si128(
					                                (const __m128i *)&Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r]), _m7);
					for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
						__m128i p0 = RCT_SV128(RCT_BC128_SEC(X_tmp[0 * SNOVA_l + ti2]), amv);
						__m128i p1 = RCT_SV128(RCT_BC128_SEC(X_tmp[1 * SNOVA_l + ti2]), amv);
						__m128i p2 = RCT_SV128(RCT_BC128_SEC(X_tmp[2 * SNOVA_l + ti2]), amv);
						__m128i p3 = RCT_SV128(RCT_BC128_SEC(X_tmp[3 * SNOVA_l + ti2]), amv);
						__m256i comb = _mm256_set_m128i(_mm_unpacklo_epi64(p2, p3), _mm_unpacklo_epi64(p0, p1));
						__m256i *g = (__m256i *)&gaussp[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx][0];
						*g = _mm256_xor_si256(*g, comb);
					}
				}
#elif RCT_SJ_M4
				{
					_Alignas(32) uint16_t xe[SNOVA_l2 + 16];
					rct_m4_expand_buf(xe, X_tmp, SNOVA_l2);
					for (int ti1 = 0; ti1 < SNOVA_r; ti1++) {
						__m128i amv = _mm256_castsi256_si128(_mm256_cvtepu8_epi16(
						        _mm_loadu_si128((const __m128i *)
						                        &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r])));
						for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
							__m128i *gs = (__m128i *)gaussp16[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx];
							for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
								gs[tj1] = _mm_xor_si128(gs[tj1], _mm_mullo_epi16(
								                            _mm_set1_epi16((short)xe[tj1 * SNOVA_l + ti2]), amv));
						}
					}
				}
#elif RCT_SIGN_JOG
				{
					__m128i bcx[SNOVA_l * SNOVA_l];
					for (int k = 0; k < SNOVA_l * SNOVA_l; ++k) {
						bcx[k] = rct_sj_bc128(X_tmp[k]);
					}
					for (int ti1 = 0; ti1 < SNOVA_r; ti1++) {
						_Alignas(16) uint8_t amr[16] = {0};
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							amr[tj2] = Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r + tj2];
						}
						__m128i amv = _mm_load_si128((const __m128i *)amr);
						for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
							__m128i *gs = (__m128i *)gaussp[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx];
							for (int tj1 = 0; tj1 < SNOVA_l; tj1++) {
								gs[tj1] = _mm_xor_si128(gs[tj1], rct_sj_sv128(bcx[tj1 * SNOVA_l + ti2], amv));
							}
						}
					}
				}
#else
				for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
					for (int ti2 = 0; ti2 < SNOVA_l; ti2++)
						for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
							for (int tj2 = 0; tj2 < SNOVA_r; tj2++) {
								int ti = ti1 * SNOVA_l + ti2;
								int tj = tj1 * SNOVA_r + tj2;
								gf_set_add(&gauss[mi * SNOVA_lr + ti][idx * SNOVA_lr + tj],
								           gf_mult_sec(X_tmp[tj1 * SNOVA_l + ti2],
								                       Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r + tj2]));
							}
#endif
#ifdef RCT_PROFILE
				_t2 = __rdtsc();
				RCT_PACC6B(1, _t1, _t2);
#endif
			}

	for (int mi = 0; mi < SNOVA_o; mi++)
		for (int idx = 0; idx < SNOVA_o; idx++)
			for (int alpha = 0; alpha < SNOVA_alpha; alpha++) {
				gf_t gf16m_temp0[SNOVA_lr + 16] = {0};
				gf_t gf16m_temp1[SNOVA_lr + 16] = {0};
				gf_t X_tmp[SNOVA_lr + 16] = {0};
				int mi_prime = i_prime(mi, alpha);
#ifdef RCT_PROFILE
				uint64_t _t0 = __rdtsc(), _t1, _t2;
#endif
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
				{
					__m256i acc = _mm256_setzero_si256();
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						__m256i qv = RCT_BC(q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]);
						__m256i wv = _mm256_loadu_si256(
						                 (const __m256i *)&whipped_F12[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr]);
						acc = _mm256_xor_si256(acc, RCT_SV(qv, wv));
					}
					_Alignas(32) uint8_t accbuf[32];
					_mm256_store_si256((__m256i *)accbuf, acc);
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++) {
							gf16m_temp1[j1 * SNOVA_l + i1] = rct_gfni_cleanup(accbuf[i1 * SNOVA_r + j1]);
						}
				}
#elif RCT_SIGN_JOG
				{
					_Alignas(16) uint8_t fbuf[SNOVA_lr + 16];
					for (int cc = 0; cc < SNOVA_lr; cc += 16) {
						__m128i acc = _mm_setzero_si128();
						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							acc = _mm_xor_si128(acc, rct_sj_sv128(
							                        rct_sj_bc128_pub(q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
							                        _mm_loadu_si128((const __m128i *)&whipped_F12[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + cc])));
						_mm_store_si128((__m128i *)&fbuf[cc], RCT_SJ_CLEAN(acc));
					}
					for (int i1 = 0; i1 < SNOVA_l; ++i1)
						for (int j1 = 0; j1 < SNOVA_r; ++j1) {
							gf16m_temp1[j1 * SNOVA_l + i1] = fbuf[i1 * SNOVA_r + j1];
						}
				}
#else
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							gf_set_add(&gf16m_temp1[j1 * SNOVA_l + i1],
							           gf_mult_sec(whipped_F12[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1],
							                       q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]));
#endif
#if RCT_USE_SIMD && SNOVA_l == 4
				rct_matmul_l4rows(gf16m_temp0, &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp1, SNOVA_r, SNOVA_r);
				rct_matmul_l4rows_sec(X_tmp, gf16m_temp0, &Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2], SNOVA_r, SNOVA_l);
#elif RCT_SIGN_JOG
				rct_sj_mm_add_pub(gf16m_temp0, &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp1, SNOVA_r, SNOVA_r, SNOVA_l);
#if RCT_SJ_M4
				rct_m4_mm_add(X_tmp, gf16m_temp0, &Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2], SNOVA_r, SNOVA_l, SNOVA_l);
#else
				rct_sj_mm_add(X_tmp, gf16m_temp0, &Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2], SNOVA_r, SNOVA_l, SNOVA_l);
#endif
#else
				gf_mat_mul_add_lr_sec(gf16m_temp0, &Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp1, SNOVA_r, SNOVA_r, SNOVA_l);
				gf_mat_mul_add_lr_sec(X_tmp, gf16m_temp0, &Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2], SNOVA_r, SNOVA_l, SNOVA_l);
#endif
#ifdef RCT_PROFILE
				_t1 = __rdtsc();
				RCT_PACC6B(0, _t0, _t1);
#endif
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
				__m128i bmcol[SNOVA_l];
				for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
					_Alignas(16) uint8_t cc[16] = {0};
					for (int tj2 = 0; tj2 < SNOVA_r; tj2++) {
						cc[tj2] = Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2];
					}
					bmcol[ti2] = _mm_load_si128((const __m128i *)cc);
				}
				for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
					for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
						__m128i b = bmcol[ti2];
						__m128i p0 = RCT_SV128(RCT_BC128_SEC(X_tmp[ti1 * SNOVA_l + 0]), b);
						__m128i p1 = RCT_SV128(RCT_BC128_SEC(X_tmp[ti1 * SNOVA_l + 1]), b);
						__m128i p2 = RCT_SV128(RCT_BC128_SEC(X_tmp[ti1 * SNOVA_l + 2]), b);
						__m128i p3 = RCT_SV128(RCT_BC128_SEC(X_tmp[ti1 * SNOVA_l + 3]), b);
						__m256i comb = _mm256_set_m128i(_mm_unpacklo_epi64(p2, p3), _mm_unpacklo_epi64(p0, p1));
						__m256i *g = (__m256i *)&gaussp[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx][0];
						*g = _mm256_xor_si256(*g, comb);
					}
#elif RCT_SJ_M4
				{
					_Alignas(32) uint16_t xe2[SNOVA_lr + 16];
					rct_m4_expand_buf(xe2, X_tmp, SNOVA_lr);
					for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
						_Alignas(16) uint8_t bmc[16] = {0};
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							bmc[tj2] = Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2];
						}
						__m128i bmv = _mm256_castsi256_si128(_mm256_cvtepu8_epi16(
						        _mm_load_si128((const __m128i *)bmc)));
						for (int ti1 = 0; ti1 < SNOVA_r; ti1++) {
							__m128i *gs = (__m128i *)gaussp16[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx];
							for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
								gs[tj1] = _mm_xor_si128(gs[tj1], _mm_mullo_epi16(
								                            _mm_set1_epi16((short)xe2[ti1 * SNOVA_l + tj1]), bmv));
						}
					}
				}
#elif RCT_SIGN_JOG
				{
					__m128i bcx[SNOVA_lr];
					for (int k = 0; k < SNOVA_lr; ++k) {
						bcx[k] = rct_sj_bc128(X_tmp[k]);
					}
					for (int ti2 = 0; ti2 < SNOVA_l; ti2++) {
						_Alignas(16) uint8_t bmc[16] = {0};
						for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
							bmc[tj2] = Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2];
						}
						__m128i bmv = _mm_load_si128((const __m128i *)bmc);
						for (int ti1 = 0; ti1 < SNOVA_r; ti1++) {
							__m128i *gs = (__m128i *)gaussp[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx];
							for (int tj1 = 0; tj1 < SNOVA_l; tj1++) {
								gs[tj1] = _mm_xor_si128(gs[tj1], rct_sj_sv128(bcx[ti1 * SNOVA_l + tj1], bmv));
							}
						}
					}
				}
#else
				for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
					for (int ti2 = 0; ti2 < SNOVA_l; ti2++)
						for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
							for (int tj2 = 0; tj2 < SNOVA_r; tj2++) {
								int ti = ti1 * SNOVA_l + ti2;
								int tj = tj1 * SNOVA_r + tj2;
								gf_set_add(&gauss[mi * SNOVA_lr + ti][idx * SNOVA_lr + tj],
								           gf_mult_sec(X_tmp[ti1 * SNOVA_l + tj1],
								                       Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2]));
							}
#endif
#ifdef RCT_PROFILE
				_t2 = __rdtsc();
				RCT_PACC6B(1, _t1, _t2);
#endif
			}

#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
#ifdef RCT_PROFILE
	uint64_t _tc0 = __rdtsc(), _tc1;
#endif
	for (int rr = 0; rr < SNOVA_o * SNOVA_lr; ++rr)
		for (int ix = 0; ix < SNOVA_o; ++ix)
			for (int tj1 = 0; tj1 < SNOVA_l; ++tj1)
				for (int tj2 = 0; tj2 < SNOVA_r; ++tj2)
					gauss[rr][ix * SNOVA_lr + tj1 * SNOVA_r + tj2] =
					    rct_gfni_cleanup(gaussp[rr][ix][tj1 * 8 + tj2]);
#ifdef RCT_PROFILE
	_tc1 = __rdtsc();
	RCT_PACC6B(2, _tc0, _tc1);
#endif
#undef RCT_GPW
#elif RCT_SJ_M4
#ifdef RCT_PROFILE
	uint64_t _tc0m = __rdtsc(), _tc1m;
#endif
	for (int rr = 0; rr < SNOVA_o * SNOVA_lr; ++rr)
		for (int ix = 0; ix < SNOVA_o; ++ix)
			for (int tj1 = 0; tj1 < SNOVA_l; ++tj1) {
				__m128i c16 = rct_m4_compress128(
				                  _mm_load_si128((const __m128i *)gaussp16[rr][ix] + tj1));
				_Alignas(16) uint8_t cb[16];
				_mm_store_si128((__m128i *)cb, _mm_packus_epi16(c16, _mm_setzero_si128()));
				for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
					gauss[rr][ix * SNOVA_lr + tj1 * SNOVA_r + tj2] = cb[tj2];
				}
			}
#ifdef RCT_PROFILE
	_tc1m = __rdtsc();
	RCT_PACC6B(2, _tc0m, _tc1m);
#endif
#undef RCT_GPW
#elif RCT_SIGN_JOG
#ifdef RCT_PROFILE
	uint64_t _tc0 = __rdtsc(), _tc1;
#endif
	for (int rr = 0; rr < SNOVA_o * SNOVA_lr; ++rr)
		for (int ix = 0; ix < SNOVA_o; ++ix)
			for (int tj1 = 0; tj1 < SNOVA_l; ++tj1) {
				_Alignas(16) uint8_t cb[16];
				_mm_store_si128((__m128i *)cb, RCT_SJ_CLEAN(
				                    _mm_load_si128((const __m128i *)&gaussp[rr][ix][tj1 * 16])));
				for (int tj2 = 0; tj2 < SNOVA_r; ++tj2) {
					gauss[rr][ix * SNOVA_lr + tj1 * SNOVA_r + tj2] = cb[tj2];
				}
			}
#ifdef RCT_PROFILE
	_tc1 = __rdtsc();
	RCT_PACC6B(2, _tc0, _tc1);
#endif
#undef RCT_GPW
#endif
#if RCT_HOT_SIMD && SNOVA_l == 4 && SNOVA_r <= 7
	SNOVA_CLEAR_OBJ(gaussp);
#elif RCT_SJ_M4
	SNOVA_CLEAR_OBJ(gaussp16);
#elif RCT_SIGN_JOG
	SNOVA_CLEAR_OBJ(gaussp);
#endif
#endif
}
#endif

#if !RCT_SIGN_JOG && !RCT_OQDF && !(RCT_USE_SIMD && SNOVA_r <= 16) && !RCT_OQWV
static void rct_sign_whipbuild_scalar(rct_sign_ctx *c) {
	const gf_t *signature_in_GF = c->signature_in_GF;
	gf_t *whipped_sig = c->whipped_sig;
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int j1 = 0; j1 < SNOVA_r; j1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						gf_set_add(&whipped_sig[(ab * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r + j1],
						           gf_mult_sec(rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1],
						                       signature_in_GF[ni * SNOVA_lr + k1 * SNOVA_r + j1]));
}
#endif

#if !RCT_SIGN_JOG && !RCT_USE_SIMD && !(RCT_Q_SIMD && (SNOVA_r == SNOVA_l)) && !(RCT_Q_SIMD && RCT_Q_HAVE_MAGIC)
static void rct_sign_sumt_scalar(rct_sign_ctx *c) {
	const gf_t *P11 = c->P11, *whipped_sig = c->whipped_sig;
	gf_t *sum_t1 = c->sum_t1;
	RCT_SCRATCH _Alignas(32) gf_t sum_t0[SNOVA_m1 * SNOVA_l * SNOVA_v * SNOVA_lr];
	memset(sum_t0, 0, sizeof(sum_t0));
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					gf_mat_mul_add_lr_sec(&sum_t0[((mi * SNOVA_l + b1) * SNOVA_v + ni) * SNOVA_lr],
					                      &P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2],
					                      &whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr], SNOVA_l, SNOVA_l, SNOVA_r);

		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								gf_set_add(&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1],
								           gf_mult_sec(whipped_sig[(a1 * SNOVA_v + ni) * SNOVA_lr + k1 * SNOVA_r + i1],
								                       sum_t0[((mi * SNOVA_l + b1) * SNOVA_v + ni) * SNOVA_lr + k1 * SNOVA_r + j1]));
	}
	SNOVA_CLEAR_OBJ(sum_t0);
}
#endif

#if !RCT_SIGN_JOG && !RCT_Q_SIMD && !(RCT_USE_GFNI && (SNOVA_l == 4)) && !(RCT_HOT_QRP16 && (SNOVA_l == 4)) \
    && !(RCT_USE_PSHUFB && (SNOVA_l == 4) && defined(RCT_MULLO) && (RCT_MULLO + 0))
static void rct_skx_fold_F_scalar(gf_t *F21, gf_t *F12, const gf_t *T12, const gf_t *P11) {
	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int j1 = 0; j1 < SNOVA_v; j1++)
			for (int j2 = 0; j2 < SNOVA_v; j2++)
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					RCT_MATMUL_ADD_ASEC(&F21[((i1 * SNOVA_o + k1) * SNOVA_v + j1) * SNOVA_l2],
					                    &T12[(j2 * SNOVA_o + k1) * SNOVA_l2],
					                    &P11[((i1 * SNOVA_v + j2) * SNOVA_v + j1) * SNOVA_l2]);

	for (int i1 = 0; i1 < SNOVA_m1; i1++)
		for (int j1 = 0; j1 < SNOVA_v; j1++)
			for (int j2 = 0; j2 < SNOVA_v; j2++)
				for (int k1 = 0; k1 < SNOVA_o; k1++)
					RCT_MATMUL_ADD_BSEC(&F12[((i1 * SNOVA_v + j1) * SNOVA_o + k1) * SNOVA_l2],
					                    &P11[((i1 * SNOVA_v + j1) * SNOVA_v + j2) * SNOVA_l2],
					                    &T12[(j2 * SNOVA_o + k1) * SNOVA_l2]);
}
#endif

#endif

static void rct_sk_expand(const uint8_t *sk, rct_skx_t *skx) {
	rct_init();
	uint64_t _pa, _pb;
	(void)_pa;
	(void)_pb;
	const uint8_t *seed = sk;
	memcpy(skx->sk, sk, BYTES_SK);
	memset(skx->T12 + SNOVA_o * SNOVA_v * SNOVA_l2, 0, 16);
	memset(skx->Am + SNOVA_o * SNOVA_alpha * SNOVA_r2, 0, 16);
	memset(skx->Bm + SNOVA_o * SNOVA_alpha * SNOVA_lr, 0, 16);
	memset(skx->Q2 + SNOVA_o * SNOVA_alpha * SNOVA_l2, 0, 16);

	expand_T12(skx->T12, seed + SEED_LENGTH_PUBLIC);
#if RCT_SIGN_STREAM || RCT_SKX_SLIM
	// gf_t *P_stream = (gf_t *)malloc((size_t)NUM_GEN_PUB_GF * sizeof(gf_t));
	if (!P_stream) {
		memset(skx, 0, sizeof(*skx));
		return;
	}
#if RCT_SIGN_STREAM
	snova_pgen_fill_pblocks(seed, P_stream);
#else
	expand_public(P_stream, seed);
#endif
	gf_t *T12 = skx->T12;
	gf_t *P11 = P_stream;
	gf_t *P12 = P_stream + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = P_stream + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;
#else
	expand_public(skx->P_matrix, seed);

	gf_t *T12 = skx->T12;
	gf_t *P11 = skx->P_matrix;
	gf_t *P12 = skx->P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = skx->P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;
#endif
	gf_t *F21 = skx->F21;
	gf_t *F12 = skx->F12;
	memset(F21, 0, sizeof(skx->F21));
	memset(F12, 0, sizeof(skx->F12));

	RCT_PT(_pa);
#if RCT_Q_SIMD
	rct_skx_fold_F_oddq(F21, F12, T12, P11, P12, P21);
#else
#if RCT_SIGN_JOG
	rct_skx_fold_F_jog(F21, F12, T12, P11);
#elif RCT_USE_GFNI && (SNOVA_l == 4)
	rct_skx_fold_F_gfni(F21, F12, T12, P11);
#elif RCT_USE_PSHUFB && (SNOVA_l == 4) && defined(RCT_MULLO) && (RCT_MULLO + 0)
	rct_skx_fold_F_mullo(F21, F12, T12, P11);
#elif RCT_HOT_QRP16 && (SNOVA_l == 4)
	rct_skx_fold_F_qrp16(F21, F12, T12, P11);
#else
	rct_skx_fold_F_scalar(F21, F12, T12, P11);
#endif

	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2; i1++) {
		gf_set_add(&F12[i1], P12[i1]);
	}
	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2; i1++) {
		gf_set_add(&F21[i1], P21[i1]);
	}
#endif
	RCT_PT(_pb);
	RCT_PACC(3, _pa, _pb);

#if RCT_SIGN_STREAM || RCT_SKX_SLIM
#if RCT_SKX_SLIM && !RCT_SIGN_STREAM
	memcpy(skx->P11, P_stream, (size_t)SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2 * sizeof(gf_t));
	memset(skx->P11 + (size_t)SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2, 0, 32 * sizeof(gf_t));
#endif
	// free(P_stream);
	gf_t *aptr = skx->abq;
#else
	gf_t *aptr = skx->P_matrix + SNOVA_m1 * (SNOVA_v * SNOVA_v + 2 * SNOVA_v * SNOVA_o) * SNOVA_l2;
#endif
#if FIXED_ABQ
	memcpy(aptr, rct_fixed_abq, sizeof(rct_fixed_abq));
	memcpy(skx->Am, rct_fixed_Am, sizeof(rct_fixed_Am));
	memcpy(skx->Bm, rct_fixed_Bm, sizeof(rct_fixed_Bm));
	memcpy(skx->Q1, rct_fixed_Q1, sizeof(rct_fixed_Q1));
	memcpy(skx->Q2, rct_fixed_Q2, sizeof(rct_fixed_Q2));
#else
	gen_ABQ(aptr, skx->Am, skx->Bm, skx->Q1, skx->Q2);
#endif
}

static int rct_sign_expanded(rct_skx_t *skx, uint8_t *sig, const uint8_t *digest,
                             const size_t len_digest, const uint8_t *salt) {
	rct_init();
	uint64_t _pa, _pb;
	(void)_pa;
	(void)_pb;
	const uint8_t *seed = skx->sk;

	gf_t *T12 = skx->T12;
#if RCT_SIGN_STREAM
	size_t p11_gf = (size_t)SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	// gf_t *P11 = (gf_t *)malloc((p11_gf + 32) * sizeof(gf_t));
	if (!P11) {
		memset(sig, 0, BYTES_SIGNATURE);
		return -1;
	}
	memset(P11 + p11_gf, 0, 32 * sizeof(gf_t));
	snova_pgen_fill_p11(seed, P11);
	gf_t *aptr = skx->abq;
#elif RCT_SKX_SLIM
	gf_t *P11 = skx->P11;
	gf_t *aptr = skx->abq;
#else
	gf_t *P11 = skx->P_matrix;
	gf_t *aptr = skx->P_matrix + SNOVA_m1 * (SNOVA_v * SNOVA_v + 2 * SNOVA_v * SNOVA_o) * SNOVA_l2;
#endif
	gf_t *F21 = skx->F21;
	gf_t *F12 = skx->F12;
	gf_t *Am = skx->Am;
	gf_t *Bm = skx->Bm;
	gf_t *Q1 = skx->Q1;
	gf_t *Q2 = skx->Q2;
	gf_t *q1 = aptr + SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr);
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;

	gf_t hash_in_GF16[GF16_HASH];
	uint8_t sign_hashb[BYTES_HASH];
#if HASH_PK
	hash_combined(sign_hashb, digest, len_digest, seed + SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE, salt);
#else
	hash_combined(sign_hashb, digest, len_digest, seed, salt);
#endif
	expand_gf(hash_in_GF16, sign_hashb, GF16_HASH);

	RCT_SCRATCH _Alignas(32) gf_t gauss[SNOVA_o * SNOVA_lr][SNOVA_o * SNOVA_lr + 1 + 64];
	gf_t solution[SNOVA_o * SNOVA_lr] = {0};
#if RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC
	enum { RCT_GN = SNOVA_o * SNOVA_lr, RCT_GPAD = (SNOVA_o * SNOVA_lr / 16 + 1) * 16 };
	RCT_SCRATCH _Alignas(32) uint16_t gu[RCT_GN][RCT_GPAD];
	_Alignas(32) uint16_t sol16[RCT_GPAD];
#endif
#if SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR)
	// enum { RCT_SNB = (SNOVA_o * SNOVA_lr + 1 + 31) / 32 };
	_Alignas(32) gf_t solpad[RCT_SNB * 32 + 32];
#endif
	RCT_SCRATCH _Alignas(32) gf_t signature_in_GF[SNOVA_n * SNOVA_lr + 32];
	memset(signature_in_GF, 0, sizeof(signature_in_GF));
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
	// enum { RCT_CM_LR16 = SNOVA_lr32 / 16,
	//        RCT_CM_OLR16 = (SNOVA_o * SNOVA_lr + 15) / 16,
	//        RCT_CM_OLR = RCT_CM_OLR16 * 16,
	//        RCT_CM_OLR32 = (RCT_CM_OLR + 31) / 32 * 32,
	//        RCT_CM_OLR32N = RCT_CM_OLR32 / 32 };
#if (SNOVA_r == SNOVA_l) && defined(RCT_SQ_CM_TIGHT) && (RCT_SQ_CM_TIGHT + 0)
	// enum { RCT_CMW = SNOVA_lr, RCT_CMW16 = SNOVA_lr / 16 };
#else
	// enum { RCT_CMW = SNOVA_lr32, RCT_CMW16 = SNOVA_lr32 / 16 };
#endif
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_whip[SNOVA_l * SNOVA_v * RCT_CMW];
#if RCT_USE_GFNI
	RCT_SCRATCH _Alignas(32) uint8_t rct_cm_whipb[SNOVA_l * SNOVA_v * RCT_CMW];
#endif
#endif
#if defined(RCT_CM_ACTIVE) || defined(RCT_CMS3_ONLY)
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_Amx[SNOVA_o * SNOVA_alpha * SNOVA_r2];
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_Bmx[SNOVA_o * SNOVA_alpha * SNOVA_lr];
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_Q1x[SNOVA_o * SNOVA_alpha * SNOVA_l2];
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_Q2x[SNOVA_o * SNOVA_alpha * SNOVA_l2];
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_q1x[SNOVA_o * SNOVA_alpha * SNOVA_l];
	RCT_SCRATCH _Alignas(32) uint16_t rct_cm_q2x[SNOVA_o * SNOVA_alpha * SNOVA_l];
	rct_cm_expand_arr(rct_cm_Amx, Am, SNOVA_o * SNOVA_alpha * SNOVA_r2);
	rct_cm_expand_arr(rct_cm_Bmx, Bm, SNOVA_o * SNOVA_alpha * SNOVA_lr);
	rct_cm_expand_arr(rct_cm_Q1x, Q1, SNOVA_o * SNOVA_alpha * SNOVA_l2);
	rct_cm_expand_arr(rct_cm_Q2x, Q2, SNOVA_o * SNOVA_alpha * SNOVA_l2);
	rct_cm_expand_arr(rct_cm_q1x, q1, SNOVA_o * SNOVA_alpha * SNOVA_l);
	rct_cm_expand_arr(rct_cm_q2x, q2, SNOVA_o * SNOVA_alpha * SNOVA_l);
#endif
#if RCT_OQDF
	rct_oq_expand_u16(rct_oq_P11u, P11, SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2);
	rct_oq_expand_u16(rct_oq_F21u, F21, SNOVA_m1 * SNOVA_o * SNOVA_v * SNOVA_l2);
	rct_oq_expand_u16(rct_oq_F12u, F12, SNOVA_m1 * SNOVA_v * SNOVA_o * SNOVA_l2);
#endif

#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
	rct_cm_ctx CM;
	CM.cm_whip = rct_cm_whip;
#if RCT_USE_GFNI
	CM.cm_whipb = rct_cm_whipb;
#else
	CM.cm_whipb = (uint8_t *)0;
#endif
#if defined(RCT_CM_ACTIVE) || defined(RCT_CMS3_ONLY)
	CM.Amx = rct_cm_Amx;
	CM.Bmx = rct_cm_Bmx;
	CM.Q1x = rct_cm_Q1x;
	CM.Q2x = rct_cm_Q2x;
	CM.q1x = rct_cm_q1x;
	CM.q2x = rct_cm_q2x;
#else
	CM.Amx = CM.Bmx = CM.Q1x = CM.Q2x = CM.q1x = CM.q2x = (const uint16_t *)0;
#endif
#endif
	rct_sign_ctx C;
	rct_sign_ctx *c = &C;
	c->skx = skx;
	c->T12 = T12;
	c->P11 = P11;
	c->aptr = aptr;
	c->F21 = F21;
	c->F12 = F12;
	c->Am = Am;
	c->Bm = Bm;
	c->Q1 = Q1;
	c->Q2 = Q2;
	c->q1 = q1;
	c->q2 = q2;
	c->signature_in_GF = signature_in_GF;
	c->hash_in_GF16 = hash_in_GF16;
	c->gauss = gauss;
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
	c->cm = &CM;
#else
	c->cm = (rct_cm_ctx *)0;
#endif
	c->_pa = c->_pb = 0;
	c->num_sign = 0;
	c->flag_redo = 0;
	(void)c;

	int flag_redo = 1;
	uint8_t num_sign = 0;

	SNOVA_DUDECT_RETRY_RESET();
	int sign_rc = 0;
	do {
		memset(gauss, 0, sizeof(gauss));
		num_sign++;
		SNOVA_DUDECT_RETRY_TICK();
		if (num_sign == 255) {
			memset(sig, 0, BYTES_SIGNATURE);
			sign_rc = -1;
			goto sign_cleanup;
		}
		// flag_redo = 0;

		uint8_t vinegar_in_byte[NUM_GEN_SEC_BYTES];
		shake_t v_instance;
		shake256_init(&v_instance);
		shake_absorb(&v_instance, seed + SEED_LENGTH_PUBLIC, SEED_LENGTH_PRIVATE);
		shake_absorb(&v_instance, digest, BYTES_DIGEST);
		shake_absorb(&v_instance, salt, BYTES_SALT);
		shake_absorb(&v_instance, &num_sign, 1);
		shake_finalize(&v_instance);
		shake_squeeze(vinegar_in_byte, NUM_GEN_SEC_BYTES, &v_instance);

		expand_gf(signature_in_GF, vinegar_in_byte, SNOVA_v * SNOVA_lr);

		gf_t Fvv_in_GF16Matrix[SNOVA_o * SNOVA_lr] = {0};
#if !RCT_OQDF
		RCT_SCRATCH _Alignas(32) gf_t sum_t1[SNOVA_m1 * SNOVA_l2 * SNOVA_r2 + 64];
		memset(sum_t1, 0, sizeof(sum_t1));
		RCT_SCRATCH _Alignas(32) gf_t whipped_sig[SNOVA_l * SNOVA_v * SNOVA_lr + 16];
		memset(whipped_sig, 0, sizeof(whipped_sig));
		c->sum_t1 = sum_t1;
		c->whipped_sig = whipped_sig;
#endif
		c->Fvv = Fvv_in_GF16Matrix;

		RCT_PT(_pa);
#if RCT_SIGN_JOG
		rct_sign_whipbuild_jog(c);
#elif RCT_OQDF
		rct_sign_whipbuild_oqdf(c);
#elif RCT_USE_SIMD && SNOVA_r <= 16
		rct_sign_whipbuild_q16(c);
#elif RCT_OQWV
		rct_sign_whipbuild_oqwv(c);
#else
		rct_sign_whipbuild_scalar(c);
#endif

#if RCT_SIGN_JOG
		rct_sign_sumt_jog(c);
#elif RCT_USE_SIMD
		rct_sign_s1whip_simd(c);
#elif RCT_Q_SIMD && (SNOVA_r == SNOVA_l)
		rct_sign_sumt_oddq_sq(c);
#elif RCT_Q_SIMD && RCT_Q_HAVE_MAGIC
		rct_sign_sumt_oddq_rect(c);
#else
		rct_sign_sumt_scalar(c);
#endif
		RCT_PT(_pb);
		RCT_PACC(4, _pa, _pb);
		RCT_PT(_pa);

#if defined(SNOVA_CT_CANARY) && (SNOVA_CT_CANARY + 0) == 3
#if !RCT_SIGN_JOG
#error "SNOVA_CT_CANARY=3 needs the RCT_SIGN_JOG arm (l=5 shapes); pick an l=5 parameter set"
#endif
		{
			volatile int ct_canary3_sink = 0;
			__m128i cb = rct_sj_bc128_pub(c->sum_t1[0]);
			ct_canary3_sink ^= _mm_cvtsi128_si32(cb);
			(void)ct_canary3_sink;
		}
#endif
#if RCT_SIGN_JOG
		rct_sign_fvv_jog(c);
#elif RCT_Q_SIMD && (SNOVA_r == SNOVA_l) && RCT_Q_HAVE_MAGIC && !defined(RCT_FVV_SCALAR)
		rct_sign_fvv_oddq_sq(c);
#elif RCT_Q_SIMD && (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC
		rct_sign_fvv_oddq_rect(c);
#elif defined(RCT_CMS2_ONLY)
		rct_sign_fvv_cms2(c);
#else
		rct_sign_fvv_std(c);
#endif

		RCT_PT(_pb);
		RCT_PACC(5, _pa, _pb);
		RCT_PT(_pa);
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int i1 = 0; i1 < SNOVA_lr; i1++)
				gauss[mi * SNOVA_lr + i1][SNOVA_o * SNOVA_lr] =
				    gf_sub(hash_in_GF16[mi * SNOVA_lr + i1], Fvv_in_GF16Matrix[mi * SNOVA_lr + i1]);

#ifndef RCT_CM_ACTIVE
		RCT_SCRATCH _Alignas(32) gf_t whipped_F21[SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr + 32];
		RCT_SCRATCH _Alignas(32) gf_t whipped_F12[SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr + 32];
		memset(whipped_F21, 0, sizeof(whipped_F21));
		memset(whipped_F12, 0, sizeof(whipped_F12));
		c->whipped_F21 = whipped_F21;
		c->whipped_F12 = whipped_F12;
#endif
#ifdef RCT_PROFILE
		uint64_t _s0 = __rdtsc(), _s1, _s2;
		RCT_PACC6(0, _pa, _s0);
#endif

#if RCT_SIGN_JOG
		rct_sign_wf_jog(c);
#elif defined(RCT_CM_ACTIVE) || defined(RCT_CMS3_ONLY)
		rct_sign_wF_gauss_cm(c);
#elif RCT_Q_SIMD && (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC
		rct_sign_wF_gauss_oddq_rect(c);
#elif RCT_Q_SIMD && (SNOVA_r == SNOVA_l)
		rct_sign_wf_oddq_sq(c);
#else
		rct_sign_wf_std(c);
#endif
#ifdef RCT_PROFILE
		_s1 = __rdtsc();
		RCT_PACC6(1, _s0, _s1);
#endif

#if defined(RCT_CM_ACTIVE) || defined(RCT_CMS3_ONLY)
#elif RCT_Q_SIMD && (SNOVA_r != SNOVA_l) && RCT_Q_HAVE_MAGIC
#elif RCT_Q_SIMD && (SNOVA_r == SNOVA_l)
		rct_sign_gauss_scatter_oddq_sq(c);
#else
		rct_sign_gauss_scatter_std(c);
#endif
#ifdef RCT_PROFILE
		_s2 = __rdtsc();
		RCT_PACC6(2, _s1, _s2);
#endif

		RCT_PT(_pb);
		RCT_PACC(6, _pa, _pb);
		RCT_PT(_pa);
#if RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR)
#if RCT_Q_HAVE_MAGIC
		flag_redo = rct_sign_gauss_oddq_magic(c, gu, sol16, solution);
#else
		flag_redo = rct_sign_gauss_oddq_nomagic(c);
#endif
#else
		flag_redo = rct_sign_gauss_q16(c);
#endif

		if (!flag_redo) {
#if SNOVA_Q == 16
#if defined(RCT_GAUSS_SCALAR)
#define RCT_SOLPAD_HAS_GS 1
#else
#define RCT_SOLPAD_HAS_GS 0
#endif
#if defined(RCT_T12_MULLO)
#define RCT_SOLPAD_HAS_TM 1
#else
#define RCT_SOLPAD_HAS_TM 0
#endif
#define RCT_SOLPAD_BACKSUB (RCT_GFMUL_ANY && !RCT_SOLPAD_HAS_GS)
#define RCT_SOLPAD_T12 ( \
    (RCT_HOT_QRP16 && !RCT_USE_GFNI && RCT_SOLPAD_HAS_TM && (RCT_T12_MULLO + 0) && !RCT_SOLPAD_HAS_GS && (SNOVA_L == 4)) || \
    ((RCT_USE_GFNI || RCT_HOT_QRP16) && !RCT_SOLPAD_HAS_GS && (SNOVA_L == 4)) || \
    (RCT_SIGN_JOG && RCT_GFMUL_ANY && !RCT_SOLPAD_HAS_GS) )
#if (RCT_SOLPAD_BACKSUB) != (RCT_SOLPAD_T12)
#error "solpad interlock broken: backsub SIMD selection != apply_t12 solpad selection (RCT_SOLPAD_BACKSUB/RCT_SOLPAD_T12 must be flipped together)"
#endif
#undef RCT_SOLPAD_T12
#endif
#if RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC
#elif SNOVA_Q == 16 && RCT_SOLPAD_BACKSUB
			rct_sign_backsub_gfni(c, solpad, solution);
#else
			rct_sign_backsub_scalar(c, solution);
#endif
#if SNOVA_Q == 16
#undef RCT_SOLPAD_BACKSUB
#endif
			memcpy(signature_in_GF + SNOVA_v * SNOVA_lr, solution, SNOVA_o * SNOVA_lr);
#if RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC
			rct_sign_apply_t12_oddq(c, sol16);
#elif SNOVA_Q == 16 && RCT_HOT_QRP16 && !RCT_USE_GFNI && defined(RCT_T12_MULLO) \
    && (RCT_T12_MULLO + 0) && !defined(RCT_GAUSS_SCALAR) && SNOVA_L == 4
			rct_sign_apply_t12_mullo(c, solpad);
#elif SNOVA_Q == 16 && (RCT_USE_GFNI || RCT_HOT_QRP16) && !defined(RCT_GAUSS_SCALAR) && SNOVA_L == 4
			rct_sign_apply_t12_gfni(c, solpad);
#elif RCT_SIGN_JOG && SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR)
			rct_sign_apply_t12_jog(c, solpad);
#else
			rct_sign_apply_t12_scalar(c, solution);
#endif
		}
		RCT_PT(_pb);
		RCT_PACC(7, _pa, _pb);
		SNOVA_CLEAR_OBJ(vinegar_in_byte);
#if !RCT_OQDF
		SNOVA_CLEAR_OBJ(sum_t1);
		SNOVA_CLEAR_OBJ(whipped_sig);
#endif
#ifndef RCT_CM_ACTIVE
		SNOVA_CLEAR_OBJ(whipped_F21);
		SNOVA_CLEAR_OBJ(whipped_F12);
#endif
	} while (flag_redo);
#ifdef RCT_PROFILE
	fprintf(stderr, "[SG] s0=%lu s1=%lu s2=%lu s3=%lu s4=%lu\n",
	        (unsigned long)rct_pf[3], (unsigned long)rct_pf[4], (unsigned long)rct_pf[5],
	        (unsigned long)rct_pf[6], (unsigned long)rct_pf[7]);
	fprintf(stderr, "[SG6] s0=%lu s1=%lu s2=%lu\n",
	        (unsigned long)rct_pf6[0], (unsigned long)rct_pf6[1], (unsigned long)rct_pf6[2]);
	fprintf(stderr, "[SG6b] s0=%lu s1=%lu s2=%lu\n",
	        (unsigned long)rct_pf6b[0], (unsigned long)rct_pf6b[1], (unsigned long)rct_pf6b[2]);
	rct_pf6b[0] = rct_pf6b[1] = rct_pf6b[2] = 0;
	rct_pf6[0] = rct_pf6[1] = rct_pf6[2] = 0;
	rct_pf[3] = rct_pf[4] = rct_pf[5] = rct_pf[6] = rct_pf[7] = 0;
#endif

	compress_gf(sig, signature_in_GF, SNOVA_n * SNOVA_lr);
	memcpy(sig + BYTES_SIGNATURE - BYTES_SALT, salt, BYTES_SALT);
sign_cleanup:
	SNOVA_CLEAR_OBJ(signature_in_GF);
	SNOVA_CLEAR_OBJ(gauss);
	SNOVA_CLEAR_OBJ(solution);
#if RCT_Q_SIMD && !defined(RCT_GAUSS_SCALAR) && RCT_Q_HAVE_MAGIC
	SNOVA_CLEAR_OBJ(gu);
	SNOVA_CLEAR_OBJ(sol16);
#endif
#if SNOVA_Q == 16 && RCT_GFMUL_ANY && !defined(RCT_GAUSS_SCALAR)
	SNOVA_CLEAR_OBJ(solpad);
#endif
#if defined(RCT_CM_ACTIVE) || defined(RCT_CML_ONLY) || defined(RCT_CMS3_ONLY)
	SNOVA_CLEAR_OBJ(rct_cm_whip);
#if RCT_USE_GFNI
	SNOVA_CLEAR_OBJ(rct_cm_whipb);
#endif
#endif
#if RCT_OQDF
	SNOVA_CLEAR_OBJ(rct_oq_whip_w);
	SNOVA_CLEAR_OBJ(rct_oq_sum_t1u);
	SNOVA_CLEAR_OBJ(rct_oq_F21u);
	SNOVA_CLEAR_OBJ(rct_oq_F12u);
#endif
#if RCT_SIGN_STREAM
	// free(P11);
#endif
	return sign_rc;
}

#endif

//#include "snova_rect_verify.h"
#ifndef SNOVA_RECT_VERIFY_H
#define SNOVA_RECT_VERIFY_H

typedef struct {
#if RCT_VERIFY_STREAM
	gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#elif RCT_JOG_PKXJOG
	gf_t P[SNOVA_m1 * RCT_JOG_NL * RCT_JOG_L32];
#else
	gf_t P[SNOVA_m1 * SNOVA_n * SNOVA_n * SNOVA_l2];
#endif
	gf_t Am[SNOVA_o * SNOVA_alpha * SNOVA_r2];
	gf_t Bm[SNOVA_o * SNOVA_alpha * SNOVA_lr];
	gf_t q1[SNOVA_o * SNOVA_alpha * SNOVA_l];
	gf_t q2[SNOVA_o * SNOVA_alpha * SNOVA_l];
	uint8_t pk_seed[SEED_LENGTH_PUBLIC];
#if HASH_PK
	uint8_t pk_hash[BYTES_PK_HASH];
#endif
} rct_pk_t;

#if FIXED_ABQ && ((SNOVA_q != 16) || ((SNOVA_l2 % 2) == 0)) && !RCT_JOG_PKXJOG
#define RCT_PKX_FUSED 1
#else
#define RCT_PKX_FUSED 0
#endif

typedef struct rct_pkx_ctx {
	rct_pk_t *pkx;
	const uint8_t *pk;
} rct_pkx_ctx;

typedef struct rct_vf_ctx {
	const rct_pk_t *pkx;
	const uint8_t *sig;
	gf_t *sig_gf;
	gf_t *hash_gf;
	gf_t *sum_t1;
	gf_t *sum_t1q;
	uint16_t *sum_t1s;
	uint8_t *whipped_sig2;
	uint8_t *sum_t1p;
#ifdef RCT_VPROF
	uint64_t *vpf;
	int *vpc;
	uint64_t va, vb;
#endif
} rct_vf_ctx;

// #include "platforms/generic/rct_pkx.h"
#ifndef RCT_PKX_H
#define RCT_PKX_H

#ifndef RCT_PKX_JOGSIMD
#define RCT_PKX_JOGSIMD 1
#endif
#if RCT_JOG_PKXJOG && RCT_PKX_JOGSIMD && (SNOVA_l >= 4) && (SNOVA_l <= 7)
#define RCT_PKX_JOG_SIMD 1
static inline void rct_pkx_jog_scatter(gf_t *P, size_t mi_row_base, int ni,
                                       size_t col0, const gf_t *src, int K) {
	gf_t *row[SNOVA_l];
	for (int ei = 0; ei < SNOVA_l; ++ei) {
		row[ei] = P + (mi_row_base + (size_t)ni * SNOVA_l + ei) * RCT_JOG_L32 + col0;
	}
	for (int nj = 0; nj + 1 < K; ++nj) {
		const gf_t *s = src + (size_t)nj * SNOVA_l2;
		for (int ei = 0; ei < SNOVA_l; ++ei) {
			memcpy(row[ei] + (size_t)nj * SNOVA_l, s + (size_t)ei * SNOVA_l, 8);
		}
	}
	const gf_t *s = src + (size_t)(K - 1) * SNOVA_l2;
	for (int ei = 0; ei < SNOVA_l; ++ei) {
		memcpy(row[ei] + (size_t)(K - 1) * SNOVA_l, s + (size_t)ei * SNOVA_l, SNOVA_l);
	}
}
#else
#define RCT_PKX_JOG_SIMD 0
#endif

static int rct_pkx_expand(rct_pkx_ctx *c) {
	rct_pk_t *pkx = c->pkx;
	const uint8_t *pk = c->pk;
#if RCT_VERIFY_STREAM
	memset(pkx, 0, sizeof(*pkx));
	memcpy(pkx->pk_seed, pk, SEED_LENGTH_PUBLIC);
#if HASH_PK
	shake256(pkx->pk_hash, BYTES_PK_HASH, pk, BYTES_PK);
#endif
	memcpy(pkx->Am, rct_fixed_Am, sizeof(pkx->Am));
	memcpy(pkx->Bm, rct_fixed_Bm, sizeof(pkx->Bm));
	memcpy(pkx->q1, rct_fixed_abq + (size_t)SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr),
	       SNOVA_o * SNOVA_alpha * SNOVA_l);
	memcpy(pkx->q2, rct_fixed_abq + (size_t)SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + SNOVA_l),
	       SNOVA_o * SNOVA_alpha * SNOVA_l);
#if SNOVA_q != 16
	if (expand_pk(pkx->P22, pk + SEED_LENGTH_PUBLIC)) {
		return -1;
	}
#else
	(void)expand_pk;
	rct_unpack_nib_seg(pkx->P22, pk + SEED_LENGTH_PUBLIC, NUMGF_PK / 2);
#endif
	return 0;
#elif RCT_PKX_FUSED
	memset((uint8_t *)pkx + offsetof(rct_pk_t, Am), 0, sizeof(*pkx) - offsetof(rct_pk_t, Am));
	memcpy(pkx->pk_seed, pk, SEED_LENGTH_PUBLIC);
#if HASH_PK
	shake256(pkx->pk_hash, BYTES_PK_HASH, pk, BYTES_PK);
#endif

	memcpy(pkx->Am, rct_fixed_Am, sizeof(pkx->Am));
	memcpy(pkx->Bm, rct_fixed_Bm, sizeof(pkx->Bm));
	memcpy(pkx->q1, rct_fixed_abq + (size_t)SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr),
	       SNOVA_o * SNOVA_alpha * SNOVA_l);
	memcpy(pkx->q2, rct_fixed_abq + (size_t)SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + SNOVA_l),
	       SNOVA_o * SNOVA_alpha * SNOVA_l);

#if !RCT_PKX_PGEN
#if SNOVA_WRAPPER_STACK
	_Alignas(32) uint8_t pub_bytes[((NUM_GEN_PUB_BYTES + 15) & ~(size_t)7u)];
#else
	_Alignas(8) static uint8_t pub_bytes[((NUM_GEN_PUB_BYTES + 15) & ~(size_t)7u)];
#endif
	rct_public_xof(pk, pub_bytes, NUM_GEN_PUB_BYTES);
#endif

#if SNOVA_q != 16
#if SNOVA_WRAPPER_STACK
	_Alignas(32) gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#else
	static gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#endif
	if (expand_pk(P22, pk + SEED_LENGTH_PUBLIC)) {
		return -1;
	}
#else
	(void)expand_pk;
#endif

	enum {
		RCT_OFF_P11 = 0,
		RCT_OFF_P12 = SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2,
		RCT_OFF_P21 = SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2,
	};
#if SNOVA_q == 16
#if !RCT_PKX_PGEN
#define RCT_PKX_SEG(dst, gfoff, ngf) \
    rct_unpack_nib_seg((dst), pub_bytes + ((size_t)(gfoff) >> 1), (size_t)(ngf) >> 1)
#endif
#define RCT_PKX_P22SEG(dst, gfoff, ngf) \
    rct_unpack_nib_seg((dst), pk + SEED_LENGTH_PUBLIC + ((size_t)(gfoff) >> 1), (size_t)(ngf) >> 1)
#else
#if !RCT_PKX_PGEN
#define RCT_PKX_SEG(dst, gfoff, ngf) rct_unpack_modq_seg((dst), pub_bytes + (size_t)(gfoff), (size_t)(ngf))
#endif
#define RCT_PKX_P22SEG(dst, gfoff, ngf) memcpy((dst), P22 + (size_t)(gfoff), (size_t)(ngf))
#endif
#if RCT_PKX_PGEN
	{
		snova_pgen_t pg;
		snova_pgen_init(&pg, pk);
		snova_prow_t rw;
		int blk, bmi, bni, bnc;
		(void)bnc;
		while (snova_pgen_peek(&pg, &blk, &bmi, &bni, &bnc)) {
			gf_t *dst;
			if (blk == 0) {
				dst = &pkx->P[((size_t)(bmi * SNOVA_n + bni) * SNOVA_n) * SNOVA_l2];
			} else if (blk == 1) {
				dst = &pkx->P[(((size_t)(bmi * SNOVA_n + bni) * SNOVA_n) + SNOVA_v) * SNOVA_l2];
			} else {
				dst = &pkx->P[((size_t)(bmi * SNOVA_n + SNOVA_v + bni) * SNOVA_n) * SNOVA_l2];
			}
			(void)snova_pgen_next_row_into(&pg, &rw, dst);
		}
	}
	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int ni = 0; ni < SNOVA_o; ++ni)
			RCT_PKX_P22SEG(&pkx->P[(((size_t)(mi * SNOVA_n + SNOVA_v + ni) * SNOVA_n) + SNOVA_v) * SNOVA_l2],
			               (size_t)(mi * SNOVA_o + ni) * SNOVA_o * SNOVA_l2, SNOVA_o * SNOVA_l2);
#else
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		for (int ni = 0; ni < SNOVA_v; ++ni) {
			gf_t *row = &pkx->P[((size_t)(mi * SNOVA_n + ni) * SNOVA_n) * SNOVA_l2];
			RCT_PKX_SEG(row, RCT_OFF_P11 + (size_t)(mi * SNOVA_v + ni) * SNOVA_v * SNOVA_l2,
			            SNOVA_v * SNOVA_l2);
			RCT_PKX_SEG(row + (size_t)SNOVA_v * SNOVA_l2,
			            RCT_OFF_P12 + (size_t)(mi * SNOVA_v + ni) * SNOVA_o * SNOVA_l2,
			            SNOVA_o * SNOVA_l2);
		}
		for (int ni = 0; ni < SNOVA_o; ++ni) {
			gf_t *row = &pkx->P[((size_t)(mi * SNOVA_n + SNOVA_v + ni) * SNOVA_n) * SNOVA_l2];
			RCT_PKX_SEG(row, RCT_OFF_P21 + (size_t)(mi * SNOVA_o + ni) * SNOVA_v * SNOVA_l2,
			            SNOVA_v * SNOVA_l2);
			RCT_PKX_P22SEG(row + (size_t)SNOVA_v * SNOVA_l2,
			               (size_t)(mi * SNOVA_o + ni) * SNOVA_o * SNOVA_l2, SNOVA_o * SNOVA_l2);
		}
	}
#undef RCT_PKX_SEG
#endif
#undef RCT_PKX_P22SEG
	return 0;
#else
	memset(pkx, 0, sizeof(*pkx));
	memcpy(pkx->pk_seed, pk, SEED_LENGTH_PUBLIC);
#if HASH_PK
	shake256(pkx->pk_hash, BYTES_PK_HASH, pk, BYTES_PK);
#endif

#if SNOVA_WRAPPER_STACK
	_Alignas(32) gf_t P_matrix[NUM_PUB_GF];
	_Alignas(32) gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#else
	gf_t *P_matrix = rct_pub_Pmatrix;
	static gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];
#endif
	gf_t *P11 = P_matrix;
	gf_t *P12 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;

	if (expand_pk(P22, pk + SEED_LENGTH_PUBLIC)) {
		return -1;
	}
	expand_public(P_matrix, pk);

#if RCT_PKX_JOG_SIMD
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		const size_t mrb = (size_t)mi * RCT_JOG_NL;
		for (int ni = 0; ni < SNOVA_v; ++ni) {
			rct_pkx_jog_scatter(pkx->P, mrb, ni, 0,
			                    P11 + ((size_t)(mi * SNOVA_v + ni) * SNOVA_v) * SNOVA_l2, SNOVA_v);
			rct_pkx_jog_scatter(pkx->P, mrb, ni, (size_t)SNOVA_v * SNOVA_l,
			                    P12 + ((size_t)(mi * SNOVA_v + ni) * SNOVA_o) * SNOVA_l2, SNOVA_o);
		}
		for (int ni = SNOVA_v; ni < SNOVA_n; ++ni) {
			const int nio = ni - SNOVA_v;
			rct_pkx_jog_scatter(pkx->P, mrb, ni, 0,
			                    P21 + ((size_t)(mi * SNOVA_o + nio) * SNOVA_v) * SNOVA_l2, SNOVA_v);
			rct_pkx_jog_scatter(pkx->P, mrb, ni, (size_t)SNOVA_v * SNOVA_l,
			                    P22 + ((size_t)(mi * SNOVA_o + nio) * SNOVA_o) * SNOVA_l2, SNOVA_o);
		}
	}
#else
#if RCT_JOG_PKXJOG
#define RCT_PKX_DST(mi, ni, nj, idx)                                              \
    pkx->P[(((size_t)(mi) * RCT_JOG_NL + (size_t)(ni) * SNOVA_l + (idx) / SNOVA_l) \
            * RCT_JOG_L32) + (size_t)(nj) * SNOVA_l + (idx) % SNOVA_l]
#else
#define RCT_PKX_DST(mi, ni, nj, idx) \
    pkx->P[(((mi) * SNOVA_n + (ni)) * SNOVA_n + (nj)) * SNOVA_l2 + (idx)]
#endif
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		for (int ni = 0; ni < SNOVA_v; ++ni) {
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int idx = 0; idx < SNOVA_l2; idx++)
					RCT_PKX_DST(mi, ni, nj, idx) =
					    P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2 + idx];
			for (int nj = SNOVA_v; nj < SNOVA_n; ++nj)
				for (int idx = 0; idx < SNOVA_l2; idx++)
					RCT_PKX_DST(mi, ni, nj, idx) =
					    P12[((mi * SNOVA_v + ni) * SNOVA_o + (nj - SNOVA_v)) * SNOVA_l2 + idx];
		}
		for (int ni = SNOVA_v; ni < SNOVA_n; ++ni) {
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int idx = 0; idx < SNOVA_l2; idx++)
					RCT_PKX_DST(mi, ni, nj, idx) =
					    P21[((mi * SNOVA_o + (ni - SNOVA_v)) * SNOVA_v + nj) * SNOVA_l2 + idx];
			for (int nj = SNOVA_v; nj < SNOVA_n; ++nj)
				for (int idx = 0; idx < SNOVA_l2; idx++)
					RCT_PKX_DST(mi, ni, nj, idx) =
					    P22[((mi * SNOVA_o + (ni - SNOVA_v)) * SNOVA_o + nj - SNOVA_v) * SNOVA_l2 + idx];
		}
	}
#undef RCT_PKX_DST
#endif

	gf_t *A = P_matrix + (SNOVA_m1 * (SNOVA_n * SNOVA_n - SNOVA_o * SNOVA_o)) * SNOVA_l2;
	gf_t *B = A + SNOVA_o * SNOVA_alpha * SNOVA_r2;
	gf_t *q1 = B + SNOVA_o * SNOVA_alpha * SNOVA_lr;
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;
#if FIXED_ABQ
	memcpy(A, rct_fixed_abq, sizeof(rct_fixed_abq));
#endif
	for (size_t idx = 0; idx < (size_t)SNOVA_o * SNOVA_alpha; idx++) {
		be_invertible_by_add_aS(&pkx->Am[idx * SNOVA_r2], &A[idx * SNOVA_r2], SNOVA_r, SNOVA_r);
		be_invertible_by_add_aS(&pkx->Bm[idx * SNOVA_lr], &B[idx * SNOVA_lr], SNOVA_r, SNOVA_l);
#if ROUND2_T12
		if (!q1[idx * SNOVA_l + SNOVA_l - 1]) {
			q1[idx * SNOVA_l + SNOVA_l - 1] = SNOVA_q - (q1[idx * SNOVA_l] + (q1[idx * SNOVA_l] == 0));
		}
		if (!q2[idx * SNOVA_l + SNOVA_l - 1]) {
			q2[idx * SNOVA_l + SNOVA_l - 1] = SNOVA_q - (q2[idx * SNOVA_l] + (q2[idx * SNOVA_l] == 0));
		}
#endif
	}
	memcpy(pkx->q1, q1, SNOVA_o * SNOVA_alpha * SNOVA_l);
	memcpy(pkx->q2, q2, SNOVA_o * SNOVA_alpha * SNOVA_l);
	return 0;
#endif
}

#endif

// #include "platforms/x86_avx2/rct_verify_avx2.h"
#ifndef RCT_VERIFY_AVX2_H
#define RCT_VERIFY_AVX2_H

#if RCT_USE_SIMD

static void rct_vf_whip_q16(rct_vf_ctx *c) {
	const gf_t *signature_in_GF = c->sig_gf;
	uint8_t *whipped_sig2 = c->whipped_sig2;
#if RCT_USE_GFNI && RCT_VF_MTK2
	for (int idx = 0; idx < SNOVA_n; ++idx) {
		const gf_t *sg = &signature_in_GF[idx * SNOVA_lr];
		__m256i b0 = _mm256_broadcastsi128_si256(_mm_loadu_si128((const __m128i *)sg));
		__m256i b1 = _mm256_broadcastsi128_si256(_mm_loadu_si128((const __m128i *)(sg + 2 * SNOVA_r)));
		__m256i R[SNOVA_l];
		for (int k1 = 0; k1 < SNOVA_l; k1++)
			R[k1] = _mm256_shuffle_epi8((k1 < 2) ? b0 : b1,
			                            _mm256_load_si256((const __m256i *)rct_whipR[k1]));
		for (int i1 = 0; i1 < SNOVA_l; i1++) {
			__m256i acc = _mm256_setzero_si256();
			for (int k1 = 0; k1 < SNOVA_l; k1++)
				acc = _mm256_xor_si256(acc, _mm256_gf2p8mul_epi8(
				                           _mm256_load_si256((const __m256i *)rct_whipM[i1 * SNOVA_l + k1]), R[k1]));
			_mm256_store_si256((__m256i *)&whipped_sig2[(idx * SNOVA_l + i1) * SNOVA_lr32],
			                   rct_gfni_cleanup256(acc));
		}
	}
#elif SNOVA_lr <= 32
	for (int idx = 0; idx < SNOVA_n; ++idx)
		for (int i1 = 0; i1 < SNOVA_l; i1++)
			for (int ab = 0; ab < SNOVA_l; ++ab) {
				__m256i acc = _mm256_setzero_si256();
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					__m256i sv = _mm256_loadu_si256((const __m256i *)&signature_in_GF[idx * SNOVA_lr + k1 * SNOVA_r]);
					__m256i sc = RCT_BC(rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1]);
					acc = _mm256_xor_si256(acc, RCT_SV(sc, sv));
				}
				_Alignas(32) uint8_t tmp[32];
				_mm256_store_si256((__m256i *)tmp, rct_gfni_cleanup256(acc));
				for (int j1 = 0; j1 < SNOVA_r; j1++) {
					whipped_sig2[idx * SNOVA_l * SNOVA_lr32 + i1 * SNOVA_lr32 + ab * SNOVA_r + j1] = tmp[j1];
				}
			}
#else
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int idx = 0; idx < SNOVA_n; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int j1 = 0; j1 < SNOVA_r; j1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						gf_set_add(&whipped_sig2[idx * SNOVA_l * SNOVA_lr32 + i1 * SNOVA_lr32 + ab * SNOVA_r + j1],
						           gf_mult(rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1],
						                   signature_in_GF[idx * SNOVA_lr + k1 * SNOVA_r + j1]));
#endif
}

#if !RCT_VF_JOG
static void rct_vf_reindex_q16(rct_vf_ctx *c) {
	uint8_t *sum_t1p = c->sum_t1p;
#if RCT_VF_EMM
	gf_t *sum_t1q = c->sum_t1q;
#else
	gf_t *sum_t1 = c->sum_t1;
#endif
#if !RCT_VF_MTK2
	for (int i = 0; i < SNOVA_m1 * SNOVA_l * SNOVA_r * SNOVA_lr32; ++i) {
		sum_t1p[i] = rct_gfni_cleanup(sum_t1p[i]);
	}
#endif
#if RCT_VF_EMM
	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				rct_vf_tr8(&sum_t1q[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * 64],
				           &sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + b1 * SNOVA_r],
				           SNOVA_lr32);
#else
	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int i1 = 0; i1 < SNOVA_r; i1++)
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1] =
						    sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + i1 * SNOVA_lr32 + b1 * SNOVA_r + j1];
#endif
}

#endif

#if !RCT_VF_AQ && !RCT_VF_JOG
static void rct_vf_contract_nonaq(rct_vf_ctx *c1) {
	const rct_pk_t *pkx = c1->pkx;
	uint8_t *whipped_sig2 = c1->whipped_sig2;
	uint8_t *sum_t1p = c1->sum_t1p;
#if RCT_VF_MTK2
	static uint16_t rct_vf_ppair[SNOVA_m1 * SNOVA_n * SNOVA_n * 8];
	RCT_SCRATCH _Alignas(32) uint16_t rct_vf_wpair[SNOVA_n * SNOVA_l * 16];
	{
		const __m256i ppl = _mm256_load_si256((const __m256i *)RCT_VF_PPL);
		const __m256i pph = _mm256_load_si256((const __m256i *)RCT_VF_PPH);
		const int ncell = SNOVA_m1 * SNOVA_n * SNOVA_n;
		int c = 0;
		for (; c + 2 <= ncell; c += 2)
			_mm256_storeu_si256(
			    (__m256i *)&rct_vf_ppair[c * 8],
			    _mm256_slli_epi16(
			        _mm256_cvtepu8_epi16(rct_vf_pack_pair32(
			                                 _mm256_loadu_si256((const __m256i *)&pkx->P[c * SNOVA_l2]), ppl, pph)),
			        4));
		if (c < ncell)
			_mm_storeu_si128(
			    (__m128i *)&rct_vf_ppair[c * 8],
			    _mm256_castsi256_si128(_mm256_slli_epi16(
			                               _mm256_cvtepu8_epi16(rct_vf_pack_pair32(
			                                       _mm256_zextsi128_si256(
			                                               _mm_loadu_si128((const __m128i *)&pkx->P[c * SNOVA_l2])),
			                                       ppl, pph)),
			                               4)));
		const __m256i wpl = _mm256_load_si256((const __m256i *)RCT_VF_WPL);
		const __m256i wph = _mm256_load_si256((const __m256i *)RCT_VF_WPH);
		for (int rw = 0; rw < SNOVA_n * SNOVA_l; ++rw)
			_mm256_store_si256(
			    (__m256i *)&rct_vf_wpair[rw * 16],
			    _mm256_slli_epi16(
			        _mm256_cvtepu8_epi16(rct_vf_pack_pair32(
			                                 _mm256_load_si256((const __m256i *)&whipped_sig2[rw * SNOVA_lr32]),
			                                 wpl, wph)),
			        4));
	}
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		RCT_SCRATCH _Alignas(32) uint8_t sum_t0[SNOVA_n * SNOVA_l * SNOVA_lr32];
		for (int ni = 0; ni < SNOVA_n; ++ni) {
			__m256i acc01 = _mm256_setzero_si256(), acc23 = _mm256_setzero_si256();
			const uint16_t *pp = &rct_vf_ppair[(mi * SNOVA_n + ni) * SNOVA_n * 8];
			for (int nj = 0; nj < SNOVA_n; ++nj)
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					__m256i wp = _mm256_load_si256(
					                 (const __m256i *)&whipped_sig2[(nj * SNOVA_l + k1) * SNOVA_lr32]);
					uint32_t ii;
					memcpy(&ii, &pp[nj * 8 + k1 * 2], 4);
					acc01 = _mm256_xor_si256(
					            acc01, _mm256_shuffle_epi8(rct_mtk2t16((uint16_t)ii), wp));
					acc23 = _mm256_xor_si256(
					            acc23, _mm256_shuffle_epi8(rct_mtk2t16((uint16_t)(ii >> 16)), wp));
				}
			uint8_t *s0 = &sum_t0[ni * SNOVA_l * SNOVA_lr32];
			_mm256_store_si256((__m256i *)(s0 + 0 * SNOVA_lr32), rct_nib_lo(acc01));
			_mm256_store_si256((__m256i *)(s0 + 1 * SNOVA_lr32), rct_nib_hi(acc01));
			_mm256_store_si256((__m256i *)(s0 + 2 * SNOVA_lr32), rct_nib_lo(acc23));
			_mm256_store_si256((__m256i *)(s0 + 3 * SNOVA_lr32), rct_nib_hi(acc23));
		}
		for (int h = 0; h < 2; ++h) {
			__m256i acc[SNOVA_r];
			for (int p = 0; p < SNOVA_r; p++) {
				acc[p] = _mm256_setzero_si256();
			}
			for (int ni = 0; ni < SNOVA_n; ++ni)
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					__m256i s0 = _mm256_load_si256(
					                 (const __m256i *)&sum_t0[(ni * SNOVA_l + k1) * SNOVA_lr32]);
					const uint16_t *wq = &rct_vf_wpair[(ni * SNOVA_l + k1) * 16 + h * SNOVA_r];
					uint64_t w0, w1;
					memcpy(&w0, wq, 8);
					memcpy(&w1, wq + 4, 8);
					for (int p = 0; p < SNOVA_r; p++) {
						uint16_t idx = (uint16_t)((p < 4 ? (w0 >> (16 * p))
						                           : (w1 >> (16 * (p - 4)))));
						acc[p] = _mm256_xor_si256(acc[p],
						                          _mm256_shuffle_epi8(rct_mtk2t16(idx), s0));
					}
				}
			for (int p = 0; p < SNOVA_r; p++) {
				int j0 = h * 2 * SNOVA_r + 2 * p;
				_mm256_store_si256(
				    (__m256i *)&sum_t1p[(mi * SNOVA_l * SNOVA_r + j0) * SNOVA_lr32],
				    rct_nib_lo(acc[p]));
				_mm256_store_si256(
				    (__m256i *)&sum_t1p[(mi * SNOVA_l * SNOVA_r + j0 + 1) * SNOVA_lr32],
				    rct_nib_hi(acc[p]));
			}
		}
	}
#elif SNOVA_lr16 == 1
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		RCT_SCRATCH _Alignas(32) uint8_t sum_t0[SNOVA_n * SNOVA_l * SNOVA_lr32];
		for (int ni = 0; ni < SNOVA_n; ++ni) {
			__m256i acc[SNOVA_l];
			for (int i1 = 0; i1 < SNOVA_l; i1++) {
				acc[i1] = _mm256_setzero_si256();
			}
			for (int nj = 0; nj < SNOVA_n; ++nj)
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					__m256i wp = _mm256_load_si256(
					                 (const __m256i *)&whipped_sig2[(nj * SNOVA_l + k1) * SNOVA_lr32]);
					const gf_t *prow =
					    &pkx->P[((mi * SNOVA_n + ni) * SNOVA_n + nj) * SNOVA_l2 + k1];
					for (int i1 = 0; i1 < SNOVA_l; i1++) {
						acc[i1] = _mm256_xor_si256(acc[i1], RCT_SV(RCT_BC(prow[i1 * SNOVA_l]), wp));
					}
				}
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				_mm256_store_si256((__m256i *)&sum_t0[(ni * SNOVA_l + i1) * SNOVA_lr32],
				                   rct_gfni_cleanup256(acc[i1]));
		}
		for (int a1 = 0; a1 < SNOVA_l; ++a1) {
			__m256i acc[SNOVA_r];
			for (int i1 = 0; i1 < SNOVA_r; i1++) {
				acc[i1] = _mm256_setzero_si256();
			}
			for (int ni = 0; ni < SNOVA_n; ++ni)
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					__m256i s0 = _mm256_load_si256(
					                 (const __m256i *)&sum_t0[(ni * SNOVA_l + k1) * SNOVA_lr32]);
					const gf_t *wrow =
					    &whipped_sig2[(ni * SNOVA_l + k1) * SNOVA_lr32 + a1 * SNOVA_r];
					for (int i1 = 0; i1 < SNOVA_r; i1++) {
						acc[i1] = _mm256_xor_si256(acc[i1], RCT_SV(RCT_BC(wrow[i1]), s0));
					}
				}
			for (int i1 = 0; i1 < SNOVA_r; i1++)
				_mm256_store_si256(
				    (__m256i *)&sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + i1 * SNOVA_lr32],
				    acc[i1]);
		}
	}
#else
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		RCT_SCRATCH _Alignas(32) uint8_t sum_t0[SNOVA_n * SNOVA_l * SNOVA_lr32];
		memset(sum_t0, 0, sizeof(sum_t0));
		for (int ni = 0; ni < SNOVA_n; ++ni)
			for (int nj = 0; nj < SNOVA_n; ++nj)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int b1 = 0; b1 < SNOVA_lr16; ++b1) {
							__m256i *s0 = (__m256i *)&sum_t0[(ni * SNOVA_l + i1) * SNOVA_lr32];
							__m256i q1v = RCT_BC(
							                  pkx->P[((mi * SNOVA_n + ni) * SNOVA_n + nj) * SNOVA_l2 + i1 * SNOVA_l + k1]);
							__m256i *wp = (__m256i *)&whipped_sig2[nj * SNOVA_l * SNOVA_lr32 + k1 * SNOVA_lr32];
							s0[b1] = _mm256_xor_si256(s0[b1], RCT_SV(q1v, wp[b1]));
						}
		for (int i = 0; i < SNOVA_n * SNOVA_l * SNOVA_lr32; ++i) {
			sum_t0[i] = rct_gfni_cleanup(sum_t0[i]);
		}
		for (int ni = 0; ni < SNOVA_n; ++ni)
			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int b1 = 0; b1 < SNOVA_lr16; ++b1) {
							__m256i *s1 = (__m256i *)&sum_t1p[(mi * SNOVA_l + a1) * SNOVA_r * SNOVA_lr32 + i1 * SNOVA_lr32];
							__m256i wp = RCT_BC(
							                 whipped_sig2[ni * SNOVA_l * SNOVA_lr32 + k1 * SNOVA_lr32 + a1 * SNOVA_r + i1]);
							__m256i *s0 = (__m256i *)&sum_t0[(ni * SNOVA_l + k1) * SNOVA_lr32];
							s1[b1] = _mm256_xor_si256(s1[b1], RCT_SV(wp, s0[b1]));
						}
	}
#endif
}
#endif

#endif

#endif

// #include "platforms/x86_avx2/rct_verify_aq.h"
#ifndef RCT_VERIFY_AQ_H
#define RCT_VERIFY_AQ_H

#if RCT_VF_AQ

static void rct_vf_contract_aq(rct_vf_ctx *c) {
	const rct_pk_t *pkx = c->pkx;
	uint8_t *whipped_sig2 = c->whipped_sig2;
	uint8_t *sum_t1p = c->sum_t1p;
#define RCT_VF_N4 (((SNOVA_n + 3) / 4) * 4)
	RCT_SCRATCH _Alignas(32) uint8_t rct_vf_wpk[RCT_VF_N4 * 2 * 32];
	memset(rct_vf_wpk, 0, sizeof(rct_vf_wpk));
	RCT_SCRATCH _Alignas(32) uint8_t rct_vf_lqt[SNOVA_n * 2 * 128];
	for (int nj = 0; nj < SNOVA_n; ++nj) {
		__m256i r0 = _mm256_load_si256((const __m256i *)&whipped_sig2[(nj * 4 + 0) * SNOVA_lr32]);
		__m256i r1 = _mm256_load_si256((const __m256i *)&whipped_sig2[(nj * 4 + 1) * SNOVA_lr32]);
		__m256i r2 = _mm256_load_si256((const __m256i *)&whipped_sig2[(nj * 4 + 2) * SNOVA_lr32]);
		__m256i r3 = _mm256_load_si256((const __m256i *)&whipped_sig2[(nj * 4 + 3) * SNOVA_lr32]);
		__m256i p0 = _mm256_or_si256(r0, _mm256_slli_epi16(r1, 4));
		__m256i p1 = _mm256_or_si256(r2, _mm256_slli_epi16(r3, 4));
		_mm256_store_si256((__m256i *)&rct_vf_wpk[(nj * 2 + 0) * 32], p0);
		_mm256_store_si256((__m256i *)&rct_vf_wpk[(nj * 2 + 1) * 32], p1);
		__m256i q[4];
		rct_vf_aq_tree(_mm256_shuffle_epi8(p0, rct_vf_aqlo), q);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 0) * 128 + 0], q[0]);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 0) * 128 + 32], q[1]);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 0) * 128 + 64], q[2]);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 0) * 128 + 96], q[3]);
		rct_vf_aq_tree(_mm256_shuffle_epi8(p1, rct_vf_aqlo), q);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 1) * 128 + 0], q[0]);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 1) * 128 + 32], q[1]);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 1) * 128 + 64], q[2]);
		_mm256_store_si256((__m256i *)&rct_vf_lqt[(nj * 2 + 1) * 128 + 96], q[3]);
	}
#if RCT_VERIFY_STREAM
	RCT_SCRATCH _Alignas(32) uint8_t vf_st0p_all[SNOVA_m1 * SNOVA_n * 2 * 32];
	memset(vf_st0p_all, 0, sizeof(vf_st0p_all));
	{
		snova_pgen_t pg;
		snova_pgen_init(&pg, pkx->pk_seed);
		snova_prow_t rw;
		_Alignas(32) uint8_t segbuf[RCT_VF_N4 * SNOVA_l2];
		_Alignas(32) uint8_t qrow[(RCT_VF_N4 / 4) * 128];
		int blk, bmi, bni, bnc;
		(void)bmi;
		(void)bni;
		(void)bnc;
		while (snova_pgen_peek(&pg, &blk, &bmi, &bni, &bnc)) {
			const int col0 = (blk == 1) ? SNOVA_v : 0;
			const int qa = col0 & ~3;
			const int pre = col0 - qa;
			(void)snova_pgen_next_row_into(&pg, &rw,
			                               (gf_t *)(segbuf + (size_t)pre * SNOVA_l2));
			const int mi = rw.mi;
			const int ni = (rw.block == 2) ? SNOVA_v + rw.ni : rw.ni;
			int ncols = rw.ncols;
			if (rw.block == 2) {
				memcpy(segbuf + (size_t)SNOVA_v * SNOVA_l2,
				       &pkx->P22[((size_t)mi * SNOVA_o + rw.ni) * SNOVA_o * SNOVA_l2],
				       (size_t)SNOVA_o * SNOVA_l2);
				ncols = SNOVA_n;
			}
			const int nq = (pre + ncols + 3) >> 2;
			memset(segbuf, 0, (size_t)pre * SNOVA_l2);
			memset(segbuf + (size_t)(pre + ncols) * SNOVA_l2, 0,
			       (size_t)(nq * 4 - pre - ncols) * SNOVA_l2);
			for (int q4 = 0; q4 < nq; ++q4) {
				rct_vf_aq_quad(segbuf + q4 * 64, qrow + q4 * 128);
			}
			__m256i a01a = _mm256_setzero_si256(), a23a = _mm256_setzero_si256();
			__m256i a01b = _mm256_setzero_si256(), a23b = _mm256_setzero_si256();
			for (int njq = 0; njq < nq * 4; njq += 4) {
				const uint8_t *qb = qrow + (njq >> 2) * 128;
				const uint8_t *wb = &rct_vf_wpk[(qa + njq) * 64];
				for (int c = 0; c < 4; c += 2) {
					const int cb = (c >> 1) * 64;
					__m256i wv0 = _mm256_load_si256((const __m256i *)(wb + c * 64));
					__m256i wv1 = _mm256_load_si256((const __m256i *)(wb + c * 64 + 32));
					__m256i xv0 = _mm256_load_si256((const __m256i *)(wb + c * 64 + 64));
					__m256i xv1 = _mm256_load_si256((const __m256i *)(wb + c * 64 + 96));
					a01a = _mm256_xor_si256(a01a, _mm256_gf2p8affine_epi64_epi8(
					                            wv0, rct_vf_aq_bq(qb, cb + 0), 0));
					a23a = _mm256_xor_si256(a23a, _mm256_gf2p8affine_epi64_epi8(
					                            wv0, rct_vf_aq_bq(qb, cb + 8), 0));
					a01a = _mm256_xor_si256(a01a, _mm256_gf2p8affine_epi64_epi8(
					                            wv1, rct_vf_aq_bq(qb, cb + 32), 0));
					a23a = _mm256_xor_si256(a23a, _mm256_gf2p8affine_epi64_epi8(
					                            wv1, rct_vf_aq_bq(qb, cb + 40), 0));
					a01b = _mm256_xor_si256(a01b, _mm256_gf2p8affine_epi64_epi8(
					                            xv0, rct_vf_aq_bq(qb, cb + 16), 0));
					a23b = _mm256_xor_si256(a23b, _mm256_gf2p8affine_epi64_epi8(
					                            xv0, rct_vf_aq_bq(qb, cb + 24), 0));
					a01b = _mm256_xor_si256(a01b, _mm256_gf2p8affine_epi64_epi8(
					                            xv1, rct_vf_aq_bq(qb, cb + 48), 0));
					a23b = _mm256_xor_si256(a23b, _mm256_gf2p8affine_epi64_epi8(
					                            xv1, rct_vf_aq_bq(qb, cb + 56), 0));
				}
			}
			uint8_t *sp = &vf_st0p_all[((size_t)mi * SNOVA_n + ni) * 64];
			_mm256_store_si256((__m256i *)sp,
			                   _mm256_xor_si256(_mm256_load_si256((const __m256i *)sp),
			                                    _mm256_xor_si256(a01a, a01b)));
			_mm256_store_si256((__m256i *)(sp + 32),
			                   _mm256_xor_si256(_mm256_load_si256((const __m256i *)(sp + 32)),
			                                    _mm256_xor_si256(a23a, a23b)));
			if (rw.block == 2 && rw.ni == SNOVA_o - 1) {
				for (int h = 0; h < 2; ++h) {
					__m256i acc[SNOVA_r];
					for (int p = 0; p < SNOVA_r; p++) {
						acc[p] = _mm256_setzero_si256();
					}
					for (int nn = 0; nn < SNOVA_n; ++nn)
						for (int kp = 0; kp < 2; ++kp) {
							__m256i s0 = _mm256_load_si256((const __m256i *)
							                               &vf_st0p_all[(((size_t)mi * SNOVA_n + nn) * 2 + kp) * 32]);
							const uint8_t *lb = &rct_vf_lqt[(nn * 2 + kp) * 128];
							for (int p = 0; p < SNOVA_r; p++)
								acc[p] = _mm256_xor_si256(acc[p], _mm256_gf2p8affine_epi64_epi8(
								                              s0, rct_vf_aq_bq(lb, RCT_VF_AQ_LOFF(h * SNOVA_r + p)), 0));
						}
					for (int p = 0; p < SNOVA_r; p++) {
						int j0 = h * 2 * SNOVA_r + 2 * p;
						_mm256_store_si256(
						    (__m256i *)&sum_t1p[(mi * SNOVA_l * SNOVA_r + j0) * SNOVA_lr32],
						    rct_nib_lo(acc[p]));
						_mm256_store_si256(
						    (__m256i *)&sum_t1p[(mi * SNOVA_l * SNOVA_r + j0 + 1) * SNOVA_lr32],
						    rct_nib_hi(acc[p]));
					}
				}
			}
		}
	}
#else
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		RCT_SCRATCH _Alignas(32) uint8_t rct_vf_st0p[SNOVA_n * 2 * 32];
		_Alignas(32) uint8_t qrow[(RCT_VF_N4 / 4) * 128];
		for (int ni = 0; ni < SNOVA_n; ++ni) {
			const uint8_t *prow = &pkx->P[(size_t)(mi * SNOVA_n + ni) * SNOVA_n * SNOVA_l2];
			for (int q4 = 0; q4 < RCT_VF_N4 / 4; ++q4) {
				rct_vf_aq_quad(prow + q4 * 64, qrow + q4 * 128);
			}
			__m256i a01a = _mm256_setzero_si256(), a23a = _mm256_setzero_si256();
			__m256i a01b = _mm256_setzero_si256(), a23b = _mm256_setzero_si256();
			for (int njq = 0; njq < RCT_VF_N4; njq += 4) {
				const uint8_t *qb = qrow + (njq >> 2) * 128;
				const uint8_t *wb = &rct_vf_wpk[njq * 64];
				for (int c1 = 0; c1 < 4; c1 += 2) {
					const int cb = (c1 >> 1) * 64;
					__m256i wv0 = _mm256_load_si256((const __m256i *)(wb + c1 * 64));
					__m256i wv1 = _mm256_load_si256((const __m256i *)(wb + c1 * 64 + 32));
					__m256i xv0 = _mm256_load_si256((const __m256i *)(wb + c1 * 64 + 64));
					__m256i xv1 = _mm256_load_si256((const __m256i *)(wb + c1 * 64 + 96));
					a01a = _mm256_xor_si256(a01a, _mm256_gf2p8affine_epi64_epi8(
					                            wv0, rct_vf_aq_bq(qb, cb + 0), 0));
					a23a = _mm256_xor_si256(a23a, _mm256_gf2p8affine_epi64_epi8(
					                            wv0, rct_vf_aq_bq(qb, cb + 8), 0));
					a01a = _mm256_xor_si256(a01a, _mm256_gf2p8affine_epi64_epi8(
					                            wv1, rct_vf_aq_bq(qb, cb + 32), 0));
					a23a = _mm256_xor_si256(a23a, _mm256_gf2p8affine_epi64_epi8(
					                            wv1, rct_vf_aq_bq(qb, cb + 40), 0));
					a01b = _mm256_xor_si256(a01b, _mm256_gf2p8affine_epi64_epi8(
					                            xv0, rct_vf_aq_bq(qb, cb + 16), 0));
					a23b = _mm256_xor_si256(a23b, _mm256_gf2p8affine_epi64_epi8(
					                            xv0, rct_vf_aq_bq(qb, cb + 24), 0));
					a01b = _mm256_xor_si256(a01b, _mm256_gf2p8affine_epi64_epi8(
					                            xv1, rct_vf_aq_bq(qb, cb + 48), 0));
					a23b = _mm256_xor_si256(a23b, _mm256_gf2p8affine_epi64_epi8(
					                            xv1, rct_vf_aq_bq(qb, cb + 56), 0));
				}
			}
			_mm256_store_si256((__m256i *)&rct_vf_st0p[(ni * 2 + 0) * 32],
			                   _mm256_xor_si256(a01a, a01b));
			_mm256_store_si256((__m256i *)&rct_vf_st0p[(ni * 2 + 1) * 32],
			                   _mm256_xor_si256(a23a, a23b));
		}
		for (int h = 0; h < 2; ++h) {
			__m256i acc[SNOVA_r];
			for (int p = 0; p < SNOVA_r; p++) {
				acc[p] = _mm256_setzero_si256();
			}
			for (int ni = 0; ni < SNOVA_n; ++ni)
				for (int kp = 0; kp < 2; ++kp) {
					__m256i s0 = _mm256_load_si256(
					                 (const __m256i *)&rct_vf_st0p[(ni * 2 + kp) * 32]);
					const uint8_t *lb = &rct_vf_lqt[(ni * 2 + kp) * 128];
					for (int p = 0; p < SNOVA_r; p++)
						acc[p] = _mm256_xor_si256(acc[p], _mm256_gf2p8affine_epi64_epi8(
						                              s0, rct_vf_aq_bq(lb, RCT_VF_AQ_LOFF(h * SNOVA_r + p)), 0));
				}
			for (int p = 0; p < SNOVA_r; p++) {
				int j0 = h * 2 * SNOVA_r + 2 * p;
				_mm256_store_si256(
				    (__m256i *)&sum_t1p[(mi * SNOVA_l * SNOVA_r + j0) * SNOVA_lr32],
				    rct_nib_lo(acc[p]));
				_mm256_store_si256(
				    (__m256i *)&sum_t1p[(mi * SNOVA_l * SNOVA_r + j0 + 1) * SNOVA_lr32],
				    rct_nib_hi(acc[p]));
			}
		}
	}
#endif
#if defined(RCT_AQ_SELFTEST) && !RCT_VERIFY_STREAM
	{
		static int aq_st_done = 0;
		if (!aq_st_done) {
			aq_st_done = 1;
			int bad = 0;
			RCT_SCRATCH gf_t st0r[SNOVA_n * SNOVA_l][SNOVA_lr];
			RCT_SCRATCH gf_t st1r[SNOVA_l * SNOVA_r][SNOVA_lr];
			for (int mi = 0; mi < SNOVA_m1; ++mi) {
				memset(st0r, 0, sizeof(st0r));
				memset(st1r, 0, sizeof(st1r));
				for (int ni = 0; ni < SNOVA_n; ++ni)
					for (int nj = 0; nj < SNOVA_n; ++nj)
						for (int i1 = 0; i1 < SNOVA_l; ++i1)
							for (int k1 = 0; k1 < SNOVA_l; ++k1) {
								gf_t s = pkx->P[((size_t)(mi * SNOVA_n + ni) * SNOVA_n + nj) * SNOVA_l2 + i1 * SNOVA_l + k1];
								for (int b = 0; b < SNOVA_lr; ++b)
									st0r[ni * SNOVA_l + i1][b] ^=
									    rct_multtab[s * SNOVA_q + whipped_sig2[(nj * SNOVA_l + k1) * SNOVA_lr32 + b]];
							}
				for (int ni = 0; ni < SNOVA_n; ++ni)
					for (int k1 = 0; k1 < SNOVA_l; ++k1)
						for (int j = 0; j < SNOVA_l * SNOVA_r; ++j) {
							gf_t s = whipped_sig2[(ni * SNOVA_l + k1) * SNOVA_lr32 + j];
							for (int b = 0; b < SNOVA_lr; ++b) {
								st1r[j][b] ^= rct_multtab[s * SNOVA_q + st0r[ni * SNOVA_l + k1][b]];
							}
						}
				for (int j = 0; j < SNOVA_l * SNOVA_r; ++j)
					for (int b = 0; b < SNOVA_lr; ++b)
						if (st1r[j][b] != sum_t1p[(mi * SNOVA_l * SNOVA_r + j) * SNOVA_lr32 + b]) {
							bad++;
						}
			}
			fprintf(stderr, "[AQ-SELFTEST] mismatches=%d\n", bad);
		}
	}
#endif
}

#endif

#endif

// #include "platforms/x86_avx2/rct_verify_jog.h"
#ifndef RCT_VERIFY_JOG_H
#define RCT_VERIFY_JOG_H

#if RCT_VF_JOG

static inline __m256i rct_jog_sv256(gf_t s, __m256i v) {
	return _mm256_shuffle_epi8(rct_mtk2t((uint8_t)s), v);
}
static inline __m128i rct_jog_sv128(gf_t s, __m128i v) {
	return _mm_shuffle_epi8(_mm_load_si128((const __m128i *)rct_mtk2[(uint8_t)s]), v);
}

static void rct_vf_jog(rct_vf_ctx *c) {
#if RCT_USE_SIMD
	const uint8_t *W2 = c->whipped_sig2;
#define RCT_JOG_W(ab, ni, row, col) \
    (W2[(((size_t)(ni) * SNOVA_l + (row)) * SNOVA_lr32) + (size_t)(ab) * SNOVA_r + (col)])
#else
	RCT_SCRATCH _Alignas(32) gf_t jog_whip[SNOVA_l * SNOVA_n * SNOVA_lr];
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int idx = 0; idx < SNOVA_n; ++idx) {
			const gf_t *sig = &c->sig_gf[(size_t)idx * SNOVA_lr];
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				const gf_t *Srow = &rct_S[ab * SNOVA_l2 + i1 * SNOVA_l];
				__m128i acc = _mm_setzero_si128();
				for (int k1 = 0; k1 < SNOVA_l; ++k1)
					acc = _mm_xor_si128(acc,
					                    rct_jog_sv128(Srow[k1],
					                                  _mm_loadu_si128((const __m128i *)&sig[k1 * SNOVA_r])));
				_Alignas(16) uint8_t wb[16];
				_mm_store_si128((__m128i *)wb, acc);
				memcpy(&jog_whip[((size_t)ab * SNOVA_n + idx) * SNOVA_lr + i1 * SNOVA_r],
				       wb, SNOVA_r);
			}
		}
#define RCT_JOG_W(ab, ni, row, col) \
    (jog_whip[(((size_t)(ab) * SNOVA_n + (ni)) * SNOVA_lr) + (size_t)(row) * SNOVA_r + (col)])
#endif

	RCT_SCRATCH _Alignas(32) uint16_t jog_wpair[SNOVA_l][RCT_JOG_RP][RCT_JOG_NL];
	RCT_SCRATCH _Alignas(32) uint8_t jog_WALL[RCT_JOG_NL][SNOVA_lr32];
	memset(jog_WALL, 0, sizeof(jog_WALL));
	for (int a1 = 0; a1 < SNOVA_l; ++a1)
		for (int ni = 0; ni < SNOVA_n; ++ni)
			for (int k1 = 0; k1 < SNOVA_l; ++k1) {
				const int p = ni * SNOVA_l + k1;
				for (int i1p = 0; i1p < RCT_JOG_RP; ++i1p) {
					const unsigned lo = RCT_JOG_W(a1, ni, k1, 2 * i1p);
					const unsigned hi =
					    (2 * i1p + 1 < SNOVA_r) ? RCT_JOG_W(a1, ni, k1, 2 * i1p + 1) : 0u;
					jog_wpair[a1][i1p][p] = (uint16_t)(((hi << 4) | lo) << 4);
				}
			}
	for (int nj = 0; nj < SNOVA_n; ++nj)
		for (int ej = 0; ej < SNOVA_l; ++ej) {
			const int cc = nj * SNOVA_l + ej;
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int j1 = 0; j1 < SNOVA_r; ++j1) {
					jog_WALL[cc][b1 * SNOVA_r + j1] = (uint8_t)RCT_JOG_W(b1, nj, ej, j1);
				}
		}

#if !RCT_JOG_PKXJOG
	RCT_SCRATCH _Alignas(32) uint8_t jog_PJ[RCT_JOG_NL][RCT_JOG_L32];
	memset(jog_PJ, 0, sizeof(jog_PJ));
#endif
	RCT_SCRATCH _Alignas(32) uint8_t jog_tall[SNOVA_l][RCT_JOG_RP][RCT_JOG_L32];
	memset(jog_tall, 0, sizeof(jog_tall));

	gf_t *sum_t1 = c->sum_t1;
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
#if RCT_JOG_PKXJOG
		const uint8_t *PJ = &c->pkx->P[(size_t)mi * RCT_JOG_NL * RCT_JOG_L32];
#define RCT_JOG_PROW(p) (PJ + (size_t)(p) * RCT_JOG_L32)
#else
		{
			const __m256i perm = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
			for (int ni = 0; ni < SNOVA_n; ++ni) {
				const gf_t *crow =
				    &c->pkx->P[(((size_t)mi * SNOVA_n + ni) * SNOVA_n) * SNOVA_l2];
				uint8_t *prow = jog_PJ[ni * SNOVA_l];
				int nj = 0;
				for (; nj + 8 <= SNOVA_n; nj += 8) {
					const uint8_t *src = crow + (size_t)nj * SNOVA_l2;
					__m256i A = _mm256_loadu_si256((const __m256i *)(src + 0));
					__m256i B = _mm256_loadu_si256((const __m256i *)(src + 32));
					__m256i C = _mm256_loadu_si256((const __m256i *)(src + 64));
					__m256i D = _mm256_loadu_si256((const __m256i *)(src + 96));
					__m256i t0 = _mm256_unpacklo_epi32(A, B);
					__m256i t1 = _mm256_unpackhi_epi32(A, B);
					__m256i t2 = _mm256_unpacklo_epi32(C, D);
					__m256i t3 = _mm256_unpackhi_epi32(C, D);
					__m256i u0 = _mm256_unpacklo_epi64(t0, t2);
					__m256i u1 = _mm256_unpackhi_epi64(t0, t2);
					__m256i u2 = _mm256_unpacklo_epi64(t1, t3);
					__m256i u3 = _mm256_unpackhi_epi64(t1, t3);
					_mm256_storeu_si256((__m256i *)(prow + 0 * RCT_JOG_L32 + nj * 4),
					                    _mm256_permutevar8x32_epi32(u0, perm));
					_mm256_storeu_si256((__m256i *)(prow + 1 * RCT_JOG_L32 + nj * 4),
					                    _mm256_permutevar8x32_epi32(u1, perm));
					_mm256_storeu_si256((__m256i *)(prow + 2 * RCT_JOG_L32 + nj * 4),
					                    _mm256_permutevar8x32_epi32(u2, perm));
					_mm256_storeu_si256((__m256i *)(prow + 3 * RCT_JOG_L32 + nj * 4),
					                    _mm256_permutevar8x32_epi32(u3, perm));
				}
				for (; nj < SNOVA_n; ++nj) {
					const gf_t *cell = crow + (size_t)nj * SNOVA_l2;
					for (int ei = 0; ei < SNOVA_l; ++ei)
						memcpy(prow + (size_t)ei * RCT_JOG_L32 + (size_t)nj * SNOVA_l,
						       cell + (size_t)ei * SNOVA_l, SNOVA_l);
				}
			}
		}
#define RCT_JOG_PROW(p) (jog_PJ[(p)])
#endif
#define RCT_JOG_BG ((RCT_JOG_VTL <= 5) ? RCT_JOG_VTL : 4)
		for (int vb = 0; vb < RCT_JOG_VTL; vb += RCT_JOG_BG) {
			const int gw = (vb + RCT_JOG_BG <= RCT_JOG_VTL) ? RCT_JOG_BG : (RCT_JOG_VTL - vb);
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				int i1p = 0;
				for (; i1p + 2 <= RCT_JOG_RP; i1p += 2) {
					__m256i acc0[RCT_JOG_BG], acc1[RCT_JOG_BG];
					for (int g = 0; g < gw; ++g) {
						acc0[g] = _mm256_setzero_si256();
						acc1[g] = _mm256_setzero_si256();
					}
					const uint16_t *wp0 = jog_wpair[a1][i1p];
					const uint16_t *wp1 = jog_wpair[a1][i1p + 1];
					for (int p = 0; p < RCT_JOG_NL; ++p) {
						const __m256i tb0 = rct_mtk2t16(wp0[p]);
						const __m256i tb1 = rct_mtk2t16(wp1[p]);
						const uint8_t *pr = RCT_JOG_PROW(p) + (size_t)vb * 32;
						for (int g = 0; g < gw; ++g) {
							const __m256i pv =
							    _mm256_loadu_si256((const __m256i *)(pr + g * 32));
							acc0[g] = _mm256_xor_si256(acc0[g], _mm256_shuffle_epi8(tb0, pv));
							acc1[g] = _mm256_xor_si256(acc1[g], _mm256_shuffle_epi8(tb1, pv));
						}
					}
					for (int g = 0; g < gw; ++g) {
						_mm256_store_si256(
						    (__m256i *)&jog_tall[a1][i1p][(vb + g) * 32], acc0[g]);
						_mm256_store_si256(
						    (__m256i *)&jog_tall[a1][i1p + 1][(vb + g) * 32], acc1[g]);
					}
				}
				for (; i1p < RCT_JOG_RP; ++i1p) {
					__m256i acc[RCT_JOG_BG];
					for (int g = 0; g < gw; ++g) {
						acc[g] = _mm256_setzero_si256();
					}
					const uint16_t *wp = jog_wpair[a1][i1p];
					for (int p = 0; p < RCT_JOG_NL; ++p) {
						const __m256i tbl = rct_mtk2t16(wp[p]);
						const uint8_t *pr = RCT_JOG_PROW(p) + (size_t)vb * 32;
						for (int g = 0; g < gw; ++g)
							acc[g] = _mm256_xor_si256(
							             acc[g], _mm256_shuffle_epi8(
							                 tbl, _mm256_loadu_si256((const __m256i *)(pr + g * 32))));
					}
					for (int g = 0; g < gw; ++g) {
						_mm256_store_si256((__m256i *)&jog_tall[a1][i1p][(vb + g) * 32], acc[g]);
					}
				}
			}
		}
#undef RCT_JOG_BG
		for (int a1 = 0; a1 < SNOVA_l; ++a1) {
			__m256i accB[RCT_JOG_RP][SNOVA_lr16];
			for (int i1p = 0; i1p < RCT_JOG_RP; ++i1p)
				for (int w = 0; w < SNOVA_lr16; ++w) {
					accB[i1p][w] = _mm256_setzero_si256();
				}
			for (int cc = 0; cc < RCT_JOG_NL; ++cc) {
				__m256i wall[SNOVA_lr16];
				for (int w = 0; w < SNOVA_lr16; ++w) {
					wall[w] = _mm256_load_si256((const __m256i *)&jog_WALL[cc][w * 32]);
				}
				for (int i1p = 0; i1p < RCT_JOG_RP; ++i1p) {
					const __m256i tbl = rct_mtk2t(jog_tall[a1][i1p][cc]);
					for (int w = 0; w < SNOVA_lr16; ++w)
						accB[i1p][w] =
						    _mm256_xor_si256(accB[i1p][w], _mm256_shuffle_epi8(tbl, wall[w]));
				}
			}
			gf_t *st1_a = &sum_t1[((size_t)mi * SNOVA_l2 + (size_t)a1 * SNOVA_l) * SNOVA_r2];
			for (int i1p = 0; i1p < RCT_JOG_RP; ++i1p) {
				_Alignas(32) uint8_t rowlo[SNOVA_lr32], rowhi[SNOVA_lr32];
				for (int w = 0; w < SNOVA_lr16; ++w) {
					_mm256_store_si256((__m256i *)&rowlo[w * 32], rct_nib_lo(accB[i1p][w]));
					_mm256_store_si256((__m256i *)&rowhi[w * 32], rct_nib_hi(accB[i1p][w]));
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					gf_t *st1 = st1_a + (size_t)b1 * SNOVA_r2;
					for (int j1 = 0; j1 < SNOVA_r; ++j1) {
						st1[(2 * i1p) * SNOVA_r + j1] = rowlo[b1 * SNOVA_r + j1];
						if (2 * i1p + 1 < SNOVA_r) {
							st1[(2 * i1p + 1) * SNOVA_r + j1] = rowhi[b1 * SNOVA_r + j1];
						}
					}
				}
			}
		}
#undef RCT_JOG_PROW
	}
#undef RCT_JOG_W
}

_Static_assert(SNOVA_r2 <= 64, "rct_vf_emat_jog: temp1[64]/2-ymm q1q2 fold assumes r2<=64");
static void rct_vf_emat_jog(rct_vf_ctx *c) {
	const rct_pk_t *pkx = c->pkx;
	const gf_t *sum_t1 = c->sum_t1;
	gf_t *hash_in_GF = c->hash_gf;
	for (int mi = 0; mi < SNOVA_o; ++mi) {
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			const int mi_prime = i_prime(mi, alpha);
			const gf_t *q1 = &pkx->q1[(mi * SNOVA_alpha + alpha) * SNOVA_l];
			const gf_t *q2 = &pkx->q2[(mi * SNOVA_alpha + alpha) * SNOVA_l];
			_Alignas(32) uint8_t temp1[64];
			__m256i t1lo = _mm256_setzero_si256(), t1hi = _mm256_setzero_si256();
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				__m256i slo = _mm256_setzero_si256(), shi = _mm256_setzero_si256();
				for (int b1 = 0; b1 < SNOVA_l; ++b1) {
					const gf_t *base =
					    &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2];
					slo = _mm256_xor_si256(slo,
					                       rct_jog_sv256(q2[b1], _mm256_loadu_si256((const __m256i *)base)));
					shi = _mm256_xor_si256(shi,
					                       rct_jog_sv256(q2[b1], _mm256_loadu_si256((const __m256i *)(base + 32))));
				}
				t1lo = _mm256_xor_si256(t1lo, rct_jog_sv256(q1[a1], slo));
				t1hi = _mm256_xor_si256(t1hi, rct_jog_sv256(q1[a1], shi));
			}
			_mm256_store_si256((__m256i *)temp1, t1lo);
			_mm256_store_si256((__m256i *)(temp1 + 32), t1hi);
			const gf_t *Bm = &pkx->Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr];
			const gf_t *Am = &pkx->Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2];
			_Alignas(16) uint8_t temp2[SNOVA_r * 16];
			for (int i1 = 0; i1 < SNOVA_r; ++i1) {
				__m128i acc = _mm_setzero_si128();
				for (int k1 = 0; k1 < SNOVA_r; ++k1)
					acc = _mm_xor_si128(acc,
					                    rct_jog_sv128(temp1[i1 * SNOVA_r + k1],
					                                  _mm_loadu_si128((const __m128i *)&Bm[k1 * SNOVA_l])));
				_mm_store_si128((__m128i *)&temp2[i1 * 16], acc);
			}
			gf_t *hrow = &hash_in_GF[mi * SNOVA_lr];
			for (int i1 = 0; i1 < SNOVA_r; ++i1) {
				_Alignas(16) uint8_t hb[16];
				__m128i acc = _mm_setzero_si128();
				for (int k1 = 0; k1 < SNOVA_r; ++k1)
					acc = _mm_xor_si128(acc,
					                    rct_jog_sv128(Am[i1 * SNOVA_r + k1],
					                                  _mm_load_si128((const __m128i *)&temp2[k1 * 16])));
				_mm_store_si128((__m128i *)hb, acc);
				for (int j = 0; j < SNOVA_l; ++j) {
					hrow[i1 * SNOVA_l + j] = (gf_t)(hrow[i1 * SNOVA_l + j] ^ hb[j]);
				}
			}
		}
	}
}

#endif

#endif

// #include "platforms/x86_avx2/rct_verify_tile4.h"
#ifndef RCT_VERIFY_TILE4_H
#define RCT_VERIFY_TILE4_H

#if RCT_VF_TILE4

#ifdef RCT_T4_SELFTEST
#include <stdio.h>
#endif

#define RCT_T4_NB ((RCT_JOG_NL + 3) / 4)
#define RCT_T4_RP4 (RCT_T4_NB * 4)
#define RCT_T4_NQ ((RCT_JOG_NL + 15) / 16)
#define RCT_T4_CP (RCT_T4_NQ * 16)
#define RCT_T4_NCB (RCT_T4_NQ * 4)
#define RCT_T4_NP ((SNOVA_lr + 1) / 2)
#define RCT_T4_LH SNOVA_lr16
_Static_assert(RCT_T4_CP <= RCT_JOG_L32, "rct_vf_tile4: column pad must sit inside jogress zero pad");
_Static_assert(SNOVA_lr <= 64, "rct_vf_tile4: lane axis assumes lr <= 64 (RCT_T4_LH <= 2 lr32 halves)");
_Static_assert(RCT_T4_NP <= 32, "rct_vf_tile4: Left dispatch + dual-LOFF-block addressing cover <=32 pairs");
_Static_assert(!RCT_USE_SIMD, "rct_vf_tile4: whip_flat copies the l!=4 in-kernel whip branch");

static const _Alignas(32) uint8_t rct_t4_zrow[RCT_JOG_L32] = {0};

static __m256i rct_t4_thv[4];
static __m256i rct_t4_aqko;
static __m256i rct_t4_aqlo;
static __m256i rct_t4_madp;
static void rct_build_t4(void) {
	for (int t = 0; t < 4; ++t) {
		uint8_t colb[8];
		for (int j = 0; j < 8; ++j) {
			uint8_t v = (uint8_t)(1u << (j & 3));
			uint8_t rh = 0;
			for (int k = 0; k < 4; ++k) {
				rh |= (uint8_t)(((rct_multtab[v * SNOVA_q + (1u << k)] >> t) & 1) << k);
			}
			colb[j] = (j < 4) ? rh : (uint8_t)(rh << 4);
		}
		uint64_t qw = 0;
		for (int i = 0; i < 8; ++i) {
			uint8_t row = 0;
			for (int j = 0; j < 8; ++j) {
				row |= (uint8_t)(((colb[j] >> i) & 1) << j);
			}
			qw |= (uint64_t)row << (8 * (7 - i));
		}
		rct_t4_thv[t] = _mm256_set1_epi64x((long long)qw);
	}
	_Alignas(32) uint8_t ko[32], lo[32];
	static const uint8_t ord8[8] = {2, 0, 6, 4, 3, 1, 7, 5};
	for (int b = 0; b < 32; ++b) {
		ko[b] = (uint8_t)((b & 8) | ord8[b & 7]);
		lo[b] = (uint8_t)((b & 16) | ((b & 15) ^ 1));
	}
	rct_t4_aqko = _mm256_load_si256((const __m256i *)ko);
	rct_t4_aqlo = _mm256_load_si256((const __m256i *)lo);
	rct_t4_madp = _mm256_set1_epi16(0x1001);
}
static inline void rct_t4_tree(__m256i keys, __m256i out[4]) {
	__m256i R0 = _mm256_gf2p8affine_epi64_epi8(keys, rct_t4_thv[0], 0);
	__m256i R1 = _mm256_gf2p8affine_epi64_epi8(keys, rct_t4_thv[1], 0);
	__m256i R2 = _mm256_gf2p8affine_epi64_epi8(keys, rct_t4_thv[2], 0);
	__m256i R3 = _mm256_gf2p8affine_epi64_epi8(keys, rct_t4_thv[3], 0);
	__m256i u32lo = _mm256_unpacklo_epi8(R3, R2);
	__m256i u10lo = _mm256_unpacklo_epi8(R1, R0);
	__m256i u32hi = _mm256_unpackhi_epi8(R3, R2);
	__m256i u10hi = _mm256_unpackhi_epi8(R1, R0);
	out[0] = _mm256_unpacklo_epi16(u32lo, u10lo);
	out[1] = _mm256_unpackhi_epi16(u32lo, u10lo);
	out[2] = _mm256_unpacklo_epi16(u32hi, u10hi);
	out[3] = _mm256_unpackhi_epi16(u32hi, u10hi);
}
static inline void rct_t4_quad(const uint8_t *const rp[4], int coff, uint8_t *dst) {
	__m128i r0 = _mm_loadu_si128((const __m128i *)(rp[0] + coff));
	__m128i r1 = _mm_loadu_si128((const __m128i *)(rp[1] + coff));
	__m128i r2 = _mm_loadu_si128((const __m128i *)(rp[2] + coff));
	__m128i r3 = _mm_loadu_si128((const __m128i *)(rp[3] + coff));
	__m128i t0 = _mm_unpacklo_epi32(r0, r1), t1 = _mm_unpackhi_epi32(r0, r1);
	__m128i t2 = _mm_unpacklo_epi32(r2, r3), t3 = _mm_unpackhi_epi32(r2, r3);
	__m256i c01 = _mm256_set_m128i(_mm_unpackhi_epi64(t0, t2), _mm_unpacklo_epi64(t0, t2));
	__m256i c23 = _mm256_set_m128i(_mm_unpackhi_epi64(t1, t3), _mm_unpacklo_epi64(t1, t3));
	__m256i k01 = _mm256_maddubs_epi16(c01, rct_t4_madp);
	__m256i k23 = _mm256_maddubs_epi16(c23, rct_t4_madp);
	__m256i keys = _mm256_shuffle_epi8(_mm256_packus_epi16(k01, k23), rct_t4_aqko);
	__m256i q[4];
	rct_t4_tree(keys, q);
	_mm256_store_si256((__m256i *)(dst + 0), q[0]);
	_mm256_store_si256((__m256i *)(dst + 32), q[1]);
	_mm256_store_si256((__m256i *)(dst + 64), q[2]);
	_mm256_store_si256((__m256i *)(dst + 96), q[3]);
}
#define RCT_T4_LOFF(p) (32 * (((p) & 7) >> 1) + 16 * ((p) >> 3) + 8 * ((p) & 1))
#define RCT_T4_LQOFF(p) (((p) >> 4) * 128 + RCT_T4_LOFF((p) & 15))
static inline __m256i rct_t4_bq(const uint8_t *base, int off) {
	int64_t w;
	memcpy(&w, base + off, 8);
	return _mm256_set1_epi64x(w);
}

static inline __attribute__((always_inline)) void rct_t4_left(
    const uint8_t *st0p_t, const uint8_t *t4_lqt, gf_t *st1_mi,
    const int g0, const int gw) {
#if RCT_T4_LH == 1
	__m256i acc[5];
	for (int pi = 0; pi < gw; ++pi) {
		acc[pi] = _mm256_setzero_si256();
	}
	for (int tb = 0; tb < RCT_T4_NB; ++tb)
		for (int kp = 0; kp < 2; ++kp) {
			const __m256i s0 =
			    _mm256_load_si256((const __m256i *)&st0p_t[((size_t)tb * 2 + kp) * 32]);
			const uint8_t *lb = &t4_lqt[((size_t)tb * 2 + kp) * 128];
			for (int pi = 0; pi < gw; ++pi)
				acc[pi] = _mm256_xor_si256(acc[pi], _mm256_gf2p8affine_epi64_epi8(
				                               s0, rct_t4_bq(lb, RCT_T4_LOFF(g0 + pi)), 0));
		}
	for (int pi = 0; pi < gw; ++pi) {
		_Alignas(32) uint8_t rowlo[32], rowhi[32];
		_mm256_store_si256((__m256i *)rowlo, rct_nib_lo(acc[pi]));
		_mm256_store_si256((__m256i *)rowhi, rct_nib_hi(acc[pi]));
#else
	__m256i acc[5][RCT_T4_LH];
	for (int pi = 0; pi < gw; ++pi)
		for (int h = 0; h < RCT_T4_LH; ++h) {
			acc[pi][h] = _mm256_setzero_si256();
		}
	for (int tb = 0; tb < RCT_T4_NB; ++tb)
		for (int kp = 0; kp < 2; ++kp) {
			__m256i s0[RCT_T4_LH];
			for (int h = 0; h < RCT_T4_LH; ++h)
				s0[h] = _mm256_load_si256((const __m256i *)
				                          &st0p_t[(((size_t)tb * 2 + kp) * RCT_T4_LH + h) * 32]);
			const uint8_t *lb = &t4_lqt[((size_t)tb * 2 + kp) * (RCT_T4_LH * 128)];
			for (int pi = 0; pi < gw; ++pi) {
				const __m256i mq = rct_t4_bq(lb, RCT_T4_LQOFF(g0 + pi));
				for (int h = 0; h < RCT_T4_LH; ++h)
					acc[pi][h] = _mm256_xor_si256(acc[pi][h],
					                              _mm256_gf2p8affine_epi64_epi8(s0[h], mq, 0));
			}
		}
	for (int pi = 0; pi < gw; ++pi) {
		_Alignas(32) uint8_t rowlo[SNOVA_lr32], rowhi[SNOVA_lr32];
		for (int h = 0; h < RCT_T4_LH; ++h) {
			_mm256_store_si256((__m256i *)(rowlo + h * 32), rct_nib_lo(acc[pi][h]));
			_mm256_store_si256((__m256i *)(rowhi + h * 32), rct_nib_hi(acc[pi][h]));
		}
#endif
		for (int half = 0; half < 2; ++half) {
			const int jrow = 2 * (g0 + pi) + half;
			if (jrow >= SNOVA_lr) {
				break;
			}
			const int a1 = jrow / SNOVA_r, i1 = jrow % SNOVA_r;
			const uint8_t *row = half ? rowhi : rowlo;
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int j1 = 0; j1 < SNOVA_r; ++j1)
					st1_mi[((size_t)a1 * SNOVA_l + b1) * SNOVA_r2 + (size_t)i1 * SNOVA_r + j1]
					    = row[(size_t)b1 * SNOVA_r + j1];
		}
	}
}

#ifdef RCT_T4_SELFTEST
static uint8_t rct_t4_affb(uint64_t qm, uint8_t src) {
	uint8_t out = 0;
	for (int i = 0; i < 8; ++i) {
		const uint8_t row = (uint8_t)(qm >> (8 * (7 - i)));
		out |= (uint8_t)((__builtin_parity((unsigned)(row & src)) & 1) << i);
	}
	return out;
}
static int rct_t4_ckmat(uint64_t qm, gf_t a, gf_t b, gf_t cq, gf_t d) {
	for (int s = 0; s < 256; ++s) {
		const gf_t in0 = (gf_t)(s & 15), in1 = (gf_t)(s >> 4);
		const uint8_t want = (uint8_t)((gf_mult(a, in0) ^ gf_mult(b, in1)) |
		                               ((gf_mult(cq, in0) ^ gf_mult(d, in1)) << 4));
		if (rct_t4_affb(qm, (uint8_t)s) != want) {
			return 1;
		}
	}
	return 0;
}
#endif

#if RCT_TILE4_PREBAKE
static _Alignas(32) uint8_t rct_t4_qall[(size_t)SNOVA_m1 * RCT_T4_NB * RCT_T4_NQ * 128];

static void rct_t4_prebake(const rct_pk_t *pkx) {
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		const uint8_t *PJ = &pkx->P[(size_t)mi * RCT_JOG_NL * RCT_JOG_L32];
		for (int tb = 0; tb < RCT_T4_NB; ++tb) {
			const uint8_t *rp[4];
			for (int k = 0; k < 4; ++k) {
				const int p = 4 * tb + k;
				rp[k] = (p < RCT_JOG_NL) ? (PJ + (size_t)p * RCT_JOG_L32) : rct_t4_zrow;
			}
			uint8_t *dst = rct_t4_qall + ((size_t)mi * RCT_T4_NB + tb) * RCT_T4_NQ * 128;
			for (int q4 = 0; q4 < RCT_T4_NQ; ++q4) {
				rct_t4_quad(rp, 16 * q4, dst + (size_t)q4 * 128);
			}
		}
	}
}
#endif

static void rct_vf_tile4(rct_vf_ctx *c) {
	RCT_SCRATCH _Alignas(32) uint8_t t4_whip[RCT_T4_CP * SNOVA_lr32];
	memset(t4_whip, 0, sizeof(t4_whip));
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int idx = 0; idx < SNOVA_n; ++idx) {
			const gf_t *sig = &c->sig_gf[(size_t)idx * SNOVA_lr];
			for (int i1 = 0; i1 < SNOVA_l; ++i1) {
				const gf_t *Srow = &rct_S[ab * SNOVA_l2 + i1 * SNOVA_l];
				__m128i acc = _mm_setzero_si128();
				for (int k1 = 0; k1 < SNOVA_l; ++k1)
					acc = _mm_xor_si128(acc,
					                    rct_jog_sv128(Srow[k1],
					                                  _mm_loadu_si128((const __m128i *)&sig[k1 * SNOVA_r])));
				_Alignas(16) uint8_t wb[16];
				_mm_store_si128((__m128i *)wb, acc);
				memcpy(&t4_whip[((size_t)idx * SNOVA_l + i1) * SNOVA_lr32
				                + (size_t)ab * SNOVA_r],
				       wb, SNOVA_r);
			}
		}

	RCT_SCRATCH _Alignas(32) uint8_t t4_wpk[RCT_T4_NCB * 2 * SNOVA_lr32];
	RCT_SCRATCH _Alignas(32) uint8_t t4_lqt[RCT_T4_NCB * 2 * (RCT_T4_LH * 128)];
#if RCT_T4_LH == 1
	for (int tb = 0; tb < RCT_T4_NCB; ++tb) {
		__m256i r0 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 0) * SNOVA_lr32]);
		__m256i r1 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 1) * SNOVA_lr32]);
		__m256i r2 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 2) * SNOVA_lr32]);
		__m256i r3 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 3) * SNOVA_lr32]);
		__m256i p0 = _mm256_or_si256(r0, _mm256_slli_epi16(r1, 4));
		__m256i p1 = _mm256_or_si256(r2, _mm256_slli_epi16(r3, 4));
		_mm256_store_si256((__m256i *)&t4_wpk[((size_t)tb * 2 + 0) * 32], p0);
		_mm256_store_si256((__m256i *)&t4_wpk[((size_t)tb * 2 + 1) * 32], p1);
		__m256i q[4];
		rct_t4_tree(_mm256_shuffle_epi8(p0, rct_t4_aqlo), q);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * 128 + 0], q[0]);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * 128 + 32], q[1]);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * 128 + 64], q[2]);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * 128 + 96], q[3]);
		rct_t4_tree(_mm256_shuffle_epi8(p1, rct_t4_aqlo), q);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * 128 + 0], q[0]);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * 128 + 32], q[1]);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * 128 + 64], q[2]);
		_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * 128 + 96], q[3]);
	}
#else
	for (int tb = 0; tb < RCT_T4_NCB; ++tb)
		for (int h = 0; h < RCT_T4_LH; ++h) {
			__m256i r0 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 0) * SNOVA_lr32 + (size_t)h * 32]);
			__m256i r1 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 1) * SNOVA_lr32 + (size_t)h * 32]);
			__m256i r2 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 2) * SNOVA_lr32 + (size_t)h * 32]);
			__m256i r3 = _mm256_load_si256((const __m256i *)&t4_whip[((size_t)tb * 4 + 3) * SNOVA_lr32 + (size_t)h * 32]);
			__m256i p0 = _mm256_or_si256(r0, _mm256_slli_epi16(r1, 4));
			__m256i p1 = _mm256_or_si256(r2, _mm256_slli_epi16(r3, 4));
			_mm256_store_si256((__m256i *)&t4_wpk[(((size_t)tb * 2 + 0) * RCT_T4_LH + h) * 32], p0);
			_mm256_store_si256((__m256i *)&t4_wpk[(((size_t)tb * 2 + 1) * RCT_T4_LH + h) * 32], p1);
			__m256i q[4];
			rct_t4_tree(_mm256_shuffle_epi8(p0, rct_t4_aqlo), q);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * (RCT_T4_LH * 128) + (size_t)h * 128 + 0], q[0]);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * (RCT_T4_LH * 128) + (size_t)h * 128 + 32], q[1]);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * (RCT_T4_LH * 128) + (size_t)h * 128 + 64], q[2]);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 0) * (RCT_T4_LH * 128) + (size_t)h * 128 + 96], q[3]);
			rct_t4_tree(_mm256_shuffle_epi8(p1, rct_t4_aqlo), q);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * (RCT_T4_LH * 128) + (size_t)h * 128 + 0], q[0]);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * (RCT_T4_LH * 128) + (size_t)h * 128 + 32], q[1]);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * (RCT_T4_LH * 128) + (size_t)h * 128 + 64], q[2]);
			_mm256_store_si256((__m256i *)&t4_lqt[((size_t)tb * 2 + 1) * (RCT_T4_LH * 128) + (size_t)h * 128 + 96], q[3]);
		}
#endif

#ifdef RCT_T4_SELFTEST
	{
		static int t4_ut_done = 0;
		if (!t4_ut_done) {
			t4_ut_done = 1;
			size_t bad_wpk = 0, bad_pad = 0, bad_lqt = 0, bad_quad = 0;
			for (int tb = 0; tb < RCT_T4_NCB; ++tb)
				for (int kp = 0; kp < 2; ++kp) {
					const int c0 = 4 * tb + 2 * kp;
					const int rowpad = (c0 >= RCT_JOG_NL);
					for (int j = 0; j < SNOVA_lr32; ++j) {
						const uint8_t got = t4_wpk[((size_t)tb * 2 + kp) * SNOVA_lr32 + j];
						const uint8_t want = (uint8_t)(t4_whip[(size_t)c0 * SNOVA_lr32 + j] |
						                               (t4_whip[((size_t)c0 + 1) * SNOVA_lr32 + j] << 4));
						if (got != want) {
							++bad_wpk;
						}
						if ((rowpad || j >= SNOVA_lr) && got != 0) {
							++bad_pad;
						}
					}
					if (rowpad) {
						for (int j = 0; j < RCT_T4_LH * 128; ++j)
							if (t4_lqt[((size_t)tb * 2 + kp) * (RCT_T4_LH * 128) + j] != 0) {
								++bad_pad;
							}
					}
					for (int p = 0; p < RCT_T4_NP; ++p) {
						uint64_t qm;
						memcpy(&qm, &t4_lqt[((size_t)tb * 2 + kp) * (RCT_T4_LH * 128) + RCT_T4_LQOFF(p)], 8);
						bad_lqt += (size_t)rct_t4_ckmat(qm,
						                                t4_whip[(size_t)c0 * SNOVA_lr32 + 2 * p],
						                                t4_whip[((size_t)c0 + 1) * SNOVA_lr32 + 2 * p],
						                                t4_whip[(size_t)c0 * SNOVA_lr32 + 2 * p + 1],
						                                t4_whip[((size_t)c0 + 1) * SNOVA_lr32 + 2 * p + 1]);
					}
				}
			{
				_Alignas(32) uint8_t qd[128];
				const int mis[2] = {0, SNOVA_m1 - 1};
				const int tbs[2] = {0, RCT_T4_NB - 1};
				const int qs[2] = {0, RCT_T4_NQ - 1};
				for (int t = 0; t < 2; ++t) {
					const uint8_t *PJt = &c->pkx->P[(size_t)mis[t] * RCT_JOG_NL * RCT_JOG_L32];
					const uint8_t *rp[4];
					for (int k = 0; k < 4; ++k) {
						const int p = 4 * tbs[t] + k;
						rp[k] = (p < RCT_JOG_NL) ? (PJt + (size_t)p * RCT_JOG_L32) : rct_t4_zrow;
					}
					const int coff = 16 * qs[t];
					rct_t4_quad(rp, coff, qd);
					for (int cc = 0; cc < 4; ++cc)
						for (int ip = 0; ip < 2; ++ip)
							for (int kp = 0; kp < 2; ++kp) {
								uint64_t qm;
								memcpy(&qm, qd + (cc >> 1) * 64 + kp * 32 + (cc & 1) * 16 + ip * 8, 8);
								bad_quad += (size_t)rct_t4_ckmat(qm,
								                                 rp[2 * ip][coff + 4 * cc + 2 * kp],
								                                 rp[2 * ip][coff + 4 * cc + 2 * kp + 1],
								                                 rp[2 * ip + 1][coff + 4 * cc + 2 * kp],
								                                 rp[2 * ip + 1][coff + 4 * cc + 2 * kp + 1]);
							}
				}
			}
			printf("[RCT_T4_UNIT] wpk=%zu pad=%zu lqt=%zu quad=%zu (all 0 = PASS)\n",
			       bad_wpk, bad_pad, bad_lqt, bad_quad);
		}
	}
#endif

	gf_t *sum_t1 = c->sum_t1;
	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		const uint8_t *PJ = &c->pkx->P[(size_t)mi * RCT_JOG_NL * RCT_JOG_L32];
		(void)PJ;

		RCT_SCRATCH _Alignas(32) uint8_t st0p_t[RCT_T4_NB * 2 * SNOVA_lr32];
		for (int tb = 0; tb < RCT_T4_NB; ++tb) {
#if !RCT_TILE4_PREBAKE
			const uint8_t *rp[4];
			for (int k = 0; k < 4; ++k) {
				const int p = 4 * tb + k;
				rp[k] = (p < RCT_JOG_NL) ? (PJ + (size_t)p * RCT_JOG_L32) : rct_t4_zrow;
			}
			_Alignas(32) uint8_t qrow[RCT_T4_NQ * 128];
			for (int q4 = 0; q4 < RCT_T4_NQ; ++q4) {
				rct_t4_quad(rp, 16 * q4, qrow + q4 * 128);
			}
#endif
#if RCT_T4_LH == 1
			__m256i a01a = _mm256_setzero_si256(), a23a = _mm256_setzero_si256();
			__m256i a01b = _mm256_setzero_si256(), a23b = _mm256_setzero_si256();
			for (int njq = 0; njq < RCT_T4_NCB; njq += 4) {
#if RCT_TILE4_PREBAKE
				const uint8_t *qb = rct_t4_qall
				                    + (((size_t)mi * RCT_T4_NB + tb) * RCT_T4_NQ + (njq >> 2)) * 128;
#else
				const uint8_t *qb = qrow + (njq >> 2) * 128;
#endif
				const uint8_t *wb = &t4_wpk[(size_t)njq * 64];
				for (int cc = 0; cc < 4; cc += 2) {
					const int cb = (cc >> 1) * 64;
					__m256i wv0 = _mm256_load_si256((const __m256i *)(wb + cc * 64));
					__m256i wv1 = _mm256_load_si256((const __m256i *)(wb + cc * 64 + 32));
					__m256i xv0 = _mm256_load_si256((const __m256i *)(wb + cc * 64 + 64));
					__m256i xv1 = _mm256_load_si256((const __m256i *)(wb + cc * 64 + 96));
					a01a = _mm256_xor_si256(a01a, _mm256_gf2p8affine_epi64_epi8(
					                            wv0, rct_t4_bq(qb, cb + 0), 0));
					a23a = _mm256_xor_si256(a23a, _mm256_gf2p8affine_epi64_epi8(
					                            wv0, rct_t4_bq(qb, cb + 8), 0));
					a01a = _mm256_xor_si256(a01a, _mm256_gf2p8affine_epi64_epi8(
					                            wv1, rct_t4_bq(qb, cb + 32), 0));
					a23a = _mm256_xor_si256(a23a, _mm256_gf2p8affine_epi64_epi8(
					                            wv1, rct_t4_bq(qb, cb + 40), 0));
					a01b = _mm256_xor_si256(a01b, _mm256_gf2p8affine_epi64_epi8(
					                            xv0, rct_t4_bq(qb, cb + 16), 0));
					a23b = _mm256_xor_si256(a23b, _mm256_gf2p8affine_epi64_epi8(
					                            xv0, rct_t4_bq(qb, cb + 24), 0));
					a01b = _mm256_xor_si256(a01b, _mm256_gf2p8affine_epi64_epi8(
					                            xv1, rct_t4_bq(qb, cb + 48), 0));
					a23b = _mm256_xor_si256(a23b, _mm256_gf2p8affine_epi64_epi8(
					                            xv1, rct_t4_bq(qb, cb + 56), 0));
				}
			}
			_mm256_store_si256((__m256i *)&st0p_t[((size_t)tb * 2 + 0) * 32],
			                   _mm256_xor_si256(a01a, a01b));
			_mm256_store_si256((__m256i *)&st0p_t[((size_t)tb * 2 + 1) * 32],
			                   _mm256_xor_si256(a23a, a23b));
#else
			__m256i a01a[RCT_T4_LH], a23a[RCT_T4_LH], a01b[RCT_T4_LH], a23b[RCT_T4_LH];
			for (int h = 0; h < RCT_T4_LH; ++h) {
				a01a[h] = _mm256_setzero_si256();
				a23a[h] = _mm256_setzero_si256();
				a01b[h] = _mm256_setzero_si256();
				a23b[h] = _mm256_setzero_si256();
			}
			for (int njq = 0; njq < RCT_T4_NCB; njq += 4) {
#if RCT_TILE4_PREBAKE
				const uint8_t *qb = rct_t4_qall
				                    + (((size_t)mi * RCT_T4_NB + tb) * RCT_T4_NQ + (njq >> 2)) * 128;
#else
				const uint8_t *qb = qrow + (njq >> 2) * 128;
#endif
				const uint8_t *wb = &t4_wpk[(size_t)njq * (2 * RCT_T4_LH * 32)];
				for (int cc = 0; cc < 4; cc += 2) {
					const int cb = (cc >> 1) * 64;
					for (int h = 0; h < RCT_T4_LH; ++h) {
						__m256i wv0 = _mm256_load_si256((const __m256i *)(wb + ((size_t)(cc * 2 + 0) * RCT_T4_LH + h) * 32));
						__m256i wv1 = _mm256_load_si256((const __m256i *)(wb + ((size_t)(cc * 2 + 1) * RCT_T4_LH + h) * 32));
						__m256i xv0 = _mm256_load_si256((const __m256i *)(wb + ((size_t)(cc * 2 + 2) * RCT_T4_LH + h) * 32));
						__m256i xv1 = _mm256_load_si256((const __m256i *)(wb + ((size_t)(cc * 2 + 3) * RCT_T4_LH + h) * 32));
						a01a[h] = _mm256_xor_si256(a01a[h], _mm256_gf2p8affine_epi64_epi8(
						                               wv0, rct_t4_bq(qb, cb + 0), 0));
						a23a[h] = _mm256_xor_si256(a23a[h], _mm256_gf2p8affine_epi64_epi8(
						                               wv0, rct_t4_bq(qb, cb + 8), 0));
						a01a[h] = _mm256_xor_si256(a01a[h], _mm256_gf2p8affine_epi64_epi8(
						                               wv1, rct_t4_bq(qb, cb + 32), 0));
						a23a[h] = _mm256_xor_si256(a23a[h], _mm256_gf2p8affine_epi64_epi8(
						                               wv1, rct_t4_bq(qb, cb + 40), 0));
						a01b[h] = _mm256_xor_si256(a01b[h], _mm256_gf2p8affine_epi64_epi8(
						                               xv0, rct_t4_bq(qb, cb + 16), 0));
						a23b[h] = _mm256_xor_si256(a23b[h], _mm256_gf2p8affine_epi64_epi8(
						                               xv0, rct_t4_bq(qb, cb + 24), 0));
						a01b[h] = _mm256_xor_si256(a01b[h], _mm256_gf2p8affine_epi64_epi8(
						                               xv1, rct_t4_bq(qb, cb + 48), 0));
						a23b[h] = _mm256_xor_si256(a23b[h], _mm256_gf2p8affine_epi64_epi8(
						                               xv1, rct_t4_bq(qb, cb + 56), 0));
					}
				}
			}
			for (int h = 0; h < RCT_T4_LH; ++h) {
				_mm256_store_si256((__m256i *)&st0p_t[(((size_t)tb * 2 + 0) * RCT_T4_LH + h) * 32],
				                   _mm256_xor_si256(a01a[h], a01b[h]));
				_mm256_store_si256((__m256i *)&st0p_t[(((size_t)tb * 2 + 1) * RCT_T4_LH + h) * 32],
				                   _mm256_xor_si256(a23a[h], a23b[h]));
			}
#endif
		}

		gf_t *st1_mi = &sum_t1[(size_t)mi * SNOVA_l2 * SNOVA_r2];
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 0, (RCT_T4_NP < 5 ? RCT_T4_NP : 5));
#if RCT_T4_NP > 5
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 5, (RCT_T4_NP - 5 < 5 ? RCT_T4_NP - 5 : 5));
#endif
#if RCT_T4_NP > 10
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 10, (RCT_T4_NP - 10 < 5 ? RCT_T4_NP - 10 : 5));
#endif
#if RCT_T4_NP > 15
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 15, (RCT_T4_NP - 15 < 5 ? RCT_T4_NP - 15 : 5));
#endif
#if RCT_T4_NP > 20
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 20, (RCT_T4_NP - 20 < 5 ? RCT_T4_NP - 20 : 5));
#endif
#if RCT_T4_NP > 25
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 25, (RCT_T4_NP - 25 < 5 ? RCT_T4_NP - 25 : 5));
#endif
#if RCT_T4_NP > 30
		rct_t4_left(st0p_t, t4_lqt, st1_mi, 30, (RCT_T4_NP - 30 < 5 ? RCT_T4_NP - 30 : 5));
#endif
	}

#ifdef RCT_T4_SELFTEST
	{
		RCT_SCRATCH gf_t t4_ref[SNOVA_m1 * SNOVA_l2 * SNOVA_r2];
		memcpy(t4_ref, c->sum_t1, sizeof(t4_ref));
		rct_vf_jog(c);
		size_t t4_mm = 0;
		for (size_t i = 0; i < sizeof(t4_ref); ++i)
			if (t4_ref[i] != c->sum_t1[i]) {
				++t4_mm;
			}
		printf("[RCT_T4_SELFTEST] sum_t1 mismatches=%zu / %zu\n", t4_mm, sizeof(t4_ref));
	}
#else
	(void)rct_vf_jog;
#endif
}

#endif

#endif

// #include "platforms/x86_avx2/rct_verify_oddq.h"
// #include "platforms/generic/rct_verify_scalar.h"
#ifndef RCT_VERIFY_SCALAR_H
#define RCT_VERIFY_SCALAR_H

#if !RCT_USE_SIMD && !RCT_Q_SIMD && !RCT_VF_JOG

static void rct_vf_contract_ref(rct_vf_ctx *c) {
	const rct_pk_t *pkx = c->pkx;
	const gf_t *signature_in_GF = c->sig_gf;
	gf_t *sum_t1 = c->sum_t1;
	static gf_t whipped_sig[SNOVA_l * SNOVA_n * SNOVA_lr];
	memset(whipped_sig, 0, sizeof(whipped_sig));
	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int idx = 0; idx < SNOVA_n; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int j1 = 0; j1 < SNOVA_r; j1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						gf_set_add(&whipped_sig[(ab * SNOVA_n + idx) * SNOVA_lr + i1 * SNOVA_r + j1],
						           gf_mult(rct_S[ab * SNOVA_l2 + i1 * SNOVA_l + k1],
						                   signature_in_GF[idx * SNOVA_lr + k1 * SNOVA_r + j1]));

	static gf_t sum_t0[SNOVA_m1 * SNOVA_l * SNOVA_n * SNOVA_lr];
	memset(sum_t0, 0, sizeof(sum_t0));

	for (int mi = 0; mi < SNOVA_m1; ++mi) {
		for (int ni = 0; ni < SNOVA_n; ++ni)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int nj = 0; nj < SNOVA_n; ++nj)
					gf_mat_mul_add_lr(&sum_t0[((mi * SNOVA_l + b1) * SNOVA_n + ni) * SNOVA_lr],
					                  &pkx->P[((mi * SNOVA_n + ni) * SNOVA_n + nj) * SNOVA_l2],
					                  &whipped_sig[(b1 * SNOVA_n + nj) * SNOVA_lr], SNOVA_l, SNOVA_l, SNOVA_r);

		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int b1 = 0; b1 < SNOVA_l; ++b1)
				for (int ni = 0; ni < SNOVA_n; ++ni)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								gf_set_add(&sum_t1[(mi * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1],
								           gf_mult(whipped_sig[(a1 * SNOVA_n + ni) * SNOVA_lr + k1 * SNOVA_r + i1],
								                   sum_t0[((mi * SNOVA_l + b1) * SNOVA_n + ni) * SNOVA_lr + k1 * SNOVA_r + j1]));
	}
}

#endif

#endif

// #include "platforms/x86_avx2/rct_verify_emat.h"
#ifndef RCT_VERIFY_EMAT_H
#define RCT_VERIFY_EMAT_H

static void rct_vf_emat(rct_vf_ctx *c) {
#if RCT_VF_JOG
	rct_vf_emat_jog(c);
	return;
#endif
	RCT_SCRATCH __attribute__((unused)) _Alignas(64) uint8_t rct_vf_amt[SNOVA_o * SNOVA_alpha * 64];
	RCT_SCRATCH __attribute__((unused)) _Alignas(16) uint8_t rct_vf_q12[SNOVA_o * SNOVA_alpha * 16];
	const rct_pk_t *pkx = c->pkx;
	gf_t *hash_in_GF = c->hash_gf;
#if RCT_VF_EMM
	gf_t *sum_t1q = c->sum_t1q;
#elif !RCT_Q_SIMD
	gf_t *sum_t1 = c->sum_t1;
#endif
#if RCT_Q_SIMD && !RCT_Q_EMM
	uint16_t *sum_t1s = c->sum_t1s;
#endif
#if RCT_Q_SIMD
	uint16_t hash_u16[SNOVA_o * SNOVA_lr + 16] = {0};
#endif
#if RCT_Q_EMM
	for (int t_idx = 0; t_idx < SNOVA_o * SNOVA_alpha; t_idx++) {
		rct_qv_tr8(&rct_qv_amt[t_idx * 64], &pkx->Am[t_idx * SNOVA_r2], SNOVA_r);
	}
#endif
#if RCT_VF_EMM
	for (int t_idx = 0; t_idx < SNOVA_o * SNOVA_alpha; t_idx++) {
		rct_vf_tr8(&rct_vf_amt[t_idx * 64], &pkx->Am[t_idx * SNOVA_r2], SNOVA_r);
	}
	{
		const __m128i m0f_ = _mm_set1_epi8(0x0f);
		for (int t_idx = 0; t_idx < SNOVA_o * SNOVA_alpha; t_idx++) {
			const gf_t *q1p = &pkx->q1[t_idx * SNOVA_l];
			const gf_t *q2p = &pkx->q2[t_idx * SNOVA_l];
			uint16_t a01, a23;
			memcpy(&a01, q1p, 2);
			memcpy(&a23, q1p + 2, 2);
			unsigned i01 = (a01 & 0xFFu) | (unsigned)((a01 >> 8) << 4);
			unsigned i23 = (a23 & 0xFFu) | (unsigned)((a23 >> 8) << 4);
			int32_t q2w;
			memcpy(&q2w, q2p, 4);
			__m128i q2v = _mm_cvtsi32_si128(q2w);
			__m128i r01 = _mm_shuffle_epi8(_mm_load_si128((const __m128i *)rct_mtk2[i01]), q2v);
			__m128i r23 = _mm_shuffle_epi8(_mm_load_si128((const __m128i *)rct_mtk2[i23]), q2v);
			__m128i p01 = _mm_unpacklo_epi32(_mm_and_si128(r01, m0f_),
			                                 _mm_and_si128(_mm_srli_epi16(r01, 4), m0f_));
			__m128i p23 = _mm_unpacklo_epi32(_mm_and_si128(r23, m0f_),
			                                 _mm_and_si128(_mm_srli_epi16(r23, 4), m0f_));
			_mm_store_si128((__m128i *)&rct_vf_q12[t_idx * 16], _mm_unpacklo_epi64(p01, p23));
		}
	}
#endif
	for (int mi = 0; mi < SNOVA_o; ++mi) {
#if RCT_Q_EMM
		__m256i qv_h0 = _mm256_setzero_si256(), qv_h1 = _mm256_setzero_si256();
#endif
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			int mi_prime = i_prime(mi, alpha);
			gf_t gf16m_temp1[SNOVA_r2] = {0};
			gf_t gf16m_temp2[SNOVA_lr + 16] = {0};
			(void)gf16m_temp2;
			(void)gf16m_temp1;
#if RCT_USE_SIMD && SNOVA_l == 4 && SNOVA_r2 <= 64
			_Alignas(32) uint8_t t1b[64];
#endif
#if RCT_VF_EMM
			{
				const uint8_t *cc = &rct_vf_q12[(mi * SNOVA_alpha + alpha) * 16];
				__m256i t1lo = _mm256_setzero_si256(), t1hi = _mm256_setzero_si256();
				for (int ab = 0; ab < SNOVA_l2; ab++) {
					const gf_t *base = &sum_t1q[(mi_prime * SNOVA_l2 + ab) * 64];
					__m256i cv = RCT_BC(cc[ab]);
					t1lo = _mm256_xor_si256(t1lo, RCT_SV(cv, _mm256_load_si256((const __m256i *)base)));
					t1hi = _mm256_xor_si256(t1hi, RCT_SV(cv, _mm256_load_si256((const __m256i *)(base + 32))));
				}
				_mm256_store_si256((__m256i *)t1b, rct_gfni_cleanup256(t1lo));
				_mm256_store_si256((__m256i *)(t1b + 32), rct_gfni_cleanup256(t1hi));
			}
#elif RCT_USE_SIMD && SNOVA_l == 4 && SNOVA_r2 <= 64
			{
#if SNOVA_r2 <= 32
				__m256i t1 = _mm256_setzero_si256();
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					__m256i s = _mm256_setzero_si256();
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						const gf_t *base = &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2];
						__m256i qv = RCT_BC(pkx->q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]);
						s = _mm256_xor_si256(s, RCT_SV(qv, _mm256_loadu_si256((const __m256i *)base)));
					}
					__m256i qa = RCT_BC(pkx->q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]);
					t1 = _mm256_xor_si256(t1, RCT_SV(qa, rct_gfni_cleanup256(s)));
				}
				_mm256_store_si256((__m256i *)t1b, rct_gfni_cleanup256(t1));
				memcpy(gf16m_temp1, t1b, SNOVA_r2);
#else
				__m256i t1lo = _mm256_setzero_si256(), t1hi = _mm256_setzero_si256();
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					__m256i slo = _mm256_setzero_si256(), shi = _mm256_setzero_si256();
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						const gf_t *base = &sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2];
						__m256i qv = RCT_BC(pkx->q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]);
						slo = _mm256_xor_si256(slo, RCT_SV(qv, _mm256_loadu_si256((const __m256i *)base)));
						shi = _mm256_xor_si256(shi, RCT_SV(qv, _mm256_loadu_si256((const __m256i *)(base + 32))));
					}
					slo = rct_gfni_cleanup256(slo);
					shi = rct_gfni_cleanup256(shi);
					__m256i qa = RCT_BC(pkx->q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]);
					t1lo = _mm256_xor_si256(t1lo, RCT_SV(qa, slo));
					t1hi = _mm256_xor_si256(t1hi, RCT_SV(qa, shi));
				}
				_mm256_store_si256((__m256i *)t1b, rct_gfni_cleanup256(t1lo));
				_mm256_store_si256((__m256i *)(t1b + 32), rct_gfni_cleanup256(t1hi));
				memcpy(gf16m_temp1, t1b, SNOVA_r2);
#endif
			}
#elif RCT_Q_EMM
			if ((alpha & 1) == 0) {
				const int t0i = mi * SNOVA_alpha + alpha, t1i = t0i + 1;
				const int mp1 = i_prime(mi, alpha + 1);
				_Alignas(32) uint8_t q12b[32];
				{
					int32_t q1d, q2d;
					memcpy(&q1d, &pkx->q1[t0i * SNOVA_l], 4);
					memcpy(&q2d, &pkx->q2[t0i * SNOVA_l], 4);
					_mm_store_si128((__m128i *)q12b, rct_qv_pack16(rct_q_barrett16(_mm256_mullo_epi16(
					            _mm256_shuffle_epi8(_mm256_set1_epi32(q1d), RCT_QV_PA0),
					            _mm256_shuffle_epi8(_mm256_set1_epi32(q2d), RCT_QV_PB)))));
					memcpy(&q1d, &pkx->q1[t1i * SNOVA_l], 4);
					memcpy(&q2d, &pkx->q2[t1i * SNOVA_l], 4);
					_mm_store_si128((__m128i *)(q12b + 16), rct_qv_pack16(rct_q_barrett16(_mm256_mullo_epi16(
					            _mm256_shuffle_epi8(_mm256_set1_epi32(q1d), RCT_QV_PA0),
					            _mm256_shuffle_epi8(_mm256_set1_epi32(q2d), RCT_QV_PB)))));
				}
				const uint16_t *qp0 = (const uint16_t *)q12b;
				const uint16_t *qp1 = (const uint16_t *)(q12b + 16);
				const uint8_t *sp0 = &rct_qv_s1p8[mi_prime * 8 * 128];
				const uint8_t *sp1 = &rct_qv_s1p8[mp1 * 8 * 128];
				_Alignas(64) uint8_t t1b8[2][64];
				{
					__m256i ta = _mm256_setzero_si256(), tb = _mm256_setzero_si256();
					__m256i tc = _mm256_setzero_si256(), td = _mm256_setzero_si256();
					__m256i ua = _mm256_setzero_si256(), ub = _mm256_setzero_si256();
					__m256i uc = _mm256_setzero_si256(), ud = _mm256_setzero_si256();
					for (int p = 0; p < 8; p++) {
						__m256i bq = _mm256_set1_epi16((short)qp0[p]);
						ta = _mm256_add_epi16(ta, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp0 + p * 128)), bq));
						tb = _mm256_add_epi16(tb, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp0 + p * 128 + 32)), bq));
						tc = _mm256_add_epi16(tc, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp0 + p * 128 + 64)), bq));
						td = _mm256_add_epi16(td, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp0 + p * 128 + 96)), bq));
						bq = _mm256_set1_epi16((short)qp1[p]);
						ua = _mm256_add_epi16(ua, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp1 + p * 128)), bq));
						ub = _mm256_add_epi16(ub, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp1 + p * 128 + 32)), bq));
						uc = _mm256_add_epi16(uc, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp1 + p * 128 + 64)), bq));
						ud = _mm256_add_epi16(ud, _mm256_maddubs_epi16(
						                          _mm256_load_si256((const __m256i *)(sp1 + p * 128 + 96)), bq));
					}
					_mm256_store_si256((__m256i *)t1b8[0],
					                   rct_qv_pack32(rct_q_barrett16(ta), rct_q_barrett16(tb)));
					_mm256_store_si256((__m256i *)(t1b8[0] + 32),
					                   rct_qv_pack32(rct_q_barrett16(tc), rct_q_barrett16(td)));
					_mm256_store_si256((__m256i *)t1b8[1],
					                   rct_qv_pack32(rct_q_barrett16(ua), rct_q_barrett16(ub)));
					_mm256_store_si256((__m256i *)(t1b8[1] + 32),
					                   rct_qv_pack32(rct_q_barrett16(uc), rct_q_barrett16(ud)));
				}
				_Alignas(64) uint8_t t1t8[2][64];
				rct_qv_tr8(t1t8[0], t1b8[0], SNOVA_r);
				rct_qv_tr8(t1t8[1], t1b8[1], SNOVA_r);
				__m256i m0, m1, m2, m3;
				rct_qv_mm_rx4(&m0, &m1, t1t8[0], &pkx->Bm[t0i * SNOVA_lr]);
				rct_qv_mm_rx4(&m2, &m3, t1t8[1], &pkx->Bm[t1i * SNOVA_lr]);
				_Alignas(32) uint8_t t2b8[2][32];
				_mm256_store_si256((__m256i *)t2b8[0],
				                   rct_qv_pack32(rct_q_barrett16(m0), rct_q_barrett16(m1)));
				_mm256_store_si256((__m256i *)t2b8[1],
				                   rct_qv_pack32(rct_q_barrett16(m2), rct_q_barrett16(m3)));
				rct_qv_mm_rx4(&m0, &m1, &rct_qv_amt[t0i * 64], t2b8[0]);
				rct_qv_mm_rx4(&m2, &m3, &rct_qv_amt[t1i * 64], t2b8[1]);
				qv_h0 = _mm256_add_epi16(qv_h0, _mm256_add_epi16(m0, m2));
				qv_h1 = _mm256_add_epi16(qv_h1, _mm256_add_epi16(m1, m3));
			}
#elif RCT_Q_SIMD && (SNOVA_r == SNOVA_l) && RCT_Q_HAVE_MAGIC
			{
				__m256i t1v = _mm256_setzero_si256();
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					__m256i t0v = _mm256_setzero_si256();
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						t0v = _mm256_add_epi16(t0v, _mm256_mullo_epi16(
						                           _mm256_set1_epi16((short)pkx->q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]),
						                           _mm256_load_si256((const __m256i *)&sum_t1s[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2])));
					t0v = rct_q_barrett16(t0v);
					t1v = _mm256_add_epi16(t1v, _mm256_mullo_epi16(
					                           _mm256_set1_epi16((short)pkx->q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]), t0v));
				}
				t1v = rct_q_barrett16(t1v);
				_Alignas(32) uint16_t t1buf[SNOVA_r2];
				_mm256_store_si256((__m256i *)t1buf, t1v);
				for (int k = 0; k < SNOVA_r2; ++k) {
					gf16m_temp1[k] = (gf_t)t1buf[k];
				}
			}
#elif RCT_Q_SIMD
			{
				uint16_t t0acc[SNOVA_r2];
				uint16_t t1[SNOVA_r2] = {0};
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						t0acc[i1] = 0;
					}
					for (int b1 = 0; b1 < SNOVA_l; ++b1) {
						uint16_t qb = pkx->q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];
						const uint16_t *st = &sum_t1s[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2];
						for (int i1 = 0; i1 < SNOVA_r2; i1++) {
							t0acc[i1] += qb * st[i1];
						}
					}
					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						t0acc[i1] %= SNOVA_q;
					}
					uint16_t qa = pkx->q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1];
					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						t1[i1] += t0acc[i1] * qa;
					}
				}
				for (int i1 = 0; i1 < SNOVA_r2; i1++) {
					gf16m_temp1[i1] = (gf_t)(t1[i1] % SNOVA_q);
				}
			}
#else
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				gf_t sumb[SNOVA_r2] = {0};
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							gf_set_add(&sumb[i1 * SNOVA_r + j1],
							           gf_mult(sum_t1[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1],
							                   pkx->q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1]));
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						gf_set_add(&gf16m_temp1[i1 * SNOVA_r + j1],
						           gf_mult(sumb[i1 * SNOVA_r + j1], pkx->q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1]));
			}
#endif
#if RCT_VF_EMM
			{
				_Alignas(32) uint8_t t2b[32];
				__m256i t2 = rct_vf_mm_dw(t1b, &pkx->Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr]);
				_mm256_store_si256((__m256i *)t2b, rct_gfni_cleanup256(t2));
				__m256i h = rct_vf_mm_dw(&rct_vf_amt[(mi * SNOVA_alpha + alpha) * 64], t2b);
				__m256i cur = _mm256_loadu_si256((const __m256i *)&hash_in_GF[mi * SNOVA_lr]);
				_mm256_storeu_si256((__m256i *)&hash_in_GF[mi * SNOVA_lr], _mm256_xor_si256(cur, h));
			}
#elif RCT_USE_SIMD && SNOVA_l == 4
			rct_matmul_l4rows(gf16m_temp2, gf16m_temp1, &pkx->Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_r, SNOVA_r);
			rct_matmul_l4rows_add(&hash_in_GF[mi * SNOVA_lr], &pkx->Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2],
			                      gf16m_temp2, SNOVA_r, SNOVA_r);
#elif RCT_Q_EMM
#elif RCT_Q_SIMD && (SNOVA_r == SNOVA_l)
			{
				_Alignas(32) uint16_t t2acc[SNOVA_l2] = {0};
				rct_q_matmul4_add(t2acc, gf16m_temp1, &pkx->Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr]);
				gf_t t2u8[SNOVA_l2];
				for (int k = 0; k < SNOVA_l2; ++k) {
					t2u8[k] = (gf_t)(t2acc[k] % SNOVA_q);
				}
				rct_q_matmul4_add(&hash_u16[mi * SNOVA_lr], &pkx->Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], t2u8);
			}
#elif RCT_Q_SIMD
			{
				uint16_t t2[SNOVA_lr] = {0};
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_r; k1++)
							t2[i1 * SNOVA_l + j1] += gf16m_temp1[i1 * SNOVA_r + k1] *
							                         pkx->Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];
				for (int k = 0; k < SNOVA_lr; ++k) {
					t2[k] %= SNOVA_q;
				}
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_r; k1++)
							hash_u16[mi * SNOVA_lr + i1 * SNOVA_l + j1] +=
							    pkx->Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] *
							    t2[k1 * SNOVA_l + j1];
			}
#else
			gf_mat_mul_add_lr(gf16m_temp2, gf16m_temp1, &pkx->Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr], SNOVA_r, SNOVA_r, SNOVA_l);
			gf_mat_mul_add_lr(&hash_in_GF[mi * SNOVA_lr], &pkx->Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2], gf16m_temp2,
			                  SNOVA_r, SNOVA_r, SNOVA_l);
#endif
		}
#if RCT_Q_EMM
		{
			uint16_t *hp = &hash_u16[mi * SNOVA_lr];
			_mm256_storeu_si256((__m256i *)hp,
			                    _mm256_add_epi16(_mm256_loadu_si256((const __m256i *)hp), qv_h0));
			_mm256_storeu_si256((__m256i *)(hp + 16),
			                    _mm256_add_epi16(_mm256_loadu_si256((const __m256i *)(hp + 16)), qv_h1));
		}
#endif
	}
#if RCT_Q_SIMD
	for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
		hash_in_GF[i1] = (gf_t)(hash_u16[i1] % SNOVA_q);
	}
#endif
#if RCT_VF_EMM
	for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1 += 32)
		_mm256_storeu_si256((__m256i *)&hash_in_GF[i1],
		                    rct_gfni_cleanup256(_mm256_loadu_si256((const __m256i *)&hash_in_GF[i1])));
#endif
}

#endif


static int rct_pk_expand(rct_pk_t *pkx, const uint8_t *pk) {
	rct_init();
	rct_pkx_ctx C = { .pkx = pkx, .pk = pk };
#if RCT_VF_TILE4 && RCT_TILE4_PREBAKE
	{
		const int prc = rct_pkx_expand(&C);
		if (prc == 0) {
			rct_t4_prebake(pkx);
		}
		return prc;
	}
#else
	return rct_pkx_expand(&C);
#endif
}

static int rct_verify(const rct_pk_t *pkx, const uint8_t *sig, const uint8_t *digest, const size_t len_digest) {
	rct_init();
#ifdef RCT_VPROF
	static uint64_t vpf[8];
	static int vpc = 0;
	uint64_t va, vb;
#endif
	RCT_SCRATCH _Alignas(32) gf_t signature_in_GF[NUMGF_SIGNATURE + 32];
	VPT(va);
#if RCT_VF_MTK2 && (NUMGF_SIGNATURE % 2 == 0)
	rct_vf_expand_sig(signature_in_GF, sig, NUMGF_SIGNATURE);
#else
	if (expand_gf(signature_in_GF, sig, NUMGF_SIGNATURE)) {
		return -1;
	}
#endif
	VPT(vb);
	VPA(0, va, vb);

	gf_t hash_in_GF[SNOVA_o * SNOVA_lr + 32] = {0};
#if !RCT_VF_EMM
	RCT_SCRATCH _Alignas(32) gf_t sum_t1[SNOVA_m1 * SNOVA_l2 * SNOVA_r2 + 64];
	memset(sum_t1, 0, sizeof(sum_t1));
#else
	RCT_SCRATCH _Alignas(64) gf_t sum_t1q[SNOVA_m1 * SNOVA_l2 * 64];
#endif
#if RCT_Q_SIMD && !RCT_Q_EMM
	RCT_SCRATCH _Alignas(32) uint16_t sum_t1s[SNOVA_m1 * SNOVA_l2 * SNOVA_r2];
#endif

	rct_vf_ctx C;
	rct_vf_ctx *c = &C;
	c->pkx = pkx;
	c->sig = sig;
	c->sig_gf = signature_in_GF;
	c->hash_gf = hash_in_GF;
#if !RCT_VF_EMM
	c->sum_t1 = sum_t1;
#else
	c->sum_t1q = sum_t1q;
#endif
#if RCT_Q_SIMD && !RCT_Q_EMM
	c->sum_t1s = sum_t1s;
#endif
#ifdef RCT_VPROF
	c->vpf = vpf;
	c->vpc = &vpc;
#endif

#if RCT_VF_JOG
	{
#if RCT_USE_SIMD
		RCT_SCRATCH _Alignas(32) uint8_t jog_whipped_sig2[SNOVA_l * SNOVA_n * SNOVA_lr32];
		memset(jog_whipped_sig2, 0, sizeof(jog_whipped_sig2));
		c->whipped_sig2 = jog_whipped_sig2;
		VPT(va);
		rct_vf_whip_q16(c);
		VPT(vb);
		VPA(1, va, vb);
#endif
		VPT(va);
#if RCT_VF_TILE4
		rct_vf_tile4(c);
#else
		rct_vf_jog(c);
#endif
		VPT(vb);
		VPA(2, va, vb);
	}
#elif RCT_USE_SIMD
	{
		RCT_SCRATCH _Alignas(32) uint8_t whipped_sig2[SNOVA_l * SNOVA_n * SNOVA_lr32];
#if !(RCT_USE_GFNI && RCT_VF_MTK2)
		memset(whipped_sig2, 0, sizeof(whipped_sig2));
#endif
		c->whipped_sig2 = whipped_sig2;
		VPT(va);
		rct_vf_whip_q16(c);
		VPT(vb);
		VPA(1, va, vb);
		RCT_SCRATCH _Alignas(32) uint8_t sum_t1p[(SNOVA_m1 * SNOVA_l * SNOVA_r
		        + ((SNOVA_r) < 7 ? (8 - (SNOVA_r)) : 1)) * SNOVA_lr32];
		memset(sum_t1p, 0, sizeof(sum_t1p));
		c->sum_t1p = sum_t1p;
		VPT(va);
#if RCT_VF_AQ
		rct_vf_contract_aq(c);
#else
		rct_vf_contract_nonaq(c);
#endif
		VPT(vb);
		VPA(2, va, vb);
		VPT(va);
		rct_vf_reindex_q16(c);
		VPT(vb);
		VPA(3, va, vb);
	}
#else
	memset(sum_t1, 0, sizeof(sum_t1));
#if RCT_Q_SIMD
#if (SNOVA_r == SNOVA_l)
	_Static_assert((uint32_t)SNOVA_n * SNOVA_l * (SNOVA_q - 1) * (SNOVA_q - 1) < 65536u,
	               "u16 accumulation overflow guard");
#else
#endif
	{
		VPT(va);
		rct_vf_whip_oddq(c);
		VPT(vb);
		VPA(1, va, vb);
		VPT(va);
		rct_vf_rl_oddq(c);
		VPT(vb);
		VPA(2, va, vb);
		VPT(va);
		rct_vf_reindex_oddq(c);
		VPT(vb);
		VPA(3, va, vb);
	}
#else
	rct_vf_contract_ref(c);
#endif
#endif

	VPT(va);
	rct_vf_emat(c);

	VPT(vb);
	VPA(4, va, vb);
	VPT(va);
	uint8_t signed_bytes[BYTES_HASH];
	uint8_t signed_gf[GF16_HASH] = {0};
	const uint8_t *salt = sig + BYTES_SIGNATURE - BYTES_SALT;
#if HASH_PK
	hash_combined(signed_bytes, digest, len_digest, pkx->pk_hash, salt);
#else
	hash_combined(signed_bytes, digest, len_digest, pkx->pk_seed, salt);
#endif
	expand_gf(signed_gf, signed_bytes, GF16_HASH);

	int result = 0;
	for (int i = 0; i < GF16_HASH; ++i)
		if (hash_in_GF[i] != signed_gf[i]) {
			result = -1;
			break;
		}
	VPT(vb);
	VPA(5, va, vb);
#ifdef RCT_VPROF
	if (++vpc == VPROF_AT) {
		fprintf(stderr, "[VF-PROF n=%d] s0=%lu s1=%lu s2=%lu s3=%lu s4=%lu s5=%lu  (per-call avg)\n",
		        VPROF_AT,
		        (unsigned long)(vpf[0] / VPROF_AT), (unsigned long)(vpf[1] / VPROF_AT),
		        (unsigned long)(vpf[2] / VPROF_AT), (unsigned long)(vpf[3] / VPROF_AT),
		        (unsigned long)(vpf[4] / VPROF_AT), (unsigned long)(vpf[5] / VPROF_AT));
	}
#endif
	return result;
}

#endif

_Static_assert(sizeof(expanded_SK) == sizeof(rct_skx_t), "rct_skx_t size error");
_Static_assert(sizeof(expanded_PK) == sizeof(rct_pk_t), "rct_pk_t size error");

int SNOVA_NAMESPACE(genkeys)(uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
	return rct_genkeys(pk, sk, seed);
}
int SNOVA_NAMESPACE(sk_expand)(expanded_SK* skx, const uint8_t *sk) {
	rct_sk_expand(sk, (rct_skx_t *)skx);
	return 0;
}
int SNOVA_NAMESPACE(sign)(const expanded_SK* skx, uint8_t *sig, const uint8_t *digest, const size_t len_digest,
                          const uint8_t *salt) {
	return rct_sign_expanded((rct_skx_t *)skx, sig, digest, len_digest, salt);
}
int SNOVA_NAMESPACE(pk_expand)(expanded_PK* pkx, const uint8_t *pk) {
	return rct_pk_expand((rct_pk_t *)pkx, pk);
}
int SNOVA_NAMESPACE(verify)(const expanded_PK* pkx, const uint8_t *sig, const uint8_t *digest, const size_t len_digest) {
	return rct_verify((const rct_pk_t *)pkx, sig, digest, len_digest);
}

#endif
