/*! @file LowMCEnc.c
 *  @brief C implementation of LowMC encrypt. This is used to generate keys
 *  for the Picnic signature scheme.
 *
 *  This file is part of the reference implementation of the Picnic and Fish
 *  signature schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha
 *  Cryptology ePrint Archive: Report 2017/279
 *  http://eprint.iacr.org/2017/279
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <immintrin.h>

#include "LowMCConstants.h"
#include "platform.h"

#define t 8

uint32_t (*linearMatricesLookupTable)[STATE_SIZE_BITS / t][1 << t][STATE_SIZE_WORDS];
uint32_t (*keyMatricesLookupTable)[STATE_SIZE_BITS / t][1 << t][STATE_SIZE_WORDS];
uint32_t (*roundConstants)[STATE_SIZE_WORDS];

__m256i (*linearMatricesLookupTableVec)[STATE_SIZE_BITS / t][1 << t][1];
__m256i (*keyMatricesLookupTableVec)[STATE_SIZE_BITS / t][1 << t][1];
__m256i (*roundConstantsVec)[1];

/* Helper functions */

/* Get one bit from a byte array */
static uint8_t getBit(const uint32_t *array, uint32_t bitNumber) {
	return (array[bitNumber / WORD_SIZE_BITS] >> (WORD_SIZE_BITS - 1 - (bitNumber % WORD_SIZE_BITS))) & 0x01;
}

/* Set a specific bit in a byte array to a given value */
static void setBit(uint32_t *bytes, uint32_t bitNumber, uint8_t val) {
	bytes[bitNumber / WORD_SIZE_BITS] = (bytes[bitNumber / WORD_SIZE_BITS] & ~(1 << (WORD_SIZE_BITS - 1 - (bitNumber % WORD_SIZE_BITS)))) | (val << (WORD_SIZE_BITS - 1 - (bitNumber % WORD_SIZE_BITS)));
}

static void xor_bytes(uint32_t *in1, uint32_t *in2, uint32_t *out, uint32_t numBytes) {
	for (uint32_t i = 0; i < numBytes; i++) {
		out[i] = in1[i] ^ in2[i];
	}
}

static void multiplyWithMatrix(uint32_t *state,
                               uint32_t matrix[STATE_SIZE_BITS / t][1 << t][STATE_SIZE_WORDS],
                               uint32_t *output, lowmcparams_t *params) {
	uint32_t temp[STATE_SIZE_WORDS] = {0};

	for (uint32_t i = 0; i < params->stateSizeWords; i++) {
		xor_bytes(temp, matrix[i * (sizeof(uint32_t))][(state[i] >> 24) & 0xFF], temp,
		          params->stateSizeWords);
		xor_bytes(temp, matrix[i * (sizeof(uint32_t)) + 1][(state[i] >> 16) & 0xFF],
		          temp, params->stateSizeWords);
		xor_bytes(temp, matrix[i * (sizeof(uint32_t)) + 2][(state[i] >> 8) & 0xFF],
		          temp, params->stateSizeWords);
		xor_bytes(temp, matrix[i * (sizeof(uint32_t)) + 3][(state[i]) & 0xFF], temp,
		          params->stateSizeWords);
	}
	memcpy(output, &temp, params->stateSizeBytes);
}

// starts from the left, and interprets the first bit t encounters as the highest order bit (compared to the lookup table
// in lowmc reference code. may need to change this
static void substitution(uint32_t *state, lowmcparams_t *params) {
	for (uint32_t i = 0; i < params->numSboxes * 3; i += 3) {
		uint8_t a = getBit(state, params->stateSizeBits - 1 - i - 2);
		uint8_t b = getBit(state, params->stateSizeBits - 1 - i - 1);
		uint8_t c = getBit(state, params->stateSizeBits - 1 - i);
		setBit(state, params->stateSizeBits - 1 - i - 2, a ^ (b & c));
		setBit(state, params->stateSizeBits - 1 - i - 1, a ^ b ^ (a & c));
		setBit(state, params->stateSizeBits - 1 - i, a ^ b ^ c ^ (a & b));
	}
}

static void computeRoundKey(uint32_t *key, uint32_t *output, uint32_t round, lowmcparams_t *params) {
	multiplyWithMatrix(key, keyMatricesLookupTable[round], output, params);
}

int readRoundConstants(lowmcparams_t *params, FILE *fp) {
	size_t ret;
	uint32_t numRounds = params->numRounds;
	uint32_t stateWords = params->stateSizeWords;

	roundConstants = malloc(
	    numRounds * sizeof(*roundConstants));
	if (roundConstants == NULL) {
		fprintf(stderr, "%s: Failed to allocate roundConstants.\n", __func__);
		fclose(fp);
		return -1;
	}

#ifdef WITH_AVX
	roundConstantsVec = _mm_malloc(
	    numRounds * sizeof(*roundConstantsVec), 32);
	if (roundConstantsVec == NULL) {
		fprintf(stderr, "%s: Failed to allocate roundConstantsVec.\n", __func__);
		fclose(fp);
		return -1;
	}
#endif

	for (uint32_t i = 0; i < numRounds; i++) {
		ret = fread(&roundConstants[i],
		            sizeof(uint32_t), stateWords, fp);
#ifdef WITH_AVX
		*roundConstantsVec[i] = _mm256_loadu_si256(
		    (__m256i *) roundConstants[i]);
#endif
		if (ret != stateWords) {
			fprintf(stderr, "%s: Failed to read round constants, aborting.\n", __func__);
			fclose(fp);
			free(roundConstants);
#ifdef WITH_AVX
			_mm_free(roundConstantsVec);
#endif
			return -1;
		}
	}
	fclose(fp);
	return 0;
}

