#include <quaternion.h>
#include "internal.h"

/** @file
 *
 * @authors Antonin Leroux
 *
 * @brief Functions related to norm equation solving or special extremal orders
 */

void
quat_lattice_O0_set(quat_lattice_t *O0)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&(O0->basis[i][j]), 0);
        }
    }
    ibz_set(&(O0->denom), 2);
    ibz_set(&(O0->basis[0][0]), 2);
    ibz_set(&(O0->basis[1][1]), 2);
    ibz_set(&(O0->basis[2][2]), 1);
    ibz_set(&(O0->basis[1][2]), 1);
    ibz_set(&(O0->basis[3][3]), 1);
    ibz_set(&(O0->basis[0][3]), 1);
}

void
quat_lattice_O0_set_extremal(quat_p_extremal_maximal_order_t *O0)
{
    ibz_set(&O0->z.coord[1], 1);
    ibz_set(&O0->t.coord[2], 1);
    ibz_set(&O0->z.denom, 1);
    ibz_set(&O0->t.denom, 1);
    O0->q = 1;
    quat_lattice_O0_set(&(O0->order));
}

void
quat_order_elem_create(quat_alg_elem_t *elem,
                       const quat_p_extremal_maximal_order_t *order,
                       const ibz_vec_4_t *coeffs,
                       const quat_alg_t *Bpoo)
{

    // var dec
    quat_alg_elem_t quat_temp;

    // var init
    quat_alg_elem_init(&quat_temp);

    // elem = x
    quat_alg_scalar(elem, &(*coeffs)[0], &ibz_const_one);

    // quat_temp = i*y
    quat_alg_scalar(&quat_temp, &((*coeffs)[1]), &ibz_const_one);
    quat_alg_mul(&quat_temp, &order->z, &quat_temp, Bpoo);

    // elem = x + i*y
    quat_alg_add(elem, elem, &quat_temp);

    // quat_temp = z * j
    quat_alg_scalar(&quat_temp, &(*coeffs)[2], &ibz_const_one);
    quat_alg_mul(&quat_temp, &order->t, &quat_temp, Bpoo);

    // elem = x + i* + z*j
    quat_alg_add(elem, elem, &quat_temp);

    // quat_temp = t * j * i
    quat_alg_scalar(&quat_temp, &(*coeffs)[3], &ibz_const_one);
    quat_alg_mul(&quat_temp, &order->t, &quat_temp, Bpoo);
    quat_alg_mul(&quat_temp, &quat_temp, &order->z, Bpoo);

    // elem =  x + i*y + j*z + j*i*t
    quat_alg_add(elem, elem, &quat_temp);

    quat_alg_elem_finalize(&quat_temp);
}

