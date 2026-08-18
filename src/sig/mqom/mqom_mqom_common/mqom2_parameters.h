#ifndef __MQOM2_PARAMETERS_GENERIC_H__
#define __MQOM2_PARAMETERS_GENERIC_H__

/* Set the default security parameters to cat1-gf16-fast-r3 */
#ifndef MQOM2_PARAM_SECURITY
#define MQOM2_PARAM_SECURITY 128
#endif

/* === 128 bits security === */
#if MQOM2_PARAM_SECURITY == 128
/* === 192 bits security === */
#elif MQOM2_PARAM_SECURITY == 192
/* === 256 bits security === */
#elif MQOM2_PARAM_SECURITY == 256
#else
#error "Sorry, unsupported security parameters: must be one of 128, 192, 256"
#endif

#ifndef MQOM2_PARAM_BASE_FIELD
#define MQOM2_PARAM_BASE_FIELD 4
#endif

/* === Base field GF(2) === */
#if MQOM2_PARAM_BASE_FIELD == 1
/* === Base field GF(4) === */
#elif MQOM2_PARAM_BASE_FIELD == 2
/* === Base field GF(16) === */
#elif MQOM2_PARAM_BASE_FIELD == 4
/* === Base field GF(256) === */
#elif MQOM2_PARAM_BASE_FIELD == 8
#else
#error "Sorry, unsupported base field: must be one of 1, 2, 4, 8"
#endif

#ifndef MQOM2_PARAM_TRADEOFF
#define MQOM2_PARAM_TRADEOFF 0
#endif

/* === Trade-off "Short" === */
#if MQOM2_PARAM_TRADEOFF == 1
/* === Trade-off "Fast" === */
#elif MQOM2_PARAM_TRADEOFF == 0
#else
#error "Sorry, unsupported trade-off: must be one of 'short', 'fast'"
#endif

#ifndef MQOM2_PARAM_NBROUNDS
#define MQOM2_PARAM_NBROUNDS 3
#endif

/* === Variant "3 rounds" === */
#if MQOM2_PARAM_NBROUNDS == 3
/* === Variant "5 rounds" === */
#elif MQOM2_PARAM_NBROUNDS == 5
#else
#error "Sorry, unsupported nb rounds: must be one of 3, 5"
#endif

/******************************************/

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS) && !defined(SUPERCOP)
#if MQOM2_PARAM_SECURITY == 128
#if MQOM2_PARAM_BASE_FIELD == 1
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf2-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf2-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf2-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf2-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 2
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf4-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf4-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf4-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf4-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 4
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf16-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf16-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf16-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf16-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 8
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf256-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf256-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat1-gf256-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat1-gf256-fast-r5.h"
#endif
#endif
#endif

#elif MQOM2_PARAM_SECURITY == 192
#if MQOM2_PARAM_BASE_FIELD == 1
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf2-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf2-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf2-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf2-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 2
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf4-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf4-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf4-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf4-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 4
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf16-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf16-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf16-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf16-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 8
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf256-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf256-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat3-gf256-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat3-gf256-fast-r5.h"
#endif
#endif
#endif

#elif MQOM2_PARAM_SECURITY == 256
#if MQOM2_PARAM_BASE_FIELD == 1
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf2-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf2-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf2-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf2-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 2
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf4-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf4-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf4-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf4-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 4
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf16-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf16-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf16-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf16-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 8
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf256-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf256-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "parameters/mqom2_parameters_cat5-gf256-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "parameters/mqom2_parameters_cat5-gf256-fast-r5.h"
#endif
#endif
#endif
#endif
/*********************************************************/
/****** SUPERCOP/MUPQ/libOQS have a "flat" structure ********************/
#else
#if MQOM2_PARAM_SECURITY == 128
#if MQOM2_PARAM_BASE_FIELD == 1
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf2-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf2-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf2-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf2-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 2
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf4-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf4-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf4-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf4-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 4
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf16-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf16-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf16-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf16-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 8
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf256-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf256-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat1-gf256-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat1-gf256-fast-r5.h"
#endif
#endif
#endif

#elif MQOM2_PARAM_SECURITY == 192
#if MQOM2_PARAM_BASE_FIELD == 1
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf2-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf2-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf2-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf2-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 2
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf4-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf4-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf4-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf4-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 4
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf16-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf16-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf16-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf16-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 8
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf256-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf256-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat3-gf256-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat3-gf256-fast-r5.h"
#endif
#endif
#endif

#elif MQOM2_PARAM_SECURITY == 256
#if MQOM2_PARAM_BASE_FIELD == 1
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf2-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf2-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf2-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf2-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 2
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf4-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf4-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf4-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf4-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 4
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf16-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf16-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf16-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf16-fast-r5.h"
#endif
#endif
#elif MQOM2_PARAM_BASE_FIELD == 8
#if MQOM2_PARAM_TRADEOFF == 1
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf256-short-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf256-short-r5.h"
#endif
#elif MQOM2_PARAM_TRADEOFF == 0
#if MQOM2_PARAM_NBROUNDS == 3
#include "mqom2_parameters_cat5-gf256-fast-r3.h"
#elif MQOM2_PARAM_NBROUNDS == 5
#include "mqom2_parameters_cat5-gf256-fast-r5.h"
#endif
#endif
#endif
#endif
#endif

#endif /* __MQOM2_PARAMETERS_GENERIC_H__ */
