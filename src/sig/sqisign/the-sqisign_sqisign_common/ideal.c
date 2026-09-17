#include <internal.h>
#include <math.h>
#include <stdio.h>
#include <prng.h>
#include <quaternion_data.h>
#include <quaternion_constants.h>

void
quat_to_lattice(quat_lattice_t *lattice, const quat_ideal_t *ideal)
{
    quat_lattice_O0_set(lattice);
    ibz_mul_2exp(&lattice->basis.m[0][0], &ideal->norm, 1);
    ibz_set_bound(&lattice->basis.m[0][0], ibz_get_bound(&ideal->norm) + 1);
    ibz_copy(&lattice->basis.m[1][1], &lattice->basis.m[0][0]);
    ibz_set(&lattice->basis.m[2][2], 1, 2);
    ibz_set(&lattice->basis.m[2][2], 1, 2);
    ibz_mul_2exp(&lattice->basis.m[0][2], &ideal->x, 1);
    ibz_mul_2exp(&lattice->basis.m[1][2], &ideal->y, 1);
    ibz_set_bound(&lattice->basis.m[0][2], ibz_get_bound(&ideal->norm) + 1);
    ibz_set_bound(&lattice->basis.m[1][2], ibz_get_bound(&ideal->norm) + 1);
    ibz_add(&lattice->basis.m[1][2], &lattice->basis.m[1][2], &ibz_const_one);
    ibz_copy(&lattice->basis.m[1][3], &lattice->basis.m[0][2]);
    ibz_sub(&lattice->basis.m[0][3], &lattice->basis.m[0][0], &lattice->basis.m[1][2]);
    ibz_set_bound(&lattice->basis.m[0][3], ibz_get_bound(&ideal->norm) + 1);
}

void
quat_xyn_to_inert_ideal(quat_ideal_t *ideal, const ibz_t *x, const ibz_t *y, const ibz_t *n)
{
    ibz_copy(&ideal->norm, n);
    ibz_copy(&ideal->x, x);
    ibz_copy(&ideal->y, y);
    ibz_set_bound(&ideal->x, ibz_get_bound(&ideal->norm));
    ibz_set_bound(&ideal->y, ibz_get_bound(&ideal->norm));
}

void
quat_ideal_odd_inert_gen(quat_alg_elem_t *gen, const quat_ideal_t *ideal)
{
    quat_alg_elem_set(gen, 2, 0, 0, 1, 0);
    ibz_add(&gen->coord.v[0], &ideal->x, &ideal->x);
    ibz_add(&gen->coord.v[1], &ideal->y, &ideal->y);
    ibz_add(&gen->coord.v[1], &gen->coord.v[1], &ibz_const_one);
}

void
quat_ideal_copy(quat_ideal_t *copy, const quat_ideal_t *copied)
{
    ibz_copy(&copy->norm, &copied->norm);
    ibz_copy(&copy->x, &copied->x);
    ibz_copy(&copy->y, &copied->y);
}

int
quat_ideal_create_O0_inert_odd(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm)
{
    ibz_t tmp, s, t, x, y, r, q;
    quat_alg_elem_t tmp_gen, mul_helper;
    ibz_vec_2_t tmp_g;
    quat_alg_elem_init(&mul_helper);
    ibz_init(&tmp);
    ibz_init(&s);
    ibz_init(&t);
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&r);
    ibz_init(&q);
    ibz_vec_2_init(&tmp_g);
    quat_alg_elem_init(&tmp_gen);
    quat_alg_elem_copy(&tmp_gen, gen);
    quat_alg_normalize(&tmp_gen);
    quat_mod_O0(&tmp_gen, &tmp_gen, norm);
    // get coords in Z
    assert(ibz_is_one(&tmp_gen.denom) || (0 == ibz_cmp(&tmp_gen.denom, &ibz_const_two)));
    ibz_set(&tmp_gen.denom, 1, 2);

    // gen = a,b,c,d, s = c^2+d^2 mod n
    ibz_mul(&tmp, &tmp_gen.coord.v[2], &tmp_gen.coord.v[2]);
    ibz_mul(&r, &tmp_gen.coord.v[3], &tmp_gen.coord.v[3]);
    ibz_add(&tmp, &tmp, &r);
    ibz_mod(&s, &tmp, norm);

    ibz_gcd(&tmp, norm, &s);

    // if s=0, get t, reset gen coords
    if (!ibz_is_one(&tmp)) {
        {
            ibz_copy(&q, norm);
            while (!ibz_is_one(&tmp)) {
                ibz_div(&q, &r, &q, &tmp);
                assert(ibz_is_zero(&r));
                ibz_gcd(&tmp, &q, &s);
            }
            quat_alg_elem_set(&mul_helper, 1, 0, 0, 1, 0);
            ibz_copy(&mul_helper.coord.v[0], &q);
            quat_alg_mul(&tmp_gen, &mul_helper, &tmp_gen, &QUATALG_PINFTY);
            // mod N
            for (int i = 0; i < 4; i++)
                ibz_mod(&tmp_gen.coord.v[i], &tmp_gen.coord.v[i], norm);
            quat_alg_normalize(&tmp_gen);
        }
        // gen = a,b,c,d, s = c^2+d^2 mod n
        ibz_mul(&tmp, &tmp_gen.coord.v[2], &tmp_gen.coord.v[2]);
        ibz_mul(&r, &tmp_gen.coord.v[3], &tmp_gen.coord.v[3]);
        ibz_add(&s, &tmp, &r);
        // ibz_mod(&s, &s, norm);
        ibz_gcd(&tmp, &s, norm);
    }
    if (ibz_is_one(&tmp)) {
        assert(ibz_is_one(&tmp_gen.denom));
        // t = (2s)^-1 mod n
        ibz_add(&tmp, &s, &s);
        ibz_invmod(&t, &tmp, norm);
        // x = 2 ((ac + bd)t mod N )
        ibz_neg(&(tmp_gen.coord.v[3]), &(tmp_gen.coord.v[3]));
        ibz_vec_2_gaussian_mul(&tmp_g, (ibz_vec_2_t *)&(tmp_gen.coord.v[0]), (ibz_vec_2_t *)&(tmp_gen.coord.v[2]));
        ibz_neg(&(tmp_gen.coord.v[3]), &(tmp_gen.coord.v[3]));
        ibz_mod(&tmp_g.v[0], &tmp_g.v[0], norm);
        ibz_mod(&tmp_g.v[1], &tmp_g.v[1], norm);
        ibz_mul(&tmp, &tmp_g.v[0], &t);
        ibz_mod(&x, &tmp, norm);
        //  y = 1 + 2 ((bc − ad − s)t mod N )
        ibz_sub(&tmp, &tmp_g.v[1], &s);
        ibz_mod(&tmp, &tmp, norm);
        ibz_mul(&tmp, &tmp, &t);
        ibz_mod(&y, &tmp, norm);
        //  Mat_inert
        quat_xyn_to_inert_ideal(ideal, &x, &y, norm);
    }
    return (1);
}

