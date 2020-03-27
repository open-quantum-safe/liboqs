/// @file rainbow_keypair_computation.c
/// @brief Implementations for functions in rainbow_keypair_computation.h
///

#include "rainbow_keypair_computation.h"
#include "blas.h"
#include "blas_comm.h"
#include "rainbow_blas.h"
#include "rainbow_keypair.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void PQCLEAN_RAINBOWVCCLASSIC_CLEAN_extcpk_to_pk(pk_t *pk, const ext_cpk_t *cpk) {
    const unsigned char *idx_l1 = cpk->l1_Q1;
    const unsigned char *idx_l2 = cpk->l2_Q1;
    for (unsigned int i = 0; i < _V1; i++) {
        for (unsigned int j = i; j < _V1; j++) {
            unsigned int pub_idx = PQCLEAN_RAINBOWVCCLASSIC_CLEAN_idx_of_trimat(i, j, _PUB_N);
            memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx_l1, _O1_BYTE);
            memcpy((&pk->pk[_PUB_M_BYTE * pub_idx]) + _O1_BYTE, idx_l2, _O2_BYTE);
            idx_l1 += _O1_BYTE;
            idx_l2 += _O2_BYTE;
        }
    }
    idx_l1 = cpk->l1_Q2;
    idx_l2 = cpk->l2_Q2;
    for (unsigned int i = 0; i < _V1; i++) {
        for (unsigned int j = _V1; j < _V1 + _O1; j++) {
            unsigned int pub_idx = PQCLEAN_RAINBOWVCCLASSIC_CLEAN_idx_of_trimat(i, j, _PUB_N);
            memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx_l1, _O1_BYTE);
            memcpy((&pk->pk[_PUB_M_BYTE * pub_idx]) + _O1_BYTE, idx_l2, _O2_BYTE);
            idx_l1 += _O1_BYTE;
            idx_l2 += _O2_BYTE;
        }
    }
    idx_l1 = cpk->l1_Q3;
    idx_l2 = cpk->l2_Q3;
    for (unsigned int i = 0; i < _V1; i++) {
        for (unsigned int j = _V1 + _O1; j < _PUB_N; j++) {
            unsigned int pub_idx = PQCLEAN_RAINBOWVCCLASSIC_CLEAN_idx_of_trimat(i, j, _PUB_N);
            memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx_l1, _O1_BYTE);
            memcpy((&pk->pk[_PUB_M_BYTE * pub_idx]) + _O1_BYTE, idx_l2, _O2_BYTE);
            idx_l1 += _O1_BYTE;
            idx_l2 += _O2_BYTE;
        }
    }
    idx_l1 = cpk->l1_Q5;
    idx_l2 = cpk->l2_Q5;
    for (unsigned int i = _V1; i < _V1 + _O1; i++) {
        for (unsigned int j = i; j < _V1 + _O1; j++) {
            unsigned int pub_idx = PQCLEAN_RAINBOWVCCLASSIC_CLEAN_idx_of_trimat(i, j, _PUB_N);
            memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx_l1, _O1_BYTE);
            memcpy((&pk->pk[_PUB_M_BYTE * pub_idx]) + _O1_BYTE, idx_l2, _O2_BYTE);
            idx_l1 += _O1_BYTE;
            idx_l2 += _O2_BYTE;
        }
    }
    idx_l1 = cpk->l1_Q6;
    idx_l2 = cpk->l2_Q6;
    for (unsigned int i = _V1; i < _V1 + _O1; i++) {
        for (unsigned int j = _V1 + _O1; j < _PUB_N; j++) {
            unsigned int pub_idx = PQCLEAN_RAINBOWVCCLASSIC_CLEAN_idx_of_trimat(i, j, _PUB_N);
            memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx_l1, _O1_BYTE);
            memcpy((&pk->pk[_PUB_M_BYTE * pub_idx]) + _O1_BYTE, idx_l2, _O2_BYTE);
            idx_l1 += _O1_BYTE;
            idx_l2 += _O2_BYTE;
        }
    }
    idx_l1 = cpk->l1_Q9;
    idx_l2 = cpk->l2_Q9;
    for (unsigned int i = _V1 + _O1; i < _PUB_N; i++) {
        for (unsigned int j = i; j < _PUB_N; j++) {
            unsigned int pub_idx = PQCLEAN_RAINBOWVCCLASSIC_CLEAN_idx_of_trimat(i, j, _PUB_N);
            memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx_l1, _O1_BYTE);
            memcpy((&pk->pk[_PUB_M_BYTE * pub_idx]) + _O1_BYTE, idx_l2, _O2_BYTE);
            idx_l1 += _O1_BYTE;
            idx_l2 += _O2_BYTE;
        }
    }
}

