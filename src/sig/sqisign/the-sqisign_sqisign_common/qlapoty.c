#include <quaternion.h>
#include <lll.h>
#include "internal.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <quaternion_constants.h>
#include <quaternion_data.h>

#define DEBUG_PRINTS 1

// reduce a lattice such that we know its first line is in fact the first minima
void
quat_qlapoty_dim2_reduce_last_step(ibz_mat_2x2_t *red, const ibz_mat_2x2_t *ipt)
{
    ibz_mat_2x2_t mat;
    ibz_t n0, t, tmp;
    ibz_init(&n0);
    ibz_init(&t);
    ibz_init(&tmp);
    ibz_mat_2x2_init(&mat);
    ibz_sum_two_squares(&n0, &(ipt->m[0][0]), &(ipt->m[1][0]));
    ibz_mul(&tmp, &(ipt->m[0][0]), &(ipt->m[0][1]));
    ibz_mul(&t, &(ipt->m[1][0]), &(ipt->m[1][1]));
    ibz_add(&t, &tmp, &t);
    ibz_rounded_div(&tmp, &t, &n0);
    ibz_copy(&mat.m[0][0], &(ipt->m[0][0]));
    ibz_copy(&mat.m[1][0], &(ipt->m[1][0]));
    ibz_mul(&mat.m[0][1], &tmp, &(ipt->m[0][0]));
    ibz_sub(&mat.m[0][1], &(ipt->m[0][1]), &mat.m[0][1]);
    ibz_mul(&mat.m[1][1], &tmp, &(ipt->m[1][0]));
    ibz_sub(&mat.m[1][1], &(ipt->m[1][1]), &mat.m[1][1]);
    ibz_mat_2x2_copy(red, &mat);
}

void
quat_qlapoty_gen_to_dim2_lattice(ibz_mat_2x2_t *lat, ibz_vec_2_t *target, const quat_alg_elem_t *gen, const ibz_t *N)
{
    ibz_vec_2_t uv, ab, kl;
    ibz_t c, gcd, N4div, N4, r;
    ibz_vec_2_init(&ab);
    ibz_vec_2_init(&uv);
    ibz_vec_2_init(&kl);
    ibz_init(&c);
    ibz_init(&gcd);
    ibz_init(&N4div);
    ibz_init(&N4);
    ibz_init(&r);
    assert(ibz_cmp(&gen->denom, &ibz_const_two) == 0);
    ibz_copy(&ab.v[0], &gen->coord.v[0]);
    ibz_copy(&ab.v[1], &gen->coord.v[1]);
    ibz_mul_2exp(&N4, N, 2);
#ifndef NDEBUG
    ibz_gcd(&gcd, &ab.v[0], &ab.v[1]);
    ibz_gcd(&gcd, &gcd, &N4);
    assert(ibz_is_one(&gcd));
#endif
    // set gcd and Bézout coefficients
    ibz_gcd(&gcd, &ab.v[0], &N4);
    ibz_div(&N4div, &r, &N4, &gcd);
    assert(ibz_is_zero(&r));
    ibz_div(&c, &r, &ab.v[0], &gcd);
    assert(ibz_is_zero(&r));
    // compute c
    ibz_invmod(&c, &c, &N4div);
    ibz_mul(&c, &c, &ab.v[1]);
    ibz_neg(&c, &c);
    ibz_mod(&c, &c, &N4div);
    // copy into basis
    ibz_copy(&lat->m[0][0], &c);
    ibz_copy(&lat->m[1][0], &gcd);
    ibz_copy(&lat->m[0][1], &N4div);
    ibz_copy(&lat->m[1][1], &ibz_const_zero);
    // set target
    if (target != NULL) {
        ibz_xgcd(&gcd, &uv.v[0], &uv.v[1], &ab.v[0], &ab.v[1]);
        ibz_xgcd(&gcd, &kl.v[0], &kl.v[1], &gcd, &N4);
        assert(ibz_is_one(&gcd));
        ibz_xgcd(&gcd, &uv.v[0], &uv.v[1], &ab.v[0], &ab.v[1]);
        ibz_mul(&target->v[0], &kl.v[0], &uv.v[0]);
        ibz_mul(&target->v[1], &kl.v[0], &uv.v[1]);
        ibz_mod(&target->v[0], &target->v[0], &N4);
        ibz_mod(&target->v[1], &target->v[1], &N4);
    }
}

// norm/det must be bounded by 4N
static void
quat_qlapoty_dim2_ct_reduce(ibz_mat_2x2_t *Li, const ibz_t *N)
{
    ibz_t tmp;
    ibz_init(&tmp);

    // column swap
    ibz_copy(&tmp, &Li->m[0][0]);
    ibz_copy(&Li->m[0][0], &Li->m[0][1]);
    ibz_copy(&Li->m[0][1], &tmp);
    ibz_copy(&tmp, &Li->m[1][0]);
    ibz_copy(&Li->m[1][0], &Li->m[1][1]);
    ibz_copy(&Li->m[1][1], &tmp);

    // bounds
    int det_bits = ibz_get_bound(N) + 2;
    quat_lll_dim2_short_basis(Li, Li, det_bits);
}

