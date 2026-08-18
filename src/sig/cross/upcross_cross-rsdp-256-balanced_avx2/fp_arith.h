/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.2 (July 2025)
 *
 * Authors listed in alphabetical order:
 *
 * @author: Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author: Marco Gianvecchio <marco.gianvecchio@mail.polimi.it>
 * @author: Patrick Karl <patrick.karl@tum.de>
 * @author: Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author: Jonas Schupp <jonas.schupp@tum.de>
 *
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#pragma once

#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

#include "architecture_detect.h"
#include "csprng_hash.h"
#include "parameters.h"
#include "restr_arith.h"

#define NUM_BITS_P (BITS_TO_REPRESENT(P))

#define FPRED_SINGLE(x) (((x) & 0x7F) + ((x) >> 7))
#define FPRED_DOUBLE(x) FPRED_SINGLE(FPRED_SINGLE(x))
#define FPRED_OPPOSITE(x) ((x) ^ 0x7F)
#define FP_DOUBLE_ZERO_NORM(x) (((x) + (((x) + 1) >> 7)) & 0x7F)
#define RESTR_TO_VAL(x) ( (FP_ELEM) (RESTR_G_TABLE >> (8*(uint64_t)(x))) )


/* AVX2 utility functions */

/* reduce modulo 509 eigth 32-bit integers packed into a 256-bit vector, using Barrett's method
 * each 32-bit integer sould be in the range [0, 508*508] i.e. the result of a mul in FP
 * however, the function actually works for integers in the wider range [0, 8339743] */
static inline __m256i mm256_mod509_epu32(__m256i a) {
	int b_shift = 18; // ceil(log2(509))*2
	int b_mul = (((uint64_t)1U << b_shift) / P);
	/* r = a - ((B_MUL * a) >> B_SHIFT) * P) */
	__m256i b_mul_32 = _mm256_set1_epi32(b_mul);
	__m256i p_32 = _mm256_set1_epi32(P);
	__m256i r = _mm256_mullo_epi32(a, b_mul_32);
	r = _mm256_srli_epi32(r, b_shift);
	r = _mm256_mullo_epi32(r, p_32);
	r = _mm256_sub_epi32(a, r);
	/* r = min(r, r - P) */
	__m256i rs = _mm256_sub_epi32(r, p_32);
	r = _mm256_min_epu32(r, rs);
	return r;
}

/* reduce modulo 509 sixteen 16-bit integers packed into a 256-bit vector
 * each 16-bit integer sould be in the range [0, 508*2] */
static inline __m256i mm256_mod509_epu16(__m256i a) {
	/* r = min(r, r - P) */
	__m256i p_256 = _mm256_set1_epi16(509);
	__m256i as = _mm256_sub_epi16(a, p_256);
	return _mm256_min_epu16(a, as);
}

/* shuffle sixteen 16-bit integers packed into a 256-bit vector:
 * shuffle(a[], b[]) returns c[] where c[i]=a[b[i]]
 * operates within 128-bit lanes, so b[i] must be in the range [0,7] */
