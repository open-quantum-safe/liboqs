#ifndef __BLC_CT_H__
#define __BLC_CT_H__

#if defined(MEMORY_EFFICIENT_BLC)
#include "blc_ct_memopt.h"
#define CT_blc_key_t CT_blc_key_memopt_t
#define CT_BLC_Commit CT_BLC_Commit_memopt
#define CT_BLC_Open CT_BLC_Open_memopt
#define CT_BLC_Eval CT_BLC_Eval_memopt
#define CT_BLC_PrintConfig CT_BLC_PrintConfig_memopt
#else
#include "blc_ct_default.h"
#define CT_blc_key_t CT_blc_key_default_t
#define CT_BLC_Commit CT_BLC_Commit_default
#define CT_BLC_Open CT_BLC_Open_default
#define CT_BLC_Eval CT_BLC_Eval_default
#define CT_BLC_PrintConfig CT_BLC_PrintConfig_default
#endif

/* CT has no cross-execution tree structure to validate: any i_star is valid. */
static inline int CT_BLC_IsValidChallenge(const uint16_t i_star[MQOM3_PARAM_TAU]) {
	(void)i_star;
	return 1;
}

#endif /* __BLC_CT_H__ */
