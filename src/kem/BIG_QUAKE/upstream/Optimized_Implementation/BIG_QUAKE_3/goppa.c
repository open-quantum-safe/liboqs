#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "goppa.h"

// if sqrtzmod != NULL it is equal to sqrt(z) mod g(z)

/**
    Initialize a goppa_t structure from the given parameters.
*/
goppa_t goppa_init(gfelt_t * L, poly_t g, int length, int degree, int order, poly_t sqrtzmod) {
    int i;
    goppa_t gamma = malloc(sizeof (struct goppa));

    gamma->length = length;
    gamma->degree = degree;
    gamma->order = order;
    gamma->L = L;
    gamma->g = g;
    gamma->Linv = (gfindex_t *) calloc(gf_card(), sizeof (gfindex_t));
    for (i = 0; i < length; ++i)
        gamma->Linv[gf_to_index(L + i)] = i;
    gamma->sqrtzmod = sqrtzmod;
    
#ifndef SQRT_NO_PRECOMP
    gamma->sqrtmod = NULL;
#endif
#ifndef PARITY_NO_PRECOMP
    gamma->parity = NULL;
#endif
    
    return gamma;
}


/**
   Free memory allocated by goppa_t.
 */
void free_goppa(goppa_t gamma) {
    if (gamma->L)
        free(gamma->L);
    if (gamma->g)
        poly_free(gamma->g);
    if (gamma->Linv)
        free(gamma->Linv);
    if (gamma->sqrtzmod)
        poly_free(gamma->sqrtzmod);
#ifndef SQRT_NO_PRECOMP
    if (gamma->sqrtmod) {     
        for (int i = 0; i < gamma->degree / 2; ++i)
            poly_free(gamma->sqrtmod[i]);
        free(gamma->sqrtmod);
    }
#endif
#ifndef PARITY_NO_PRECOMP
    if (gamma->parity) {
        free(gamma->parity[0]);
        free(gamma->parity);
    }
#endif
    free(gamma);    
}


/**
   Computes a random support of size n for an l-quasi-cyclic Goppa code.
   We assume that n <= 2^m.
 */
gfelt_t * goppa_rand_support(int n, int l) {

    int i, j, k, s, nbr_o = n/l;
    gf_t a;
    gfelt_t *L;
    int tab[gf_card()-1];
    index_t * perm_orb = malloc(nbr_o*sizeof(index_t));
    
    L = malloc(n * sizeof(gfelt_t));
    
    for (i = 0; i < (gf_card()-1)/l; i++){
        for (k = 0; k < l; k++){
            tab[i*l+k] = (i + k*(gf_card()-1)/l);
        }
    }
    
    generate_permutation(perm_orb, (index_t) nbr_o);
    
    for (k = 0; k < nbr_o; k++) {
        i = (int) perm_orb[k];
        randindex_init(l);
        j = (int) uniform((index_t)l);

        for(s = 0; s < l; s++) {
            a[0] = gf_exp[tab[i*l + ((j + s)%l)]];
            gf_set(L + k*l + s, a);
        }
    }
    
    free(perm_orb);
    
    return L;
}


void xor_long(unsigned long * a, unsigned long * b, int n) {
    int i;
    for (i = 0; i < n; ++i)
	a[i] ^= b[i];
}


/**
   In matrix M, swaps the l consecutive rows beginning at row i,
   with the l consecutive rows beginning at row k.
*/
void swap_block_rows(unsigned long ** M, int i, int k, int l) {
    int u;
    for(u = 0; u < l; u++)
        swaprows(M, i + u, k + u);
}




/**
   Performs a gaussian elimination on M of size (r x c), and outputs its rank.

   The elimination allows swaps of columns, but these swaps are not effective
   in the sense that the permutation of column is stored in *perm.
*/
/*
int gausselim_perm(unsigned long ** M, int r, int c, int * perm) {
    int i, j, k, l, s;
    int rwdcnt = 1 + (c - 1) / __WORDSIZE;
    int * nonpivot = NULL, * pivot = NULL;
    
    // pivots are stored in the last indices of perm, and non-pivots in the first ones 
    if (perm) {
        pivot = perm + (c - r);
        nonpivot = perm;
    }

    for (j = 0, k = 0, s = 0; (k < r) && (j < c); ++j) {
        // we search for a pivot
        for (i = k; i < r; ++i) {
            if (coeff(M, i, j)) {
                swaprows(M, i, k);
                break;
            }
        }
        if (i < r) {      // found
            if (perm)
                pivot[k] = j;
            for (l = 0; l < k; ++l)
                if (coeff(M, l, j))
                    addrowto(M, k, l);
            for (l = i + 1; l < r; ++l)
                if (coeff(M, l, j))
                    addrowto(M, k, l);
            ++k;
        } else {          // not found
            if (perm)
                nonpivot[s] = j;
            ++s;
        }
    }
    if (perm)
        for (; j < c; ++j, ++s)
            nonpivot[s] = j;
    
    return k;
}*/

