#include <quaternion.h>
#include "internal.h"

// internal helper functions
void
ibz_mat_4x4_mul(ibz_mat_4x4_t *res, const ibz_mat_4x4_t *a, const ibz_mat_4x4_t *b)
{
    ibz_mat_4x4_t mat;
    ibz_t prod;
    ibz_init(&prod);
    ibz_mat_4x4_init(&mat);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&(mat.m[i][j]), 0);
            for (int k = 0; k < 4; k++) {
                ibz_mul(&prod, &(a->m[i][k]), &(b->m[k][j]));
                ibz_add(&(mat.m[i][j]), &(mat.m[i][j]), &prod);
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_copy(&(res->m[i][j]), &(mat.m[i][j]));
        }
    }
    ibz_mat_4x4_finalize(&mat);
    ibz_finalize(&prod);
}

// helper functions for lattices
void
ibz_vec_4_set(ibz_vec_4_t *vec, int32_t coord0, int32_t coord1, int32_t coord2, int32_t coord3)
{
    ibz_set(&(vec->v[0]), coord0);
    ibz_set(&(vec->v[1]), coord1);
    ibz_set(&(vec->v[2]), coord2);
    ibz_set(&(vec->v[3]), coord3);
}

void
ibz_vec_4_copy(ibz_vec_4_t *new, const ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_copy(&(new->v[i]), &(vec->v[i]));
    }
}

void
ibz_vec_4_copy_ibz(ibz_vec_4_t *res, const ibz_t *coord0, const ibz_t *coord1, const ibz_t *coord2, const ibz_t *coord3)
{
    ibz_copy(&(res->v[0]), coord0);
    ibz_copy(&(res->v[1]), coord1);
    ibz_copy(&(res->v[2]), coord2);
    ibz_copy(&(res->v[3]), coord3);
}

void
ibz_vec_4_content(ibz_t *content, const ibz_vec_4_t *v)
{
    ibz_gcd(content, &(v->v[0]), &(v->v[1]));
    ibz_gcd(content, &(v->v[2]), content);
    ibz_gcd(content, &(v->v[3]), content);
}

void
ibz_vec_4_negate(ibz_vec_4_t *neg, const ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_neg(&(neg->v[i]), &(vec->v[i]));
    }
}

void
ibz_vec_4_add(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b)
{
    ibz_add(&(res->v[0]), &(a->v[0]), &(b->v[0]));
    ibz_add(&(res->v[1]), &(a->v[1]), &(b->v[1]));
    ibz_add(&(res->v[2]), &(a->v[2]), &(b->v[2]));
    ibz_add(&(res->v[3]), &(a->v[3]), &(b->v[3]));
}

void
ibz_vec_4_sub(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b)
{
    ibz_sub(&(res->v[0]), &(a->v[0]), &(b->v[0]));
    ibz_sub(&(res->v[1]), &(a->v[1]), &(b->v[1]));
    ibz_sub(&(res->v[2]), &(a->v[2]), &(b->v[2]));
    ibz_sub(&(res->v[3]), &(a->v[3]), &(b->v[3]));
}

int
ibz_vec_4_is_zero(const ibz_vec_4_t *x)
{
    int res = 1;
    for (int i = 0; i < 4; i++) {
        res &= ibz_is_zero(&(x->v[i]));
    }
    return (res);
}

void
ibz_vec_4_linear_combination(ibz_vec_4_t *lc,
                             const ibz_t *coeff_a,
                             const ibz_vec_4_t *vec_a,
                             const ibz_t *coeff_b,
                             const ibz_vec_4_t *vec_b)
{
    ibz_t prod;
    ibz_vec_4_t sums;
    ibz_vec_4_init(&sums);
    ibz_init(&prod);
    for (int i = 0; i < 4; i++) {
        ibz_mul(&(sums.v[i]), coeff_a, &(vec_a->v[i]));
        ibz_mul(&prod, coeff_b, &(vec_b->v[i]));
        ibz_add(&(sums.v[i]), &(sums.v[i]), &prod);
    }
    for (int i = 0; i < 4; i++) {
        ibz_copy(&(lc->v[i]), &(sums.v[i]));
    }
    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sums);
}

