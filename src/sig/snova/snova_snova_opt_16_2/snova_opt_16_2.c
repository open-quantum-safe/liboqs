// SPDX-License-Identifier: MIT

/**
 * Optimized q=16 implementation for l=2. Uses vectorization if available
 *
 * Copyright (c) 2026 SNOVA TEAM
 */

#include <stdalign.h>
#include <stdint.h>
#include <string.h>

#include "snova.h"
#include "symmetric.h"

#if SNOVA_q != 16
#error "SNOVA_q != 16"
#include "stop"
#endif

#if SNOVA_l != 2
#error "SNOVA_l must be 2"
#include "stop"
#endif

#ifndef USE_VECTOR
#define USE_VECTOR __AVX2__ || __ARM_NEON
#endif

typedef uint8_t gf_t;

static inline uint16_t gf16_expand(const gf_t a) {
	uint16_t val = a | (a << 3) | (a << 6) | (a << 9);
	return val & 0x1111;
}

static inline uint16_t gf16_compress(const uint16_t a) {
	uint16_t val = (a & 0xf) ^ ((a & 0xf0) >> 3) ^ ((a & 0xf00) >> 6) ^ ((a & 0xf000) >> 9);
	return (val ^ ((val & 0xf0) >> 3) ^ (val >> 4)) & 0xf;
}

static inline uint16_t gf16_cleanup(const uint16_t val) {
	return gf16_expand(gf16_compress(val));
}

/**
 * Constant time function. CT is according to valgrind
 */
static inline uint32_t ct_is_not_zero(uint8_t val) {
	// return (val | (val >> 1) | (val >> 2) | (val >> 3)) & 1;
	return val != 0;
}

/**
 * Constant time GF(16) inverse
 *
 * Use that x^q = x and therefore x^(q-2) = x^-1
 */
static uint16_t ct_gf_inverse(uint16_t val) {
	uint16_t fact = gf16_compress(val * gf16_expand(val));
	uint16_t res = fact;

	fact = gf16_compress(fact * gf16_expand(fact));
	res = gf16_compress(res * gf16_expand(fact));

	fact = gf16_compress(fact * gf16_expand(fact));
	res = gf16_compress(res * gf16_expand(fact));

	return gf16_expand(res);
}

/**
 * Initialization
 */
static int first_time = 1;

#if FIXED_ABQ
static void gen_fixed_ABQ(const char *abq_seed);
#endif

static gf_t gf_multtab[SNOVA_q * SNOVA_q] = {0};
static gf_t gf_invtab[SNOVA_q] = {0};
static gf_t gf_addtab[SNOVA_q * SNOVA_q] = {0};
static gf_t gf_S[SNOVA_l * SNOVA_l2] = {0};
static uint16_t gf_Sx[SNOVA_l * SNOVA_l2] = {0};

static inline gf_t gf_mult(const gf_t a, const gf_t b) {
	return gf_multtab[a * SNOVA_q + b];
}

#if SNOVA_l == 5
static inline gf_t gf_add(const gf_t a, const gf_t b) {
	return gf_addtab[a * SNOVA_q + b];
}
#endif

static inline void gf_set_add(gf_t *a, const gf_t b) {
	*a = gf_addtab[*a * SNOVA_q + b];
}

static inline gf_t gf_sub(const gf_t a, const gf_t b) {
#if SNOVA_q != 16
	return gf_addtab[a * SNOVA_q + (SNOVA_q - b) % SNOVA_q];
#else
	return gf_addtab[a * SNOVA_q + b];
#endif
}

static void init_gf_tables(void) {
	// GF(16)
	uint8_t F_star[15] = {1, 2, 4, 8, 3, 6, 12, 11, 5, 10, 7, 14, 15, 13, 9}; // Z2[x]/(x^4+x+1)
	for (int i1 = 0; i1 < 16; i1++) {
		gf_multtab[i1] = 0;
		gf_multtab[i1 * SNOVA_q] = 0;
	}
	for (int i1 = 0; i1 < SNOVA_q - 1; i1++)
		for (int j1 = 0; j1 < SNOVA_q - 1; j1++) {
			gf_multtab[F_star[i1] * SNOVA_q + F_star[j1]] = F_star[(i1 + j1) % (SNOVA_q - 1)];
		}

	for (int i1 = 0; i1 < SNOVA_q; i1++)
		for (int j1 = 0; j1 < SNOVA_q; j1++) {
			gf_addtab[i1 * SNOVA_q + j1] = (i1 ^ j1);
		}
	// Use that x^q = x and therefore x^(q-2) = x^-1
	for (int i1 = 0; i1 < SNOVA_q; i1++) {
		gf_t val = i1;
		for (int j1 = 3; j1 < SNOVA_q; j1++) {
			val = gf_mult(val, i1);
		}
		gf_invtab[i1] = val;
	}
}

#if USE_VECTOR
/**
 * Vectorization
 */
#if __AVX2__
#include <immintrin.h>
typedef __m256i VECTOR;
#define VEC_LENGTH 32
#define VEC_BYTE(vect, idx) (((uint8_t *)vect)[idx])
#define VEC_SHUFFLE _mm256_shuffle_epi8
#define VEC_CMP_GT _mm256_cmpgt_epi32
#define VEC_CMP_EQ _mm256_cmpeq_epi8

#elif __ARM_NEON
#include <arm_neon.h>
typedef uint8x16_t VECTOR;
#define VEC_LENGTH 16
#define VEC_BYTE(vect, idx) vect[(idx) / 16][(idx) % 16]
#define VEC_SHUFFLE vqtbl1q_u8
#define VEC_CMP_GT vcgtq_u8
#define VEC_CMP_EQ vceqq_u8

#else
#error "Vectorization not supported"
#endif

static alignas(16 * VEC_LENGTH) uint8_t mt4b2_16[256][VEC_LENGTH] = {0};
static VECTOR *mtk2_16 = (VECTOR *)mt4b2_16;

// inverse table, runs in constant time
static VECTOR vector_inv_table = {0};
static VECTOR l_mask = {0};

// Table used by vtl_ct_multtab
static VECTOR vtl_multmask1, vtl_multmask2, vtl_multmask4, vtl_multmask8;
static VECTOR vtl_mult_table1, vtl_mult_table2, vtl_mult_table4, vtl_mult_table8;
static VECTOR zero256 = {0};

static int init_vector_table(void) {
	static int vector_table_init_flag = 0;
	if (vector_table_init_flag) {
		return 0;
	}
	vector_table_init_flag = 1;

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			for (int k = 0; k < 16; ++k) {
				uint8_t temp = (gf_mult(i, k) << 4) ^ gf_mult(j, k);
				mt4b2_16[i * 16 + j][k] = temp;
#if VEC_LENGTH > 16
				mt4b2_16[i * 16 + j][k + 16] = temp;
#endif
			}
		}
	}
#if __ARM_NEON
	// GF16 inverse table
	uint8_t inv_table[16] = {0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8};
	vector_inv_table = vld1q_u8(inv_table);

	uint8_t numf = 0x0f;
	l_mask = vld1q_dup_u8(&numf);

	uint8_t num1 = 1;
	uint8_t num2 = 2;
	uint8_t num4 = 4;
	uint8_t num8 = 8;
	vtl_multmask1 = vld1q_dup_u8(&num1);
	vtl_multmask2 = vld1q_dup_u8(&num2);
	vtl_multmask4 = vld1q_dup_u8(&num4);
	vtl_multmask8 = vld1q_dup_u8(&num8);

#else
	// GF16 inverse table
#if VEC_LENGTH > 16
	vector_inv_table = _mm256_setr_epi8(0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8, 0, 1, 9, 14, 13, 11, 7, 6, 15, 2,
	                                    12, 5, 10, 4, 3, 8);
#else
	vector_inv_table = _mm_setr_epi8(0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8);
#endif
	l_mask = _mm256_set1_epi8(0x0f);

	vtl_multmask1 = _mm256_set1_epi8(1);
	vtl_multmask2 = _mm256_set1_epi8(2);
	vtl_multmask4 = _mm256_set1_epi8(4);
	vtl_multmask8 = _mm256_set1_epi8(8);
#endif

	vtl_mult_table1 = mtk2_16[1];
	vtl_mult_table2 = mtk2_16[2];
	vtl_mult_table4 = mtk2_16[4];
	vtl_mult_table8 = mtk2_16[8];

	return 1;
}

