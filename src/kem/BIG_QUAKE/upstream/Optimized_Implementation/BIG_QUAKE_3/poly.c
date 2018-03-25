#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gf.h"
#include "poly.h"

poly_t poly_alloc(int d) {
  poly_t p;

  p = (poly_t) malloc(sizeof (struct polynome));
  p->deg = -1;
  p->size = d + 1;
  p->coeff = (gfelt_t *) calloc(p->size, sizeof (gfelt_t));
  return p;
}

// assumes s has the proper allocated size
poly_t poly_alloc_from_string(int d, const unsigned char * s) {
  poly_t p;

  p = (poly_t) malloc(sizeof (struct polynome));
  p->deg = -1;
  p->size = d + 1;
  p->coeff = (gfelt_t *) s;
  return p;
}

poly_t poly_copy(poly_t p) {
  poly_t q;

  q = (poly_t) malloc(sizeof (struct polynome));
  q->deg = p->deg;
  q->size = p->size;
  q->coeff = (gfelt_t *) calloc(q->size, sizeof (gfelt_t));
  memcpy(q->coeff, p->coeff, p->size * sizeof (gfelt_t));
  return q;
}

void poly_free(poly_t p) {
  free(p->coeff);
  free(p);
}

void poly_set_to_zero(poly_t p) {
  memset(p->coeff, 0, p->size * sizeof (gfelt_t));
  p->deg = -1;
}

int poly_calcule_deg(poly_t p) {
  int d = p->size - 1;
  while ((d >= 0) && (gf_is_zero(p->coeff + d)))
    --d;
  p->deg = d;
  return d;
}

/* to be used if p->deg is an upper bound of the actual degree */
int poly_adjust_deg(poly_t p) {
  int d = p->deg;
  while ((d >= 0) && (gf_is_zero(p->coeff + d)))
    --d;
  p->deg = d;
  return d;
}

// copy q in p
void poly_set(poly_t p, poly_t q) {
  int d = p->size - q->size;
  if (d < 0) {
    memcpy(p->coeff, q->coeff, p->size * sizeof (gfelt_t));
    poly_calcule_deg(p);
  }
  else {
    memcpy(p->coeff, q->coeff, q->size * sizeof (gfelt_t));
    memset(p->coeff + q->size, 0, d * sizeof (gfelt_t));
    p->deg = q->deg;
  }
}

// p <- p+q
poly_t poly_addto(poly_t p, poly_t q) {
  int i;

  for (i = 0; i < p->size; ++i)
		if (i < q->size) 
			poly_addto_coeff(p, i, poly_coeff(q, i));
  poly_calcule_deg(p);

  return(p);
}

poly_t poly_mul(poly_t p, poly_t q) {
  int i,j,dp,dq;
  poly_t r;
    gf_t a;

  poly_calcule_deg(p);
  poly_calcule_deg(q);
  dp = poly_deg(p);
  dq = poly_deg(q);
  r = poly_alloc(dp + dq);
  for (i = 0; i <= dp; ++i) {
    for (j = 0; j <= dq; ++j) {
            gf_mul(a, poly_coeff(p, i), poly_coeff(q, j));
      poly_addto_coeff(r, i + j, a);
        }
    }
  poly_calcule_deg(r);

  return(r);
}

// a != 0
void poly_eval_aux(gf_t res, gfelt_t * coeff, gf_t a, int d) {
  gf_set(res, coeff + d);
  for (--d; d >= 0; --d) {
        gf_mul_fast(res, a, res);
        gf_add(res, res, coeff + d);
    }
}

void poly_eval(gf_t res, poly_t p, gf_t a) {
    if (gf_is_zero(a)) {
        gf_set(res, poly_coeff(p, 0));
    }
    else {
        poly_eval_aux(res, p->coeff, a, poly_deg(p));
    }
}

// return p(q)
poly_t poly_compose(poly_t p, poly_t q) {
    int i,j,dp,dq;
    poly_t r;
    gf_t a;
    
    poly_calcule_deg(p);
    poly_calcule_deg(q);
    dp = poly_deg(p);
    dq = poly_deg(q);
    r = poly_alloc(dp*dq);
    for (i = 0; i <= dp; ++i) {
        for (j = 0; j <= dq; ++j) {
            gf_mul(a, poly_coeff(p,i), poly_coeff(q,j));
            poly_addto_coeff(r, i*j, a);
        }
    }
    poly_calcule_deg(r);
   
    return(r);
}