int readLookupTables(lowmcparams_t *params, const char *path) {
	FILE *fp;
	size_t ret;
	char filename[256];

	if (path == NULL) {
		path = DEFAULT_DATA_PATH;
	} else if (strlen(path) > 220) {
		fprintf(stderr, "%s: Faield to read lookup tables. Provided path is too long\n", __func__);
		return -1;
	}

	sprintf(filename, "%slookupTables_%d_%d_%d.bin", path, params->stateSizeBits, params->numSboxes, params->numRounds);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "%s: Failed to open '%s'.\n", __func__, filename);
		return -1;
	}

	uint32_t numRounds = params->numRounds;
	uint32_t stateBits = params->stateSizeBits;
	uint32_t stateWords = params->stateSizeWords;
	linearMatricesLookupTable = malloc(
	    numRounds * sizeof(*linearMatricesLookupTable));
	if (linearMatricesLookupTable == NULL) {
		fprintf(stderr, "%s: Failed to allocate linearMatricesLookupTable.\n", __func__);
		fclose(fp);
		return -1;
	}

#ifdef WITH_AVX
	linearMatricesLookupTableVec = _mm_malloc(
	    numRounds * sizeof(*linearMatricesLookupTableVec), 32);
	if (linearMatricesLookupTableVec == NULL) {
		fprintf(stderr, "%s: Failed to allocate keyMatricesLookupTableVec.\n", __func__);
		fclose(fp);
		return -1;
	}
#endif

	for (uint32_t i = 0; i < numRounds; i++) {
		for (uint32_t j = 0; j < stateBits / t; j++) {
			for (uint32_t k = 0; k < (1 << t); k++) {
				ret = fread(linearMatricesLookupTable[i][j][k], sizeof(uint32_t), stateWords, fp);
#ifdef WITH_AVX
				*linearMatricesLookupTableVec[i][j][k] = _mm256_loadu_si256(
				    (__m256i *) linearMatricesLookupTable[i][j][k]);
#endif
				if (ret != stateWords) {
					fprintf(stderr, "%s: Failed to read look up table, aborting.\n", __func__);
					fclose(fp);
					free(linearMatricesLookupTable);
#ifdef WITH_AVX
					_mm_free(linearMatricesLookupTableVec);
#endif
					return -1;
				}
			}
		}
	}

	numRounds++;

	keyMatricesLookupTable = malloc(numRounds * sizeof(*keyMatricesLookupTable));

	if (keyMatricesLookupTable == NULL) {
		fprintf(stderr, "%s: Failed to allocate keyMatricesLookupTable.\n", __func__);
		fclose(fp);
		free(linearMatricesLookupTable);
		return -1;
	}

#ifdef WITH_AVX
	keyMatricesLookupTableVec = _mm_malloc(numRounds * sizeof(*keyMatricesLookupTableVec), 32);

	if (keyMatricesLookupTableVec == NULL) {
		fprintf(stderr, "%s: Failed to allocate keyMatricesLookupTableVec.\n", __func__);
		fclose(fp);
		_mm_free(linearMatricesLookupTableVec);
		return -1;
	}
#endif

	for (uint32_t i = 0; i < numRounds; i++) {
		for (uint32_t j = 0; j < stateBits / t; j++) {
			for (uint32_t k = 0; k < (1 << t); k++) {
				ret = fread(keyMatricesLookupTable[i][j][k], sizeof(uint32_t),
				            stateWords, fp);
#ifdef WITH_AVX
				*keyMatricesLookupTableVec[i][j][k] = _mm256_loadu_si256(
				    (__m256i *) keyMatricesLookupTable[i][j][k]);
#endif

				if (ret != stateWords) {
					fprintf(stderr, "%s: Failed to read look up table, aborting. \n", __func__);
					fclose(fp);
					free(linearMatricesLookupTable);
					free(keyMatricesLookupTable);
#ifdef WITH_AVX
					_mm_free(linearMatricesLookupTableVec);
					_mm_free(keyMatricesLookupTable);
#endif
					return -1;
				}
			}
		}
	}

	return readRoundConstants(params, fp);
}
void freeRoundConstants() {
	free(roundConstants);
#ifdef WITH_AVX
	_mm_free(roundConstantsVec);
#endif
}

void freeLookupTables() {
	free(linearMatricesLookupTable);
	free(keyMatricesLookupTable);
#ifdef WITH_AVX
	_mm_free(linearMatricesLookupTableVec);
	_mm_free(keyMatricesLookupTableVec);
#endif
	freeRoundConstants();
}

void LowMCEnc(const uint32_t *plaintext, uint32_t *output, uint32_t *key, lowmcparams_t *params) {
	uint32_t roundKey[LOWMC_MAX_STATE_SIZE / sizeof(uint32_t)];

	if (plaintext != output) {
		/* output will hold the intermediate state */
		memcpy(output, plaintext, params->stateSizeBytes);
	}

	computeRoundKey(key, roundKey, 0, params);
	xor_bytes(output, roundKey, output, params->stateSizeWords);

	for (uint32_t r = 1; r <= params->numRounds; r++) {
		computeRoundKey(key, roundKey, r, params);
		substitution(output, params);
		multiplyWithMatrix(output, linearMatricesLookupTable[r - 1], output, params);
		xor_bytes(output, roundConstants[r - 1], output, params->stateSizeWords);
		xor_bytes(output, roundKey, output, params->stateSizeWords);
	}
}