// Constant time VTL table
static inline VECTOR vtl_ct_multtab(uint8_t val) {
#if __ARM_NEON
	VECTOR val256 = vld1q_dup_u8(&val);
#else
	VECTOR val256 = _mm256_set1_epi8(val);
#endif

	return (vtl_mult_table1 & VEC_CMP_GT(val256 & vtl_multmask1, zero256)) ^
	       (vtl_mult_table2 & VEC_CMP_GT(val256 & vtl_multmask2, zero256)) ^
	       (vtl_mult_table4 & VEC_CMP_GT(val256 & vtl_multmask4, zero256)) ^
	       (vtl_mult_table8 & VEC_CMP_GT(val256 & vtl_multmask8, zero256));
}
/**
 * End Vectorization
 */
#else
#define init_vector_table(x)
#define VEC_LENGTH 8
#endif

#define VEC_LENGTH2 (VEC_LENGTH / 2)

#define SNOVA_nl (SNOVA_n * SNOVA_l)
#define SNOVA_mnl16 ((SNOVA_m1 * SNOVA_n * SNOVA_l + VEC_LENGTH2 - 1) / VEC_LENGTH2)
#define SNOVA_mnl (SNOVA_mnl16 * VEC_LENGTH2)

#define SNOVA_mvl32 ((SNOVA_m1 * SNOVA_v * SNOVA_l + VEC_LENGTH - 1) / VEC_LENGTH)
#define SNOVA_mvl (SNOVA_mvl32 * VEC_LENGTH)

#define SNOVA_mol16 ((SNOVA_m1 * SNOVA_o * SNOVA_l + VEC_LENGTH2 - 1) / VEC_LENGTH2)
#define SNOVA_mol (SNOVA_mol16 * VEC_LENGTH2)
#define SNOVA_olr16 (SNOVA_o * SNOVA_lr / VEC_LENGTH2 + 1)
#define SNOVA_olr (SNOVA_olr16 * VEC_LENGTH2)
#define SNOVA_vl (SNOVA_v * SNOVA_l)
#define SNOVA_ol16 ((SNOVA_o * SNOVA_l + VEC_LENGTH2 - 1) / VEC_LENGTH2)
#define SNOVA_ol (SNOVA_ol16 * VEC_LENGTH2)

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

// Set the irreducible S matrix
static void set_S(gf_t *gf_S1) {
	for (int i1 = 0; i1 < SNOVA_l; i1++)
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			gf_S1[i1 * SNOVA_l + j1] = 8 - (i1 + j1);
		}
#if SNOVA_l == 5
	gf_S1[SNOVA_l2 - 1] = 9;
#endif
}

static void gen_S_array(void) {
	memset(gf_S, 0, sizeof(gf_S));

	for (int i1 = 0; i1 < SNOVA_l; i1++) {
		gf_S[i1 * SNOVA_l + i1] = 1;
	}

#if SNOVA_l > 1
	set_S(&gf_S[1 * SNOVA_l2]);

	for (int i1 = 2; i1 < SNOVA_l; i1++) {
		gf_mat_mul(&gf_S[i1 * SNOVA_l2], &gf_S[1 * SNOVA_l2], &gf_S[(i1 - 1) * SNOVA_l2]);
	}
#endif

	for (int i1 = 0; i1 < SNOVA_l * SNOVA_l2; i1++) {
		gf_Sx[i1] = gf16_expand(gf_S[i1]);
	}
}

static void snova_init(void) {
	first_time = 0;
	init_gf_tables();
	gen_S_array();
#if FIXED_ABQ
// Special values only for parameter sets that are not recommended for Round 3.
#if !ROUND2_T12 && SNOVA_l == 2
#if SNOVA_o == 17
	gen_fixed_ABQ("SNOVA_ABQ_2");
#elif SNOVA_o == 25
	gen_fixed_ABQ("SNOVA_ABQ_3");
#elif SNOVA_o == 33
	gen_fixed_ABQ("SNOVA_ABQ_4");
#else
#error "Unsupported SNOVA_o for SNOVA_l ==2"
#endif
#else
	gen_fixed_ABQ("SNOVA_ABQ");
#endif
#endif
}

#define SNOVA_INIT           \
    if (first_time)          \
    {                        \
        first_time = 0;      \
        snova_init();        \
        init_vector_table(); \
    }

/**
 * Utilities
 */

#if SNOVA_l == 4

static gf_t gf_mat_det(gf_t *a) {
#define DET_SUB(a, b) (a ^ b)
#define DET_MULT(a, b) gf_multtab[a * SNOVA_q + b]
	gf_t det = 0;
	gf_t det_l;
	gf_t det_r;
#define DET_L(x, y) det_l = DET_SUB(DET_MULT(a[x], a[4 + y]), DET_MULT(a[y], a[4 + x]))
#define DET_R(x, y) det_r = DET_SUB(DET_MULT(a[8 + x], a[12 + y]), DET_MULT(a[8 + y], a[12 + x]))
#define DET22(x1, y1, x2, y2) \
    DET_L(x1, y1);            \
    DET_R(x2, y2);            \
    det ^= DET_MULT(det_l, det_r)
	DET22(0, 1, 2, 3);
	DET22(0, 2, 3, 1);
	DET22(0, 3, 1, 2);
	DET22(1, 2, 0, 3);
	DET22(1, 3, 2, 0);
	DET22(2, 3, 0, 1);

	return det;
}

#else
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
#elif SNOVA_l == 5
	gf_t det_l;
	gf_t det_r;
#define DET_L(x, y) det_l = gf_sub(gf_mult(a[x], a[5 + y]), gf_mult(a[y], a[5 + x]))
#define DET_R2(x, y, z) gf_mult(gf_sub(gf_mult(a[10 + x], a[15 + y]), gf_mult(a[10 + y], a[15 + x])), a[20 + z])
#define DET_R3(x, y, z) det_r = gf_add(DET_R2(x, y, z), gf_add(DET_R2(y, z, x), DET_R2(z, x, y)))
#define DET23(x1, y1, x2, y2, z2) \
    DET_L(x1, y1);                \
    DET_R3(x2, y2, z2);           \
    gf_set_add(&det, gf_mult(det_l, det_r))
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
#endif

static int convert_bytes_to_GF(gf_t *gf_array, const uint8_t *byte_array, size_t num) {
	for (size_t idx = 0; idx < num / 2; idx++) {
		gf_array[2 * idx] = (byte_array[idx] & 0xf) % SNOVA_q;
		gf_array[2 * idx + 1] = (byte_array[idx] >> 4) % SNOVA_q;
	}
	if (num & 1) {
		gf_array[num - 1] = (byte_array[num / 2] & 0xf) % SNOVA_q;
		return byte_array[num / 2] & 0xF0;
	}

	return 0;
}

// Used to compress PK (genkey) and SIG(sign)
static void compress_gf(uint8_t *byte_array, const gf_t *gf_array, size_t num) {
	for (size_t idx = 0; idx < num / 2; idx++) {
		byte_array[idx] = gf_array[2 * idx] ^ (gf_array[2 * idx + 1] << 4);
	}
	if (num & 1) {
		byte_array[num / 2] = gf_array[num - 1];
	}
}

// Used to expand PK(verify) and SIG(verify)
static int expand_gf(gf_t *gf_array, const uint8_t *byte_array, size_t num) {
	return convert_bytes_to_GF(gf_array, byte_array, num);
}

// Used to compress PK (genkey)
static void compress_pk(uint8_t *pk, const gf_t *P22) {
	compress_gf(pk, P22, NUMGF_PK);
}

// Used to expand PK(verify)
static int expand_pk(gf_t *P22, const uint8_t *pk) {
	return expand_gf(P22, pk, NUMGF_PK);
}

/**
 * Expand the public key from a seed.
 */
static void expand_public(gf_t *P_matrix, const uint8_t *seed) {
	uint8_t pk_bytes[NUM_GEN_PUB_BYTES];

	snova_pk_expand(pk_bytes, NUM_GEN_PUB_BYTES, seed, SEED_LENGTH_PUBLIC);

	convert_bytes_to_GF(P_matrix, (uint8_t *)pk_bytes, NUM_GEN_PUB_GF);
}

static inline void hash_combined(uint8_t *hash_out, const uint8_t *m, size_t mlen, const uint8_t *pk_seed,
                                 const uint8_t *salt) {
	shake_t state;
	shake256_init(&state);
#if HASH_PK
	shake_absorb(&state, pk_seed, BYTES_PK_HASH);
#else
	shake_absorb(&state, pk_seed, SEED_LENGTH_PUBLIC);
#endif
	shake_absorb(&state, m, mlen);
	shake_absorb(&state, salt, BYTES_SALT);
	shake_finalize(&state);
	shake_squeeze(hash_out, BYTES_HASH, &state);
}

/**
 * Improve q and calculate Q matrix
 */
