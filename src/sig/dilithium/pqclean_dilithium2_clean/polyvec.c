#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <stdint.h>

/*************************************************
* Name:        expand_mat
*
* Description: Implementation of ExpandA. Generates matrix A with uniformly
*              random coefficients a_{i,j} by performing rejection
*              sampling on the output stream of SHAKE128(rho|j|i)
*              or AES256CTR(rho,j|i).
*
* Arguments:   - polyvecl mat[K]: output matrix
*              - const uint8_t rho[]: byte array containing seed rho
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyvec_matrix_expand(polyvecl mat[K], const uint8_t rho[SEEDBYTES]) {
    unsigned int i, j;

    for (i = 0; i < K; ++i) {
        for (j = 0; j < L; ++j) {
            PQCLEAN_DILITHIUM2_CLEAN_poly_uniform(&mat[i].vec[j], rho, (uint16_t) ((i << 8) + j));
        }
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyvec_matrix_pointwise_montgomery(polyveck *t, const polyvecl mat[K], const polyvecl *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyvecl_pointwise_acc_montgomery(&t->vec[i], &mat[i], v);
    }
}

/**************************************************************/
/************ Vectors of polynomials of length L **************/
/**************************************************************/

void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_uniform_eta(polyvecl *v, const uint8_t seed[SEEDBYTES], uint16_t nonce) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_uniform_eta(&v->vec[i], seed, nonce++);
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_uniform_gamma1(polyvecl *v, const uint8_t seed[CRHBYTES], uint16_t nonce) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_uniform_gamma1(&v->vec[i], seed, (uint16_t) (L * nonce + i));
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_reduce(polyvecl *v) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_reduce(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyvecl_freeze
*
* Description: Reduce coefficients of polynomials in vector of length L
*              to standard representatives.
*
* Arguments:   - polyvecl *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_freeze(polyvecl *v) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_freeze(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyvecl_add
*
* Description: Add vectors of polynomials of length L.
*              No modular reduction is performed.
*
* Arguments:   - polyvecl *w: pointer to output vector
*              - const polyvecl *u: pointer to first summand
*              - const polyvecl *v: pointer to second summand
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_add(polyvecl *w, const polyvecl *u, const polyvecl *v) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_add(&w->vec[i], &u->vec[i], &v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyvecl_ntt
*
* Description: Forward NTT of all polynomials in vector of length L. Output
*              coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyvecl *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_ntt(polyvecl *v) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_ntt(&v->vec[i]);
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_invntt_tomont(polyvecl *v) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_invntt_tomont(&v->vec[i]);
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_pointwise_poly_montgomery(polyvecl *r, const poly *a, const polyvecl *v) {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyvecl_pointwise_acc_montgomery
*
* Description: Pointwise multiply vectors of polynomials of length L, multiply
*              resulting vector by 2^{-32} and add (accumulate) polynomials
*              in it. Input/output vectors are in NTT domain representation.
*
* Arguments:   - poly *w: output polynomial
*              - const polyvecl *u: pointer to first input vector
*              - const polyvecl *v: pointer to second input vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyvecl_pointwise_acc_montgomery(poly *w,
        const polyvecl *u,
        const polyvecl *v) {
    unsigned int i;
    poly t;

    PQCLEAN_DILITHIUM2_CLEAN_poly_pointwise_montgomery(w, &u->vec[0], &v->vec[0]);
    for (i = 1; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_pointwise_montgomery(&t, &u->vec[i], &v->vec[i]);
        PQCLEAN_DILITHIUM2_CLEAN_poly_add(w, w, &t);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyvecl_chknorm
*
* Description: Check infinity norm of polynomials in vector of length L.
*              Assumes input polyvecl to be reduced by PQCLEAN_DILITHIUM2_CLEAN_polyvecl_reduce().
*
* Arguments:   - const polyvecl *v: pointer to vector
*              - int32_t B: norm bound
*
* Returns 0 if norm of all polynomials is strictly smaller than B <= (Q-1)/8
* and 1 otherwise.
**************************************************/
int PQCLEAN_DILITHIUM2_CLEAN_polyvecl_chknorm(const polyvecl *v, int32_t bound)  {
    unsigned int i;

    for (i = 0; i < L; ++i) {
        if (PQCLEAN_DILITHIUM2_CLEAN_poly_chknorm(&v->vec[i], bound)) {
            return 1;
        }
    }

    return 0;
}

/**************************************************************/
/************ Vectors of polynomials of length K **************/
/**************************************************************/

void PQCLEAN_DILITHIUM2_CLEAN_polyveck_uniform_eta(polyveck *v, const uint8_t seed[SEEDBYTES], uint16_t nonce) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_uniform_eta(&v->vec[i], seed, nonce++);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_reduce
*
* Description: Reduce coefficients of polynomials in vector of length K
*              to representatives in [-6283009,6283007].
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_reduce(polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_reduce(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_caddq
*
* Description: For all coefficients of polynomials in vector of length K
*              add Q if coefficient is negative.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_caddq(polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_caddq(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_freeze
*
* Description: Reduce coefficients of polynomials in vector of length K
*              to standard representatives.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_freeze(polyveck *v)  {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_freeze(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_add
*
* Description: Add vectors of polynomials of length K.
*              No modular reduction is performed.
*
* Arguments:   - polyveck *w: pointer to output vector
*              - const polyveck *u: pointer to first summand
*              - const polyveck *v: pointer to second summand
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_add(polyveck *w, const polyveck *u, const polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_add(&w->vec[i], &u->vec[i], &v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_sub
*
* Description: Subtract vectors of polynomials of length K.
*              No modular reduction is performed.
*
* Arguments:   - polyveck *w: pointer to output vector
*              - const polyveck *u: pointer to first input vector
*              - const polyveck *v: pointer to second input vector to be
*                                   subtracted from first input vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_sub(polyveck *w, const polyveck *u, const polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_sub(&w->vec[i], &u->vec[i], &v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_shiftl
*
* Description: Multiply vector of polynomials of Length K by 2^D without modular
*              reduction. Assumes input coefficients to be less than 2^{31-D}.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_shiftl(polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_shiftl(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_ntt
*
* Description: Forward NTT of all polynomials in vector of length K. Output
*              coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_ntt(polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_ntt(&v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_invntt_tomont
*
* Description: Inverse NTT and multiplication by 2^{32} of polynomials
*              in vector of length K. Input coefficients need to be less
*              than 2*Q.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_invntt_tomont(polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_invntt_tomont(&v->vec[i]);
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyveck_pointwise_poly_montgomery(polyveck *r, const poly *a, const polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
    }
}


/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_chknorm
*
* Description: Check infinity norm of polynomials in vector of length K.
*              Assumes input polyveck to be reduced by PQCLEAN_DILITHIUM2_CLEAN_polyveck_reduce().
*
* Arguments:   - const polyveck *v: pointer to vector
*              - int32_t B: norm bound
*
* Returns 0 if norm of all polynomials are strictly smaller than B <= (Q-1)/8
* and 1 otherwise.
**************************************************/
int PQCLEAN_DILITHIUM2_CLEAN_polyveck_chknorm(const polyveck *v, int32_t bound) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        if (PQCLEAN_DILITHIUM2_CLEAN_poly_chknorm(&v->vec[i], bound)) {
            return 1;
        }
    }

    return 0;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_power2round
*
* Description: For all coefficients a of polynomials in vector of length K,
*              compute a0, a1 such that a mod^+ Q = a1*2^D + a0
*              with -2^{D-1} < a0 <= 2^{D-1}. Assumes coefficients to be
*              standard representatives.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients a0
*              - const polyveck *v: pointer to input vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_power2round(polyveck *v1, polyveck *v0, const polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_power2round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_decompose
*
* Description: For all coefficients a of polynomials in vector of length K,
*              compute high and low bits a0, a1 such a mod^+ Q = a1*ALPHA + a0
*              with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (Q-1)/ALPHA where we
*              set a1 = 0 and -ALPHA/2 <= a0 = a mod Q - Q < 0.
*              Assumes coefficients to be standard representatives.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients a0
*              - const polyveck *v: pointer to input vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_decompose(polyveck *v1, polyveck *v0, const polyveck *v) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_make_hint
*
* Description: Compute hint vector.
*
* Arguments:   - polyveck *h: pointer to output vector
*              - const polyveck *v0: pointer to low part of input vector
*              - const polyveck *v1: pointer to high part of input vector
*
* Returns number of 1 bits.
**************************************************/
unsigned int PQCLEAN_DILITHIUM2_CLEAN_polyveck_make_hint(polyveck *h,
        const polyveck *v0,
        const polyveck *v1) {
    unsigned int i, s = 0;

    for (i = 0; i < K; ++i) {
        s += PQCLEAN_DILITHIUM2_CLEAN_poly_make_hint(&h->vec[i], &v0->vec[i], &v1->vec[i]);
    }

    return s;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_polyveck_use_hint
*
* Description: Use hint vector to correct the high bits of input vector.
*
* Arguments:   - polyveck *w: pointer to output vector of polynomials with
*                             corrected high bits
*              - const polyveck *u: pointer to input vector
*              - const polyveck *h: pointer to input hint vector
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_polyveck_use_hint(polyveck *w, const polyveck *u, const polyveck *h) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_poly_use_hint(&w->vec[i], &u->vec[i], &h->vec[i]);
    }
}

void PQCLEAN_DILITHIUM2_CLEAN_polyveck_pack_w1(uint8_t r[K * POLYW1_PACKEDBYTES], const polyveck *w1) {
    unsigned int i;

    for (i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyw1_pack(&r[i * POLYW1_PACKEDBYTES], &w1->vec[i]);
    }
}
