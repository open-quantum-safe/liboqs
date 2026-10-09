#include <ec.h>

#include <ct_testing.h>

void
ec_apply_isomorphism(ec_point_t *P, const ec_change_coord_matrix_t *M)
{
    fp2_t x, z, u, v;
    fp2_mul(&x, &P->x, &M->a);
    fp2_mul(&z, &P->z, &M->b);
    fp2_mul(&u, &P->x, &M->c);
    fp2_mul(&v, &P->z, &M->d);
    fp2_add(&P->x, &x, &z);
    fp2_add(&P->z, &u, &v);
}

void
ec_compute_montgomery_coefficient(ec_point_t *mont, const ec_point_t *th)
{
    fp2_t xx, zz;

    // xx = x^2, zz = z^2
    fp2_sqr(&xx, &th->x);
    fp2_sqr(&zz, &th->z);
    // xx = x^4, zz = y^4
    fp2_sqr(&xx, &xx);
    fp2_sqr(&zz, &zz);

    // A = 2(x^4+z^4)/(x^4-z^4)
    fp2_add(&mont->x, &xx, &zz);
    fp2_sub(&mont->z, &xx, &zz);
    fp2_add(&mont->x, &mont->x, &mont->x);
}

// Given a list of Fp2 element of length len, find max the biggest element and return the index of the maximum in the
// list
uint8_t
ec_find_max_coefficient_in_list(fp2_t *max, const fp2_t *list, const uint8_t len)
{
    uint32_t max_index = 0;
    uint32_t ctl;
    fp2_copy(max, &list[0]);
    for (uint8_t i = 1; i < len; i++) {
        ctl = fp2_less_than(max, &list[i]);
        fp2_select(max, max, &list[i], ctl);
        max_index = max_index ^ (((max_index) ^ (i)) & ctl);
    }
    return (uint8_t)max_index;
}

bool
ec_theta_to_montgomery(ec_curve_t *E, ec_change_coord_matrix_t *M, const ec_point_t *th)
{
    ec_point_t mont1, mont2, mont3;
    ec_point_t th2, th3;
    fp2_t ma, mb, ia, ib;
    fp2_t Montgomery_coefs[6];

    fp2_neg(&ma, &th->x);
    fp2_neg(&mb, &th->z);
    fp2_mul_by_i(&ia, &th->x, 0);
    fp2_mul_by_i(&ib, &th->z, 0);

    ec_curve_init(E);

    ec_compute_montgomery_coefficient(&mont1, th);

    // a2,b2 = a+b, a-b
    fp2_add(&th2.x, &th->x, &th->z);
    fp2_sub(&th2.z, &th->x, &th->z);
    ec_compute_montgomery_coefficient(&mont2, &th2);

    // a3,b3 = ia+b, a+ib
    fp2_add(&th3.x, &ia, &th->z);
    fp2_add(&th3.z, &th->x, &ib);
    ec_compute_montgomery_coefficient(&mont3, &th3);

    fp2_copy(&Montgomery_coefs[0], &mont1.z);
    fp2_copy(&Montgomery_coefs[1], &mont2.z);
    fp2_copy(&Montgomery_coefs[2], &mont3.z);
    fp2_batched_inv(Montgomery_coefs, 3);

    // If one of the three Montgomery coefficient is 0, the batched inversion returns only 0
    // At this point we detect that our theta null point was invalid
    uint32_t coef_is_zero = fp2_is_zero(&Montgomery_coefs[0]);
    // Validity check on secret-derived curve coefficients that never fails for honest inputs; only the pass/fail bit is
    // declassified, the coefficients stay tainted
    CT_TESTING_MAKE_PUBLIC(&coef_is_zero, sizeof(coef_is_zero));
    if (coef_is_zero)
        return 0;

    fp2_mul(&Montgomery_coefs[0], &mont1.x, &Montgomery_coefs[0]); // A1
    fp2_mul(&Montgomery_coefs[1], &mont2.x, &Montgomery_coefs[1]); // A2
    fp2_mul(&Montgomery_coefs[2], &mont3.x, &Montgomery_coefs[2]); // A3
    fp2_neg(&Montgomery_coefs[3], &Montgomery_coefs[0]);           // A1'
    fp2_neg(&Montgomery_coefs[4], &Montgomery_coefs[1]);           // A2'
    fp2_neg(&Montgomery_coefs[5], &Montgomery_coefs[2]);           // A3'

    uint8_t max = ec_find_max_coefficient_in_list(&E->A, Montgomery_coefs, 6);
    ec_normalize_curve_and_A24(E);

    // max =
    // 0:  M   = [ b,  a;  b, -a]
    // 1:  M2  = [-a,  b;  b, -a]
    // 2:  M3  = [ia, ib: -b,  a]
    // 3:  M'  = [ b,  a; -b,  a]
    // 4:  M2' = [-a,  b; -b,  a]
    // 5:  M3' = [ia, ib:  b, -a]
    if (M != NULL) {
        fp2_select(&M->a, &th->z, &ma, -((max == 1) || (max == 4)));
        fp2_select(&M->a, &M->a, &ia, -((max == 2) || (max == 5)));

        fp2_select(&M->b, &th->x, &th->z, -((max == 1) || (max == 4)));
        fp2_select(&M->b, &M->b, &ib, -((max == 2) || (max == 5)));

        fp2_select(&M->c, &th->z, &mb, -((max == 2) || (max == 3) || (max == 4)));
        fp2_select(&M->d, &th->x, &ma, -((max == 0) || (max == 1) || (max == 5)));
    }

    return 1;
}

bool
ec_normalize_montgomery(ec_curve_t *E, const ec_point_t *four_torsion, ec_basis_t *B)
{
    ec_change_coord_matrix_t M;

    if (!ec_theta_to_montgomery(E, &M, four_torsion))
        return 0;

    if (B != NULL) {
        ec_apply_isomorphism(&B->P, &M);
        ec_apply_isomorphism(&B->Q, &M);
        ec_apply_isomorphism(&B->PmQ, &M);
    }
    return 1;
}
