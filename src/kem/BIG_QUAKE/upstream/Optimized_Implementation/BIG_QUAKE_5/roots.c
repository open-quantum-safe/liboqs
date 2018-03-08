#include <stdlib.h>
#include <string.h>
#include "gf.h"
#include "poly.h"

int roots_kernel(gfindex_t * M, gfindex_t * K) {
	int i, j, k;
	gfindex_t a;
	int p[gf_extd()];

	for (j = 0, k = 0; j < gf_extd(); ++j) {
		for (i = 0; i < k; ++i) {
			if (M[p[i]] & (1 << j)) {
				a = M[p[i]];
				M[p[i]] = M[j];
				M[j] = a;
				break;
			}
		}
		for (i = j; i < gf_extd(); ++i) {
			if (M[i] & (1 << j)) {
				a = M[i];
				M[i] = M[j];
				M[j] = a;
				break;
			}
		}
		if (i < gf_extd()) {
			for (i = 0; i < j; ++i) {
				if (M[i] & (1 << j)) {
					M[i] ^= M[j];
				}
			}
			for (i = j + 1; i < gf_extd(); ++i) {
				if (M[i] & (1 << j)) {
					M[i] ^= M[j];
				}
			}
		}
		else {
			p[k] = j;
			K[k] = 0;
			for (i = 0; i < j; ++i) {
				K[k] ^= ((M[i] >> j) & 1) << i;
			}
			K[k] ^= 1 << j;
			++k;
		}
	}
	return k;
}

int roots_invert(gfindex_t * M, gfindex_t * U, gfindex_t * K) {
	int i, j, k;
	gfindex_t a;
	int p[gf_extd()];

	for (i = 0; i < gf_extd(); ++i) {
		U[i] = 1 << i;
	}

	for (j = 0, k = 0; j < gf_extd(); ++j) {
		for (i = 0; i < k; ++i) {
			if (M[p[i]] & (1 << j)) {
				a = M[p[i]];
				M[p[i]] = M[j];
				M[j] = a;
				a = U[p[i]];
				U[p[i]] = U[j];
				U[j] = a;
				break;
			}
		}
		for (i = j; i < gf_extd(); ++i) {
			if (M[i] & (1 << j)) {
				a = M[i];
				M[i] = M[j];
				M[j] = a;
				a = U[i];
				U[i] = U[j];
				U[j] = a;
				break;
			}
		}
		if (i < gf_extd()) {
			for (i = 0; i < j; ++i) {
				if (M[i] & (1 << j)) {
					M[i] ^= M[j];
					U[i] ^= U[j];
				}
			}
			for (i = j + 1; i < gf_extd(); ++i) {
				if (M[i] & (1 << j)) {
					M[i] ^= M[j];
					U[i] ^= U[j];
				}
			}
		}
		else {
			p[k] = j;
			if (K != NULL) {
				K[k] = 0;
				for (i = 0; i < j; ++i) {
					K[k] ^= ((M[i] >> j) & 1) << i;
				}
				K[k] ^= 1 << j;
			}
			++k;
		}
	}
	return k;
}

void roots_transpose(gfindex_t * M, gfindex_t * T) {
	int i, j;

	for (i = 0; i < gf_extd(); ++i) {
		T[i] = 0;
		for (j = 0; j < gf_extd(); ++j) {
			T[i] ^= ((M[j] >> i) & 1) << j;
		}
	}
}

gfindex_t roots_mulvm(gfindex_t a, gfindex_t * M) {
	int i;
	gfindex_t b = 0;

	for (i = 0; i < gf_extd(); ++i) {
		if (a & (1 << i)) {
			b ^= M[i];
		}
	}
	return b;
}

static int parity(gfindex_t a) {
	int i = gf_extd();
	while (i > 1) {
		i = (i + 1) / 2;
		a ^= (a >> i);
		a &= (1 << i) - 1;
	}
	return a & 1;
}