void
quat_qlapoty_get_short_basis(ibz_mat_2x2_t *basis,
                             ibz_vec_2_t *target,
                             const ibz_vec_2_t *st,
                             const quat_alg_elem_t *alpha0,
                             const ibz_t *N)
{
    ibz_t gcd, N4, tmp, r, Nmod, c;
    ibz_vec_2_t uv, kl, omega;
    ibz_mat_2x2_t xy;
    ibz_vec_2_init(&uv);
    ibz_vec_2_init(&kl);
    ibz_vec_2_init(&omega);
    ibz_init(&gcd);
    ibz_init(&N4);
    ibz_init(&tmp);
    ibz_init(&r);
    ibz_init(&Nmod);
    ibz_init(&c);
    ibz_mat_2x2_init(&xy);
    assert(ibz_cmp(&alpha0->denom, &ibz_const_two) == 0);

    // setup
    ibz_mul_2exp(&N4, N, 2);
    ibz_vec_2_copy(&omega, st);
    quat_qlapoty_gen_to_dim2_lattice(basis, target, alpha0, N);
    //  copies
    ibz_copy(&c, &basis->m[0][0]);
    ibz_copy(&gcd, &basis->m[1][0]);
    ibz_copy(&Nmod, &basis->m[0][1]);
#ifndef NDEBUG
    ibz_mul(&tmp, &gcd, &Nmod);
    assert(ibz_cmp(&tmp, &N4) == 0);
#endif
    // a1
    ibz_vec_2_copy(&omega, st);
    ibz_div(&xy.m[0][0], &r, &omega.v[1], &gcd);
    assert(ibz_is_zero(&r));
    // a2
    ibz_mul(&xy.m[1][0], &c, &omega.v[1]);
    ibz_mul(&tmp, &gcd, &omega.v[0]);
    ibz_sub(&xy.m[1][0], &tmp, &xy.m[1][0]);
    ibz_div(&xy.m[1][0], &r, &xy.m[1][0], &N4);
    assert(ibz_is_zero(&r));
    // a3 a4
    ibz_xgcd(&r, &xy.m[1][1], &xy.m[0][1], &xy.m[0][0], &xy.m[1][0]);
    assert(ibz_is_one(&r));
    ibz_neg(&xy.m[0][1], &xy.m[0][1]);
    // basis
    ibz_mat_2x2_mul(basis, basis, &xy);
#ifndef NDEBUG
    // check proposed algo
    assert(ibz_cmp(&basis->m[0][0], &omega.v[0]) == 0);
    assert(ibz_cmp(&basis->m[1][0], &omega.v[1]) == 0);
    ibz_mul(&tmp, &xy.m[0][1], &c);
    ibz_mul(&r, &xy.m[1][1], &Nmod);
    ibz_add(&tmp, &tmp, &r);
    assert(ibz_cmp(&basis->m[0][1], &tmp) == 0);
    ibz_mul(&tmp, &xy.m[0][1], &gcd);
    assert(ibz_cmp(&basis->m[1][1], &tmp) == 0);
#endif
    int N_len = ibz_get_bound(N);

    ibz_set_bound(&basis->m[0][0], N_len + 10); // 10 is somewhat arbitrary, it is just to have a bit of margin
    ibz_set_bound(&basis->m[0][1], N_len + 10);
    ibz_set_bound(&basis->m[1][0], N_len + 10);
    ibz_set_bound(&basis->m[1][1], N_len + 10);
    quat_qlapoty_dim2_reduce_last_step(basis, basis);
}