static inline __m256i mm256_shuffle_epi16(__m256i a, __m256i b) {
	__m256i x1 = _mm256_setr_epi8(0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14, 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14);
	__m256i x2 = _mm256_setr_epi8(0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
	b = _mm256_adds_epu16(b, b);
	b = _mm256_shuffle_epi8(b, x1);
	b = _mm256_adds_epu8(b, x2);
	b = _mm256_shuffle_epi8(a, b);
	return b;
}

/* for each 16-bit integer packed into a 256-bit vector, select one of two
 * values based on a boolean condition, without using if-else statements:
 * cmov(cond[], true_val[], false_val[]) returns r[] where r[i]=true_val[i]
 * if cond[i]==1, and r[i]=false_val[i] if cond[i]==0 */
static inline __m256i mm256_cmov_epu16(__m256i c, __m256i t, __m256i f) {
	__m256i zeros  = _mm256_setzero_si256();
	__m256i cmask  = _mm256_sub_epi16(zeros, c);
	__m256i cmaskn = _mm256_xor_si256(cmask, _mm256_set1_epi16(-1));
	__m256i tval   = _mm256_and_si256(cmask, t);
	__m256i fval   = _mm256_and_si256(cmaskn, f);
	__m256i r      = _mm256_or_si256(tval, fval);
	return r;
}

/* multiply 16-bit integers packed into 256-bit vectors and reduce the result
 * modulo 509: mulmod509(a[], b[]) returns c[] where c[i]=(a[i]*b[i])%509 */
static inline __m256i mm256_mulmod509_epu16(__m256i a, __m256i b) {
	/* multiply */
	__m256i l = _mm256_mullo_epi16(a, b);
	__m256i h = _mm256_mulhi_epu16(a, b);
	/* unpack 16-bit to 32-bit */
	__m256i u0 = _mm256_unpacklo_epi16(l, h);
	__m256i u1 = _mm256_unpackhi_epi16(l, h);
	/* reduce */
	u0 = mm256_mod509_epu32(u0);
	u1 = mm256_mod509_epu32(u1);
	/* pack 32-bit to 16-bit */
	__m256i r = _mm256_packs_epi32(u0, u1);
	return r;
}

/* for each 16-bit integer x packed into a 256-bit vector, with x in [1, 127],
 * compute: (16^x) mod 509 */
static inline __m256i mm256_exp16mod509_epu16(__m256i a) {
	/* high 3 bits */
	__m256i h3 = _mm256_srli_epi16(a, 4);
	__m256i pre_h3 = _mm256_setr_epi16(
	                     1, 302, 93, 91, 505, 319, 137, 145,
	                     1, 302, 93, 91, 505, 319, 137, 145);
	__m256i h3_shu = mm256_shuffle_epi16(pre_h3, h3);
	/* low 4 bits */
	__m256i mask_l4 = _mm256_set1_epi16(0x0F); //0b1111
	__m256i l4 = _mm256_and_si256(a, mask_l4);
	__m256i mask_l4_bit4 = _mm256_set1_epi16(0x8); //0b1000
	__m256i l4_bit4 = _mm256_and_si256(a, mask_l4_bit4);
	l4_bit4 = _mm256_srli_epi16(l4_bit4, 3);
	__m256i l4_sub8 = _mm256_sub_epi16(l4, _mm256_set1_epi16(8));
	__m256i pre_l4_0 = _mm256_setr_epi16(
	                       1, 16, 256, 24, 384, 36, 67, 54,
	                       1, 16, 256, 24, 384, 36, 67, 54);
	__m256i l4_shu_0 = mm256_shuffle_epi16(pre_l4_0, l4);
	__m256i pre_l4_1 = _mm256_setr_epi16(
	                       355, 81, 278, 376, 417, 55, 371, 337,
	                       355, 81, 278, 376, 417, 55, 371, 337);
	__m256i l4_shu_1 = mm256_shuffle_epi16(pre_l4_1, l4_sub8);
	__m256i l4_shu = mm256_cmov_epu16(l4_bit4, l4_shu_1, l4_shu_0);
	/* multiply */
	__m256i r = mm256_mulmod509_epu16(h3_shu, l4_shu);
	return r;
}



/* in-place normalization of redundant zero representation for syndromes*/
static inline
void fp_dz_norm_synd(FP_ELEM v[N - K]) {
	for (int i = 0; i < N - K; i++) {
		v[i] = FP_DOUBLE_ZERO_NORM(v[i]);
	}
}

static inline
void fp_dz_norm(FP_ELEM v[N]) {
	for (int i = 0; i < N; i++) {
		v[i] = FP_DOUBLE_ZERO_NORM(v[i]);
	}
}

/* Computes the product e*H of an n-element restricted vector by a (n-k)*n
 * FP H is in systematic form. Only the non systematic portion of H =[V I],
 * V, is provided, transposed, hence linearized by columns so that syndrome
 * computation is vectorizable. */

static
void restr_vec_by_fp_matrix(FP_ELEM res[N - K],
                            FZ_ELEM e[N],
                            FP_ELEM V_tr[K][N - K]) {
	FP_DOUBLEPREC res_dprec[N - K] = {0};
	for (int i = 0; i < N - K; i++) {
		res_dprec[i] = RESTR_TO_VAL(e[K + i]);
	}
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N - K; j++) {
			res_dprec[j] += FPRED_SINGLE(
			                    (FP_DOUBLEPREC) RESTR_TO_VAL(e[i]) *
			                    (FP_DOUBLEPREC) V_tr[i][j]);
			if (i == P - 1) {
				res_dprec[j] = FPRED_SINGLE(res_dprec[j]);
			}
		}
	}
	/* Save result trimming to regular precision */
	for (int i = 0; i < N - K; i++) {
		res[i] = FPRED_SINGLE(res_dprec[i]);
	}
}