/**
   Performs a gaussian elimination on M of size (r x c), and outputs its rank.
*/
int gausselim(unsigned long ** M, int r, int c) {
    int i, j, k, l, s;
    int rwdcnt = 1 + (c - 1) / __WORDSIZE;

    for (j = 0, k = 0, s = 0; (k < r) && (j < c); ++j) {
        // we search for a pivot
        for (i = k; i < r; ++i) {
            if (coeff(M, i, j)) {
                swaprows(M, i, k);
                break;
            }
        }
        if (i < r) {      // found
            for (l = 0; l < k; ++l)
                if (coeff(M, l, j))
                    addrowto(M, k, l);
            for (l = i + 1; l < r; ++l)
                if (coeff(M, l, j))
                    addrowto(M, k, l);
            ++k;
        } else {          // not found
            ++s;
        }
    }
    
    return k;
}


int gausselim_aux(unsigned long ** M, int r, int c, int ind_cols) {
    int i, j, k, l, s;
    int rwdcnt = 1 + (c - 1) / __WORDSIZE;

    for (j = ind_cols, k = 0, s = 0; (k < r) && (j < c); ++j) {
        // we search for a pivot
        for (i = k; i < r; ++i) {
            if (coeff(M, i, j)) {
                swaprows(M, i, k);
                break;
            }
        }
        if (i < r) {      // found
            for (l = 0; l < k; ++l)
                if (coeff(M, l, j))
                    addrowto(M, k, l);
            for (l = i + 1; l < r; ++l)
                if (coeff(M, l, j))
                    addrowto(M, k, l);
            ++k;
        } else {          // not found
            ++s;
        }
    }
    
    return k;
}



/**
   Computes the rank of the lxl submatrix of M whose top-left element is in 
   position (i,j).
   Matrix M is not modified.
*/ 
int rank_block(unsigned long ** M, int i, int j, int l) {

    int k, s, rk;
    unsigned long ** Bl;
    
    // Bl is a copy of the lxl submatrix of M whose rank must be computed
    Bl = malloc(l * sizeof (unsigned long *));
    for (k = 0; k < l; k++) {
        Bl[k] = calloc(1 + (l - 1) / __WORDSIZE, sizeof (unsigned long));
        for (s = 0; s < l; s++) {
            if (coeff(M, i + k, j + s)) {
                Bl[k][s / __WORDSIZE] ^= (1LU << (s % __WORDSIZE));
            }
        }
    }
    
    rk = gausselim(Bl, l, l);
    for (k = 0; k < l; k++)
        free(Bl[k]);
    free(Bl);
    
    return(rk);
}


