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
/**
 * Reference version
 */
void snova_shake(const uint8_t *pt_seed_array, size_t input_bytes, uint64_t *data64, size_t output_bytes) {
	assert(input_bytes <= 152);
	assert(output_bytes % 8 == 0);

	size_t index = 0;
	uint64_t block = 0;
	uint8_t *data = (uint8_t *)data64;

	while (index < output_bytes) {
		Keccak_HashInstance hashInstance;
		Keccak_HashInitialize_SHAKE128(&hashInstance);
		Keccak_HashUpdate(&hashInstance, pt_seed_array, 8 * input_bytes);

		// Turn SHAKE128 into SHAKE128 CTR-XOF
		for (int idx = 0; idx < 8; idx++) {
			// Little endian
			uint8_t block_i = (block >> (8 * idx)) & 0xff;
			Keccak_HashUpdate(&hashInstance, &block_i, 8);
		}

		Keccak_HashFinal(&hashInstance, NULL);
		size_t bytes = output_bytes - index;
		if (bytes > (hashInstance.sponge.rate / 8)) {
			bytes = (hashInstance.sponge.rate / 8);
		}

		Keccak_HashSqueeze(&hashInstance, data, 8 * bytes);

		block++;
		data += bytes;
		index += bytes;
	}
}

#elif defined(SNOVA_LIBOQS) && (OPTIMISATION == 1)

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
		OQS_SHA3_shake128_inc_absorb(&hashInstance, pt_seed_array,  input_bytes);

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

#if defined(SNOVA_LIBOQS) && !__ARM_ARCH
#define KeccakP1600times4_PermuteAll_24rounds KeccakP1600times4_PermuteAll_24rounds_avx2
#endif

#ifndef PARALLELISM
#if __AVX512F__ && !defined(SNOVA_LIBOQS)
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
	uint8_t prepared_state[200 * PARALLELISM] __attribute__((aligned(8)));
	uint64_t states[25 * PARALLELISM];
	uint64_t *states64 = states;
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
			states64[byteIOIndex * PARALLELISM / 8 + idx] ^= block;
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

			uint64_t *source64 = states64 + idx;
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
