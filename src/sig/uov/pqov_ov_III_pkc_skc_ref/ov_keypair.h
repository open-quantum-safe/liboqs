// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file ov_keypair.h
/// @brief Formats of key pairs and functions for generating key pairs.
/// Formats of key pairs and functions for generating key pairs.
///

#ifndef _ov_KEYPAIR_H_
#define _ov_KEYPAIR_H_


#include "params.h"

#ifdef  __cplusplus
extern  "C" {
#endif

/// alignment 1 for sturct
#pragma pack(push,1)


/// @brief public key for classic ov
///
///  public key for classic ov
///
typedef
struct {
    unsigned char pk[_PK_BYTE];
} pk_t;


/// @brief secret key for classic ov
///
/// secret key for classic ov
///
typedef
struct {
    unsigned char sk_seed[LEN_SKSEED];   ///< seed for generating secret key

    unsigned char O[_V_BYTE * _O]; ///< T map

    unsigned char P1[_PK_P1_BYTE];  ///< part of C-map, P1
    unsigned char S[_PK_P2_BYTE];                 ///< part of C-map, S
} sk_t;



/// @brief compressed public key
///
///  compressed public key
///
typedef
struct {
    unsigned char pk_seed[LEN_PKSEED];                      ///< seed for generating l1_Q1,l1_Q2
    unsigned char P3[_PK_P3_BYTE];
} cpk_t;



/// @brief compressed secret key
///
/// compressed secret key
///
typedef
struct {
    unsigned char sk_seed[LEN_SKSEED];   ///< seed for generating a part of secret key.
} csk_t;





/// restores alignment
#pragma pack(pop)


/////////////////////////////////////






#ifdef  __cplusplus
}
#endif

#endif //  _ov_KEYPAIR_H_