void
quat_ideal_create_O0_odd(quat_ideal_t *ideal, quat_alg_elem_t *split_gen, const quat_alg_elem_t *gen, const ibz_t *norm)
{
    // int found = 0;
    assert(ibz_is_odd(norm));
    ibz_t tmp, n2, n;
    ibz_vec_2_t uv;
    quat_alg_elem_t tmp_gen, setter;
    quat_lattice_t O0;
    quat_lattice_init(&O0);
    ibz_init(&tmp);
    ibz_init(&n);
    ibz_init(&n2);
    ibz_vec_2_init(&uv);
    quat_alg_elem_init(&tmp_gen);
    quat_alg_elem_init(&setter);
    quat_lattice_O0_set(&O0);
    quat_alg_elem_set(&tmp_gen, 1, 0, 0, 0, 0);
    quat_alg_elem_set(&setter, 1, 0, 0, 0, 0);

    quat_alg_make_primitive(&tmp_gen.coord, &tmp, gen, &O0);
    quat_mod_O0(&tmp_gen, &tmp_gen, norm);
    ibz_gcd(&tmp, &tmp, norm);
    assert(ibz_is_one(&tmp));

    // found = 1;
    quat_alg_elem_copy(&tmp_gen, gen);
    quat_mod_O0(&tmp_gen, &tmp_gen, norm);
    quat_alg_normalize(&tmp_gen);
    assert(ibz_is_one(&tmp_gen.denom) || (0 == ibz_cmp(&tmp_gen.denom, &ibz_const_two)));
    ibz_set(&tmp_gen.denom, 1, 2);
    // to fix ij -> ji
    ibz_neg(&tmp_gen.coord.v[3], &tmp_gen.coord.v[3]);
    ibz_vec_2_gaussian_gcd(&uv, (ibz_vec_2_t *)&tmp_gen.coord.v[0], (ibz_vec_2_t *)&tmp_gen.coord.v[2]);
    // to fix ji -> ij
    ibz_neg(&tmp_gen.coord.v[3], &tmp_gen.coord.v[3]);

    quat_alg_elem_set(&setter, 1, 0, 0, 0, 0);
    ibz_copy(&setter.coord.v[0], norm);
    ibz_vec_2_gaussian_gcd(&uv, &uv, (ibz_vec_2_t *)&setter.coord.v[0]);
    ibz_neg(&tmp, &uv.v[1]);
    if (ibz_is_zero(&uv.v[0]) && (ibz_is_one(&tmp) || ibz_is_one(&uv.v[1]))) {
        ibz_vec_2_set(&uv, 1, 0);
    }
    ibz_sum_two_squares(&n2, &uv.v[0], &uv.v[1]);
    ibz_div(&n, &tmp, norm, &n2);
    assert(ibz_is_zero(&tmp));
    quat_alg_elem_set(&setter, 1, 0, 0, 0, 0);
    ibz_copy(&setter.coord.v[0], &uv.v[0]);
    ibz_neg(&setter.coord.v[1], &uv.v[1]);
    ibz_copy(&setter.denom, &n2);
    quat_alg_mul(&tmp_gen, &tmp_gen, &setter, &QUATALG_PINFTY);
    quat_mod_O0(&tmp_gen, &tmp_gen, norm);
    quat_alg_normalize(&tmp_gen);

    // Ideal_inert
    int UNUSED inertfound = quat_ideal_create_O0_inert_odd(ideal, &tmp_gen, &n);
    assert(inertfound);
    quat_alg_elem_set(split_gen, 1, 1, 0, 0, 0);
    ibz_copy(&split_gen->coord.v[0], &uv.v[0]);
    ibz_copy(&split_gen->coord.v[1], &uv.v[1]);
}

