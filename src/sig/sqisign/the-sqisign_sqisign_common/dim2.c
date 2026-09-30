#include <quaternion.h>
#include "internal.h"

// internal helpers, also for other files
void
ibz_vec_2_set(ibz_vec_2_t *vec, int a0, int a1)
{
    assert(a0 != INT32_MIN && a1 != INT32_MIN);
    ibz_set(&(vec->v[0]), a0, 32);
    ibz_set(&(vec->v[1]), a1, 32);
}
void
ibz_mat_2x2_set(ibz_mat_2x2_t *mat, int a00, int a01, int a10, int a11)
{
    assert(a00 != INT32_MIN && a01 != INT32_MIN && a10 != INT32_MIN && a11 != INT32_MIN);
    ibz_set(&(mat->m[0][0]), a00, 32);
    ibz_set(&(mat->m[0][1]), a01, 32);
    ibz_set(&(mat->m[1][0]), a10, 32);
    ibz_set(&(mat->m[1][1]), a11, 32);
}

void
ibz_mat_2x2_copy(ibz_mat_2x2_t *copy, const ibz_mat_2x2_t *copied)
{
    ibz_copy(&(copy->m[0][0]), &(copied->m[0][0]));
    ibz_copy(&(copy->m[0][1]), &(copied->m[0][1]));
    ibz_copy(&(copy->m[1][0]), &(copied->m[1][0]));
    ibz_copy(&(copy->m[1][1]), &(copied->m[1][1]));
}

void
ibz_mat_2x2_det_from_ibz(ibz_t *det, const ibz_t *a11, const ibz_t *a12, const ibz_t *a21, const ibz_t *a22)
{
    ibz_t prod;
    ibz_init(&prod);
    ibz_mul(&prod, a12, a21);
    ibz_mul(det, a11, a22);
    ibz_sub(det, det, &prod);
}

void
ibz_mat_2x2_eval(ibz_vec_2_t *res, const ibz_mat_2x2_t *mat, const ibz_vec_2_t *vec)
{
    ibz_t prod;
    ibz_vec_2_t matvec;
    ibz_init(&prod);
    ibz_vec_2_init(&matvec);
    ibz_mul(&prod, &(mat->m[0][0]), &(vec->v[0]));
    ibz_copy(&(matvec.v[0]), &prod);
    ibz_mul(&prod, &(mat->m[0][1]), &(vec->v[1]));
    ibz_add(&(matvec.v[0]), &(matvec.v[0]), &prod);
    ibz_mul(&prod, &(mat->m[1][0]), &(vec->v[0]));
    ibz_copy(&(matvec.v[1]), &prod);
    ibz_mul(&prod, &(mat->m[1][1]), &(vec->v[1]));
    ibz_add(&(matvec.v[1]), &(matvec.v[1]), &prod);
    ibz_copy(&(res->v[0]), &(matvec.v[0]));
    ibz_copy(&(res->v[1]), &(matvec.v[1]));
}

void
ibz_mat_2x2_scalar_mul(ibz_mat_2x2_t *prod, const ibz_t *scalar, const ibz_mat_2x2_t *mat)
{
    ibz_t fact;
    ibz_init(&fact);
    ibz_copy(&fact, scalar);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_mul(&prod->m[i][j], &fact, &mat->m[i][j]);
        }
    }
}

void
ibz_mat_2x2_mul(ibz_mat_2x2_t *prod, const ibz_mat_2x2_t *mat_a, const ibz_mat_2x2_t *mat_b)
{
    ibz_t mul;
    ibz_mat_2x2_t sums;
    ibz_init(&mul);
    ibz_mat_2x2_init(&sums);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_set(&(sums.m[i][j]), 0, 0);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                ibz_mul(&mul, &(mat_a->m[i][k]), &(mat_b->m[k][j]));
                ibz_add(&(sums.m[i][j]), &(sums.m[i][j]), &mul);
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_copy(&(prod->m[i][j]), &(sums.m[i][j]));
        }
    }
}

int
ibz_mat_2x2_inv_with_det_as_denom(ibz_mat_2x2_t *inv, ibz_t *det, const ibz_mat_2x2_t *mat)
{
    ibz_t deti, prod;
    ibz_init(&deti);
    ibz_init(&prod);
    ibz_mul(&deti, &(mat->m[0][0]), &(mat->m[1][1]));
    ibz_mul(&prod, &(mat->m[0][1]), &(mat->m[1][0]));
    ibz_sub(&deti, &deti, &prod);
    int res = (0 == ibz_is_zero(&deti));
    // compute inverse
    if (res && (inv != NULL)) {
        ibz_copy(&prod, &(mat->m[0][0]));
        ibz_copy(&(inv->m[0][0]), &(mat->m[1][1]));
        ibz_copy(&(inv->m[1][1]), &prod);
        ibz_neg(&(inv->m[1][0]), &(mat->m[1][0]));
        ibz_neg(&(inv->m[0][1]), &(mat->m[0][1]));
    }
    if (det != NULL) {
        ibz_copy(det, &deti);
    }
    return (res);
}