static inline void gen_a_FqS(gf_t *Qm, gf_t *q) {
#if ROUND2_T12
	int16_t not_zero = -ct_is_not_zero(q[SNOVA_l - 1]);
	q[SNOVA_l - 1] = (not_zero & q[SNOVA_l - 1]) | ((not_zero ^ -1) & (SNOVA_q - (q[0] + 1 - ct_is_not_zero(q[0]))));
#endif

	for (int i1 = 0; i1 < SNOVA_l2; i1++) {
		uint16_t sum = 0;
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			sum ^= q[j1] * gf_Sx[j1 * SNOVA_l2 + i1];
		}
		Qm[i1] = gf16_compress(sum);
	}
}

static inline void gen_a_FqS_sec(gf_t *Qm, gf_t *q) {
#if ROUND2_T12
	int16_t not_zero = -ct_is_not_zero(q[SNOVA_l - 1]);
	q[SNOVA_l - 1] = (not_zero & q[SNOVA_l - 1]) | ((not_zero ^ -1) & (SNOVA_q - (q[0] + 1 - ct_is_not_zero(q[0]))));
#endif

	for (int i1 = 0; i1 < SNOVA_l2; i1++) {
		uint16_t sum = 0;
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			sum ^= q[j1] * gf_Sx[j1 * SNOVA_l2 + i1];
		}
		Qm[i1] = gf16_compress(sum);
	}
}

/**
 * Expand T12 matrix and coefficients. Shared by genkey and sign
 */
#define SK_BLOCK_SIZE ((SNOVA_o * SNOVA_v * SNOVA_l + 1) / 2)
static void expand_T12(gf_t *T12, const uint8_t *seed) {
	gf_t T12coef[SNOVA_o * SNOVA_v * SNOVA_l];
	gf_t sk_data[(SNOVA_o * SNOVA_v * SNOVA_l + 1) / 2];

	shake256(sk_data, (SNOVA_o * SNOVA_v * SNOVA_l + 1) / 2, seed, SEED_LENGTH_PRIVATE);
	convert_bytes_to_GF(T12coef, sk_data, SNOVA_o * SNOVA_v * SNOVA_l);

	for (size_t i1 = 0; i1 < SNOVA_o * SNOVA_v; i1++) {
		gen_a_FqS_sec(&T12[i1 * SNOVA_l2], &T12coef[i1 * SNOVA_l]);
	}
}

/**
 * Ensure that a matrix is invertible by adding multiples of S
 */
static inline void be_invertible_by_add_aS(gf_t *mat, const gf_t *orig, const int l1, const int l2) {
	memcpy(mat, orig, l1 * l2);
#if ABQ_ALG2
#if SNOVA_l > 1
	if ((l1 == SNOVA_l) && (l2 == SNOVA_l)) {
		for (gf_t f1 = 1; gf_mat_det(mat) == 0; f1++)
			for (int i1 = 0; i1 < SNOVA_l2; i1++) {
				mat[i1] ^= gf16_compress(f1 * gf_Sx[SNOVA_l2 + i1]);
			}
	}
#endif
#endif
}

/**
 * Fix the ABQ to constants
 */
#if FIXED_ABQ
static uint8_t fixed_abq[SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + 2 * SNOVA_l)] = {0};

static uint16_t fixedAm[SNOVA_o * SNOVA_alpha * SNOVA_r2];
static uint16_t fixedBm[SNOVA_o * SNOVA_alpha * SNOVA_lr];
static uint16_t fixedQ1[SNOVA_o * SNOVA_alpha * SNOVA_l2];
static uint16_t fixedQ2[SNOVA_o * SNOVA_alpha * SNOVA_l2];
static uint16_t fixedq1[SNOVA_o * SNOVA_alpha * SNOVA_l];
static uint16_t fixedq2[SNOVA_o * SNOVA_alpha * SNOVA_l];

static void gen_fixed_ABQ(const char *abq_seed) {
	uint8_t rng_out[SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + 2 * SNOVA_l)] = {0};

	shake256(rng_out, SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + 2 * SNOVA_l), (uint8_t *)abq_seed, strlen(abq_seed));
	convert_bytes_to_GF(fixed_abq, rng_out, SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr + 2 * SNOVA_l));

	gf_t *A = fixed_abq;
	gf_t *B = A + SNOVA_o * SNOVA_alpha * SNOVA_r2;
	gf_t *q1 = B + SNOVA_o * SNOVA_alpha * SNOVA_lr;
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;

	uint8_t Am[SNOVA_o * SNOVA_alpha * SNOVA_r2];
	uint8_t Bm[SNOVA_o * SNOVA_alpha * SNOVA_lr];
	uint8_t Q1[SNOVA_o * SNOVA_alpha * SNOVA_l2];
	uint8_t Q2[SNOVA_o * SNOVA_alpha * SNOVA_l2];

	for (int idx = 0; idx < SNOVA_o * SNOVA_alpha; idx++) {
		be_invertible_by_add_aS(&(Am[idx * SNOVA_r2]), &A[idx * SNOVA_r2], SNOVA_r, SNOVA_r);
		be_invertible_by_add_aS(&(Bm[idx * SNOVA_lr]), &B[idx * SNOVA_lr], SNOVA_r, SNOVA_l);
		gen_a_FqS(&(Q1[idx * SNOVA_l2]), &q1[idx * SNOVA_l]);
		gen_a_FqS(&(Q2[idx * SNOVA_l2]), &q2[idx * SNOVA_l]);

#if ROUND2_T12
		if (!q1[idx * SNOVA_l + SNOVA_l - 1]) {
			q1[idx * SNOVA_l + SNOVA_l - 1] = SNOVA_q - (q1[idx * SNOVA_l] + (q1[idx * SNOVA_l] == 0));
		}
		if (!q2[idx * SNOVA_l + SNOVA_l - 1]) {
			q2[idx * SNOVA_l + SNOVA_l - 1] = SNOVA_q - (q2[idx * SNOVA_l] + (q2[idx * SNOVA_l] == 0));
		}
#endif
	}

	for (int idx = 0; idx < SNOVA_o * SNOVA_alpha * SNOVA_r2; ++idx) {
		fixedAm[idx] = gf16_expand(Am[idx]);
	}
	for (int idx = 0; idx < SNOVA_o * SNOVA_alpha * SNOVA_lr; ++idx) {
		fixedBm[idx] = gf16_expand(Bm[idx]);
	}
	for (int idx = 0; idx < SNOVA_o * SNOVA_alpha * SNOVA_l2; ++idx) {
		fixedQ1[idx] = gf16_expand(Q1[idx]);
		fixedQ2[idx] = gf16_expand(Q2[idx]);
	}
	for (int idx = 0; idx < SNOVA_o * SNOVA_alpha * SNOVA_l; ++idx) {
		fixedq1[idx] = gf16_expand(q1[idx]);
		fixedq2[idx] = gf16_expand(q2[idx]);
	}
}
#else

/**
 * Use last part of the P matrix to establish ABQ
 */
static void gen_ABQ(gf_t *A, gf_t *Am, gf_t *Bm, gf_t *Q1m, gf_t *Q2m) {
	gf_t *B = A + SNOVA_o * SNOVA_alpha * SNOVA_r2;
	gf_t *q1 = B + SNOVA_o * SNOVA_alpha * SNOVA_lr;
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;

	for (size_t idx = 0; idx < SNOVA_o * SNOVA_alpha; idx++) {
		be_invertible_by_add_aS(&Am[idx * SNOVA_r2], &A[idx * SNOVA_r2], SNOVA_r, SNOVA_r);
		be_invertible_by_add_aS(&Bm[idx * SNOVA_lr], &B[idx * SNOVA_lr], SNOVA_r, SNOVA_l);
		gen_a_FqS(&Q1m[idx * SNOVA_l2], &q1[idx * SNOVA_l]);
		gen_a_FqS(&Q2m[idx * SNOVA_l2], &q2[idx * SNOVA_l]);
	}
}
#endif

/**
 * Optimized version of genkey.
 */
int SNOVA_NAMESPACE(genkeys)(uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
	SNOVA_INIT

	/**
	 * Gen T12 matrix
	 */
	gf_t T12[SNOVA_o * SNOVA_v * SNOVA_l2];

	expand_T12(T12, seed + SEED_LENGTH_PUBLIC);

	/**
	 * Gen Public matrix but not ABQ
	 */
	gf_t P_matrix[NUM_PUB_GF];

	expand_public(P_matrix, seed);

	/**
	 * Calculate F12 matrix, use P11
	 */
	gf_t *P11 = P_matrix;
	gf_t *P12 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;

	alignas(32) uint16_t F12[SNOVA_mvl * SNOVA_ol] = {0};

	uint16_t T12x[SNOVA_o * SNOVA_v * SNOVA_l2];
	for (int i1 = 0; i1 < SNOVA_o * SNOVA_v * SNOVA_l2; i1++) {
		T12x[i1] = gf16_expand(T12[i1]);
	}

#if USE_VECTOR
	alignas(32) uint8_t F12x[SNOVA_mvl * SNOVA_ol] = {0};
	alignas(32) uint8_t P_mat16[SNOVA_mvl * SNOVA_vl];
#else
	alignas(32) uint16_t F12x[SNOVA_mvl * SNOVA_ol] = {0};
	alignas(32) uint16_t P_mat16[SNOVA_mvl * SNOVA_vl];
#endif

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_v; ni++)
			for (int nj = 0; nj < SNOVA_v; nj++)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						P_mat16[(ni * SNOVA_l + k1) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + i1] =
						    P11[((mi * SNOVA_v + nj) * SNOVA_v + ni) * SNOVA_l2 + i1 * SNOVA_l + k1];