void
ibz_vec_4_scalar_mul(ibz_vec_4_t *prod, const ibz_t *scalar, const ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_mul(&(prod->v[i]), &(vec->v[i]), scalar);
    }
}

int
ibz_vec_4_scalar_div(ibz_vec_4_t *quot, const ibz_t *scalar, const ibz_vec_4_t *vec)
{
    int res = 1;
    ibz_t r;
    ibz_init(&r);
    for (int i = 0; i < 4; i++) {
        ibz_div(&(quot->v[i]), &r, &(vec->v[i]), scalar);
        res = res && ibz_is_zero(&r);
    }
    ibz_finalize(&r);
    return (res);
}

void
ibz_mat_4x4_copy(ibz_mat_4x4_t *new, const ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_copy(&(new->m[i][j]), &(mat->m[i][j]));
        }
    }
}

void
ibz_mat_4x4_negate(ibz_mat_4x4_t *neg, const ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_neg(&(neg->m[i][j]), &(mat->m[i][j]));
        }
    }
}

void
ibz_mat_4x4_transpose(ibz_mat_4x4_t *transposed, const ibz_mat_4x4_t *mat)
{
    ibz_mat_4x4_t work;
    ibz_mat_4x4_init(&work);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_copy(&(work.m[i][j]), &(mat->m[j][i]));
        }
    }
    ibz_mat_4x4_copy(transposed, &work);
    ibz_mat_4x4_finalize(&work);
}

void
ibz_mat_4x4_zero(ibz_mat_4x4_t *zero)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&(zero->m[i][j]), 0);
        }
    }
}

void
ibz_mat_4x4_identity(ibz_mat_4x4_t *id)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&(id->m[i][j]), 0);
        }
        ibz_set(&(id->m[i][i]), 1);
    }
}

int
ibz_mat_4x4_is_identity(const ibz_mat_4x4_t *mat)
{
    int res = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res = res && ibz_is_one(&(mat->m[i][j])) == (i == j);
        }
    }
    return (res);
}

int
ibz_mat_4x4_equal(const ibz_mat_4x4_t *mat1, const ibz_mat_4x4_t *mat2)
{
    int res = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res = res | ibz_cmp(&(mat1->m[i][j]), &(mat2->m[i][j]));
        }
    }
    return (!res);
}

void
ibz_mat_4x4_scalar_mul(ibz_mat_4x4_t *prod, const ibz_t *scalar, const ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_mul(&(prod->m[i][j]), &(mat->m[i][j]), scalar);
        }
    }
}

void
ibz_mat_4x4_gcd(ibz_t *gcd, const ibz_mat_4x4_t *mat)
{
    ibz_t d;
    ibz_init(&d);
    ibz_copy(&d, &(mat->m[0][0]));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_gcd(&d, &d, &(mat->m[i][j]));
        }
    }
    ibz_copy(gcd, &d);
    ibz_finalize(&d);
}

int
ibz_mat_4x4_scalar_div(ibz_mat_4x4_t *quot, const ibz_t *scalar, const ibz_mat_4x4_t *mat)
{
    int res = 1;
    ibz_t r;
    ibz_init(&r);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_div(&(quot->m[i][j]), &r, &(mat->m[i][j]), scalar);
            res = res && ibz_is_zero(&r);
        }
    }
    ibz_finalize(&r);
    return (res);
}

// 4x4 inversion helper functions
void
ibz_inv_dim4_make_coeff_pmp(ibz_t *coeff,
                            const ibz_t *a1,
                            const ibz_t *a2,
                            const ibz_t *b1,
                            const ibz_t *b2,
                            const ibz_t *c1,
                            const ibz_t *c2)
{
    ibz_t prod, sum;
    ibz_init(&prod);
    ibz_init(&sum);
    ibz_mul(&sum, a1, a2);
    ibz_mul(&prod, b1, b2);
    ibz_sub(&sum, &sum, &prod);
    ibz_mul(&prod, c1, c2);
    ibz_add(coeff, &sum, &prod);
    ibz_finalize(&prod);
    ibz_finalize(&sum);
}

