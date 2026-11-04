// SPDX-License-Identifier: Apache-2.0

#ifndef PRNG_H
#define PRNG_H

#include <stddef.h>
#include <stdint.h>
#include <fips202.h>
#include <tutil.h>
#include <sqisign_namespace.h>

/**
 * Domain separators for the SHAKE-256-based PRNG.
 *
 * A separator is a 3-character ASCII string (size is checked and enforced during seeding), stored as a 4-byte array so
 * that it is a valid C string literal; only the 3 characters are absorbed during seed expansion
 * (ctx_d = SHAKE256(seed || d)). Changing the text of an existing separator will break KATs.
 *
 * DS_DEFAULT_DOMAIN is the stream used by call sites which have not (yet) been assigned a domain of their own.
 */
#define PRNG_DOMAIN_SEP_BYTES 3

#define DS_DEFAULT_DOMAIN "def"
#define DS_RESPONSE_DOMAIN "res"
#define DS_COMMITMENT_DOMAIN "com"
#define DS_KEYGEN_DOMAIN "key"

/**
 * Size of the root seed every domain stream is derived from: ctx_d = SHAKE256(seed || d). Changing it will break KATs.
 */
#define PRNG_SEED_BYTES 48

/**
 * One independent SHAKE-256 stream, derived from the calling thread's root seed by prng_domain_seed().
 *
 * Storage belongs to the caller, so a domain may be a long-lived global (see prng_default_domain) or a local whose
 * lifetime is a single function. If global, remember to declare it with SQISIGN_THREAD_LOCAL, as done for
 * prng_default_domain.
 *
 * Treat it as an opaque struct; it is only declared here in the .h so that callers can allocate one on the stack.
 */
typedef struct prng_domain_ctx
{
    shake256incctx ctx;
    uint32_t seeded;
} prng_domain_ctx_t;

/**
 * The DS_DEFAULT_DOMAIN stream, seeded by prng_seed() and zeroized by prng_clear().
 *
 * Thread-local unless SQISIGN_SINGLE_THREADED is defined, so each thread derives its own stream and no two concurrent
 * operations share this state.
 *
 * Thread-safety is contingent on randombytes() also being thread-safe, which should be a safe assumption now: system
 * randomness should naturally have no issues, and the AES-CTR-DRBG implementation for deterministic randomness, used in
 * KAT/test/benchmarks binaries, was also made thread-safe.
 */
extern SQISIGN_THREAD_LOCAL prng_domain_ctx_t PRNG_default_domain;

/**
 * (Re)seed the calling thread's PRNG using randombytes(), then seed PRNG_default_domain from it. May be called at any
 * time to reseed.
 *
 * The root seed is retained until prng_clear(), so that further domains can be derived from it at any point during the
 * operation without consuming more entropy.
 *
 * @return int 0 on success, -1 on failure (state left unseeded)
 */
int prng_seed(void);

/**
 * Zeroize the calling thread's root seed and PRNG_default_domain to avoid leakage of secret data.
 *
 * Note: domain contexts owned by the caller are out of reach here and must be zeroized with prng_domain_clear().
 */
void prng_clear(void);

/**
 * Seed ctx with the stream belonging to the given domain separator, derived from the calling thread's root seed.
 *
 * Deriving two contexts for the same domain from the same root yields the same stream, by design.
 *
 * @param[out] ctx Domain context to seed.
 * @param[in] domain 3-character, NUL-terminated domain separator selecting the stream to derive.
 * @return int 0 on success, -1 on a malformed separator or unseeded root.
 */
int prng_domain_seed(prng_domain_ctx_t *ctx, const char domain[PRNG_DOMAIN_SEP_BYTES + 1]);

/**
 * Zeroize a domain context.
 *
 * @param[out] ctx Domain context to zeroize.
 */
void prng_domain_clear(prng_domain_ctx_t *ctx);

/**
 * Generate len pseudorandom bytes from the given domain's stream.
 *
 * Streaming semantics: two consecutive calls of n bytes each return the same bytes as one call of 2n bytes. The
 * context must have been seeded by a successful prng_domain_seed() (or, for PNRG_default_domain, prng_seed());
 * calling unseeded is a usage error that asserts in debug builds and returns -1 in release builds. On failure
 * nothing is written to out. This function is not async-signal-safe.
 *
 * @param[in,out] ctx Domain context to squeeze from.
 * @param[out] out Memory to hold the pseudorandom bytes.
 * @param[in] len Number of pseudorandom bytes to be generated.
 * @return int 0 on success, -1 on unseeded context
 */
int prng_random_bytes(prng_domain_ctx_t *ctx, unsigned char *out, size_t len);

#endif /* PRNG_H */
