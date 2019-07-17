/**
 *  additive_fft.c
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
#include "additive_fft.h"
#include "stack.h"
#include "bits.h"

/* Taylor expansion of a polynomial */
static poly** ff_poly_taylor_expansion(const FF2m* ff2m, const poly* f, int t, int* size)
{
    typedef struct item {
        poly *fx;
        int n;
    } item;
    stack *S = NULL;
    int32_t i, h_size, h_idx = 0, status = 0;
    uint32_t n_t, k;
    int32_t bound_f0, bound_f1, bound_f2;
    ff_unit *L0 = NULL, *L1 = NULL, *L2 = NULL, *h = NULL;
    poly *g0, *g1, **t_list;
    item *obj, *ptr;
    
    *size = ((f->degree + t) + (t-1))/t; /* ceil(n/t) */
    if (NULL == (t_list = (poly **)calloc(*size, sizeof(poly *))))
        goto taylor_fail;

    /* Prepare our stack */
    if (!(S = PQCLEAN_NTSKEM1264_CLEAN_stack_create()) || !(obj = (item *)malloc(sizeof(item))))
        goto taylor_fail;
    obj->fx = PQCLEAN_NTSKEM1264_CLEAN_clone_poly(f);
    obj->n = f->degree + t;
    PQCLEAN_NTSKEM1264_CLEAN_stack_push(S, obj);
    
    /* Allocate memory for L0, L1, L2 and h */
    if (!(L0 = (ff_unit *)calloc(obj->n, sizeof(ff_unit))) ||
        !(L1 = (ff_unit *)calloc(obj->n, sizeof(ff_unit))) ||
        !(L2 = (ff_unit *)calloc(obj->n, sizeof(ff_unit))) ||
        !(h  = (ff_unit *)calloc(obj->n, sizeof(ff_unit))))
        goto taylor_fail;
    
    /* Process the contents of our stack */
    while (PQCLEAN_NTSKEM1264_CLEAN_stack_size(S) > 0) {
        /* Pop the content of our stack */
        ptr = PQCLEAN_NTSKEM1264_CLEAN_stack_pop(S);
        
        /**
         * If the content of our stack is a polynomial of degree < t,
         * add it to our output array/list (t_list)
         **/
        if (ptr->n <= t) {
            t_list[h_idx++] = ptr->fx;
            free(ptr);
            continue;
        }
        
        /**
         * Obtain the value of k that satisfies t*2^k < n <= t*2^{k+1}
         **/
        n_t = ptr->n/t;
        k = (uint32_t)highest_bit_idx(n_t);
        if ((n_t >= 2) && CT_is_power_of_2(n_t)) { /* k is a power of 2 */
            --k;
        }

        /**
         * 1. Compute the bound of f_0(x) such that deg(f_0(x)) < bound
         *    and store the coefficients of fx within this bound in L0.
         *
         * We know that bound_0 is less than the size of L0, because
         * t*^2k < n
         **/
        bound_f0 = t*(1 << k);
        memcpy(L0, ptr->fx->coeff, bound_f0*sizeof(ff_unit));
        /**
         * 2. Compute the bound of f_1(x) such that deg(f_1(x)) < bound
         *    and store the coefficients of fx within this bound in L1.
         * 3. Compute the bound of f_2(x) such that deg(f_2(x)) < bound
         *    and store the coefficients of fx within this bound in L2
         *    (if f_2(x) exists).
         * 4. Pad L1 or L2 with zeros so that they are of equal length
         **/
        bound_f1 = CT_max(0, CT_min(ptr->fx->degree+1 - bound_f0, (t-1)*(1 << k)));
        bound_f2 = CT_max(0, (ptr->fx->degree+1) - (bound_f0 + bound_f1));
        h_size = CT_max(bound_f1, bound_f2);
        memset(L1, 0, h_size*sizeof(ff_unit));
        memset(L2, 0, h_size*sizeof(ff_unit));
        memcpy(L1, &ptr->fx->coeff[bound_f0], bound_f1*sizeof(ff_unit));
        memcpy(L2, &ptr->fx->coeff[bound_f0+bound_f1], bound_f2*sizeof(ff_unit));
        
        /**
         * h(x) = f_1(x) + f_2(x), the coefficients of f_i(x) are
         * stored in L_i for i={1,2}.
         **/
        memset(h, 0, h_size*sizeof(ff_unit));
        for (i=0; i<h_size; i++) {
            h[i] = ff2m->ff_add(ff2m, L1[i], L2[i]);
        }
        
        /**
         * Construct polynomial g_0(x) = f_0(x) + x^(2^k)*h(x)
         * where degree(g_0(x)) < t*2^k
         *
         * Additional t bytes are allocated in order to make sure
         * that the buffer containing the polynomial coefficients
         * will always be zero, even if the polynomial degree is
         * less than t.
         **/
        g0 = PQCLEAN_NTSKEM1264_CLEAN_init_poly(t*(1 << k) + t);
        memcpy(g0->coeff, L0, bound_f0*sizeof(ff_unit));
        
        for (i=0; i<h_size; i++) {
            g0->coeff[(1 << k) + i] = ff2m->ff_add(ff2m, g0->coeff[(1 << k) + i], h[i]);
        }
        PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree(g0); /* Update degree(g_0(x)) */
        
        /**
         * Construct polynomial g_1(x) = h(x) + x^((t-1)*2^k)*f_2(x)
         * where degree(g_1(x)) < n - t*2^k
         *
         * Additional t bytes are allocated in order to make sure
         * that the buffer containing the polynomial coefficients
         * will always be zero, even if the polynomial degree is
         * less than t.
         **/
        g1 = PQCLEAN_NTSKEM1264_CLEAN_init_poly(ptr->n - (t*(1 << k)) + t);
        memcpy(g1->coeff, h, h_size*sizeof(ff_unit));
        for (i=0; i<bound_f2; i++) {
            g1->coeff[(t-1)*(1 << k) + i] = ff2m->ff_add(ff2m, g1->coeff[(t-1)*(1 << k) + i], L2[i]);
        }
        PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree(g1); /* Update degree(g_1(x)) */
        
        /**
         * Add g_1(x) and g_0(x) to our stack. Note that g_1(x) must be added first
         **/
        if (!(obj = (item *)malloc(sizeof(item))))
            goto taylor_fail;
        obj->fx = g1;
        obj->n = ptr->n - (t*(1 << k));
        PQCLEAN_NTSKEM1264_CLEAN_stack_push(S, obj);
        if (!(obj = (item *)malloc(sizeof(item))))
            goto taylor_fail;
        obj->fx = g0;
        obj->n = t*(1 << k);
        PQCLEAN_NTSKEM1264_CLEAN_stack_push(S, obj);
        
        /**
         * Release memory of the item that we popped from stack earlier
         **/
        PQCLEAN_NTSKEM1264_CLEAN_zero_poly(ptr->fx);
        PQCLEAN_NTSKEM1264_CLEAN_free_poly(ptr->fx);
        free(ptr);
    }
    
    *size = h_idx;
    status = 1;
taylor_fail:
    if (!status) {
        if (t_list) {
            for (h_idx=0; h_idx<*size; h_idx++) {
                PQCLEAN_NTSKEM1264_CLEAN_free_poly(t_list[h_idx]); t_list[h_idx] = NULL;
            }
            free(t_list); t_list = NULL;
        }
        *size = 0;
    }
    if (L0) {
        memset(L0, 0, (f->degree + t)*sizeof(ff_unit));
        free(L0); L0 = NULL;
    }
    if (L1) {
        memset(L1, 0, (f->degree + t)*sizeof(ff_unit));
        free(L1); L1 = NULL;
    }
    if (L2) {
        memset(L2, 0, (f->degree + t)*sizeof(ff_unit));
        free(L2); L2 = NULL;
    }
    if (h) {
        memset(h,  0, (f->degree + t)*sizeof(ff_unit));
        free(h);  h  = NULL;
    }
    while (PQCLEAN_NTSKEM1264_CLEAN_stack_size(S) > 0) {
        obj = PQCLEAN_NTSKEM1264_CLEAN_stack_pop(S);
        if (obj) {
            PQCLEAN_NTSKEM1264_CLEAN_zero_poly(obj->fx);
            PQCLEAN_NTSKEM1264_CLEAN_free_poly(obj->fx);
            free(obj);
        }
    }
    PQCLEAN_NTSKEM1264_CLEAN_stack_free(S); S = NULL;
    
    return t_list;
}

