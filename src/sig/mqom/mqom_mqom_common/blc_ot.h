#ifndef __BLC_OT_H__
#define __BLC_OT_H__

#if defined(MEMORY_EFFICIENT_BLC)
#include "blc_ot_memopt.h"
#define OT_blc_key_t OT_blc_key_memopt_t
#define OT_BLC_Commit OT_BLC_Commit_memopt
#define OT_BLC_Open OT_BLC_Open_memopt
#define OT_BLC_Eval OT_BLC_Eval_memopt
#define OT_BLC_PrintConfig OT_BLC_PrintConfig_memopt
#else
#include "blc_ot_default.h"
#define OT_blc_key_t OT_blc_key_default_t
#define OT_BLC_Commit OT_BLC_Commit_default
#define OT_BLC_Open OT_BLC_Open_default
#define OT_BLC_Eval OT_BLC_Eval_default
#define OT_BLC_PrintConfig OT_BLC_PrintConfig_default
#endif

#include "ggm_tree_large.h"
#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif
static inline int OT_BLC_IsValidChallenge(const uint16_t i_star[MQOM3_PARAM_TAU]) {
	uint32_t e;
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];
	/* Convert per-execution indices to absolute tree leaf indices: tau*i*[e] + e (column-major BLC convention) */
	for(e = 0; e < MQOM3_PARAM_TAU; e++) {
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing. Required here too: this function is called from the Sign-side grinding loop, and
		 * LargeGGMIsValidOpeningSet below sorts the derived indexes with a value-dependent insertion sort. */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
		hidden_leaf_idxs[e] = (uint32_t)MQOM3_PARAM_TAU * i_star[e] + e;
	}
	return LargeGGMIsValidOpeningSet(hidden_leaf_idxs);
}

#endif /* __BLC_OT_H__ */