int
quat_represent_integer(quat_alg_elem_t *gamma,
                       const ibz_t *n_gamma,
                       int non_diag,
                       const quat_represent_integer_params_t *params)
{

    if (ibz_is_even(n_gamma)) {
        return 0;
    }
    // var dec
    int found;
    ibz_t cornacchia_target;
    ibz_t adjusted_n_gamma, q;
    ibz_t bound, sq_bound, temp;
    ibz_t test;
    ibz_vec_4_t coeffs; // coeffs = [x,y,z,t]
    quat_alg_elem_t quat_temp;

    if (non_diag)
        assert(params->order->q % 4 == 1);

    // var init
    found = 0;
    ibz_init(&bound);
    ibz_init(&test);
    ibz_init(&temp);
    ibz_init(&q);
    ibz_init(&sq_bound);
    ibz_vec_4_init(&coeffs);
    quat_alg_elem_init(&quat_temp);
    ibz_init(&adjusted_n_gamma);
    ibz_init(&cornacchia_target);

    ibz_set(&q, params->order->q);

    // this could be removed in the current state
    int standard_order = (params->order->q == 1);

    // adjusting the norm of gamma (multiplying by 4 to find a solution in an order of odd level)
    if (non_diag || standard_order) {
        ibz_mul(&adjusted_n_gamma, n_gamma, &ibz_const_two);
        ibz_mul(&adjusted_n_gamma, &adjusted_n_gamma, &ibz_const_two);
    } else {
        ibz_copy(&adjusted_n_gamma, n_gamma);
    }
    // computation of the first bound = sqrt (adjust_n_gamma / p - q)
    ibz_div(&sq_bound, &bound, &adjusted_n_gamma, &((params->algebra)->p));
    ibz_set(&temp, params->order->q);
    ibz_sub(&sq_bound, &sq_bound, &temp);
    ibz_sqrt_floor(&bound, &sq_bound);

    // the size of the search space is roughly n_gamma / (p√q)
    ibz_t counter;
    ibz_init(&counter);
    ibz_mul(&temp, &temp, &((params->algebra)->p));
    ibz_mul(&temp, &temp, &((params->algebra)->p));
    ibz_sqrt_floor(&temp, &temp);
    ibz_div(&counter, &temp, &adjusted_n_gamma, &temp);

    // entering the main loop
    while (!found && ibz_cmp(&counter, &ibz_const_zero) != 0) {
        // decreasing the counter
        ibz_sub(&counter, &counter, &ibz_const_one);

        // we start by sampling the first coordinate
        ibz_rand_interval(&coeffs[2], &ibz_const_one, &bound);

        // then, we sample the second coordinate
        // computing the second bound in temp as sqrt( (adjust_n_gamma - p*coeffs[2]²)/qp )
        ibz_mul(&cornacchia_target, &coeffs[2], &coeffs[2]);
        ibz_mul(&temp, &cornacchia_target, &(params->algebra->p));
        ibz_sub(&temp, &adjusted_n_gamma, &temp);
        ibz_mul(&sq_bound, &q, &(params->algebra->p));
        ibz_div(&temp, &sq_bound, &temp, &sq_bound);
        ibz_sqrt_floor(&temp, &temp);

        if (ibz_cmp(&temp, &ibz_const_zero) == 0) {
            continue;
        }
        // sampling the second value
        ibz_rand_interval(&coeffs[3], &ibz_const_one, &temp);

        // compute cornacchia_target = n_gamma - p * (z² + q*t²)
        ibz_mul(&temp, &coeffs[3], &coeffs[3]);
        ibz_mul(&temp, &q, &temp);
        ibz_add(&cornacchia_target, &cornacchia_target, &temp);
        ibz_mul(&cornacchia_target, &cornacchia_target, &((params->algebra)->p));
        ibz_sub(&cornacchia_target, &adjusted_n_gamma, &cornacchia_target);
        assert(ibz_cmp(&cornacchia_target, &ibz_const_zero) > 0);

        // applying cornacchia
        if (ibz_probab_prime(&cornacchia_target, params->primality_test_iterations))
            found = ibz_cornacchia_prime(&(coeffs[0]), &(coeffs[1]), &q, &cornacchia_target);
        else
            found = 0;

        if (found && non_diag && standard_order) {
            // check that we can divide by two at least once
            // the treatmeat depends if the basis contains (1+j)/2 or (1+k)/2
            // we must have x = t mod 2 and y = z mod 2
            // if q=1 we can simply swap x and y
            if (ibz_is_odd(&coeffs[0]) != ibz_is_odd(&coeffs[3])) {
                ibz_swap(&coeffs[1], &coeffs[0]);
            }
            // we further check that (x-t)/2 = 1 mod 2 and (y-z)/2 = 1 mod 2 to ensure that the
            // resulting endomorphism will behave well for dim 2 computations
            found = found && ((ibz_get(&coeffs[0]) - ibz_get(&coeffs[3])) % 4 == 2) &&
                    ((ibz_get(&coeffs[1]) - ibz_get(&coeffs[2])) % 4 == 2);
        }
        if (found) {

#ifndef NDEBUG
            ibz_set(&temp, (params->order->q));
            ibz_mul(&temp, &temp, &(coeffs[1]));
            ibz_mul(&temp, &temp, &(coeffs[1]));
            ibz_mul(&test, &(coeffs[0]), &(coeffs[0]));
            ibz_add(&temp, &temp, &test);
            assert(0 == ibz_cmp(&temp, &cornacchia_target));

            ibz_mul(&cornacchia_target, &(coeffs[3]), &(coeffs[3]));
            ibz_mul(&cornacchia_target, &cornacchia_target, &(params->algebra->p));
            ibz_mul(&temp, &(coeffs[1]), &(coeffs[1]));
            ibz_add(&cornacchia_target, &cornacchia_target, &temp);
            ibz_set(&temp, (params->order->q));
            ibz_mul(&cornacchia_target, &cornacchia_target, &temp);
            ibz_mul(&temp, &(coeffs[0]), &coeffs[0]);
            ibz_add(&cornacchia_target, &cornacchia_target, &temp);
            ibz_mul(&temp, &(coeffs[2]), &coeffs[2]);
            ibz_mul(&temp, &temp, &(params->algebra->p));
            ibz_add(&cornacchia_target, &cornacchia_target, &temp);
            assert(0 == ibz_cmp(&cornacchia_target, &adjusted_n_gamma));
#endif
            // translate x,y,z,t into the quaternion element gamma
            quat_order_elem_create(gamma, (params->order), &coeffs, (params->algebra));
#ifndef NDEBUG
            quat_alg_norm(&temp, &(coeffs[0]), gamma, (params->algebra));
            assert(ibz_is_one(&(coeffs[0])));
            assert(0 == ibz_cmp(&temp, &adjusted_n_gamma));
            assert(quat_lattice_contains(NULL, &((params->order)->order), gamma));
#endif
            // making gamma primitive
            // coeffs contains the coefficients of primitivized gamma in the basis of order
            quat_alg_make_primitive(&coeffs, &temp, gamma, &((params->order)->order));

            if (non_diag || standard_order)
                found = (ibz_cmp(&temp, &ibz_const_two) == 0);
            else
                found = (ibz_cmp(&temp, &ibz_const_one) == 0);
        }
    }

    if (found) {
        // new gamma
        ibz_mat_4x4_eval(&coeffs, &(((params->order)->order).basis), &coeffs);
        ibz_copy(&gamma->coord[0], &coeffs[0]);
        ibz_copy(&gamma->coord[1], &coeffs[1]);
        ibz_copy(&gamma->coord[2], &coeffs[2]);
        ibz_copy(&gamma->coord[3], &coeffs[3]);
        ibz_copy(&gamma->denom, &(((params->order)->order).denom));
    }
    // var finalize
    ibz_finalize(&counter);
    ibz_finalize(&bound);
    ibz_finalize(&temp);
    ibz_finalize(&sq_bound);
    ibz_vec_4_finalize(&coeffs);
    quat_alg_elem_finalize(&quat_temp);
    ibz_finalize(&adjusted_n_gamma);
    ibz_finalize(&cornacchia_target);
    ibz_finalize(&q);
    ibz_finalize(&test);

    return found;
}