// modular 2x2 operations

void
ibz_mat_2x2_mul_mod(ibz_mat_2x2_t *prod, const ibz_mat_2x2_t *mat_a, const ibz_mat_2x2_t *mat_b, const ibz_t *m)
{
    ibz_t mul;
    ibz_mat_2x2_t sums;
    ibz_init(&mul);
    ibz_mat_2x2_init(&sums);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_set(&(sums.m[i][j]), 0, 0);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                ibz_mul(&mul, &(mat_a->m[i][k]), &(mat_b->m[k][j]));
                ibz_add(&(sums.m[i][j]), &(sums.m[i][j]), &mul);
                ibz_mod(&(sums.m[i][j]), &(sums.m[i][j]), m);
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_copy(&(prod->m[i][j]), &(sums.m[i][j]));
        }
    }
}

int
ibz_mat_2x2_inv_mod(ibz_mat_2x2_t *inv, const ibz_mat_2x2_t *mat, const ibz_t *m)
{
    ibz_t det, prod;
    ibz_init(&det);
    ibz_init(&prod);
    ibz_mul(&det, &(mat->m[0][0]), &(mat->m[1][1]));
    ibz_mod(&det, &det, m);
    ibz_mul(&prod, &(mat->m[0][1]), &(mat->m[1][0]));
    ibz_sub(&det, &det, &prod);
    ibz_mod(&det, &det, m);
    int res = (ibz_invmod(&det, &det, m) != 0);
    // return 0 matrix if non invertible determinant
    ibz_set(&prod, res, 2);
    ibz_mul(&det, &det, &prod);
    // compute inverse
    ibz_copy(&prod, &(mat->m[0][0]));
    ibz_copy(&(inv->m[0][0]), &(mat->m[1][1]));
    ibz_copy(&(inv->m[1][1]), &prod);
    ibz_neg(&(inv->m[1][0]), &(mat->m[1][0]));
    ibz_neg(&(inv->m[0][1]), &(mat->m[0][1]));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_mul(&(inv->m[i][j]), &(inv->m[i][j]), &det);
            ibz_mod(&(inv->m[i][j]), &(inv->m[i][j]), m);
        }
    }
    return (res);
}

void
ibz_mat_2x2_normalize(ibz_mat_2x2_t *mat, int e)
{
    ibz_t tmp;
    ibz_mul_2exp(&tmp, &ibz_const_one, e - 1);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            assert(ibz_is_positive(&mat->m[i][j]));
            if (ibz_is_zero(&mat->m[i][j]))
                continue;
            int r = ibz_cmp(&mat->m[i][j], &tmp);
            if (r < 0)
                return;
            if (r > 0) {
                // negate the matrix
                ibz_mul_2exp(&tmp, &tmp, 1);
                for (int k = 0; k < 2; ++k)
                    for (int l = 0; l < 2; ++l) {
                        ibz_sub(&mat->m[k][l], &tmp, &mat->m[k][l]);
                        assert(ibz_is_positive(&mat->m[k][l]));
                    }
                return;
            }
        }
    }
    return;
}

// gaussian helpers
void
ibz_vec_2_add(ibz_vec_2_t *sum, const ibz_vec_2_t *a, const ibz_vec_2_t *b)
{
    ibz_add(&sum->v[0], &a->v[0], &b->v[0]);
    ibz_add(&sum->v[1], &a->v[1], &b->v[1]);
}
void
ibz_vec_2_sub(ibz_vec_2_t *diff, const ibz_vec_2_t *a, const ibz_vec_2_t *b)
{
    ibz_sub(&diff->v[0], &a->v[0], &b->v[0]);
    ibz_sub(&diff->v[1], &a->v[1], &b->v[1]);
}
void
ibz_vec_2_copy(ibz_vec_2_t *copy, const ibz_vec_2_t *copied)
{
    ibz_copy(&copy->v[0], &copied->v[0]);
    ibz_copy(&copy->v[1], &copied->v[1]);
}

int
ibz_vec_2_is_zero(const ibz_vec_2_t *x)
{
    return (ibz_is_zero(&x->v[0]) && ibz_is_zero(&x->v[1]));
}

