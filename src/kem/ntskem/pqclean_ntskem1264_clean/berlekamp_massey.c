/**
 *  berlekamp_massey.c
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "berlekamp_massey.h"
#include "bits.h"

/*
 * Berlekamp-Massey algorithm
 *
 * Question: How do we know if there is an error
 *           at the extended coordinate of an
 *           extended code?
 * Answer:   A run of R zero discrepancies at the
 *           end of decoding process indicates
 *           that there are t-(R/2) errors located.
 *           The Berlekamp-Massey algorithm produces
 *           an error-locating polynomial a(x) and
 *           if deg(a(x)) < t-(R/2), then there is
 *           an error at the extended coordinate.
 */
poly *PQCLEAN_NTSKEM1264_CLEAN_berlekamp_massey(const FF2m *ff2m,
                       const ff_unit *S,
                       int slen,
                       int *extended_error)
{
    poly *ex = NULL;
    ff_unit *sigma = NULL, *beta = NULL, *varphi = NULL;
    ff_unit *src0_ptr = NULL, *src1_ptr = NULL, *dst_ptr = NULL;
    ff_unit d, delta = 1;
    ff_unit inv = 0;
    uint32_t control, d_eq_0;
    int32_t i, j, v, t, L = 0, R = 0;
    
    t = slen >> 1;
    sigma  = (ff_unit *)calloc(t+1, sizeof(ff_unit));
    beta   = (ff_unit *)calloc(t+1, sizeof(ff_unit));
    varphi = (ff_unit *)calloc(t+1, sizeof(ff_unit));
    if (!sigma || !beta || !varphi) {
        goto BMA_fail;
    }
    sigma[0] = 1;   /* sigma(x) = 1 */
    beta[1] = 1;    /* beta(x) = x */
    *extended_error = 0;
    
    /* Loop until we process all 2t syndromes */
    for (i=0; i<slen; i++) {
        /**
         * d = \sum_{i}^{\min{i, t}} sigma_j * S_{i-j}
         **/
        v = CT_min(i, t);
        for (d=0,j=0; j<=v; j++) {
            d = ff2m->ff_add(ff2m, d,
                             ff2m->ff_mul(ff2m, sigma[j], S[i-j]));
        }
        /**
         * varphi(x) = delta.sigma(x) - d.beta(x)
         **/
        for (j=0; j<=t; j++) {
            varphi[j] = ff2m->ff_add(ff2m,
                                     ff2m->ff_mul(ff2m, delta, sigma[j]),
                                     ff2m->ff_mul(ff2m, d, beta[j]));
        }

        d_eq_0 = CT_is_equal_zero((uint32_t)d);   /* d == 0? */
        control = d_eq_0 || CT_is_less_than(i, (L << 1)); /* (d == 0) OR (i < 2L) */
        
        /**
         * if control is 1 -> beta(x) = x.beta(x)
         * otherwise       -> beta(x) = x.sigma(x)
         **/
        v = t;
        src0_ptr = (ff_unit *)&sigma[t-1];
        src1_ptr = (ff_unit *)&beta[t-1];
        dst_ptr = (ff_unit *)&beta[t];
        while (v-- > 0) {
            *dst_ptr = CT_mux(control, *src1_ptr, *src0_ptr);
            --dst_ptr;
            --src1_ptr;
            --src0_ptr;
        }
        beta[0] = 0x00;
        
        /**
         * if control is 1 -> 
         *     R = R + 1 if d == 0
         * otherwise       -> 
         *     R = 0
         *     L = i - L + 1
         *     delta = d
         **/
        L = (int32_t)CT_mux(control, L, i-L+1);
        R = (int32_t)CT_mux(control, R + d_eq_0, 0);
        delta = (ff_unit)CT_mux(control, delta, d);
        
        memcpy(sigma, varphi, (t+1)*sizeof(ff_unit));
    }
    
    ex = PQCLEAN_NTSKEM1264_CLEAN_init_poly(t+1);
    if (!ex) {
        goto BMA_fail;
    }
    ex->degree = t;
    while (ex->degree > 0 && !sigma[ex->degree]) --ex->degree;
    inv = ff2m->ff_inv(ff2m, sigma[0]);
    for (i=0; i<=ex->degree; i++) {
        ex->coeff[i] = ff2m->ff_mul(ff2m, sigma[ex->degree-i], inv); /* Reverse the returned polynomial */
    }
    *extended_error = CT_is_less_than(ex->degree, t - (R>>1));
    
BMA_fail:
    if (varphi) {
        memset(varphi, 0, (t+1)*sizeof(ff_unit));
        free(varphi);
    }
    if (beta) {
        memset(beta,  0, (t+1)*sizeof(ff_unit));
        free(beta);
    }
    if (sigma) {
        memset(sigma, 0, (t+1)*sizeof(ff_unit));
        free(sigma);
    }
    
    return ex;
}
