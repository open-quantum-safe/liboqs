/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
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

#define NUM_BITS_Q (BITS_TO_REPRESENT(Q))

#define FQRED_SINGLE(x) (((x) & 0x7F) + ((x) >> 7))
#define FQRED_DOUBLE(x) FQRED_SINGLE(FQRED_SINGLE(x))
#define FQRED_OPPOSITE(x) ((x) ^ 0x7F)
#define FQ_DOUBLE_ZERO_NORM(x) (((x) + (((x) + 1) >> 7)) & 0x7F)
#define RESTR_TO_VAL(x) ( (FQ_ELEM) (RESTR_G_TABLE >> (8*(uint64_t)(x))) )

/* in-place normalization of redundant zero representation for syndromes*/
static inline
void fq_dz_norm_synd(FQ_ELEM v[N - K]) {
	for (int i = 0; i < N - K; i++) {
		v[i] = FQ_DOUBLE_ZERO_NORM(v[i]);
	}
}

static inline
void fq_dz_norm(FQ_ELEM v[N]) {
	for (int i = 0; i < N; i++) {
		v[i] = FQ_DOUBLE_ZERO_NORM(v[i]);
	}
}

/* Computes the product e*H of an n-element restricted vector by a (n-k)*n
 * F_q H is in systematic form. Only the non systematic portion of H =[V I],
 * V, is provided, transposed, hence linearized by columns so that syndrome
 * computation is vectorizable. */
/* Used by keygen */
static
void restr_vec_by_fq_matrix(FQ_ELEM res[N - K],
                            const FZ_ELEM e[N],
                            FQ_ELEM V_tr[K][N - K]) {
	FQ_DOUBLEPREC res_dprec[N - K] = {0};
	for (int i = 0; i < N - K; i++) {
		res_dprec[i] = RESTR_TO_VAL(e[K + i]);
	}
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N - K; j++) {
			res_dprec[j] += FQRED_SINGLE(
			                    (FQ_DOUBLEPREC) RESTR_TO_VAL(e[i]) *
			                    (FQ_DOUBLEPREC) V_tr[i][j]);
			if (i == Q - 1) {
				res_dprec[j] = FQRED_SINGLE(res_dprec[j]);
			}
		}
	}
	/* Save result trimming to regular precision */
	for (int i = 0; i < N - K; i++) {
		res[i] = FQRED_SINGLE(res_dprec[i]);
	}
}

/* Computes e * [I_k V]^T, V is already in transposed form
 * since  */
static
void fq_vec_by_fq_matrix(FQ_ELEM res[N - K],
                         const FQ_ELEM e[N],
                         FQ_DOUBLEPREC V_tr[K][ROUND_UP(N - K, EPI16_PER_REG)]) {

	alignas(EPI8_PER_REG) FQ_DOUBLEPREC res_dprec[ROUND_UP(N - K, EPI16_PER_REG)] = {0};
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
		res[i] = FQRED_DOUBLE(res_dprec[i]);
	}
}

static inline
void fq_vec_by_fq_vec_pointwise(FQ_ELEM res[N],
                                const FQ_ELEM in1[N],
                                const FQ_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FQRED_SINGLE( (FQ_DOUBLEPREC) in1[i] *
		                       (FQ_DOUBLEPREC) in2[i] );
	}
}

static inline
void restr_by_fq_vec_pointwise(FQ_ELEM res[N],
                               const FZ_ELEM in1[N],
                               const FQ_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FQRED_SINGLE( (FQ_DOUBLEPREC) RESTR_TO_VAL(in1[i]) *
		                       (FQ_DOUBLEPREC) in2[i]);
	}
}

/* e*beta + u_tilde*/
static inline
void fq_vec_by_restr_vec_scaled(FQ_ELEM res[N],
                                const FZ_ELEM e[N],
                                const FQ_ELEM beta,
                                const FQ_ELEM u_tilde[N]) {

	alignas(EPI8_PER_REG) FZ_ELEM e_align[ROUND_UP(N, EPI8_PER_REG)];
	alignas(EPI8_PER_REG) FQ_ELEM utilde_align[ROUND_UP(N, EPI8_PER_REG)];
	alignas(EPI8_PER_REG) FQ_ELEM res_align[ROUND_UP(N, EPI8_PER_REG)];
	__m256i mred_mask = _mm256_set1_epi16 ((uint16_t)0x007f);
	__m256i dense_mred_mask = _mm256_set1_epi8 ((uint8_t)0x7f);
	__m256i dense_neg_mred_mask = _mm256_set1_epi8 ((uint8_t)0x80);
	memcpy(e_align, e, N);
	memcpy(utilde_align, u_tilde, N);

	/* set a register with beta, EPI16_PER_REG times */
	__m256i beta_comb = _mm256_set1_epi16 ((uint16_t) beta);

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
	LUT_sparse = _mm256_mullo_epi16(LUT_sparse, beta_comb);
	/* Vector Mersenne reduction */
	__m256i tmp = _mm256_and_si256 (LUT_sparse, mred_mask);
	LUT_sparse = _mm256_srli_epi16(LUT_sparse, 7);
	LUT_sparse = _mm256_add_epi16(tmp, LUT_sparse);
	/*semantics from the manual call for a *byte amount* in _mm256_srli_si256*/
	tmp = _mm256_srli_si256(LUT_sparse, 7);
	LUT_sparse = _mm256_or_si256(LUT_sparse, tmp);

	/* Convert restricted to rescaled values by batch exponentiating them
	 * via shuffle: "shuffling" the table according to the restricted values
	 * yields beta-multiplied elements of F_q*/
	for (int i = 0; i < ROUND_UP(N, EPI8_PER_REG) / EPI8_PER_REG; i++ ) {
		__m256i e_word = _mm256_load_si256( (__m256i const *) &e_align[i * EPI8_PER_REG]);
		e_word = _mm256_shuffle_epi8(LUT_sparse, e_word);
		/* add to u tilde */
		__m256i utilde_word = _mm256_load_si256( (__m256i const *) &utilde_align[i * EPI8_PER_REG]);
		__m256i res_word = _mm256_add_epi8(e_word, utilde_word);
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
void fq_synd_minus_fq_vec_scaled(FQ_ELEM res[N - K],
                                 const FQ_ELEM synd[N - K],
                                 const FQ_ELEM beta,
                                 const FQ_ELEM s[N - K]) {
	for (int j = 0; j < N - K; j++) {
		FQ_ELEM tmp = FQRED_SINGLE( (FQ_DOUBLEPREC) s[j] * (FQ_DOUBLEPREC) beta);
		tmp = FQ_DOUBLE_ZERO_NORM(tmp);
		res[j] = FQRED_SINGLE( (FQ_DOUBLEPREC) synd[j] + FQRED_OPPOSITE(tmp) );
	}
}

static inline
void convert_restr_vec_to_fq(FQ_ELEM res[N],
                             const FZ_ELEM in[N]) {
	for (int j = 0; j < N; j++) {
		res[j] = RESTR_TO_VAL(in[j]);
	}
}
