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
            ibz_set(&(mat[i][j]), 0);
            for (int k = 0; k < 4; k++) {
                ibz_mul(&prod, &((*a)[i][k]), &((*b)[k][j]));
                ibz_add(&(mat[i][j]), &(mat[i][j]), &prod);
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_copy(&((*res)[i][j]), &(mat[i][j]));
        }
    }
    ibz_mat_4x4_finalize(&mat);
    ibz_finalize(&prod);
}

// helper functions for lattices
void
ibz_vec_4_set(ibz_vec_4_t *vec, int32_t coord0, int32_t coord1, int32_t coord2, int32_t coord3)
{
    ibz_set(&((*vec)[0]), coord0);
    ibz_set(&((*vec)[1]), coord1);
    ibz_set(&((*vec)[2]), coord2);
    ibz_set(&((*vec)[3]), coord3);
}

void
ibz_vec_4_copy(ibz_vec_4_t *new, const ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_copy(&((*new)[i]), &((*vec)[i]));
    }
}

void
ibz_vec_4_copy_ibz(ibz_vec_4_t *res, const ibz_t *coord0, const ibz_t *coord1, const ibz_t *coord2, const ibz_t *coord3)
{
    ibz_copy(&((*res)[0]), coord0);
    ibz_copy(&((*res)[1]), coord1);
    ibz_copy(&((*res)[2]), coord2);
    ibz_copy(&((*res)[3]), coord3);
}

void
ibz_vec_4_content(ibz_t *content, const ibz_vec_4_t *v)
{
    ibz_gcd(content, &((*v)[0]), &((*v)[1]));
    ibz_gcd(content, &((*v)[2]), content);
    ibz_gcd(content, &((*v)[3]), content);
}

void
ibz_vec_4_negate(ibz_vec_4_t *neg, const ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_neg(&((*neg)[i]), &((*vec)[i]));
    }
}

void
ibz_vec_4_add(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b)
{
    ibz_add(&((*res)[0]), &((*a)[0]), &((*b)[0]));
    ibz_add(&((*res)[1]), &((*a)[1]), &((*b)[1]));
    ibz_add(&((*res)[2]), &((*a)[2]), &((*b)[2]));
    ibz_add(&((*res)[3]), &((*a)[3]), &((*b)[3]));
}

void
ibz_vec_4_sub(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b)
{
    ibz_sub(&((*res)[0]), &((*a)[0]), &((*b)[0]));
    ibz_sub(&((*res)[1]), &((*a)[1]), &((*b)[1]));
    ibz_sub(&((*res)[2]), &((*a)[2]), &((*b)[2]));
    ibz_sub(&((*res)[3]), &((*a)[3]), &((*b)[3]));
}

int
ibz_vec_4_is_zero(const ibz_vec_4_t *x)
{
    int res = 1;
    for (int i = 0; i < 4; i++) {
        res &= ibz_is_zero(&((*x)[i]));
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
        ibz_mul(&(sums[i]), coeff_a, &((*vec_a)[i]));
        ibz_mul(&prod, coeff_b, &((*vec_b)[i]));
        ibz_add(&(sums[i]), &(sums[i]), &prod);
    }
    for (int i = 0; i < 4; i++) {
        ibz_copy(&((*lc)[i]), &(sums[i]));
    }
    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sums);
}

void
ibz_vec_4_scalar_mul(ibz_vec_4_t *prod, const ibz_t *scalar, const ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_mul(&((*prod)[i]), &((*vec)[i]), scalar);
    }
}

int
ibz_vec_4_scalar_div(ibz_vec_4_t *quot, const ibz_t *scalar, const ibz_vec_4_t *vec)
{
    int res = 1;
    ibz_t r;
    ibz_init(&r);
    for (int i = 0; i < 4; i++) {
        ibz_div(&((*quot)[i]), &r, &((*vec)[i]), scalar);
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
            ibz_copy(&((*new)[i][j]), &((*mat)[i][j]));
        }
    }
}

void
ibz_mat_4x4_negate(ibz_mat_4x4_t *neg, const ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_neg(&((*neg)[i][j]), &((*mat)[i][j]));
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
            ibz_copy(&(work[i][j]), &((*mat)[j][i]));
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
            ibz_set(&((*zero)[i][j]), 0);
        }
    }
}

void
ibz_mat_4x4_identity(ibz_mat_4x4_t *id)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&((*id)[i][j]), 0);
        }
        ibz_set(&((*id)[i][i]), 1);
    }
}

int
ibz_mat_4x4_is_identity(const ibz_mat_4x4_t *mat)
{
    int res = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res = res && ibz_is_one(&((*mat)[i][j])) == (i == j);
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
            res = res | ibz_cmp(&((*mat1)[i][j]), &((*mat2)[i][j]));
        }
    }
    return (!res);
}