int
quat_sampling_random_ideal_O0_given_norm(quat_left_ideal_t *lideal,
                                         const ibz_t *norm,
                                         int is_prime,
                                         const quat_represent_integer_params_t *params,
                                         const ibz_t *prime_cofactor)
{

    ibz_t n_temp, norm_d;
    ibz_t disc;
    quat_alg_elem_t gen, gen_rerand;
    int found = 0;
    ibz_init(&n_temp);
    ibz_init(&norm_d);
    ibz_init(&disc);
    quat_alg_elem_init(&gen);
    quat_alg_elem_init(&gen_rerand);

    // when the norm is prime we can be quite efficient
    // by avoiding to run represent integer
    // the first step is to generate one ideal of the correct norm
    if (is_prime) {

        // we find a quaternion element of norm divisible by norm
        while (!found) {
            // generating a trace-zero element at random
            ibz_set(&gen.coord[0], 0);
            ibz_sub(&n_temp, norm, &ibz_const_one);
            for (int i = 1; i < 4; i++)
                ibz_rand_interval(&gen.coord[i], &ibz_const_zero, &n_temp);

            // first, we compute the norm of the gen
            quat_alg_norm(&n_temp, &norm_d, &gen, (params->algebra));
            assert(ibz_is_one(&norm_d));

            // and finally the negation mod norm
            ibz_neg(&disc, &n_temp);
            ibz_mod(&disc, &disc, norm);
            // now we check that -n is a square mod norm
            // and if the square root exists we compute it
            found = ibz_sqrt_mod_p(&gen.coord[0], &disc, norm);
            found = found && !quat_alg_elem_is_zero(&gen);
        }
    } else {
        assert(prime_cofactor != NULL);
        // if it is not prime or we don't know if it is prime, we may just use represent integer
        // and use a precomputed prime as cofactor
        assert(!ibz_is_zero(norm));
        ibz_mul(&n_temp, prime_cofactor, norm);
        found = quat_represent_integer(&gen, &n_temp, 0, params);
        found = found && !quat_alg_elem_is_zero(&gen);
    }
#ifndef NDEBUG
    if (found) {
        // first, we compute the norm of the gen
        quat_alg_norm(&n_temp, &norm_d, &gen, (params->algebra));
        assert(ibz_is_one(&norm_d));
        ibz_mod(&n_temp, &n_temp, norm);
        assert(ibz_cmp(&n_temp, &ibz_const_zero) == 0);
    }
#endif

    // now we just have to rerandomize the class of the ideal generated by gen
    found = 0;
    while (!found) {
        for (int i = 0; i < 4; i++) {
            ibz_rand_interval(&gen_rerand.coord[i], &ibz_const_one, norm);
        }
        quat_alg_norm(&n_temp, &norm_d, &gen_rerand, (params->algebra));
        assert(ibz_is_one(&norm_d));
        ibz_gcd(&disc, &n_temp, norm);
        found = ibz_is_one(&disc);
        found = found && !quat_alg_elem_is_zero(&gen_rerand);
    }

    quat_alg_mul(&gen, &gen, &gen_rerand, (params->algebra));
    // in both cases, whether norm is prime or not prime,
    // gen is not divisible by any integer factor of the target norm
    // therefore the call below will yield an ideal of the correct norm
    quat_lideal_create(lideal, &gen, norm, &((params->order)->order), (params->algebra));
    assert(ibz_cmp(norm, &(lideal->norm)) == 0);

    ibz_finalize(&n_temp);
    quat_alg_elem_finalize(&gen);
    quat_alg_elem_finalize(&gen_rerand);
    ibz_finalize(&norm_d);
    ibz_finalize(&disc);
    return (found);
}

