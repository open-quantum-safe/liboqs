#include "theta_structure.h"
#include <assert.h>

void
theta_extract_dual_null_point(theta_point_t *dual_null_point, theta_structure_t *A)
{
    // Reminder, inv_dual_null_point is
    // A^{-1} = CzBxDw = (BCD)(xzw)
    // C^{-1} = AzBxDw = (ABD)(xzw)
    // B^{-1} = AxCzDw = (ACD)(xzw)
    // D^{-1} = AxCzBw = (ABC)(xzw)
    // and dbl_data := (Az, Ax, Bx, Bw), so
    // C = AxCzBw = C(ABxzw) = D^{-1}
    // D = AzBxDw = D(ABxzw) = C^{-1}
    // A = AzAxBw = A(ABxzw) = Az * Ax * Bw
    // B = AzBxBw = B(ABxzw) = Az * Bx * Bw

    fp2_t tmp;
    fp2_copy(&dual_null_point->t, &A->inv_dual_null_point.z);
    fp2_copy(&dual_null_point->z, &A->inv_dual_null_point.t);
    fp2_mul(&tmp, &A->dbl_data.x, &A->dbl_data.t);
    fp2_mul(&dual_null_point->x, &tmp, &A->dbl_data.y);
    fp2_mul(&dual_null_point->y, &tmp, &A->dbl_data.z);
}

void
theta_precomputation(theta_structure_t *A)
{
    if (A->precomputation) {
        return;
    }

    theta_point_t dual_null_point;
    theta_extract_dual_null_point(&dual_null_point, A);
    to_squared_theta(&dual_null_point, &dual_null_point);
    theta_invert_point(&A->inv_sqr_null_point, &dual_null_point);

    A->precomputation = true;
}

void
theta_DBL(theta_point_t *out, theta_structure_t *A, const theta_point_t *in)
{
    if (!A->precomputation) {
        theta_precomputation(A);
    }
    to_squared_theta(out, in);
    pointwise_square(out, out);

    pointwise_product(out, out, &A->inv_sqr_null_point);
    theta_hadamard(out, out);
    pointwise_product(out, out, &A->inv_dual_null_point);
}

void
theta_DBL_iter(theta_point_t *out, theta_structure_t *A, const theta_point_t *in, uint16_t exp)
{
    if (exp == 0) {
        *out = *in;
    } else {
        theta_DBL(out, A, in);
        for (uint16_t i = 1; i < exp; i++) {
            theta_DBL(out, A, out);
        }
    }
}

uint32_t
theta_is_product_theta_point(const theta_point_t *P)
{
    fp2_t t1, t2;
    fp2_mul(&t1, &P->x, &P->t);
    fp2_mul(&t2, &P->y, &P->z);
    return fp2_is_equal(&t1, &t2);
}