// f and big interval needs to be of length 2
// 2d lattice Li should already be reduced
// C=1
// gen canonical generator (x+yi+j)/2
int
quat_qlapoty_initialize_bounds(ibz_vec_2_t *ab,
                               ibz_vec_2_t *omega,
                               quat_alg_elem_t *gen,
                               uint32_t *f_and_big_interval,
                               const ibz_t *N,
                               const ibz_mat_2x2_t *Li,
                               const ibz_t *twoe)
{
    ibz_vec_2_t B;
    quat_alg_elem_t fivevec;
    ibz_t li, Ba, tmp, gcd, fivef, r, cst, cst2;
    ibz_vec_2_init(&B);
    quat_alg_elem_init(&fivevec);
    ibz_init(&li);
    ibz_init(&Ba);
    ibz_init(&gcd);
    ibz_init(&cst);
    ibz_init(&cst2);
    ibz_init(&tmp);
    ibz_init(&r);
    ibz_init(&fivef);
    // get min
    ibz_copy(&omega->v[0], &Li->m[0][0]);
    ibz_copy(&omega->v[1], &Li->m[1][0]);
    ibz_sum_two_squares(&li, &omega->v[0], &omega->v[1]);
    // B2 (N) ← sqrt(5N/2) , B1 (N ) = sqrt(N/2)
    // but square everything
    ibz_set(&tmp, 5, 4);
    ibz_mul(&B.v[1], N, &tmp);
    ibz_div_2exp(&B.v[1], &B.v[1], 1); // B2^2  = 5*N/2
    ibz_div_2exp(&B.v[0], N, 1);       // B1^2  = 5*N/2
    //  Ba =  sqrt((2 ^(e+3) N ) - 16N 2 )/p), here also compute square
    ibz_mul_2exp(&Ba, twoe, 3);
    ibz_mul_2exp(&tmp, N, 4);
    ibz_sub(&Ba, &Ba, &tmp);
    ibz_mul(&Ba, &Ba, N);
    ibz_div(&Ba, &r, &Ba, &QUATALG_PINFTY.p);
    // if li ≤ 2B[0] /Ba: Return ⊥
    ibz_mul(&r, &li, &Ba);
    ibz_mul_2exp(&tmp, &B.v[0], 1);
    if (ibz_cmp(&r, &tmp) <= 0) {
        fprintf(stderr, "Too low norm in Qlapoty. This should never happen.\n");
        return (0);
    }
    // a = B[0]/li
    ibz_div(&ab->v[0], &r, &B.v[0], &li);
    if (!ibz_is_zero(&r))
        ibz_add(&ab->v[0], &ab->v[0], &ibz_const_one);
    // b = min Ba/2, B[1]/li
    ibz_div(&ab->v[1], &r, &B.v[1], &li);
    ibz_div_2exp(&tmp, &Ba, 1);
    if (ibz_cmp(&ab->v[1], &tmp) > 0)
        ibz_copy(&ab->v[1], &tmp);
    // assume log in base 2, and C=1
    ibz_sub(&tmp, &ab->v[1], &ab->v[0]);
    ibz_set(&cst, ibz_bitsize(N), 32);
    ibz_mul(&cst, &cst, &cst);
    f_and_big_interval[1] = (ibz_cmp(&tmp, &cst) > 0);
    if (!f_and_big_interval[1]) {
        ibz_gcd(&gcd, &omega->v[0], &omega->v[1]);
        f_and_big_interval[0] = 0;
        ibz_set(&tmp, 5, 4);
        ibz_set(&fivef, 1, 2);
        ibz_mul(&cst2, &cst, &cst);
        // is there a faster way to do this pecisely enough?
        while (ibz_divides(&gcd, &tmp) && (ibz_cmp(&fivef, &cst2) < 0)) {
            ibz_div(&gcd, &r, &gcd, &tmp);
            assert(ibz_is_zero(&r));
            ibz_mul(&fivef, &fivef, &tmp);
            f_and_big_interval[0] = f_and_big_interval[0] + 1;
        }
        if (f_and_big_interval[0] != 0) {
            ibz_div(&omega->v[0], &r, &omega->v[0], &fivef);
            assert(ibz_is_zero(&r));
            ibz_div(&omega->v[1], &r, &omega->v[1], &fivef);
            assert(ibz_is_zero(&r));
            ibz_mul_2exp(&r, &omega->v[0], 1);
            ibz_sub(&r, &r, &omega->v[1]);
            // r needed for test in 2 lines
            quat_alg_elem_set(&fivevec, 1, 2, 1, 0, 0);
            // tmp is still 5
            if (ibz_divides(&r, &tmp)) {
                ibz_neg(&fivevec.coord.v[1], &fivevec.coord.v[1]);
            }
            // maybe implement fast exponentiation if needed
            for (uint32_t i = 1; i < f_and_big_interval[0]; i++) {
                ibz_vec_2_gaussian_mul((ibz_vec_2_t *)&fivevec.coord.v[0],
                                       (ibz_vec_2_t *)&fivevec.coord.v[0],
                                       (ibz_vec_2_t *)&fivevec.coord.v[0]);
            }
            ibz_vec_2_gaussian_mul(omega, (ibz_vec_2_t *)&fivevec.coord.v[0], omega);
            quat_alg_mul(gen, &fivevec, gen, &QUATALG_PINFTY);
        }
        // set ouput
        ibz_mul(&ab->v[0], &fivef, &ab->v[0]);
        ibz_mul(&ab->v[1], &fivef, &ab->v[1]);
        ibz_sub(&tmp, &ab->v[1], &ab->v[0]);
        f_and_big_interval[1] = (ibz_cmp(&tmp, &cst) > 0);
    } else {
        f_and_big_interval[0] = 0;
    }

    return (1);
}

