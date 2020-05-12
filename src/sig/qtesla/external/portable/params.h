/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: provably-secure qTESLA parameters
**************************************************************************************/

#ifndef PARAMS_H
#define PARAMS_H

#if defined(_qTESLA_p_I_)

#define PARAM_N 1024
#define PARAM_N_LOG 10
#define PARAM_SIGMA 8.5
#define PARAM_Q 343576577
#define PARAM_Q_LOG 29
#define PARAM_QINV 2205847551
#define PARAM_BARR_MULT 3
#define PARAM_BARR_DIV 30
#define PARAM_B 524287
#define PARAM_B_BITS 19
#define PARAM_S_BITS 8
#define PARAM_K 4
#define PARAM_SIGMA_E PARAM_SIGMA
#define PARAM_H 25
#define PARAM_D 22
#define PARAM_GEN_A 108
#define PARAM_KEYGEN_BOUND_E 554
#define PARAM_E PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 554
#define PARAM_S PARAM_KEYGEN_BOUND_S
#define PARAM_R2_INVN 13632409
#define PARAM_R 172048372
#define SHAKE OQS_SHA3_shake128
#define cSHAKE OQS_SHA3_cshake128_simple
#define SHAKE_RATE OQS_SHA3_SHAKE128_RATE

#elif defined(_qTESLA_p_III_)

#define PARAM_N 2048
#define PARAM_N_LOG 11
#define PARAM_SIGMA 8.5
#define PARAM_Q 856145921
#define PARAM_Q_LOG 30
#define PARAM_QINV 587710463
#define PARAM_BARR_MULT 5
#define PARAM_BARR_DIV 32
#define PARAM_B 2097151
#define PARAM_B_BITS 21
#define PARAM_S_BITS 8
#define PARAM_K 5
#define PARAM_SIGMA_E PARAM_SIGMA
#define PARAM_H 40
#define PARAM_D 24
#define PARAM_GEN_A 180
#define PARAM_KEYGEN_BOUND_E 901
#define PARAM_E PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 901
#define PARAM_S PARAM_KEYGEN_BOUND_S
#define PARAM_R2_INVN 513161157
#define PARAM_R 14237691
#define SHAKE OQS_SHA3_shake256
#define cSHAKE OQS_SHA3_cshake256_simple
#define SHAKE_RATE OQS_SHA3_SHAKE256_RATE

#endif

#endif
