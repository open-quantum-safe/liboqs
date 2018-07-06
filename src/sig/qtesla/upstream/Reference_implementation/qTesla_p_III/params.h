/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: "qTesla-p-III" parameters
**************************************************************************************/

#ifndef PARAMS_H
#define PARAMS_H

#define PARAM_N 2048
#define PARAM_N_LOG 11
#define PARAM_SIGMA 8.5
#define PARAM_Xi 10
#define PARAM_Q 1129725953
#define PARAM_Q_LOG 31
#define PARAM_QINV 861290495
#define PARAM_BARR_MULT 15
#define PARAM_BARR_DIV 34
#define PARAM_B 8388607
#define PARAM_B_BITS 23

#define PARAM_K 5
#define PARAM_SIGMA_E PARAM_SIGMA

#define PARAM_W 40
#define PARAM_D 24

#define PARAM_GEN_A 180
	
#define PARAM_KEYGEN_BOUND_E 901
#define PARAM_REJECTION PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 901
#define PARAM_U PARAM_KEYGEN_BOUND_S

#define PARAM_R2_INVN 851423148

#endif
