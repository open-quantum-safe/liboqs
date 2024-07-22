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

#include <stdlib.h>
#include <string.h>

#include "csprng_hash.h"
#include "parameters.h"
#include "restr_arith.h"

#define NUM_BITS_Q (BITS_TO_REPRESENT(Q))

#define FQRED_SINGLE(x) ((x)% Q)
#define FQRED_DOUBLE(x) FQRED_SINGLE(FQRED_SINGLE(x))
#define FQRED_OPPOSITE(x) ((Q-(x)) % Q)
/* no redundant zero notation in F_509 */
#define FQ_DOUBLE_ZERO_NORM(x) (x)

/* for i in [0,1,2,4,8,16,32,64] RESTR_G_GEN**i mod 509 yields
 * [1, 16, 256, 384, 355, 302, 93, 505]
 * the following is a precomputed-squares S&M, to be optimized into muxed
 * register stored tables */

#define RESTR_G_GEN_1  ((FQ_ELEM)RESTR_G_GEN)
#define RESTR_G_GEN_2  ((FQ_ELEM) 256)
#define RESTR_G_GEN_4  ((FQ_ELEM) 384)
#define RESTR_G_GEN_8  ((FQ_ELEM) 355)
#define RESTR_G_GEN_16 ((FQ_ELEM) 302)
#define RESTR_G_GEN_32 ((FQ_ELEM) 93)
#define RESTR_G_GEN_64 ((FQ_ELEM) 505)

#define FQ_ELEM_CMOV(BIT,TRUE_V,FALSE_V)  ( (((FQ_ELEM)0 - (BIT)) & (TRUE_V)) | (~((FQ_ELEM)0 - (BIT)) & (FALSE_V)) )

/* log reduction, constant time unrolled S&M w/precomputed squares.
 * To be further optimized with muxed register-fitting tables */
static inline
FQ_ELEM RESTR_TO_VAL(FQ_ELEM x) {
	FQ_ELEM res1, res2, res3, res4;
	res1 = FQRED_SINGLE(
	           ( FQ_ELEM_CMOV(((x >> 0) & 1), RESTR_G_GEN_1, 1)) *
	           ( FQ_ELEM_CMOV(((x >> 1) & 1), RESTR_G_GEN_2, 1)) );
	res2 = FQRED_SINGLE(
	           ( FQ_ELEM_CMOV(((x >> 2) & 1), RESTR_G_GEN_4, 1)) *
	           ( FQ_ELEM_CMOV(((x >> 3) & 1), RESTR_G_GEN_8, 1)) );
	res3 = FQRED_SINGLE(
	           ( FQ_ELEM_CMOV(((x >> 4) & 1), RESTR_G_GEN_16, 1)) *
	           ( FQ_ELEM_CMOV(((x >> 5) & 1), RESTR_G_GEN_32, 1)) );
	res4 = FQ_ELEM_CMOV(((x >> 6) & 1), RESTR_G_GEN_64, 1);

	return FQRED_SINGLE( FQRED_SINGLE(res1 * res2) *
	                     FQRED_SINGLE(res3 * res4) );
}

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
/* computes the product e*H of an n-element restricted vector by a (n-k)*n
 * F_q H is in systematic form. Only the non systematic portion of H =[V I],
 * V, is provided, transposed, hence linearized by columns so that syndrome
 * computation is vectorizable. */

static
void restr_vec_by_fq_matrix(FQ_ELEM res[N - K],
                            const FZ_ELEM e[N],
                            FQ_ELEM V_tr[K][N - K]) {
	for (int i = K ; i < N; i++) {
		res[i - K] = RESTR_TO_VAL(e[i]);
	}
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N - K; j++) {
			res[j] = FQRED_DOUBLE( (FQ_DOUBLEPREC) res[j] +
			                       (FQ_DOUBLEPREC) RESTR_TO_VAL(e[i]) *
			                       (FQ_DOUBLEPREC) V_tr[i][j]);
		}
	}
}

static
void fq_vec_by_fq_matrix(FQ_ELEM res[N - K],
                         const FQ_ELEM e[N],
                         FQ_ELEM V_tr[K][N - K]) {
	memcpy(res, e + K, (N - K)*sizeof(FQ_ELEM));
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N - K; j++) {
			res[j] = FQRED_DOUBLE( (FQ_DOUBLEPREC) res[j] +
			                       (FQ_DOUBLEPREC) e[i] *
			                       (FQ_DOUBLEPREC) V_tr[i][j]);
		}
	}
}

static inline
void fq_vec_by_fq_vec_pointwise(FQ_ELEM res[N],
                                const FQ_ELEM in1[N],
                                const FQ_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FQRED_DOUBLE( (FQ_DOUBLEPREC) in1[i] *
		                       (FQ_DOUBLEPREC) in2[i] );
	}
}

static inline
void restr_by_fq_vec_pointwise(FQ_ELEM res[N],
                               const FZ_ELEM in1[N],
                               const FQ_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FQRED_DOUBLE( (FQ_DOUBLEPREC) RESTR_TO_VAL(in1[i]) *
		                       (FQ_DOUBLEPREC) in2[i]);
	}
}

/* e*beta + u_tilde*/
static inline
void fq_vec_by_restr_vec_scaled(FQ_ELEM res[N],
                                const FZ_ELEM e[N],
                                const FQ_ELEM beta,
                                const FQ_ELEM u_tilde[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FQRED_DOUBLE( (FQ_DOUBLEPREC) u_tilde[i] +
		                       (FQ_DOUBLEPREC) RESTR_TO_VAL(e[i]) * (FQ_DOUBLEPREC) beta) ;
	}
}

static inline
void fq_synd_minus_fq_vec_scaled(FQ_ELEM res[N - K],
                                 const FQ_ELEM synd[N - K],
                                 const FQ_ELEM beta,
                                 const FQ_ELEM s[N - K]) {
	for (int j = 0; j < N - K; j++) {
		FQ_ELEM tmp = FQRED_DOUBLE( (FQ_DOUBLEPREC) s[j] * (FQ_DOUBLEPREC) beta);
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