// big_interval is 1 if b-a>=ClogN^2
void
quat_qlapoty_loop_one(quat_alg_elem_t *alpha0,
                      ibz_vec_2_t *short_solution,
                      const quat_alg_elem_t *gen,
                      const ibz_t *N,
                      const ibz_vec_2_t *absq,
                      const ibz_vec_2_t *omega,
                      uint32_t f,
                      int big_interval)
{
    int size_k;
    ibz_vec_2_t k, onebounds, twobounds, gamma, st;
    ibz_t tmp, q, r, sum, ksq, ngamma, gcdomega;
    quat_alg_elem_t work;
    ibz_init(&tmp);
    ibz_init(&sum);
    ibz_init(&q);
    ibz_init(&r);
    ibz_init(&gcdomega);
    ibz_init(&ksq);
    ibz_init(&ngamma);
    ibz_vec_2_init(&k);
    ibz_vec_2_init(&onebounds);
    ibz_vec_2_init(&twobounds);
    ibz_vec_2_init(&gamma);
    ibz_vec_2_init(&st);
    quat_alg_elem_init(&work);

    // sampling bounds for k1
    // gcd omega
    ibz_gcd(&gcdomega, &omega->v[0], &omega->v[1]);

    // sqrt{5^fa^2/2}
    ibz_div_2exp(&tmp, &absq->v[0], 1);
    ibz_sqrt_floor(&onebounds.v[0], &tmp);
    // rounding up
    ibz_mul(&sum, &onebounds.v[0], &onebounds.v[0]);
    if (!(ibz_cmp(&sum, &tmp) == 0) && ibz_is_even(&absq->v[0]))
        ibz_add(&onebounds.v[0], &onebounds.v[0], &ibz_const_one);

    // bsqrt{5^f} with sqrt rounded up this time
    ibz_sqrt_floor(&onebounds.v[1], &absq->v[1]);

    int need_random_sampling = 1; // boolean indicating the need to sample randomly or not.
    ibz_set(&tmp, 11, 5);
    if (ibz_cmp(&onebounds.v[1], &tmp) != 1) // bounds_1 <= 11 < sqrt(5^3):  sol = (4 + 3i)
    {
        ibz_set(&k.v[0], 4, 4);
        ibz_set(&k.v[1], 3, 4);
        need_random_sampling = 0;
    }
    ibz_set(&tmp, 4, 4);
    if (ibz_cmp(&onebounds.v[1], &tmp) != 1) // bounds_1 <= 4 < 5:  sol = (2 + i)
    {
        ibz_set(&k.v[0], 2, 3);
        ibz_set(&k.v[1], 1, 3);
        need_random_sampling = 0;
    }
    ibz_set(&tmp, 2, 3);
    if (ibz_cmp(&onebounds.v[1], &tmp) != 1) // bounds_1 <= 2 < sqrt(5):  sol = 1
    {
        ibz_set(&k.v[0], 1, 2);
        ibz_set(&k.v[1], 0, 2);
        need_random_sampling = 0;
    }

    while (1) {
        if (need_random_sampling) {
            assert(ibz_cmp(&onebounds.v[0], &onebounds.v[1]) < 0);
            // sample k1
            ibz_rand_interval(&k.v[0], &onebounds.v[0], &onebounds.v[1]);
            // sqrt(max (0,(a^2-k1^2)) rounded up
            ibz_mul(&ksq, &k.v[0], &k.v[0]);
            if (ibz_cmp(&absq->v[0], &ksq) > 0) {
                ibz_sub(&tmp, &absq->v[0], &ksq);
                ibz_sqrt_floor(&twobounds.v[0], &tmp);
                ibz_mul(&sum, &twobounds.v[0], &twobounds.v[0]);
                if (ibz_cmp(&sum, &tmp) < 0)
                    ibz_add(&twobounds.v[0], &twobounds.v[0], &ibz_const_one);
            } else {
                ibz_set(&twobounds.v[0], 0, 0);
            }
            // sqrt(b^2-k1^2) rounded down
            ibz_sub(&tmp, &absq->v[1], &ksq);
            ibz_sqrt_floor(&twobounds.v[1], &tmp);

            // in case somehow the value of k1 we choose was too small to permit the existence of a solution we retry
            if (ibz_cmp(&twobounds.v[0], &twobounds.v[1]) > 0) {
                continue;
            }

            // sample k2
            ibz_rand_interval(&k.v[1], &twobounds.v[0], &twobounds.v[1]);
            // end samping
        }
        if (ibz_is_even(&k.v[0]) == ibz_is_even(&k.v[1]))
            continue;
        ibz_gcd(&tmp, &k.v[1], &k.v[0]);
        if (!ibz_is_one(&tmp))
            continue;
        ibz_mul(&tmp, &k.v[1], &k.v[1]);
        ibz_mul(&ksq, &k.v[0], &k.v[0]);
        ibz_add(&tmp, &ksq, &tmp);

        if (big_interval) {
            ibz_gcd(&tmp, &tmp, N);
            if (!ibz_is_one(&tmp))
                continue;
        } else {
            ibz_gcd(&tmp, &tmp, &gcdomega);
            if (!ibz_is_one(&tmp))
                continue;
        }
        if (ibz_is_odd(&k.v[0]))
            ibz_swap(&k.v[0], &k.v[1]);
        if (f & 1) {
            ibz_swap(&k.v[0], &k.v[1]);
            ibz_neg(&k.v[0], &k.v[0]);
        }

        // gaussian gcd (continue line 9)

        ibz_neg(&k.v[1], &k.v[1]);
        ibz_vec_2_gaussian_gcd(&gamma, &k, omega);
        ibz_neg(&k.v[1], &k.v[1]);
        // ngamma
        ibz_sum_two_squares(&ngamma, &gamma.v[0], &gamma.v[1]);

        size_k = 2 * ibz_get_bound(&k.v[0]);
        if (size_k < 2 * ibz_get_bound(&k.v[1]))
            size_k = 2 * ibz_get_bound(&k.v[1]);

        while (!ibz_is_one(&ngamma)) {
            // update k
            ibz_vec_2_gaussian_mul(&gamma, &gamma, &gamma);
            ibz_vec_2_gaussian_mul(&k, &k, &gamma);
            ibz_div(&k.v[0], &r, &k.v[0], &ngamma);
            assert(ibz_is_zero(&r));
            ibz_div(&k.v[1], &r, &k.v[1], &ngamma);
            assert(ibz_is_zero(&r));

            ibz_set_bound(&k.v[0], size_k);
            ibz_set_bound(&k.v[1], size_k);

            // prepare for next test
            ibz_neg(&k.v[1], &k.v[1]);
            ibz_vec_2_gaussian_gcd(&gamma, &k, omega);
            ibz_neg(&k.v[1], &k.v[1]);

            // ngamma
            ibz_sum_two_squares(&ngamma, &gamma.v[0], &gamma.v[1]);
        }
        ibz_vec_2_gaussian_mul(&gamma, &gamma, &gamma);
        ibz_vec_2_gaussian_mul(&k, &k, &gamma);

        ibz_div(&k.v[0], &r, &k.v[0], &ngamma);
        assert(ibz_is_zero(&r));
        ibz_div(&k.v[1], &r, &k.v[1], &ngamma);
        assert(ibz_is_zero(&r));

        ibz_set_bound(&k.v[0], (ibz_get_bound(N) + ibz_bitsize(&QUATALG_PINFTY.p)) / 2);
        ibz_set_bound(&k.v[1], (ibz_get_bound(N) + ibz_bitsize(&QUATALG_PINFTY.p)) / 2);

        // alpha0
        quat_alg_elem_set(&work, 1, 0, 0, 0, 0);
        ibz_vec_2_copy((ibz_vec_2_t *)&work.coord.v[0], &k);

        quat_alg_mul(&work, &work, gen, &QUATALG_PINFTY);
        assert(ibz_cmp(&work.denom, &ibz_const_two) == 0);
        ibz_gcd(&tmp, &work.coord.v[0], &work.coord.v[1]);
        ibz_gcd(&tmp, N, &tmp);
        if (!ibz_is_one(&tmp))
            continue;
        break;
    }
    ibz_vec_2_gaussian_mul(&st, &k, omega);
    quat_alg_elem_copy(alpha0, &work);
    ibz_vec_2_copy(short_solution, &st);
}

