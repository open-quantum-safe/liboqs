#ifndef PLASMA_AVX2_H
#define PLASMA_AVX2_H

#include <immintrin.h>

static uint8_t mt4b2_16[256][32] __attribute__((aligned(512)));
__m256i *mtk2_16 = (__m256i *)mt4b2_16;

// inverse table, runs in constant time
static __m256i avx_inv_table = {0};
static __m256i l_mask = {0};

// Table used by vtl_ct_multtab
static __m256i vtl_multmask1, vtl_multmask2, vtl_multmask4, vtl_multmask8;
static __m256i vtl_mult_table1, vtl_mult_table2, vtl_mult_table4, vtl_mult_table8;
static __m256i zero256 = {0};

int init_avx_table() {
	static int avx_table_init_flag = 0;
	if (avx_table_init_flag) {
		return 0;
	}
	avx_table_init_flag = 1;

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			for (int k = 0; k < 16; ++k) {
				uint8_t temp = (mt(i, k) << 4) ^ mt(j, k);
				mt4b2_16[i * 16 + j][k] = temp;
				mt4b2_16[i * 16 + j][k + 16] = temp;
			}
		}
	}

	l_mask = _mm256_set1_epi8(0x0f);

	vtl_multmask1 = _mm256_set1_epi32(1);
	vtl_multmask2 = _mm256_set1_epi32(2);
	vtl_multmask4 = _mm256_set1_epi32(4);
	vtl_multmask8 = _mm256_set1_epi32(8);

	vtl_mult_table1 = mtk2_16[1];
	vtl_mult_table2 = mtk2_16[2];
	vtl_mult_table4 = mtk2_16[4];
	vtl_mult_table8 = mtk2_16[8];

	// GF16 inverse table
	avx_inv_table = _mm256_setr_epi8(0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8,
	                                 0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5, 10, 4, 3, 8);

	return 1;
}

// Constant time VTL table
static inline __m256i vtl_ct_multtab(uint8_t val) {
	__m256i val256 = _mm256_set1_epi32(val);

	return (vtl_mult_table1 & _mm256_cmpgt_epi32(val256 & vtl_multmask1, zero256)) ^
	       (vtl_mult_table2 & _mm256_cmpgt_epi32(val256 & vtl_multmask2, zero256)) ^
	       (vtl_mult_table4 & _mm256_cmpgt_epi32(val256 & vtl_multmask4, zero256)) ^
	       (vtl_mult_table8 & _mm256_cmpgt_epi32(val256 & vtl_multmask8, zero256));
}

static inline void gf16_32_mul_k(gf16_t *a, gf16_t k, gf16_t *ak) {
	__m256i a_256 = *((__m256i *)a);
	*((__m256i *)ak) = _mm256_shuffle_epi8(vtl_ct_multtab(k), a_256);
}

static inline void gf16_32_mul_k_add(gf16_t  *a, gf16_t k, gf16_t  *ak) {
	__m256i a_256 = *((__m256i *)a);
	*((__m256i *)ak) ^= _mm256_shuffle_epi8(vtl_ct_multtab(k), a_256);
}

/**
 * c[i] = a[i] * b[i]
 */
static inline void gf16_32_mul_32(gf16_t  *a, gf16_t  *b, gf16_t  *c) {
	__m256i a_256 = *((__m256i *)a);
	__m256i b_256 = *((__m256i *)b);
	__m256i *sum = ((__m256i *)c);
	*sum = _mm256_setzero_si256();

	__m256i b4_256[4] = {
		b_256, // x1
		_mm256_shuffle_epi8(vtl_mult_table2, b_256), // x2
		_mm256_shuffle_epi8(vtl_mult_table4, b_256), // x4
		_mm256_shuffle_epi8(vtl_mult_table8, b_256)
	};// x8

	__m256i mask[4] = {_mm256_set1_epi64x(0x0101010101010101ull), _mm256_set1_epi64x(0x0202020202020202ull),
	                   _mm256_set1_epi64x(0x0404040404040404ull), _mm256_set1_epi64x(0x0808080808080808ull)
	                  };

	for (int i = 0; i < 4; i++) {
		*sum ^= b4_256[i] &  _mm256_cmpeq_epi8(_mm256_and_si256(a_256, mask[i]), mask[i]);
	}
}

/**
 * c[i] += a[i] * b[i]
 */
static inline void gf16_32_mul_32_add(gf16_t *a, gf16_t *b, gf16_t *c) {
	__m256i a_256 = *((__m256i *)a);
	__m256i b_256 = *((__m256i *)b);
	__m256i *sum = ((__m256i *)c);

	__m256i b4_256[4] = {
		b_256, // x1
		_mm256_shuffle_epi8(vtl_mult_table2, b_256), // x2
		_mm256_shuffle_epi8(vtl_mult_table4, b_256), // x4
		_mm256_shuffle_epi8(vtl_mult_table8, b_256)
	};// x8

	__m256i mask[4] = {_mm256_set1_epi64x(0x0101010101010101ull), _mm256_set1_epi64x(0x0202020202020202ull),
	                   _mm256_set1_epi64x(0x0404040404040404ull), _mm256_set1_epi64x(0x0808080808080808ull)
	                  };

	for (int i = 0; i < 4; i++) {
		*sum ^= b4_256[i] &  _mm256_cmpeq_epi8(_mm256_and_si256(a_256, mask[i]), mask[i]);
	}
}