#if USE_VECTOR
	for (int ni = 0; ni < SNOVA_v; ni++)
		for (int nk = 0; nk < SNOVA_o; nk++)
			for (int mi = 0; mi < SNOVA_mvl32; mi++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++) {
						VECTOR *f12_256 = (VECTOR *)&F12x[(nk * SNOVA_l + j1) * SNOVA_mvl];
						VECTOR t12_256 = vtl_ct_multtab(T12[(ni * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + j1]);
						VECTOR *P_mat256 = (VECTOR *)&P_mat16[(ni * SNOVA_l + k1) * SNOVA_mvl];

						f12_256[mi] ^= VEC_SHUFFLE(t12_256, P_mat256[mi]);
					}
#else
	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_v; ni++)
			for (int nk = 0; nk < SNOVA_o; nk++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int nj = 0; nj < SNOVA_v * SNOVA_l; nj++)
							F12x[(nk * SNOVA_l + j1) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj] ^=
							    T12x[(ni * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + j1] *
							    P_mat16[(ni * SNOVA_l + k1) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj];

	for (int i1 = 0; i1 < SNOVA_mvl * SNOVA_ol; i1++) {
		F12x[i1] = gf16_compress(F12x[i1]);
	}
#endif

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_o; ni++)
			for (int j1 = 0; j1 < SNOVA_l; j1++)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						F12[(nj * SNOVA_l + k1) * SNOVA_mol + mi * SNOVA_o * SNOVA_l + ni * SNOVA_l + j1] =
						    F12x[(ni * SNOVA_l + j1) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + k1] ^
						    P12[((mi * SNOVA_v + nj) * SNOVA_o + ni) * SNOVA_l2 + k1 * SNOVA_l + j1];

	uint16_t P22x[SNOVA_mol * SNOVA_ol] = {0};
	uint16_t P22y[SNOVA_mol * SNOVA_ol] = {0};

	for (int nj = 0; nj < SNOVA_v; nj++)
		for (int nk = 0; nk < SNOVA_o; nk++)
			for (int mi = 0; mi < SNOVA_mol; mi++)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						P22x[(nk * SNOVA_l + i1) * SNOVA_mol + mi] ^= T12x[(nj * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + k1] *
						    F12[(nj * SNOVA_l + k1) * SNOVA_mol + mi];

	/**
	 * Calculate P22. Uses P21
	 */
	uint16_t P21x[SNOVA_mol * SNOVA_vl];

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int nk = 0; nk < SNOVA_o; nk++)
			for (int nj = 0; nj < SNOVA_v; nj++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						P21x[(nj * SNOVA_l + k1) * SNOVA_mol + (mi * SNOVA_o + nk) * SNOVA_l + i1] =
						    P21[((mi * SNOVA_o + nk) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + k1];

	for (int nj = 0; nj < SNOVA_v; nj++)
		for (int j1 = 0; j1 < SNOVA_l; j1++)
			for (int ni = 0; ni < SNOVA_o; ni++)
				for (int mi = 0; mi < SNOVA_mol; mi++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						P22y[(ni * SNOVA_l + j1) * SNOVA_mol + mi] ^= P21x[(nj * SNOVA_l + k1) * SNOVA_mol + mi] *
						    T12x[(nj * SNOVA_o + ni) * SNOVA_l2 + k1 * SNOVA_l + j1];

	gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_o; ni++)
			for (int nk = 0; nk < SNOVA_o; nk++)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						P22[((mi * SNOVA_o + nk) * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + j1] =
						    gf16_compress(P22x[(nk * SNOVA_l + i1) * SNOVA_mol + mi * SNOVA_o * SNOVA_l + ni * SNOVA_l + j1] ^
						                  P22y[(ni * SNOVA_l + j1) * SNOVA_mol + (mi * SNOVA_o + nk) * SNOVA_l + i1]);

	/**
	 * Output public and secret keys
	 */
	memcpy(pk, seed, SEED_LENGTH_PUBLIC);
	compress_pk(pk + SEED_LENGTH_PUBLIC, P22);
	memcpy(sk, seed, SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE);

#if HASH_PK
	shake256(sk + SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE, BYTES_PK_HASH, pk, BYTES_PK);
#endif

	return 0;
}

/**
 * SK expansion.
 */
int SNOVA_NAMESPACE(sk_expand)(expanded_SK *skx, const uint8_t *sk) {
	SNOVA_INIT

	memcpy(skx->pk_seed, sk, SEED_LENGTH_PUBLIC);
	memcpy(skx->sk_seed, sk + SEED_LENGTH_PUBLIC, SEED_LENGTH_PRIVATE);
#if HASH_PK
	memcpy(skx->pk_hash, sk + SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE, BYTES_PK_HASH);
#endif

	alignas(32) gf_t T12[SNOVA_o * SNOVA_v * SNOVA_l2];

	expand_T12(T12, skx->sk_seed);
	for (int i1 = 0; i1 < SNOVA_o * SNOVA_v * SNOVA_l2; i1++) {
		skx->T12[i1] = gf16_expand(T12[i1]);
	}

	gf_t P_matrix[NUM_PUB_GF];

	expand_public(P_matrix, skx->pk_seed);

	/**
	 * Calculate F12, F21
	 */
	gf_t *P11 = P_matrix;
	gf_t *P12 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;

	alignas(32) uint16_t pub[SNOVA_v * SNOVA_l * SNOVA_mvl];

	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2; i1++) {
		pub[i1] = P11[i1];
	}

	// Reorder
	for (int nj = 0; nj < SNOVA_v; ++nj)
		for (int k1 = 0; k1 < SNOVA_l; k1++)
			for (int mi = 0; mi < SNOVA_m1; ++mi)
				for (int ni = 0; ni < SNOVA_v; ++ni)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						skx->P11[(nj * SNOVA_l + k1) * SNOVA_m1 * SNOVA_v * SNOVA_l + ((mi * SNOVA_v + ni) * SNOVA_l + i1)] =
						    pub[((mi * SNOVA_v + nj) * SNOVA_v + ni) * SNOVA_l2 + k1 * SNOVA_l + i1];

#if USE_VECTOR
	alignas(32) uint8_t F21[SNOVA_mvl * SNOVA_ol] = {0};
	alignas(32) uint8_t P_mat16[SNOVA_mvl * SNOVA_vl];
#else
	alignas(32) uint16_t F21[SNOVA_mvl * SNOVA_ol] = {0};
	alignas(32) uint16_t P_mat16[SNOVA_mvl * SNOVA_vl];
#endif

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int nk = 0; nk < SNOVA_v; nk++)
			for (int nj = 0; nj < SNOVA_v; nj++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						P_mat16[(k1 * SNOVA_v + nk) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + j1] =
						    pub[((mi * SNOVA_v + nk) * SNOVA_v + nj) * SNOVA_l2 + k1 * SNOVA_l + j1];

#if USE_VECTOR
	alignas(32) uint8_t F21x[SNOVA_mvl * SNOVA_ol];

	for (int nk = 0; nk < SNOVA_v; nk++)
		for (int ni = 0; ni < SNOVA_o; ni++)
			for (int mi = 0; mi < SNOVA_mvl32; mi++)
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					VECTOR *f21_256 = (VECTOR *)&F21[(0 * SNOVA_o + ni) * SNOVA_mvl];
					VECTOR t12_256 = vtl_ct_multtab(T12[(nk * SNOVA_o + ni) * SNOVA_l2 + 0 * SNOVA_l + k1]) ^
					                 (vtl_ct_multtab(T12[(nk * SNOVA_o + ni) * SNOVA_l2 + 1 * SNOVA_l + k1]) << 4);
					VECTOR *P_mat256 = (VECTOR *)&P_mat16[(k1 * SNOVA_v + nk) * SNOVA_mvl];

					f21_256[mi] ^= VEC_SHUFFLE(t12_256, P_mat256[mi]);
				}

	for (int ni = 0; ni < SNOVA_o; ni++)
		for (int mi = 0; mi < SNOVA_mvl; mi++) {
			F21x[(0 * SNOVA_o + ni) * SNOVA_mvl + mi] = F21[(0 * SNOVA_o + ni) * SNOVA_mvl + mi] & 0xf;
			F21x[(1 * SNOVA_o + ni) * SNOVA_mvl + mi] = F21[(0 * SNOVA_o + ni) * SNOVA_mvl + mi] >> 4;
		}

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_o; ni++)
			for (int j1 = 0; j1 < SNOVA_l; j1++)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						skx->F21[((mi * SNOVA_v + nj) * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + j1] =
						    F21x[(i1 * SNOVA_o + ni) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + j1] ^
						    P21[((mi * SNOVA_o + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + j1];
#else
	for (int nk = 0; nk < SNOVA_v; nk++)
		for (int ni = 0; ni < SNOVA_o; ni++)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int mi = 0; mi < SNOVA_mvl; mi++)
						F21[(i1 * SNOVA_o + ni) * SNOVA_mvl + mi] ^=
						    skx->T12[(nk * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + k1] *
						    P_mat16[(k1 * SNOVA_v + nk) * SNOVA_mvl + mi];

	for (int i1 = 0; i1 < SNOVA_mvl * SNOVA_ol; i1++) {
		F21[i1] = gf16_compress(F21[i1]);
	}

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_o; ni++)
			for (int j1 = 0; j1 < SNOVA_l; j1++)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						skx->F21[((mi * SNOVA_v + nj) * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + j1] =
						    F21[(i1 * SNOVA_o + ni) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + j1] ^
						    P21[((mi * SNOVA_o + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + j1];
#endif

#if USE_VECTOR
	alignas(32) uint8_t F12[SNOVA_ol * SNOVA_mvl] = {0};
	alignas(32) uint8_t P_mat16b[SNOVA_mvl * SNOVA_vl];
#else
	alignas(32) uint16_t F12[SNOVA_ol * SNOVA_mvl] = {0};
	alignas(32) uint16_t P_mat16b[SNOVA_mvl * SNOVA_vl];
#endif

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int nj = 0; nj < SNOVA_v; nj++)
			for (int ni = 0; ni < SNOVA_v; ni++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						P_mat16b[(k1 * SNOVA_v + ni) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + i1] =
						    pub[((mi * SNOVA_v + nj) * SNOVA_v + ni) * SNOVA_l2 + i1 * SNOVA_l + k1];

#if USE_VECTOR
	alignas(32) uint8_t F12x[SNOVA_mvl * SNOVA_ol];

	for (int nk = 0; nk < SNOVA_o; nk++)
		for (int ni = 0; ni < SNOVA_v; ni++)
			for (int mi = 0; mi < SNOVA_mvl32; mi++)
				for (int k1 = 0; k1 < SNOVA_l; k1++) {
					VECTOR *f12_256 = (VECTOR *)&F12[(0 * SNOVA_o + nk) * SNOVA_mvl];
					VECTOR t12_256 = vtl_ct_multtab(T12[(ni * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + 0]) ^
					                 (vtl_ct_multtab(T12[(ni * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + 1]) << 4);
					VECTOR *P_mat256 = (VECTOR *)&P_mat16b[(k1 * SNOVA_v + ni) * SNOVA_mvl];

					f12_256[mi] ^= VEC_SHUFFLE(t12_256, P_mat256[mi]);
				}

	for (int nk = 0; nk < SNOVA_o; nk++)
		for (int mi = 0; mi < SNOVA_mvl; mi++) {
			F12x[(0 * SNOVA_o + nk) * SNOVA_mvl + mi] = F12[(0 * SNOVA_o + nk) * SNOVA_mvl + mi] & 0xf;
			F12x[(1 * SNOVA_o + nk) * SNOVA_mvl + mi] = F12[(0 * SNOVA_o + nk) * SNOVA_mvl + mi] >> 4;
		}

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int nk = 0; nk < SNOVA_o; nk++)
			for (int j1 = 0; j1 < SNOVA_l; j1++)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						skx->F12[((mi * SNOVA_v + nj) * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + j1] =
						    F12x[(j1 * SNOVA_o + nk) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + i1] ^
						    P12[((mi * SNOVA_v + nj) * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + j1];

#else
	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int ni = 0; ni < SNOVA_v; ni++)
			for (int nk = 0; nk < SNOVA_o; nk++)
				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int nj = 0; nj < SNOVA_v * SNOVA_l; nj++)
							F12[(nk * SNOVA_l + j1) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj] ^=
							    skx->T12[(ni * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + j1] *
							    P_mat16b[(k1 * SNOVA_v + ni) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj];

	for (int i1 = 0; i1 < SNOVA_mvl * SNOVA_ol; i1++) {
		F12[i1] = gf16_compress(F12[i1]);
	}

	for (int mi = 0; mi < SNOVA_m1; mi++)
		for (int nk = 0; nk < SNOVA_o; nk++)
			for (int j1 = 0; j1 < SNOVA_l; j1++)
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						skx->F12[((mi * SNOVA_v + nj) * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + j1] =
						    F12[(nk * SNOVA_l + j1) * SNOVA_mvl + mi * SNOVA_v * SNOVA_l + nj * SNOVA_l + i1] ^
						    P12[((mi * SNOVA_v + nj) * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + j1];
#endif

#if !FIXED_ABQ
	// Generate ABQ
	gf_t Am[SNOVA_o * SNOVA_alpha * SNOVA_r2];
	gf_t Bm[SNOVA_o * SNOVA_alpha * SNOVA_lr];
	gf_t Q1[SNOVA_o * SNOVA_alpha * SNOVA_l2];
	gf_t Q2[SNOVA_o * SNOVA_alpha * SNOVA_l2];

	gf_t *aptr = P_matrix + SNOVA_m1 * (SNOVA_v * SNOVA_v + 2 * SNOVA_v * SNOVA_o) * SNOVA_l2;
	gf_t *q1 = aptr + SNOVA_o * SNOVA_alpha * (SNOVA_r2 + SNOVA_lr);
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;

	gf_t *A = P_matrix + (SNOVA_m1 * (SNOVA_n * SNOVA_n - SNOVA_o * SNOVA_o)) * SNOVA_l2;

	gen_ABQ(A, Am, Bm, Q1, Q2);

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_r2; i1++) {
		skx->Am[i1] = gf16_expand(Am[i1]);
	}
	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_lr; i1++) {
		skx->Bm[i1] = gf16_expand(Bm[i1]);
	}
	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_l2; i1++) {
		skx->Q1[i1] = gf16_expand(Q1[i1]);
		skx->Q2[i1] = gf16_expand(Q2[i1]);
	}

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_l; i1++) {
		skx->q1[i1] = gf16_expand(q1[i1]);
		skx->q2[i1] = gf16_expand(q2[i1]);
	}
#endif

	return 0;
}

/**
 * Optimized version of Sign. Deterministic using the salt provided
 */
int SNOVA_NAMESPACE(sign)(const expanded_SK *skx, uint8_t *sig, const uint8_t *digest, const size_t len_digest,
                          const uint8_t *salt) {
	SNOVA_INIT

#if FIXED_ABQ
#define Am fixedAm
#define Bm fixedBm
#define Q1 fixedQ1
#define Q2 fixedQ2
#define q1 fixedq1
#define q2 fixedq2
#else
#define Am skx->Am
#define Bm skx->Bm
#define Q1 skx->Q1
#define Q2 skx->Q2
#define q1 skx->q1
#define q2 skx->q2
#endif

	// Calculate message has of size l^2o
	gf_t hash_in_GF16[GF16_HASH];

	uint8_t sign_hashb[BYTES_HASH];
#if HASH_PK
	hash_combined(sign_hashb, digest, len_digest, skx->pk_hash, salt);
#else
	hash_combined(sign_hashb, digest, len_digest, skx->pk_seed, salt);
#endif
	expand_gf(hash_in_GF16, sign_hashb, GF16_HASH);

	// Find a solution for T.X
	alignas(32) uint16_t gauss16[SNOVA_o * SNOVA_lr][SNOVA_olr];
	gf_t solution[SNOVA_o * SNOVA_lr] = {0};
	gf_t signature_in_GF[SNOVA_n * SNOVA_lr] = {0};
	int flag_redo = 1;
	uint8_t num_sign = 0;

	do {
		memset(gauss16, 0, sizeof(gauss16));
		num_sign++;
		if (num_sign == 255) {
			// Probability of getting here is about q^{-255}
			memset(sig, 0, BYTES_SIGNATURE);
			return -1;
		}
		flag_redo = 0;

		// generate the vinegar value
		uint8_t vinegar_in_byte[NUM_GEN_SEC_BYTES];
		shake_t v_instance;

		shake256_init(&v_instance);
		shake_absorb(&v_instance, skx->sk_seed, SEED_LENGTH_PRIVATE);
		shake_absorb(&v_instance, digest, BYTES_DIGEST);
		shake_absorb(&v_instance, salt, BYTES_SALT);
		shake_absorb(&v_instance, &num_sign, 1);
		shake_finalize(&v_instance);
		shake_squeeze(vinegar_in_byte, NUM_GEN_SEC_BYTES, &v_instance);

		expand_gf(signature_in_GF, vinegar_in_byte, SNOVA_v * SNOVA_lr);

		// Calculate Fvv
		uint16_t Fvv_in_GF16Matrix[SNOVA_o * SNOVA_lr] = {0};

		/**
		 * Whip signature
		 */
		uint16_t whipped_sig[SNOVA_l * SNOVA_v * SNOVA_lr] = {0};

		for (int ab = 0; ab < SNOVA_l; ++ab)
			for (int ni = 0; ni < SNOVA_v; ++ni)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						for (int k1 = 0; k1 < SNOVA_l; k1++)
							whipped_sig[(ab * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r + j1] ^=
							    gf_Sx[ab * SNOVA_l2 + i1 * SNOVA_l + k1] * signature_in_GF[ni * SNOVA_lr + k1 * SNOVA_r + j1];

		for (int i1 = 0; i1 < SNOVA_l * SNOVA_v * SNOVA_lr; i1++) {
			whipped_sig[i1] = gf16_cleanup(whipped_sig[i1]);
		}

		/**
		 * Evaluate whipped central map
		 */
		uint16_t sum_t0[SNOVA_mvl * SNOVA_lr] = {0};
		uint16_t sum_t1[SNOVA_m1 * SNOVA_l2 * SNOVA_r2] = {0};

		uint16_t whipped_sig2[SNOVA_vl * SNOVA_lr] = {0};
		uint16_t sum_t1s[SNOVA_m1 * SNOVA_l2 * SNOVA_r2] = {0};

		for (int ab = 0; ab < SNOVA_l; ++ab)
			for (int ni = 0; ni < SNOVA_v; ++ni)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						whipped_sig2[ni * SNOVA_l * SNOVA_lr + i1 * SNOVA_lr + ab * SNOVA_r + j1] =
						    whipped_sig[(ab * SNOVA_v + ni) * SNOVA_lr + i1 * SNOVA_r + j1];

		for (int ni = 0; ni < SNOVA_v; ni++)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int mi = 0; mi < SNOVA_mvl; mi++)
					for (int a1 = 0; a1 < SNOVA_lr; a1++) {
						sum_t0[a1 * SNOVA_mvl + mi] ^= skx->P11[(ni * SNOVA_l + i1) * SNOVA_m1 * SNOVA_v * SNOVA_l + mi] *
						                               whipped_sig2[(ni * SNOVA_l + i1) * SNOVA_lr + a1];
					}

		for (int i1 = 0; i1 < SNOVA_mvl * SNOVA_lr; ++i1) {
			sum_t0[i1] = gf16_compress(sum_t0[i1]);
		}

		// Right
		for (int mi = 0; mi < SNOVA_m1; ++mi)
			for (int nj = 0; nj < SNOVA_vl; nj++)
				for (int a1 = 0; a1 < SNOVA_lr; a1++)
					for (int b1 = 0; b1 < SNOVA_lr; b1++)
						sum_t1[mi * SNOVA_lr * SNOVA_lr + a1 * SNOVA_lr + b1] ^=
						    sum_t0[(mi * SNOVA_vl + nj) + a1 * SNOVA_mvl] * whipped_sig2[nj * SNOVA_lr + b1];

		for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_l2 * SNOVA_r2; i1++) {
			sum_t1[i1] = gf16_compress(sum_t1[i1]);
		}

		for (int mi = 0; mi < SNOVA_m1; ++mi)
			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							sum_t1s[(mi * SNOVA_l + a1) * SNOVA_l * SNOVA_r2 + b1 * SNOVA_r2 + i1 * SNOVA_r + j1] =
							    sum_t1[(mi * SNOVA_l + a1) * SNOVA_l * SNOVA_r2 + i1 * SNOVA_lr + b1 * SNOVA_r + j1];

		/**
		 * Apply A, B, q1 and q2, aka E matrix
		 */
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
				int mi_prime = i_prime(mi, alpha);

				uint16_t gfm_temp1[SNOVA_r2] = {0};
				uint16_t gfm_temp2[SNOVA_lr] = {0};

				// apply q1 and q2
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					uint16_t gfm_temp0[SNOVA_r2] = {0};

					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								gfm_temp0[i1 * SNOVA_r + j1] ^=
								    sum_t1s[(mi_prime * SNOVA_l2 + a1 * SNOVA_l + b1) * SNOVA_r2 + i1 * SNOVA_r + j1] *
								    q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];

					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
					}

					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++) {
							gfm_temp1[i1 * SNOVA_r + j1] ^=
							    gfm_temp0[i1 * SNOVA_r + j1] * q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1];
						}
				}

				for (int i1 = 0; i1 < SNOVA_r2; i1++) {
					gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
				}

				// A and B
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_r; k1++) {
							gfm_temp2[i1 * SNOVA_l + j1] ^=
							    gfm_temp1[i1 * SNOVA_r + k1] * Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];
						}

				for (int i1 = 0; i1 < SNOVA_lr; i1++) {
					gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
				}

				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_r; k1++) {
							Fvv_in_GF16Matrix[mi * SNOVA_lr + i1 * SNOVA_l + j1] ^=
							    Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] * gfm_temp2[k1 * SNOVA_l + j1];
						}

				// Set the last column of gauss matrix
				for (int i1 = 0; i1 < SNOVA_lr; i1++) {
					gauss16[mi * SNOVA_lr + i1][SNOVA_o * SNOVA_lr] =
					    hash_in_GF16[mi * SNOVA_lr + i1] ^ Fvv_in_GF16Matrix[mi * SNOVA_lr + i1];
				}
			}

		// Whipped F21
		uint16_t whipped_F21[SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr] = {0};

		for (int mi = 0; mi < SNOVA_m1; mi++)
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++)
									whipped_F21[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1] ^=
									    skx->F21[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + i1 * SNOVA_l + k1] *
									    whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r + j1];

		for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr; i1++) {
			whipped_F21[i1] = gf16_compress(whipped_F21[i1]);
		}

		// Whipped F12
		uint16_t whipped_F12[SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr] = {0};

		for (int mi = 0; mi < SNOVA_m1; mi++)
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++)
									whipped_F12[((mi * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1] ^=
									    skx->F12[((mi * SNOVA_v + nj) * SNOVA_o + idx) * SNOVA_l2 + k1 * SNOVA_l + i1] *
									    whipped_sig[(b1 * SNOVA_v + nj) * SNOVA_lr + k1 * SNOVA_r + j1];

		for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_l * SNOVA_o * SNOVA_lr; i1++) {
			whipped_F12[i1] = gf16_compress(whipped_F12[i1]);
		}

		// compute the coefficients of Xo and put into gauss matrix and compute
		// the coefficients of Xo^t and add into gauss matrix
		for (int mi = 0; mi < SNOVA_o; mi++) {
			for (int alpha = 0; alpha < SNOVA_alpha; alpha++) {
				uint16_t gfm_temp0[SNOVA_o * SNOVA_lr] = {0};
				uint16_t gfm_temp1[SNOVA_o * SNOVA_l2] = {0};
				uint16_t gfm_temp2[SNOVA_o * SNOVA_l2] = {0};

				int mi_prime = i_prime(mi, alpha);

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								gfm_temp0[idx * SNOVA_lr + i1 * SNOVA_r + j1] ^=
								    whipped_F21[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1] *
								    q2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
					gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
				}

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_r; k1++) {
								gfm_temp1[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
								    gfm_temp0[idx * SNOVA_lr + i1 * SNOVA_r + k1] *
								    Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];
							}

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
					gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
				}

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++) {
								gfm_temp2[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
								    Q1[(mi * SNOVA_alpha + alpha) * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    gfm_temp1[idx * SNOVA_l2 + k1 * SNOVA_l + j1];
							}

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
					gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
				}

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
						for (int ti2 = 0; ti2 < SNOVA_l; ti2++)
							for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
								for (int tj2 = 0; tj2 < SNOVA_r; tj2++) {
									gauss16[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx * SNOVA_lr + tj1 * SNOVA_r + tj2] ^=
									    gfm_temp2[idx * SNOVA_l2 + tj1 * SNOVA_l + ti2] *
									    Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r + tj2];
								}
			}
		}

		for (int mi = 0; mi < SNOVA_o; mi++) {
			for (int alpha = 0; alpha < SNOVA_alpha; alpha++) {
				uint16_t gfm_temp0[SNOVA_o * SNOVA_lr] = {0};
				uint16_t gfm_temp1[SNOVA_o * SNOVA_lr] = {0};
				uint16_t gfm_temp2[SNOVA_o * SNOVA_lr] = {0};

				int mi_prime = i_prime(mi, alpha);

				// Transpose
				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								gfm_temp0[idx * SNOVA_lr + i1 * SNOVA_r + j1] ^=
								    whipped_F12[((mi_prime * SNOVA_l + b1) * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1] *
								    q1[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
					gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
				}

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_r; k1++)
								gfm_temp1[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
								    Am[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] *
								    gfm_temp0[idx * SNOVA_lr + j1 * SNOVA_r + k1];

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
					gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
				}

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								gfm_temp2[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
								    gfm_temp1[idx * SNOVA_lr + i1 * SNOVA_l + k1] *
								    Q2[(mi * SNOVA_alpha + alpha) * SNOVA_l2 + k1 * SNOVA_l + j1];

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
					gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
				}

				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
						for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
							for (int ti2 = 0; ti2 < SNOVA_l; ti2++)
								for (int tj2 = 0; tj2 < SNOVA_r; tj2++)
									gauss16[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx * SNOVA_lr + tj1 * SNOVA_r + tj2] ^=
									    gfm_temp2[idx * SNOVA_lr + ti1 * SNOVA_l + tj1] *
									    Bm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2];
			}
		}

		for (int ti = 0; ti < SNOVA_o * SNOVA_lr; ti++)
			for (int tj = 0; tj < SNOVA_o * SNOVA_lr; tj++) {
				gauss16[ti][tj] = gf16_compress(gauss16[ti][tj]);
			}

		// Gaussian elimination in constant time
		for (int i = 0; i < SNOVA_o * SNOVA_lr; ++i) {
			for (int j = i + 1; j < SNOVA_o * SNOVA_lr; ++j) {
				int16_t mask = ct_is_not_zero(gauss16[i][i]) - 1;
				for (int k = 0; k < SNOVA_olr; ++k) {
					gauss16[i][k] ^= mask & gauss16[j][k];
				}
			}

			for (int k = 0; k < SNOVA_olr; ++k) {
				gauss16[i][k] = gf16_compress(gauss16[i][k]);
			}

			flag_redo |= 1 - ct_is_not_zero(gauss16[i][i]);

			uint16_t t_GF16 = ct_gf_inverse(gauss16[i][i]);
			for (int k = 0; k < SNOVA_olr; ++k) {
				gauss16[i][k] = gauss16[i][k] * t_GF16;
			}

			for (int k = 0; k < SNOVA_olr; ++k) {
				gauss16[i][k] = gf16_compress(gauss16[i][k]);
			}

			for (int j = i + 1; j < SNOVA_o * SNOVA_lr; ++j) {
				uint16_t gji = gf16_expand(gauss16[j][i]);
				for (int k = 0; k < SNOVA_olr; ++k) {
					gauss16[j][k] ^= gauss16[i][k] * gji;
				}
			}

			for (int j = i + 1; j < SNOVA_o * SNOVA_lr; ++j) {
				gauss16[j][i + 1] = gf16_compress(gauss16[j][i + 1]);
			}
		}

		if (!flag_redo) {
			// Last step of Gaussian elimination
			uint16_t solution16[SNOVA_o * SNOVA_lr] = {0};

			for (int i = SNOVA_o * SNOVA_lr - 1; i >= 0; --i) {
				uint16_t sum = 0;
				for (int k = i + 1; k < SNOVA_o * SNOVA_lr; ++k) {
					sum ^= gauss16[i][k] * solution16[k];
				}
				solution16[i] = gf16_cleanup(sum ^ gf16_expand(gauss16[i][SNOVA_o * SNOVA_lr]));
			}
			for (int i = 0; i < SNOVA_o * SNOVA_lr; i++) {
				solution[i] = gf16_compress(solution16[i]);
			}

			memcpy(signature_in_GF + SNOVA_v * SNOVA_lr, solution, SNOVA_o * SNOVA_lr);

			// Establish signature using T12
			for (int index = 0; index < SNOVA_v; ++index)
				for (int mi = 0; mi < SNOVA_o; ++mi)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++) {
							uint16_t sum = 0;
							for (int k1 = 0; k1 < SNOVA_l; k1++) {
								sum ^= skx->T12[(index * SNOVA_o + mi) * SNOVA_l2 + i1 * SNOVA_l + k1] *
								       solution[mi * SNOVA_lr + k1 * SNOVA_r + j1];
							}
							signature_in_GF[index * SNOVA_lr + i1 * SNOVA_r + j1] ^= gf16_compress(sum);
						}

			memcpy(signature_in_GF + SNOVA_v * SNOVA_lr, solution, SNOVA_o * SNOVA_lr);
		}
	} while (flag_redo);

	compress_gf(sig, signature_in_GF, SNOVA_n * SNOVA_lr);
	memcpy(sig + BYTES_SIGNATURE - BYTES_SALT, salt, BYTES_SALT);

	return 0;
}

