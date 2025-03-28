#ifndef PLASMA_VECTOR_H
#define PLASMA_VECTOR_H

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

static alignas(16 * VEC_LENGTH) uint8_t mt4b2_16[256][VEC_LENGTH];
VECTOR *mtk2_16 = (VECTOR *)mt4b2_16;

// inverse table, runs in constant time
static VECTOR vector_inv_table = {0};
static VECTOR l_mask = {0};

// Table used by vtl_ct_multtab
static VECTOR vtl_multmask1, vtl_multmask2, vtl_multmask4, vtl_multmask8;
static VECTOR vtl_mult_table1, vtl_mult_table2, vtl_mult_table4, vtl_mult_table8;
static VECTOR zero256 = {0};

int init_vector_table(void) {
	static int vector_table_init_flag = 0;
	if (vector_table_init_flag) {
		return 0;
	}
	vector_table_init_flag = 1;

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			for (int k = 0; k < 16; ++k) {
				uint8_t temp = (mt(i, k) << 4) ^ mt(j, k);
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
	vector_inv_table = _mm256_setr_epi8(0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8,
	                                    0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8);
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

static inline void gf16_32_mul_k(VECTOR *a_256, gf16_t k, VECTOR *ak) {
	*ak = VEC_SHUFFLE(vtl_ct_multtab(k), *a_256);
}

static inline void gf16_32_mul_k_add(VECTOR  *a_256, gf16_t k, VECTOR  *ak) {
	*ak ^= VEC_SHUFFLE(vtl_ct_multtab(k), *a_256);
}

/**
 * c[i] = a[i] * b[i]
 */
static inline void gf16_32_mul_32(VECTOR  *a_256, VECTOR  *b_256, VECTOR  *sum) {
	memset(sum, 0, sizeof(VECTOR));

	*sum ^= *b_256 &  VEC_CMP_EQ(*a_256 & vtl_multmask1, vtl_multmask1);
	*sum ^= VEC_SHUFFLE(vtl_mult_table2, *b_256) &  VEC_CMP_EQ(*a_256 & vtl_multmask2, vtl_multmask2);
	*sum ^= VEC_SHUFFLE(vtl_mult_table4, *b_256) &  VEC_CMP_EQ(*a_256 & vtl_multmask4, vtl_multmask4);
	*sum ^= VEC_SHUFFLE(vtl_mult_table8, *b_256) &  VEC_CMP_EQ(*a_256 & vtl_multmask8, vtl_multmask8);
}

/**
 * c[i] += a[i] * b[i]
 */
static inline void gf16_32_mul_32_add(VECTOR *a_256, VECTOR *b_256, VECTOR *sum) {
	*sum ^= *b_256 &  VEC_CMP_EQ(*a_256 & vtl_multmask1, vtl_multmask1);
	*sum ^= VEC_SHUFFLE(vtl_mult_table2, *b_256) &  VEC_CMP_EQ(*a_256 & vtl_multmask2, vtl_multmask2);
	*sum ^= VEC_SHUFFLE(vtl_mult_table4, *b_256) &  VEC_CMP_EQ(*a_256 & vtl_multmask4, vtl_multmask4);
	*sum ^= VEC_SHUFFLE(vtl_mult_table8, *b_256) &  VEC_CMP_EQ(*a_256 & vtl_multmask8, vtl_multmask8);
}

/**
 * Establish P22 during keygen
 */
#define mol_SNOVA32 ((m_SNOVA * o_SNOVA * l_SNOVA + VEC_LENGTH - 1) / VEC_LENGTH)
#define mol_SNOVA (mol_SNOVA32 * VEC_LENGTH)

/**
 * Generate public key (P22 part), use vector vtl
 * @param outP22 - output
 * @param T12 - input
 * @param P21 - input
 * @param F12 - input
 */
void gen_P22_vtl(P22_byte_t outP22, T12_t T12, P21_t P21, F12_t F12) {
	P22_t P22 = {0};

	alignas(VEC_LENGTH) uint8_t temp1_8[mol_SNOVA * o_SNOVA * l_SNOVA] = {0};
	alignas(VEC_LENGTH) uint8_t F12_8[mol_SNOVA * v_SNOVA * l_SNOVA] = {0};

	VECTOR *temp1_256 = (VECTOR *)temp1_8;
	VECTOR *F12_256 = (VECTOR *)F12_8;

	for (int di = 0; di < v_SNOVA; ++di)
		for (int k1 = 0; k1 < l_SNOVA; ++k1)
			for (int mi = 0; mi < m_SNOVA; ++mi)
				for (int dk = 0; dk < o_SNOVA; ++dk)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						F12_8[(di * l_SNOVA + k1) * mol_SNOVA + mi * o_SNOVA * l_SNOVA + dk * l_SNOVA + j1] =
						    F12[mi][di][dk][k1 * l_SNOVA + j1];

	for (int di = 0; di < v_SNOVA; ++di)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int k1 = 0; k1 < l_SNOVA; ++k1) {
					VECTOR k_lh = vtl_ct_multtab(T12[di][dj][i1 * l_SNOVA + k1]);

					for (int mi_dk_j1 = 0; mi_dk_j1 < mol_SNOVA32; mi_dk_j1++)
						temp1_256[(dj * l_SNOVA + i1) * mol_SNOVA32 + mi_dk_j1] ^=
						    VEC_SHUFFLE(k_lh, F12_256[(di * l_SNOVA + k1) * mol_SNOVA32 + mi_dk_j1]);
				}

	for (int mi = 0; mi < m_SNOVA; ++mi)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int dk = 0; dk < o_SNOVA; ++dk)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						P22[mi][dj][dk][i1 * l_SNOVA + j1] ^=
						    temp1_8[(dj * l_SNOVA + i1) * mol_SNOVA + mi * o_SNOVA * l_SNOVA + dk * l_SNOVA + j1];

	alignas(VEC_LENGTH) uint8_t temp2_8[mol_SNOVA * o_SNOVA * l_SNOVA] = {0};
	alignas(VEC_LENGTH) uint8_t P21_8[mol_SNOVA * v_SNOVA * l_SNOVA] = {0};

	VECTOR *temp2_256 = (VECTOR *)temp2_8;
	VECTOR *P21_256 = (VECTOR *)P21_8;

	for (int di = 0; di < v_SNOVA; ++di)
		for (int k1 = 0; k1 < l_SNOVA; ++k1)
			for (int mi = 0; mi < m_SNOVA; ++mi)
				for (int dj = 0; dj < o_SNOVA; ++dj)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						P21_8[(di * l_SNOVA + k1) * mol_SNOVA + mi * o_SNOVA * l_SNOVA + dj * l_SNOVA + i1] =
						    P21[mi][dj][di][i1 * l_SNOVA + k1];

	for (int di = 0; di < v_SNOVA; ++di)
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int k1 = 0; k1 < l_SNOVA; ++k1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					VECTOR k_lh = vtl_ct_multtab(T12[di][dk][k1 * l_SNOVA + j1]);

					for (int mi_dj_i1 = 0; mi_dj_i1 < mol_SNOVA32; mi_dj_i1++)
						temp2_256[(dk * l_SNOVA + j1) * mol_SNOVA32 + mi_dj_i1] ^=
						    VEC_SHUFFLE(k_lh, P21_256[(di * l_SNOVA + k1) * mol_SNOVA32 + mi_dj_i1]);
				}

	for (int mi = 0; mi < m_SNOVA; ++mi)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int dk = 0; dk < o_SNOVA; ++dk)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						P22[mi][dj][dk][i1 * l_SNOVA + j1] ^=
						    temp2_8[(dk * l_SNOVA + j1) * mol_SNOVA + mi * o_SNOVA * l_SNOVA + dj * l_SNOVA + i1];

	convert_GF16s_to_bytes(outP22, (uint8_t *)P22, m_SNOVA * o_SNOVA * o_SNOVA * lsq_SNOVA);
}