static int _additive_fft_core(const FF2m* ff2m,
                       const poly* f,
                       const ff_unit* B,
                       int m,
                       poly** g0,
                       poly** g1)
{
    int i, h_size = 0;
    ff_unit b = 1;
    poly *g = NULL, **h = NULL;

    /**
     * Create a polynomial g(x) = f(B[m-1]*x), i.e.
     * each coefficient of g(x) is that of f(x)
     * multiplied by B[m-1].
     **/
    if (!(g = PQCLEAN_NTSKEM1264_CLEAN_clone_poly(f)))
        return 0;    
    for (i=1; i<=g->degree; i++) {
        b = ff2m->ff_mul(ff2m, b, B[m-1]);
        g->coeff[i] = ff2m->ff_mul(ff2m, g->coeff[i], b);
    }

    /**
     * Compute the taylor expansion of g
     **/
    if (!(h = ff_poly_taylor_expansion(ff2m, g, 2, &h_size)))
        return 0;

    /**
     * Construct polynomial g_0 and g_1
     **/
    if (!(*g0 = PQCLEAN_NTSKEM1264_CLEAN_init_poly((1 << (m-1)) + 2)) ||
        !(*g1 = PQCLEAN_NTSKEM1264_CLEAN_init_poly((1 << (m-1)) + 2)))
        return 0;
    
    for (i=0; i<h_size; i++) {
        (*g0)->coeff[i] = h[i]->coeff[0];
        (*g1)->coeff[i] = h[i]->coeff[1];
        PQCLEAN_NTSKEM1264_CLEAN_free_poly(h[i]);
    }
    PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree((*g0));
    PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree((*g1));
    
    PQCLEAN_NTSKEM1264_CLEAN_zero_poly(g);
    PQCLEAN_NTSKEM1264_CLEAN_free_poly(g);
    free(h);
    
    return 1;
}

