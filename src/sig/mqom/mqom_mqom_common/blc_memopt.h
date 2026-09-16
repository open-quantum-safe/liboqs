#ifndef __BLC_MEMOPT_H__
#define __BLC_MEMOPT_H__

/* Slow folding accumulates each leaf's contribution into x0/u0 directly (see
 * blc_memopt_folding.h). Batching several leaves' worth of SeedExpand output
 * together (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1) is supported there via a
 * per-leaf-in-batch snapshot (leaf_snapshot[]) that isolates each leaf's own
 * contribution before it is scaled and folded in - so, unlike before, N > 1
 * is no longer rejected here. Default (when unset) stays 1, matching the
 * original minimal-footprint behavior exactly - this is purely additive, not
 * a default change. */
#ifdef BLC_NO_FAST_FOLDING
#ifndef BLC_NB_LEAF_SEEDS_IN_PARALLEL
#define BLC_NB_LEAF_SEEDS_IN_PARALLEL (1)
#endif
#else
#ifndef BLC_NB_LEAF_SEEDS_IN_PARALLEL
#define BLC_NB_LEAF_SEEDS_IN_PARALLEL (8)
#endif
#endif

#if MQOM3_PARAM_NB_EVALS % BLC_NB_LEAF_SEEDS_IN_PARALLEL != 0
#error BLC_NB_LEAF_SEEDS_IN_PARALLEL should divide MQOM3_PARAM_NB_EVALS.
#endif

#endif /* __BLC_MEMOPT_H__ */
