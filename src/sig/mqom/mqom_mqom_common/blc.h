#ifndef __BLC_H__
#define __BLC_H__

#if defined(MEMORY_EFFICIENT_BLC)
#include "blc_memopt.h"
#define blc_key_t blc_key_memopt_t
#define BLC_Commit BLC_Commit_memopt
#define BLC_Open BLC_Open_memopt
#define BLC_Eval BLC_Eval_memopt
#else
#include "blc_default.h"
#define blc_key_t blc_key_default_t
#define BLC_Commit BLC_Commit_default
#define BLC_Open BLC_Open_default
#define BLC_Eval BLC_Eval_default
#endif

#endif /* __BLC_H__ */
