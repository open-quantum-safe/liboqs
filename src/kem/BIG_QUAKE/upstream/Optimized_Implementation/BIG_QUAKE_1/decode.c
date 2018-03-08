#include <stdlib.h>
#include <string.h>
#include "goppa.h"

/* computes (and store in f) the polynomial f(z)=1/(z-b) mod g(z),
	 note that g(b)f(z)=(g(z)-g(b))/(z-b) */
void poly_syndrome_patterson(poly_t f, gf_t b, poly_t g) {
    int i, t;
    gf_t a, c;

    t = poly_deg(g);
	if (gf_is_zero(b)) { // g(0)f(z)=(g(z)-g(0))/z and allows gf_mul_fast
		gf_inv(a, poly_coeff(g, 0));
		for (i = 0; i < t; i++) {
			gf_mul_fast(c, a, poly_coeff(g, i + 1));
			poly_set_coeff(f, i, c);
		}
	}
	else {
		poly_set_coeff_to_unit(f, t - 1);
		for (i = t - 2; i >= 0; i--) {
			gf_mul_fast(a, b, poly_coeff(f, i + 1));
			gf_add(a, a, poly_coeff(g, i + 1));
			poly_set_coeff(f, i, a);
		}
		gf_mul_fast(a, b, poly_coeff(f, 0));
		gf_add(c, a, poly_coeff(g, 0));
		// a equals g(b) and is assumed to be non zero
		gf_inv(a, c);
		for (i = 0; i < t; i++) {
			gf_mul_fast(c, a, poly_coeff(f, i));
			poly_set_coeff(f, i, c);
		}
	}

}

int partition(int * tableau, int gauche, int droite, int pivot) {
    int i, temp;
    for (i = gauche; i < droite; i++) {
        if (tableau[i] <= pivot) {
            temp = tableau[i];
            tableau[i] = tableau[gauche];
            tableau[gauche] = temp;
            ++gauche;
        }
	}
    return gauche;
}

void quickSort(int * tableau, int gauche, int droite, int min, int max) {
  if (gauche < droite - 1) {
    int milieu = partition(tableau, gauche, droite, (max + min) / 2);
    quickSort(tableau, gauche, milieu, min, (max + min) / 2);
    quickSort(tableau, milieu, droite, (max + min) / 2, max);
  }
}

poly_t goppa_keyequation_patterson(poly_t R, goppa_t gamma) {
    int i, j;
    poly_t u, v, h, sigma, S, aux;
    gf_t a, b;

  //1. Compute S(z), such that, S(z)^2=(h(z)+z)%g(z).
  //2. Compute u(z),v(z), such that, deg(u)<=t/2, deg(v)<=(t-1)/2 and u(z)=S(z).v(z)%g(z).
  //3. Compute Sigma_e(z)=u(z^2)+z(v(z)^2).->The locator polynomial of the code C.

    if (poly_calcule_deg(R) < 0) {
	    sigma = poly_alloc(gamma->degree);
		poly_set_coeff_to_unit(sigma, 0);
		poly_set_deg(sigma, 0);
		return sigma;
	}
	
    poly_eeaux(&h ,&aux, R, gamma->g, 1);
    gf_inv(a, poly_coeff(aux,0));
    for (i = 0; i <= poly_deg(h); ++i) {
	    gf_mul_fast(b, a, poly_coeff(h, i));
        poly_set_coeff(h, i, b);
	}
    poly_free(aux);

  //  compute h(z) += z
	gf_set_to_unit(a);
    poly_addto_coeff(h, 1, a);

  // compute S square root of h (using sqrtmod)
  S = poly_alloc(gamma->degree - 1);
  for (i = 0; i < gamma->degree; i++) {
		gf_sqrt(a, poly_coeff(h, i));
		if (i & 1) {
#ifndef SQRT_NO_PRECOMP
			aux = gamma->sqrtmod[i / 2];
#else
			if (i == 1) {
				aux = poly_copy(gamma->sqrtzmod);
			}
			else {
				poly_shiftmod(aux, gamma->g);
			}
#endif
			if (!gf_is_zero(a)) {
				for (j = 0; j < gamma->degree; j++) {
					gf_mul_fast(b, a, poly_coeff(aux, j));
					poly_addto_coeff(S, j, b);
				}
			}
		}
		else {
			poly_addto_coeff(S, i / 2, a);
		}
  }
  poly_calcule_deg(S);
  poly_free(h);

#ifdef SQRT_NO_PRECOMP
	poly_free(aux);
#endif

  // solve the key equation u(z) = v(z)*S(z) mod g(z)
  poly_eeaux(&v, &u, S, gamma->g, gamma->degree / 2 + 1);
  poly_free(S);

  // sigma = u^2+z*v^2
  sigma = poly_alloc(gamma->degree);
  for (i = 0; i <= poly_deg(u); ++i) {
		gf_square(b, poly_coeff(u, i));
        poly_set_coeff(sigma, 2 * i, b);
  }
  for (i = 0; i <= poly_deg(v); ++i) {
		gf_square(b, poly_coeff(v, i));
        poly_set_coeff(sigma, 2 * i + 1, b);
    }
    poly_free(u);
    poly_free(v);

    poly_calcule_deg(sigma);

  return sigma;
}