static int _additive_fft_construct_basis(const FF2m* ff2m,
                                  const ff_unit* B,
                                  int m,
                                  ff_unit** G,
                                  ff_unit** D)
{
    int i;
    ff_unit b_inv;
    
    *G = (ff_unit *)calloc(m-1, sizeof(ff_unit));
    *D = (ff_unit *)calloc(m-1, sizeof(ff_unit));
    if (!*G || !*D)
        return 0;

    b_inv = ff2m->ff_inv(ff2m, B[m-1]);
    for (i=0; i<m-1; i++) {
        /* G_i = B_i/B_{m-1} */
        (*G)[i] = ff2m->ff_mul(ff2m, B[i], b_inv);
        /* D_i = G_i^2 - G_i */
        (*D)[i] = ff2m->ff_add(ff2m,
                             ff2m->ff_sqr(ff2m, (*G)[i]),
                             (*G)[i]);
    }
    
    return 1;
}

static ff_unit _additive_fft_ff_value(const FF2m* ff2m,
                               int index,
                               int m,
                               ff_unit *B)
{
    int i = 0;
    ff_unit v = 0;
    
    for (i=0; i<m; i++) {
        v = ff2m->ff_add(ff2m, v, ((index >> i) & 1) * B[i]);
    }
    
    return v;
}

