#ifndef PQCLEAN_MCELIECE6688128F_VEC_VEC_H
#define PQCLEAN_MCELIECE6688128F_VEC_VEC_H

#include "params.h"

#include <stdint.h>

typedef uint64_t vec;

vec PQCLEAN_MCELIECE6688128F_VEC_vec_setbits(vec b);

vec PQCLEAN_MCELIECE6688128F_VEC_vec_set1_16b(uint16_t v);

void PQCLEAN_MCELIECE6688128F_VEC_vec_copy(vec *out, const vec *in);

vec PQCLEAN_MCELIECE6688128F_VEC_vec_or_reduce(const vec *a);

int PQCLEAN_MCELIECE6688128F_VEC_vec_testz(vec a);

void PQCLEAN_MCELIECE6688128F_VEC_vec_mul(vec * /*h*/, const vec * /*f*/, const vec * /*g*/);
void PQCLEAN_MCELIECE6688128F_VEC_vec_sq(vec * /*out*/, const vec * /*in*/);
void PQCLEAN_MCELIECE6688128F_VEC_vec_inv(vec * /*out*/, const vec * /*in*/);

#endif