/**
 * Establish F during keygen
*/
#define mvl_SNOVA32 ((m_SNOVA * v_SNOVA * l_SNOVA + VEC_LENGTH - 1) / VEC_LENGTH)
#define mvl_SNOVA8 (mvl_SNOVA32 * VEC_LENGTH)
#define vl_SNOVA (v_SNOVA * l_SNOVA)

/**
 * Generate private key (F part), use vector vtl
 * @param map2 - output: F11 F12 F21
 * @param map1 - input: P11 P12 P21 Aalpha Balpha Qalpha1 Qalpha2
 * @param T12 - input
 */
void gen_F_vtl(map_group2 *map2, map_group1 *map1, T12_t T12) {
	VECTOR p11_256[mvl_SNOVA32 * vl_SNOVA] = {0};
	VECTOR t12_256[mvl_SNOVA32 * l_SNOVA] = {0};
	VECTOR res256[mvl_SNOVA32 * o_SNOVA * l_SNOVA] = {0};

	uint8_t *p11_8 = (uint8_t *)p11_256;
	uint8_t *t12_8 = (uint8_t *)t12_256;
	uint8_t *res8 = (uint8_t *)res256;

	memcpy(map2->F11, map1->P11, m_SNOVA * v_SNOVA * v_SNOVA * lsq_SNOVA);
	memcpy(map2->F12, map1->P12, m_SNOVA * v_SNOVA * o_SNOVA * lsq_SNOVA);
	memcpy(map2->F21, map1->P21, m_SNOVA * o_SNOVA * v_SNOVA * lsq_SNOVA);

	// F12

	for (int k1 = 0; k1 < l_SNOVA; ++k1)
		for (int dk = 0; dk < v_SNOVA; ++dk)
			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					t12_8[(dk * l_SNOVA + k1) * o_SNOVA * l_SNOVA + dj * l_SNOVA + j1] = T12[dk][dj][k1 * l_SNOVA + j1];
				}

	for (int dk = 0; dk < v_SNOVA; dk++)
		for (int di = 0; di < v_SNOVA; di++)
			for (int j1 = 0; j1 < l_SNOVA; ++j1)
				for (int mi = 0; mi < m_SNOVA; mi++)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						p11_8[(dk * l_SNOVA + j1) * mvl_SNOVA8 + mi * v_SNOVA * l_SNOVA + di * l_SNOVA + i1] =
						    map1->P11[mi][di][dk][i1 * l_SNOVA + j1];

	for (int dj_j1 = 0; dj_j1 < o_SNOVA * l_SNOVA; ++dj_j1)
		for (int dk_k1 = 0; dk_k1 < v_SNOVA * l_SNOVA; ++dk_k1) {
			VECTOR k_lh = vtl_ct_multtab(t12_8[dk_k1 * o_SNOVA * l_SNOVA + dj_j1]);
			for (int mi_di_i1 = 0; mi_di_i1 < mvl_SNOVA32; mi_di_i1++) {
				res256[dj_j1 * mvl_SNOVA32 + mi_di_i1] ^= VEC_SHUFFLE(k_lh, p11_256[dk_k1 * mvl_SNOVA32 + mi_di_i1]);
			}
		}

	for (int di = 0; di < v_SNOVA; ++di)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int j1 = 0; j1 < l_SNOVA; ++j1)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int mi = 0; mi < m_SNOVA; ++mi)
						map2->F12[mi][di][dj][i1 * l_SNOVA + j1] ^=
						    res8[dj * l_SNOVA * mvl_SNOVA8 + j1 * mvl_SNOVA8 + mi * v_SNOVA * l_SNOVA + di * l_SNOVA + i1];

	// Same for F21
	memset(res256, 0, sizeof(res256));

	for (int dj = 0; dj < v_SNOVA; ++dj)
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					t12_8[(dj * l_SNOVA + j1) * o_SNOVA * l_SNOVA + dk * l_SNOVA + i1] = T12[dj][dk][i1 * l_SNOVA + j1];
				}

	for (int mi = 0; mi < m_SNOVA; mi++)
		for (int di = 0; di < v_SNOVA; di++)
			for (int dk = 0; dk < v_SNOVA; dk++)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						p11_8[(di * l_SNOVA + i1) * mvl_SNOVA8 + mi * v_SNOVA * l_SNOVA + dk * l_SNOVA + j1] =
						    map1->P11[mi][di][dk][i1 * l_SNOVA + j1];

	for (int dj_j1 = 0; dj_j1 < o_SNOVA * l_SNOVA; ++dj_j1)
		for (int dk_k1 = 0; dk_k1 < v_SNOVA * l_SNOVA; ++dk_k1) {
			VECTOR k_lh = vtl_ct_multtab(t12_8[dk_k1 * o_SNOVA * l_SNOVA + dj_j1]);
			for (int mi_di_i1 = 0; mi_di_i1 < mvl_SNOVA32; mi_di_i1++) {
				res256[dj_j1 * mvl_SNOVA32 + mi_di_i1] ^= VEC_SHUFFLE(k_lh, p11_256[dk_k1 * mvl_SNOVA32 + mi_di_i1]);
			}
		}

	// Shuffle back
	for (int mi = 0; mi < m_SNOVA; ++mi)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int di = 0; di < v_SNOVA; ++di)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						map2->F21[mi][dj][di][i1 * l_SNOVA + j1] ^=
						    res8[dj * l_SNOVA * mvl_SNOVA8 + i1 * mvl_SNOVA8 + mi * v_SNOVA * l_SNOVA + di * l_SNOVA + j1];
}

