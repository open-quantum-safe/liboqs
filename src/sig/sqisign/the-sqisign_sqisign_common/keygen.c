#include <signature.h>
#include <quaternion_constants.h>
#include <quaternion_data.h>
#include <id2iso.h>
#include <torsion_constants.h>
#include <endomorphism_action.h>
#include <encoded_sizes.h>
#include <prng.h>
#include <ct_testing.h>

void
secret_key_init(secret_key_t *sk)
{
    quat_ideal_init(&sk->secret_ideal);
    ibz_mat_2x2_init(&sk->mat_BAcan_to_BA0_two);
    ec_curve_init(&sk->curve);
}

int
protocols_keygen(public_key_t *pk, secret_key_t *sk)
{
    int found = 0;
    ec_basis_t B_0_two;

    if (prng_seed() != 0)
        return 0;

    prng_domain_ctx_t prng_keygen_domain;
    prng_domain_seed(&prng_keygen_domain, DS_KEYGEN_DOMAIN);
    // iterating until a solution has been found
    while (!found) {
        found = quat_random_ideal_O0_given_prime_norm(&sk->secret_ideal, &SEC_DEGREE, &prng_keygen_domain);

        // replacing the secret key ideal by a shorter equivalent one for efficiency
        found = found && quat_ideal_small_equivalent_coprime(
                             NULL, &sk->secret_ideal, &sk->secret_ideal, &ibz_const_zero, &prng_keygen_domain);
        found = found && dim2id2iso_arbitrary_isogeny_evaluation(&B_0_two, &sk->curve, &sk->secret_ideal);
    }
    prng_domain_clear(&prng_keygen_domain);

    // Assert the isogeny was found and images have the correct order
    assert(test_basis_order_twof(&B_0_two, &sk->curve, TORSION_EVEN_POWER));

    // Compute a deterministic basis with a hint to speed up verification
    pk->hint_pk = ec_curve_to_basis_2f_to_hint(&sk->canonical_basis, &sk->curve, CHALLENGE_BITS + EC_EXTRA_TORSION, 0);

    // Assert the deterministic basis we computed has the correct order
    assert(test_basis_order_twof(&sk->canonical_basis, &sk->curve, CHALLENGE_BITS + EC_EXTRA_TORSION));

    // Compute the 2x2 matrix basis change from the canonical basis to the evaluation of our secret isogeny
    id2iso_change_of_basis_matrix_tate(
        &sk->mat_BAcan_to_BA0_two, &sk->canonical_basis, &B_0_two, &sk->curve, CHALLENGE_BITS + EC_EXTRA_TORSION);

    // adjusting the size
    ibz_mod2exp(&sk->mat_BAcan_to_BA0_two.m[0][0], &sk->mat_BAcan_to_BA0_two.m[0][0], CHALLENGE_BITS);
    ibz_mod2exp(&sk->mat_BAcan_to_BA0_two.m[0][1], &sk->mat_BAcan_to_BA0_two.m[0][1], CHALLENGE_BITS);
    ibz_mod2exp(&sk->mat_BAcan_to_BA0_two.m[1][0], &sk->mat_BAcan_to_BA0_two.m[1][0], CHALLENGE_BITS);
    ibz_mod2exp(&sk->mat_BAcan_to_BA0_two.m[1][1], &sk->mat_BAcan_to_BA0_two.m[1][1], CHALLENGE_BITS);

    // normalize the matrix (modulo sign)
    ibz_mat_2x2_normalize(&sk->mat_BAcan_to_BA0_two, CHALLENGE_BITS);

    prng_clear();

    // A keygen failure should be easily observable by an attacker, so no harm in treating it as public
    CT_TESTING_MAKE_PUBLIC(&found, sizeof(found));

    if (!found)
        return 0;

    // Set the public key from the codomain curve
    ec_copy_curve(&pk->curve, &sk->curve);
    pk->curve.is_A24_computed_and_normalized = false; // We don't send any precomputation

    assert(fp2_is_one(&pk->curve.C) == 0xFFFFFFFF);

#ifndef NDEBUG
    {
        // make sure known bounds are correct
        ibz_t abs, bnd;
        ibz_init(&bnd);
        ibz_init(&abs);
        ibz_mul_2exp(&bnd, &ibz_const_one, 8 * FP_ENCODED_BYTES);
        ibz_abs(&abs, &sk->secret_ideal.norm);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_abs(&abs, &sk->secret_ideal.x);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_abs(&abs, &sk->secret_ideal.y);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_mul_2exp(&bnd, &ibz_const_one, 8 * CHALLENGE_BYTES);
        ibz_abs(&abs, &sk->mat_BAcan_to_BA0_two.m[0][0]);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_abs(&abs, &sk->mat_BAcan_to_BA0_two.m[0][1]);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_abs(&abs, &sk->mat_BAcan_to_BA0_two.m[1][0]);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_abs(&abs, &sk->mat_BAcan_to_BA0_two.m[1][1]);
        assert(ibz_cmp(&abs, &bnd) < 0);
    }
#endif

    // Apply known bounds
    ibz_set_bound(&sk->secret_ideal.norm, 8 * FP_ENCODED_BYTES + 1);
    ibz_set_bound(&sk->secret_ideal.x, 8 * FP_ENCODED_BYTES + 1);
    ibz_set_bound(&sk->secret_ideal.y, 8 * FP_ENCODED_BYTES + 1);
    ibz_set_bound(&sk->mat_BAcan_to_BA0_two.m[0][0], 8 * CHALLENGE_BYTES + 1);
    ibz_set_bound(&sk->mat_BAcan_to_BA0_two.m[0][1], 8 * CHALLENGE_BYTES + 1);
    ibz_set_bound(&sk->mat_BAcan_to_BA0_two.m[1][0], 8 * CHALLENGE_BYTES + 1);
    ibz_set_bound(&sk->mat_BAcan_to_BA0_two.m[1][1], 8 * CHALLENGE_BYTES + 1);

    return found;
}