void poly_to_bin_addto(poly_t p, unsigned long * pt, int degree) {
	int i, j, k, l;
	unsigned long c;
	for (i = 0, l = 0; l < degree; ++l, i += gf_extd()) {
		k = i / __WORDSIZE;
		j = i % __WORDSIZE;
		c = gf_to_index(poly_coeff(p, l));
		pt[k] ^=  c << j;
		if (__WORDSIZE - j < gf_extd()) {
			pt[k + 1] ^= c >> (__WORDSIZE - j);
		}
	}
}

void bin_to_poly(unsigned long * pt, poly_t p, int degree) {
	int i, j, k, l;
	unsigned long u;
	gf_t a;

	for (i = 0, l = 0; l < degree; ++l, i += gf_extd()) {
		k = i / __WORDSIZE;
		j = i % __WORDSIZE;
		u = pt[k] >> j;
		if (__WORDSIZE - j < gf_extd()) {
			u ^= pt[k + 1] << (__WORDSIZE - j);
		}
    u &= ((1UL << gf_extd()) - 1);
		gf_from_index(a, (gfindex_t) u);
    poly_set_coeff(p, l, a);
	}
}


void goppa_decode_init(goppa_t gamma) {
	if (gamma->sqrtzmod == NULL) {
		gamma->sqrtzmod = poly_sqrtmod(gamma->g);
	}
#ifndef SQRT_NO_PRECOMP
	if (gamma->sqrtmod == NULL) {
		int i;
		gamma->sqrtmod = calloc(gamma->degree / 2, sizeof (poly_t));
		gamma->sqrtmod[0] = poly_copy(gamma->sqrtzmod);
		for (i = 1; i < gamma->degree / 2; ++i) {
			gamma->sqrtmod[i] = poly_copy(gamma->sqrtmod[i - 1]);
			poly_shiftmod(gamma->sqrtmod[i], gamma->g);
			poly_calcule_deg(gamma->sqrtmod[i]);
		}
	}
#endif
#ifndef PARITY_NO_PRECOMP
	if (gamma->parity == NULL) {
		if (gamma->parity) {
			free(gamma->parity[0]);
			free(gamma->parity);
		}

		int i, nb_synd, alloc_size, degree;
		unsigned long * synd;
		poly_t p;
		gfelt_t * L;

		degree = gamma->degree;
		alloc_size = 1 + (degree * gf_extd() - 1) / __WORDSIZE;

		L = gamma->L + (gamma->length - gamma->degree * gf_extd());
		nb_synd = gamma->degree * gf_extd();		

		gamma->parity = (unsigned long **) malloc(nb_synd * sizeof (unsigned long *));
		// we need a calloc as synd must be filled with 0s
		synd = (unsigned long *) calloc(nb_synd * alloc_size, sizeof (unsigned long));
		p = poly_alloc(degree - 1);
		for (i = 0; i < nb_synd; i++, synd += alloc_size) {
			poly_syndrome_patterson(p, L + i, gamma->g);
			// value is added to synd (which is filled with 0s)
			poly_to_bin_addto(p, synd, degree);
			gamma->parity[i] = synd;
		}
		poly_free(p);
	}
#endif
}

poly_t goppa_syndrome(const unsigned char * s, goppa_t gamma) {
	int i, alloc_size, nb_synd, degree;
	poly_t R;
	unsigned long * synd;

#ifdef PARITY_NO_PRECOMP
	gfelt_t * L;
#endif

	goppa_decode_init(gamma);

	degree = gamma->degree;
	alloc_size = 1 + (degree * gf_extd() - 1) / __WORDSIZE;



#ifdef PARITY_NO_PRECOMP
	L = gamma->L + (gamma->length - gamma->degree * gf_extd());
#endif
	nb_synd = gamma->degree * gf_extd();

	// we need a calloc as synd must be filled with 0s
	synd = (unsigned long *) calloc(alloc_size, sizeof (unsigned long));
	R = poly_alloc(degree );
	for (i = 0; i < nb_synd; i++) {
		if ((s[i / 8] >> (i % 8)) & 1) {
#ifndef PARITY_NO_PRECOMP
			xor_long(synd, gamma->parity[i], alloc_size);
#else
			poly_syndrome_patterson(R, L + i, gamma->g);
			poly_to_bin_addto(R, synd, degree);
#endif
		}
	}
	bin_to_poly(synd, R, degree);
	poly_calcule_deg(R);
	free(synd);

  return R;
}


int goppa_locate_error(poly_t sigma, int * e, goppa_t gamma) {
	int i, d;
	gfelt_t * roots;

	roots = (gfelt_t *) malloc(gamma->degree * sizeof (gfelt_t));
	d = roots_berl(sigma, roots);

  for (i = 0; i < d; ++i) {
    e[i] = gamma->Linv[gf_to_index(roots + i)];
	}
	free(roots);

  // we want the error pattern sorted in increasing order
	quickSort(e, 0, d, 0, gamma->length);
  return d;
}



int goppa_decode(const unsigned char * b, int * e, goppa_t gamma) {
	int i;
	poly_t R, sigma;

	R = goppa_syndrome(b, gamma);
	sigma = goppa_keyequation_patterson(R, gamma);
	i = goppa_locate_error(sigma, e, gamma);
	poly_free(R);
	poly_free(sigma);

	return i;
}

