// For loop 2
int
quat_qlapoty_loop_two(ibz_t *l,
                      ibz_t *s,
                      ibz_t *t,
                      ibz_t *sc,
                      ibz_t *tc,
                      const quat_alg_elem_t *alpha0,
                      const ibz_t *r,
                      const ibz_vec_2_t *st0,
                      const ibz_t *N,
                      const ibz_t *twoe)
{
    int found = 0;
    ibz_t lambda, lambdainv, k, tmp, rtwo, twoemdN, quadN, sum, z, bound, Rdet, deltaN;
    ibz_vec_2_t target, target0, targetl;
    ibz_mat_2x2_t R, Rinv;
    ibz_init(&k);
    ibz_init(&Rdet);
    ibz_init(&tmp);
    ibz_init(&sum);
    ibz_init(&rtwo);
    ibz_init(&quadN);
    ibz_init(&twoemdN);
    ibz_init(&lambda);
    ibz_init(&lambdainv);
    ibz_init(&z);
    ibz_init(&deltaN);
    ibz_init(&bound);
    ibz_vec_2_init(&target);
    ibz_vec_2_init(&target0);
    ibz_vec_2_init(&targetl);
    ibz_mat_2x2_init(&R);
    ibz_mat_2x2_init(&Rinv);
    // Delta seems opposite to note, check what is right
    assert(ibz_cmp(N, &ibz_const_zero) > 0);
    ibz_set(&deltaN, 1 + 2 * (ibz_is_odd(r) == 0), 3);
    ibz_mul(&deltaN, &deltaN, N);
    // setup lattices
    quat_qlapoty_get_short_basis(&R, &target0, st0, alpha0, N);
    ibz_mat_2x2_inv_with_det_as_denom(&Rinv, &Rdet, &R);
    ibz_neg(&target0.v[0], &target0.v[0]);
    ibz_neg(&target0.v[1], &target0.v[1]);

    int N_len = ibz_get_bound(N);
    int e = ibz_get_bound(twoe);

    ibz_set_bound(&target0.v[0], 2 * N_len + 10); // 10 is somewhat arbitrary, just to give a bit of margin
    ibz_set_bound(&target0.v[1], 2 * N_len + 10);

    ibz_set(&lambda, 1, 2);
    ibz_neg(&lambda, &lambda);
    ibz_mul_2exp(&rtwo, r, 1);

    ibz_sub(&twoemdN, twoe, &deltaN); // 2^e - delta N
    ibz_mul_2exp(&quadN, N, 2);       // 4N
    ibz_div(&bound, &z, twoe, r);
    ibz_sqrt_floor(&bound, &bound);

    while (ibz_cmp(&lambda, &bound) <= 0) {
        // get lambda
        ibz_add(&lambda, &lambda, &ibz_const_two);
        ibz_set_bound(&lambda, 50); // the loop would make the size of lambda grow artificially for no reason otherwise,
        // 50 is to have a bit of margin in unlucky cases where we need to try a lot of lambda, the exact values does
        // not matter too much all the values involved are much smaller than the maximal integer bound anyway

        ibz_gcd(&tmp, &lambda, &quadN);
        if (!ibz_is_one(&tmp))
            continue;
        // get s,t
        ibz_mul(&tmp, &rtwo, &lambda); // tmp = 2 r lambda
        ibz_mul(&tmp, &tmp, &lambda);  // tmp = 2 r lambda^2
        ibz_sub(&sum, &twoemdN, &tmp); // sum = 2^e-deltaN-2 r lambda^2
        ibz_set_bound(&sum, e);

        ibz_invmod(&lambdainv, &lambda, &quadN); //  sum = (2^e -deltaN -2r lambda^2 )
        ibz_mul(&tmp, &sum, &lambdainv);         // tmp =  (2^e -deltaN -2r lambda^2 )(lambda)^{-1})
        ibz_mod(&tmp, &tmp, &quadN);             // tmp = (2^e -deltaN -2r lambda^2 )lambdainv mod 4N
        ibz_set_bound(&tmp, N_len + 10);
        ibz_mul(&targetl.v[0], &target0.v[0], &tmp); // -targetl target0 tmp
        ibz_mul(&targetl.v[1], &target0.v[1], &tmp);
        ibz_neg(&targetl.v[0], &targetl.v[0]);
        ibz_neg(&targetl.v[1], &targetl.v[1]);
        ibz_mod(&targetl.v[0], &targetl.v[0], &quadN);
        ibz_mod(&targetl.v[1], &targetl.v[1], &quadN);

        ibz_mat_2x2_eval(&target, &Rinv, &targetl);
        // inv is actually Rinv * 2N (matrix determinant), since it is integer.
        // So divide and round
        ibz_rounded_div(&target.v[0], &target.v[0], &Rdet);
        ibz_rounded_div(&target.v[1], &target.v[1], &Rdet);
        ibz_mat_2x2_eval(&target, &R, &target);
        ibz_sub(&target.v[0], &targetl.v[0], &target.v[0]);
        ibz_sub(&target.v[1], &targetl.v[1], &target.v[1]);

        ibz_set_bound(&target.v[0], 2 * N_len + 10);
        ibz_set_bound(&target.v[1], 2 * N_len + 10);

#ifndef NDEBUG
        ibz_t x, y;
        ibz_mat_2x2_t test;
        ibz_mat_2x2_init(&test);
        ibz_init(&x);
        ibz_init(&y);
        // inverse test
        ibz_mat_2x2_inv_with_det_as_denom(&test, &x, &R);
        assert(ibz_cmp(&((Rinv)).m[0][0], &test.m[0][0]) == 0);
        assert(ibz_cmp(&((Rinv)).m[0][1], &test.m[0][1]) == 0);
        assert(ibz_cmp(&((Rinv)).m[1][0], &test.m[1][0]) == 0);
        assert(ibz_cmp(&((Rinv)).m[1][1], &test.m[1][1]) == 0);
        assert(ibz_cmp(&Rdet, &x) == 0);
        // inverse test lambda
        ibz_mul(&x, &lambda, &lambdainv);
        ibz_mod(&x, &x, &quadN);
        assert(ibz_is_one(&x));
        // homogeneous test
        ibz_vec_2_t test_v;
        ibz_vec_2_init(&test_v);
        ibz_vec_2_set(&test_v, 5, -7);
        ibz_mat_2x2_eval(&test_v, &R, &test_v);
        ibz_mul(&x, &alpha0->coord.v[0], &test_v.v[0]);
        ibz_mul(&y, &alpha0->coord.v[1], &test_v.v[1]);
        ibz_add(&x, &x, &y);
        ibz_mul(&x, &x, &lambda);
        ibz_div(&y, &x, &x, &quadN);
        assert(ibz_is_zero(&x));

        // equation test
        ibz_mul(&x, &alpha0->coord.v[0], &target.v[0]);
        ibz_mul(&y, &alpha0->coord.v[1], &target.v[1]);
        ibz_add(&x, &x, &y);
        ibz_mul(&x, &x, &lambda);
        ibz_sub(&x, &sum, &x);
        ibz_div(&x, &y, &x, &quadN);
        assert(ibz_is_zero(&y));
#endif

        // Test solution is alternating sign
        if (ibz_is_even(&target.v[0]) == ibz_is_even(&target.v[1]))
            continue;
        // get k
        ibz_add(&k, &sum, &deltaN);                       // k = 2^e-deltaN -2 r lambda^2 + deltaN = 2^e-2r lambda^2
        ibz_mul(&tmp, &target.v[0], &alpha0->coord.v[0]); // tmp = 2as
        ibz_mul(&sum, &target.v[1], &alpha0->coord.v[1]); // sum=2bt
        ibz_add(&sum, &tmp, &sum);                        // sum = 2as+2bt
        ibz_mul(&tmp, &sum, &lambda);                     // tmp = 2as lambda +2bt lambda
        ibz_set_bound(&tmp, 2 * N_len + 20);
        ibz_sub(&k, &k, &tmp); // k = 2^e-2 r lambda^2- 2as lambda - 2bt lambda
        ibz_div(&k, &tmp, &k, N);

        ibz_set_bound(&k, e);
        assert(ibz_is_zero(&tmp));
        assert(ibz_is_odd(&k));

        // z
        ibz_add(&z, &k, &k);
        ibz_mul(&sum, &target.v[0], &target.v[0]);
        ibz_mul(&tmp, &target.v[1], &target.v[1]);

        ibz_sub(&z, &z, &tmp);
        ibz_sub(&z, &z, &sum);

        ibz_set_bound(&z, 2 * N_len + 10);

        // z size
        if (ibz_cmp(&z, &ibz_const_zero) < 0)
            continue;

        // z = 1 mod 4
        assert((ibz_get(&z) & 3) == 1);

        // Odd solutions
        assert((((ibz_get(r) % 8) + (((ibz_get(&k) % 8) - 1) & 3) / 2) & 1) == 1);

        // Cornacchia
        if (((ibz_get(&z) & 3) == 1) && ibz_probab_prime(&z, QUAT_primality_num_iter)) {
            found = ibz_cornacchia_prime(sc, tc, &z);
        }

        else
            found = 0;
        if (found)
            break;
    }
    if (found) {
        ibz_copy(s, &target.v[0]);
        ibz_copy(t, &target.v[1]);
        ibz_set_bound(s, N_len + 10);
        ibz_set_bound(t, N_len + 10);
        ibz_copy(l, &lambda);
#ifndef NDEBUG
        ibz_t tn1, tn2, tt;
        ibz_init(&tt);
        ibz_init(&tn1);
        ibz_init(&tn2);
        ibz_sum_two_squares(&tt, sc, tc);
        assert(ibz_cmp(&z, &tt) == 0);
        ibz_sum_two_squares(&tt, s, t);
        ibz_add(&tt, &z, &tt);
        assert(ibz_is_even(&tt));
        ibz_div_2exp(&tt, &tt, 1);
        assert(ibz_cmp(&k, &tt) == 0);
        ibz_mul(&tt, &k, N);
        // lattice part: tn2 = lambda(2as + 2bt)
        assert(ibz_cmp(&alpha0->denom, &ibz_const_two) == 0);
        ibz_mul(&tn1, &alpha0->coord.v[0], s);
        ibz_mul(&tn2, &alpha0->coord.v[1], t);
        ibz_add(&tn2, &tn2, &tn1);
        ibz_mul(&tn2, &tn2, l);
        // other part: tn1 = 2^e-2rlambda^2-tn2
        ibz_mul(&tn1, l, l);
        ibz_mul(&tn1, &tn1, r);
        ibz_add(&tn1, &tn1, &tn1);
        ibz_sub(&tn1, twoe, &tn1);
        ibz_sub(&tn1, &tn1, &tn2);
        assert(ibz_cmp(&tt, &tn1) == 0);

#endif
    }
    return (found);
}