/**
   Changes M into its quasi-cyclic systematic form.

   Returns 0 if matrix M does not admit such a form
           the rank of M otherwise
*/
int qc_systematic_form(unsigned long ** M, int r, int n, int l, int * perm) {
    
    int i, j, k, s, block_inv, ind_cols, rk, tmp;
    unsigned long ** M_bis;
    int rwdcnt = 1 + (n - 1) / __WORDSIZE; //for addrowto
    int * sigma;
    
    /* sigma = permutation of the quasi-cyclic goppa code */
    sigma = malloc(n * sizeof (int));    
    for (i=0; i < n/l; i ++) {
        for (j=0; j < l-1; j++){
            sigma[i*l + j] = i*l + j + 1 ;            
        }
        sigma[i*l+(l-1)] = i*l;
    }
    
    /* M_bis is a temporary (r*l x n) circulant matrix build from M, by stacking 
       permutations of rows of M by sigma*/
    M_bis = malloc(r * l * sizeof (unsigned long *));
    for (i = 0; i < r*l; i++) {
        M_bis[i] = calloc(1 + (n - 1) / __WORDSIZE, sizeof (unsigned long));
    }
    
    for (i = 0; i < r; i++) {
        
        for (j = 0; j < n; j++)
            if (coeff(M, i, j)) 
                M_bis[i * l][j / __WORDSIZE] ^= (1LU << (j % __WORDSIZE)) ;
        
    for (k = 0; k < l-1; k++)
        for (j = 0; j < n; j++)
                if (coeff(M_bis, i*l + k, j))
                    M_bis[i * l + k + 1 ][sigma[j] / __WORDSIZE] ^= (1LU << (sigma[j] % __WORDSIZE)) ;
            
    }
    
    /* We set the initial permutation of columns (needed for the qc-elimination)
       to identity */
    for (i = 0; i < n; i++)
        perm[i] = i;

    ind_cols = 0;
    
    for (j = 0; j < r ; j += l) {

        /* (block_inv == TRUE) iff an inversible block was found for the step j */        
        block_inv = FALSE; 
        
        while (!block_inv && ind_cols < n) { 

           /* From line j, search for an (lxl) invertible submatrix whose column 
                  indices are [ind_cols..ind_cols + l-1]. */            
            rk = 0;
            i = j;
            while (rk != l && i < r*l) {
                rk = rank_block(M_bis, i, ind_cols, l);
                i += l;
            }
            
            if (rk == l) {            // found
                                
                block_inv = TRUE;

                if (j != (i-l))
                    swap_block_rows(M_bis, i - l, j, l);

                /* Set to identity the inversible block found previously */
                gausselim_aux(M_bis + j, l, n, ind_cols);
                
                /* Set other coefficients to zero */
                for (k = 0; k < j; k++) 
                    for (s = 0; s < l; s++)
                        if (coeff(M_bis, k, ind_cols + s)) 
                            addrowto(M_bis, j + s, k); 

                for (k = j + l; k < r*l; k++)
                    for (s = 0; s < l; s++) 
                        if (coeff(M_bis, k, ind_cols + s))
                            addrowto(M_bis, j + s, k);
                       
                /* Update the column's permutation */
                if (ind_cols != j)
                    for (k = 0; k < l; k++) {
                        tmp = perm[j + k];
                        perm[j + k] = perm[ind_cols + k];
                        perm[ind_cols + k] = tmp;
                    }
            }                     // not found
            ind_cols += l;
        }
    }
    /*  Copy the r first rows of M_bis into M */
    for (i = 0; i < r; i++) {
        for (k = 0; k < rwdcnt; k++)
            M[i][k] = M_bis[i][k];
    }
    for (i = 0; i < r*l; i++)
        free(M_bis[i]);
    
    free(M_bis);
    free(sigma);
    

    if (ind_cols >= n){
        /* Inversible block not found for all column's blocks: algorithm failed */
        return 0;
    }

    return j;
}




/* Assumes pk (the compact representation of the public key) is allocated */
int goppa_keygen(gfelt_t * L, poly_t g, int n, int l, int t, unsigned char * pk) {
    //L- Support
    //t- Number of errors
    //n- Length of the Goppa code
    //m- The extension degree of the GF
    //g- The generator polynomial
    
    
    int i, j, k, r;
    int * perm;
    gfelt_t * Laux;
    gf_t a;
    gfindex_t x;
    unsigned long ** H;
    
    r = t * gf_extd(); // r = codimension 

    /* Constrution of a parity check matrix H for the Goppa code Gamma(L,g) */

    H = malloc(r * sizeof (unsigned long *));
    for (i = 0; i < r; i++) 
        H[i] = calloc(1 + (n - 1) / __WORDSIZE, sizeof (unsigned long));
    
    for (i = 0; i < n; i++) {
        poly_eval(a, g, L + i);
        gf_inv(a, a);
        for (j = 0; j < t; j++) {
            x = gf_to_index(a);
            for (k = 0; k < gf_extd(); k++) {
                if (x & (1 << k)) {
                    H[j * gf_extd() + k][i / __WORDSIZE] ^= (((unsigned long) 1) << (i % __WORDSIZE));
                }
            }
            gf_mul(a, a, L + i);
        }
    }

    /* Quasi_cyclic systematic form */
    perm = malloc(n * sizeof (int));
    gausselim(H, r, n);
    k = qc_systematic_form(H, r, n, l, perm); // k = rank(H)
    
    if (k < r) {
        for (i = 0; i < r; ++i)
            free(H[i]);
        free(H);
        free(perm);
        return k;
    }
    
    /* qc_systematic_form may need a permutation "perm" to allow the 
    systematic form, we make sure to modify the support "L" accordingly */
    Laux = malloc(n * sizeof (gfelt_t));
    for (i = 0; i < n; ++i) {
        Laux[(i + n - r) % n] = L[perm[i]];
    }
    for (i = 0; i < n; ++i) {
        L[i] = Laux[i];
    }
    
    /* Save a compact form of H in pk*/
    k = (r - 1) / 8 + 1;
    for (j = 0; j < n-r; j+=l) {
        for (i = 0; i < r; i++) {
            if (coeff(H,i,perm[j + r])){
                pk[(j/l) * k + (i / 8)] ^= (1 << (i % 8));
            }
        }
    }
    
    for (i = 0; i < r; ++i)
        free(H[i]);
    free(H);
    free(Laux);
    free(perm);
    
    return r;
}