void
ibz_inv_dim4_make_coeff_mpm(ibz_t *coeff,
                            const ibz_t *a1,
                            const ibz_t *a2,
                            const ibz_t *b1,
                            const ibz_t *b2,
                            const ibz_t *c1,
                            const ibz_t *c2)
{
    ibz_t prod, sum;
    ibz_init(&prod);
    ibz_init(&sum);
    ibz_mul(&sum, b1, b2);
    ibz_mul(&prod, a1, a2);
    ibz_sub(&sum, &sum, &prod);
    ibz_mul(&prod, c1, c2);
    ibz_sub(coeff, &sum, &prod);
    ibz_finalize(&prod);
    ibz_finalize(&sum);
}

// Method from https://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf 3rd of May
// 2023, 16h15 CEST
int
ibz_mat_4x4_inv_with_det_as_denom(ibz_mat_4x4_t *inv, ibz_t *det, const ibz_mat_4x4_t *mat)
{
    ibz_t prod, work_det;
    ibz_mat_4x4_t work;
    ibz_t s[6];
    ibz_t c[6];
    for (int i = 0; i < 6; i++) {
        ibz_init(&(s[i]));
        ibz_init(&(c[i]));
    }
    ibz_mat_4x4_init(&work);
    ibz_init(&prod);
    ibz_init(&work_det);

    // compute some 2x2 minors, store them in s and c
    for (int i = 0; i < 3; i++) {
        ibz_mat_2x2_det_from_ibz(&(s[i]), &(mat->m[0][0]), &(mat->m[0][i + 1]), &(mat->m[1][0]), &(mat->m[1][i + 1]));
        ibz_mat_2x2_det_from_ibz(&(c[i]), &(mat->m[2][0]), &(mat->m[2][i + 1]), &(mat->m[3][0]), &(mat->m[3][i + 1]));
    }
    for (int i = 0; i < 2; i++) {
        ibz_mat_2x2_det_from_ibz(
            &(s[3 + i]), &(mat->m[0][1]), &(mat->m[0][2 + i]), &(mat->m[1][1]), &(mat->m[1][2 + i]));
        ibz_mat_2x2_det_from_ibz(
            &(c[3 + i]), &(mat->m[2][1]), &(mat->m[2][2 + i]), &(mat->m[3][1]), &(mat->m[3][2 + i]));
    }
    ibz_mat_2x2_det_from_ibz(&(s[5]), &(mat->m[0][2]), &(mat->m[0][3]), &(mat->m[1][2]), &(mat->m[1][3]));
    ibz_mat_2x2_det_from_ibz(&(c[5]), &(mat->m[2][2]), &(mat->m[2][3]), &(mat->m[3][2]), &(mat->m[3][3]));

    // compute det
    ibz_set(&work_det, 0);
    for (int i = 0; i < 6; i++) {
        ibz_mul(&prod, &(s[i]), &(c[5 - i]));
        if ((i != 1) && (i != 4)) {
            ibz_add(&work_det, &work_det, &prod);
        } else {
            ibz_sub(&work_det, &work_det, &prod);
        }
    }
    // compute transposed adjugate
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 2; k++) {
            if ((k + j + 1) % 2 == 1) {
                ibz_inv_dim4_make_coeff_pmp(&(work.m[j][k]),
                                            &(mat->m[1 - k][(j == 0)]),
                                            &(c[6 - j - (j == 0)]),
                                            &(mat->m[1 - k][2 - (j > 1)]),
                                            &(c[4 - j - (j == 1)]),
                                            &(mat->m[1 - k][3 - (j == 3)]),
                                            &(c[3 - j - (j == 1) - (j == 2)]));
            } else {
                ibz_inv_dim4_make_coeff_mpm(&(work.m[j][k]),
                                            &(mat->m[1 - k][(j == 0)]),
                                            &(c[6 - j - (j == 0)]),
                                            &(mat->m[1 - k][2 - (j > 1)]),
                                            &(c[4 - j - (j == 1)]),
                                            &(mat->m[1 - k][3 - (j == 3)]),
                                            &(c[3 - j - (j == 1) - (j == 2)]));
            }
        }
        for (int k = 2; k < 4; k++) {
            if ((k + j + 1) % 2 == 1) {
                ibz_inv_dim4_make_coeff_pmp(&(work.m[j][k]),
                                            &(mat->m[3 - (k == 3)][(j == 0)]),
                                            &(s[6 - j - (j == 0)]),
                                            &(mat->m[3 - (k == 3)][2 - (j > 1)]),
                                            &(s[4 - j - (j == 1)]),
                                            &(mat->m[3 - (k == 3)][3 - (j == 3)]),
                                            &(s[3 - j - (j == 1) - (j == 2)]));
            } else {
                ibz_inv_dim4_make_coeff_mpm(&(work.m[j][k]),
                                            &(mat->m[3 - (k == 3)][(j == 0)]),
                                            &(s[6 - j - (j == 0)]),
                                            &(mat->m[3 - (k == 3)][2 - (j > 1)]),
                                            &(s[4 - j - (j == 1)]),
                                            &(mat->m[3 - (k == 3)][3 - (j == 3)]),
                                            &(s[3 - j - (j == 1) - (j == 2)]));
            }
        }
    }
    if (inv != NULL) {
        // put transposed adjugate in result, or 0 if no inverse
        ibz_set(&prod, !ibz_is_zero(&work_det));
        ibz_mat_4x4_scalar_mul(inv, &prod, &work);
    }
    // output det
    if (det != NULL)
        ibz_copy(det, &work_det);
    for (int i = 0; i < 6; i++) {
        ibz_finalize(&s[i]);
        ibz_finalize(&c[i]);
    }
    ibz_mat_4x4_finalize(&work);
    ibz_finalize(&work_det);
    ibz_finalize(&prod);
    return (!ibz_is_zero(det));
}

