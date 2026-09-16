#ifndef __BLC_H__
#define __BLC_H__

#if MQOM3_PARAM_OT_VARIANT != 1

/* === Correlated-Tree (CT) variant === */
#include "blc_ct.h"
#define blc_key_t CT_blc_key_t
#define BLC_Commit CT_BLC_Commit
#define BLC_Open CT_BLC_Open
#define BLC_Eval CT_BLC_Eval
#define BLC_PrintConfig CT_BLC_PrintConfig
#define BLC_IsValidChallenge CT_BLC_IsValidChallenge

/* Concrete memopt aliases: always resolve to the memopt backend, regardless
 * of the ambient MEMORY_EFFICIENT_BLC choice for this particular build.
 * Sign_memopt/Verify_memopt (sign_memopt.c) are always compiled whether or
 * not they are the active Sign/Verify (see sign.h), so they cannot rely on
 * the plain blc_key_t/BLC_Commit/BLC_Open above, which pick the *default*
 * backend - with a differently-shaped key struct - whenever
 * MEMORY_EFFICIENT_BLC isn't ambiently set for the build at hand. */
#include "blc_ct_memopt.h"
#define blc_key_memopt_t CT_blc_key_memopt_t
#define BLC_Commit_memopt CT_BLC_Commit_memopt
#define BLC_Open_memopt CT_BLC_Open_memopt
#define BLC_Eval_memopt CT_BLC_Eval_memopt
#define BLC_PrintConfig_memopt CT_BLC_PrintConfig_memopt

#else

/* === One-Tree (OT) variant === */
#include "blc_ot.h"
#define blc_key_t OT_blc_key_t
#define BLC_Commit OT_BLC_Commit
#define BLC_Open OT_BLC_Open
#define BLC_Eval OT_BLC_Eval
#define BLC_PrintConfig OT_BLC_PrintConfig
#define BLC_IsValidChallenge OT_BLC_IsValidChallenge

/* See the CT-side comment above. */
#include "blc_ot_memopt.h"
#define blc_key_memopt_t OT_blc_key_memopt_t
#define BLC_Commit_memopt OT_BLC_Commit_memopt
#define BLC_Open_memopt OT_BLC_Open_memopt
#define BLC_Eval_memopt OT_BLC_Eval_memopt
#define BLC_PrintConfig_memopt OT_BLC_PrintConfig_memopt

#endif /* MQOM3_PARAM_OT_VARIANT */

#endif /* __BLC_H__ */
