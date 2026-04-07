#ifndef __MQOM2_PARAMETERS_H__
#define __MQOM2_PARAMETERS_H__

#define MQOM2_PARAM_LABEL "MQOM2-L3-gf16-fast-r3"
#define MQOM2_PARAM_KAT cat3_gf16_fast_r3

#ifndef MQOM2_PARAM_SECURITY
#define MQOM2_PARAM_SECURITY 192
#endif

/* ==== Fields parameters */
#ifndef MQOM2_PARAM_BASE_FIELD
#define MQOM2_PARAM_BASE_FIELD 4
#endif

#ifndef MQOM2_PARAM_EXT_FIELD
#define MQOM2_PARAM_EXT_FIELD 8
#endif

#define MQOM2_PARAM_MU (MQOM2_PARAM_EXT_FIELD / MQOM2_PARAM_BASE_FIELD)

/* ==== MQ parameters */
#ifndef MQOM2_PARAM_MQ_N
#define MQOM2_PARAM_MQ_N 84
#endif
#ifndef MQOM2_PARAM_MQ_M
#define MQOM2_PARAM_MQ_M MQOM2_PARAM_MQ_N
#endif

/* ==== PIOP parameters */
#ifndef MQOM2_PARAM_TAU
#define MQOM2_PARAM_TAU 27
#endif

#ifndef MQOM2_PARAM_NB_EVALS_LOG
#define MQOM2_PARAM_NB_EVALS_LOG 8
#endif
#define MQOM2_PARAM_NB_EVALS (1 << MQOM2_PARAM_NB_EVALS_LOG)
#define MQOM2_PARAM_FULL_TREE_SIZE ((1 << (MQOM2_PARAM_NB_EVALS_LOG + 1)) - 1)

#define MQOM2_PARAM_WITH_STATISTICAL_BATCHING 0

#ifndef MQOM2_PARAM_ETA
#if MQOM2_PARAM_MQ_M % MQOM2_PARAM_MU != 0
#error "Sorry, unsupported parameters: mu should divide the number of MQ equations"
#endif
#define MQOM2_PARAM_ETA (MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)
#endif

#ifndef MQOM2_PARAM_W
#define MQOM2_PARAM_W 3
#endif

/* ==== Signature parameters*/
#define MQOM2_PARAM_SEED_SIZE (MQOM2_PARAM_SECURITY / 8)
#define MQOM2_PARAM_SALT_SIZE (MQOM2_PARAM_SECURITY / 8)
#define MQOM2_PARAM_DIGEST_SIZE (2*MQOM2_PARAM_SECURITY / 8)

#endif /* __MQOM2_PARAMETERS_H__ */
