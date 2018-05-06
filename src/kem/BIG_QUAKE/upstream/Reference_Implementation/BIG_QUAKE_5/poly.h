/******************************************************************************
 * BIG QUAKE -- BInary Goppa QUAsi-cyclic Key Encapsulation
 *
 * Copyright (c) 2017 the BIG QUAKE members
 * https://bigquake.inria.fr/
 *
 * Permission to use this code for BIG QUAKE is granted.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ""AS IS"" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS CORPORATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef POLY_H
#define POLY_H

#include "gf.h"

typedef struct polynome {
  int deg, size;
  gfelt_t * coeff;
} * poly_t; /* polynomial has coefficients in the finite field */

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define poly_deg(p) ((p)->deg)
#define poly_size(p) ((p)->size)
#define poly_set_deg(p, d) ((p)->deg = (d))
#define poly_coeff(p, i) ((p)->coeff + i)
#define poly_set_coeff(p, i, a) gf_set((p)->coeff + (i), (a))
#define poly_set_coeff_to_zero(p, i) gf_set_to_zero((p)->coeff + (i))
#define poly_set_coeff_to_unit(p, i) gf_set_to_unit((p)->coeff + (i))
#define poly_set_coeff_rand(p, i) gf_set_rand((p)->coeff + (i))
#define poly_addto_coeff(p, i, a) gf_add((p)->coeff + (i), (p)->coeff + (i), (a))
#define poly_multo_coeff(p, i, a) gf_mul((p)->coeff + (i), (p)->coeff + (i), (a))
#define poly_tete(p) ((p)->coeff + ((p)->deg))

/****** poly.c ******/

int poly_calcule_deg(poly_t p);
int poly_adjust_deg(poly_t p);
poly_t poly_alloc(int d);
poly_t poly_alloc_from_string(int d, const unsigned char * s);
poly_t poly_copy(poly_t p);
void poly_free(poly_t p);
void poly_set_to_zero(poly_t p);
void poly_set(poly_t p, poly_t q);
poly_t poly_mul(poly_t p, poly_t q);
poly_t poly_compose(poly_t p, poly_t q);
void poly_rem(poly_t p, poly_t g);
void poly_sqmod_init(poly_t g, poly_t * sq);
void poly_sqmod(poly_t res, poly_t p, poly_t * sq, int d);
poly_t poly_gcd_aux(poly_t p1, poly_t p2); // destructive
poly_t poly_gcd(poly_t p1, poly_t p2);
void poly_quo_aux(poly_t quo, poly_t rem, poly_t d); // destructive
poly_t poly_quo(poly_t p, poly_t d);
void poly_eval(gf_t res, poly_t p, gf_t a);
int poly_degppf(poly_t g);
int poly_ee_aux(poly_t u0, poly_t u1, poly_t r0, poly_t r1, int t);
void poly_eeaux(poly_t * u, poly_t * v, poly_t p, poly_t g, int t);

void poly_shiftmod(poly_t p, poly_t g);
poly_t poly_sqrtmod(poly_t g);
poly_t * poly_sqrtmod_init(poly_t g);
poly_t poly_randgen_irred(int t);
#endif /* POLY_H */
