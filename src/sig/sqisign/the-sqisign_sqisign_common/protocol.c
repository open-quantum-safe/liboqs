#include <internal.h>
#include <math.h>
#include <stdio.h>
#include <prng.h>
#include <quaternion_constants.h>
#include <quaternion_data.h>
#include <encoded_sizes.h>

void
quat_response_element(quat_ideal_t *sk_chall_ideal,
                      quat_alg_elem_t *resp_quat,
                      ibz_t *norm,
                      quat_alg_elem_t *sk_chall_quat,
                      const quat_ideal_t *sk_ideal,
                      const quat_ideal_t *ideal_chall_two,
                      const quat_alg_elem_t *chall_split,
                      const quat_ideal_t *ideal_commit,
                      prng_domain_ctx_t *prng_domain,
                      int e)
{
    // bound on 4th minimum
    ibz_t lattice_content, bound;
    quat_lattice_t prod;
    ibz_init(&lattice_content);
    ibz_init(&bound);
    quat_lattice_init(&prod);

    // intersection of the sk and chall ideals
    quat_ideal_intersect_O0(sk_chall_ideal, ideal_chall_two, sk_ideal, chall_split);

    // computing an equivalent (smaller) ideal of coprime norm
    int UNUSED resp = quat_ideal_small_equivalent_coprime(
        sk_chall_quat, sk_chall_ideal, sk_chall_ideal, &ibz_const_zero, &PRNG_default_domain);
    assert(resp);
    // keeping track of the isomorphism from the old sk_chall_ideal will be important
    if (sk_chall_quat != NULL)
        quat_alg_conj(sk_chall_quat, sk_chall_quat);

    // computing the multiplication
    quat_ideal_mul_O0(&prod, sk_chall_ideal, ideal_commit);

    ibz_div_2exp(&lattice_content, &prod.basis.m[0][0], 1);
    // sample lattice
    {
        ibz_mul_2exp(&bound, &ibz_const_one, e); // e resp length here
        ibz_sub(&bound, &bound, &ibz_const_one);
        ibz_set_bound(&bound, e + 1);

        int ok UNUSED = quat_lattice_sample_from_ball(resp_quat, norm, &prod, &bound, prng_domain);
        assert(ok);
        ibz_set_bound(norm, e + 1);
    }
}
