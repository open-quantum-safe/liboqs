#include <quaternion.h>
#include <quaternion_data.h>
#include <quaternion_constants.h>
#include "internal.h"
#include <stdio.h>

/** @file
 *
 * @authors Antonin Leroux
 *
 * @brief Functions related to norm equation solving or O0
 */

void
quat_lattice_O0_set(quat_lattice_t *O0)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&(O0->basis.m[i][j]), 0, 0);
        }
    }
    ibz_set(&(O0->denom), 2, 3);
    ibz_set(&(O0->basis.m[0][0]), 2, 3);
    ibz_set(&(O0->basis.m[1][1]), 2, 3);
    ibz_set(&(O0->basis.m[2][2]), 1, 2);
    ibz_set(&(O0->basis.m[1][2]), 1, 2);
    ibz_set(&(O0->basis.m[3][3]), 1, 2);
    ibz_set(&(O0->basis.m[0][3]), 1, 2);
}

void
quat_mod_O0(quat_alg_elem_t *red, const quat_alg_elem_t *x, const ibz_t *mod)
{
    ibz_vec_4_t coords;
    ibz_vec_4_init(&coords);
    quat_change_to_O0_basis(&coords, x);
    ibz_mod(&coords.v[0], &coords.v[0], mod);
    ibz_mod(&coords.v[1], &coords.v[1], mod);
    ibz_mod(&coords.v[2], &coords.v[2], mod);
    ibz_mod(&coords.v[3], &coords.v[3], mod);
    ibz_copy(&red->denom, &MAXORD_O0.denom);
    ibz_mat_4x4_eval(&red->coord, &MAXORD_O0.basis, &coords);

    // updating the bounds to account for the reduction mod N
    int len_N = ibz_get_bound(mod);
    ibz_set_bound(&red->coord.v[0], len_N + 2);
    ibz_set_bound(&red->coord.v[1], len_N + 2);
    ibz_set_bound(&red->coord.v[2], len_N + 2);
    ibz_set_bound(&red->coord.v[3], len_N + 2);
}