void
quat_change_to_O0_basis(ibz_vec_4_t *vec, const quat_alg_elem_t *el)
{
    ibz_t tmp;
    ibz_init(&tmp);
    ibz_copy(&(*vec)[2], &el->coord[2]);
    ibz_add(&(*vec)[2], &(*vec)[2], &(*vec)[2]); // double (not optimal if el->denom is even...)
    ibz_copy(&(*vec)[3], &el->coord[3]);         // double (not optimal if el->denom is even...)
    ibz_add(&(*vec)[3], &(*vec)[3], &(*vec)[3]);
    ibz_sub(&(*vec)[0], &el->coord[0], &el->coord[3]);
    ibz_sub(&(*vec)[1], &el->coord[1], &el->coord[2]);

    assert(ibz_divides(&(*vec)[0], &el->denom));
    assert(ibz_divides(&(*vec)[1], &el->denom));
    assert(ibz_divides(&(*vec)[2], &el->denom));
    assert(ibz_divides(&(*vec)[3], &el->denom));

    ibz_div(&(*vec)[0], &tmp, &(*vec)[0], &el->denom);
    ibz_div(&(*vec)[1], &tmp, &(*vec)[1], &el->denom);
    ibz_div(&(*vec)[2], &tmp, &(*vec)[2], &el->denom);
    ibz_div(&(*vec)[3], &tmp, &(*vec)[3], &el->denom);

    ibz_finalize(&tmp);
}