int
quat_ideal_create_O0_pow_two(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm)
{
    if (ibz_is_one(norm)) {
        ibz_set(&ideal->norm, 1, 2);
        ibz_set(&ideal->x, 0, 0);
        ibz_set(&ideal->y, 1, 2);
    } else {
        ibz_t tmp, s, t, x, y, r, d2;
        quat_alg_elem_t tmp_gen, setter;
        ibz_vec_4_t abcd;
        ibz_vec_4_init(&abcd);
        ibz_init(&tmp);
        ibz_init(&s);
        ibz_init(&t);
        ibz_init(&x);
        ibz_init(&y);
        ibz_init(&r);
        ibz_init(&d2);
        quat_alg_elem_init(&tmp_gen);
        quat_alg_elem_init(&setter);
        quat_alg_elem_copy(&tmp_gen, gen);
        quat_alg_normalize(&tmp_gen);
        quat_mod_O0(&tmp_gen, &tmp_gen, norm);
        // get coords in Z
        assert(ibz_is_one(&tmp_gen.denom) || (0 == ibz_cmp(&tmp_gen.denom, &ibz_const_two)));
        quat_change_to_O0_basis(&abcd, &tmp_gen);

        if (ibz_is_even(&abcd.v[2]) == ibz_is_even(&abcd.v[3])) {
            // need to check the 2nd test is doing what it should again
            if (ibz_is_odd(&abcd.v[2]) && ibz_is_odd(&abcd.v[3]) &&
                (ibz_is_even(&abcd.v[1]) != ibz_is_even(&abcd.v[0]))) {
                // This multiplication by a constant could avoid the normalize
                quat_alg_elem_set(&setter, 2, 0, 1, 1, 0);
                quat_alg_mul(&tmp_gen, &setter, &tmp_gen, &QUATALG_PINFTY);
                quat_alg_normalize(&tmp_gen);
                quat_mod_O0(&tmp_gen, &tmp_gen, norm);
                quat_change_to_O0_basis(&abcd, &tmp_gen);
            } else {
                return (0);
            }
        }
        if (ibz_is_odd(&abcd.v[2])) {
            quat_alg_elem_set(&setter, 1, 0, 1, 0, 0);
            quat_alg_mul(&tmp_gen, &setter, &tmp_gen, &QUATALG_PINFTY);
            quat_mod_O0(&tmp_gen, &tmp_gen, norm);
            quat_change_to_O0_basis(&abcd, &tmp_gen);
        }
        assert(ibz_is_even(&abcd.v[2]) && ibz_is_odd(&abcd.v[3]));

        // s = d^2+c^2
        ibz_mul(&s, &abcd.v[2], &abcd.v[2]);
        ibz_mul(&d2, &abcd.v[3], &abcd.v[3]);
        ibz_add(&s, &s, &d2);
        ibz_gcd(&tmp, &s, norm);
        // t=s^{-1}[2^e]
        ibz_invmod(&t, &s, norm);

        // x =  2 (t(ac + bd + cd) mod 2e )
        ibz_mul(&x, &abcd.v[0], &abcd.v[2]);
        ibz_mul(&y, &abcd.v[1], &abcd.v[3]);
        ibz_add(&x, &x, &y);
        ibz_mul(&y, &abcd.v[2], &abcd.v[3]);
        ibz_add(&x, &x, &y);
        ibz_mod(&x, &x, norm);
        ibz_mul(&x, &t, &x);
        ibz_mod(&x, &x, norm); // This should just be truncation
        // ibz_add(&x, &x, &x);
        //  y= 1 + 2 t(bc - da -d^2) mod 2^e
        ibz_mul(&y, &abcd.v[1], &abcd.v[2]);
        ibz_mul(&tmp, &abcd.v[0], &abcd.v[3]);
        ibz_sub(&y, &y, &tmp);
        ibz_sub(&y, &y, &d2);
        ibz_mod(&y, &y, norm);
        ibz_mul(&y, &y, &t);
        ibz_mod(&y, &y, norm); // This should just be truncation
        // ibz_add(&y, &y, &y);
        // ibz_add(&y, &y, &ibz_const_one);

        // Mat_inert
        quat_xyn_to_inert_ideal(ideal, &x, &y, norm);
    }
    return (1);
}

int
quat_random_ideal_O0_given_prime_norm(quat_ideal_t *ideal, const ibz_t *norm, prng_domain_ctx_t *prng_domain)
{
    ibz_t x, y, tmp, n2, n4, nm1, r4, rp;
    int bit;
    int found = 0;
    int ret = 1;
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&tmp);
    ibz_init(&n2);
    ibz_init(&n4);
    ibz_init(&r4);
    ibz_init(&rp);
    ibz_init(&nm1);
    assert(0 != ibz_cmp(norm, &QUATALG_PINFTY.p));
    ibz_add(&n2, norm, norm);
    ibz_add(&n4, &n2, &n2);
    ibz_sub(&nm1, norm, &ibz_const_one);

    while (!found) {
        ret = ret && ibz_rand_interval_with_domain(&x, &ibz_const_zero, &nm1, prng_domain);
        if (!ret)
            return (ret);
        ibz_add(&x, &x, &x);
        ibz_mul(&tmp, &x, &x);
        ibz_mod(&tmp, &tmp, &n4);
        ibz_add(&tmp, &QUATALG_PINFTY.p, &tmp);
        ibz_neg(&tmp, &tmp);
        ibz_mod(&tmp, &tmp, &n4);
        ibz_mod(&rp, &tmp, norm);
        found = ((ibz_legendre(&rp, norm) == 1) && ((ibz_get(&tmp) & 3) < 2));
    }
    ibz_sqrt_mod_p(&rp, &rp, norm);
    ibz_set(&r4, (ibz_get(&tmp) & 1), 2);
    ibz_set(&y, 4, 4);
    ibz_crt(&y, &r4, &rp, &y, norm);
    ret = ret && ibz_rand_interval_with_domain(&r4, &ibz_const_zero, &ibz_const_one, prng_domain);
    if (!ret)
        return (ret);
    bit = (ibz_get(&r4) & 1);
    if (bit & 1) {
        ibz_neg(&y, &y);
    }
    ibz_mod(&y, &y, &n2);

