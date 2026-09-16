#ifndef __SEED_COMMIT_MEMOPT_H__
#define __SEED_COMMIT_MEMOPT_H__

#include "enc.h"
#include "common.h"
#include "prg.h"


/* XXX: NOTE: in this SeedCommit variant, we *DO NOT* factorize the key schedules to save memory
 * in the context at the expanse of additional cycles  */

typedef struct {
	uint8_t tweaked_salt[2][MQOM3_PARAM_SALT_SIZE];
} seedcommit_ctx_t;

/* pub/non-pub share the same context struct in memopt (no cached key schedule) */
#define seedcommit_ctx_pub_t seedcommit_ctx_t

static inline int init_seedcommit(seedcommit_ctx_t *ctx, const uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], const uint8_t tweaked_salt_1[MQOM3_PARAM_SALT_SIZE]) {
	memcpy(ctx->tweaked_salt[0], tweaked_salt_0, MQOM3_PARAM_SALT_SIZE);
	memcpy(ctx->tweaked_salt[1], tweaked_salt_1, MQOM3_PARAM_SALT_SIZE);

	return 0;
}
static inline void seedcommit_clean_ctx(seedcommit_ctx_t *ctx) {
	(void)ctx;
}

/* init_seedcommit_pub / seedcommit_clean_ctx_pub alias the non-pub versions */
#define init_seedcommit_pub      init_seedcommit
#define seedcommit_clean_ctx_pub seedcommit_clean_ctx

/* Pass 1: non-pub variants (SeedCommit, SeedCommit_x2, SeedCommit_x4_x4) */
#include "seed_commit_memopt_impl.h"

/* Pass 2: pub variants (SeedCommit_pub, SeedCommit_pub_x2, SeedCommit_x4_x4_pub) */
#define SEED_COMMIT_PUBLIC
#include "seed_commit_memopt_impl.h"
#undef SEED_COMMIT_PUBLIC

#endif /* __SEED_COMMIT_MEMOPT_H__ */