/* Computes e * [I_k V]^T, V is already in transposed form
 * since  */
static
void fp_vec_by_fp_matrix(FP_ELEM res[N - K],
                         FP_ELEM e[N],
                         FP_DOUBLEPREC V_tr[K][ROUND_UP(N - K, EPI16_PER_REG)]) {

	alignas(EPI8_PER_REG) FP_DOUBLEPREC res_dprec[ROUND_UP(N - K, EPI16_PER_REG)] = {0};
	for (int i = 0; i < N - K; i++) {
		res_dprec[i] = e[K + i];
	}
	__m256i mred_mask = _mm256_set1_epi16 (0x007f);

	for (int i = 0; i < K; i++) {
		for (int j = 0; j < ROUND_UP(N - K, EPI16_PER_REG) / EPI16_PER_REG; j++) {
			__m256i res_w = _mm256_load_si256(
			                    (__m256i const *) &res_dprec[j * EPI16_PER_REG] );
			__m256i e_coeff = _mm256_set1_epi16(e[i]);
			__m256i V_tr_slice = _mm256_lddqu_si256(
			                         (__m256i const *) &V_tr[i][j * EPI16_PER_REG] );
			__m256i tmp = _mm256_mullo_epi16(e_coeff, V_tr_slice);
			/* Vector Mersenne reduction */
			__m256i tmp2 = _mm256_and_si256 (tmp, mred_mask);
			tmp = _mm256_srli_epi16(tmp, 7);
			tmp = _mm256_add_epi16(tmp, tmp2);
			res_w = _mm256_add_epi16(res_w, tmp);
			/* store back*/
			_mm256_store_si256 ((__m256i *) &res_dprec[j * EPI16_PER_REG], res_w);
		}
	}
	/* Save result trimming to regular precision */
	for (int i = 0; i < N - K; i++) {
		res[i] = FPRED_DOUBLE(res_dprec[i]);
	}
}


static inline
void fp_vec_by_fp_vec_pointwise(FP_ELEM res[N],
                                const FP_ELEM in1[N],
                                const FP_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FPRED_SINGLE( (FP_DOUBLEPREC) in1[i] *
		                       (FP_DOUBLEPREC) in2[i] );
	}
}

static inline
void restr_by_fp_vec_pointwise(FP_ELEM res[N],
                               const FZ_ELEM in1[N],
                               const FP_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FPRED_SINGLE( (FP_DOUBLEPREC) RESTR_TO_VAL(in1[i]) *
		                       (FP_DOUBLEPREC) in2[i]);
	}
}

