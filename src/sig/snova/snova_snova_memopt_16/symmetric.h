// SPDX-License-Identifier: MIT

/**
 * Glue code between SNOVA and liboqs for the symmetric primitives used by SNOVA
 *
 * Copyright (c) 2026 SNOVA TEAM
 */

#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <oqs/sha3.h>

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	uint64_t state[28];
} shake_t;

static inline void shake256_init(shake_t *instance) {
	OQS_SHA3_shake256_inc_ctx *oqs_instance = (OQS_SHA3_shake256_inc_ctx *)instance;
	OQS_SHA3_shake256_inc_init(oqs_instance);
}

static inline void shake_absorb(shake_t *instance, const uint8_t *in, size_t inlen) {
	OQS_SHA3_shake256_inc_ctx *oqs_instance = (OQS_SHA3_shake256_inc_ctx *)instance;
	OQS_SHA3_shake256_inc_absorb(oqs_instance, in, inlen);
}

static inline void shake_finalize(shake_t *instance) {
	OQS_SHA3_shake256_inc_ctx *oqs_instance = (OQS_SHA3_shake256_inc_ctx *)instance;
	OQS_SHA3_shake256_inc_finalize(oqs_instance);
}

static inline void shake_squeeze(uint8_t *out, size_t outlen, shake_t *instance) {
	OQS_SHA3_shake256_inc_ctx *oqs_instance = (OQS_SHA3_shake256_inc_ctx *)instance;
	OQS_SHA3_shake256_inc_squeeze(out, outlen, oqs_instance);
	OQS_SHA3_shake256_inc_ctx_release(oqs_instance);
}

static inline void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen) {
	shake_t instance;
	shake256_init(&instance);
	shake_absorb(&instance, in, inlen);
	shake_finalize(&instance);
	shake_squeeze(out, outlen, &instance);
}

/**
 * SNOVA public XOF
 */

#ifndef SKIP_PK_EXPAND

#ifdef AESCTR

#include <aes.h>

#if SNOVA_OPT != 5

static void snova_pk_expand(uint8_t *data, size_t num_bytes, const uint8_t *input, size_t inlen) {
	const uint8_t iv[16] = {0};
	void *state;

	assert(inlen == 16);
	(void)inlen;

	OQS_AES128_CTR_inc_init(input, &state);
	OQS_AES128_CTR_inc_stream_iv(iv, 12, state, data, num_bytes);
	OQS_AES128_free_schedule(state);
}

#else

#define NUM_BYTES 128

// Incremental API
typedef struct {
	uint8_t key[16];
	uint8_t states[2 * NUM_BYTES];
	uint64_t block_i;

	uint64_t index;
	uint64_t last_idx;
	int64_t bytes_left;
} snova_pk_expander_t;

static void snova_pk_expander_init(snova_pk_expander_t *arg, const uint8_t *seed, size_t input_bytes) {
	snova_pk_expander_t *instance = (snova_pk_expander_t *)arg;
	assert(input_bytes == SEED_LENGTH_PUBLIC);
	(void)input_bytes;

	memset(instance, 0, sizeof(snova_pk_expander_t));
	memcpy(instance->key, seed, 16);
}

static void snova_aes_expand_block(snova_pk_expander_t *instance) {
	uint8_t in[NUM_BYTES] = {0};
	uint8_t out[NUM_BYTES] = {0};

	for (int i = 0; i < NUM_BYTES / 16; i++) {
		for (int j = 0; j < 4; j++) {
			in[i * 16 + 15 - j] = (instance->block_i >> (8 * j)) & 0xff;
		}
		instance->block_i++;
	}

	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(instance->key, &schedule);
	OQS_AES128_ECB_enc_sch(in, NUM_BYTES, schedule, out);
	OQS_AES128_free_schedule(schedule);

	// Convert to GF16
	for (int32_t idx = NUM_BYTES - 1; idx >= 0; idx--) {
		instance->states[2 * idx + 1] = (out[idx] >> 4) & 0xf;
		instance->states[2 * idx] = out[idx] & 0xf;
	}
}

