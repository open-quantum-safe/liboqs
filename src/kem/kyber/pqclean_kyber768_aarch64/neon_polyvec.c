#include "NTT_params.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "reduce.h"
#include <arm_neon.h>

#define _V (((1U << 26) + KYBER_Q / 2) / KYBER_Q)

/*************************************************
* Name:        neon_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void neon_polyvec_ntt(int16_t r[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        neon_poly_ntt(r[i]);
    }
}

/*************************************************
* Name:        neon_polyvec_invntt_to_mont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void neon_polyvec_invntt_to_mont(int16_t r[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        neon_poly_invntt_tomont(r[i]);
    }
}

/*************************************************
* Name:        neon_polyvec_add_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments: - polyvec *r:       pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void neon_polyvec_add_reduce(int16_t c[KYBER_K][KYBER_N], int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        // c = c + a;
        // c = reduce(c);
        neon_poly_add_reduce(c[i], a[i]);
    }
}

