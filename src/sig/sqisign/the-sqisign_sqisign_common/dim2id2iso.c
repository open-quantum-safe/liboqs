#include <ec.h>
#include <endomorphism_action.h>
#include <hd.h>
#include <id2iso.h>
#include <inttypes.h>
#include <locale.h>
#include <quaternion.h>
#include <tools.h>
#include <torsion_constants.h>
#include <stdio.h>

int
dim2id2iso_ideal_to_isogeny_qlapoty(quat_alg_elem_t *beta1,
                                    ibz_t *d1,
                                    ec_curve_t *codomain,
                                    ec_basis_t *basis,
                                    const quat_ideal_t *ideal)

{
    quat_alg_elem_t theta, apply_to_basis;
    ibz_t d1_inv, two_e;
    ec_basis_t basis_temp;
    struct theta_kernel_couple_points ker;
    theta_couple_curve_t E0E0;
    theta_couple_curve_t theta_codomain;
    ibz_init(&two_e);
    quat_alg_elem_init(&theta);
    ibz_init(&d1_inv);
    quat_alg_elem_init(&apply_to_basis);

    int ret;
    int used_torsion = QUAT_qlapoty_used_power_of_two;

    // Compute norm equation of Qlapoty
    ret = quat_qlapoty(beta1, d1, &theta, ideal);
    assert(ibz_is_odd(d1));

    if (!ret) {
        fprintf(stderr, "Qlapoty failed. This should never happen. Please report this failure to the developers.\n");
        abort();
    }

    if (ret) {
        // Evalaute theta over basis of E0
        ibz_mul_2exp(&two_e, &ibz_const_one, TORSION_EVEN_POWER);
        assert(ibz_bitsize(&two_e) == TORSION_EVEN_POWER + 1);

        UNUSED int invertible = ibz_invmod(&d1_inv, d1, &two_e);

        quat_alg_elem_scalar_mul(&apply_to_basis, &d1_inv, &theta);

        ec_copy_basis(&basis_temp, &BASIS_EVEN);
        id2iso_endomorphism_application_even_basis(&basis_temp, &CURVE_E0, &apply_to_basis, TORSION_EVEN_POWER, false);
        assert(test_point_order_twof(&basis_temp.P, &CURVE_E0, TORSION_EVEN_POWER));
        assert(test_point_order_twof(&basis_temp.Q, &CURVE_E0, TORSION_EVEN_POWER));

        // Both are normalized to avoid extra scalar mults
        // B2 = E0_basis
        // basis_temp = norm(I1) * basis_temp

        theta_copy_bases_to_kernel(&ker, &basis_temp, &BASIS_EVEN);
        ec_copy_curve(&E0E0.E1, &CURVE_E0);
        ec_copy_curve(&E0E0.E2, &CURVE_E0);

        ec_copy_basis(&basis_temp, &BASIS_EVEN);
        assert(test_basis_order_twof(&BASIS_EVEN, &CURVE_E0, TORSION_EVEN_POWER));
        // compute to be pushed
        {
            assert(test_basis_order_twof(&basis_temp, &CURVE_E0, TORSION_EVEN_POWER));

            // Apply elem * d1^-1
            // scalar mul by d1 mod 2^Torsion
            quat_alg_elem_scalar_mul(&apply_to_basis, &d1_inv, beta1);

            id2iso_endomorphism_application_even_basis(
                &basis_temp, &CURVE_E0, &apply_to_basis, TORSION_EVEN_POWER, true);
            assert(test_basis_order_twof(&basis_temp, &CURVE_E0, TORSION_EVEN_POWER));
        }
        theta_couple_point_t push[3];
        ec_copy_point(&push[0].P1, &basis_temp.P);
        ec_copy_point(&push[1].P1, &basis_temp.Q);
        ec_copy_point(&push[2].P1, &basis_temp.PmQ);
        ec_point_init(&push[0].P2);
        ec_point_init(&push[1].P2);
        ec_point_init(&push[2].P2);

        ret = theta_chain_compute_and_eval_E2(
            used_torsion, &E0E0, &ker, &theta_codomain, push, sizeof(push) / sizeof(*push));
        if (!ret) {
            debug_print("id2iso HD chain failed\n");
            return ret;
        }

        // Because we fixed our theta structure, It is always E2
        ec_copy_point(&basis->P, &push[0].P2);
        ec_copy_point(&basis->Q, &push[1].P2);
        ec_copy_point(&basis->PmQ, &push[2].P2);
        ec_copy_curve(codomain, &theta_codomain.E2);

#ifndef NDEBUG
        if (ibz_is_odd(&ideal->norm)) {
            assert(test_point_order_twof(&basis->P, codomain, TORSION_EVEN_POWER));
            assert(test_point_order_twof(&basis->Q, codomain, TORSION_EVEN_POWER));
            assert(test_point_order_twof(&basis->PmQ, codomain, TORSION_EVEN_POWER));
        } else {
            debug_print("even order ideal\n");
        }
#endif
    }

    return ret;
}

int
dim2id2iso_arbitrary_isogeny_evaluation(ec_basis_t *basis, ec_curve_t *codomain, const quat_ideal_t *ideal)
{
    int ret;

    quat_alg_elem_t beta1;
    ibz_t d1;

    quat_alg_elem_init(&beta1);
    ibz_init(&d1);

    ret = dim2id2iso_ideal_to_isogeny_qlapoty(&beta1, &d1, codomain, basis, ideal);

    return ret;
}