#ifndef NDEBUG
    ibz_t test;
    ibz_init(&test);
    ibz_mul(&test, &y, &y);
    ibz_mod(&test, &test, &n4);
    assert(0 == ibz_cmp(&test, &tmp));
#endif
#ifndef NDEBUG
    ibz_t sum, prod;
    ibz_init(&sum);
    ibz_init(&prod);
    ibz_mul(&sum, &x, &x);
    ibz_mul(&sum, &x, &x);
    ibz_mul(&prod, &y, &y);
    ibz_mul(&prod, &y, &y);
    ibz_add(&prod, &prod, &QUATALG_PINFTY.p);
    assert(ibz_get(&prod) % 4 == 0);
    ibz_add(&sum, &sum, &prod);
    ibz_mod(&sum, &sum, &n4);
    ibz_mod(&prod, &sum, norm);
    assert(ibz_is_zero(&prod));
    assert(ibz_is_zero(&sum));
#endif

    ibz_sub(&y, &y, &ibz_const_one);
    ibz_div_2exp(&y, &y, 1);
    ibz_div_2exp(&x, &x, 1);

    quat_xyn_to_inert_ideal(ideal, &x, &y, norm);
    return (ret);
}

int
quat_random_ideal_O0_given_arbitrary_odd_norm(quat_ideal_t *ideal,
                                              quat_alg_elem_t *split_gen,
                                              const ibz_t *norm,
                                              const quat_alg_elem_t *shift_gen,
                                              prng_domain_ctx_t *prng_domain)
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
    ibz_set(&gen.denom, 1, 2);
    ibz_set(&gen_rerand.denom, 1, 2);

    assert(!ibz_is_zero(norm));
    ibz_mul(&n_temp, &QUAT_prime_cofactor, norm);
    assert(ibz_is_odd(&n_temp));
    found = quat_represent_integer(&gen, &n_temp, prng_domain);
    found = found && !quat_alg_elem_is_zero(&gen);
    if (!found)
        return (0);

#ifndef NDEBUG
    {
        // first, we compute the norm of the gen
        quat_alg_norm(&n_temp, &norm_d, &gen, &QUATALG_PINFTY);
        assert(ibz_is_one(&norm_d));
        ibz_mod(&n_temp, &n_temp, norm);
        assert(ibz_cmp(&n_temp, &ibz_const_zero) == 0);
    }
#endif

    // now we just have to rerandomize the class of the ideal generated by gen
    found = 0;
    while (!found) {
        for (int i = 0; i < 4; i++) {
            if (!ibz_rand_interval_with_domain(&gen_rerand.coord.v[i], &ibz_const_one, norm, prng_domain))
                return (0);
        }
        quat_alg_norm(&n_temp, &norm_d, &gen_rerand, &QUATALG_PINFTY);
        assert(ibz_is_one(&norm_d));
        ibz_gcd(&disc, &n_temp, norm);
        found = ibz_is_one(&disc);
        found = found && !quat_alg_elem_is_zero(&gen_rerand);
    }

    quat_alg_mul(&gen, &gen, &gen_rerand, &QUATALG_PINFTY);
    quat_mod_O0(&gen, &gen, norm);
    // applying the multiplication by shift_gen
    quat_alg_elem_copy(&gen_rerand, shift_gen);
    quat_mod_O0(&gen_rerand, &gen_rerand, norm);
    quat_alg_mul(&gen, &gen, &gen_rerand, &QUATALG_PINFTY);
    quat_mod_O0(&gen, &gen, norm);

    // in both cases, whether norm is prime or not prime, gen is not divisible by any integer factor of the target norm
    // therefore the call below will yield an ideal of the correct norm
    quat_ideal_create_O0_odd(ideal, split_gen, &gen, norm);
#ifndef NDEBUG
    ibz_sum_two_squares(&n_temp, &split_gen->coord.v[0], &split_gen->coord.v[1]);
    ibz_mul(&n_temp, &n_temp, &(ideal->norm));
    assert(ibz_cmp(norm, &n_temp) == 0);
#endif
    return (found);
}

