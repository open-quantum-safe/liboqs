#ifndef _RAINBOW_KEYPAIR_H_
#define _RAINBOW_KEYPAIR_H_
/// @file rainbow_keypair.h
/// @brief Formats of key pairs and functions for generating key pairs.
/// Formats of key pairs and functions for generating key pairs.
///

#include "rainbow_config.h"

#define N_TRIANGLE_TERMS(n_var) ((n_var) * ((n_var) + 1) / 2)

/// @brief public key for classic rainbow
///
///  public key for classic rainbow
///
typedef struct rainbow_publickey {
    unsigned char pk[(_PUB_M_BYTE)*N_TRIANGLE_TERMS(_PUB_N)];
} pk_t;

/// @brief secret key for classic rainbow
///
/// secret key for classic rainbow
///
typedef struct rainbow_secretkey {
    ///
    /// seed for generating secret key.
    /// Generating S, T, and F for classic rainbow.
    /// Generating S and T only for cyclic rainbow.
    unsigned char sk_seed[LEN_SKSEED];

    unsigned char s1[_O1_BYTE * _O2]; ///< part of S map
    unsigned char t1[_V1_BYTE * _O1]; ///< part of T map
    unsigned char t4[_V1_BYTE * _O2]; ///< part of T map
    unsigned char t3[_O1_BYTE * _O2]; ///< part of T map

    unsigned char l1_F1[_O1_BYTE * N_TRIANGLE_TERMS(_V1)]; ///< part of C-map, F1, Layer1
    unsigned char l1_F2[_O1_BYTE * _V1 * _O1];             ///< part of C-map, F2, Layer1

    unsigned char l2_F1[_O2_BYTE * N_TRIANGLE_TERMS(_V1)]; ///< part of C-map, F1, Layer2
    unsigned char l2_F2[_O2_BYTE * _V1 * _O1];             ///< part of C-map, F2, Layer2

    unsigned char l2_F3[_O2_BYTE * _V1 * _O2];             ///< part of C-map, F3, Layer2
    unsigned char l2_F5[_O2_BYTE * N_TRIANGLE_TERMS(_O1)]; ///< part of C-map, F5, Layer2
    unsigned char l2_F6[_O2_BYTE * _O1 * _O2];             ///< part of C-map, F6, Layer2
} sk_t;

/// @brief public key for cyclic rainbow
///
///  public key for cyclic rainbow
///
typedef struct rainbow_publickey_cyclic {
    unsigned char pk_seed[LEN_PKSEED]; ///< seed for generating l1_Q1,l1_Q2,l2_Q1,l2_Q2,l2_Q3,l2_Q5,l2_Q6

    unsigned char l1_Q3[_O1_BYTE * _V1 * _O2];             ///< Q3, layer1
    unsigned char l1_Q5[_O1_BYTE * N_TRIANGLE_TERMS(_O1)]; ///< Q5, layer1
    unsigned char l1_Q6[_O1_BYTE * _O1 * _O2];             ///< Q6, layer1
    unsigned char l1_Q9[_O1_BYTE * N_TRIANGLE_TERMS(_O2)]; ///< Q9, layer1

    unsigned char l2_Q9[_O2_BYTE * N_TRIANGLE_TERMS(_O2)]; ///< Q9, layer2
} cpk_t;

/// @brief compressed secret key for cyclic rainbow
///
/// compressed secret key for cyclic rainbow
///
typedef struct rainbow_secretkey_cyclic {
    unsigned char pk_seed[LEN_PKSEED]; ///< seed for generating a part of public key.
    unsigned char sk_seed[LEN_SKSEED]; ///< seed for generating a part of secret key.
} csk_t;


///
/// @brief Generate key pairs for cyclic rainbow.
///
/// @param[out] pk        - the public key.
/// @param[out] sk        - the secret key.
/// @param[in]  pk_seed   - seed for generating parts of public key.
/// @param[in]  sk_seed   - seed for generating secret key.
///
void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_generate_keypair_cyclic(cpk_t *pk, sk_t *sk, const unsigned char *pk_seed, const unsigned char *sk_seed);



////////////////////////////////////

///
/// @brief converting formats of public keys : from cyclic version to classic key
///
/// @param[out] pk       - the classic public key.
/// @param[in]  cpk      - the cyclic  public key.
///
void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_cpk_to_pk(pk_t *pk, const cpk_t *cpk);

#endif //  _RAINBOW_KEYPAIR_H_
