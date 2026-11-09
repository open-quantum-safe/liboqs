#include "internal.h"
#include <rng.h>
#include <stdio.h>

int
ibz_mat_4x4_is_triangular(const ibz_mat_4x4_t *mat)
{
    int res = 1;
    res = res && ibz_is_zero(&mat->m[1][0]);
    res = res && ibz_is_zero(&mat->m[2][0]);
    res = res && ibz_is_zero(&mat->m[2][1]);
    res = res && ibz_is_zero(&mat->m[3][0]);
    res = res && ibz_is_zero(&mat->m[3][1]);
    res = res && ibz_is_zero(&mat->m[3][2]);
    return (res);
}

int
ibz_mat_4x4_is_hnf(const ibz_mat_4x4_t *mat)
{
    int res = 1;
    int found;
    int ind = 0;
    ibz_t zero;
    ibz_init(&zero);
    ibz_set(&zero, 0, 0);
    // upper triangular
    for (int i = 0; i < 4; i++) {
        // upper triangular
        for (int j = 0; j < i; j++) {
            res = res && ibz_is_zero(&(mat->m[i][j]));
        }
        // find first non 0 element of line
        found = 0;
        for (int j = i; j < 4; j++) {
            if (found) {
                // all values are positive, and first non-0 is the largest of that line
                res = res && (ibz_cmp(&(mat->m[i][j]), &zero) >= 0);
                res = res && (ibz_cmp(&(mat->m[i][ind]), &(mat->m[i][j])) > 0);
            } else {
                if (!ibz_is_zero(&(mat->m[i][j]))) {
                    found = 1;
                    ind = j;
                    // mustbe non-negative
                    res = res && (ibz_cmp(&(mat->m[i][j]), &zero) > 0);
                }
            }
        }
    }
    // check that first nom-zero elements ndex per column is strictly increasing
    int linestart = -1;
    int i = 0;
    for (int j = 0; j < 4; j++) {
        while ((i < 4) && (ibz_is_zero(&(mat->m[i][j])))) {
            i = i + 1;
        }
        if (i != 4) {
            res = res && (linestart < i);
        }
        i = 0;
    }
    return (res);
}

void
quat_lattice_reduce_denom(quat_lattice_t *reduced, const quat_lattice_t *lat)
{
    ibz_t gcd;
    ibz_init(&gcd);
    ibz_mat_4x4_gcd(&gcd, &(lat->basis));
    ibz_gcd(&gcd, &gcd, &(lat->denom));
    ibz_mat_4x4_scalar_div(&(reduced->basis), &gcd, &(lat->basis));
    ibz_div(&(reduced->denom), &gcd, &(lat->denom), &gcd);
    ibz_abs(&(reduced->denom), &(reduced->denom));
}

// check if element is in quat_lattice_t lat. Requires lat to have a triangular basis.
int
quat_lattice_contains(ibz_vec_4_t *coords, const quat_lattice_t *lat, const quat_alg_elem_t *elem)
{
    assert(ibz_mat_4x4_is_triangular(&lat->basis));
    int UNUSED full_rank = !ibz_is_zero(&lat->basis.m[0][0]) && !ibz_is_zero(&lat->basis.m[1][1]) &&
                           !ibz_is_zero(&lat->basis.m[2][2]) && !ibz_is_zero(&lat->basis.m[3][3]);
    assert(full_rank);
    int res = 1;
    ibz_t r;
    ibz_vec_4_t replace_coords, elem_vec, tmp;
    ibz_mat_4x4_t prod;
    ibz_init(&r);
    ibz_mat_4x4_init(&prod);
    ibz_vec_4_init(&tmp);
    ibz_vec_4_init(&elem_vec);
    ibz_vec_4_init(&replace_coords);
    ibz_vec_4_scalar_mul(&elem_vec, &lat->denom, &elem->coord);
    ibz_mat_4x4_scalar_mul(&prod, &elem->denom, &lat->basis);
    for (int i = 3; i > -1; i--) {
        ibz_div(&replace_coords.v[i], &r, &elem_vec.v[i], &prod.m[i][i]);
        res = res && ibz_is_zero(&r);
        ibz_vec_4_copy_ibz(&tmp, &prod.m[0][i], &prod.m[1][i], &prod.m[2][i], &prod.m[3][i]);
        ibz_vec_4_scalar_mul(&tmp, &replace_coords.v[i], &tmp);
        ibz_vec_4_sub(&elem_vec, &elem_vec, &tmp);
    }
    if (coords != NULL)
        ibz_vec_4_copy(coords, &replace_coords);
    return (res);
}