#undef Am
#undef Bm
#undef Q1
#undef Q2
#undef q1
#undef q2

/**
 * PK expansion.
 */
int SNOVA_NAMESPACE(pk_expand)(expanded_PK *pkx, const uint8_t *pk) {
	SNOVA_INIT

	memcpy(pkx->pk_seed, pk, SEED_LENGTH_PUBLIC);
#if HASH_PK
	shake256(pkx->pk_hash, BYTES_PK_HASH, pk, BYTES_PK);
#endif

	/**
	 * Create P matrix
	 */
	gf_t P_matrix[NUM_PUB_GF];
	gf_t P22[SNOVA_m1 * SNOVA_o * SNOVA_o * SNOVA_l2];

	gf_t *P11 = P_matrix;
	gf_t *P12 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2;
	gf_t *P21 = P_matrix + SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2;

	if (expand_pk(P22, pk + SEED_LENGTH_PUBLIC)) {
		return -1;
	}
	expand_public(P_matrix, pk);

	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						pkx->P[(ni * SNOVA_l + i1) * SNOVA_m1 * SNOVA_n * SNOVA_l + ((mi * SNOVA_n + nj) * SNOVA_l + j1)] =
						    P11[((mi * SNOVA_v + ni) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + j1];

	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int ni = 0; ni < SNOVA_v; ++ni)
			for (int nj = SNOVA_v; nj < SNOVA_n; ++nj)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						pkx->P[(ni * SNOVA_l + i1) * SNOVA_m1 * SNOVA_n * SNOVA_l + ((mi * SNOVA_n + nj) * SNOVA_l + j1)] =
						    P12[((mi * SNOVA_v + ni) * SNOVA_o + (nj - SNOVA_v)) * SNOVA_l2 + i1 * SNOVA_l + j1];

	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int ni = SNOVA_v; ni < SNOVA_n; ++ni)
			for (int nj = 0; nj < SNOVA_v; ++nj)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						pkx->P[(ni * SNOVA_l + i1) * SNOVA_m1 * SNOVA_n * SNOVA_l + ((mi * SNOVA_n + nj) * SNOVA_l + j1)] =
						    P21[((mi * SNOVA_o + (ni - SNOVA_v)) * SNOVA_v + nj) * SNOVA_l2 + i1 * SNOVA_l + j1];

	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int ni = SNOVA_v; ni < SNOVA_n; ++ni)
			for (int nj = SNOVA_v; nj < SNOVA_n; ++nj)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						pkx->P[(ni * SNOVA_l + i1) * SNOVA_m1 * SNOVA_n * SNOVA_l + ((mi * SNOVA_n + nj) * SNOVA_l + j1)] =
						    P22[((mi * SNOVA_o + (ni - SNOVA_v)) * SNOVA_o + nj - SNOVA_v) * SNOVA_l2 + i1 * SNOVA_l + j1];

