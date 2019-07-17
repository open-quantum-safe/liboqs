/**
 *  polynomial.c
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polynomial.h"
#include "bits.h"
#include "randombytes.h"
#include "additive_fft.h"

#define COPY_POLY(a, b)  (b)->degree = (a)->degree;\
                         memcpy((b)->coeff, (a)->coeff, (a)->size*sizeof(ff_unit))

poly* PQCLEAN_NTSKEM1264_CLEAN_init_poly(int size)
{
    poly* px = (poly *)malloc(sizeof(poly));
    if (!px)
        return NULL;
    
    px->size = size;
    px->degree = -1; /* Indicate a zero polynomial */
    px->coeff = (ff_unit *)calloc(size, sizeof(ff_unit));
    if (!px->coeff)
        return NULL;
    
    return px;
}

void PQCLEAN_NTSKEM1264_CLEAN_free_poly(poly* px)
{
    if (px) {
        if (px->coeff)
            free(px->coeff);
        px->coeff = NULL;
        px->degree = -1;
        px->size = 0;
        free(px);
    }
}

void PQCLEAN_NTSKEM1264_CLEAN_zero_poly(poly* px)
{
    memset(px->coeff, 0, px->size*sizeof(ff_unit));
}

poly* PQCLEAN_NTSKEM1264_CLEAN_clone_poly(const poly *px)
{
    poly *qx = PQCLEAN_NTSKEM1264_CLEAN_init_poly(px->size);
    if (!qx)
        return NULL;
    
    memcpy(qx->coeff, px->coeff, px->size*sizeof(ff_unit));
    qx->degree = px->degree;
    
    return qx;
}

void PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree(poly *px)
{
    px->degree = px->size-1;
    while (px->degree > 0 && !px->coeff[px->degree]) --px->degree;
}

int PQCLEAN_NTSKEM1264_CLEAN_formal_derivative_poly(const poly* fx, poly *dx)
{
    int i;
    
    /* Make sure that d(x) has enough buffer space */
    if (dx->size < fx->size-1)
        return 0;
    
    for (i=0; i<fx->degree; ++i)
    {
        dx->coeff[i] = 0;
        if ((i & 1) == 0)
            dx->coeff[i] = fx->coeff[i+1];
    }
    dx->degree = fx->degree-1;
    for (i=0; i<fx->degree; ++i)
    {
        /* in case coefficients are zero */
        if (dx->coeff[fx->degree-1-i])
            break;
        --dx->degree;
    }
    
    return 1;
}

static void modulo_reduce_poly(const FF2m* ff2m, const poly *mx, poly *ax)
{
    int i, j;
    ff_unit a;
    
    while (ax->degree >= mx->degree) {
        a = ff2m->ff_mul(ff2m, ax->coeff[ax->degree],
                         ff2m->ff_inv(ff2m, mx->coeff[mx->degree]));
        j = ax->degree - mx->degree;
        for (i=0; i<mx->degree; ++i,++j) {
            if (mx->coeff[i])
                ax->coeff[j] = ff2m->ff_add(ff2m, ax->coeff[j],
                                            ff2m->ff_mul(ff2m, mx->coeff[i], a));
        }
        ax->coeff[j] = 0;
        while ((ax->degree >= 0) && !ax->coeff[ax->degree])
            --ax->degree;
    }
}

int PQCLEAN_NTSKEM1264_CLEAN_gcd_poly(const FF2m* ff2m, const poly* ax, const poly *bx, poly *gx)
{
    poly *sx, *tx;
    
    sx = PQCLEAN_NTSKEM1264_CLEAN_clone_poly(ax);
    tx = PQCLEAN_NTSKEM1264_CLEAN_clone_poly(bx);
    if (!sx || !tx)
        return 0;
    
    while (tx->degree >= 0) {
        /* g(x) = s(x) */
        COPY_POLY(sx, gx);
        
        /* g(x) = g(x) mod t(x) */
        modulo_reduce_poly(ff2m, tx, gx);
        
        /* s(x) = t(x) */
        COPY_POLY(tx, sx);
        
        /* t(x) = g(x) */
        COPY_POLY(gx, tx);
    }
    COPY_POLY(sx, gx);
    
    PQCLEAN_NTSKEM1264_CLEAN_free_poly(sx);
    PQCLEAN_NTSKEM1264_CLEAN_free_poly(tx);
    
    return 1;
}