// check inclusion sublat < overlat. Requires overlat to have a triagular basis.
int
quat_lattice_inclusion(const quat_lattice_t *sublat, const quat_lattice_t *overlat)
{
    int res = 1;
    quat_alg_elem_t elem;
    quat_alg_elem_init(&elem);
    ibz_copy(&elem.denom, &sublat->denom);
    for (int i = 0; i < 4; i++) {
        ibz_vec_4_copy_ibz(&elem.coord,
                           &sublat->basis.m[0][i],
                           &sublat->basis.m[1][i],
                           &sublat->basis.m[2][i],
                           &sublat->basis.m[3][i]);
        res = res && quat_lattice_contains(NULL, overlat, &elem);
    }
    return (res);
}

int
quat_lattice_equal(const quat_lattice_t *a, const quat_lattice_t *b)
{
    return (quat_lattice_inclusion(a, b) && quat_lattice_inclusion(b, a));
}

void
quat_lattice_gram(ibz_mat_4x4_t *G, const quat_lattice_t *lattice)
{
    ibz_t tmp;
    ibz_init(&tmp);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) {
            ibz_set(&G->m[i][j], 0, 0);
            for (int k = 0; k < 4; k++) {
                ibz_mul(&tmp, &(lattice->basis).m[k][i], &(lattice->basis).m[k][j]);
                if (k >= 2)
                    ibz_mul(&tmp, &tmp, &QUATALG_PINFTY.p);
                ibz_add(&G->m[i][j], &G->m[i][j], &tmp);
            }
            ibz_mul(&G->m[i][j], &G->m[i][j], &ibz_const_two);
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            ibz_copy(&G->m[i][j], &G->m[j][i]);
        }
    }
}

// is the input basis reduced or not?
int
quat_lattice_sample_from_ball(quat_alg_elem_t *res,
                              ibz_t *res_ideal_norm,
                              const quat_lattice_t *lattice,
                              const ibz_t *radius,
                              prng_domain_ctx_t *prng_domain)
{
    assert(ibz_cmp(radius, &ibz_const_zero) > 0);

    ibz_vec_4_t box;
    quat_lattice_t lat;
    quat_lattice_init(&lat);
    ibz_vec_4_init(&box);
    ibz_mat_4x4_t U, G;
    ibz_mat_4x4_init(&U);
    ibz_mat_4x4_init(&G);
    ibz_vec_4_t x;
    ibz_vec_4_init(&x);
    ibz_t rad, tmp, d, d2;
    ibz_init(&rad);
    ibz_init(&tmp);
    ibz_init(&d2);
    ibz_init(&d);
    ibz_mat_4x4_copy(&lat.basis, &lattice->basis);
    ibz_copy(&lat.denom, &lattice->denom);
    ibz_mul(&d2, &lat.denom, &lat.denom);

    // Compute the Gram matrix of the lattice
    quat_ideal_product_gram_matrix(&G, &lat);
    // Correct by 2 (Gram matrix corresponds to twice the norm)
    ibz_mul_2exp(&rad, radius, 1);

    // Compute a bounding parallelogram for the ball, stop if it only
    // contains the origin
    int ok = quat_lattice_bound_parallelogram(&box, &U, &lat, &rad);
    if (!ok)
        return (ok);

        // Rejection sampling from the parallelogram
#ifndef NDEBUG
        // int cnt = 0;
#endif
    do {
        // Sample vector
        for (int i = 0; i < 4; i++) {
            if (ibz_is_zero(&box.v[i])) {
                ibz_copy(&x.v[i], &ibz_const_zero);
            } else {
                ibz_mul_2exp(&tmp, &box.v[i], 1);
                ok &= ibz_rand_interval_with_domain(&x.v[i], &ibz_const_zero, &tmp, prng_domain);
                ibz_sub(&x.v[i], &x.v[i], &box.v[i]);
                ibz_set_bound(&x.v[i], ibz_get_bound(&box.v[i]));
                if (!ok)
                    return (ok);
            }
        }
        // Map to parallelogram
        ibz_mat_4x4_eval_t(&x, &x, &U);
        // Evaluate quadratic form
        quat_qf_eval(&tmp, &G, &x);
#ifndef NDEBUG
        // cnt++;
        // if (cnt % 100 == 0)
        //     printf("Lattice sampling rejected %d times\n", cnt - 1);
#endif

    } while ((ibz_get(&tmp) % 4 == 0) || (ibz_cmp(&tmp, &rad) > 0));
    assert(ibz_is_even(&tmp));
    if (res_ideal_norm != NULL)
        ibz_div_2exp(res_ideal_norm, &tmp, 1);

    // Evaluate linear combination
    ibz_mat_4x4_eval(&(res->coord), &(lat.basis), &x);
    ibz_copy(&(res->denom), &(lat.denom));
    quat_alg_normalize(res);
    ibz_set_bound(&res->denom, 3);
    for (int i = 0; i < 4; i++) {
        ibz_set_bound(&res->coord.v[i],
                      (ibz_get_bound(radius) + ibz_get_bound(&lat.basis.m[0][0])) / 2 + ibz_get_bound(&res->denom) + 3);
    }
    if (res_ideal_norm != NULL)
        ibz_set_bound(res_ideal_norm, ibz_get_bound(radius));

    return (ok);
}
