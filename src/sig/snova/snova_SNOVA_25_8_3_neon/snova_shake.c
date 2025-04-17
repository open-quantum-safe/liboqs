// SPDX-License-Identifier: MIT
/**
 * Vectorized SNOVA-SHAKE XOF
 *
 * The optimised implementation can be tested against the reference using the genkat function of SNOVA
 * for optimisation levels OPTIMISATION=0 versus OPTIMISATION=1 or OPTIMISATION=2.
 */

#include <assert.h>
#include <string.h>

#include "snova.h"

#if OPTIMISATION == 0

#error "Use snova_shake_ref"

#elif defined(SNOVA_LIBOQS)

#if OPTIMISATION == 2 && defined(OQS_ENABLE_SHA3_xkcp_low_avx2)

/**
 * liboqs x4 version
 */
#include <oqs/sha3x4.h>

void snova_shake(const uint8_t *pt_seed_array, size_t input_bytes, uint64_t *data64, size_t output_bytes) {

	size_t index = 0;
	uint64_t block = 0;
	uint8_t *data = (uint8_t *)data64;

	while (index < output_bytes) {
		OQS_SHA3_shake128_x4_inc_ctx hashInstance;
		OQS_SHA3_shake128_x4_inc_init(&hashInstance);
		OQS_SHA3_shake128_x4_inc_absorb(&hashInstance, pt_seed_array, pt_seed_array,
		                                pt_seed_array, pt_seed_array, input_bytes);

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
		OQS_SHA3_shake128_x4_inc_absorb(&hashInstance, (uint8_t *)&block_0, (uint8_t *)&block_1,
		                                (uint8_t *)&block_2, (uint8_t *)&block_3, 8);

		OQS_SHA3_shake128_x4_inc_finalize(&hashInstance);
		size_t bytes = output_bytes - index;
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
/**
 * liboqs version of reference
 */
void snova_shake(const uint8_t *pt_seed_array, size_t input_bytes, uint64_t *data64, size_t output_bytes) {

	size_t index = 0;
	uint64_t block = 0;
	uint8_t *data = (uint8_t *)data64;

	while (index < output_bytes) {
		OQS_SHA3_shake128_inc_ctx hashInstance;
		OQS_SHA3_shake128_inc_init(&hashInstance);
		OQS_SHA3_shake128_inc_absorb(&hashInstance, pt_seed_array, input_bytes);

		// Turn SHAKE128 into SHAKE128 CTR-XOF
		for (int idx = 0; idx < 8; idx++) {
			// Little endian
			uint8_t block_i = (block >> (8 * idx)) & 0xff;
			OQS_SHA3_shake128_inc_absorb(&hashInstance, &block_i, 1);
		}

		OQS_SHA3_shake128_inc_finalize(&hashInstance);
		size_t bytes = output_bytes - index;
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

/**
 * Optimized version
 *
 * The optimised implementation can be tested against the reference using the genkat function of SNOVA
 * for optimisation levels OPTIMISATION=0 versus OPTIMISATION=1 or OPTIMISATION=2.
 */

#if PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN
#error "PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN"
#endif

#ifndef PARALLELISM
#if __AVX512F__
#define PARALLELISM 8
#elif __AVX2__
#define PARALLELISM 4
#else
#define PARALLELISM 1
#endif
#endif

void KeccakP1600times8_PermuteAll_24rounds(void *state);
void KeccakP1600times4_PermuteAll_24rounds(void *state);

/**
 * Squeeze bytes in parallel.
 */
void snova_shake(const uint8_t *seed, size_t input_bytes, uint64_t *data, size_t num_bytes) {
	uint64_t keccak_instance[25] = {0};
	uint64_t prepared_state_[25 * PARALLELISM];
	alignas(PARALLELISM * 8) uint64_t states[25 * PARALLELISM];

	uint8_t *prepared_state = (uint8_t *)prepared_state_;
	uint64_t *data64 = data;

	// Align to uint64_t
	memcpy(&keccak_instance[0], seed, input_bytes);
	uint64_t block = 0;
	uint64_t index = 0;
	size_t last_idx = num_bytes;

	for (int idx = 0; idx < PARALLELISM; idx++) {
		uint64_t *state64 = (uint64_t *)keccak_instance;
		uint64_t *prep64 = (uint64_t *)prepared_state;

		for (int idx2 = 0; idx2 < 25; idx2++) {
			prep64[idx + idx2 * PARALLELISM] = state64[idx2];
		}
		// SHAKE padding. Use the (uint8_t *)prepared_state here
		prepared_state[idx * 8 + (input_bytes + 8) * PARALLELISM] ^= 0x1F;
		prepared_state[idx * 8 + (168 - 8) * PARALLELISM + 7] ^= 0x80;
	}

	while (index < last_idx) {
		uint32_t byteIOIndex = input_bytes;
		memcpy(states, prepared_state, PARALLELISM * 200);
		for (int idx = 0; idx < PARALLELISM; idx++) {
			states[byteIOIndex * PARALLELISM / 8 + idx] ^= block;
			block++;
		}

#if PARALLELISM == 1
		KeccakP1600_Permute_24rounds((void *)states);
#elif PARALLELISM == 4
		KeccakP1600times4_PermuteAll_24rounds((void *)states);
#elif PARALLELISM == 8
		KeccakP1600times8_PermuteAll_24rounds((void *)states);
#else
#error "PARALLELISM must be 1, 4 or 8"
#endif

		for (int idx = 0; idx < PARALLELISM; idx++) {
			size_t bytes = last_idx - index;
			if (bytes > 168) {
				bytes = 168;
			}

			uint64_t *source64 = states + idx;
			for (size_t idx2 = 0; idx2 < bytes / 8; idx2++) {
				*data64 = *source64;
				source64 += PARALLELISM;
				data64++;
			}
			index += bytes;

			if (index >= last_idx) {
				break;
			}
		}
	}
}
#endif