int
quat_qlapoty_normeq(quat_alg_elem_t *mu1,
                    quat_alg_elem_t *mu2,
                    quat_alg_elem_t *theta,
                    quat_alg_elem_t *smallest,
                    const quat_ideal_t *ideal)
{
    int found = 0;
    ibz_t s, t, r, sc, tc, lambda, twoe;
    quat_alg_elem_t gen, temp, alpha0;
    quat_ideal_t small;
    ibz_vec_2_t ab, omega, st0;
    ibz_mat_2x2_t Li;
    uint32_t f_and_big_interval[2] = { 0, 0 };
    ibz_init(&s);
    ibz_init(&t);
    ibz_init(&sc);
    ibz_init(&tc);
    ibz_init(&r);
    ibz_init(&lambda);
    ibz_init(&twoe);
    quat_alg_elem_init(&gen);
    quat_alg_elem_init(&temp);
    quat_alg_elem_init(&alpha0);
    quat_ideal_init(&small);
    ibz_vec_2_init(&ab);
    ibz_vec_2_init(&omega);
    ibz_vec_2_init(&st0);
    ibz_mat_2x2_init(&Li);

    quat_ideal_shortest_equivalent(NULL, smallest, &small, ideal);
    ibz_set_bound(&small.norm, ibz_bitsize(&QUATALG_PINFTY.p) / 2 + 10);

    // compute gen
    quat_ideal_odd_inert_gen(&gen, &small);
    ibz_set_bound(&gen.coord.v[0], ibz_get_bound(&small.norm));
    ibz_set_bound(&gen.coord.v[1], ibz_get_bound(&small.norm));
    ibz_set_bound(&gen.coord.v[2], 2);
    ibz_set_bound(&gen.coord.v[3], 1);
    ibz_set_bound(&gen.denom, 3);
    // compute 2d basis and minima
    quat_qlapoty_gen_to_dim2_lattice(&Li, NULL, &gen, &small.norm);
    quat_qlapoty_dim2_ct_reduce(&Li, &small.norm);

    // bound adjustment
    ibz_mul_2exp(&twoe, &ibz_const_one, QUAT_qlapoty_used_power_of_two);
    found = quat_qlapoty_initialize_bounds(&ab, &omega, &gen, f_and_big_interval, &small.norm, &Li, &twoe);

    if (!found)
        return (found);

#ifndef NDEBUG
    // test omega is in lattice, and lattice is correct
    ibz_t tmp1, sum1;
    ibz_init(&tmp1);
    ibz_init(&sum1);
    ibz_mul(&tmp1, &omega.v[0], &gen.coord.v[0]);
    ibz_mul(&sum1, &omega.v[1], &gen.coord.v[1]);
    ibz_add(&sum1, &tmp1, &sum1);
    ibz_mul_2exp(&tmp1, &small.norm, 2);
    ibz_mod(&sum1, &sum1, &tmp1);
    assert(ibz_is_zero(&sum1));
#endif

    // find alpha0
    quat_qlapoty_loop_one(&alpha0, &st0, &gen, &small.norm, &ab, &omega, f_and_big_interval[0], f_and_big_interval[1]);

    // compute r
    quat_alg_norm(&r, &t, &alpha0, &QUATALG_PINFTY);
    assert(ibz_is_one(&t));
    ibz_div(&r, &t, &r, &small.norm);
    assert(ibz_is_zero(&t));

    // find lambda
    found = quat_qlapoty_loop_two(&lambda, &s, &t, &sc, &tc, &alpha0, &r, &st0, &small.norm, &twoe);
    if (!found) {
        fprintf(stderr, "Qlapoty loop2 found no solution. This should never happen.\n");
        return (found);
    }

    if (ibz_is_even(&s) != ibz_is_even(&sc))
        ibz_swap(&sc, &tc);
    assert(ibz_is_even(&s) == ibz_is_even(&sc));
    assert(ibz_is_even(&t) == ibz_is_even(&tc));

    // set output
    quat_alg_elem_set(mu1, 2, 0, 0, 0, 0);
    quat_alg_elem_set(mu2, 2, 0, 0, 0, 0);
    ibz_add(&mu1->coord.v[0], &s, &sc);
    ibz_add(&mu1->coord.v[1], &t, &tc);
    ibz_sub(&mu2->coord.v[0], &s, &sc);
    ibz_sub(&mu2->coord.v[1], &t, &tc);
    quat_alg_elem_scalar_mul(&temp, &lambda, &alpha0);
    quat_alg_elem_scalar_mul(mu1, &small.norm, mu1);
    quat_alg_elem_scalar_mul(mu2, &small.norm, mu2);
    quat_alg_add(mu1, mu1, &temp);
    quat_alg_add(mu2, mu2, &temp);

#ifndef NDEBUG
    ibz_t tn1, tn2, tt;
    ibz_init(&tt);
    ibz_init(&tn1);
    ibz_init(&tn2);
    quat_alg_norm(&tn1, &tt, mu1, &QUATALG_PINFTY);
    assert(ibz_is_one(&tt));
    quat_alg_norm(&tn2, &tt, mu2, &QUATALG_PINFTY);
    assert(ibz_is_one(&tt));
    ibz_add(&tn1, &tn1, &tn2);
    ibz_div(&tn2, &tt, &tn1, &small.norm);
    assert(ibz_is_zero(&tt));
    assert(ibz_cmp(&tn2, &twoe) == 0);
#endif
    quat_alg_conj(&temp, mu1);
    ibz_mul(&temp.denom, &temp.denom, &small.norm);
    quat_alg_mul(theta, mu2, &temp, &QUATALG_PINFTY);

    return (found);
}