static void snova_pk_expander_squeeze(uint8_t *data, size_t num_gf, snova_pk_expander_t *arg) {
	snova_pk_expander_t *instance = (snova_pk_expander_t *)arg;
	uint8_t *data8 = data;
	instance->last_idx += num_gf;

	if (instance->bytes_left > 0) {
		uint8_t *state8 = (uint8_t *)instance->states + 2 * NUM_BYTES - instance->bytes_left;

		if (instance->bytes_left >= (int64_t)num_gf) {
			memcpy(data, state8, num_gf);
			instance->bytes_left -= num_gf;
			instance->index += num_gf;
			return;
		}

		memcpy(data8, state8, instance->bytes_left);
		instance->index += instance->bytes_left;
		data8 += instance->bytes_left;
	}

	while (instance->index < instance->last_idx) {
		snova_aes_expand_block(instance);

		size_t bytes = instance->last_idx - instance->index;
		if (bytes > 2 * NUM_BYTES) {
			bytes = 2 * NUM_BYTES;
		} else {
			instance->bytes_left = 2 * NUM_BYTES - bytes;
		}

		memcpy(data8, instance->states, bytes);
		instance->index += bytes;
		data8 += bytes;
	}
}

static void snova_pk_expander_goto(snova_pk_expander_t *instance, size_t index) {
	instance->block_i = index / 32;
	snova_aes_expand_block(instance);

	instance->last_idx = index;
	instance->bytes_left = 2 * NUM_BYTES - (index % 32);
	instance->index = index;
}

static void snova_pk_expander_free(snova_pk_expander_t *arg) {
	(void)arg;
}

#endif

#else

#if SNOVA_OPT != 5

#if defined(OQS_ENABLE_SHA3_xkcp_low_avx2)

/**
 * liboqs x4 version
 */
#include <oqs/sha3x4.h>

static void snova_pk_expand(uint8_t *data, size_t num_bytes, const uint8_t *pt_seed_array, size_t inlen) {
	assert(inlen == SEED_LENGTH_PUBLIC);
	(void)inlen;

	size_t index = 0;
	uint64_t block = 0;

	while (index < num_bytes) {
		OQS_SHA3_shake128_x4_inc_ctx hashInstance;
		OQS_SHA3_shake128_x4_inc_init(&hashInstance);
		OQS_SHA3_shake128_x4_inc_absorb(&hashInstance, pt_seed_array, pt_seed_array, pt_seed_array, pt_seed_array, SEED_LENGTH_PUBLIC);

		// Turn SHAKE128 into SHAKE128 CTR-XOF
		// Little endian
		uint64_t block_0 = block;
		block++;
		uint64_t block_1 = block;
		block++;
		uint64_t block_2 = block;
		block++;
		uint64_t block_3 = block;
		block++;
		OQS_SHA3_shake128_x4_inc_absorb(&hashInstance, (uint8_t *)&block_0, (uint8_t *)&block_1, (uint8_t *)&block_2,
		                                (uint8_t *)&block_3, 8);

		OQS_SHA3_shake128_x4_inc_finalize(&hashInstance);
		size_t bytes = num_bytes - index;
		if (bytes > 4 * 168) {
			OQS_SHA3_shake128_x4_inc_squeeze(data, data + 168, data + 336, data + 504, 168, &hashInstance);
			data += 4 * 168;
		} else {
			// Last round
			alignas(32) uint8_t buf[4 * 168];
			OQS_SHA3_shake128_x4_inc_squeeze(buf, buf + 168, buf + 336, buf + 504, 168, &hashInstance);
			memcpy(data, buf, bytes);
		}
		index += 4 * 168;
		OQS_SHA3_shake128_x4_inc_ctx_release(&hashInstance);
	}
}

#else

