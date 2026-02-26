#ifndef __PRG_CACHE_H__
#define __PRG_CACHE_H__

#include <common.h>
#include <stdlib.h>
#include "enc.h"

/**** PRG cache handling functions *********/
#ifndef CEIL
#define CEIL(x,y) (((x) + (y) - 1) / (y))
#endif

#ifndef PRG_CACHE_SUFFIX
#error "You have included prg_cache.h without defining PRG_CACHE_SUFFIX ... This header is supposed to use a dedicated suffix"
#endif

#ifndef concat2
#define _concat2(a, b) a ## b
#define concat2(a, b) _concat2(a, b)
#endif

/* This is a key schedule cache to factorize common key schedules across the repetitions
 * of 'e' and 'i' in the various calls to PRG.
 * NOTE: we can bound the size of the cache depending on the MQOM2 parameters.
 * */
typedef struct {
	uint8_t active;
	PRG_ENC_CTX_TYPE ctx;
	/* Size in bytes of the cache (tracked for free) */
	uint32_t size;
} concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX);

/* Function that deals with the PRG cache */
static inline void concat2(destroy_prg_cache, PRG_CACHE_SUFFIX)(concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *cache) {
	if (cache != NULL) {
		uint32_t i;
		/* Clean the enc contexts in all the active cells */
		for (i = 0; i < (cache->size / sizeof(concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX))); i++) {
			if (cache[i].active) {
				concat2(enc_clean_ctx, PRG_CACHE_SUFFIX)(&cache[i].ctx);
			}
		}
		mqom_free(cache, cache->size);
	}
}

static inline uint8_t concat2(is_entry_active_prg_cache, PRG_CACHE_SUFFIX)(const concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *cache, uint32_t i) {
	return (cache != NULL) ? cache[i].active : 0;
}

static inline void concat2(get_entry_prg_cache, PRG_CACHE_SUFFIX)(const concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *cache, uint32_t i, PRG_ENC_CTX_TYPE *ctx) {
	(*ctx) = cache[i].ctx;
}

static inline void concat2(invalidate_entry_prg_cache, PRG_CACHE_SUFFIX)(concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *cache, uint32_t i) {
	if ((cache != NULL) && cache[i].active) {
		concat2(enc_clean_ctx, PRG_CACHE_SUFFIX)(&cache[i].ctx);
		cache[i].active = 0;
	}
}

static inline void concat2(set_entry_prg_cache, PRG_CACHE_SUFFIX)(concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *cache, uint32_t i, const PRG_ENC_CTX_TYPE *ctx) {
	if (cache != NULL) {
		/* If the cell was already active from a previous run, clean the enc context */
		concat2(invalidate_entry_prg_cache, PRG_CACHE_SUFFIX)(cache, i);
		cache[i].ctx = (*ctx);
		cache[i].active = 1;
	}
}

static inline concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *concat2(init_prg_cache, PRG_CACHE_SUFFIX)(uint32_t n_bytes) {
	(void)n_bytes;
	concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *prg_cache = NULL;

#ifdef USE_PRG_CACHE
	prg_cache = (concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX)*)mqom_calloc(CEIL(n_bytes, MQOM2_PARAM_SEED_SIZE), sizeof(concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX)));
	if (prg_cache == NULL) {
		goto err;
	}
	prg_cache->size = CEIL(n_bytes, MQOM2_PARAM_SEED_SIZE) * sizeof(concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX));
err:
#endif
	/* NOTE: when USE_PRG_CACHE is not defined,
	 * prg_cache is NULL which is equivalent to not using a cache
	 * */
	return prg_cache;
}

static inline int concat2(prg_key_sched, PRG_CACHE_SUFFIX)(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, uint32_t i, PRG_ENC_CTX_TYPE *ctx, concat2(prg_key_sched_cache, PRG_CACHE_SUFFIX) *cache) {
	int ret = -1;
	uint8_t tweaked_salt[PRG_CACHE_NB_PARALLELS][MQOM2_PARAM_SALT_SIZE];

	if (concat2(is_entry_active_prg_cache, PRG_CACHE_SUFFIX)(cache, i)) {
		/* The cache line is active, get the value and return */
		concat2(get_entry_prg_cache, PRG_CACHE_SUFFIX)(cache, i, ctx);
	} else {
		/* The cache line is inactive: perform the computation and fill it */
		if (cache != NULL) {
			/* First, uninit the enc context (only when we have a cache).
			 * This allows to avoid spurious deallocations when calling
			 * enc_key_sched below.
			 */
			concat2(enc_uninit_ctx, PRG_CACHE_SUFFIX)(ctx);
		}
		/* Tweak the salt and perform the key schedule */
		for(uint32_t j=0; j<PRG_CACHE_NB_PARALLELS; j++) {
			TweakSalt(salt, tweaked_salt[j], 3, e[j], i);
		}
		/* XXX: NOTE: we invalidate the entry *before* setting it to trigger
		 * potential cleansing of the enc context by the lower layers (e.g. pointers
		 * maintenance, etc.)
		 */
		concat2(invalidate_entry_prg_cache, PRG_CACHE_SUFFIX)(cache, i);
#if PRG_CACHE_NB_PARALLELS == 1
		ret = concat2(enc_key_sched, PRG_CACHE_SUFFIX)(ctx, tweaked_salt[0]);
#elif PRG_CACHE_NB_PARALLELS == 2
		ret = concat2(enc_key_sched, PRG_CACHE_SUFFIX)(ctx, tweaked_salt[0], tweaked_salt[1]);
#elif PRG_CACHE_NB_PARALLELS == 4
		ret = concat2(enc_key_sched, PRG_CACHE_SUFFIX)(ctx, tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3]);
#elif PRG_CACHE_NB_PARALLELS == 8
		ret = concat2(enc_key_sched, PRG_CACHE_SUFFIX)(ctx, tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3], tweaked_salt[4], tweaked_salt[5], tweaked_salt[6], tweaked_salt[7]);
#else
	#error "Invalid PRG_CACHE_NB_PARALLELS: should be 1, 2, 4, or 8"
#endif
		ERR(ret, err);
		concat2(set_entry_prg_cache, PRG_CACHE_SUFFIX)(cache, i, ctx);
	}

	ret = 0;
err:
	return ret;
}

#endif
