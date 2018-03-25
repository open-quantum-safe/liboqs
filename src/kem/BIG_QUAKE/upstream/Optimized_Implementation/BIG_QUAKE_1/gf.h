#ifndef GF_H
#define GF_H

#include <stdint.h>

#define MAX_EXT_DEG 31
#define MIN_EXT_DEG 2

typedef uint32_t gfelt_t;
typedef uint32_t gfindex_t;
int gf_extension_degree, gf_cardinality, gf_multiplicative_order;
gfindex_t * gf_log;
gfelt_t * gf_exp;

typedef gfelt_t gf_t[1];

/* MACROs for most operations */

#define gf_extd() gf_extension_degree
#define gf_card() gf_cardinality
#define gf_ord() gf_multiplicative_order

#define gfelt_add(x, y) ((x) ^ (y))

/* residue modulo q-1 when 0 <= d <= 2*(q-1)
	 when 0 <= d < q, we get (d)
	 when q <= d < 2q-1, we get (d-q+1) */
#define _gfelt_modq_1(d) (((d) & gf_ord()) + ((d) >> gf_extd()))
/* we obtain a value between 0 and (q-1) included, the class of 0 is
	 represented by 0 or q-1 (this is why we write
	 _K->exp[q-1]=_K->exp[0]=1)*/

/* the cast in gfelt_mul_nocheck() and gfelt_div() might not be
	 necessary but it cannot hurt either */

#define gfelt_mul_nocheck(x, y) gf_exp[_gfelt_modq_1((uint32_t) gf_log[x] + gf_log[y])]
#define gfelt_mul_fast(x, y) ((y) ? gfelt_mul_nocheck(x, y) : 0)
#define gfelt_mul(x, y) ((x) ? gfelt_mul_fast(x, y) : 0)
/* cyclic shift left of the log */
#define gfelt_square(x) ((x) ? gf_exp[((gf_log[x] << 1) ^ (gf_log[x] >> (gf_extd()-1))) & gf_ord()] : 0)
/* cyclic shift right of the log */
#define gfelt_sqrt(x) ((x) ? gf_exp[(gf_log[x] >> 1) ^ ((gf_log[x] & 1) << (gf_extd()-1))] : 0)
/* try to divide by zero and get what you deserve! */
#define gfelt_div(x, y) ((x) ? gf_exp[_gfelt_modq_1((uint32_t) gf_log[x] + (gf_ord() - gf_log[y]))] : 0)
#define gfelt_inv(x) gf_exp[gf_ord() - gf_log[x]]

#define gf_add(a, x, y) ((a)[0] = gfelt_add((x)[0], (y)[0]))
#define gf_mul(a, x, y) ((a)[0] = gfelt_mul((x)[0], (y)[0]))
#define gf_mul_fast(a, x, y) ((a)[0] = gfelt_mul_fast((x)[0], (y)[0]))
#define gf_square(a, x) ((a)[0] = gfelt_square((x)[0]))
#define gf_sqrt(a, x) ((a)[0] = gfelt_sqrt((x)[0]))
#define gf_div(a, x, y) ((a)[0] = gfelt_div((x)[0], (y)[0]))
#define gf_inv(a, x) ((a)[0] = gfelt_inv((x)[0]))

#define gf_is_zero(x) ((x)[0] == 0)
#define gf_is_unit(x) ((x)[0] == 1)
#define gf_eq(x, y) ((x)[0] == (y)[0])

#define gf_set_to_zero(a) ((a)[0] = 0)
#define gf_set_to_unit(a) ((a)[0] = 1)
#define gf_set(a, b) ((a)[0] = (b)[0])

/* 
	 We want the elements gf_from_index(b_i,1<<i) for 0<=i<gf_extd() to
	 form a basis of GF(2^m) over GF(2), a kind of "implicit" basis

	 gf_to_index(x) writes a field element as an integer whose binary
	 representation is equal to the coordinates od the field element in
	 the above (implicit) basis

	 after gf_from_index(a, i) we have gf_to_index(a)=i for all
	 0<=i<gf_card()
*/
#define gf_from_index(a, i) ((a)[0] = (gfelt_t) (i))
#define gf_to_index(a) ((gfindex_t) (a)[0])

/****** gf.c ******/

/*
void gf_set_to_zero(gf_t a);
void gf_set_to_unit(gf_t a);
void gf_set(gf_t a, gf_t b);
void gf_from_index(gf_t a, gfindex_t i);
gfindex_t gf_to_index(gf_t a);
int gf_is_zero(gf_t a);
int gf_is_unit(gf_t a);
void gf_add(gf_t a, gf_t x, gf_t y);
void gf_mul(gf_t a, gf_t x, gf_t y);
void gf_mul_fast(gf_t a, gf_t x, gf_t y);
void gf_square(gf_t a, gf_t x);
void gf_sqrt(gf_t a, gf_t x);
void gf_div(gf_t a, gf_t x, gf_t y);
void gf_inv(gf_t a, gf_t x);
*/
void gf_pow(gf_t a, gf_t x, int i);
void gf_set_rand(gf_t a);
int gf_init(int extdeg);
void gf_clear();

#endif /* GF_H */
