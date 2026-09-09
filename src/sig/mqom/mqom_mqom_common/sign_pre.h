#ifndef __SIGN_PRE_H__
#define __SIGN_PRE_H__

#include "common.h"

/* PreSign deliberately does NOT use blc.h's generic blc_key_t/BLC_Commit/
 * BLC_Open, which resolve to whichever internal key representation the
 * ambient build picked for Sign()/Verify() (the "default", BLC_KEEP_ALL_
 * TREES_IN_MEMORY one can be several MB, caching the fully-expanded GGM
 * tree purely as a RAM/CPU tradeoff for a single synchronous call).
 * A persisted presig has no business inheriting that internal cache: it
 * always uses the *_memopt BLC functions below and their blc_key_*_memopt_t
 * key type instead, which is a small, fixed-size seed representation
 * (a handful of TAU-sized seed arrays) with NO #ifdef on its layout at all -
 * unconditionally compiled in every build (see BLC_SRC_FILES in the
 * Makefile), so it costs nothing to always pull in regardless of
 * MEMORY_EFFICIENT_BLC. This makes MQOM3_PRESIG_SIZE a true per-variant
 * constant, independent of the ambient build's memory/CPU tradeoff, and
 * makes a presig produced by one build mode byte-compatible with
 * Sign_Finalize() in another build mode of the same variant. BLC_Commit_*_
 * memopt/BLC_Open_*_memopt still produce byte-identical opening/com1 output
 * to the _default functions for the same (sk, salt, mseed) - that
 * equivalence is what lets Sign()/Verify_default()/Verify_memopt already
 * interoperate today, and PreSign here relies on the exact same property. */
#if MQOM3_PARAM_OT_VARIANT != 1
#include "blc_ct_memopt.h"
/* For CT_CHUNK_PDELTA_SIZE (the per-execution Delta_x^(1) size) and
 * DeriveDelta(), both used by the ckey serialization in sign_pre.c. */
#include "blc_ct_common.h"
#define blc_key_compact_t CT_blc_key_memopt_t
#define BLC_Commit_compact CT_BLC_Commit_memopt
#define BLC_Open_compact CT_BLC_Open_memopt
/* Compressed opening key ("ckey"), the form a pre-signature actually stores:
 *   mseed | Delta_x^(1)
 * and nothing else. blc_key_compact_t keeps all of its fields where they are
 * in memory - they are simply redundant *on the wire*:
 *
 *  - salt is already carried by the pre-signature itself, right before the
 *    ckey (masked on the wire like everything past rnd, but present in the
 *    payload all the same), so storing it twice buys nothing;
 *  - delta is DeriveDelta(x), i.e. the head of the serialized witness, which
 *    sk holds verbatim. CT_BLC_Open_memopt() does need it (it is the
 *    correlated-tree offset SmallGGMTree_ExpandPath() XORs in at level 0),
 *    but Sign_Finalize() recomputes it from sk for a handful of cycles
 *    instead of persisting it.
 *
 * Delta_x^(1) is the opposite case and is therefore kept in full: it is not
 * recomputable without redoing the whole BLC commit, which is exactly the
 * work pre-signing exists to hoist out of the message-dependent phase.
 *
 * The size is spelled out rather than taken from sizeof(blc_key_compact_t):
 * this is a wire format and must not follow the struct's in-memory padding. */
#define MQOM3_CKEY_SIZE (MQOM3_PARAM_SEED_SIZE + (MQOM3_PARAM_TAU * CT_CHUNK_PDELTA_SIZE))
#else
#include "blc_ot_memopt.h"
#define blc_key_compact_t OT_blc_key_memopt_t
#define BLC_Commit_compact OT_BLC_Commit_memopt
#define BLC_Open_compact OT_BLC_Open_memopt
/* See the CT-side comment above. The one-tree opening key carries no delta at
 * all (a single large tree has no correlated-tree offset to propagate), so
 * here the ckey drops only the redundant salt, and keeps Delta_x in full. */
#define MQOM3_CKEY_SIZE (MQOM3_PARAM_SEED_SIZE + (MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)))
#endif

/* Deal with namespacing */
#define Sign_Prepare MQOM_NAMESPACE(Sign_Prepare)
#define Sign_Finalize MQOM_NAMESPACE(Sign_Finalize)
#define crypto_sign_prepare MQOM_PUBLIC_API_NAMESPACE(crypto_sign_prepare)
#define crypto_sign_finalize MQOM_PUBLIC_API_NAMESPACE(crypto_sign_finalize)

/* Masking randomness (rnd), kept in the clear at the head of the pre-signature;
 * fixed 256-bit size regardless of the security category (spec: rnd <- {0,1}^256). */
#define MQOM3_PARAM_PRESIGN_RND_SIZE 32

