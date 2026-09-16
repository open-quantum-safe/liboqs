#ifndef __MQOM3_PARAMETERS_GENERIC_H__
#define __MQOM3_PARAMETERS_GENERIC_H__

/* Set the default security parameters to cat1_gf16_fast */
#ifndef MQOM3_PARAM_SECURITY
#define MQOM3_PARAM_SECURITY 128
#endif

/* === 128 bits security === */
#if MQOM3_PARAM_SECURITY == 128
/* === 192 bits security === */
#elif MQOM3_PARAM_SECURITY == 192
/* === 256 bits security === */
#elif MQOM3_PARAM_SECURITY == 256
#else
#error "Sorry, unsupported security parameters: must be one of 128, 192, 256"
#endif

#ifndef MQOM3_PARAM_BASE_FIELD
#define MQOM3_PARAM_BASE_FIELD 4
#endif

/* === Base field GF(2) === */
#if MQOM3_PARAM_BASE_FIELD == 1
/* === Base field GF(16) === */
#elif MQOM3_PARAM_BASE_FIELD == 4
#else
#error "Sorry, unsupported base field: must be one of 1, 4"
#endif

#ifndef MQOM3_PARAM_TRADEOFF
#define MQOM3_PARAM_TRADEOFF 0
#endif

/* === Trade-off "Fast" === */
#if MQOM3_PARAM_TRADEOFF == 0
/* === Trade-off "Short" === */
#elif MQOM3_PARAM_TRADEOFF == 1
/* === Trade-off "Shorter" === */
#elif MQOM3_PARAM_TRADEOFF == 2
#else
#error "Sorry, unsupported trade-off: must be one of 'fast', 'short', 'shorter'"
#endif

/* === Variant: 0 = Correlated-Tree (CT), 1 = One-Tree (OT) === */
#ifndef MQOM3_PARAM_OT_VARIANT
#define MQOM3_PARAM_OT_VARIANT 0
#endif

/* Sanity check */
#if defined(MQOM3_BUILD_OT_SOURCES) && (MQOM3_BUILD_OT_SOURCES != MQOM3_PARAM_OT_VARIANT)
#error "MQOM3_PARAM_OT_VARIANT contradicts the CT/OT sources selected by MQOM3_VARIANT"
#endif

/* Portable compile-time ceil(log2(x)) for x in [1, 2^32].
 * Returns the smallest h such that 2^h >= x.
 * Evaluates as a constant expression when x is a constant.  */
#ifndef CEIL_LOG2
#define CEIL_LOG2(x) \
    ((x) <= (1U)      ?  0 : (x) <= (1U<< 1) ?  1 : (x) <= (1U<< 2) ?  2 : \
     (x) <= (1U<< 3)  ?  3 : (x) <= (1U<< 4) ?  4 : (x) <= (1U<< 5) ?  5 : \
     (x) <= (1U<< 6)  ?  6 : (x) <= (1U<< 7) ?  7 : (x) <= (1U<< 8) ?  8 : \
     (x) <= (1U<< 9)  ?  9 : (x) <= (1U<<10) ? 10 : (x) <= (1U<<11) ? 11 : \
     (x) <= (1U<<12)  ? 12 : (x) <= (1U<<13) ? 13 : (x) <= (1U<<14) ? 14 : \
     (x) <= (1U<<15)  ? 15 : (x) <= (1U<<16) ? 16 : (x) <= (1U<<17) ? 17 : \
     (x) <= (1U<<18)  ? 18 : (x) <= (1U<<19) ? 19 : (x) <= (1U<<20) ? 20 : \
     (x) <= (1U<<21)  ? 21 : (x) <= (1U<<22) ? 22 : (x) <= (1U<<23) ? 23 : \
     (x) <= (1U<<24)  ? 24 : (x) <= (1U<<25) ? 25 : (x) <= (1U<<26) ? 26 : \
     (x) <= (1U<<27)  ? 27 : (x) <= (1U<<28) ? 28 : (x) <= (1U<<29) ? 29 : \
     (x) <= (1U<<30)  ? 30 : (x) <= (1U<<31) ? 31 : 32)
#endif

/******************************************/

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS) && !defined(SUPERCOP)
#if MQOM3_PARAM_SECURITY == 128
#if MQOM3_PARAM_BASE_FIELD == 1
#if MQOM3_PARAM_TRADEOFF == 2
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat1_gf2_shorter_ct.h"
#else
#include "parameters/mqom3_parameters_cat1_gf2_shorter_ot.h"
#endif
#else
#error "Sorry, GF(2)/cat1 only supports tradeoff 'shorter' (MQOM3_PARAM_TRADEOFF=2)"
#endif
#elif MQOM3_PARAM_BASE_FIELD == 4
#if MQOM3_PARAM_TRADEOFF == 0
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat1_gf16_fast_ct.h"
#else
#include "parameters/mqom3_parameters_cat1_gf16_fast_ot.h"
#endif
#elif MQOM3_PARAM_TRADEOFF == 1
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat1_gf16_short_ct.h"
#else
#include "parameters/mqom3_parameters_cat1_gf16_short_ot.h"
#endif
#else
#error "Sorry, GF(16)/cat1 only supports tradeoffs 'fast' or 'short' (MQOM3_PARAM_TRADEOFF=0 or 1)"
#endif
#endif