#if !FIXED_ABQ
	/**
	 * Create AB matrices, improve q
	 */
	gf_t *A = P_matrix + SNOVA_m1 * (SNOVA_v * SNOVA_v + 2 * SNOVA_v * SNOVA_o) * SNOVA_l2;
	gf_t *B = A + SNOVA_o * SNOVA_alpha * SNOVA_r2;
	gf_t *q1 = B + SNOVA_o * SNOVA_alpha * SNOVA_lr;
	gf_t *q2 = q1 + SNOVA_o * SNOVA_alpha * SNOVA_l;

	for (size_t idx = 0; idx < SNOVA_o * SNOVA_alpha; idx++) {
		be_invertible_by_add_aS(&(pkx->Am[idx * SNOVA_r2]), &A[idx * SNOVA_r2], SNOVA_r, SNOVA_r);
		be_invertible_by_add_aS(&(pkx->Bm[idx * SNOVA_lr]), &B[idx * SNOVA_lr], SNOVA_r, SNOVA_l);

		if (!q1[idx * SNOVA_l + SNOVA_l - 1]) {
			q1[idx * SNOVA_l + SNOVA_l - 1] = SNOVA_q - (q1[idx * SNOVA_l] + (q1[idx * SNOVA_l] == 0));
		}
		if (!q2[idx * SNOVA_l + SNOVA_l - 1]) {
			q2[idx * SNOVA_l + SNOVA_l - 1] = SNOVA_q - (q2[idx * SNOVA_l] + (q2[idx * SNOVA_l] == 0));
		}
	}

	memcpy(pkx->q1, q1, SNOVA_o * SNOVA_alpha * SNOVA_l);
	memcpy(pkx->q2, q2, SNOVA_o * SNOVA_alpha * SNOVA_l);