/* Pre-signature payload layout (everything but the leading rnd is masked):
 *   rnd | presig_id[DIGEST] | salt | alpha1[TAU][ETA*MU] | ckey[MQOM3_CKEY_SIZE]
 *
 * presig_id = Hash_2(pk, com1[0..TAU-1], com2) is the message-independent
 * part of the Fiat-Shamir transcript; Sign_Finalize completes it with
 * sig_id = Hash_4(presig_id, msg_hash). Sign()/Verify_default()/Verify_memopt
 * compute the exact same two-stage Hash_2/Hash_4 chain (sign.c, sign_memopt.c),
 * so a signature produced through PreSign+FinalizeSign is byte-identical to
 * one produced by a direct Sign() call with the same (salt, mseed, msg).
 * The masking XOF absorbs (sk, rnd) directly, with no domain-separation tag
 * (spec: mask = XOF(sk || rnd, len), as opposed to the tagged Hash_i/XOF_i
 * family used everywhere else). */
#define MQOM3_PRESIG_DATA_SIZE (                                                  \
    (MQOM3_PARAM_SALT_SIZE)                                                       \
      + (MQOM3_PARAM_DIGEST_SIZE)                                                 \
      + MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA*MQOM3_PARAM_MU)    \
      + (uint32_t)(MQOM3_CKEY_SIZE)                                              \
)
#define MQOM3_PRESIG_SIZE ((MQOM3_PARAM_PRESIGN_RND_SIZE) + MQOM3_PRESIG_DATA_SIZE)

/* SECURITY - single use only, on both counts below:
 *
 *  1. (salt, mseed, mask_rnd) MUST be fresh, uniformly random, and never
 *     reused across two Sign_Prepare() calls for the same sk. The BLC/PIOP
 *     commit phase (com1/com2/presig_id) depends only on (sk, salt, mseed),
 *     not on the message, so reusing them is fatal (same as reusing an
 *     ECDSA nonce k)
 *  2. A given presig MUST be passed to Sign_Finalize() for AT MOST ONE
 *     message, for the identical reason: two Sign_Finalize() calls on the
 *     same presig with different messages reuse the same commit under two
 *     different challenges. This library does not enforce single-use on the
 *     presig buffer itself (it is caller-owned and left untouched by
 *     Sign_Finalize() on either success or failure) - callers that persist
 *     or transmit presigs are responsible for consuming/discarding each one
 *     after its one Sign_Finalize() call, including under concurrent access.
 *
 * The XOF masking of the presig payload (everything but the leading rnd)
 * only protects confidentiality against a passive observer; it is not an
 * integrity/authentication tag, so a modified presig is only ever caught
 * indirectly, if at all, by the resulting signature failing to verify. */
int Sign_Prepare(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t mask_rnd[MQOM3_PARAM_PRESIGN_RND_SIZE], uint8_t presig[MQOM3_PRESIG_SIZE]);

int Sign_Finalize(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t presig[MQOM3_PRESIG_SIZE], uint8_t sig[MQOM3_SIG_SIZE]);

/* High-level, randomized wrappers (crypto_sign.c), matching how
 * crypto_sign_signature() wraps the equally low-level, caller-supplied-
 * randomness Sign(): (salt, mseed, mask_rnd) are generated internally with
 * randombytes(), so point 1 above is satisfied by construction - there is no
 * caller-supplied randomness left to reuse. This is the intended public
 * entry point for PreSign; Sign_Prepare/Sign_Finalize above remain available
 * as the deterministic, spec-pseudocode-matching primitives (KAT generation,
 * testing, or callers who genuinely need to supply their own randomness).
 *
 * Point 2 (presig single-use) is only partly addressed: crypto_sign_finalize()
 * wipes its presig argument in place after use, on both the success and the
 * failure path, so calling it again with that same buffer processes
 * zeroed-out data instead of silently reopening the commitment. This is
 * defense-in-depth against the common accidental bug (forgetting a presig
 * was already consumed); it does NOT make presig single-use in general - a
 * caller who copied presig before this call (persisted it, sent it
 * elsewhere, kept a second reference, ...) can still finalize that copy
 * against a different message and reopen the same commitment. That
 * operational discipline is still entirely on the caller, exactly as in
 * point 2 above. */
int crypto_sign_prepare(uint8_t presig[MQOM3_PRESIG_SIZE], const uint8_t sk[MQOM3_SK_SIZE]);

int crypto_sign_finalize(uint8_t sig[MQOM3_SIG_SIZE], unsigned long long *siglen, const uint8_t *msg, unsigned long long mlen, const uint8_t sk[MQOM3_SK_SIZE], uint8_t presig[MQOM3_PRESIG_SIZE]);

#endif /* __SIGN_PRE_H__ */
