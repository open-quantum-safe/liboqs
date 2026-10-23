#include "splitting.h"

#include <ct_testing.h>

static inline void
splitting_change_coord_to_product_theta_struct(theta_point_t *P)
{
    fp2_t t1, t2, t3, t4;

    // t1 = x + z
    fp2_add(&t1, &P->x, &P->z);
    // t2 = y + t
    fp2_add(&t2, &P->y, &P->t);
    // t3 = y - t
    fp2_sub(&t3, &P->y, &P->t);
    // t4 = x - z
    fp2_sub(&t4, &P->x, &P->z);

    fp2_copy(&P->x, &t1);
    fp2_copy(&P->y, &t2);
    fp2_copy(&P->z, &t3);
    fp2_copy(&P->t, &t4);
}

bool
splitting_tensor_product_to_couple_point(theta_couple_point_t *res, const theta_point_t *P)
{
    // Sanity check
    uint32_t is_product = theta_is_product_theta_point(P);
    // Validity check on secret-derived theta coordinates that never fails for honest inputs; only the pass/fail bit is
    // declassified, the coordinates stay tainted
    CT_TESTING_MAKE_PUBLIC(&is_product, sizeof(is_product));
    if (!is_product)
        return 0;
    fp2_copy(&res->P2.x, &P->x);
    fp2_copy(&res->P2.z, &P->y);

    fp2_copy(&res->P1.x, &P->x);
    fp2_copy(&res->P1.z, &P->z);

    return 1;
}

bool
splitting_check_valid_theta_null_point(const ec_point_t *th)
{
    fp2_t xx, zz;

    // xx = x^2, zz = z^2
    fp2_sqr(&xx, &th->x);
    fp2_sqr(&zz, &th->z);
    // xx = x^4, zz = y^4
    fp2_sqr(&xx, &xx);
    fp2_sqr(&zz, &zz);

    return !fp2_is_zero(&th->x) && !fp2_is_zero(&th->z) && !fp2_is_equal(&xx, &zz);
}

bool
splitting_to_elliptic_product(theta_couple_curve_t *E12,
                              theta_couple_point_t *P12,
                              theta_point_t *dual_null_point,
                              theta_point_t *pts,
                              const size_t numP,
                              const int index)
{
    splitting_change_coord_to_product_theta_struct(dual_null_point);

    // A valid elliptic theta null point has no zero coordinate
    uint32_t coord_is_zero = fp2_is_zero(&dual_null_point->x) | fp2_is_zero(&dual_null_point->y) |
                             fp2_is_zero(&dual_null_point->z) | fp2_is_zero(&dual_null_point->t);
    // Validity check on secret-derived theta coordinates that never fails for honest inputs; only the pass/fail bit is
    // declassified, the coordinates stay tainted
    CT_TESTING_MAKE_PUBLIC(&coord_is_zero, sizeof(coord_is_zero));
    if (coord_is_zero) {
        debug_print("a coordinate of the splitting coordinates is zero");
        return 0;
    }
    theta_couple_point_t th1th2;
    if (!splitting_tensor_product_to_couple_point(&th1th2, dual_null_point)) {
        debug_print("kernel did not generate an isogeny between elliptic products");
        return 0;
    }

    for (size_t j = 0; j < numP; ++j) {
        splitting_change_coord_to_product_theta_struct(&pts[j]);
        if (!splitting_tensor_product_to_couple_point(&P12[j], &pts[j])) {
            debug_print("pushed points are not a product of elliptic curve points ");
            return 0;
        }
    }

    ec_change_coord_matrix_t M;

    // For the verification, the curve is given by th1th2->P1
    // Checking that P1 is valid is done in ec_theta_to_montgomery
    // We still need to check that th1th2->P2 is valid as a sanity verification
    if (index == -1 && !splitting_check_valid_theta_null_point(&th1th2.P2)) {
        debug_print("kernel did not generate an isogeny between elliptic products");
        return 0;
    }

    if (index == 0 || index == 1 || index == -1) {
        if (!ec_theta_to_montgomery(&E12->E1, &M, &th1th2.P1)) {
            debug_print("failure during computation of the normal isomorphism of curve E1");
            return 0;
        }

        for (size_t j = 0; j < numP; ++j) {
            ec_apply_isomorphism(&P12[j].P1, &M);
        }
    }

    if (index == 0 || index == 2) {
        if (!ec_theta_to_montgomery(&E12->E2, &M, &th1th2.P2)) {
            debug_print("failure during computation of the normal isomorphism of curve E2");
            return 0;
        }

        for (size_t j = 0; j < numP; ++j) {
            ec_apply_isomorphism(&P12[j].P2, &M);
        }
    }

    return 1;
}