int
quat_qlapoty(quat_alg_elem_t *beta1, ibz_t *d1, quat_alg_elem_t *theta, const quat_ideal_t *ideal)
{
    quat_alg_elem_t mu1, mu2, small;
    ibz_t d, n;
    ibz_init(&d);
    ibz_init(&n);
    quat_alg_elem_init(&mu1);
    quat_alg_elem_init(&mu2);
    quat_alg_elem_init(&small);

    int found = quat_qlapoty_normeq(&mu1, &mu2, theta, &small, ideal);

    if (found) {
        // theta
        quat_alg_normalize(theta);
        quat_alg_conj(theta, theta);

        // small ideal norm
        quat_alg_norm(&n, &d, &small, &QUATALG_PINFTY);
        assert(ibz_is_one(&d));
        ibz_div(&n, &d, &n, &ideal->norm);
        assert(ibz_is_zero(&d));
        ibz_set_bound(&n, (ibz_bitsize(&QUATALG_PINFTY.p) / 2) + 1);

        // norm d1
        quat_alg_norm(d1, &d, &mu1, &QUATALG_PINFTY);
        assert(ibz_is_one(&d));
        ibz_div(d1, &d, d1, &n);
        assert(ibz_is_zero(&d));
        ibz_set_bound(d1, QUAT_qlapoty_used_power_of_two + 2);

        // beta1
        quat_alg_mul(beta1, &mu1, &small, &QUATALG_PINFTY);
        ibz_mul(&beta1->denom, &beta1->denom, &n);

        quat_alg_normalize(beta1);

        for (int i = 0; i < 4; i++)
            ibz_set_bound(&beta1->coord.v[i], (ibz_get_bound(&ideal->norm) + ibz_get_bound(d1)) / 2 + 3);
        ibz_set_bound(&beta1->denom, 3);

        for (int i = 0; i < 4; i++)
            ibz_set_bound(&theta->coord.v[i], 2 * (ibz_get_bound(&beta1->coord.v[0]) + 1));
        ibz_set_bound(&theta->denom, 4);
    }

    return (found);
}
