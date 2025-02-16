/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.0 (February 2025)
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

/* Reduction modulo P=509 as shown in:
 * Hacker's Delight, Second Edition, Chapter 10, Figure 10-4
 * Works for integers in the range [0,4294967295] i.e. all uint32_t */
#define FPRED_SINGLE(x) (((x) - (((uint64_t)(x) * 2160140723) >> 40) * P))
#define FPRED_DOUBLE(x) (FPRED_SINGLE(x))
#define FPRED_OPPOSITE(x) (FPRED_SINGLE(P - (x)))
/* no redundant zero notation in F_509 */
#define FP_DOUBLE_ZERO_NORM(x) (x)

/* for i in [0,1,2,4,8,16,32,64] RESTR_G_GEN**i mod 509 yields
 * [1, 16, 256, 384, 355, 302, 93, 505]
 * the following is a precomputed-squares S&M, to be optimized into muxed
 * register stored tables */

#define RESTR_G_GEN_1  ((FP_ELEM)RESTR_G_GEN)
#define RESTR_G_GEN_2  ((FP_ELEM) 256)
#define RESTR_G_GEN_4  ((FP_ELEM) 384)
#define RESTR_G_GEN_8  ((FP_ELEM) 355)
#define RESTR_G_GEN_16 ((FP_ELEM) 302)
#define RESTR_G_GEN_32 ((FP_ELEM) 93)
#define RESTR_G_GEN_64 ((FP_ELEM) 505)

#define FP_ELEM_CMOV(BIT,TRUE_V,FALSE_V)  ( (((FP_ELEM)0 - (BIT)) & (TRUE_V)) | (~((FP_ELEM)0 - (BIT)) & (FALSE_V)) )

/* log reduction, constant time unrolled S&M w/precomputed squares.
 * To be further optimized with muxed register-fitting tables */
static inline
FP_ELEM RESTR_TO_VAL(FP_ELEM x) {
	uint32_t res1, res2, res3, res4;
	res1 = ( FP_ELEM_CMOV(((x >> 0) & 1), RESTR_G_GEN_1, 1)) *
	       ( FP_ELEM_CMOV(((x >> 1) & 1), RESTR_G_GEN_2, 1)) ;
	res2 = ( FP_ELEM_CMOV(((x >> 2) & 1), RESTR_G_GEN_4, 1)) *
	       ( FP_ELEM_CMOV(((x >> 3) & 1), RESTR_G_GEN_8, 1)) ;
	res3 = ( FP_ELEM_CMOV(((x >> 4) & 1), RESTR_G_GEN_16, 1)) *
	       ( FP_ELEM_CMOV(((x >> 5) & 1), RESTR_G_GEN_32, 1)) ;
	res4 =   FP_ELEM_CMOV(((x >> 6) & 1), RESTR_G_GEN_64, 1);

	/* Two intermediate reductions necessary:
	 *     RESTR_G_GEN_1*RESTR_G_GEN_2*RESTR_G_GEN_4*RESTR_G_GEN_8    < 2^32
	 *     RESTR_G_GEN_16*RESTR_G_GEN_32*RESTR_G_GEN_64               < 2^32 */
	return FPRED_SINGLE( FPRED_SINGLE(res1 * res2) * FPRED_SINGLE(res3 * res4) );
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

/* reduce modulo 509 eigth 32-bit integers packed into a 256-bit vector, using Barrett's method
 * each 32-bit integer sould be in the range [0, 508*508] i.e. the result of a mul in FP
 * however, the function actually works for integers in the wider range [0, 8339743] */
static inline
__m256i reduce_avx2_32(__m256i a) {
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
static inline
void fp_vec_by_fp_matrix(FP_ELEM res[N - K], FP_ELEM e[N], FP_DOUBLEPREC V_tr[K][ROUND_UP(N - K, EPI32_PER_REG)]) {

	alignas(EPI8_PER_REG) FP_DOUBLEPREC res_dprec[ROUND_UP(N - K, EPI32_PER_REG)] = {0};
	for (int i = 0; i < N - K; i++) {
		res_dprec[i] = e[K + i];
	}

	for (int i = 0; i < K; i++) {
		__m256i e_coeff = _mm256_set1_epi32(e[i]);
		for (int j = 0; j < ROUND_UP(N - K, EPI32_PER_REG) / EPI32_PER_REG; j++) {
			__m256i res_w = _mm256_load_si256((__m256i const *) &res_dprec[j * EPI32_PER_REG] );
			__m256i V_tr_slice = _mm256_lddqu_si256((__m256i const *) &V_tr[i][j * EPI32_PER_REG] );
			__m256i a  = _mm256_mullo_epi32(e_coeff, V_tr_slice);
			/* add to result */
			res_w = _mm256_add_epi32(res_w, a);
			/* - the previous sum is performed K times with K <= 69
			 * - adding each time a value "a" in the range [0,(P-1)*(P-1)]
			 * - the reduction function reduce_avx2_32(x) works for x < 8339743
			 * therefore 3 reductions are enough */
			if (i == K / 3 || i == (K / 3) * 2 || i == K - 1) {
				res_w  = reduce_avx2_32(res_w);
			}
			/* store back */
			_mm256_store_si256 ((__m256i *) &res_dprec[j * EPI32_PER_REG], res_w);
		}
	}
	for (int i = 0; i < N - K; i++) {
		res[i] = res_dprec[i];
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
	for (int i = 0; i < N; i++) {
		res[i] = FPRED_SINGLE( (FP_DOUBLEPREC) u_prime[i] +
		                       (FP_DOUBLEPREC) RESTR_TO_VAL(e[i]) * (FP_DOUBLEPREC) chall_1) ;
	}
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