gfindex_t roots_mulmv(gfindex_t * M, gfindex_t a) {
	int i;
	gfindex_t b;

	b = 0;
	for (i = 0; i < gf_extd(); ++i) {
		b ^= parity(M[i] & a) << i;
	}
	return b;
}

gfindex_t * F, * R2, ** R3;
poly_t * tr, * aux1, * aux2, * aux3, * tr_aux, * sq_aux ;
int * level_flag;
int roots_init_done = 0;

int roots_precomp_init_done = 0;

void roots_precomp_clear() {
	int i;

	if (roots_precomp_init_done) {
		for (i = 0; i < roots_init_done; ++i) {
			poly_free(tr_aux[i]);
			if (tr[i] != NULL) {
				poly_free(tr[i]);
				poly_free(aux1[i]);
				poly_free(aux2[i]);
				poly_free(aux3[i]);
			}
		}
		for (i = 0; i < roots_precomp_init_done; ++i) {
			poly_free(sq_aux[i]);
		}
		free(sq_aux);
		roots_precomp_init_done = 0;
	}
}

void roots_precomp_init(int t) {
	if (roots_precomp_init_done < t) {
		int i;

		if (roots_precomp_init_done) {
			roots_precomp_clear();
		}
		sq_aux = malloc(t * sizeof (poly_t));
		for (i = 0; i < t; ++i)
			sq_aux[i] = poly_alloc(t + 1);
		for (i = 0; i < gf_extd(); ++i)
			tr_aux[i] = poly_alloc(t - 1);

		roots_precomp_init_done = t;
	}
}

void roots_clear() {
	int i;

	if (roots_init_done) {
		free(F);
		free(R2);
		for (i = 0; i < (1 << roots_init_done); ++i) {
			if (R3[i] != NULL) {
				free(R3[i]);
			}
		}
		free(R3);
		roots_precomp_clear();
		free(tr);
		free(tr_aux);
		free(aux1);
		free(aux2);
		free(aux3);
		free(level_flag);
		roots_init_done = 0;
	}
}

void roots_init() {
	gfindex_t M[gf_extd()], T[gf_extd()];
	int i;
	gf_t a;

	if (roots_init_done != gf_extd()) {
		if (roots_init_done > 0) {
			roots_clear();
		}
		F = malloc(gf_extd() * sizeof (gfindex_t));

		// solve z^2+z=0
		for (i = 0; i < gf_extd(); ++i) {
			gf_from_index(a, 1 << i);
			gf_square(a, a);
			M[i] = (1 << i) ^ gf_to_index(a);
		}
		roots_transpose(M, T);
		roots_invert(T, M, NULL);
		roots_transpose(M, F);
		R2 = calloc(1 << gf_extd(), sizeof (gfindex_t));
		R3 = calloc(1 << gf_extd(), sizeof (gfindex_t *));
		roots_init_done = gf_extd();

		tr = calloc(gf_extd(), sizeof (poly_t));
		tr_aux = calloc(gf_extd(), sizeof (poly_t));
		aux1 = calloc(gf_extd(), sizeof (poly_t));
		aux2 = calloc(gf_extd(), sizeof (poly_t));
		aux3 = calloc(gf_extd(), sizeof (poly_t));
		level_flag = calloc(gf_extd(), sizeof (int));
	}
}

/*
	Compute and store one of the two solutions R2[a] of the equation
	z^2+z=a in the finite field. The other solution is R2[a]+1.  If
	z^2+z=a has no solutions in the finite field, the result is
	unspecified.
*/
gfindex_t roots2_linear(gfindex_t a) {
	if (R2[a] == 0) {
		if (a == 0) {
			R2[a] = 1;
		}
		/*
			The matrix F contains an inverse of z->z^2+z, so that b=a*F is
			such that b^2+b=a
		*/
		R2[a] = roots_mulvm(a, F);
	}
	return R2[a];
}

