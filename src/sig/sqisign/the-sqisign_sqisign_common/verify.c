#include <verification.h>
#include <mp.h>
#include <hd.h>
#include <encoded_sizes.h>
#include <assert.h>

// Check that the basis change matrix elements are canonical representatives modulo 2^(RESPONSE_BITS + 2),
// and that the matrix is normalized (see NormalizeMatrix() in the specification).
static int
check_canonical_basis_change_matrix(const signature_t *sig)
{
    ibz_t mid, bound, tmp;

    ibz_mul_2exp(&mid, &ibz_const_one, RESPONSE_BITS + HD_EXTRA_TORSION - 1);
    ibz_mul_2exp(&bound, &mid, 1);

    int normal = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ibz_copy_digits(&tmp, sig->mat_Bchall_can_to_B_chall[i][j], NWORDS_ORDER);
            if (!ibz_is_positive(&tmp) || ibz_cmp(&tmp, &bound) >= 0)
                return 0;

            // normalized?
            if (normal)
                continue;
            int r = ibz_cmp(&tmp, &mid);
            if (r > 0)
                return 0;
            normal = r < 0 && !ibz_is_zero(&tmp);
        }
    }

    return 1;
}

// Compute the 2^n isogeny from the signature with kernel P + [chall_coeff]Q and store the codomain in E_chall
static int
compute_challenge_verify(ec_curve_t *E_chall, const signature_t *sig, const ec_curve_t *Epk, const uint8_t hint_pk)
{
    ec_basis_t bas_EA;
    ec_isog_even_t phi_chall;

    // Set domain and length of 2^n isogeny
    ec_copy_curve(&phi_chall.curve, Epk);
    _Static_assert(CHALLENGE_BITS + EC_EXTRA_TORSION <= TORSION_EVEN_POWER,
                   "challenge isogeny length exceeds the available 2-power torsion (TORSION_EVEN_POWER)");
    phi_chall.length = CHALLENGE_BITS + EC_EXTRA_TORSION;

    // Compute the basis from the supplied hint
    if (!ec_curve_to_basis_2f_from_hint(
            &bas_EA, &phi_chall.curve, CHALLENGE_BITS + EC_EXTRA_TORSION, hint_pk)) // canonical
        return 0;

    // recovering the exact challenge
    {
        if (!ec_ladder3pt(&phi_chall.kernel, sig->chall_coeff, &bas_EA.P, &bas_EA.Q, &bas_EA.PmQ, &phi_chall.curve)) {
            return 0;
        };
    }

    // Compute the codomain
    ec_copy_curve(E_chall, &phi_chall.curve);
    if (iso_isogeny_2chain(E_chall, &phi_chall))
        return 0;
    return 1;
}

// same as matrix_application_even_basis() in id2iso.c, with some modifications:
// - this version works with a matrix of scalars (not ibz_t).
// - reduction modulo 2^f of matrix elements is removed here, because it is
//   assumed that the elements are already cannonical representatives modulo
//   2^f; this is ensured by calling check_canonical_basis_change_matrix() at
//   the beginning of protocols_verify().
static int
matrix_scalar_application_even_basis(ec_basis_t *bas, ec_curve_t *E, scalar_mtx_2x2_t *mat, int f)
{
    ec_basis_t tmp_bas;
    ec_copy_basis(&tmp_bas, bas);

    // For a matrix [[a, c], [b, d]] we compute:
    //
    // first basis element R = [a]P + [b]Q
    if (!ec_biscalar_mul_verif(&bas->P, (*mat)[0][0], (*mat)[1][0], f, &tmp_bas, E))
        return 0;
    // second basis element S = [c]P + [d]Q
    if (!ec_biscalar_mul_verif(&bas->Q, (*mat)[0][1], (*mat)[1][1], f, &tmp_bas, E))
        return 0;

    return 1;
}

// Compute the bases for the challenge and auxillary curve from the canonical bases. Challenge basis is reconstructed
// from the compressed scalars within the challenge.
static int
challenge_and_aux_basis_verify(ec_basis_t *B_chall_can,
                               ec_basis_t *B_aux_can,
                               ec_curve_t *E_chall,
                               ec_curve_t *E_aux,
                               signature_t *sig)
{
    if (!ec_curve_to_basis_2f_from_hint(B_chall_can, E_chall, RESPONSE_BITS + HD_EXTRA_TORSION, sig->hint_chall))
        return 0;

    if (!ec_curve_to_basis_2f_from_hint(B_aux_can, E_aux, RESPONSE_BITS + HD_EXTRA_TORSION, sig->hint_aux))
        return 0;

#ifndef NDEBUG
    if (!test_basis_order_twof(B_chall_can, E_chall, HD_EXTRA_TORSION + RESPONSE_BITS))
        debug_print("canonical basis has wrong order, expect something to fail");
#endif

    // applying the change matrix on the basis of E_chall
    return matrix_scalar_application_even_basis(
        B_chall_can, E_chall, &sig->mat_Bchall_can_to_B_chall, RESPONSE_BITS + HD_EXTRA_TORSION);
}