static void snova_pk_expand(uint8_t *data, size_t num_bytes, const uint8_t *in, size_t inlen) {
	assert(inlen == SEED_LENGTH_PUBLIC);
	(void)inlen;

	size_t index = 0;
	uint64_t block = 0;

	while (index < num_bytes) {
		OQS_SHA3_shake128_inc_ctx hashInstance;
		OQS_SHA3_shake128_inc_init(&hashInstance);
		OQS_SHA3_shake128_inc_absorb(&hashInstance, in, inlen);

		// Turn SHAKE128 into SHAKE128 CTR-XOF
		for (int idx = 0; idx < 8; idx++) {
			// Little endian
			uint8_t block_i = (block >> (8 * idx)) & 0xff;
			OQS_SHA3_shake128_inc_absorb(&hashInstance, &block_i, 1);
		}

		OQS_SHA3_shake128_inc_finalize(&hashInstance);
		size_t bytes = num_bytes - index;
		if (bytes > 168) {
			bytes = 168;
		}

		OQS_SHA3_shake128_inc_squeeze(data, bytes, &hashInstance);
		OQS_SHA3_shake128_inc_ctx_release(&hashInstance);

		block++;
		data += bytes;
		index += bytes;
	}
}

#endif

#else

typedef struct {
	uint64_t states[50];
	uint8_t seed[SEED_LENGTH_PUBLIC];
	uint32_t index;
	uint32_t last_idx;
	uint16_t block;
	int16_t bytes_left;
} snova_pk_expander_t;

static void snova_pk_expander_init(snova_pk_expander_t *instance, const uint8_t *seed, size_t input_bytes) {
	assert(input_bytes == SEED_LENGTH_PUBLIC);
	(void)input_bytes;

	instance->block = 0;
	instance->index = 0;
	instance->last_idx = 0;
	instance->bytes_left = 0;

	memcpy(instance->seed, seed, SEED_LENGTH_PUBLIC);
}

static void snova_pk_expand_block(snova_pk_expander_t *instance) {
	uint8_t *data = (uint8_t *)instance->states;
	OQS_SHA3_shake128_inc_ctx hashInstance;
	OQS_SHA3_shake128_inc_init(&hashInstance);
	OQS_SHA3_shake128_inc_absorb(&hashInstance, instance->seed, SEED_LENGTH_PUBLIC);

	// Turn SHAKE128 into SHAKE128 CTR-XOF
	// Little endian
	for (int idx = 0; idx < 8; idx++) {
		uint8_t block_i = ((uint64_t)instance->block >> (8 * idx)) & 0xff;
		OQS_SHA3_shake128_inc_absorb(&hashInstance, &block_i, 1);
	}
	instance->block++;

	OQS_SHA3_shake128_inc_finalize(&hashInstance);
	OQS_SHA3_shake128_inc_squeeze(data, 168, &hashInstance);
	OQS_SHA3_shake128_inc_ctx_release(&hashInstance);

	// Convert to GF16
	uint8_t *state8 = (uint8_t *)instance->states;
	for (int32_t idx = 168 - 1; idx >= 0; idx--) {
		state8[2 * idx + 1] = (state8[idx] >> 4) & 0xf;
		state8[2 * idx] = state8[idx] & 0xf;
	}
}

static void snova_pk_expander_squeeze(uint8_t *data, size_t num_gf, snova_pk_expander_t *instance) {
	uint8_t *data8 = data;
	instance->last_idx += num_gf;

	if (instance->bytes_left > 0) {
		uint8_t *state8 = (uint8_t *)instance->states + 2 * 168 - instance->bytes_left;

		if (instance->bytes_left >= (int32_t)num_gf) {
			memcpy(data, state8, num_gf);
			instance->bytes_left -= num_gf;
			instance->index += num_gf;
			return;
		}

		memcpy(data8, state8, instance->bytes_left);
		instance->index += instance->bytes_left;
		data8 += instance->bytes_left;
	}

	while (instance->index < instance->last_idx) {
		snova_pk_expand_block(instance);

		size_t bytes = instance->last_idx - instance->index;
		if (bytes > 2 * 168) {
			bytes = 2 * 168;
		} else {
			instance->bytes_left = 2 * 168 - bytes;
		}

		memcpy(data8, instance->states, bytes);
		instance->index += bytes;
		data8 += bytes;
	}
}

static void snova_pk_expander_goto(snova_pk_expander_t *instance, size_t index) {
	instance->block = index / (2 * 168);
	snova_pk_expand_block(instance);

	instance->last_idx = index;
	instance->bytes_left = 2 * 168 - (index % (2 * 168));
	instance->index = index;
}

static void snova_pk_expander_free(snova_pk_expander_t *instance) {
	(void)instance;
}

#endif
#endif
#endif

#endif /* SYMMETRIC_H */
