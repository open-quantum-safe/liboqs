// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file ov_keypair_computation.c
/// @brief Implementations for functions in ov_keypair_computation.h
///

#include "ov_keypair.h"
#include "ov_keypair_computation.h"

#include "blas_comm.h"
#include "blas.h"
#include "ov_blas.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils_malloc.h"


////////////////////////////////////////////////////////////////////////////


void calculate_F2( unsigned char *S, const unsigned char *P1, const unsigned char *P2, const unsigned char *sk_O ) {
    if ( S != P2 ) {
        memcpy( S, P2, _PK_P2_BYTE );
    }

    // F_sk.l1_F1s[i] = Q_pk.l1_F1s[i]
    // F_sk.l1_F2s[i] = ( Q_pk.l1_F1s[i] + Q_pk.l1_F1s[i].transpose() ) * T_sk.t1 + Q_pk.l1_F2s[i]

    #if defined(_MUL_WITH_MULTAB_)
    PQOV_ALIGN unsigned char multabs[(_V) * (_O) * 32]; // size of t1
    gfv_generate_multabs( multabs, sk_O, (_V) * (_O));
    batch_2trimat_madd_multab( S, P1, multabs, _V, _V_BYTE, _O, _O_BYTE );
    #else
    batch_2trimat_madd( S, P1, sk_O, _V, _V_BYTE, _O, _O_BYTE );
    #endif
}


#if defined(_BLAS_M4F_) &&  (defined(_OV_PKC) || defined(_OV_PKC_SKC))
void ov_pkc_calculate_F_from_Q( sk_t *Fs) {
    ov_pkc_calculate_F_from_Q_m4f( Fs );
}
#else
void ov_pkc_calculate_F_from_Q( sk_t *Fs) {
    calculate_F2( Fs->S, Fs->P1, Fs->S, Fs->O );
}
#endif





/////////////////////////////////////////////////////////




void calculate_F2_P3( unsigned char *S, unsigned char *P3, const unsigned char *P1, const unsigned char *P2, const unsigned char *sk_O ) {
    //Q_pk.l1_Q2s[i] = (P1* T1 + P2) + F1tr * t1
    //Q_pk.l1_Q5s[i] = UT( T1tr* (F1 * T1 + F2) )

    if (S != P2) {
        memcpy( S, P2, _PK_P2_BYTE );
    }
    #if defined(_MUL_WITH_MULTAB_)
    PQOV_ALIGN unsigned char multabs[(_V) * (_O) * 32]; // size of t1
    gfv_generate_multabs( multabs, sk_O, (_V) * (_O));

    batch_trimat_madd_multab( S, P1, multabs, _V, _V_BYTE, _O, _O_BYTE );        // F1*T1 + F2
    batch_upper_matTr_x_mat_multab( P3, multabs, _V, _V_BYTE, _O, S, _O, _O_BYTE );     // Q5 = UT . t1_tr*(F1*T1 + F2)
    batch_trimatTr_madd_multab( S, P1, multabs, _V, _V_BYTE, _O, _O_BYTE );       // Q2
    #else
    batch_trimat_madd( S, P1, sk_O, _V, _V_BYTE, _O, _O_BYTE );        // F1*T1 + F2
    batch_upper_matTr_x_mat( P3, sk_O, _V, _V_BYTE, _O, S, _O, _O_BYTE );    // Q5 = UT . t1_tr*(F1*T1 + F2)
    batch_trimatTr_madd( S, P1, sk_O, _V, _V_BYTE, _O, _O_BYTE );       // Q2
    #endif
}


//////////////////////////////////////////////////////////////////////////////////////////////////





void calculate_P3( unsigned char *P3, const unsigned char *P1, const unsigned char *P2, const unsigned char *sk_O ) {
//  Q_pk.l1_F5s[i] = UT( T1tr* (F1 * T1 + F2) )

#define _SIZE_BUFFER_F2 (_O_BYTE * _V * _O)
    PQOV_ALIGN unsigned char buffer_F2[_SIZE_BUFFER_F2];

    #if defined(_MUL_WITH_MULTAB_)
    PQOV_ALIGN unsigned char multabs[(_V) * (_O) * 32]; // size of t1
    gfv_generate_multabs( multabs, sk_O, (_V) * (_O));

    memcpy( buffer_F2, P2, _O_BYTE * _V * _O );
    batch_trimat_madd_multab( buffer_F2, P1, multabs, _V, _V_BYTE, _O, _O_BYTE );          // F1*T1 + F2
    batch_upper_matTr_x_mat_multab( P3, multabs, _V, _V_BYTE, _O, buffer_F2, _O, _O_BYTE );    // UT . T1tr*(F1*T1 + F2) , release buffer_F2

    #else
    memcpy( buffer_F2, P2, _O_BYTE * _V * _O );
    batch_trimat_madd( buffer_F2, P1, sk_O, _V, _V_BYTE, _O, _O_BYTE );          // F1*T1 + F2
    batch_upper_matTr_x_mat( P3, sk_O, _V, _V_BYTE, _O, buffer_F2, _O, _O_BYTE );    // UT . T1tr*(F1*T1 + F2) , release buffer_F2
    #endif
}



void ov_pkc_calculate_Q_from_F( cpk_t *Qs, const sk_t *Fs, const sk_t *Ts ) {
    calculate_P3( Qs->P3, Fs->P1, Fs->S, Ts->O );
}