// The commitment curve can be recovered from the codomain of the 2D isogeny built from the bases computed during
// verification.
static int
compute_commitment_curve_verify(ec_curve_t *E_com,
                                const ec_basis_t *B_chall_can,
                                const ec_basis_t *B_aux_can,
                                const ec_curve_t *E_chall,
                                const ec_curve_t *E_aux)

{
#ifndef NDEBUG
    // Check all the points are the correct order
    if (!test_point_order_twof(&B_chall_can->P, E_chall, HD_EXTRA_TORSION + RESPONSE_BITS))
        debug_print("B_chall_can.P does not have order 2^(HD_EXTRA_TORSION + RESPONSE_BITS");
    if (!test_point_order_twof(&B_chall_can->Q, E_chall, HD_EXTRA_TORSION + RESPONSE_BITS))
        debug_print("B_chall_can.Q does not have order 2^(HD_EXTRA_TORSION + RESPONSE_BITS");

    if (!test_point_order_twof(&B_aux_can->P, E_aux, HD_EXTRA_TORSION + RESPONSE_BITS))
        debug_print("B_aux_can.P does not have order 2^(HD_EXTRA_TORSION + RESPONSE_BITS");
    if (!test_point_order_twof(&B_aux_can->Q, E_aux, HD_EXTRA_TORSION + RESPONSE_BITS))
        debug_print("B_aux_can.Q does not have order 2^(HD_EXTRA_TORSION + RESPONSE_BITS");

#endif

    // now compute the dim2 isogeny from Echall x E_aux -> E_com x E_aux' of kernel B_chall_can x B_aux_can

    // first we set-up the kernel
    theta_couple_curve_t EchallxEaux;
    ec_copy_curve(&EchallxEaux.E1, E_chall);
    ec_copy_curve(&EchallxEaux.E2, E_aux);

    theta_kernel_couple_points_t dim_two_ker;
    theta_copy_bases_to_kernel(&dim_two_ker, B_chall_can, B_aux_can);

    // computing the isogeny
    theta_couple_curve_t codomain;
    int codomain_splits;

    codomain_splits =
        theta_chain_compute_and_eval_verify(RESPONSE_BITS, &EchallxEaux, &dim_two_ker, &codomain, NULL, 0);

    // computing the commitment curve
    // its always the first one because of our (2^n,2^n)-isogeny formulae
    // on failure the chain leaves codomain unset, and the caller discards E_com
    if (codomain_splits)
        ec_copy_curve(E_com, &codomain.E1);

    return codomain_splits;
}

// SQIsign verification
int
protocols_verify(signature_t *sig, const public_key_t *pk, const unsigned char *m, size_t l)
{
    if (!check_canonical_basis_change_matrix(sig))
        return 0;

    // check the public curve is valid
    if (!ec_curve_verify_A(&pk->curve.A))
        return 0;

    // Set auxiliary curve from the A-coefficient within the signature
    ec_curve_t E_aux;
    if (!ec_curve_init_from_A(&E_aux, &sig->E_aux_A))
        return 0; // invalid curve

    // checking that we are given A-coefficients and no precomputation
    assert(fp2_is_one(&pk->curve.C) == 0xFFFFFFFF && !pk->curve.is_A24_computed_and_normalized);

    // computation of the challenge
    ec_curve_t E_chall;
    if (!compute_challenge_verify(&E_chall, sig, &pk->curve, pk->hint_pk)) {
        return 0;
    }

    // Computation of the canonical bases for the challenge and aux curve
    ec_basis_t B_chall_can, B_aux_can;

    if (!challenge_and_aux_basis_verify(&B_chall_can, &B_aux_can, &E_chall, &E_aux, sig)) {
        return 0;
    }

    // We can recover the commitment curve with a 2D isogeny
    // The supplied signature did not compute an isogeny between eliptic products and so definitely is an invalid
    // signature.
    ec_curve_t E_com;
    if (!compute_commitment_curve_verify(&E_com, &B_chall_can, &B_aux_can, &E_chall, &E_aux))
        return 0;

    scalar_t chk_chall;

    // recomputing the challenge vector
    hash_to_challenge(&chk_chall, pk, &E_com, m, l);

    // performing the final check
    for (unsigned i = 0; i < sizeof(sig->chall_coeff) / sizeof(*sig->chall_coeff); i++)
        if (sig->chall_coeff[i] != chk_chall[i])
            return 0;
    return 1;
}