// use vtl nibble (Incomplete)
static inline void gf16_64_mul_64_add(gf16_t *a, gf16_t *b, gf16_t *c) {
	__m256i a_256 = *((__m256i *)a);
	__m256i b_256 = *((__m256i *)b);
	__m256i *sum = ((__m256i *)c);

	__m256i b4_256[4] = {b_256, _mm256_shuffle_epi8(mtk2_16[0x22], b_256), _mm256_shuffle_epi8(mtk2_16[0x44], b_256), _mm256_shuffle_epi8(mtk2_16[0x88], b_256)};

	__m256i mask_l[4] = {_mm256_set1_epi64x(0x0101010101010101ull), _mm256_set1_epi64x(0x0202020202020202ull),
	                     _mm256_set1_epi64x(0x0404040404040404ull), _mm256_set1_epi64x(0x0808080808080808ull)
	                    };

	__m256i mask_h[4] = {_mm256_set1_epi64x(0x1010101010101010ull), _mm256_set1_epi64x(0x2020202020202020ull),
	                     _mm256_set1_epi64x(0x4040404040404040ull), _mm256_set1_epi64x(0x8080808080808080ull)
	                    };

	for (int i = 0; i < 4; i++) {
		*sum ^= b4_256[i] &
		        (_mm256_cmpeq_epi8(_mm256_and_si256(a_256, mask_l[i]), mask_l[i]) & _mm256_set1_epi8(0x0f)) &
		        (_mm256_cmpeq_epi8(_mm256_and_si256(a_256, mask_h[i]), mask_h[i]) & _mm256_set1_epi8((uint8_t)0xf0));
	}
}

/**
 * Establish P22 during keygen
 */
#define mol_SNOVA32 ((m_SNOVA * o_SNOVA * l_SNOVA + 31) / 32)
#define mol_SNOVA (mol_SNOVA32 * 32)

/**
 * Generate public key (P22 part), use avx2 vtl
 * @param outP22 - output
 * @param T12 - input
 * @param P21 - input
 * @param F12 - input
 */
void gen_P22_vtl(P22_byte_t outP22, T12_t T12, P21_t P21, F12_t F12) {
	P22_t P22 = {0};

	uint8_t temp1_8[mol_SNOVA * o_SNOVA * l_SNOVA] __attribute__((aligned(32))) = {0};
	uint8_t F12_8[mol_SNOVA * v_SNOVA * l_SNOVA] __attribute__((aligned(32))) = {0};

	__m256i *temp1_256 = (__m256i *)temp1_8;
	__m256i *F12_256 = (__m256i *)F12_8;

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
					__m256i k_lh = vtl_ct_multtab(T12[di][dj][i1 * l_SNOVA + k1]);

					for (int mi_dk_j1 = 0; mi_dk_j1 < mol_SNOVA32; mi_dk_j1++)
						temp1_256[(dj * l_SNOVA + i1) * mol_SNOVA32 + mi_dk_j1] ^=
						    _mm256_shuffle_epi8(k_lh, F12_256[(di * l_SNOVA + k1) * mol_SNOVA32 + mi_dk_j1]);
				}

	for (int mi = 0; mi < m_SNOVA; ++mi)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int dk = 0; dk < o_SNOVA; ++dk)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						P22[mi][dj][dk][i1 * l_SNOVA + j1] ^=
						    temp1_8[(dj * l_SNOVA + i1) * mol_SNOVA + mi * o_SNOVA * l_SNOVA + dk * l_SNOVA + j1];

	uint8_t temp2_8[mol_SNOVA * o_SNOVA * l_SNOVA] __attribute__((aligned(32))) = {0};
	uint8_t P21_8[mol_SNOVA * v_SNOVA * l_SNOVA] __attribute__((aligned(32))) = {0};

	__m256i *temp2_256 = (__m256i *)temp2_8;
	__m256i *P21_256 = (__m256i *)P21_8;

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
					__m256i k_lh = vtl_ct_multtab(T12[di][dk][k1 * l_SNOVA + j1]);

					for (int mi_dj_i1 = 0; mi_dj_i1 < mol_SNOVA32; mi_dj_i1++)
						temp2_256[(dk * l_SNOVA + j1) * mol_SNOVA32 + mi_dj_i1] ^=
						    _mm256_shuffle_epi8(k_lh, P21_256[(di * l_SNOVA + k1) * mol_SNOVA32 + mi_dj_i1]);
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
#define mvl_SNOVA32 ((m_SNOVA * v_SNOVA * l_SNOVA + 31) / 32)
#define mvl_SNOVA8 (mvl_SNOVA32 * 32)
#define vl_SNOVA (v_SNOVA * l_SNOVA)

