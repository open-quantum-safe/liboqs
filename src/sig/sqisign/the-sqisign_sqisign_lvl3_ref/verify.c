#include <verification.h>
#include <mp.h>
#include <hd.h>
#include <encoded_sizes.h>
#include <assert.h>

// Check that the basis change matrix elements are canonical
// representatives modulo 2^(SQIsign_response_length + 2).
static int
check_canonical_basis_change_matrix(const signature_t *sig)
{
    // This works as long as all values in sig->mat_Bchall_can_to_B_chall are
    // positive integers.
    int ret = 1;
    scalar_t aux;

    memset(aux, 0, NWORDS_ORDER * sizeof(digit_t));
    aux[0] = 0x1;
    multiple_mp_shiftl(aux, SQIsign_response_length + HD_extra_torsion - (int)sig->backtracking, NWORDS_ORDER);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (mp_compare(aux, sig->mat_Bchall_can_to_B_chall[i][j], NWORDS_ORDER) <= 0) {
                ret = 0;
            }
        }
    }

    return ret;
}

// Compute the 2^n isogeny from the signature with kernel
// P + [chall_coeff]Q and store the codomain in E_chall
static int
compute_challenge_verify(ec_curve_t *E_chall, const signature_t *sig, const ec_curve_t *Epk, const uint8_t hint_pk)
{
    ec_basis_t bas_EA;
    ec_isog_even_t phi_chall;

    // Set domain and length of 2^n isogeny
    copy_curve(&phi_chall.curve, Epk);
    phi_chall.length = TORSION_EVEN_POWER - sig->backtracking;

    // Compute the basis from the supplied hint
    if (!ec_curve_to_basis_2f_from_hint(&bas_EA, &phi_chall.curve, TORSION_EVEN_POWER, hint_pk)) // canonical
        return 0;

    // recovering the exact challenge
    {
        if (!ec_ladder3pt(&phi_chall.kernel, sig->chall_coeff, &bas_EA.P, &bas_EA.Q, &bas_EA.PmQ, &phi_chall.curve)) {
            return 0;
        };
    }

    // Double the kernel until is has the correct order
    ec_dbl_iter(&phi_chall.kernel, sig->backtracking, &phi_chall.kernel, &phi_chall.curve);

    // Compute the codomain
    copy_curve(E_chall, &phi_chall.curve);
    if (ec_eval_even(E_chall, &phi_chall, NULL, 0))
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
matrix_scalar_application_even_basis(ec_basis_t *bas, const ec_curve_t *E, scalar_mtx_2x2_t *mat, int f)
{
    scalar_t scalar0, scalar1;
    memset(scalar0, 0, NWORDS_ORDER * sizeof(digit_t));
    memset(scalar1, 0, NWORDS_ORDER * sizeof(digit_t));

    ec_basis_t tmp_bas;
    copy_basis(&tmp_bas, bas);

    // For a matrix [[a, c], [b, d]] we compute:
    //
    // first basis element R = [a]P + [b]Q
    if (!ec_biscalar_mul(&bas->P, (*mat)[0][0], (*mat)[1][0], f, &tmp_bas, E))
        return 0;
    // second basis element S = [c]P + [d]Q
    if (!ec_biscalar_mul(&bas->Q, (*mat)[0][1], (*mat)[1][1], f, &tmp_bas, E))
        return 0;
    // Their difference R - S = [a - c]P + [b - d]Q
    mp_sub(scalar0, (*mat)[0][0], (*mat)[0][1], NWORDS_ORDER);
    mp_mod_2exp(scalar0, f, NWORDS_ORDER);
    mp_sub(scalar1, (*mat)[1][0], (*mat)[1][1], NWORDS_ORDER);
    mp_mod_2exp(scalar1, f, NWORDS_ORDER);
    return ec_biscalar_mul(&bas->PmQ, scalar0, scalar1, f, &tmp_bas, E);
}

// Compute the bases for the challenge and auxillary curve from
// the canonical bases. Challenge basis is reconstructed from the
// compressed scalars within the challenge.
static int
challenge_and_aux_basis_verify(ec_basis_t *B_chall_can,
                               ec_basis_t *B_aux_can,
                               ec_curve_t *E_chall,
                               ec_curve_t *E_aux,
                               signature_t *sig,
                               const int pow_dim2_deg_resp)
{

    // recovering the canonical basis as TORSION_EVEN_POWER for consistency with signing
    if (!ec_curve_to_basis_2f_from_hint(B_chall_can, E_chall, TORSION_EVEN_POWER, sig->hint_chall))
        return 0;

    // setting to the right order
    ec_dbl_iter_basis(B_chall_can,
                      TORSION_EVEN_POWER - pow_dim2_deg_resp - HD_extra_torsion - sig->two_resp_length,
                      B_chall_can,
                      E_chall);

    if (!ec_curve_to_basis_2f_from_hint(B_aux_can, E_aux, TORSION_EVEN_POWER, sig->hint_aux))
        return 0;

    // setting to the right order
    ec_dbl_iter_basis(B_aux_can, TORSION_EVEN_POWER - pow_dim2_deg_resp - HD_extra_torsion, B_aux_can, E_aux);

#ifndef NDEBUG
    if (!test_basis_order_twof(B_chall_can, E_chall, HD_extra_torsion + pow_dim2_deg_resp + sig->two_resp_length))
        debug_print("canonical basis has wrong order, expect something to fail");
#endif

    // applying the change matrix on the basis of E_chall
    return matrix_scalar_application_even_basis(B_chall_can,
                                                E_chall,
                                                &sig->mat_Bchall_can_to_B_chall,
                                                pow_dim2_deg_resp + HD_extra_torsion + sig->two_resp_length);
}

// When two_resp_length is non-zero, we must compute a small 2^n-isogeny
// updating E_chall as the codomain as well as push the basis on E_chall
// through this isogeny
static int
two_response_isogeny_verify(ec_curve_t *E_chall, ec_basis_t *B_chall_can, const signature_t *sig, int pow_dim2_deg_resp)
{
    ec_point_t ker, points[3];

    // choosing the right point for the small two_isogenies
    if (mp_is_even(sig->mat_Bchall_can_to_B_chall[0][0], NWORDS_ORDER) &&
        mp_is_even(sig->mat_Bchall_can_to_B_chall[1][0], NWORDS_ORDER)) {
        copy_point(&ker, &B_chall_can->Q);
    } else {
        copy_point(&ker, &B_chall_can->P);
    }

    copy_point(&points[0], &B_chall_can->P);
    copy_point(&points[1], &B_chall_can->Q);
    copy_point(&points[2], &B_chall_can->PmQ);

    ec_dbl_iter(&ker, pow_dim2_deg_resp + HD_extra_torsion, &ker, E_chall);

#ifndef NDEBUG
    if (!test_point_order_twof(&ker, E_chall, sig->two_resp_length))
        debug_print("kernel does not have order 2^(two_resp_length");
#endif

    if (ec_eval_small_chain(E_chall, &ker, sig->two_resp_length, points, 3, false)) {
        return 0;
    }

#ifndef NDEBUG
    if (!test_point_order_twof(&points[0], E_chall, HD_extra_torsion + pow_dim2_deg_resp))
        debug_print("points[0] does not have order 2^(HD_extra_torsion + pow_dim2_deg_resp");
    if (!test_point_order_twof(&points[1], E_chall, HD_extra_torsion + pow_dim2_deg_resp))
        debug_print("points[1] does not have order 2^(HD_extra_torsion + pow_dim2_deg_resp");
    if (!test_point_order_twof(&points[2], E_chall, HD_extra_torsion + pow_dim2_deg_resp))
        debug_print("points[2] does not have order 2^(HD_extra_torsion + pow_dim2_deg_resp");
#endif

    copy_point(&B_chall_can->P, &points[0]);
    copy_point(&B_chall_can->Q, &points[1]);
    copy_point(&B_chall_can->PmQ, &points[2]);
    return 1;
}

// The commitment curve can be recovered from the codomain of the 2D
// isogeny built from the bases computed during verification.
static int
compute_commitment_curve_verify(ec_curve_t *E_com,
                                const ec_basis_t *B_chall_can,
                                const ec_basis_t *B_aux_can,
                                const ec_curve_t *E_chall,
                                const ec_curve_t *E_aux,
                                int pow_dim2_deg_resp)

{
#ifndef NDEBUG
    // Check all the points are the correct order
    if (!test_basis_order_twof(B_chall_can, E_chall, HD_extra_torsion + pow_dim2_deg_resp))
        debug_print("B_chall_can does not have order 2^(HD_extra_torsion + pow_dim2_deg_resp");

    if (!test_basis_order_twof(B_aux_can, E_aux, HD_extra_torsion + pow_dim2_deg_resp))
        debug_print("B_aux_can does not have order 2^(HD_extra_torsion + pow_dim2_deg_resp");
#endif

    // now compute the dim2 isogeny from Echall x E_aux -> E_com x E_aux'
    // of kernel B_chall_can x B_aux_can

    // first we set-up the kernel
    theta_couple_curve_t EchallxEaux;
    copy_curve(&EchallxEaux.E1, E_chall);
    copy_curve(&EchallxEaux.E2, E_aux);

    theta_kernel_couple_points_t dim_two_ker;
    copy_bases_to_kernel(&dim_two_ker, B_chall_can, B_aux_can);

    // computing the isogeny
    theta_couple_curve_t codomain;
    int codomain_splits;
    ec_curve_init(&codomain.E1);
    ec_curve_init(&codomain.E2);
    // handling the special case where we don't need to perform any dim2 computation
    if (pow_dim2_deg_resp == 0) {
        codomain_splits = 1;
        copy_curve(&codomain.E1, &EchallxEaux.E1);
        copy_curve(&codomain.E2, &EchallxEaux.E2);
        // We still need to check that E_chall is supersingular
        // This assumes that HD_extra_torsion == 2
        if (!ec_is_basis_four_torsion(B_chall_can, E_chall)) {
            return 0;
        }
    } else {
        codomain_splits = theta_chain_compute_and_eval_verify(
            pow_dim2_deg_resp, &EchallxEaux, &dim_two_ker, true, &codomain, NULL, 0);
    }

    // computing the commitment curve
    // its always the first one because of our (2^n,2^n)-isogeny formulae
    copy_curve(E_com, &codomain.E1);

    return codomain_splits;
}

// SQIsign verification
int
protocols_verify(signature_t *sig, const public_key_t *pk, const unsigned char *m, size_t l)
{
    int verify;

    if (!check_canonical_basis_change_matrix(sig))
        return 0;

    // Computation of the length of the dim 2 2^n isogeny
    int pow_dim2_deg_resp = SQIsign_response_length - (int)sig->two_resp_length - (int)sig->backtracking;

    // basic sanity test: checking that the response is not too long
    if (pow_dim2_deg_resp < 0)
        return 0;
    // The dim 2 isogeny embeds a dim 1 isogeny of odd degree, so it can
    // never be of length 2.
    if (pow_dim2_deg_resp == 1)
        return 0;

    // check the public curve is valid
    if (!ec_curve_verify_A(&(pk->curve).A))
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

    if (!challenge_and_aux_basis_verify(&B_chall_can, &B_aux_can, &E_chall, &E_aux, sig, pow_dim2_deg_resp)) {
        return 0;
    }

    // When two_resp_length != 0 we need to compute a second, short 2^r-isogeny
    if (sig->two_resp_length > 0) {
        if (!two_response_isogeny_verify(&E_chall, &B_chall_can, sig, pow_dim2_deg_resp)) {
            return 0;
        }
    }

    // We can recover the commitment curve with a 2D isogeny
    // The supplied signature did not compute an isogeny between eliptic products
    // and so definitely is an invalid signature.
    ec_curve_t E_com;
    if (!compute_commitment_curve_verify(&E_com, &B_chall_can, &B_aux_can, &E_chall, &E_aux, pow_dim2_deg_resp))
        return 0;

    scalar_t chk_chall;

    // recomputing the challenge vector
    hash_to_challenge(&chk_chall, pk, &E_com, m, l);

    // performing the final check
    verify = mp_compare(sig->chall_coeff, chk_chall, NWORDS_ORDER) == 0;

    return verify;
}