/*
	If the polynomial of degree 2 defined by coeff has 2 distinct roots
	in the finite field, those roots are placed in the table res.
	Else, the result is unspecified.
*/
int roots2(gfelt_t * coeff, gfelt_t * res) {
  gf_t a, b, c;

	// coeff[2] != 0 because the degree is 2
	// coeff[1] != 0 because all roots are distinct
	gf_div(a, coeff + 1, coeff + 2);
	gf_mul_fast(b, coeff + 2, coeff);
	gf_mul_fast(c, coeff + 1, coeff + 1);
	gf_div(b, b, c);
	gf_from_index(c, roots2_linear(gf_to_index(b)));
	gf_mul_fast(res, a, c);
	gf_add(res + 1, res, a); // =a*(c+1)

	return 2;
}

/*
	If the linear polynomial of degree 4 defined by coeff
	(i.e. coeff[3]==0) has 4 distinct roots in the finite field, those
	roots are placed in the table res.
	Else, the result is unspecified.
*/
int roots4_linear(gfelt_t * coeff, gfelt_t * res) {
	int i;
	gfindex_t M[gf_extd()], T[gf_extd()], U[gf_extd()], K[2];
	gf_t a, b;

	for (i = 0; i < gf_extd(); ++i) {
		gf_from_index(a, 1 << i);
		gf_mul_fast(b, a, coeff + 1);
		M[i] = gf_to_index(b);
		gf_square(a, a);
		gf_mul_fast(b, a, coeff + 2);
		M[i] ^= gf_to_index(b);
		gf_square(a, a);
		gf_mul_fast(b, a, coeff + 4);
		M[i] ^= gf_to_index(b);
	}
	roots_transpose(M, T);
	roots_invert(T, U, K);
	gf_from_index(res, roots_mulmv(U, gf_to_index(coeff)));
	gf_from_index(a, K[0]);
	gf_add(res + 1, res, a);
	gf_from_index(a, K[1]);
	gf_add(res + 2, res, a);
	gf_add(res + 3, res + 1, a);

	return 4;
}

/*
	If the polynomial of degree 3 defined by coeff has 3 distinct roots
	in the finite field, those roots are placed in the table res.
	Else, the result is unspecified.
*/
int roots3(gfelt_t * coeff, gfelt_t * res) {
	gf_t a, b;
	int i, j, l;
	gfelt_t r[4], c[5];

	/*
		(z + c2/c3) * (c3 z^3 + c2 z^2 + c1 z + c0)
		= c3 z^4 + (c2^2/c3+c1) z^2 + (c1c2/c3+c0) z + c0c2/c3
	*/
	gf_set(c + 4, coeff + 3);
	gf_set_to_zero(c + 3);
	gf_div(a, coeff + 2, coeff + 3);
	gf_mul(b, a, coeff + 2);
	gf_add(c + 2, b, coeff + 1);
	gf_mul(b, a, coeff + 1);
	gf_add(c + 1, b, coeff);
	gf_mul(c, a, coeff);
	i = roots4_linear(c, r);

	for (j = 0, l = 0; j < i; ++j) {
		if (gf_eq(a, r + j)) {
			continue;
		}
		gf_set(res + l, r + j);
		++l;
	}

	return i - 1;
}

/*
	If the polynomial of degree 4 defined by coeff has 4 distinct roots
	in the finite field, those roots are placed in the table res.
	Else, the result is unspecified.
*/
int roots4(gfelt_t * coeff, gfelt_t * res) {
	gf_t a, b;
	gfelt_t cinv[5];
	struct polynome p = {4, 5, coeff};

	if (gf_is_zero(coeff + 3)) {
		return roots4_linear(coeff, res);
	}
	gf_div(b, coeff + 1, coeff + 3);
	gf_sqrt(a, b);
	poly_eval(cinv + 4, &p, a);
	gf_set_to_zero(cinv + 3);
	gf_mul(b, a, coeff + 3);
	gf_add(cinv + 2, coeff + 2, b);
	gf_set(cinv + 1, coeff + 3);
	gf_set(cinv, coeff + 4);
	roots4_linear(cinv, res);
	gf_inv(b, res);
	gf_add(res, b, a);
	gf_inv(b, res + 1);
	gf_add(res + 1, b, a);
	gf_inv(b, res + 2);
	gf_add(res + 2, b, a);
	gf_inv(b, res + 3);
	gf_add(res + 3, b, a);

	return 4;
}

