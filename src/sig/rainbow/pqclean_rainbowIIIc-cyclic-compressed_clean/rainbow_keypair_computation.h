#ifndef _RAINBOW_KEYPAIR_COMP_H_
#define _RAINBOW_KEYPAIR_COMP_H_
/// @file rainbow_keypair_computation.h
/// @brief Functions for calculating pk/sk while generating keys.
///
/// Defining an internal structure of public key.
/// Functions for calculating pk/sk for key generation.
///

#include "rainbow_keypair.h"

/// @brief The (internal use) public key for rainbow
///
/// The (internal use) public key for rainbow. The public
/// polynomials are divided into l1_Q1, l1_Q2, ... l1_Q9,
/// l2_Q1, .... , l2_Q9.
///
typedef struct rainbow_extend_publickey {
    unsigned char l1_Q1[_O1_BYTE * N_TRIANGLE_TERMS(_V1)];
    unsigned char l1_Q2[_O1_BYTE * _V1 * _O1];
    unsigned char l1_Q3[_O1_BYTE * _V1 * _O2];
    unsigned char l1_Q5[_O1_BYTE * N_TRIANGLE_TERMS(_O1)];
    unsigned char l1_Q6[_O1_BYTE * _O1 * _O2];
    unsigned char l1_Q9[_O1_BYTE * N_TRIANGLE_TERMS(_O2)];

    unsigned char l2_Q1[_O2_BYTE * N_TRIANGLE_TERMS(_V1)];
    unsigned char l2_Q2[_O2_BYTE * _V1 * _O1];
    unsigned char l2_Q3[_O2_BYTE * _V1 * _O2];
    unsigned char l2_Q5[_O2_BYTE * N_TRIANGLE_TERMS(_O1)];
    unsigned char l2_Q6[_O2_BYTE * _O1 * _O2];
    unsigned char l2_Q9[_O2_BYTE * N_TRIANGLE_TERMS(_O2)];
} ext_cpk_t;

///
/// @brief converting formats of public keys : from ext_cpk_t version to pk_t
///
/// @param[out] pk       - the classic public key.
/// @param[in]  cpk      - the internel public key.
///
void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_extcpk_to_pk(pk_t *pk, const ext_cpk_t *cpk);
/////////////////////////////////////////////////

///
/// @brief Computing public key from secret key
///
/// @param[out] Qs       - the public key
/// @param[in]  Fs       - parts of the secret key: l1_F1, l1_F2, l2_F1, l2_F2, l2_F3, l2_F5, l2_F6
/// @param[in]  Ts       - parts of the secret key: T1, T4, T3
///
void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_calculate_Q_from_F(ext_cpk_t *Qs, const sk_t *Fs, const sk_t *Ts);


///
/// @brief Computing parts of the sk from parts of pk and sk
///
/// @param[out] Fs       - parts of the sk: l1_F1, l1_F2, l2_F1, l2_F2, l2_F3, l2_F5, l2_F6
/// @param[in]  Qs       - parts of the pk: l1_Q1, l1_Q2, l2_Q1, l2_Q2, l2_Q3, l2_Q5, l2_Q6
/// @param[in]  Ts       - parts of the sk: T1, T4, T3
///
void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_calculate_F_from_Q(sk_t *Fs, const sk_t *Qs, sk_t *Ts);

///
/// @brief Computing parts of the pk from the secret key
///
/// @param[out] Qs       - parts of the pk: l1_Q3, l1_Q5, l2_Q6, l1_Q9, l2_Q9
/// @param[in]  Fs       - parts of the sk: l1_F1, l1_F2, l2_F1, l2_F2, l2_F3, l2_F5, l2_F6
/// @param[in]  Ts       - parts of the sk: T1, T4, T3
///
void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_calculate_Q_from_F_cyclic(cpk_t *Qs, const sk_t *Fs, const sk_t *Ts);

#endif // _RAINBOW_KEYPAIR_COMP_H_