/**
 * Generate private key (F part), use avx2 vtl
 * @param map2 - output: F11 F12 F21
 * @param map1 - input: P11 P12 P21 Aalpha Balpha Qalpha1 Qalpha2
 * @param T12 - input
 */
void gen_F_vtl(map_group2 *map2, map_group1 *map1, T12_t T12) {
	__m256i p11_256[mvl_SNOVA32 * vl_SNOVA] = {0};
	__m256i t12_256[mvl_SNOVA32 * l_SNOVA] = {0};
	__m256i res256[mvl_SNOVA32 * o_SNOVA * l_SNOVA] = {0};

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
			__m256i k_lh = vtl_ct_multtab(t12_8[dk_k1 * o_SNOVA * l_SNOVA + dj_j1]);
			for (int mi_di_i1 = 0; mi_di_i1 < mvl_SNOVA32; mi_di_i1++) {
				res256[dj_j1 * mvl_SNOVA32 + mi_di_i1] ^= _mm256_shuffle_epi8(k_lh, p11_256[dk_k1 * mvl_SNOVA32 + mi_di_i1]);
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
			__m256i k_lh = vtl_ct_multtab(t12_8[dk_k1 * o_SNOVA * l_SNOVA + dj_j1]);
			for (int mi_di_i1 = 0; mi_di_i1 < mvl_SNOVA32; mi_di_i1++) {
				res256[dj_j1 * mvl_SNOVA32 + mi_di_i1] ^= _mm256_shuffle_epi8(k_lh, p11_256[dk_k1 * mvl_SNOVA32 + mi_di_i1]);
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

static inline uint8_t horizontal_xor_256(__m256i vec) {

	__m128i low = _mm256_castsi256_si128(vec);
	__m128i high = _mm256_extracti128_si256(vec, 1);
	__m128i combined = _mm_xor_si128(low, high);

	combined = _mm_xor_si128(combined, _mm_srli_si128(combined, 8));
	combined = _mm_xor_si128(combined, _mm_srli_si128(combined, 4));
	combined = _mm_xor_si128(combined, _mm_srli_si128(combined, 2));
	combined = _mm_xor_si128(combined, _mm_srli_si128(combined, 1));

	return _mm_extract_epi8(combined, 0);
}

#define vtl_len ((n_SNOVA * rank + 31) / 32)
#define vtl_v_len ((v_SNOVA * rank + 31) / 32)
#define vtl_o_len ((o_SNOVA * rank + 31) / 32)
#define vtl_mainRow_x_rank(mainRow) (((mainRow) * rank + 31) / 32 * 32)
#define vtl_mainRow_x_rank32(mainRow) (((mainRow) * rank + 31) / 32)
#define rank_next2 ((rank + 1) / 2 * 2)
#define rank_floor2 ((rank) / 2 * 2)
#define alpha_SNOVA_next2 ((alpha_SNOVA + 1) / 2 * 2)

// Optimize AVX2 alignment. Only useful for even rank
#if rank % 2
#define MJ_MAX mainRow
#else
#define MJ_MAX (vtl_mainRow_x_rank(mainRow)) / rank
#endif

static inline void jogressMatrix_avx2(uint8_t *AJ, const uint8_t *A, int mainCol, int mainRow) {
	for (int mi = 0; mi < mainCol; mi++) {
		for (int mj = 0; mj < mainRow; mj++) {
			for (int ei = 0; ei < rank; ei++) {
				for (int ej = 0; ej < rank; ej++) {
					// AJ[((mi * rank + ei) * (mainRow * rank)) + (mj * rank + ej)] =
					AJ[((mi * rank + ei) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ej)] =
					    A[(mi * (mainRow) * (rank * rank)) + (mj * (rank * rank)) + (ei * rank + ej)];
				}
			}
		}
	}
}

static inline void jogressTrMatrix_avx2(uint8_t *AJ, const uint8_t *A, int mainCol, int mainRow) {
	for (int mi = 0; mi < mainCol; mi++) {
		for (int mj = 0; mj < mainRow; mj++) {
			for (int ei = 0; ei < rank; ei++) {
				for (int ej = 0; ej < rank; ej++) {
					// AJ[((mi * rank + ei) * (mainRow * rank)) + (mj * rank + ej)] =
					AJ[((mi * rank + ei) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ej)] =
					    A[(mi * (mainRow) * (rank * rank)) + (mj * (rank * rank)) + (ej * rank + ei)];
					// ei <<---->> ej
				}
			}
		}
	}
}

static inline void jogressMatrixTr_avx2(uint8_t *AJ_tr, const uint8_t *AJ, int mainCol, int mainRow) {
	for (int mi = 0; mi < mainCol; mi++) {
		for (int mj = 0; mj < MJ_MAX; mj++) {
			for (int ei = 0; ei < rank; ei++) {
				for (int ej = 0; ej < rank; ej++) {
					AJ_tr[((mi * rank + ei) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ej)] =
					    AJ[((mi * rank + ej) * vtl_mainRow_x_rank(mainRow)) + (mj * rank + ei)];
					//  ei <<------------------------------------------>> ej
				}
			}
		}
	}
}

#endif