ff_unit* PQCLEAN_NTSKEM1264_CLEAN_additive_fft(const FF2m* ff2m, const poly *f)
{
    typedef struct {
        poly *fx;
        int m;
    } item;
    int i, j, s, idx=0, status = 0;
    poly *g0, *g1;
    stack *S = NULL;
    item *obj = NULL, *ptr = NULL;
    ff_unit **G = NULL, **D = NULL;
    ff_unit Gi, *w = NULL;
    
    /**
     * Construct bases G and D for m=1,..,m-1
     **/
    if (!(G = (ff_unit **)malloc(ff2m->m*sizeof(ff_unit*))) ||
        !(D = (ff_unit **)malloc(ff2m->m*sizeof(ff_unit*))))
        goto fft_fail;
    G[ff2m->m-1] = ff2m->basis; D[ff2m->m-1] = ff2m->basis;
    for (i=0; i<ff2m->m-1; i++) {
        if (!_additive_fft_construct_basis(ff2m,
                                           D[ff2m->m-1-i],
                                           ff2m->m-i,
                                           &G[ff2m->m-2-i],
                                           &D[ff2m->m-2-i]))
            goto fft_fail;
    }

    if (!(w = (ff_unit *)calloc((1 << ff2m->m), sizeof(ff_unit))))
        goto fft_fail;
    
    /* Prepare our stack */
    if (!(S = PQCLEAN_NTSKEM1264_CLEAN_stack_create()) || !(obj = (item *)malloc(sizeof(item))))
        goto fft_fail;
    obj->fx = PQCLEAN_NTSKEM1264_CLEAN_clone_poly(f);
    obj->m = ff2m->m;
    PQCLEAN_NTSKEM1264_CLEAN_stack_push(S, obj);
    
    /* Traverse the FFT tree in depth-first manner and accumulate values in vector v */
    while (PQCLEAN_NTSKEM1264_CLEAN_stack_size(S) > 0) {
        /* Pop the content of our stack */
        ptr = PQCLEAN_NTSKEM1264_CLEAN_stack_pop(S);

        if (ptr->m == 1 || ptr->fx->degree <= 0) {
            /**
             * We are only interested in a polynomial of degree <= 0
             * (regardless of the level m) or whatever the polynomial
             * is at level m equals to 1. By construction, at level
             * m=1, the polynomial cannot have degree > 1.
             **/
            s = (1 << ptr->m);
            memset(&w[idx], 0, s*sizeof(ff_unit));
            w[idx]   = ptr->fx->coeff[0];
            w[idx+1] = ptr->fx->coeff[0] + ff2m->ff_mul(ff2m, ptr->fx->coeff[1], D[0][0]);
            idx += s;
            PQCLEAN_NTSKEM1264_CLEAN_zero_poly(ptr->fx);
            PQCLEAN_NTSKEM1264_CLEAN_free_poly(ptr->fx);
            free(ptr);
            continue;
        }
        
        if (!_additive_fft_core(ff2m, ptr->fx, D[ptr->m-1], ptr->m, &g0, &g1))
            goto fft_fail;
        if (!(obj = (item *)malloc(sizeof(item))))
            goto fft_fail;
        obj->fx = g1;
        obj->m = ptr->m-1;
        PQCLEAN_NTSKEM1264_CLEAN_stack_push(S, obj);

        if (!(obj = (item *)malloc(sizeof(item))))
            goto fft_fail;
        obj->fx = g0;
        obj->m = ptr->m-1;
        PQCLEAN_NTSKEM1264_CLEAN_stack_push(S, obj);
        
        /**
         * Release memory of the item that we popped from stack earlier
         **/
        PQCLEAN_NTSKEM1264_CLEAN_zero_poly(ptr->fx);
        PQCLEAN_NTSKEM1264_CLEAN_free_poly(ptr->fx);
        free(ptr);
    }

    /**
     * Combine the values in v by multipying them with twiddle factors
     * that are obtained from the bases of G
     **/
    for (s=1; s<ff2m->m; s++) {
        for (i=0; i<(1 << ff2m->m); i+=(1 << (s+1))) {
            for (j=i; j<i+(1 << s); j++) {
                Gi = _additive_fft_ff_value(ff2m, j-i, s, G[s-1]);
                w[j] = ff2m->ff_add(ff2m, w[j],
                                    ff2m->ff_mul(ff2m, Gi, w[(1<<s) + j]));
                w[(1<<s) + j] = ff2m->ff_add(ff2m, w[(1<<s) + j], w[j]);
            }
        }
    }

    status = 1;
fft_fail:
    if (!status) {
        memset(w, 0, (1 << ff2m->m)*sizeof(ff_unit));
        free(w); w = NULL;
    }
    if (G && D) {
        for (i=0; i<ff2m->m-1; i++) {
            if (G[i]) {
                memset(G[i], 0, (i+1)*sizeof(ff_unit));
                free(G[i]);
            }
            if (D[i]) {
                memset(D[i], 0, (i+1)*sizeof(ff_unit));
                free(D[i]);
            }
        }
        free(G);
        free(D);
    }
    while (PQCLEAN_NTSKEM1264_CLEAN_stack_size(S) > 0) {
        obj = PQCLEAN_NTSKEM1264_CLEAN_stack_pop(S);
        if (obj) {
            PQCLEAN_NTSKEM1264_CLEAN_zero_poly(obj->fx);
            PQCLEAN_NTSKEM1264_CLEAN_free_poly(obj->fx);
            free(obj);
        }
    }
    PQCLEAN_NTSKEM1264_CLEAN_stack_free(S); S = NULL;
    
    return w;
}