#define BZ_LIMIT 4

void roots_precomp_init_level(int e) {
  if (level_flag[e] == 0) {
		int i, j, t = roots_precomp_init_done;
		gf_t a, b;
		if (tr[e] == NULL) {
			tr[e] = poly_alloc(t - 1);
			aux1[e] = poly_alloc(t);
			aux2[e] = poly_alloc(t);
			aux3[e] = poly_alloc(t - 1);
		}
		else {
			poly_set_to_zero(tr[e]);
		}
		if (e == 0) { // level 0 is much simpler
			poly_set_coeff_to_unit(tr[0], 1);
			for (i = 1; i < gf_extd(); ++i) {
				for (j = 0; j < t; ++j) {
					poly_addto_coeff(tr[0], j, poly_coeff(tr_aux[i], j));
				}
			}
		}
		else {
			gf_from_index(a, 1 << e);
			for (i = 0; i < gf_extd(); ++i) {
				for (j = 0; j < t; ++j) {
					gf_mul_fast(b, a, poly_coeff(tr_aux[i], j));
					poly_addto_coeff(tr[e], j, b);
				}
				gf_square(a, a);
			}
		}
    poly_calcule_deg(tr[e]);
		level_flag[e] = 1;
  }
}

// destructive for sigma
int roots_berl_aux(poly_t sigma, int d, int e, gfelt_t * res) {
	poly_t gcd1, gcd2;
	int i, j;

  if (d == 0) {
    return 0;
  }

  if (d == 1) {
		gf_div(res, poly_coeff(sigma, 0), poly_coeff(sigma, 1));
    return 1;
  }

#if BZ_LIMIT >= 2
  if (d == 2) {
		return roots2(sigma->coeff, res);
  }
#endif

#if BZ_LIMIT >= 3
  if (d == 3) {
		return roots3(sigma->coeff, res);
	}
#endif

#if BZ_LIMIT >= 4
  if (d == 4) {
		return roots4(sigma->coeff, res);
	}
#endif

  // not before because we may have e == gf_extd() and d == 1
  if (e >= gf_extd()) {
    return 0;
  }

	roots_precomp_init_level(e);
	poly_set(aux3[e], tr[e]);
	/* poly_ee_aux() is destructive - the value of sigma is changed - we
		 will use either (aux1[e], aux3[e]) or (aux2[e], sigma) */
	if (poly_deg(aux3[e]) >= poly_deg(sigma)) {
		poly_rem(aux3[e], sigma);
	}
	i = poly_ee_aux(aux1[e], aux2[e], sigma, aux3[e], 0);
	if (i & 1) {
		gcd1 = aux1[e];
		gcd2 = aux3[e];
	}
	else {
		gcd1 = aux2[e];
		gcd2 = sigma;
	}
	poly_set_deg(gcd2, d - poly_deg(gcd1));

  i = poly_deg(gcd1);
  j = roots_berl_aux(gcd1, i, e + 1, res);
  j += roots_berl_aux(gcd2, d - i, e + 1, res + j);

  return j;
}

int roots_berl(poly_t sigma, gfelt_t * res) {
	int i, d, t;

	t = poly_calcule_deg(sigma);
	roots_init();
	roots_precomp_init(t);

	poly_sqmod_init(sigma, sq_aux);
	poly_set_to_zero(tr_aux[0]);
	poly_set_coeff_to_unit(tr_aux[0], 1);
	poly_set_deg(tr_aux[0], 1);
	for (i = 1; i < gf_extd(); ++i) {
		poly_sqmod(tr_aux[i], tr_aux[i - 1], sq_aux, t);
	}

	for (i = 0; i < gf_extd(); ++i) {
		level_flag[i] = 0;
	}
	d = roots_berl_aux(sigma, t, 0, res);
	return d;
}
