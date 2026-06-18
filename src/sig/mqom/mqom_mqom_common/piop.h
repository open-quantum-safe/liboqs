#ifndef __PIOP_H__
#define __PIOP_H__

#if defined(PIOP_BITSLICE) && defined(MEMORY_EFFICIENT_PIOP)
#error "PIOP bitslice and memopt can not be activated simultaneously."
#endif

#define ComputePAlpha_default MQOM_NAMESPACE(ComputePAlpha_default)
#define RecomputePAlpha_default MQOM_NAMESPACE(RecomputePAlpha_default)
#define ComputePAlpha_memopt MQOM_NAMESPACE(ComputePAlpha_memopt)
#define RecomputePAlpha_memopt MQOM_NAMESPACE(RecomputePAlpha_memopt)
#define ComputePAlpha_bitslice MQOM_NAMESPACE(ComputePAlpha_bitslice)
#define RecomputePAlpha_bitslice MQOM_NAMESPACE(RecomputePAlpha_bitslice)

#if defined(PIOP_BITSLICE)
#include "piop_bitslice.h"
#define ComputePAlpha ComputePAlpha_bitslice
#define RecomputePAlpha RecomputePAlpha_bitslice
#elif defined(MEMORY_EFFICIENT_PIOP)
#include "piop_memopt.h"
#define ComputePAlpha ComputePAlpha_memopt
#define RecomputePAlpha RecomputePAlpha_memopt
#else
#include "piop_default.h"
#define ComputePAlpha ComputePAlpha_default
#define RecomputePAlpha RecomputePAlpha_default
#endif

#endif /* __PIOP_H__ */