// p contains its remainder modulo g
void poly_rem(poly_t p, poly_t g) {
  int i, j, d;
  gf_t a, b, c;

  d = poly_deg(p) - poly_deg(g);
  if (d >= 0) {
    gf_inv(a, poly_tete(g));
    for (i = poly_deg(p); d >= 0; --i, --d) {
      if (!gf_is_zero(poly_coeff(p, i))) {
                gf_mul_fast(b, a, poly_coeff(p, i));
                for (j = 0; j < poly_deg(g); ++j) {
                    gf_mul_fast(c, b, poly_coeff(g, j));
                    poly_addto_coeff(p, j + d, c);
                }
                poly_set_coeff_to_zero(p, i);
      }
    }
    poly_set_deg(p, poly_deg(g) - 1);
    while ((poly_deg(p) >= 0) && (gf_is_zero(poly_coeff(p, poly_deg(p))))) {
      poly_set_deg(p, poly_deg(p) - 1);
        }
  }
}

void poly_sqmod_init(poly_t g, poly_t * sq) {
  int i, d;

  d = poly_deg(g);

  for (i = 0; 2 * i < d; ++i) {
    // sq[i] = x^(2i) mod g = x^(2i)
    poly_set_to_zero(sq[i]);
    poly_set_deg(sq[i], 2 * i);
    poly_set_coeff_to_unit(sq[i], 2 * i);
  }

  for (; i < d; ++i) {
    // sq[i] = x^(2i) mod g = (x^2 * sq[i-1]) mod g
        poly_set_to_zero(sq[i]);
    memcpy(sq[i]->coeff + 2, sq[i - 1]->coeff, d * sizeof (gfelt_t));
    poly_set_deg(sq[i], poly_deg(sq[i - 1]) + 2);
    poly_rem(sq[i], g);
  }
}

/*Modulo p square of a certain polynomial g, sq[] contains the square
    Modulo g of the base canonical polynomials of degree < d, where d is
    the degree of G. The table sq[] will be calculated by poly_sqmod_init*/

void poly_sqmod(poly_t res, poly_t p, poly_t * sq, int d) {
  int i, j;
  gf_t a, b;

  poly_set_to_zero(res);

  // terms of low degree
  for (i = 0; i < d / 2; ++i) {
        gf_square(a, poly_coeff(p, i));
    poly_set_coeff(res, i * 2, a);
    }

  // terms of high degree
  for (; i < d; ++i) {
    if (!gf_is_zero(poly_coeff(p, i))) {
      gf_square(a, poly_coeff(p, i));
      for (j = 0; j < d; ++j) {
                gf_mul_fast(b, a, poly_coeff(sq[i], j));
                poly_addto_coeff(res, j, b);
            }
        }
  }

  // Update degre
  poly_set_deg(res, d - 1);
  while ((poly_deg(res) >= 0) && (gf_is_zero(poly_coeff(res, poly_deg(res))))) {
    poly_set_deg(res, poly_deg(res) - 1);
    }
}

// destructive
poly_t poly_gcd_aux(poly_t p1, poly_t p2) {
  if (poly_deg(p2) == -1)
    return p1;
  else {
    poly_rem(p1, p2);
    return poly_gcd_aux(p2, p1);
  }
}

poly_t poly_gcd(poly_t p1, poly_t p2) {
  poly_t a, b, c;

  a = poly_copy(p1);
  b = poly_copy(p2);
  if (poly_deg(a) < poly_deg(b))
    c = poly_copy(poly_gcd_aux(b, a));
  else
    c = poly_copy(poly_gcd_aux(a, b));
  poly_free(a);
  poly_free(b);
  return c;
}