/* e*chall_1 + u_prime*/
static inline
void fp_vec_by_restr_vec_scaled(FP_ELEM res[N],
                                const FZ_ELEM e[N],
                                const FP_ELEM chall_1,
                                const FP_ELEM u_prime[N]) {

	alignas(EPI8_PER_REG) FZ_ELEM e_align[ROUND_UP(N, EPI8_PER_REG)];
	alignas(EPI8_PER_REG) FP_ELEM u_prime_align[ROUND_UP(N, EPI8_PER_REG)];
	alignas(EPI8_PER_REG) FP_ELEM res_align[ROUND_UP(N, EPI8_PER_REG)];
	__m256i mred_mask = _mm256_set1_epi16 ((uint16_t)0x007f);
	__m256i dense_mred_mask = _mm256_set1_epi8 ((uint8_t)0x7f);
	__m256i dense_neg_mred_mask = _mm256_set1_epi8 ((uint8_t)0x80);
	memcpy(e_align, e, N);
	memcpy(u_prime_align, u_prime, N);

	/* set a register with chall_1, EPI16_PER_REG times */
	__m256i chall_1_comb = _mm256_set1_epi16 ((uint16_t) chall_1);

	/* Since the single-cycle shuffle acts only on 128b lanes separately,
	 * prepare two pairs of tables, with alternating scattered elements, to be
	 * broadcast-multiplied. Results are then reduced, and byte packed again
	 * to allow the use of the shuffle instruction */
	__m256i LUT_sparse = _mm256_set_epi16 (0x0001, 0x0020, 0x0008, 0x0002,
	                                       0x0040, 0x0010, 0x0004, 0x0001,
	                                       0x0001, 0x0020, 0x0008, 0x0002,
	                                       0x0040, 0x0010, 0x0004, 0x0001);

	/* comb-multiply the lookup table made of a single register obtaining
	 * pre-scaled values */
	LUT_sparse = _mm256_mullo_epi16(LUT_sparse, chall_1_comb);
	/* Vector Mersenne reduction */
	__m256i tmp = _mm256_and_si256 (LUT_sparse, mred_mask);
	LUT_sparse = _mm256_srli_epi16(LUT_sparse, 7);
	LUT_sparse = _mm256_add_epi16(tmp, LUT_sparse);
	/*semantics from the manual call for a *byte amount* in _mm256_srli_si256*/
	tmp = _mm256_srli_si256(LUT_sparse, 7);
	LUT_sparse = _mm256_or_si256(LUT_sparse, tmp);

	/* Convert restricted to rescaled values by batch exponentiating them
	 * via shuffle: "shuffling" the table according to the restricted values
	 * yields chall_1-multiplied elements of FP*/
	for (int i = 0; i < ROUND_UP(N, EPI8_PER_REG) / EPI8_PER_REG; i++ ) {
		__m256i e_word = _mm256_load_si256( (__m256i const *) &e_align[i * EPI8_PER_REG]);
		e_word = _mm256_shuffle_epi8(LUT_sparse, e_word);
		/* add to u_prime */
		__m256i u_prime_word = _mm256_load_si256( (__m256i const *) &u_prime_align[i * EPI8_PER_REG]);
		__m256i res_word = _mm256_add_epi8(e_word, u_prime_word);
		/* reduce, knowing that a single out bit is the max overflow */
		tmp = _mm256_and_si256 (res_word, dense_mred_mask);
		/* no _mm256_srli_epi8 available, cope with the lack hand-clearing
		 * all other bits before shifting */
		res_word = _mm256_srli_epi16(
		               _mm256_and_si256(res_word, dense_neg_mred_mask),
		               7);
		res_word = _mm256_add_epi8(res_word, tmp);
		_mm256_store_si256 ((__m256i *) &res_align[i * EPI8_PER_REG], res_word);

	}
	memcpy(res, res_align, N);
}



static inline
void fp_synd_minus_fp_vec_scaled(FP_ELEM res[N - K],
                                 const FP_ELEM synd[N - K],
                                 const FP_ELEM chall_1,
                                 const FP_ELEM s[N - K]) {
	for (int j = 0; j < N - K; j++) {
		FP_ELEM tmp = FPRED_SINGLE( (FP_DOUBLEPREC) s[j] * (FP_DOUBLEPREC) chall_1);
		tmp = FP_DOUBLE_ZERO_NORM(tmp);
		res[j] = FPRED_SINGLE( (FP_DOUBLEPREC) synd[j] + FPRED_OPPOSITE(tmp) );
	}
}

static inline
void convert_restr_vec_to_fp(FP_ELEM res[N],
                             const FZ_ELEM in[N]) {
	for (int j = 0; j < N; j++) {
		res[j] = RESTR_TO_VAL(in[j]);
	}
}