void
quat_ideal_intersect_O0(quat_ideal_t *intersection,
                        const quat_ideal_t *a,
                        const quat_ideal_t *b,
                        const quat_alg_elem_t *split_a)
{
    assert(!(split_a != NULL) || ibz_is_one(&split_a->denom));
    ibz_t x2, y2, x, y, t, r, u2, v2;
    ibz_vec_2_t tmp, setter;
    ibz_init(&x2);
    ibz_init(&y2);
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&t);
    ibz_init(&r);
    ibz_init(&u2);
    ibz_init(&v2);
    ibz_vec_2_init(&tmp);
    ibz_vec_2_init(&setter);

    if (split_a == NULL) {
        // x2=b[0,2]/2
        ibz_copy(&x, &b->x);
        // y2=(b[1,2]-1)/2
        ibz_copy(&y, &b->y);
    } else {
        // setter = x2+iy2
        ibz_add(&setter.v[0], &b->x, &b->x);
        ibz_add(&setter.v[1], &b->y, &b->y);
        ibz_add(&setter.v[1], &setter.v[1], &ibz_const_one);

        ibz_copy(&tmp.v[0], &(split_a->coord).v[0]);
        ibz_neg(&tmp.v[1], &(split_a->coord).v[1]);
        ibz_add(&t, &b->norm, &b->norm);
        ibz_vec_2_gaussian_mul(&tmp, &tmp, &tmp);
        ibz_mod(&tmp.v[0], &tmp.v[0], &t);
        ibz_mod(&tmp.v[1], &tmp.v[1], &t);
        ibz_vec_2_gaussian_mul(&tmp, &setter, &tmp);
        ibz_mod(&tmp.v[0], &tmp.v[0], &t);
        ibz_mod(&tmp.v[1], &tmp.v[1], &t);

        ibz_mul(&u2, &split_a->coord.v[0], &split_a->coord.v[0]);
        ibz_mul(&v2, &split_a->coord.v[1], &split_a->coord.v[1]);
        ibz_add(&t, &u2, &v2);

        // t = (u^2+v^2)^{-1}mod N2
        ibz_invmod(&t, &t, &b->norm);
        assert(!ibz_is_odd(&tmp.v[0]));
        ibz_div_2exp(&x, &tmp.v[0], 1);
        // X2 = (X2 / 2) t mod N2
        ibz_mul(&x, &x, &t);
        ibz_mod(&x, &x, &b->norm);
        // Y2 = ((Y2 − u^2 - v^2 ) / 2) t mod N2
        ibz_sub(&y, &tmp.v[1], &u2);
        ibz_sub(&y, &y, &v2);
        assert(!ibz_is_odd(&y));
        ibz_div_2exp(&y, &y, 1);
        ibz_mul(&y, &y, &t);
        ibz_mod(&y, &y, &b->norm);
    }

    // x2=a[0,2]/2
    ibz_copy(&x2, &a->x);
    // y2=(a[1,2]-1)/2
    ibz_copy(&y2, &a->y);

    // x =  CRT([x1 , X2 ], [N1 , N2 ])
    // y =  CRT([y1 , Y2 ], [N1 , N2 ])
    ibz_crt(&x, &x2, &x, &a->norm, &b->norm);
    ibz_crt(&y, &y2, &y, &a->norm, &b->norm);

    ibz_mul(&r, &a->norm, &b->norm);
    // inert mat
    quat_xyn_to_inert_ideal(intersection, &x, &y, &r);
}

void
quat_ideal_mul_O0(quat_lattice_t *prod, const quat_ideal_t *a, const quat_ideal_t *b)
{
    ibz_t x1, y1, x2, y2, u, v, d, tmp, an2, bn2;
    ibz_init(&x1);
    ibz_init(&y1);
    ibz_init(&x2);
    ibz_init(&y2);
    ibz_init(&bn2);
    ibz_init(&an2);
    ibz_init(&u);
    ibz_init(&v);
    ibz_init(&d);
    ibz_init(&tmp);
    ibz_xgcd(&d, &u, &v, &a->norm, &b->norm);
    assert(ibz_is_one(&d));
    // X 1 = N 2 (−vx 1 mod (2N 1 ))
    ibz_add(&d, &a->x, &a->x);
    ibz_mul(&d, &v, &d);
    ibz_neg(&d, &d);
    ibz_add(&an2, &a->norm, &a->norm);
    ibz_mod(&d, &d, &an2);
    ibz_mul(&x1, &d, &b->norm);
    // X 2 ← N 1 (ux 2 mod (2N 2 ))
    ibz_add(&d, &b->x, &b->x);
    ibz_mul(&d, &u, &d);
    ibz_add(&bn2, &b->norm, &b->norm);
    ibz_mod(&d, &d, &bn2);
    ibz_mul(&x2, &d, &a->norm);
    // Y 1 ← N 2 (−vy 1 mod (2N 1 ))
    ibz_add(&tmp, &a->y, &a->y);
    ibz_add(&tmp, &tmp, &ibz_const_one);
    ibz_mul(&d, &v, &tmp);
    ibz_neg(&d, &d);
    ibz_mod(&d, &d, &an2);
    ibz_mul(&y1, &d, &b->norm);
    // Y 2 ← N 1 (uy 2 mod (2N 2 ))
    ibz_add(&tmp, &b->y, &b->y);
    ibz_add(&tmp, &tmp, &ibz_const_one);
    ibz_mul(&d, &u, &tmp);
    ibz_mod(&d, &d, &bn2);
    ibz_mul(&y2, &d, &a->norm);
    // make matrix
    ibz_mul(&tmp, &a->norm, &b->norm);
    ibz_set(&prod->denom, 2, 3);
    ibz_mat_4x4_identity(&prod->basis);
    ibz_add(&prod->basis.m[0][0], &tmp, &tmp);
    ibz_copy(&prod->basis.m[1][1], &prod->basis.m[0][0]);
    ibz_add(&prod->basis.m[0][2], &x2, &x1);
    ibz_sub(&prod->basis.m[1][2], &y2, &y1);
    ibz_sub(&prod->basis.m[1][3], &x2, &x1);
    ibz_add(&prod->basis.m[0][3], &y2, &y1);
    ibz_neg(&prod->basis.m[0][3], &prod->basis.m[0][3]);
}

