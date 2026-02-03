#ifndef __SEED_COMMIT_H__
#define __SEED_COMMIT_H__

#include "enc.h"
#include "common.h"
#include "prg.h"

/* SeedCommit variants:
 * By default, we factorize the key schedule, the tweaked salt is inside the encryption context.
 * If asked to with SEED_COMMIT_MEMOPT is defined, we do not perform such factorization
 * to gain some memory but loose cycles since we have to perform the key schedule each time.
 * This allows for time-memory tradeoffs.
 */
#if defined(SEED_COMMIT_MEMOPT)
#include "seed_commit_memopt.h"
#else
#include "seed_commit_default.h"
#endif

#endif /* __SEED_COMMIT_H__ */
