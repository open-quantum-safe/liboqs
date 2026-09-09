#ifndef __MQOM3_PARAMETERS_H__
#define __MQOM3_PARAMETERS_H__

#define MQOM3_PARAM_LABEL "MQOM3-L3-gf16-fast-ct"
#define MQOM3_PARAM_KAT cat3_gf16_fast_ct

/* Which BLC scheme this parameter set describes. Affirmed here (and not
 * only in mqom3_parameters.h) so the header stays self-sufficient. */
#ifndef MQOM3_PARAM_OT_VARIANT
#define MQOM3_PARAM_OT_VARIANT 0   /* Correlated-Tree (CT) */
#elif MQOM3_PARAM_OT_VARIANT != 0
#error "mqom3_parameters_cat3_gf16_fast_ct.h included for a build that is not Correlated-Tree (CT)"
#endif

#ifndef MQOM3_PARAM_SECURITY
#define MQOM3_PARAM_SECURITY 192
#endif

/* ==== Fields parameters */
#ifndef MQOM3_PARAM_BASE_FIELD
#define MQOM3_PARAM_BASE_FIELD 4
#endif

#ifndef MQOM3_PARAM_EXT_FIELD
#define MQOM3_PARAM_EXT_FIELD 8
#endif

#define MQOM3_PARAM_MU (MQOM3_PARAM_EXT_FIELD / MQOM3_PARAM_BASE_FIELD)

/* ==== MQ parameters */
#ifndef MQOM3_PARAM_MQ_N
#define MQOM3_PARAM_MQ_N 96
#endif
#ifndef MQOM3_PARAM_MQ_M
#define MQOM3_PARAM_MQ_M MQOM3_PARAM_MQ_N
#endif

/* ==== PIOP parameters */
#ifndef MQOM3_PARAM_TAU
#define MQOM3_PARAM_TAU 26
#endif

#ifndef MQOM3_PARAM_NB_EVALS_LOG
#define MQOM3_PARAM_NB_EVALS_LOG 8
#endif
#define MQOM3_PARAM_NB_EVALS (1 << MQOM3_PARAM_NB_EVALS_LOG)
#define MQOM3_PARAM_FULL_TREE_SIZE ((1 << (MQOM3_PARAM_NB_EVALS_LOG + 1)) - 1)


#ifndef MQOM3_PARAM_ETA
#define MQOM3_PARAM_ETA (MQOM3_PARAM_SECURITY/MQOM3_PARAM_EXT_FIELD)
#endif

#ifndef MQOM3_PARAM_W
#define MQOM3_PARAM_W 10
#endif

/* Total grinding proof-of-work in bits: w_tot = -log2(p1*p2), where p1 is the
 * AES truncation-check acceptance probability (from W above) and p2 is the
 * BLC_IsValidChallenge acceptance probability (=1 for CT; for OT, the chance
 * a random T_open-bounded co-path is valid - no closed form, empirically
 * tuned, see specs table 'Rejection sampling of MQOM'). Verified empirically:
 * the real average number of grinding trials is 2^(MQOM3_PARAM_GRIND_WTOT - 1),
 * for both CT (w_tot == W exactly) and OT. */
#ifndef MQOM3_PARAM_GRIND_WTOT
#define MQOM3_PARAM_GRIND_WTOT 10.0
#endif

/* Precomputed round(2^(MQOM3_PARAM_GRIND_WTOT - 1)): integer companion
 * to GRIND_WTOT (a float literal, unusable in integer/preprocessor
 * arithmetic) so embedded code can compute the theoretical mean trial
 * count without floats. */
#ifndef MQOM3_PARAM_GRIND_MEAN_TRIALS
#define MQOM3_PARAM_GRIND_MEAN_TRIALS 512
#endif

/* ==== Signature parameters*/
#define MQOM3_PARAM_SEED_SIZE (MQOM3_PARAM_SECURITY / 8)
#define MQOM3_PARAM_SALT_SIZE (MQOM3_PARAM_SECURITY / 8)
#define MQOM3_PARAM_DIGEST_SIZE (2*MQOM3_PARAM_SECURITY / 8)

#endif /* __MQOM3_PARAMETERS_H__ */
