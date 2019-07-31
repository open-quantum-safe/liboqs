/// @file rainbow_keypair.c
/// @brief implementations of functions in rainbow_keypair.h
///

#include "rainbow_keypair.h"
#include "blas.h"
#include "blas_comm.h"
#include "rainbow_blas.h"
#include "rainbow_keypair_computation.h"
#include "utils_prng.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void generate_S_T(unsigned char *s_and_t, prng_t *prng0) {
    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, s_and_t, _O1_BYTE * _O2); // S1
    s_and_t += _O1_BYTE * _O2;
    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, s_and_t, _V1_BYTE * _O1); // T1
    s_and_t += _V1_BYTE * _O1;
    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, s_and_t, _V1_BYTE * _O2); // T2
    s_and_t += _V1_BYTE * _O2;
    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, s_and_t, _O1_BYTE * _O2); // T3
}

static unsigned int generate_l1_F12(unsigned char *sk, prng_t *prng0) {
    unsigned int n_byte_generated = 0;
    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O1_BYTE * N_TRIANGLE_TERMS(_V1)); // l1_F1
    sk += _O1_BYTE * N_TRIANGLE_TERMS(_V1);
    n_byte_generated += _O1_BYTE * N_TRIANGLE_TERMS(_V1);

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O1_BYTE * _V1 * _O1); // l1_F2
    n_byte_generated += _O1_BYTE * _V1 * _O1;
    return n_byte_generated;
}

static unsigned int generate_l2_F12356(unsigned char *sk, prng_t *prng0) {
    unsigned int n_byte_generated = 0;

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O2_BYTE * N_TRIANGLE_TERMS(_V1)); // l2_F1
    sk += _O2_BYTE * N_TRIANGLE_TERMS(_V1);
    n_byte_generated += _O2_BYTE * N_TRIANGLE_TERMS(_V1);

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O2_BYTE * _V1 * _O1); // l2_F2
    sk += _O2_BYTE * _V1 * _O1;
    n_byte_generated += _O2_BYTE * _V1 * _O1;

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O2_BYTE * _V1 * _O2); // l2_F3
    sk += _O2_BYTE * _V1 * _O1;
    n_byte_generated += _O2_BYTE * _V1 * _O1;

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O2_BYTE * N_TRIANGLE_TERMS(_O1)); // l2_F5
    sk += _O2_BYTE * N_TRIANGLE_TERMS(_O1);
    n_byte_generated += _O2_BYTE * N_TRIANGLE_TERMS(_O1);

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng0, sk, _O2_BYTE * _O1 * _O2); // l2_F6
    n_byte_generated += _O2_BYTE * _O1 * _O2;

    return n_byte_generated;
}

static void generate_B1_B2(unsigned char *sk, prng_t *prng0) {
    sk += generate_l1_F12(sk, prng0);
    generate_l2_F12356(sk, prng0);
}

static void calculate_t4(unsigned char *t2_to_t4, const unsigned char *t1, const unsigned char *t3) {
    //  t4 = T_sk.t1 * T_sk.t3 - T_sk.t2
    unsigned char temp[_V1_BYTE + 32];
    unsigned char *t4 = t2_to_t4;
    for (unsigned int i = 0; i < _O2; i++) { /// t3 width
        gfmat_prod(temp, t1, _V1_BYTE, _O1, t3);
        gf256v_add(t4, temp, _V1_BYTE);
        t4 += _V1_BYTE;
        t3 += _O1_BYTE;
    }
}

static void obsfucate_l1_polys(unsigned char *l1_polys, const unsigned char *l2_polys, unsigned int n_terms, const unsigned char *s1) {
    unsigned char temp[_O1_BYTE + 32];
    while (n_terms--) {
        gfmat_prod(temp, s1, _O1_BYTE, _O2, l2_polys);
        gf256v_add(l1_polys, temp, _O1_BYTE);
        l1_polys += _O1_BYTE;
        l2_polys += _O2_BYTE;
    }
}

///////////////////  Classic //////////////////////////////////

static void _generate_secretkey(sk_t *sk, const unsigned char *sk_seed) {
    memcpy(sk->sk_seed, sk_seed, LEN_SKSEED);

    // set up prng
    prng_t prng0;
    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_set(&prng0, sk_seed, LEN_SKSEED);

    // generating secret key with prng.
    generate_S_T(sk->s1, &prng0);
    generate_B1_B2(sk->l1_F1, &prng0);

    // clean prng
    memset(&prng0, 0, sizeof(prng_t));
}

void PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_generate_keypair(pk_t *rpk, sk_t *sk, const unsigned char *sk_seed) {
    _generate_secretkey(sk, sk_seed);

    // set up a temporary structure ext_cpk_t for calculating public key.
    ext_cpk_t pk;

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_calculate_Q_from_F(&pk, sk, sk); // compute the public key in ext_cpk_t format.
    calculate_t4(sk->t4, sk->t1, sk->t3);

    obsfucate_l1_polys(pk.l1_Q1, pk.l2_Q1, N_TRIANGLE_TERMS(_V1), sk->s1);
    obsfucate_l1_polys(pk.l1_Q2, pk.l2_Q2, _V1 * _O1, sk->s1);
    obsfucate_l1_polys(pk.l1_Q3, pk.l2_Q3, _V1 * _O2, sk->s1);
    obsfucate_l1_polys(pk.l1_Q5, pk.l2_Q5, N_TRIANGLE_TERMS(_O1), sk->s1);
    obsfucate_l1_polys(pk.l1_Q6, pk.l2_Q6, _O1 * _O2, sk->s1);
    obsfucate_l1_polys(pk.l1_Q9, pk.l2_Q9, N_TRIANGLE_TERMS(_O2), sk->s1);
    // so far, the pk contains the full pk but in ext_cpk_t format.

    PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_extcpk_to_pk(rpk, &pk); // convert the public key from ext_cpk_t to pk_t.
}