void
ibz_mat_4x4_scalar_mul(ibz_mat_4x4_t *prod, const ibz_t *scalar, const ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_mul(&((*prod)[i][j]), &((*mat)[i][j]), scalar);
        }
    }
}

void
ibz_mat_4x4_gcd(ibz_t *gcd, const ibz_mat_4x4_t *mat)
{
    ibz_t d;
    ibz_init(&d);
    ibz_copy(&d, &((*mat)[0][0]));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_gcd(&d, &d, &((*mat)[i][j]));
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
            ibz_div(&((*quot)[i][j]), &r, &((*mat)[i][j]), scalar);
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
        ibz_mat_2x2_det_from_ibz(&(s[i]), &((*mat)[0][0]), &((*mat)[0][i + 1]), &((*mat)[1][0]), &((*mat)[1][i + 1]));
        ibz_mat_2x2_det_from_ibz(&(c[i]), &((*mat)[2][0]), &((*mat)[2][i + 1]), &((*mat)[3][0]), &((*mat)[3][i + 1]));
    }
    for (int i = 0; i < 2; i++) {
        ibz_mat_2x2_det_from_ibz(
            &(s[3 + i]), &((*mat)[0][1]), &((*mat)[0][2 + i]), &((*mat)[1][1]), &((*mat)[1][2 + i]));
        ibz_mat_2x2_det_from_ibz(
            &(c[3 + i]), &((*mat)[2][1]), &((*mat)[2][2 + i]), &((*mat)[3][1]), &((*mat)[3][2 + i]));
    }
    ibz_mat_2x2_det_from_ibz(&(s[5]), &((*mat)[0][2]), &((*mat)[0][3]), &((*mat)[1][2]), &((*mat)[1][3]));
    ibz_mat_2x2_det_from_ibz(&(c[5]), &((*mat)[2][2]), &((*mat)[2][3]), &((*mat)[3][2]), &((*mat)[3][3]));

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
                ibz_inv_dim4_make_coeff_pmp(&(work[j][k]),
                                            &((*mat)[1 - k][(j == 0)]),
                                            &(c[6 - j - (j == 0)]),
                                            &((*mat)[1 - k][2 - (j > 1)]),
                                            &(c[4 - j - (j == 1)]),
                                            &((*mat)[1 - k][3 - (j == 3)]),
                                            &(c[3 - j - (j == 1) - (j == 2)]));
            } else {
                ibz_inv_dim4_make_coeff_mpm(&(work[j][k]),
                                            &((*mat)[1 - k][(j == 0)]),
                                            &(c[6 - j - (j == 0)]),
                                            &((*mat)[1 - k][2 - (j > 1)]),
                                            &(c[4 - j - (j == 1)]),
                                            &((*mat)[1 - k][3 - (j == 3)]),
                                            &(c[3 - j - (j == 1) - (j == 2)]));
            }
        }
        for (int k = 2; k < 4; k++) {
            if ((k + j + 1) % 2 == 1) {
                ibz_inv_dim4_make_coeff_pmp(&(work[j][k]),
                                            &((*mat)[3 - (k == 3)][(j == 0)]),
                                            &(s[6 - j - (j == 0)]),
                                            &((*mat)[3 - (k == 3)][2 - (j > 1)]),
                                            &(s[4 - j - (j == 1)]),
                                            &((*mat)[3 - (k == 3)][3 - (j == 3)]),
                                            &(s[3 - j - (j == 1) - (j == 2)]));
            } else {
                ibz_inv_dim4_make_coeff_mpm(&(work[j][k]),
                                            &((*mat)[3 - (k == 3)][(j == 0)]),
                                            &(s[6 - j - (j == 0)]),
                                            &((*mat)[3 - (k == 3)][2 - (j > 1)]),
                                            &(s[4 - j - (j == 1)]),
                                            &((*mat)[3 - (k == 3)][3 - (j == 3)]),
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
            ibz_mul(&prod, &(*mat)[i][j], &(*vec)[j]);
            ibz_add(&(sum[i]), &(sum[i]), &prod);
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
            ibz_mul(&prod, &(*mat)[j][i], &(*vec)[j]);
            ibz_add(&(sum[i]), &(sum[i]), &prod);
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
        ibz_mul(&prod, &(sum[i]), &(*coord)[i]);
        if (i > 0) {
            ibz_add(&(sum[0]), &(sum[0]), &prod);
        } else {
            ibz_copy(&sum[0], &prod);
        }
    }
    ibz_copy(res, &sum[0]);
    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sum);
}