static void calculate_Q_from_F_ref(ext_cpk_t *Qs, const sk_t *Fs, const sk_t *Ts) {
    /*
          Layer 1
          Computing :
          Q_pk.l1_F1s[i] = F_sk.l1_F1s[i]

          Q_pk.l1_F2s[i] = (F1* T1 + F2) + F1tr * t1
          Q_pk.l1_F5s[i] = UT( T1tr* (F1 * T1 + F2) )
      */
    const unsigned char *t2 = Ts->t4;

    memcpy(Qs->l1_Q1, Fs->l1_F1, _O1_BYTE * N_TRIANGLE_TERMS(_V1));

    memcpy(Qs->l1_Q2, Fs->l1_F2, _O1_BYTE * _V1 * _O1);
    batch_trimat_madd(Qs->l1_Q2, Fs->l1_F1, Ts->t1, _V1, _V1_BYTE, _O1, _O1_BYTE); // F1*T1 + F2

    memset(Qs->l1_Q3, 0, _O1_BYTE * _V1 * _O2);
    memset(Qs->l1_Q5, 0, _O1_BYTE * N_TRIANGLE_TERMS(_O1));
    memset(Qs->l1_Q6, 0, _O1_BYTE * _O1 * _O2);
    memset(Qs->l1_Q9, 0, _O1_BYTE * N_TRIANGLE_TERMS(_O2));

    // l1_Q5 : _O1_BYTE * _O1 * _O1
    // l1_Q9 : _O1_BYTE * _O2 * _O2
    // l2_Q5 : _O2_BYTE * _V1 * _O1
    // l2_Q9 : _O2_BYTE * _V1 * _O2

    unsigned char tempQ[_O1_BYTE * _O1 * _O1 + 32];

    memset(tempQ, 0, _O1_BYTE * _O1 * _O1);                                        // l1_Q5
    batch_matTr_madd(tempQ, Ts->t1, _V1, _V1_BYTE, _O1, Qs->l1_Q2, _O1, _O1_BYTE); // t1_tr*(F1*T1 + F2)
    PQCLEAN_RAINBOWVCCLASSIC_CLEAN_UpperTrianglize(Qs->l1_Q5, tempQ, _O1, _O1_BYTE);            // UT( ... )   // Q5

    batch_trimatTr_madd(Qs->l1_Q2, Fs->l1_F1, Ts->t1, _V1, _V1_BYTE, _O1, _O1_BYTE); // Q2
    /*
          Computing:
          F1_T2     = F1 * t2
          F2_T3     = F2 * t3
          F1_F1T_T2 + F2_T3 = F1_T2 + F2_T3 + F1tr * t2
          Q_pk.l1_F3s[i] =         F1_F1T_T2 + F2_T3
          Q_pk.l1_F6s[i] = T1tr* ( F1_F1T_T2 + F2_T3 ) + F2tr * t2
          Q_pk.l1_F9s[i] = UT( T2tr* ( F1_T2 + F2_T3 ) )
      */
    batch_trimat_madd(Qs->l1_Q3, Fs->l1_F1, t2, _V1, _V1_BYTE, _O2, _O1_BYTE);       // F1*T2
    batch_mat_madd(Qs->l1_Q3, Fs->l1_F2, _V1, Ts->t3, _O1, _O1_BYTE, _O2, _O1_BYTE); // F1_T2 + F2_T3

    memset(tempQ, 0, _O1_BYTE * _O2 * _O2);                                    // l1_Q9
    batch_matTr_madd(tempQ, t2, _V1, _V1_BYTE, _O2, Qs->l1_Q3, _O2, _O1_BYTE); // T2tr * ( F1_T2 + F2_T3 )
    PQCLEAN_RAINBOWVCCLASSIC_CLEAN_UpperTrianglize(Qs->l1_Q9, tempQ, _O2, _O1_BYTE);        // Q9

    batch_trimatTr_madd(Qs->l1_Q3, Fs->l1_F1, t2, _V1, _V1_BYTE, _O2, _O1_BYTE); // F1_F1T_T2 + F2_T3  // Q3

    batch_bmatTr_madd(Qs->l1_Q6, Fs->l1_F2, _O1, t2, _V1, _V1_BYTE, _O2, _O1_BYTE);    // F2tr*T2
    batch_matTr_madd(Qs->l1_Q6, Ts->t1, _V1, _V1_BYTE, _O1, Qs->l1_Q3, _O2, _O1_BYTE); // Q6

    /*
          layer 2
          Computing:
          Q1 = F1
          Q2 = F1_F1T*T1 + F2
          Q5 = UT( T1tr( F1*T1 + F2 )  + F5 )
      */
    memcpy(Qs->l2_Q1, Fs->l2_F1, _O2_BYTE * N_TRIANGLE_TERMS(_V1));

    memcpy(Qs->l2_Q2, Fs->l2_F2, _O2_BYTE * _V1 * _O1);
    batch_trimat_madd(Qs->l2_Q2, Fs->l2_F1, Ts->t1, _V1, _V1_BYTE, _O1, _O2_BYTE); // F1*T1 + F2

    memcpy(Qs->l2_Q5, Fs->l2_F5, _O2_BYTE * N_TRIANGLE_TERMS(_O1));
    memset(tempQ, 0, _O2_BYTE * _O1 * _O1);                                        // l2_Q5
    batch_matTr_madd(tempQ, Ts->t1, _V1, _V1_BYTE, _O1, Qs->l2_Q2, _O1, _O2_BYTE); // t1_tr*(F1*T1 + F2)
    PQCLEAN_RAINBOWVCCLASSIC_CLEAN_UpperTrianglize(Qs->l2_Q5, tempQ, _O1, _O2_BYTE);            // UT( ... )   // Q5

    batch_trimatTr_madd(Qs->l2_Q2, Fs->l2_F1, Ts->t1, _V1, _V1_BYTE, _O1, _O2_BYTE); // Q2

    /*
          Computing:
          F1_T2     = F1 * t2
          F2_T3     = F2 * t3
          F1_F1T_T2 + F2_T3 = F1_T2 + F2_T3 + F1tr * t2

          Q3 =        F1_F1T*T2 + F2*T3 + F3
          Q9 = UT( T2tr*( F1*T2 + F2*T3 + F3 )  +      T3tr*( F5*T3 + F6 ) )
          Q6 = T1tr*( F1_F1T*T2 + F2*T3 + F3 )  + F2Tr*T2 + F5_F5T*T3 + F6
      */
    memcpy(Qs->l2_Q3, Fs->l2_F3, _O2_BYTE * _V1 * _O2);
    batch_trimat_madd(Qs->l2_Q3, Fs->l2_F1, t2, _V1, _V1_BYTE, _O2, _O2_BYTE);       // F1*T2 + F3
    batch_mat_madd(Qs->l2_Q3, Fs->l2_F2, _V1, Ts->t3, _O1, _O1_BYTE, _O2, _O2_BYTE); // F1_T2 + F2_T3 + F3

    memset(tempQ, 0, _O2_BYTE * _O2 * _O2);                                    // l2_Q9
    batch_matTr_madd(tempQ, t2, _V1, _V1_BYTE, _O2, Qs->l2_Q3, _O2, _O2_BYTE); // T2tr * ( ..... )

    memcpy(Qs->l2_Q6, Fs->l2_F6, _O2_BYTE * _O1 * _O2);

    batch_trimat_madd(Qs->l2_Q6, Fs->l2_F5, Ts->t3, _O1, _O1_BYTE, _O2, _O2_BYTE); // F5*T3 + F6
    batch_matTr_madd(tempQ, Ts->t3, _O1, _O1_BYTE, _O2, Qs->l2_Q6, _O2, _O2_BYTE); // T2tr*( ..... ) + T3tr*( ..... )
    memset(Qs->l2_Q9, 0, _O2_BYTE * N_TRIANGLE_TERMS(_O2));
    PQCLEAN_RAINBOWVCCLASSIC_CLEAN_UpperTrianglize(Qs->l2_Q9, tempQ, _O2, _O2_BYTE); // Q9

    batch_trimatTr_madd(Qs->l2_Q3, Fs->l2_F1, t2, _V1, _V1_BYTE, _O2, _O2_BYTE); // F1_F1T_T2 + F2_T3 + F3 // Q3

    batch_bmatTr_madd(Qs->l2_Q6, Fs->l2_F2, _O1, t2, _V1, _V1_BYTE, _O2, _O2_BYTE);    //  F5*T3 + F6 +  F2tr*T2
    batch_trimatTr_madd(Qs->l2_Q6, Fs->l2_F5, Ts->t3, _O1, _O1_BYTE, _O2, _O2_BYTE);   //   F2tr*T2 + F5_F5T*T3 + F6
    batch_matTr_madd(Qs->l2_Q6, Ts->t1, _V1, _V1_BYTE, _O1, Qs->l2_Q3, _O2, _O2_BYTE); // Q6
}
#define calculate_Q_from_F_impl calculate_Q_from_F_ref
void PQCLEAN_RAINBOWVCCLASSIC_CLEAN_calculate_Q_from_F(ext_cpk_t *Qs, const sk_t *Fs, const sk_t *Ts) {
    calculate_Q_from_F_impl(Qs, Fs, Ts);
}
