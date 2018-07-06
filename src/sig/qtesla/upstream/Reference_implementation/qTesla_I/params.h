/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: "qTESLA-I" parameters
**************************************************************************************/

#ifndef PARAMS_H
#define PARAMS_H

#define PARAM_N 512
#define PARAM_N_LOG 9
#define PARAM_SIGMA 23.78
#define PARAM_Xi 27.9988
#define PARAM_Q 4205569
#define PARAM_Q_LOG 23
#define PARAM_QINV 3098553343
#define PARAM_BARR_MULT 1021
#define PARAM_BARR_DIV 32
#define PARAM_B 1048575
#define PARAM_B_BITS 20

#define PARAM_K 1
#define PARAM_SIGMA_E PARAM_SIGMA

#define PARAM_W 30
#define PARAM_D 21

#define PARAM_GEN_A 19
	
#define PARAM_KEYGEN_BOUND_E 1586 
#define PARAM_REJECTION PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 1586
#define PARAM_U PARAM_KEYGEN_BOUND_S

#define PARAM_R2_INVN 113307

#endif
