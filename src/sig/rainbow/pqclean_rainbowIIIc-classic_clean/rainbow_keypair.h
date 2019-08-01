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


///
/// @brief Generate key pairs for classic rainbow.
///
/// @param[out] pk        - the public key.
/// @param[out] sk        - the secret key.
/// @param[in]  sk_seed   - seed for generating the secret key.
///
void PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_generate_keypair(pk_t *pk, sk_t *sk, const unsigned char *sk_seed);





#endif //  _RAINBOW_KEYPAIR_H_
