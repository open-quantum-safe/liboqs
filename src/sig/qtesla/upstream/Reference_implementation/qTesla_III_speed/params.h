/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: "qTesla-III-speed" parameters
**************************************************************************************/

#ifndef PARAMS_H
#define PARAMS_H

#define PARAM_N 1024
#define PARAM_N_LOG 10
#define PARAM_SIGMA 10.2
#define PARAM_Xi 12
#define PARAM_Q 8404993
#define PARAM_Q_LOG 24
#define PARAM_QINV 4034936831
#define PARAM_BARR_MULT 511
#define PARAM_BARR_DIV 32
#define PARAM_B 2097151
#define PARAM_B_BITS 21

#define PARAM_K 1
#define PARAM_SIGMA_E PARAM_SIGMA

#define PARAM_W 48
#define PARAM_D 22
	
#define PARAM_GEN_A 38

#define PARAM_KEYGEN_BOUND_E 1147 
#define PARAM_REJECTION PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 1233
#define PARAM_U PARAM_KEYGEN_BOUND_S

#define PARAM_R2_INVN 237839

#endif
