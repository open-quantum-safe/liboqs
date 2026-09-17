#include "quaternion_tests.h"
#include <stdlib.h>
#include <stdio.h>
#include <prng.h>

int
ibz_rand_interval_bits(ibz_t *res, int bitsize)
{
    ibz_t twof, neg;
    ibz_init(&twof);
    ibz_init(&neg);
    ibz_mul_2exp(&twof, &ibz_const_two, bitsize);
    ibz_neg(&neg, &twof);
    int ret = ibz_rand_interval(res, &neg, &twof);
    return (ret);
}

int
quat_test_input_random_ideal_generation(quat_ideal_t *ideals, int norm_bitsize, int iterations)
{
    int randret = 0;
    ibz_t norm;
    ibz_init(&norm);

    for (int iter = 0; iter < iterations; iter++) {
        // generate random odd norm
        randret = !ibz_generate_random_prime(&norm, 0, norm_bitsize);
        if ((randret != 0) || ibz_is_zero(&norm)) {
            printf("Randomness failure of ibz_generate_random_prime in quat_test_input_random_ideal_generation\n");
            return (randret);
        }
        ibz_abs(&norm, &norm);
        // compute ideal
        randret = !quat_random_ideal_O0_given_prime_norm(&(ideals[iter]), &norm, &PRNG_default_domain);
        if (randret != 0) {
            printf("Randomness failure in of quat_random_ideal_O0_given_prime_norm in  "
                   "quat_test_input_random_ideal_generation\n");
            return (randret);
        }
    }

    return (randret);
}

// norms is either of length iterations or NULL
int
quat_test_input_random_ideal_lattice_generation(quat_lattice_t *lattices,
                                                ibz_t *norms,
                                                int norm_bitsize,
                                                int iterations)
{
    quat_ideal_t *ideals;
    ideals = malloc(iterations * sizeof(quat_ideal_t));
    for (int i = 0; i < iterations; i++)
        quat_ideal_init(&(ideals[i]));
    int randret = quat_test_input_random_ideal_generation(ideals, norm_bitsize, iterations);

    for (int iter = 0; iter < iterations; iter++) {
        quat_to_lattice(&(lattices[iter]), &(ideals[iter]));
        if (norms != NULL) {
            ibz_copy(&(norms[iter]), &(ideals[iter].norm));
        }
    }
    free(ideals);
    return (randret);
}

int
quat_test_input_random_lattice_generation(quat_lattice_t *lattices, int bitsize, int iterations)
{
    ibz_t det;
    ibz_init(&det);
    int randret = 0;
    for (int iter = 0; iter < iterations; iter++) {
        // generate random invertible matrix
        ibz_set(&det, 0, 0);
        while (ibz_is_zero(&det)) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    randret = !ibz_rand_interval_bits(&((lattices[iter]).basis.m[i][j]), bitsize);
                    if (randret != 0)
                        goto randomness_failure;
                }
            }
            randret = !ibz_rand_interval_bits(&((lattices[iter]).denom), bitsize);
            if (randret != 0)
                goto randomness_failure;
            ibz_mat_4x4_inv_with_det_as_denom(NULL, &det, &((lattices[iter]).basis));
            ibz_mul(&det, &det, &(lattices[iter].denom));
        }
    }
randomness_failure:;
    if (randret != 0)
        printf("Randomness failure in quat_test_input_random_lattice_generation\n");
    return (randret);
}

int
quat_test_input_resplike_lattice_generation(quat_lattice_t *lattices, ibz_t *norms, int bitsize, int iterations)
{
    int ret = 1;
    quat_ideal_t a, b;
    ibz_t norm_a, norm_b, g;
    ibz_init(&norm_b);
    ibz_init(&norm_a);
    ibz_init(&g);
    quat_ideal_init(&b);
    quat_ideal_init(&a);
    for (int i = 0; i < iterations; i++) {
        ret = ret && ibz_generate_random_prime(&norm_a, 0, (bitsize / 2));
        ret = ret && ibz_generate_random_prime(&norm_b, 0, (bitsize / 2));
        if (!ret)
            return (!ret);
        ret = ret && quat_random_ideal_O0_given_prime_norm(&a, &norm_a, &PRNG_default_domain);
        ret = ret && quat_random_ideal_O0_given_prime_norm(&b, &norm_b, &PRNG_default_domain);
        if (!ret)
            return (!ret);
        ibz_gcd(&g, &norm_a, &norm_b);
        if (!ibz_is_one(&g)) {
            printf("Unlikely (in 2^-bitsize) event in resplike_lattice_generation for bitsize %d\n", bitsize);
            i = i - 1;
            continue;
        }
        quat_ideal_mul_O0(&(lattices[i]), &a, &b);
        if (norms != NULL) {
            ibz_mul(&norms[i], &a.norm, &b.norm);
        }
    }
    return (!ret);
}