// assumes lattice is exactly the one output by quat_ideal_mul_O0
void
quat_ideal_product_gram_matrix(ibz_mat_4x4_t *gram, const quat_lattice_t *lat)
{
    ibz_t tmp, r, s, t, ax2, bx2, nanb, ay, by;
    ibz_init(&ax2);
    ibz_init(&bx2);
    ibz_init(&ay);
    ibz_init(&by);
    ibz_init(&tmp);
    ibz_init(&r);
    ibz_init(&s);
    ibz_init(&t);
    ibz_init(&nanb);
    ibz_mat_4x4_zero(gram);
    assert(ibz_cmp(&lat->denom, &ibz_const_two) == 0);
    // xs
    ibz_sub(&ax2, &lat->basis.m[0][2], &lat->basis.m[1][3]);
    ibz_add(&bx2, &lat->basis.m[0][2], &lat->basis.m[1][3]);
    ibz_div_2exp(&ax2, &ax2, 1);
    ibz_div_2exp(&bx2, &bx2, 1);
    // assert(ibz_is_positive(&ax2));
    // assert(ibz_is_positive(&bx2));
    //  ys
    ibz_add(&by, &lat->basis.m[0][3], &lat->basis.m[1][2]);
    ibz_neg(&by, &by);
    ibz_sub(&ay, &lat->basis.m[1][2], &lat->basis.m[0][3]);
    ibz_div_2exp(&ay, &ay, 1);
    ibz_div_2exp(&by, &by, 1);
    // assert(ibz_is_positive(&ay));
    // assert(ibz_is_positive(&by));
    //  G1,1 ← 2N1 N2
    //  G2,2 ← 2N1 N2
    ibz_div_2exp(&nanb, &lat->basis.m[0][0], 1);
    ibz_copy(&gram->m[1][1], &lat->basis.m[0][0]);
    ibz_copy(&gram->m[0][0], &gram->m[1][1]);
    // G3,3 ← (p + 4(X1 + X2)2 + (Y2 − Y1)^2) / (2N1 N2)
    ibz_add(&tmp, &ax2, &bx2);
    ibz_mul(&gram->m[2][2], &tmp, &tmp);
    ibz_sub(&tmp, &ay, &by);
    ibz_mul(&tmp, &tmp, &tmp);
    ibz_add(&gram->m[2][2], &gram->m[2][2], &tmp);
    ibz_add(&gram->m[2][2], &gram->m[2][2], &QUATALG_PINFTY.p);
    ibz_div(&gram->m[2][2], &tmp, &gram->m[2][2], &gram->m[0][0]);
    assert(ibz_is_zero(&tmp));
    // A ← Y1 Y2
    ibz_mul(&r, &ay, &by);
    // B ← 4X1 X2
    ibz_mul(&s, &ax2, &bx2);
    // C ← (Y1 + 2X1)(Y2 + 2X2)
    ibz_add(&tmp, &ax2, &by);
    ibz_add(&t, &bx2, &ay);
    ibz_mul(&t, &t, &tmp);
    // G4,4 ← G3,3 + (A − B) / (N1 N2) = G3,3 + (Y1 Y2 − 4X1 X2 )/(N1 N2 )
    ibz_sub(&gram->m[3][3], &r, &s);
    ibz_div(&gram->m[3][3], &tmp, &gram->m[3][3], &nanb);
    assert(ibz_is_zero(&tmp));
    ibz_mul_2exp(&gram->m[3][3], &gram->m[3][3], 1);
    ibz_add(&gram->m[3][3], &gram->m[3][3], &gram->m[2][2]);
    // G3,4 ← (−C + A + B) / (N1 N2)= −2(X2 Y1 + X1 Y2 )/(N1 N2 )
    ibz_add(&gram->m[2][3], &r, &s);
    ibz_sub(&gram->m[2][3], &gram->m[2][3], &t);
    ibz_div(&gram->m[2][3], &tmp, &gram->m[2][3], &nanb);
    assert(ibz_is_zero(&tmp));
    // G1,3 ← 2(X1 + X2)
    ibz_add(&gram->m[0][2], &ax2, &bx2);
    // G1,4 ← −(Y1 + Y2)
    ibz_add(&gram->m[0][3], &ay, &by);
    ibz_neg(&gram->m[0][3], &gram->m[0][3]);
    // G2,3 ← (Y2 − Y1)
    ibz_sub(&gram->m[1][2], &ay, &by);
    // G2,4 ← 2(X2 − X1)
    ibz_sub(&gram->m[1][3], &bx2, &ax2);
    int norms_bound = ibz_get_bound(&nanb);
    int upper = norms_bound;
    if (ibz_bitsize(&QUATALG_PINFTY.p) > upper)
        upper = ibz_bitsize(&QUATALG_PINFTY.p);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) {
            ibz_set_bound(&gram->m[j][i], upper + 5);
            if (i != j)
                ibz_copy(&gram->m[i][j], &gram->m[j][i]);
        }
    }
}

