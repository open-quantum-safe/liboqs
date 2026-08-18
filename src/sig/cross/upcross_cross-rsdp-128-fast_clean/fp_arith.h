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

#include <stdlib.h>
#include <string.h>

#include "csprng_hash.h"
#include "parameters.h"
#include "restr_arith.h"

#define NUM_BITS_P (BITS_TO_REPRESENT(P))

#define FPRED_SINGLE(x) (((x) & 0x7F) + ((x) >> 7))
#define FPRED_DOUBLE(x) FPRED_SINGLE(FPRED_SINGLE(x))
#define FPRED_OPPOSITE(x) ((x) ^ 0x7F)
#define FP_DOUBLE_ZERO_NORM(x) (((x) + (((x) + 1) >> 7)) & 0x7F)
#define RESTR_TO_VAL(x) ( (FP_ELEM) (RESTR_G_TABLE >> (8*(uint64_t)(x))) )



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
	for (int i = K ; i < N; i++) {
		res[i - K] = RESTR_TO_VAL(e[i]);
	}
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N - K; j++) {
			res[j] = FPRED_DOUBLE( (FP_DOUBLEPREC) res[j] +
			                       (FP_DOUBLEPREC) RESTR_TO_VAL(e[i]) *
			                       (FP_DOUBLEPREC) V_tr[i][j]);
		}
	}
}

static
void fp_vec_by_fp_matrix(FP_ELEM res[N - K],
                         FP_ELEM e[N],
                         FP_ELEM V_tr[K][N - K]) {
	memcpy(res, e + K, (N - K)*sizeof(FP_ELEM));
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N - K; j++) {
			res[j] = FPRED_DOUBLE( (FP_DOUBLEPREC) res[j] +
			                       (FP_DOUBLEPREC) e[i] *
			                       (FP_DOUBLEPREC) V_tr[i][j]);
		}
	}
}

static inline
void fp_vec_by_fp_vec_pointwise(FP_ELEM res[N],
                                const FP_ELEM in1[N],
                                const FP_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FPRED_DOUBLE( (FP_DOUBLEPREC) in1[i] *
		                       (FP_DOUBLEPREC) in2[i] );
	}
}

static inline
void restr_by_fp_vec_pointwise(FP_ELEM res[N],
                               const FZ_ELEM in1[N],
                               const FP_ELEM in2[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FPRED_DOUBLE( (FP_DOUBLEPREC) RESTR_TO_VAL(in1[i]) *
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
		res[i] = FPRED_DOUBLE( (FP_DOUBLEPREC) u_prime[i] +
		                       (FP_DOUBLEPREC) RESTR_TO_VAL(e[i]) * (FP_DOUBLEPREC) chall_1) ;
	}
}


static inline
void fp_synd_minus_fp_vec_scaled(FP_ELEM res[N - K],
                                 const FP_ELEM synd[N - K],
                                 const FP_ELEM chall_1,
                                 const FP_ELEM s[N - K]) {
	for (int j = 0; j < N - K; j++) {
		FP_ELEM tmp = FPRED_DOUBLE( (FP_DOUBLEPREC) s[j] * (FP_DOUBLEPREC) chall_1);
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