// destroys rem, the "deg" field of rem and d must be correct
void poly_quo_aux(poly_t quo, poly_t rem, poly_t d) {
  int i, j, dd;
  gf_t a, b, c;

  dd = poly_deg(d);
  gf_inv(a, poly_coeff(d, dd));
  poly_set_to_zero(quo);
  for (i = poly_deg(rem); i >= dd; --i) {
    gf_mul_fast(b, a, poly_coeff(rem, i));
    poly_set_coeff(quo, i - dd, b);
    if (!gf_is_zero(b)) {
      poly_set_coeff_to_zero(rem, i);
      for (j = i - 1; j >= i - dd; --j) {
                gf_mul_fast(c, b, poly_coeff(d, dd - i + j));
                poly_addto_coeff(rem, j, c);
            }
        }
  }
  poly_set_deg(quo, poly_deg(rem) - dd);
}

poly_t poly_quo(poly_t p, poly_t d) {
  int dd, dp;
  poly_t quo, rem;

  dd = poly_calcule_deg(d);
  dp = poly_calcule_deg(p);
  rem = poly_copy(p);
  quo = poly_alloc(dp - dd);
    poly_quo_aux(quo, rem, d);
  poly_free(rem);

  return quo;
}

// Returns the degree of the smallest factor
int poly_degppf(poly_t g) {
    int i, d, res;
    poly_t *u, p, r, s;
    gf_t one;
    gf_set_to_unit(one);

    d = poly_deg(g);
    if (d == 1)
        return 1;
    
    u = malloc(d * sizeof (poly_t *));
    for (i = 0; i < d; ++i)
        u[i] = poly_alloc(d + 1);
    poly_sqmod_init(g, u);

    p = poly_alloc(d - 1);
    poly_set_deg(p, 1);
    poly_set_coeff_to_unit(p, 1);
    r = poly_alloc(d - 1);
    res = d;
    for (i = 1; i <= (d / 2) * gf_extd(); ++i) {
        poly_sqmod(r, p, u, d);
        // r = x^(2^i) mod g
        if ((i % gf_extd()) == 0) { // so 2^i = (2^m)^j (m ext. degree)
            poly_addto_coeff(r, 1, one);
            poly_calcule_deg(r); // The degree may change
            s = poly_gcd(g, r);
            if (poly_deg(s) > 0) {
            poly_free(s);
            res = i / gf_extd();
            break;
            }
            poly_free(s);
            poly_addto_coeff(r, 1, one);
            poly_calcule_deg(r); // The degree may change
        }
        // No need for the exchange s
        s = p;
        p = r;
        r = s;
    }

    poly_free(p);
    poly_free(r);
    for (i = 0; i < d; ++i) {
        poly_free(u[i]);
    }
    free(u);

    return res;
}

void poly_print(poly_t p) {
    int i;

    poly_calcule_deg(p);
    for (i = 0; i < p->size; ++i) {
        if (gf_is_zero(poly_coeff(p, i))) {
            printf("0");
        }
        else if (gf_is_unit(poly_coeff(p, i))) {
            printf("1");
        }
        else {
            printf("*");
        }
    }
    printf("\n");
}

/* destructive for r0 and r1

     we denote p and g respectively the initial values of r1 and r0
     we will set initially u0 and u1 to 0 and 1
     invariants:
     r0 + u0 * p = r1 + u1 * p = 0 (mod g)
     u1 * r0 + u0 * r1 = g
     the computation stops when:
     deg r1 < t <= deg r0
 */
int poly_ee_aux(poly_t u0, poly_t u1, poly_t r0, poly_t r1, int t) {
  int i, j, count = 0;
  gf_t a, b;
  poly_t aux;

  poly_set_to_zero(u0);
  poly_set_to_zero(u1);
  poly_set_coeff_to_unit(u1, 0);
  poly_set_deg(u1, 0);
  while (r1->deg >= t) {
        for (j = r0->deg - r1->deg; j >= 0; --j) {
      gf_div(a, poly_coeff(r0, r1->deg + j), poly_coeff(r1, r1->deg));
      if (!gf_is_zero(a)) {
                // u0(z) <- u0(z) + a * u1(z) * z^j
                for (i = 0; i <= u1->deg; ++i) {
                    gf_mul_fast(b, a, poly_coeff(u1, i));
                    poly_addto_coeff(u0, i + j, b);
                }
                // r0(z) <- r0(z) + a * r1(z) * z^j
                for (i = 0; i <= r1->deg; ++i) {
                    gf_mul_fast(b, a, poly_coeff(r1, i));
                    poly_addto_coeff(r0, i + j, b);
                }
            }
    }
        u0->deg = u1->deg + r0->deg - r1->deg; // exact
        r0->deg = r1->deg - 1; // upper bound
        poly_adjust_deg(r0);
    // exchange
    aux = r0; r0 = r1; r1 = aux;
    aux = u0; u0 = u1; u1 = aux;
        count++;
  }

    return count;
}

