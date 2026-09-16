#ifndef __VERIFY_STREAM_H__
#define __VERIFY_STREAM_H__

#include "common.h"
#include "piop_memopt.h"

/* StreamedVerify calls the memopt BLC/PIOP primitives (BLC_Eval_*_memopt_partial,
 * OT_BLC_Eval_memopt_partial, RecomputePAlpha_partial_memopt) by their
 * explicit names rather than through blc.h/piop.h's generic dispatch, so it
 * needs neither MEMORY_EFFICIENT_BLC nor MEMORY_EFFICIENT_PIOP to be set:
 * those symbols are unconditionally compiled in (see BLC_SRC_FILES and
 * PIOP_SRC_FILES in the Makefile), regardless of what Verify()/Sign() itself
 * is currently wired to. */

/* STREAM_VERIFY_BATCH: number of executions accumulated between two
 * RecomputePAlpha_partial_memopt calls while streaming a signature body.
 * 1 = minimal per-call memory (one execution's worth of BLC/PIOP scratch);
 * its max allowed value, PIOP_NB_PARALLEL_REPETITIONS_VERIFY, gives the
 * fewest PIOP calls. Both are compile-time choices, independent of how the
 * caller actually chunks bytes into Update(). */
#ifndef STREAM_VERIFY_BATCH
#define STREAM_VERIFY_BATCH PIOP_NB_PARALLEL_REPETITIONS_VERIFY
#endif
#if (STREAM_VERIFY_BATCH <= 0) || (STREAM_VERIFY_BATCH > PIOP_NB_PARALLEL_REPETITIONS_VERIFY)
#error "STREAM_VERIFY_BATCH must be > 0 and <= PIOP_NB_PARALLEL_REPETITIONS_VERIFY."
#endif

/* Streaming-verify state machine. A context starts ACTIVE; any internal
 * failure (bad chunk, out-of-range execution index, ...) or a completed
 * Finalize() latches it into FAILED/DONE. Update()/Finalize() reject the call
 * outright once the context has left ACTIVE, so a caller that ignores an
 * error return (or calls Finalize() twice) cannot resume processing with
 * stale/partial batch state - which would otherwise let extra,
 * attacker-controlled chunks index accumulator arrays out of bounds. */
#define STREAM_VERIFY_STATE_ACTIVE 0
#define STREAM_VERIFY_STATE_FAILED 1
#define STREAM_VERIFY_STATE_DONE   2

/* Ownership contract. Init() returns a heap-allocated context (OT also allocates a
 * separate prefix buffer, released early by Update() once the prefix is consumed).
 * StreamedVerify_Clean() is the ONLY function that releases any of it: neither
 * Update() nor Finalize() ever cleans up, not even on the paths that latch the
 * context into FAILED. So the caller owns the context from a successful Init()
 * until its own call to Clean(), and that includes every abandonment path - an
 * Update() that returned non-zero, a Finalize() that rejected the signature, or a
 * caller that simply gives up mid-stream. Skipping Clean() leaks the context and
 * leaves the XOF states and accumulators it holds in memory.
 * Clean(NULL) is a no-op; Clean() on an already-cleaned pointer is a double free. */

#if MQOM3_PARAM_OT_VARIANT != 1

/* === Correlated-Tree (CT) variant === */
#include "verify_stream_ct.h"
#define stream_verify_ctx_t CT_stream_verify_ctx_t
#define StreamedVerify_Init CT_StreamedVerify_Init
#define StreamedVerify_Update CT_StreamedVerify_Update
#define StreamedVerify_Finalize CT_StreamedVerify_Finalize
#define StreamedVerify_Clean CT_StreamedVerify_Clean

/* Nominal (optimal) Update() chunking: feeding exactly these sizes, in this
 * order (header, then MQOM3_PARAM_TAU execution chunks), avoids all internal
 * byte-level reassembly (ctx->unprocessed[]/prefix_buf copies). CT has no
 * monolithic prefix phase, hence PREFIX_SIZE == 0. */
#define STREAM_VERIFY_HEADER_SIZE STREAM_VERIFY_CT_HEADER_SIZE
#define STREAM_VERIFY_PREFIX_SIZE 0
#define STREAM_VERIFY_EXEC_CHUNK_SIZE CT_CHUNK_SIZE

#else

/* === One-Tree (OT) variant === */
#include "verify_stream_ot.h"
#define stream_verify_ctx_t OT_stream_verify_ctx_t
#define StreamedVerify_Init OT_StreamedVerify_Init
#define StreamedVerify_Update OT_StreamedVerify_Update
#define StreamedVerify_Finalize OT_StreamedVerify_Finalize
#define StreamedVerify_Clean OT_StreamedVerify_Clean

/* Nominal (optimal) Update() chunking: header, then one monolithic prefix
 * chunk (OT's shared large tree can only be reconstructed once the full
 * T_open path and full hidden-index set are in, see verify_stream_ot.h),
 * then MQOM3_PARAM_TAU execution chunks. */
#define STREAM_VERIFY_HEADER_SIZE STREAM_VERIFY_OT_HEADER_SIZE
#define STREAM_VERIFY_PREFIX_SIZE STREAM_VERIFY_OT_PREFIX_SIZE
#define STREAM_VERIFY_EXEC_CHUNK_SIZE STREAM_VERIFY_OT_ALPHA1_CHUNK_SIZE

#endif /* MQOM3_PARAM_OT_VARIANT */

#endif /* __VERIFY_STREAM_H__ */