// matrix evaluation

void
ibz_mat_4x4_eval(ibz_vec_4_t *res, const ibz_mat_4x4_t *mat, const ibz_vec_4_t *vec)
{
    ibz_vec_4_t sum;
    ibz_t prod;
    ibz_init(&prod);
    ibz_vec_4_init(&sum);
    // assume initialization to 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_mul(&prod, &mat->m[i][j], &vec->v[j]);
            ibz_add(&(sum.v[i]), &(sum.v[i]), &prod);
        }
    }
    ibz_vec_4_copy(res, &sum);
    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sum);
}

void
ibz_mat_4x4_eval_t(ibz_vec_4_t *res, const ibz_vec_4_t *vec, const ibz_mat_4x4_t *mat)
{
    ibz_vec_4_t sum;
    ibz_t prod;
    ibz_init(&prod);
    ibz_vec_4_init(&sum);
    // assume initialization to 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_mul(&prod, &mat->m[j][i], &vec->v[j]);
            ibz_add(&(sum.v[i]), &(sum.v[i]), &prod);
        }
    }
    ibz_vec_4_copy(res, &sum);
    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sum);
}

// quadratic forms

void
quat_qf_eval(ibz_t *res, const ibz_mat_4x4_t *qf, const ibz_vec_4_t *coord)
{
    ibz_vec_4_t sum;
    ibz_t prod;
    ibz_init(&prod);
    ibz_vec_4_init(&sum);
    ibz_mat_4x4_eval(&sum, qf, coord);
    for (int i = 0; i < 4; i++) {
        ibz_mul(&prod, &(sum.v[i]), &coord->v[i]);
        if (i > 0) {
            ibz_add(&(sum.v[0]), &(sum.v[0]), &prod);
        } else {
            ibz_copy(&sum.v[0], &prod);
        }
    }
    ibz_copy(res, &sum.v[0]);
    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sum);
}