int
quat_represent_integer(quat_alg_elem_t *gamma, const ibz_t *n_gamma, prng_domain_ctx_t *prng_domain)
{
    // var dec
    int found;
    int randret = 0;
    ibz_t cornacchia_target;
    ibz_t bound, sq_bound, temp;
    ibz_t test;
    ibz_vec_4_t coeffs; // coeffs = [x,y,z,t]
    quat_alg_elem_t quat_temp;

    // var init
    found = 0;
    ibz_init(&bound);
    ibz_init(&test);
    ibz_init(&temp);
    ibz_init(&sq_bound);
    ibz_vec_4_init(&coeffs);
    quat_alg_elem_init(&quat_temp);
    ibz_init(&cornacchia_target);

    // check that the norm is odd
    assert((ibz_get(n_gamma) & 1) == 1);

    // bound on the bitlen of n_gamma / p
    // this is probably too big
    int bit_bound = (int)((ibz_bitsize(&QUATALG_PINFTY.p)));

    // the size of the search space is roughly n_gamma / p
    ibz_t counter;
    ibz_init(&counter);
    ibz_mul(&temp, &QUATALG_PINFTY.p, &QUATALG_PINFTY.p);
    ibz_sqrt_floor(&temp, &temp);
    ibz_div(&counter, &temp, n_gamma, &temp);
    ibz_set_bound(&counter, bit_bound);

    // computation of the first bound = sqrt (n_gamma / p - 1)
    ibz_div(&sq_bound, &bound, n_gamma, &QUATALG_PINFTY.p);
    ibz_sub(&sq_bound, &sq_bound, &ibz_const_one);
    ibz_set_bound(&sq_bound, bit_bound);
    ibz_sqrt_floor(&bound, &sq_bound);

    // entering the main loop
    while (!found && ibz_cmp(&counter, &ibz_const_zero) != 0) {
        // decreasing the counter
        ibz_sub(&counter, &counter, &ibz_const_one);
        ibz_set_bound(&counter, bit_bound);

        // we start by sampling the first coordinate
        randret = !ibz_rand_interval_with_domain(&coeffs.v[2], &ibz_const_one, &bound, prng_domain);
        if (randret) {
            debug_print("Randomness failed in sampling first coordinate with quat_represent_integer");
            return (found);
        }
        // then, we sample the second coordinate
        // computing the second bound in temp as sqrt( (adjust_n_gamma - p*coeffs.v[2]²)/ p )
        ibz_mul(&cornacchia_target, &coeffs.v[2], &coeffs.v[2]);
        ibz_mul(&temp, &cornacchia_target, &QUATALG_PINFTY.p);
        ibz_sub(&temp, n_gamma, &temp);
        ibz_div(&temp, &sq_bound, &temp, &QUATALG_PINFTY.p);
        ibz_set_bound(&temp, bit_bound);
        ibz_sqrt_floor(&temp, &temp);

        if (ibz_cmp(&temp, &ibz_const_zero) == 0) {
            continue;
        }
        // sampling the second value
        randret = !ibz_rand_interval_with_domain(&coeffs.v[3], &ibz_const_one, &temp, prng_domain);
        if (randret) {
            debug_print("Randomness failed in sampling second coordinate with quat_represent_integer");
            return (found);
        }
        // compute cornacchia_target = n_gamma - p * (z² + t²)
        ibz_mul(&temp, &coeffs.v[3], &coeffs.v[3]);
        ibz_add(&cornacchia_target, &cornacchia_target, &temp);
        ibz_mul(&cornacchia_target, &cornacchia_target, &QUATALG_PINFTY.p);
        ibz_sub(&cornacchia_target, n_gamma, &cornacchia_target);
        ibz_set_bound(&cornacchia_target, ibz_get_bound(n_gamma));
        assert(ibz_cmp(&cornacchia_target, &ibz_const_zero) > 0);

        // applying cornacchia
        if (ibz_probab_prime(&cornacchia_target, QUAT_primality_num_iter))
            found = ibz_cornacchia_prime(&(coeffs.v[0]), &(coeffs.v[1]), &cornacchia_target);
        else
            found = 0;

        if (found) {
            for (int i = 0; i < 2; i++)
                ibz_set_bound(&coeffs.v[i], (ibz_get_bound(&cornacchia_target) / 2) + 2);

#ifndef NDEBUG
            ibz_set(&temp, 1, 2);
            ibz_mul(&temp, &temp, &(coeffs.v[1]));
            ibz_mul(&temp, &temp, &(coeffs.v[1]));
            ibz_mul(&test, &(coeffs.v[0]), &(coeffs.v[0]));
            ibz_add(&temp, &temp, &test);
            assert(0 == ibz_cmp(&temp, &cornacchia_target));

            ibz_mul(&cornacchia_target, &(coeffs.v[3]), &(coeffs.v[3]));
            ibz_mul(&cornacchia_target, &cornacchia_target, &QUATALG_PINFTY.p);
            ibz_mul(&temp, &(coeffs.v[1]), &(coeffs.v[1]));
            ibz_add(&cornacchia_target, &cornacchia_target, &temp);
            ibz_mul(&temp, &(coeffs.v[0]), &coeffs.v[0]);
            ibz_add(&cornacchia_target, &cornacchia_target, &temp);
            ibz_mul(&temp, &(coeffs.v[2]), &coeffs.v[2]);
            ibz_mul(&temp, &temp, &QUATALG_PINFTY.p);
            ibz_add(&cornacchia_target, &cornacchia_target, &temp);
            assert(0 == ibz_cmp(&cornacchia_target, n_gamma));

            ibz_vec_4_copy(&gamma->coord, &coeffs);
            ibz_set(&gamma->denom, 1, 2);
            quat_alg_norm(&temp, &test, gamma, &QUATALG_PINFTY);
            assert(ibz_is_one(&test));
            assert(0 == ibz_cmp(&temp, n_gamma));
            assert(quat_lattice_contains(NULL, &MAXORD_O0, gamma));
#endif
        }
    }

    if (found) {
        // new gamma
        ibz_copy(&gamma->coord.v[0], &coeffs.v[0]);
        ibz_copy(&gamma->coord.v[1], &coeffs.v[1]);
        ibz_copy(&gamma->coord.v[2], &coeffs.v[2]);
        ibz_copy(&gamma->coord.v[3], &coeffs.v[3]);
        ibz_set(&gamma->denom, 1, 2);
    } else {
        fprintf(stderr, "RepresentInteger found no solution. This should never happen.\n");
    }

    return (found);
}

void
quat_change_to_O0_basis(ibz_vec_4_t *vec, const quat_alg_elem_t *el)
{
    ibz_t tmp;
    ibz_init(&tmp);
    ibz_copy(&vec->v[2], &el->coord.v[2]);
    ibz_mul_2exp(&vec->v[2], &vec->v[2], 1); // double (not optimal if el->denom is even...)
    ibz_copy(&vec->v[3], &el->coord.v[3]);
    ibz_mul_2exp(&vec->v[3], &vec->v[3], 1); // double (not optimal if el->denom is even...)
    ibz_sub(&vec->v[0], &el->coord.v[0], &el->coord.v[3]);
    ibz_sub(&vec->v[1], &el->coord.v[1], &el->coord.v[2]);

    assert(ibz_divides(&vec->v[0], &el->denom));
    assert(ibz_divides(&vec->v[1], &el->denom));
    assert(ibz_divides(&vec->v[2], &el->denom));
    assert(ibz_divides(&vec->v[3], &el->denom));

    ibz_div(&vec->v[0], &tmp, &vec->v[0], &el->denom);
    ibz_div(&vec->v[1], &tmp, &vec->v[1], &el->denom);
    ibz_div(&vec->v[2], &tmp, &vec->v[2], &el->denom);
    ibz_div(&vec->v[3], &tmp, &vec->v[3], &el->denom);
}
