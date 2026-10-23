#include <hd.h>

void
theta_DBL_couple_point(theta_couple_point_t *out, const theta_couple_point_t *in, const theta_couple_curve_t *E1E2)
{
    ec_dbl(&out->P1, &in->P1, &E1E2->E1);
    ec_dbl(&out->P2, &in->P2, &E1E2->E2);
}

void
theta_DBL_couple_point_iter(theta_couple_point_t *out,
                            uint16_t n,
                            const theta_couple_point_t *in,
                            const theta_couple_curve_t *E1E2)
{
    if (n == 0) {
        memmove(out, in, sizeof(theta_couple_point_t));
    } else {
        theta_DBL_couple_point(out, in, E1E2);
        for (uint16_t i = 0; i < n - 1; i++) {
            theta_DBL_couple_point(out, out, E1E2);
        }
    }
}

void
theta_copy_bases_to_kernel(theta_kernel_couple_points_t *ker, const ec_basis_t *B1, const ec_basis_t *B2)
{
    // Copy the basis on E1 to (P, _) on T1, T2
    ec_copy_point(&ker->T1.P1, &B1->P);
    ec_copy_point(&ker->T2.P1, &B1->Q);

    // Copy the basis on E2 to (_, P) on T1, T2
    ec_copy_point(&ker->T1.P2, &B2->P);
    ec_copy_point(&ker->T2.P2, &B2->Q);
}