#endif

	return 0;
}

/**
 * Optimized version of verify.
 */
int SNOVA_NAMESPACE(verify)(const expanded_PK *pkx, const uint8_t *sig, const uint8_t *digest, const size_t len_digest) {
	SNOVA_INIT

	gf_t signature_in_GF[NUMGF_SIGNATURE];
	if (expand_gf(signature_in_GF, sig, NUMGF_SIGNATURE)) {
		return -1;
	}

	/**
	 * Whip signature
	 */
	uint16_t whipped_sig[SNOVA_l * SNOVA_n * SNOVA_lr] = {0};

	for (int ab = 0; ab < SNOVA_l; ++ab)
		for (int idx = 0; idx < SNOVA_n; ++idx)
			for (int i1 = 0; i1 < SNOVA_l; i1++)
				for (int j1 = 0; j1 < SNOVA_r; j1++)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						whipped_sig[idx * SNOVA_l * SNOVA_lr + i1 * SNOVA_lr + ab * SNOVA_r + j1] ^=
						    gf_Sx[ab * SNOVA_l2 + i1 * SNOVA_l + k1] * signature_in_GF[idx * SNOVA_lr + k1 * SNOVA_r + j1];

	for (int i1 = 0; i1 < SNOVA_l * SNOVA_n * SNOVA_lr; i1++) {
		whipped_sig[i1] = gf16_cleanup(whipped_sig[i1]);
	}

	/**
	 * Evaluate whipped central map
	 */
	uint16_t hash_in_GF[SNOVA_o * SNOVA_lr] = {0};
	uint16_t sum_t1[SNOVA_m1 * SNOVA_l2 * SNOVA_r2] = {0};
	uint16_t sum_t1s[SNOVA_m1 * SNOVA_l2 * SNOVA_r2] = {0};

	uint16_t sum_t0[SNOVA_mnl * SNOVA_lr] = {0};

	// Left, transposed whipped_sig
	for (int ni = 0; ni < SNOVA_nl; ni++)
		for (int mi = 0; mi < SNOVA_mnl; mi++)
			for (int a1 = 0; a1 < SNOVA_lr; a1++) {
				sum_t0[a1 * SNOVA_mnl + mi] ^= pkx->P[ni * SNOVA_m1 * SNOVA_n * SNOVA_l + mi] * whipped_sig[ni * SNOVA_lr + a1];
			}

	for (int i1 = 0; i1 < SNOVA_mnl * SNOVA_lr; ++i1) {
		sum_t0[i1] = gf16_compress(sum_t0[i1]);
	}

	// Right
	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int nj = 0; nj < SNOVA_nl; nj++)
			for (int a1 = 0; a1 < SNOVA_lr; a1++)
				for (int b1 = 0; b1 < SNOVA_lr; b1++)
					sum_t1[mi * SNOVA_lr * SNOVA_lr + a1 * SNOVA_lr + b1] ^=
					    sum_t0[(mi * SNOVA_nl + nj) + a1 * SNOVA_mnl] * whipped_sig[nj * SNOVA_lr + b1];

	for (int i1 = 0; i1 < SNOVA_m1 * SNOVA_l2 * SNOVA_r2; i1++) {
		sum_t1[i1] = gf16_compress(sum_t1[i1]);
	}

	for (int mi = 0; mi < SNOVA_m1; ++mi)
		for (int a1 = 0; a1 < SNOVA_l; ++a1)
			for (int i1 = 0; i1 < SNOVA_r; i1++)
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						sum_t1s[(mi * SNOVA_l + a1) * SNOVA_l * SNOVA_r2 + b1 * SNOVA_r2 + i1 * SNOVA_r + j1] =
						    sum_t1[(mi * SNOVA_l + a1) * SNOVA_l * SNOVA_r2 + i1 * SNOVA_lr + b1 * SNOVA_r + j1];

	/**
	 * Prepare
	 */
