#include <quaternion.h>
#include <rng.h>
#include <stdio.h>
#include "internal.h"
#include "lll_internals.h"

int
quat_lattice_bound_parallelogram(ibz_vec_4_t *box, ibz_mat_4x4_t *U, const ibz_mat_4x4_t *G, const ibz_t *radius)
{
    ibz_t denom, rem;
    ibz_init(&denom);
    ibz_init(&rem);
    ibz_mat_4x4_t dualG;
    ibz_mat_4x4_init(&dualG);

// Compute the Gram matrix of the dual lattice
#ifndef NDEBUG
    int inv_check = ibz_mat_4x4_inv_with_det_as_denom(&dualG, &denom, G);
    assert(inv_check);
#else
    (void)ibz_mat_4x4_inv_with_det_as_denom(&dualG, &denom, G);
#endif
    // Initialize the dual lattice basis to the identity matrix
    ibz_mat_4x4_identity(U);
    // Reduce the dual lattice
    quat_lll_core(&dualG, U);

    // Compute the parallelogram's bounds
    int trivial = 1;
    for (int i = 0; i < 4; i++) {
        ibz_mul(&(*box)[i], &dualG[i][i], radius);
        ibz_div(&(*box)[i], &rem, &(*box)[i], &denom);
        ibz_sqrt_floor(&(*box)[i], &(*box)[i]);
        trivial &= ibz_is_zero(&(*box)[i]);
    }

    // Compute the transpose transformation matrix
#ifndef NDEBUG
    int inv = ibz_mat_4x4_inv_with_det_as_denom(U, &denom, U);
#else
    (void)ibz_mat_4x4_inv_with_det_as_denom(U, &denom, U);
#endif
    // U is unitary, det(U) = ± 1
    ibz_mat_4x4_scalar_mul(U, &denom, U);
#ifndef NDEBUG
    assert(inv);
    ibz_abs(&denom, &denom);
    assert(ibz_is_one(&denom));
#endif

    ibz_mat_4x4_finalize(&dualG);
    ibz_finalize(&denom);
    ibz_finalize(&rem);
    return !trivial;
}

int
quat_lattice_sample_from_ball(quat_alg_elem_t *res,
                              const quat_lattice_t *lattice,
                              const quat_alg_t *alg,
                              const ibz_t *radius)
{
    assert(ibz_cmp(radius, &ibz_const_zero) > 0);

    ibz_vec_4_t box;
    ibz_vec_4_init(&box);
    ibz_mat_4x4_t U, G;
    ibz_mat_4x4_init(&U);
    ibz_mat_4x4_init(&G);
    ibz_vec_4_t x;
    ibz_vec_4_init(&x);
    ibz_t rad, tmp;
    ibz_init(&rad);
    ibz_init(&tmp);

    // Compute the Gram matrix of the lattice
    quat_lattice_gram(&G, lattice, alg);

    // Correct ball radius by the denominator
    ibz_mul(&rad, radius, &lattice->denom);
    ibz_mul(&rad, &rad, &lattice->denom);
    // Correct by 2 (Gram matrix corresponds to twice the norm)
    ibz_mul(&rad, &rad, &ibz_const_two);

    // Compute a bounding parallelogram for the ball, stop if it only
    // contains the origin
    int ok = quat_lattice_bound_parallelogram(&box, &U, &G, &rad);
    if (!ok)
        goto err;

    // Rejection sampling from the parallelogram
#ifndef NDEBUG
    int cnt = 0;
#endif
    do {
        // Sample vector
        for (int i = 0; i < 4; i++) {
            if (ibz_is_zero(&box[i])) {
                ibz_copy(&x[i], &ibz_const_zero);
            } else {
                ibz_add(&tmp, &box[i], &box[i]);
                ok &= ibz_rand_interval(&x[i], &ibz_const_zero, &tmp);
                ibz_sub(&x[i], &x[i], &box[i]);
                if (!ok)
                    goto err;
            }
        }
        // Map to parallelogram
        ibz_mat_4x4_eval_t(&x, &x, &U);
        // Evaluate quadratic form
        quat_qf_eval(&tmp, &G, &x);
#ifndef NDEBUG
        cnt++;
        if (cnt % 100 == 0)
            printf("Lattice sampling rejected %d times", cnt - 1);
#endif
    } while (ibz_is_zero(&tmp) || (ibz_cmp(&tmp, &rad) > 0));

    // Evaluate linear combination
    ibz_mat_4x4_eval(&(res->coord), &(lattice->basis), &x);
    ibz_copy(&(res->denom), &(lattice->denom));
    quat_alg_normalize(res);

#ifndef NDEBUG
    // Check norm is smaller than radius
    quat_alg_norm(&tmp, &rad, res, alg);
    ibz_mul(&rad, &rad, radius);
    assert(ibz_cmp(&tmp, &rad) <= 0);
#endif

err:
    ibz_finalize(&rad);
    ibz_finalize(&tmp);
    ibz_vec_4_finalize(&x);
    ibz_mat_4x4_finalize(&U);
    ibz_mat_4x4_finalize(&G);
    ibz_vec_4_finalize(&box);
    return ok;
}