int
quat_ideal_create_O0_inert(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm)
{
    int inert_two = 1;
    ibz_t n_odd, n_even;
    quat_alg_elem_t split, elem;
    ibz_vec_4_t coeffs;
    quat_ideal_t even;
    quat_ideal_t odd;
    ibz_init(&n_odd);
    ibz_init(&n_even);
    quat_ideal_init(&even);
    quat_ideal_init(&odd);
    quat_alg_elem_init(&split);
    quat_alg_elem_init(&elem);
    ibz_vec_4_init(&coeffs);
    int val = ibz_two_adic(norm);
    ibz_mul_2exp(&n_even, &ibz_const_one, val);
    ibz_div_2exp(&n_odd, norm, val);
    quat_alg_elem_copy(&elem, gen);
    quat_ideal_create_O0_odd(&odd, &split, &elem, &n_odd);
    quat_alg_norm(&n_odd, &n_even, &split, &QUATALG_PINFTY);
    assert(ibz_is_one(&n_even));
    int inert_odd = ibz_is_one(&n_odd);
    ibz_mul_2exp(&n_even, &ibz_const_one, val);
    if (ibz_two_adic(&n_even) > 0) {
        quat_mod_O0(&elem, &elem, &n_even);
        quat_change_to_O0_basis(&coeffs, &elem);
        quat_alg_elem_set(&split, 2, 1, -1, 0, 0);
        while (!ibz_is_one(&n_even) && (ibz_is_odd(&coeffs.v[2]) == ibz_is_odd(&coeffs.v[3])) &&
               ((ibz_is_even(&coeffs.v[1]) == ibz_is_even(&coeffs.v[0])) || ibz_is_even(&coeffs.v[3]))) {
            inert_two = 0;
            quat_alg_mul(&elem, &split, &elem, &QUATALG_PINFTY);
            quat_mod_O0(&elem, &elem, &n_even);
            quat_change_to_O0_basis(&coeffs, &elem);
            ibz_div_2exp(&n_even, &n_even, 1);
        }
    }
    int UNUSED check_inert_two = quat_ideal_create_O0_pow_two(&even, &elem, &n_even);
    assert(check_inert_two);
    quat_ideal_intersect_O0(ideal, &odd, &even, NULL);
    return (inert_odd && inert_two);
}

// qlapoty needs the equiv, other tests the gen
void
quat_ideal_shortest_equivalent(quat_alg_elem_t *gen,
                               quat_alg_elem_t *equiv,
                               quat_ideal_t *red,
                               const quat_ideal_t *ideal)
{
    ibz_vec_4_t coeffs;
    quat_alg_elem_t elem, tmp;
    quat_lattice_t lat;
    quat_ideal_t intermediary;
    ibz_t gcd, n, d, prod;
    ibz_init(&gcd);
    ibz_init(&n);
    ibz_init(&d);
    ibz_init(&prod);
    ibz_vec_4_init(&coeffs);
    quat_alg_elem_init(&elem);
    quat_alg_elem_init(&tmp);
    quat_lattice_init(&lat);
    quat_ideal_init(&intermediary);
    quat_ideal_reduce_basis(&lat, ideal);
    ibz_vec_4_set(&coeffs, 1, 0, 0, 0);
    ibz_mat_4x4_eval(&(elem.coord), &(lat.basis), &coeffs);
    ibz_copy(&elem.denom, &lat.denom);
    for (int i = 0; i < 2; i++)
        ibz_set_bound(&elem.coord.v[i], (ibz_bitsize(&QUATALG_PINFTY.p) / 2 + ibz_get_bound(&ideal->norm)) / 2 + 1);
    for (int i = 2; i < 4; i++) {
        int b = (-ibz_bitsize(&QUATALG_PINFTY.p) / 2 + ibz_get_bound(&ideal->norm)) / 2 + 1;
        if (b < 1) {
            // norm below sqrt(p): the shortest vector has no j/k-component at all
            assert(ibz_is_zero(&elem.coord.v[i]));
            b = 1;
        }
        ibz_set_bound(&elem.coord.v[i], b);
    }
    ibz_set_bound(&elem.denom, 3);
    if (equiv != NULL)
        quat_alg_elem_copy(equiv, &elem);

    quat_alg_norm(&n, &gcd, &elem, &QUATALG_PINFTY);
    assert(ibz_is_one(&gcd));
    ibz_div(&n, &gcd, &n, &ideal->norm);
    ibz_set_bound(&n, ibz_bitsize(&QUATALG_PINFTY.p) + 1);
    assert(ibz_is_zero(&gcd));
    ibz_gcd(&gcd, &n, &ideal->norm);
    ibz_copy(&tmp.denom, &lat.denom);

    quat_alg_conj(&elem, &elem);
    // quat_mod_O0(&elem, &elem, &n);
    if (!ibz_is_one(&gcd)) {
        ibz_mul(&prod, &n, &ideal->norm);
        quat_ideal_small_equivalent_coprime_enumeration(
            &tmp, &intermediary, &lat, &ideal->norm, &prod, &PRNG_default_domain);
        quat_alg_mul(&elem, &tmp, &elem, &QUATALG_PINFTY);
        ibz_mul(&elem.denom, &elem.denom, &ideal->norm);
        quat_alg_normalize(&elem);
        quat_mod_O0(&elem, &elem, &n);
    }
    quat_ideal_create_O0_inert(red, &elem, &n);
    // not an issue if it is not inert, since then the inert factor is the shortest equivlent
    if (gen != NULL)
        quat_alg_elem_copy(gen, &elem);

    // make canonical choice despite i
    ibz_mul_2exp(&d, &red->x, 1);
    if (ibz_cmp(&d, &red->norm) > 0) {
        ibz_sub(&red->x, &red->norm, &red->x);
        ibz_sub(&red->y, &red->norm, &red->y);
        ibz_sub(&red->y, &red->y, &ibz_const_one);
        quat_alg_elem_set(&elem, 1, 0, 1, 0, 0);
        if (equiv != NULL)
            quat_alg_mul(equiv, &elem, equiv, &QUATALG_PINFTY);
        if (gen != NULL)
            quat_alg_mul(gen, gen, &elem, &QUATALG_PINFTY);
    }
}