static inline uint8_t horizontal_xor_256(VECTOR vec) {

#if __ARM_NEON
	int64x2_t combined = (int64x2_t)vec;
#elif VEC_LENGTH > 16
	__m128i low = _mm256_castsi256_si128(vec);
	__m128i high = _mm256_extracti128_si256(vec, 1);
	__m128i combined = _mm_xor_si128(low, high);
#else
	__m128i combined = vec;
#endif

	combined ^= combined >> 32;
	combined ^= combined >> 16;
	combined ^= combined >> 8;

	return combined[0] ^ combined[1];
}

#define vtl_len ((n_SNOVA * rank + VEC_LENGTH - 1) / VEC_LENGTH)
#define vtl_v_len ((v_SNOVA * rank + VEC_LENGTH - 1) / VEC_LENGTH)
#define vtl_o_len ((o_SNOVA * rank + VEC_LENGTH - 1) / VEC_LENGTH)
#define vtl_mainRow_x_rank(mainRow) (((mainRow) * rank + VEC_LENGTH - 1) / VEC_LENGTH * VEC_LENGTH)
#define vtl_mainRow_x_rank32(mainRow) (((mainRow) * rank + VEC_LENGTH - 1) / VEC_LENGTH)
#define rank_next32 ((rank + 1) / 2)
#define rank_next2 (rank_next32 * 2)
#define rank_floor2 ((rank) / 2 * 2)
#define alpha_SNOVA_next2 ((alpha_SNOVA + 1) / 2 * 2)

