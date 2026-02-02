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
			 * - the reduction function mm256_mod509_epu32(x) works for x < 8339743
			 * therefore 3 reductions are enough */
			if (i == K / 3 || i == (K / 3) * 2 || i == K - 1) {
				res_w  = mm256_mod509_epu32(res_w);
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
void fp_vec_by_restr_vec_scaled(FP_ELEM res[N], const FZ_ELEM e[N], const FP_ELEM chall_1, const FP_ELEM u_prime[N]) {

	/* res: expand, align */
	alignas(32) FP_ELEM res_x[ROUND_UP(N, EPI16_PER_REG)];
	/* e: convert from uint8 to uint16, expand, align */
	alignas(32) FP_ELEM e_x[ROUND_UP(N, EPI16_PER_REG)] = {0};
	for (int i = 0; i < N; i++) {
		e_x[i] = e[i];
	}
	/* chall_1: convert to m256i */
	__m256i chall_1_256 = _mm256_set1_epi16(chall_1);
	/* u_prime: expand, align */
	alignas(32) FP_ELEM u_prime_x[ROUND_UP(N, EPI16_PER_REG)];
	memcpy(u_prime_x, u_prime, N * sizeof(FP_ELEM));

	/* res = u_prime + RTV(e) * chall_1 */
	for (int i = 0; i < ROUND_UP(N, EPI16_PER_REG) / EPI16_PER_REG; i++ ) {
		__m256i u_prime_256 = _mm256_load_si256( (__m256i const *) &u_prime_x[i * EPI16_PER_REG] );
		__m256i e_256 = _mm256_load_si256( (__m256i const *) &e_x[i * EPI16_PER_REG] );
		__m256i r_256;
		r_256 = mm256_exp16mod509_epu16(e_256);
		r_256 = mm256_mulmod509_epu16(r_256, chall_1_256);
		r_256 = _mm256_add_epi16(r_256, u_prime_256);
		r_256 = mm256_mod509_epu16(r_256);
		_mm256_store_si256 ((__m256i *) &res_x[i * EPI16_PER_REG], r_256);
	}
	memcpy(res, res_x, N * sizeof(FP_ELEM));
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

/* liboqs-edit: don't inline for Category 1 (liboqs/issues/2343) */
static
void convert_restr_vec_to_fp(FP_ELEM res[N], const FZ_ELEM in[N]) {
	/* res: expand, align */
	alignas(32) FP_ELEM res_x[ROUND_UP(N, EPI16_PER_REG)];
	/* in: convert from uint8 to uint16, expand, align */
	alignas(32) FP_ELEM in_x[ROUND_UP(N, EPI16_PER_REG)] = {0};
	for (int i = 0; i < N; i++) {
		in_x[i] = in[i];
	}

	for (int i = 0; i < ROUND_UP(N, EPI16_PER_REG) / EPI16_PER_REG; i++ ) {
		__m256i in_256 = _mm256_load_si256( (__m256i const *) &in_x[i * EPI16_PER_REG] );
		__m256i res_256 = mm256_exp16mod509_epu16(in_256);
		_mm256_store_si256 ((__m256i *) &res_x[i * EPI16_PER_REG], res_256);
	}
	memcpy(res, res_x, N * sizeof(FP_ELEM));
}
