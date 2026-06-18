#ifndef __PIOP_CACHE_H__
#define __PIOP_CACHE_H__

#include <common.h>
#include <stdlib.h>
#include <fields.h>

/**** PIOP cache handling functions *********/
/*
 * This is a cache to factorize t1 computation across all the tau
 * repetitions.
 */
typedef struct {
	uint8_t active;
	field_ext_elt t1[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	/* Size in bytes of the cache (tracked for free) */
	uint32_t size;
} piop_cache;

static inline void get_entry_piop_cache(const piop_cache *cache, uint32_t i, field_ext_elt t1[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)]) {
	memcpy(t1, cache[i].t1, FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
}
static inline void set_entry_piop_cache(piop_cache *cache, uint32_t i, const field_ext_elt t1[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)]) {
	if (cache != NULL) {
		memcpy(cache[i].t1, t1, FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
		cache[i].active = 1;
	}
}

/* Function that deals with the PIOP cache */
static inline void destroy_piop_cache(piop_cache *cache) {
	if (cache != NULL) {
		mqom_free(cache, cache->size);
	}
}

static inline uint8_t is_entry_active_piop_cache(const piop_cache *cache, uint32_t i) {
	return (cache != NULL) ? cache[i].active : 0;
}

static inline piop_cache *init_piop_cache(uint32_t num_elt) {
	(void)num_elt;
	piop_cache *cache = NULL;

#ifdef USE_PIOP_CACHE
	cache = (piop_cache*)mqom_calloc(num_elt, sizeof(piop_cache));
	if (cache == NULL) {
		goto err;
	}
	cache->size = num_elt * sizeof(piop_cache);
err:
#endif
	/* NOTE: when USE_PIOP_CACHE is not defined,
	 * cache is NULL which is equivalent to not using a cache
	 * */
	return cache;
}
#endif