// Optimize vector alignment. Only useful for even rank
#if rank % 2
#define MJ_MAX mainRow
#else
#define MJ_MAX (vtl_mainRow_x_rank(mainRow)) / rank
#endif

static inline void jogressMatrix_vector(VECTOR *AJ, const uint8_t *A, int mainCol, int mainRow) {
	for (int mi = 0; mi < mainCol; mi++) {
		for (int mj = 0; mj < mainRow; mj++) {
			for (int ei = 0; ei < rank; ei++) {
				for (int ej = 0; ej < rank; ej++) {
					// AJ[((mi * rank + ei) * (mainRow * rank)) + (mj * rank + ej)] =
					VEC_BYTE(AJ, ((mi * rank + ei) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ej)) =
					    A[(mi * (mainRow) * (rank * rank)) + (mj * (rank * rank)) + (ei * rank + ej)];
				}
			}
		}
	}
}

static inline void jogressTrMatrix_vector(VECTOR *AJ, const uint8_t *A, int mainCol, int mainRow) {
	for (int mi = 0; mi < mainCol; mi++) {
		for (int mj = 0; mj < mainRow; mj++) {
			for (int ei = 0; ei < rank; ei++) {
				for (int ej = 0; ej < rank; ej++) {
					// AJ[((mi * rank + ei) * (mainRow * rank)) + (mj * rank + ej)] =
					VEC_BYTE(AJ, ((mi * rank + ei) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ej)) =
					    A[(mi * (mainRow) * (rank * rank)) + (mj * (rank * rank)) + (ej * rank + ei)];
					// ei <<---->> ej
				}
			}
		}
	}
}

static inline void jogressMatrixTr_vector(VECTOR *AJ_tr, const VECTOR *AJ, int mainCol, int mainRow) {
	for (int mi = 0; mi < mainCol; mi++) {
		for (int mj = 0; mj < MJ_MAX; mj++) {
			for (int ei = 0; ei < rank; ei++) {
				for (int ej = 0; ej < rank; ej++) {
					VEC_BYTE(AJ_tr, ((mi * rank + ei) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ej)) =
					    VEC_BYTE(AJ, ((mi * rank + ej) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ei));
					//  ei <<------------------------------------------>> ej
				}
			}
		}
	}
}

#endif
