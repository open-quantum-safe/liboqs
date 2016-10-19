/****************************************************************************************
* LatticeCrypto: an efficient post-quantum Ring-Learning With Errors cryptography library
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
*
* Abstract: NTT functions and other low-level operations
*
*****************************************************************************************/

#include "../LatticeCrypto_priv.h"
    

void NTT_CT_std2rev_12289(int32_t* a, const int32_t* psi_rev, unsigned int N)
{
    NTT_CT_std2rev_12289_asm(a, psi_rev, N);
}


void INTT_GS_rev2std_12289(int32_t* a, const int32_t* omegainv_rev, const int32_t omegainv1N_rev, const int32_t Ninv, unsigned int N)
{
    INTT_GS_rev2std_12289_asm(a, omegainv_rev, omegainv1N_rev, Ninv, N);
}


void two_reduce12289(int32_t* a, unsigned int N)
{
    two_reduce12289_asm(a, N);
}


void pmul(int32_t* a, int32_t* b, int32_t* c, unsigned int N)
{
    pmul_asm(a, b, c, N);
}


void pmuladd(int32_t* a, int32_t* b, int32_t* c, int32_t* d, unsigned int N)
{
    pmuladd_asm(a, b, c, d, N);
}


void smul(int32_t* a, int32_t scalar, unsigned int N)
{
    unsigned int i; 

    for (i = 0; i < N; i++) {
        a[i] = a[i]*scalar;
    }
}


void correction(int32_t* a, int32_t p, unsigned int N)
{  
    unsigned int i; 
    int32_t mask;

    for (i = 0; i < N; i++) {
        mask = a[i] >> (4*sizeof(int32_t) - 1);
        a[i] += (p & mask) - p;
        mask = a[i] >> (4*sizeof(int32_t) - 1);
        a[i] += (p & mask);
    }
}
