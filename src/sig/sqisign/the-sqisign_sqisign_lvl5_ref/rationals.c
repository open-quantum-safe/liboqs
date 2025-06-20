#include <stdio.h>
#include "internal.h"
#include "lll_internals.h"

void
ibq_init(ibq_t *x)
{
    ibz_init(&((*x)[0]));
    ibz_init(&((*x)[1]));
    ibz_set(&((*x)[1]), 1);
}

void
ibq_finalize(ibq_t *x)
{
    ibz_finalize(&((*x)[0]));
    ibz_finalize(&((*x)[1]));
}

void
ibq_mat_4x4_init(ibq_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibq_init(&(*mat)[i][j]);
        }
    }
}
void
ibq_mat_4x4_finalize(ibq_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibq_finalize(&(*mat)[i][j]);
        }
    }
}

void
ibq_vec_4_init(ibq_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibq_init(&(*vec)[i]);
    }
}
void
ibq_vec_4_finalize(ibq_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibq_finalize(&(*vec)[i]);
    }
}

void
ibq_mat_4x4_print(const ibq_mat_4x4_t *mat)
{
    printf("matrix: ");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_print(&((*mat)[i][j][0]), 10);
            printf("/");
            ibz_print(&((*mat)[i][j][1]), 10);
            printf(" ");
        }
        printf("\n        ");
    }
    printf("\n");
}

void
ibq_vec_4_print(const ibq_vec_4_t *vec)
{
    printf("vector: ");
    for (int i = 0; i < 4; i++) {
        ibz_print(&((*vec)[i][0]), 10);
        printf("/");
        ibz_print(&((*vec)[i][1]), 10);
        printf(" ");
    }
    printf("\n\n");
}

void
ibq_reduce(ibq_t *x)
{
    ibz_t gcd, r;
    ibz_init(&gcd);
    ibz_init(&r);
    ibz_gcd(&gcd, &((*x)[0]), &((*x)[1]));
    ibz_div(&((*x)[0]), &r, &((*x)[0]), &gcd);
    assert(ibz_is_zero(&r));
    ibz_div(&((*x)[1]), &r, &((*x)[1]), &gcd);
    assert(ibz_is_zero(&r));
    ibz_finalize(&gcd);
    ibz_finalize(&r);
}

void
ibq_add(ibq_t *sum, const ibq_t *a, const ibq_t *b)
{
    ibz_t add, prod;
    ibz_init(&add);
    ibz_init(&prod);

    ibz_mul(&add, &((*a)[0]), &((*b)[1]));
    ibz_mul(&prod, &((*b)[0]), &((*a)[1]));
    ibz_add(&((*sum)[0]), &add, &prod);
    ibz_mul(&((*sum)[1]), &((*a)[1]), &((*b)[1]));
    ibz_finalize(&add);
    ibz_finalize(&prod);
}

void
ibq_neg(ibq_t *neg, const ibq_t *x)
{
    ibz_copy(&((*neg)[1]), &((*x)[1]));
    ibz_neg(&((*neg)[0]), &((*x)[0]));
}

void
ibq_sub(ibq_t *diff, const ibq_t *a, const ibq_t *b)
{
    ibq_t neg;
    ibq_init(&neg);
    ibq_neg(&neg, b);
    ibq_add(diff, a, &neg);
    ibq_finalize(&neg);
}

void
ibq_abs(ibq_t *abs, const ibq_t *x) // once
{
    ibq_t neg;
    ibq_init(&neg);
    ibq_neg(&neg, x);
    if (ibq_cmp(x, &neg) < 0)
        ibq_copy(abs, &neg);
    else
        ibq_copy(abs, x);
    ibq_finalize(&neg);
}

void
ibq_mul(ibq_t *prod, const ibq_t *a, const ibq_t *b)
{
    ibz_mul(&((*prod)[0]), &((*a)[0]), &((*b)[0]));
    ibz_mul(&((*prod)[1]), &((*a)[1]), &((*b)[1]));
}

int
ibq_inv(ibq_t *inv, const ibq_t *x)
{
    int res = !ibq_is_zero(x);
    if (res) {
        ibz_copy(&((*inv)[0]), &((*x)[0]));
        ibz_copy(&((*inv)[1]), &((*x)[1]));
        ibz_swap(&((*inv)[1]), &((*inv)[0]));
    }
    return (res);
}

int
ibq_cmp(const ibq_t *a, const ibq_t *b)
{
    ibz_t x, y;
    ibz_init(&x);
    ibz_init(&y);
    ibz_copy(&x, &((*a)[0]));
    ibz_copy(&y, &((*b)[0]));
    ibz_mul(&y, &y, &((*a)[1]));
    ibz_mul(&x, &x, &((*b)[1]));
    if (ibz_cmp(&((*a)[1]), &ibz_const_zero) > 0) {
        ibz_neg(&y, &y);
        ibz_neg(&x, &x);
    }
    if (ibz_cmp(&((*b)[1]), &ibz_const_zero) > 0) {
        ibz_neg(&y, &y);
        ibz_neg(&x, &x);
    }
    int res = ibz_cmp(&x, &y);
    ibz_finalize(&x);
    ibz_finalize(&y);
    return (res);
}

int
ibq_is_zero(const ibq_t *x)
{
    return ibz_is_zero(&((*x)[0]));
}

int
ibq_is_one(const ibq_t *x)
{
    return (0 == ibz_cmp(&((*x)[0]), &((*x)[1])));
}

int
ibq_set(ibq_t *q, const ibz_t *a, const ibz_t *b)
{
    ibz_copy(&((*q)[0]), a);
    ibz_copy(&((*q)[1]), b);
    return !ibz_is_zero(b);
}

void
ibq_copy(ibq_t *target, const ibq_t *value) // once
{
    ibz_copy(&((*target)[0]), &((*value)[0]));
    ibz_copy(&((*target)[1]), &((*value)[1]));
}

int
ibq_is_ibz(const ibq_t *q)
{
    ibz_t r;
    ibz_init(&r);
    ibz_mod(&r, &((*q)[0]), &((*q)[1]));
    int res = ibz_is_zero(&r);
    ibz_finalize(&r);
    return (res);
}

int
ibq_to_ibz(ibz_t *z, const ibq_t *q)
{
    ibz_t r;
    ibz_init(&r);
    ibz_div(z, &r, &((*q)[0]), &((*q)[1]));
    int res = ibz_is_zero(&r);
    ibz_finalize(&r);
    return (res);
}
