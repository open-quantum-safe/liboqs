/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: "qTesla-p-I" parameters
**************************************************************************************/

#ifndef PARAMS_H
#define PARAMS_H

#define PARAM_N 1024
#define PARAM_N_LOG 10
#define PARAM_SIGMA 8.5
#define PARAM_Xi 10
#define PARAM_Q 485978113
#define PARAM_Q_LOG 29
#define PARAM_QINV 3421990911
#define PARAM_BARR_MULT 1
#define PARAM_BARR_DIV 29
#define PARAM_B 2097151
#define PARAM_B_BITS 21

#define PARAM_K 4
#define PARAM_SIGMA_E PARAM_SIGMA

#define PARAM_W 25
#define PARAM_D 22

#define PARAM_GEN_A 108
	
#define PARAM_KEYGEN_BOUND_E 554
#define PARAM_REJECTION PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 554
#define PARAM_U PARAM_KEYGEN_BOUND_S

#define PARAM_R2_INVN 472064468

#endif
