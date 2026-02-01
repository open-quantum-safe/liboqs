#ifndef __BLC_COMMON_H__
#define __BLC_COMMON_H__

#include <string.h>
#include "prg.h"
#include "fields.h"

#define PRG_BLC_SIZE (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA) - MQOM2_PARAM_SEED_SIZE)

static inline void DeriveDelta(const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], uint8_t delta[MQOM2_PARAM_SEED_SIZE]) {
	uint8_t _x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	field_base_serialize(x, MQOM2_PARAM_MQ_N, _x);
	memcpy(delta, _x, MQOM2_PARAM_SEED_SIZE);
}

static inline int DeriveRootSeeds(const uint8_t mseed[MQOM2_PARAM_SEED_SIZE], uint8_t rseed[MQOM2_PARAM_TAU][MQOM2_PARAM_SEED_SIZE]) {
	int ret = -1;

	uint8_t tree_prg_salt[MQOM2_PARAM_SALT_SIZE] = { 0 };
	ret = PRG(tree_prg_salt, 0, mseed, MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE, (uint8_t*)rseed, NULL);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

#endif /* __BLC_COMMON_H__ */