int
ibz_vec_2_gaussian_is_unit(const ibz_vec_2_t *x)
{
    int res;
    ibz_t n;
    ibz_init(&n);
    ibz_sum_two_squares(&n, &x->v[0], &x->v[1]);
    res = ibz_is_one(&n);
    return (res);
}

void
ibz_vec_2_gaussian_mul(ibz_vec_2_t *prod, const ibz_vec_2_t *a, const ibz_vec_2_t *b)
{
    ibz_t r, s, t;
    ibz_init(&r);
    ibz_init(&s);
    ibz_init(&t);
    ibz_add(&r, &a->v[0], &a->v[1]);
    ibz_sub(&s, &a->v[0], &a->v[1]);
    ibz_add(&t, &b->v[0], &b->v[1]);
    ibz_mul(&r, &r, &b->v[0]);
    ibz_mul(&s, &s, &b->v[1]);
    ibz_mul(&t, &t, &a->v[1]);
    ibz_sub(&prod->v[0], &r, &t);
    ibz_add(&prod->v[1], &t, &s);
}

void
ibz_vec_2_gaussian_euclidean_division(ibz_vec_2_t *q, ibz_vec_2_t *r, const ibz_vec_2_t *a, const ibz_vec_2_t *b)
{
    ibz_t n, tmp, sum;
    ibz_vec_2_t tmp_q, prod, tmp_r;
    ibz_init(&tmp);
    ibz_init(&sum);
    ibz_init(&n);
    ibz_vec_2_init(&tmp_q);
    ibz_vec_2_init(&tmp_r);
    ibz_vec_2_init(&prod);
    ibz_sum_two_squares(&n, &b->v[0], &b->v[1]);
    ibz_vec_2_copy(&tmp_q, b);
    ibz_neg(&tmp_q.v[1], &tmp_q.v[1]);
    ibz_vec_2_gaussian_mul(&tmp_q, &tmp_q, a);
    ibz_rounded_div(&tmp_q.v[1], &tmp_q.v[1], &n);
    ibz_rounded_div(&tmp_q.v[0], &tmp_q.v[0], &n);
    ibz_vec_2_gaussian_mul(&prod, &tmp_q, b);
    ibz_vec_2_sub(&tmp_r, a, &prod);
    int max_a = ibz_get_bound(&a->v[1]);
    if (ibz_get_bound(&a->v[0]) > max_a)
        max_a = ibz_get_bound(&a->v[0]);
    ibz_set_bound(&tmp_r.v[0], ibz_get_bound(&b->v[0]));
    ibz_set_bound(&tmp_r.v[1], ibz_get_bound(&b->v[1]));
    ibz_set_bound(&tmp_q.v[0], max_a);
    ibz_set_bound(&tmp_q.v[1], max_a);
#ifndef NDEBUG
    ibz_sum_two_squares(&tmp, &tmp_r.v[0], &tmp_r.v[1]);
    assert(ibz_cmp(&tmp, &n) < 0);
    ibz_vec_2_gaussian_mul(&prod, &tmp_q, b);
    ibz_vec_2_add(&prod, &prod, &tmp_r);
    assert(ibz_cmp(&prod.v[0], &a->v[0]) == 0);
    assert(ibz_cmp(&prod.v[1], &a->v[1]) == 0);
#endif
    if (r != NULL)
        ibz_vec_2_copy(r, &tmp_r);
    if (q != NULL)
        ibz_vec_2_copy(q, &tmp_q);
}

void
ibz_vec_2_gaussian_gcd(ibz_vec_2_t *gcd, const ibz_vec_2_t *a, const ibz_vec_2_t *b)
{
    ibz_vec_2_t q, r;
    ibz_vec_2_init(&q);
    ibz_vec_2_init(&r);
    ibz_vec_2_copy(&r, b);
    ibz_vec_2_copy(&q, a);
    if (ibz_vec_2_is_zero(a)) {
        ibz_vec_2_copy(&r, a);
        ibz_vec_2_copy(&q, b);
    }
    while (!ibz_vec_2_is_zero(&r)) {
        ibz_vec_2_gaussian_euclidean_division(NULL, &q, &q, &r);
        ibz_swap(&q.v[0], &r.v[0]);
        ibz_swap(&q.v[1], &r.v[1]);
    }
    ibz_vec_2_copy(gcd, &q);
#ifndef NDEBUG
    ibz_vec_2_gaussian_euclidean_division(NULL, &q, a, gcd);
    assert(ibz_vec_2_is_zero(&q));
    ibz_vec_2_gaussian_euclidean_division(NULL, &r, b, gcd);
    assert(ibz_vec_2_is_zero(&r));
#endif
}