#if FIXED_ABQ
#define Amx fixedAm
#define Bmx fixedBm
#define q1x fixedq1
#define q2x fixedq2
#else

	uint16_t Amx[SNOVA_o * SNOVA_alpha * SNOVA_r2];
	uint16_t Bmx[SNOVA_o * SNOVA_alpha * SNOVA_lr];
	uint16_t q1x[SNOVA_o * SNOVA_alpha * SNOVA_l];
	uint16_t q2x[SNOVA_o * SNOVA_alpha * SNOVA_l];

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_r2; i1++) {
		Amx[i1] = gf16_expand(pkx->Am[i1]);
	}

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_lr; i1++) {
		Bmx[i1] = gf16_expand(pkx->Bm[i1]);
	}

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_l; i1++) {
		q1x[i1] = gf16_expand(pkx->q1[i1]);
	}

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_alpha * SNOVA_l; i1++) {
		q2x[i1] = gf16_expand(pkx->q2[i1]);
	}
#endif

	/**
	 * Apply A, B, q1 and q2, aka E matrix
	 */
	for (int mi = 0; mi < SNOVA_o; ++mi)
		for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
			int mi_prime = i_prime(mi, alpha);

			uint16_t gfm_temp1[SNOVA_r2] = {0};
			uint16_t gfm_temp2[SNOVA_lr] = {0};

			// apply q1 and q2
			for (int a1 = 0; a1 < SNOVA_l; ++a1) {
				uint16_t gfm_temp0[SNOVA_r2] = {0};

				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							gfm_temp0[i1 * SNOVA_r + j1] ^=
							    q2x[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1] *
							    sum_t1s[(mi_prime * SNOVA_l + a1) * SNOVA_l * SNOVA_r2 + b1 * SNOVA_r2 + i1 * SNOVA_r + j1];

				for (int i1 = 0; i1 < SNOVA_r2; i1++) {
					gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
				}

				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++) {
						gfm_temp1[i1 * SNOVA_r + j1] ^=
						    gfm_temp0[i1 * SNOVA_r + j1] * q1x[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1];
					}
			}

			for (int i1 = 0; i1 < SNOVA_r2; i1++) {
				gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
			}

			// A and B
			for (int i1 = 0; i1 < SNOVA_r; i1++)
				for (int j1 = 0; j1 < SNOVA_l; j1++)
					for (int k1 = 0; k1 < SNOVA_r; k1++)
						gfm_temp2[i1 * SNOVA_l + j1] ^=
						    gfm_temp1[i1 * SNOVA_r + k1] * Bmx[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];

			for (int i1 = 0; i1 < SNOVA_lr; i1++) {
				gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
			}

			for (int i1 = 0; i1 < SNOVA_r; i1++)
				for (int j1 = 0; j1 < SNOVA_l; j1++)
					for (int k1 = 0; k1 < SNOVA_r; k1++)
						hash_in_GF[mi * SNOVA_lr + i1 * SNOVA_l + j1] ^=
						    Amx[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] * gfm_temp2[k1 * SNOVA_l + j1];
		}

	for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
		hash_in_GF[i1] = gf16_compress(hash_in_GF[i1]);
	}

	/**
	 * Check hashes
	 */
	uint8_t signed_bytes[BYTES_HASH];
	uint8_t signed_gf[GF16_HASH] = {0};
	const uint8_t *salt = sig + BYTES_SIGNATURE - BYTES_SALT;
#if HASH_PK
	hash_combined(signed_bytes, digest, len_digest, pkx->pk_hash, salt);
#else
	hash_combined(signed_bytes, digest, len_digest, pkx->pk_seed, salt);
#endif
	expand_gf(signed_gf, signed_bytes, GF16_HASH);

	for (int i = 0; i < GF16_HASH; ++i) {
		if (hash_in_GF[i] != signed_gf[i]) {
			return -1;
		}
	}

	return 0;
}
