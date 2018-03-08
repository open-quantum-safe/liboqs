#include <stdio.h>
#include <stdlib.h>
#include "gf.h"
#include "rng.h"

// binary primitive polynomials of degree up to 31
static unsigned prim_poly[MAX_EXT_DEG + 1] = {
    01,		        /* extension degree 0 (!) never used */
    03,		        /* extension degree 1 (!) never used */
    07, 		      /* extension degree 2 */
    013, 		      /* extension degree 3 */
    023, 		      /* extension degree 4 */
    045, 		      /* extension degree 5 */
    0103, 		    /* extension degree 6 */
    0203, 		    /* extension degree 7 */
    0435, 		    /* extension degree 8 */
    01041, 		    /* extension degree 9 */
    02011,		    /* extension degree 10 */
    04005,		    /* extension degree 11 */
    010123,		    /* extension degree 12 */
    020033,		    /* extension degree 13 */
    042103,		    /* extension degree 14 */
    0100003,	    /* extension degree 15 */
    0210013,	    /* extension degree 16 */
    0400011,			/* extension degree 17 */
    01000201,			/* extension degree 18 */
    02000047,			/* extension degree 19 */
    04000011,			/* extension degree 20 */
    010000005,		/* extension degree 21 */
    020000003,		/* extension degree 22 */
    040000041,		/* extension degree 23 */
    0100000207,		/* extension degree 24 */
    0200000011,		/* extension degree 25 */
    0400000107,		/* extension degree 26 */
    01000000047,	/* extension degree 27 */
    02000000011,	/* extension degree 28 */
    04000000005,	/* extension degree 29 */
    010040000007,	/* extension degree 30 */
    020000000011	/* extension degree 31 */
};

/*
void gf_set_to_zero(gf_t a) {
	a[0] = 0;
}

void gf_set_to_unit(gf_t a) {
	a[0] = 1;
}

void gf_set(gf_t a, gf_t b) {
	a[0] = b[0];
}

void gf_from_index(gf_t a, gfindex_t i) {
	a[0] = (gfelt_t) i;
}

gfindex_t gf_to_index(gf_t a) {
	return (gfindex_t) a[0];
}

int gf_is_zero(gf_t a) {
	return a[0] == 0;
}

int gf_is_unit(gf_t a) {
	return a[0] == 1;
}

int gf_eq(x, y) {
	return x[0] ==  y[0];
}

void gf_add(gf_t a, gf_t x, gf_t y) {
	a[0] = gfelt_add(x[0], y[0]);
}

void gf_mul(gf_t a, gf_t x, gf_t y) {
	a[0] = gfelt_mul(x[0], y[0]);
}

void gf_mul_fast(gf_t a, gf_t x, gf_t y) {
	a[0] = gfelt_mul_fast(x[0], y[0]);
}

void gf_square(gf_t a, gf_t x) {
	a[0] = gfelt_square(x[0]);
}

void gf_sqrt(gf_t a, gf_t x) {
	a[0] = gfelt_sqrt(x[0]);
}

void gf_div(gf_t a, gf_t x, gf_t y) {
	a[0] = gfelt_div(x[0], y[0]);
}

void gf_inv(gf_t a, gf_t x) {
	a[0] = gfelt_inv(x[0]);
}
*/

// construct the table gf_exp[i]=alpha^i
void gf_init_exp() {
  int i;

  gf_exp[0] = 1;
  for (i = 1; i < gf_ord(); ++i) {
    gf_exp[i] = gf_exp[i - 1] << 1;
    if (gf_exp[i - 1] & (1 << (gf_extd()-1)))
      gf_exp[i] ^= prim_poly[gf_extd()];
  }
  // hack for the multiplication
  gf_exp[gf_ord()] = 1;
}

// construct the table gf_log[alpha^i]=i
void gf_init_log() {
  int i;

  gf_log[0] = gf_ord();// log of 0 by convention
  for (i = 0; i < gf_ord() ; ++i)
    gf_log[gf_exp[i]] = i;
}

int init_done = 0;

int gf_init(int extdeg) {
  if ((extdeg < MIN_EXT_DEG) || (extdeg > MAX_EXT_DEG)) {
    fprintf(stderr,"Extension degree %d not implemented !\n", extdeg);
    exit(0);
  }
  if (init_done != extdeg) {
    if (init_done) {
      free(gf_exp);
      free(gf_log);
    }
		gf_extension_degree = extdeg;
    gf_cardinality = 1 << extdeg;
    gf_multiplicative_order = gf_cardinality - 1;
		gf_log = (gfindex_t *) malloc((1 << gf_extd()) * sizeof (gfindex_t));
		if (gf_log == NULL) {
			fprintf(stderr,"allocation failed for gf_log!\n");
			return 0;
		}
		gf_exp = (gfelt_t *) malloc((1 << gf_extd()) * sizeof (gfelt_t));
		if (gf_exp == NULL) {
			fprintf(stderr,"allocation failed for gf_exp!\n");
			free(gf_log);
			return 0;
		}
    gf_init_exp();
    gf_init_log();
		init_done = extdeg;
  }

  return extdeg;
}

void gf_clear() {
	if (init_done) {
		free(gf_exp);
		free(gf_log);
	}
	init_done = 0;
}

// we suppose i >= 0. By convention 0^0 = 1
void gf_pow(gf_t a, gf_t x, int i) {
	uint64_t y;

  if (i == 0)
    a[0] = 1;
  else if (x[0] == 0)
    a[0] = 0;
  else {
    // i mod (q-1)
    while (i >> gf_extd())
      i = (i & (gf_ord())) + (i >> gf_extd());
    y = i * gf_log[x[0]];
    while (y >> gf_extd())
      y = (y & (gf_ord())) + (i >> gf_extd());
    a[0] = gf_exp[y];
  }
}

static int ucharToInt(unsigned char * c, int s) {
    int i, res = 0;
    
    for(i=0; i < s; i++){
        res <<= 8;
        res ^= (c[i] & 0xff);
    }
    
    return res;
}

void gf_set_rand(gf_t a) {

	int m = gf_extd();
	unsigned char * rand = malloc(((m - 1)/ 8 + 1) * sizeof(unsigned char));
	randombytes(rand, (m - 1)/ 8 + 1);
	
	a[0] = 0;
	while (m > 0) {
		a[0] = ucharToInt(rand + m/8, 1) ^ (a[0] << 8); //$Elise : À VÉRIFIER!!!
		m -= 8;
	}
	
	free(rand);
	a[0] &= gf_ord();
}