// requires the basis of ideal to be reduced
int
quat_ideal_small_equivalent_coprime_enumeration(quat_alg_elem_t *gen,
                                                quat_ideal_t *equiv,
                                                const quat_lattice_t *ideal,
                                                const ibz_t *ideal_norm,
                                                const ibz_t *coprime_to,
                                                prng_domain_ctx_t *prng_domain)
{
    int ret = 1;
    int found = 0;
    int prime = (coprime_to == NULL) || (ibz_is_zero(coprime_to));
    int add_check = (coprime_to == NULL) || (ibz_cmp(coprime_to, &ibz_const_two) != 0);
    ibz_t n, d, gcd;
    ibz_vec_4_t coeffs;
    quat_alg_elem_t elem;
    quat_alg_elem_init(&elem);
    ibz_init(&n);
    ibz_init(&d);
    ibz_init(&gcd);
    ibz_vec_4_init(&coeffs);
    assert(QUAT_equiv_bound_coeff > 0);
    ibz_copy(&elem.denom, &ideal->denom);

    while (!found) {
        for (int i = 0; i < 4; i++) {
            ret = ret && ibz_rand_interval_minm_m_with_domain(&(coeffs.v[i]), QUAT_equiv_bound_coeff, prng_domain);
            if (!ret)
                return (found);
        }
        if (ibz_vec_4_is_zero(&coeffs))
            continue;
        ibz_vec_4_content(&n, &coeffs);
        ibz_vec_4_scalar_div(&coeffs, &n, &coeffs);
        ibz_mat_4x4_eval(&(elem.coord), &(ideal->basis), &coeffs);

        int bound = (int)(log2(QUAT_equiv_bound_coeff) + 1);
        // adjusting the bound as we expect elem to have norm smaller than 4 ideal_norm * p * sampling_bound^2
        ibz_set_bound(&elem.coord.v[0], (ibz_get_bound(ideal_norm) + bound * 2 + ibz_bitsize(&QUATALG_PINFTY.p)) / 2);
        ibz_set_bound(&elem.coord.v[1], (ibz_get_bound(ideal_norm) + bound * 2 + ibz_bitsize(&QUATALG_PINFTY.p)) / 2);
        ibz_set_bound(&elem.coord.v[2], (ibz_get_bound(ideal_norm) + bound * 2) / 2);
        ibz_set_bound(&elem.coord.v[3], (ibz_get_bound(ideal_norm) + bound * 2) / 2);

        quat_alg_norm(&n, &d, &elem, &QUATALG_PINFTY);
        assert(ibz_is_one(&d));
        ibz_div(&n, &d, &n, ideal_norm);
        assert(ibz_is_zero(&d));
        found = ibz_is_odd(&n);

        if (found && add_check) {
            // check a_alpha invertible
            if (prime) {
                found = ibz_probab_prime(&n, QUAT_primality_num_iter);
            } else {
                ibz_gcd(&gcd, &n, coprime_to);
                found = ibz_is_one(&gcd);
            }
        }

        if (found) {
            ibz_set_bound(&n, bound * 2 + ibz_bitsize(&QUATALG_PINFTY.p) + 2);
            if (gen != NULL)
                quat_alg_elem_copy(gen, &elem);
            quat_alg_conj(&elem, &elem);
            quat_ideal_create_O0_odd(equiv, &elem, &elem, &n);
            // factoring out the split part
            if (gen != NULL) {
                quat_alg_mul(gen, &elem, gen, &QUATALG_PINFTY);
                quat_alg_norm(&n, &d, &elem, &QUATALG_PINFTY);
                assert(ibz_is_one(&d));
                ibz_vec_4_scalar_div(&gen->coord, &n, &gen->coord);
            }
        }
    }
    return (found);
}

int
quat_ideal_small_equivalent_coprime(quat_alg_elem_t *gen,
                                    quat_ideal_t *equiv,
                                    const quat_ideal_t *ideal,
                                    const ibz_t *coprime_to,
                                    prng_domain_ctx_t *prng_domain)
{
    int found = 0;
    ibz_t n, d, in;
    quat_lattice_t red;
    quat_alg_elem_t elem;
    quat_lattice_init(&red);
    quat_alg_elem_init(&elem);
    ibz_init(&n);
    ibz_init(&in);
    ibz_init(&d);
    quat_to_lattice(&red, ideal);
    quat_ideal_reduce_basis(&red, ideal);
    assert(QUAT_equiv_bound_coeff > 0);
    ibz_copy(&in, &ideal->norm);

    ibz_vec_4_copy_ibz(&elem.coord, &red.basis.m[0][3], &red.basis.m[1][3], &red.basis.m[2][3], &red.basis.m[3][3]);
    ibz_copy(&elem.denom, &red.denom);
    quat_alg_norm(&n, &d, &elem, &QUATALG_PINFTY);
    assert(ibz_is_one(&d));
    ibz_div(&n, &d, &n, &ideal->norm);
    assert(ibz_is_zero(&d));
    found = quat_ideal_small_equivalent_coprime_enumeration(gen, equiv, &red, &ideal->norm, coprime_to, prng_domain);
    if (gen != NULL) {
        quat_alg_normalize(gen);
        for (int j = 0; j < 4; j++)
            ibz_set_bound(&gen->coord.v[j], (ibz_get_bound(&in) + ibz_get_bound(&equiv->norm)) / 2 + 1);
        ibz_set_bound(&gen->denom, 3);
    }
    return (found);
}
