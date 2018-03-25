#ifndef GOPPA_H
#define GOPPA_H
#define GOPPA_NO_PRECOMP

#include "gf.h"
#include "poly.h"
#include "permutation.h"

#ifdef GOPPA_NO_PRECOMP
#define SQRT_NO_PRECOMP
#define PARITY_NO_PRECOMP
#endif



typedef struct goppa {
    /* the finite field is implicit (defined by gf) and is an extension
       of degree m of GF(2), we must have length <= 2^m */
    int length, degree, order; // order = order of the permutation acting on the Goppa code
    poly_t g; // generator of degree "degree"
    gfelt_t * L; // support of size "length"
    /* (L[i] = a) <=> (Linv[a] = i), for finding the error positions
       corresponding to a particular field element */
    gfindex_t * Linv;
    /* z^(1/2) mod g(z) = z^(2^(m-1)) mod g(z), for use in Paterson
       decoding algorithm */
    poly_t sqrtzmod;
#ifndef SQRT_NO_PRECOMP
    poly_t * sqrtmod;
#endif
#ifndef PARITY_NO_PRECOMP
    /* the "parity" table possibly contains data used for computing
       syndromes before decoding */
    unsigned long ** parity;
#endif
} * goppa_t;

// goppa.c
#define coeff(M, i, j) (M[i][(j) / __WORDSIZE] >> ((j) % __WORDSIZE) & 1)
#define addrowto(M, i, l) xor_long(M[l], M[i], rwdcnt)
#define swaprows(M, i, k) {unsigned long * pt = M[i]; M[i] = M[k]; M[k] = pt;}

goppa_t goppa_init(gfelt_t * L, poly_t g, int length, int degree, int order, poly_t sqrtzmod);
void free_goppa(goppa_t goppa);
int goppa_check_params(int n, int m, int l, int t);
gfelt_t * goppa_rand_support(int n, int l);
int gausselim(unsigned long ** M, int r, int c);
int gausselim_aux(unsigned long ** M, int r, int c, int ind_cols);
int gausselim_perm(unsigned long ** M, int r, int c, int * perm);
int qc_systematic_form(unsigned long ** M, int r, int n, int l, int * perm);
goppa_t goppa_keygen_rand(int n, int l, int m, int t, unsigned char ** pk);

// decode.c
void xor_long(unsigned long * a, unsigned long * b, int n);
int goppa_decode(const unsigned char * s, int * e, goppa_t gamma);

// roots.c
void roots_init();
void roots_clear();
int roots_berl(poly_t sigma, gfelt_t * res);

#endif // GOPPA_H
