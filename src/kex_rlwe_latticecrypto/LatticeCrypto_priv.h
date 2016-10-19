/****************************************************************************************
* LatticeCrypto: an efficient post-quantum Ring-Learning With Errors cryptography library
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
*
* Abstract: internal header file
*
*****************************************************************************************/  

#ifndef __LatticeCrypto_priv_H__
#define __LatticeCrypto_priv_H__


// For C++
#ifdef __cplusplus
extern "C" {
#endif


#include "LatticeCrypto.h"


// Basic constants            
#define PARAMETER_N         1024 
#define PARAMETER_Q         12289 
#define SEED_BYTES          256/8
#define ERROR_SEED_BYTES    256/8
#define NONCE_SEED_BYTES    256/8
#define PARAMETER_Q4        3073 
#define PARAMETER_3Q4       9217 
#define PARAMETER_5Q4       15362 
#define PARAMETER_7Q4       21506 
#define PARAMETER_Q2        6145 
#define PARAMETER_3Q2       18434
    

// Macro definitions

#define NBITS_TO_NWORDS(nbits)      (((nbits)+(sizeof(digit_t)*8)-1)/(sizeof(digit_t)*8))    // Conversion macro from number of bits to number of computer words
#define NBYTES_TO_NWORDS(nbytes)    (((nbytes)+sizeof(digit_t)-1)/sizeof(digit_t))           // Conversion macro from number of bytes to number of computer words

// Macro to avoid compiler warnings when detecting unreferenced parameters
#define UNREFERENCED_PARAMETER(PAR) (PAR)


/******************** Function prototypes *******************/
/******************* Polynomial functions *******************/

// Forward NTT
void NTT_CT_std2rev_12289(int32_t* a, const int32_t* psi_rev, unsigned int N);
void NTT_CT_std2rev_12289_asm(int32_t* a, const int32_t* psi_rev, unsigned int N);

// Inverse NTT
void INTT_GS_rev2std_12289(int32_t* a, const int32_t* omegainv_rev, const int32_t omegainv1N_rev, const int32_t Ninv, unsigned int N);
void INTT_GS_rev2std_12289_asm(int32_t* a, const int32_t* omegainv_rev, const int32_t omegainv1N_rev, const int32_t Ninv, unsigned int N);

// Reduction modulo q
int32_t reduce12289(int64_t a);

// Two merged reductions modulo q
int32_t reduce12289_2x(int64_t a);

// Two consecutive reductions modulo q
void two_reduce12289(int32_t* a, unsigned int N);
void two_reduce12289_asm(int32_t* a, unsigned int N);

// Correction modulo q
void correction(int32_t* a, int32_t p, unsigned int N);

// Component-wise multiplication
void pmul(int32_t* a, int32_t* b, int32_t* c, unsigned int N);
void pmul_asm(int32_t* a, int32_t* b, int32_t* c, unsigned int N);

// Component-wise multiplication and addition
void pmuladd(int32_t* a, int32_t* b, int32_t* c, int32_t* d, unsigned int N);
void pmuladd_asm(int32_t* a, int32_t* b, int32_t* c, int32_t* d, unsigned int N);

// Component-wise multiplication with scalar
void smul(int32_t* a, int32_t scalar, unsigned int N);

/******************* Key exchange functions *******************/

// Alice's message encoding
void encode_A(const uint32_t* pk, const unsigned char* seed, unsigned char* m);

// Alice's message decoding
void decode_A(const unsigned char* m, uint32_t *pk, unsigned char* seed); 
    
// Bob's message encoding
void encode_B(const uint32_t* pk, const uint32_t* rvec, unsigned char* m);
    
// Bob's message decoding
void decode_B(unsigned char* m, uint32_t* pk, uint32_t* rvec);

// Partial message encoding/decoding (assembly optimized) 
void encode_asm(const uint32_t* pk, unsigned char* m);
void decode_asm(const unsigned char* m, uint32_t *pk);

// Reconciliation helper
CRYPTO_STATUS HelpRec(const uint32_t* x, uint32_t* rvec, const unsigned char* seed, unsigned int nonce);

// Partial reconciliation helper (assembly optimized)        
void helprec_asm(const uint32_t* x, uint32_t* rvec, unsigned char* random_bits);

// Reconciliation
void Rec(const uint32_t *x, const uint32_t* rvec, unsigned char *key);
void rec_asm(const uint32_t *x, const uint32_t* rvec, unsigned char *key);

// Error sampling
CRYPTO_STATUS get_error(int32_t* e, unsigned char* seed, unsigned int nonce);

// Partial error sampling (assembly optimized)        
void error_sampling_asm(unsigned char* stream, int32_t* e);

// Generation of parameter a
CRYPTO_STATUS generate_a(uint32_t* a, const unsigned char* seed);

#ifdef __cplusplus
}
#endif


#endif