/**
   Checks whether n, m, l, t are admissible parameters for an
   l-quasi-cyclic Goppa code 
*/
int goppa_check_params(int n, int m, int l, int t) {
    return ( (n > 0) && (m > 0) && (t > 0) &&
         (n <= (1 << m)) && (t * m < n) &&
         (m == gf_init(m)) &&
         ((gf_card() - 1) % l == 0) && (n % l == 0) );
}



goppa_t goppa_keygen_rand(int n, int l, int m, int t, unsigned char ** pk) {

    // first, we check if parameters are admissible
    if (! goppa_check_params(n, m, l, t))
        return NULL;

    // we pick at random a support of length n
    gfelt_t * L = goppa_rand_support(n, l);
    
    if (L == NULL)
        return NULL;

    // we pick at random an irreducible polynomial of degree t/l, and
    // we compose it by X^l to define the Goppa polynomial

		poly_t g = NULL;
    if (l != 1) {
			int smallestFactorDegree=0;
			gf_t u;
			gf_set_to_unit(u);
        
			poly_t xl = poly_alloc(l);
			poly_set_deg(xl, l);
			poly_set_coeff(xl, l, u);
			while (smallestFactorDegree <t){
				poly_t aux_poly = poly_randgen_irred(t/l);
				g = poly_compose(aux_poly, xl);
				poly_free(aux_poly);
				smallestFactorDegree=poly_degppf(g);
				if (smallestFactorDegree < t)
					poly_free(g);
			}
			poly_free(xl);
    } else {
			g = poly_randgen_irred(t);
    }    

		/*
    poly_t g;
    if (l != 1) {
        gf_t u;
        gf_set_to_unit(u);
        
        poly_t xl = poly_alloc(l);
        poly_set_deg(xl, l);
        poly_set_coeff(xl, l, u);
        poly_t aux_poly = poly_randgen_irred(t/l);
        g = poly_compose(aux_poly, xl);
    
        poly_free(aux_poly);
        poly_free(xl);
    } else {
        g = poly_randgen_irred(t);
    }
		*/
    if (g == NULL) {
        free(L);
        return NULL;
    }
    
    // we allocate space for storing the public key
    int alloc_pk = ((pk) && (*pk == NULL));
    if (alloc_pk) {
        size_t s = (((m*t) - 1) / (8 * sizeof (long)) + 1) * sizeof (long);
        *pk = malloc(s * ((1 << m) - m * t));
        if (*pk == NULL) {
            free(L);
            poly_free(g);
            return NULL;
        }
    }
    
    if (pk) {
        if (goppa_keygen(L, g, n, l, t, *pk) != m * t) {
            free(L);
            poly_free(g);
            if (alloc_pk) {
                free(*pk);
            }
            *pk = NULL;
            return NULL;
        }
    }
    
    goppa_t gamma = goppa_init(L, g, n, t, l, NULL);
    if (gamma == NULL) {
        free(L);
        poly_free(g);
        if (pk) {
            if (alloc_pk) {
                free(*pk);
            }
            *pk = NULL;
        }
        return NULL;
    }
    
    return gamma;
}