#elif MQOM3_PARAM_SECURITY == 192
#if MQOM3_PARAM_BASE_FIELD == 1
#if MQOM3_PARAM_TRADEOFF == 2
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat3_gf2_shorter_ct.h"
#else
#include "parameters/mqom3_parameters_cat3_gf2_shorter_ot.h"
#endif
#else
#error "Sorry, GF(2)/cat3 only supports tradeoff 'shorter' (MQOM3_PARAM_TRADEOFF=2)"
#endif
#elif MQOM3_PARAM_BASE_FIELD == 4
#if MQOM3_PARAM_TRADEOFF == 0
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat3_gf16_fast_ct.h"
#else
#include "parameters/mqom3_parameters_cat3_gf16_fast_ot.h"
#endif
#elif MQOM3_PARAM_TRADEOFF == 1
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat3_gf16_short_ct.h"
#else
#include "parameters/mqom3_parameters_cat3_gf16_short_ot.h"
#endif
#else
#error "Sorry, GF(16)/cat3 only supports tradeoffs 'fast' or 'short' (MQOM3_PARAM_TRADEOFF=0 or 1)"
#endif
#endif

#elif MQOM3_PARAM_SECURITY == 256
#if MQOM3_PARAM_BASE_FIELD == 1
#if MQOM3_PARAM_TRADEOFF == 2
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat5_gf2_shorter_ct.h"
#else
#include "parameters/mqom3_parameters_cat5_gf2_shorter_ot.h"
#endif
#else
#error "Sorry, GF(2)/cat5 only supports tradeoff 'shorter' (MQOM3_PARAM_TRADEOFF=2)"
#endif
#elif MQOM3_PARAM_BASE_FIELD == 4
#if MQOM3_PARAM_TRADEOFF == 0
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat5_gf16_fast_ct.h"
#else
#include "parameters/mqom3_parameters_cat5_gf16_fast_ot.h"
#endif
#elif MQOM3_PARAM_TRADEOFF == 1
#if MQOM3_PARAM_OT_VARIANT == 0
#include "parameters/mqom3_parameters_cat5_gf16_short_ct.h"
#else
#include "parameters/mqom3_parameters_cat5_gf16_short_ot.h"
#endif
#else
#error "Sorry, GF(16)/cat5 only supports tradeoffs 'fast' or 'short' (MQOM3_PARAM_TRADEOFF=0 or 1)"
#endif
#endif
#endif
/*********************************************************/
/****** SUPERCOP/MUPQ/libOQS have a "flat" structure ********************/
#else
#if MQOM3_PARAM_SECURITY == 128
#if MQOM3_PARAM_BASE_FIELD == 1
#if MQOM3_PARAM_TRADEOFF == 2
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat1_gf2_shorter_ct.h"
#else
#include "mqom3_parameters_cat1_gf2_shorter_ot.h"
#endif
#else
#error "Sorry, GF(2)/cat1 only supports tradeoff 'shorter' (MQOM3_PARAM_TRADEOFF=2)"
#endif
#elif MQOM3_PARAM_BASE_FIELD == 4
#if MQOM3_PARAM_TRADEOFF == 0
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat1_gf16_fast_ct.h"
#else
#include "mqom3_parameters_cat1_gf16_fast_ot.h"
#endif
#elif MQOM3_PARAM_TRADEOFF == 1
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat1_gf16_short_ct.h"
#else
#include "mqom3_parameters_cat1_gf16_short_ot.h"
#endif
#else
#error "Sorry, GF(16)/cat1 only supports tradeoffs 'fast' or 'short' (MQOM3_PARAM_TRADEOFF=0 or 1)"
#endif
#endif

#elif MQOM3_PARAM_SECURITY == 192
#if MQOM3_PARAM_BASE_FIELD == 1
#if MQOM3_PARAM_TRADEOFF == 2
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat3_gf2_shorter_ct.h"
#else
#include "mqom3_parameters_cat3_gf2_shorter_ot.h"
#endif
#else
#error "Sorry, GF(2)/cat3 only supports tradeoff 'shorter' (MQOM3_PARAM_TRADEOFF=2)"
#endif
#elif MQOM3_PARAM_BASE_FIELD == 4
#if MQOM3_PARAM_TRADEOFF == 0
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat3_gf16_fast_ct.h"
#else
#include "mqom3_parameters_cat3_gf16_fast_ot.h"
#endif
#elif MQOM3_PARAM_TRADEOFF == 1
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat3_gf16_short_ct.h"
#else
#include "mqom3_parameters_cat3_gf16_short_ot.h"
#endif
#else
#error "Sorry, GF(16)/cat3 only supports tradeoffs 'fast' or 'short' (MQOM3_PARAM_TRADEOFF=0 or 1)"
#endif
#endif

#elif MQOM3_PARAM_SECURITY == 256
#if MQOM3_PARAM_BASE_FIELD == 1
#if MQOM3_PARAM_TRADEOFF == 2
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat5_gf2_shorter_ct.h"
#else
#include "mqom3_parameters_cat5_gf2_shorter_ot.h"
#endif
#else
#error "Sorry, GF(2)/cat5 only supports tradeoff 'shorter' (MQOM3_PARAM_TRADEOFF=2)"
#endif
#elif MQOM3_PARAM_BASE_FIELD == 4
#if MQOM3_PARAM_TRADEOFF == 0
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat5_gf16_fast_ct.h"
#else
#include "mqom3_parameters_cat5_gf16_fast_ot.h"
#endif
#elif MQOM3_PARAM_TRADEOFF == 1
#if MQOM3_PARAM_OT_VARIANT == 0
#include "mqom3_parameters_cat5_gf16_short_ct.h"
#else
#include "mqom3_parameters_cat5_gf16_short_ot.h"
#endif
#else
#error "Sorry, GF(16)/cat5 only supports tradeoffs 'fast' or 'short' (MQOM3_PARAM_TRADEOFF=0 or 1)"
#endif
#endif
#endif
#endif

#endif /* __MQOM3_PARAMETERS_GENERIC_H__ */