// We suppose deg(g) >= deg(p)
void poly_eeaux(poly_t * u, poly_t * v, poly_t p, poly_t g, int t) {
  int count, dr;
  poly_t r0, r1, u0, u1;

  // initialisation of the local variables
  // r0 <- g, r1 <- p, u0 <- 0, u1 <- 1
  dr = poly_deg(g);

  r0 = poly_copy(g);
  r1 = poly_copy(p);
  u0 = poly_alloc(dr);
  u1 = poly_alloc(dr);

    count = poly_ee_aux(u0, u1, r0, r1, t);
    /* at this point we do not know which is which, (u0,r0) and (u1,r1)
         have been switched count times in poly_ee_aux() */
    if (count & 1) {
        *u = u0;
        *v = r0;
        poly_free(r1);
        poly_free(u1);
    }
    else {
        *u = u1;
        *v = r1;
        poly_free(r0);
        poly_free(u0);
    }
}


// The field is already defined
// Return a polynomial of degree t irreducible in the field
poly_t poly_randgen_irred(int t) {

  int i;
  poly_t g;

  g = poly_alloc(t);
  poly_set_deg(g, t);
  poly_set_coeff_to_unit(g, t);

  i = 0;
  do {
      for (i = 0; i < t; ++i) {
        poly_set_coeff_rand(g, i);
      }
  } while (poly_degppf(g) < t);

  return g;
}


// p = p * x mod g
// p of degree <= deg(g)-1
void poly_shiftmod(poly_t p, poly_t g) {
  int i, t;
  gf_t a, b;

  t = poly_deg(g);
  gf_div(a, p->coeff + (t - 1), g->coeff + t);
  for (i = t - 1; i > 0; --i) {
        gf_mul(b, a, g->coeff + i);
        gf_add(p->coeff + i, p->coeff + (i - 1), b);
    }
    gf_mul(p->coeff, a, g->coeff);
}

// computes sqrt(z) mod g(z)
poly_t poly_sqrtmod(poly_t g) {
  int i, t;
  poly_t aux, p, q, * sq_aux;

  t = poly_deg(g);

  sq_aux = malloc(t * sizeof (poly_t));
  for (i = 0; i < t; ++i)
    sq_aux[i] = poly_alloc(t + 1);
  poly_sqmod_init(g, sq_aux);

  q = poly_alloc(t - 1);
  p = poly_alloc(t - 1);
  poly_set_deg(p, 1);

  poly_set_coeff_to_unit(p, 1);
  // q(z) = 0, p(z) = z
  for (i = 0; i < t * gf_extd() - 1; ++i) {
    // q(z) <- p(z)^2 mod g(z)
    poly_sqmod(q, p, sq_aux, t);
    // q(z) <-> p(z)
    aux = q; q = p; p = aux;
  }
  // p(z) = z^(2^(tm-1)) mod g(z) = sqrt(z) mod g(z)

  for (i = 0; i < t; ++i)
    poly_free(sq_aux[i]);
  free(sq_aux);
    poly_free(q);

  poly_calcule_deg(p);
    return p;
}

poly_t * poly_sqrtmod_init(poly_t g) {
  int i, t;
  poly_t * sqrt;

  t = poly_deg(g);

  sqrt = malloc(t * sizeof (poly_t));
    sqrt[0] = poly_alloc(t - 1);
    sqrt[1] = poly_sqrtmod(g); // p(z) = sqrt(z) mod g(z)
  for (i = 2; i < t; ++i)
    sqrt[i] = poly_alloc(t - 1);

  for(i = 3; i < t; i += 2) {
    poly_set(sqrt[i], sqrt[i - 2]);
    poly_shiftmod(sqrt[i], g);
    poly_calcule_deg(sqrt[i]);
  }

  for (i = 0; i < t; i += 2) {
    poly_set_to_zero(sqrt[i]);
    gf_set_to_unit(sqrt[i]->coeff + (i / 2));
    sqrt[i]->deg = i / 2;
  }

  return sqrt;
}
