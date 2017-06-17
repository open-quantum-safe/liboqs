/*! @file LowMC.c
 *  @brief This is the main file of the signature scheme. All of the LowMC MPC
 *  code is here as well as lower-level versions of sign and verify that are
 *  called by the signature API.
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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WITH_AVX
#include <immintrin.h>
#endif

#include <openssl/sha.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/engine.h>

#include "LowMC.h"
#include "picnic.h"
#include "platform.h"

#define t 8

/* prefixes for domain separation of multiple uses of SHA256 */
#define H_FOR_COMMITMENT_PREFIX 0
#define H_FOR_CHALLENGE_PREFIX 1
#define G_PREFIX 2

#define UNUSED_PARAMETER(x) (void) (x)

/* Type definitions */
typedef struct randomTape_t {
	uint8_t *tape[3];
	uint32_t pos;
} randomTape_t;

typedef struct view_t {
	uint32_t *inputShare;
	uint8_t *communicatedBits;
	uint32_t *outputShare;
} view_t;

typedef struct commitments_t {
	uint8_t hashes[3][SHA256_DIGEST_LENGTH];
} commitments_t;

typedef struct unruh_permutations_t {
	uint8_t *G[3];
} unruh_permutations_t;

/* Allocate/free functions for dynamically sized types */
void allocateView(view_t *view, lowmcparams_t *params) {
	view->inputShare = malloc(params->stateSizeBytes);
	view->communicatedBits = malloc(params->numAndBytes);
	view->outputShare = malloc(params->stateSizeBytes);
}

void freeView(view_t *view) {
	free(view->inputShare);
	free(view->communicatedBits);
	free(view->outputShare);
}

void allocateRandomTape(randomTape_t *tape, lowmcparams_t *params) {
	for (uint8_t i = 0; i < 3; i++) {
		tape->tape[i] = malloc(numBytes(params->numSboxes * 3 * params->numRounds * NUM_ZKB_ROUNDS - 1));
	}
}

void freeRandomTape(randomTape_t *tape) {
	for (uint8_t i = 0; i < 3; i++) {
		free(tape->tape[i]);
	}
}

void allocateProof(proof_t *proof, lowmcparams_t *params) {
	proof->inputShare = malloc(params->stateSizeBytes);
	proof->communicatedBits = malloc(params->numAndBytes);
	if (params->UnruhGWithInputBytes > 0) {
		proof->view3UnruhG = malloc(params->UnruhGWithInputBytes);
	} else {
		proof->view3UnruhG = NULL;
	}
}

void freeProof(proof_t *proof) {
	free(proof->inputShare);
	free(proof->communicatedBits);
	free(proof->view3UnruhG);
}

void allocateSignature(signature_t *sig, lowmcparams_t *params) {
	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		allocateProof(&(sig->proofs[i]), params);
	}
}

void freeSignature(signature_t *sig) {
	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		freeProof(&(sig->proofs[i]));
	}
}

void allocateUnruhPermutations(unruh_permutations_t *permutations, lowmcparams_t *params) {
	for (uint8_t i = 0; i < 3; i++) {
		permutations->G[i] = malloc(params->UnruhGWithInputBytes); // we initialize to the max length for all 3, even though G[0] and G[1] will be smaller
	}
}

void freeUnruhPermutations(unruh_permutations_t *permutations) {
	for (uint8_t i = 0; i < 3; i++) {
		free(permutations->G[i]);
	}
}

/* Global data */
#ifdef WITH_AVX
__m256i (*linearMatricesLookupTableVec)[STATE_SIZE_BITS / t][1 << t][1];
__m256i (*keyMatricesLookupTableVec)[STATE_SIZE_BITS / t][1 << t][1];
__m256i (*roundConstantsVec)[1];
#else
uint32_t (*linearMatricesLookupTable)[STATE_SIZE_BITS / t][1 << t][STATE_SIZE_WORDS];
uint32_t (*keyMatricesLookupTable)[STATE_SIZE_BITS / t][1 << t][STATE_SIZE_WORDS];
uint32_t (*roundConstants)[STATE_SIZE_WORDS];
#endif

/* Helper functions */

/* Get one bit from a 32-bit int array */
uint8_t getBitFromWordArray(const uint32_t *array, uint32_t bitNumber) {
	return (array[bitNumber / WORD_SIZE_BITS] >> (WORD_SIZE_BITS - 1 - (bitNumber % WORD_SIZE_BITS))) & 0x01;
}

/* Set a specific bit in a byte array to a given value */
void setBitInWordArray(uint32_t *bytes, uint32_t bitNumber, uint8_t val) {
	bytes[bitNumber / WORD_SIZE_BITS] = (bytes[bitNumber / WORD_SIZE_BITS] & ~(1 << (WORD_SIZE_BITS - 1 - (bitNumber % WORD_SIZE_BITS)))) | (val << (WORD_SIZE_BITS - 1 - (bitNumber % WORD_SIZE_BITS)));
}

/* Get one bit from a byte array */
uint8_t getBit(const uint8_t *array, uint32_t bitNumber) {
	return (array[bitNumber >> 3] >> (7 - (bitNumber % 8))) & 0x01;
}

/* Set a specific bit in a byte array to a given value */
void setBit(uint8_t *bytes, uint32_t bitNumber, uint8_t val) {
	bytes[bitNumber >> 3] = (bytes[bitNumber >> 3] & ~(1 << (7 - (bitNumber % 8)))) | (val << (7 - (bitNumber % 8)));
}

uint32_t getRandom32(uint8_t *randomness, uint32_t randCount) {
	uint32_t ret;

	memcpy(&ret, &randomness[randCount], 4);
	return ret;
}

uint32_t numBytes(uint32_t numBits) {
	return (numBits == 0) ? 0 : ((numBits - 1) / 8 + 1);
}

bool getAllRandomness(const uint8_t seed[PRG_SEED_LENGTH], uint8_t *tape,
                      uint32_t tapeLengthBytes, const unsigned char iv[16]) {
	/* Zero the tape, then encrypt it with AES256-CTR. The seed is the key. */
	EVP_CIPHER_CTX *ctx;
	//EVP_CIPHER_CTX ctx;

	//EVP_CIPHER_CTX_init(&ctx);
	if (!(ctx = EVP_CIPHER_CTX_new()))
		return false;
	//if (1 != EVP_EncryptInit_ex(&ctx, EVP_aes_256_ctr(), NULL, seed, iv)) {
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, seed, iv)) {
		fprintf(stderr, "%s: Error setting up AES", __func__);
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_cleanup(ctx);
		return false;
	}

	memset(tape, 0, tapeLengthBytes);
	int len;
	if (1 != EVP_EncryptUpdate(ctx, tape, &len, tape, tapeLengthBytes)) {
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_cleanup(ctx);
		return false;
	}

	//EVP_CIPHER_CTX_cleanup(&ctx);
	EVP_CIPHER_CTX_free(ctx);
	return true;
}

void init_EVP() {
	/* Initialize the library */
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
}

void cleanup_EVP() {
	/* Clean up sequence recommended here: https://wiki.openssl.org/index.php/Library_Initialization */
	ENGINE_cleanup();
	CONF_modules_unload(1);
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	//ERR_remove_thread_state(NULL);
	ERR_free_strings();
}

void mpc_xor(uint32_t *state[3], uint32_t *in2[3],
             uint32_t count) {
	uint64_t **state_64 = (uint64_t **) state;
	uint64_t **in2_64 = (uint64_t **) in2;

	count /= 2;
	for (uint8_t i = 0; i < 3; i++) {
		for (uint32_t j = 0; j < count; j++) {
			state_64[i][j] = state_64[i][j] ^ in2_64[i][j];
		}
	}
}

void mpc_xor4_3(uint32_t *temp[3], uint32_t *in[4][3], lowmcparams_t *params) {
	uint64_t **in1_64 = (uint64_t **) in[0];
	uint64_t **in2_64 = (uint64_t **) in[1];
	uint64_t **in3_64 = (uint64_t **) in[2];
	uint64_t **in4_64 = (uint64_t **) in[3];
	uint64_t **temp_64 = (uint64_t **) temp;

	for (uint8_t i = 0; i < 3; i++) {
		for (uint32_t j = 0; j < params->stateSizeWords / 2; j++) {
			temp_64[i][j] = in1_64[i][j] ^ in2_64[i][j] ^ in3_64[i][j] ^ in4_64[i][j] ^ temp_64[i][j];
		}
	}
}

void mpc_xor4_2(uint32_t *temp[2], uint32_t *in[4][2], lowmcparams_t *params) {
	uint64_t **in1_64 = (uint64_t **) in[0];
	uint64_t **in2_64 = (uint64_t **) in[1];
	uint64_t **in3_64 = (uint64_t **) in[2];
	uint64_t **in4_64 = (uint64_t **) in[3];
	uint64_t **temp_64 = (uint64_t **) temp;

	for (uint8_t i = 0; i < 2; i++) {
		for (uint32_t j = 0; j < params->stateSizeWords / 2; j++) {
			temp_64[i][j] = in1_64[i][j] ^ in2_64[i][j] ^ in3_64[i][j] ^ in4_64[i][j] ^ temp_64[i][j];
		}
	}
}

void mpc_xor2(uint32_t *state[2], uint32_t *in2[2], uint32_t count) {

	uint64_t **state_64 = (uint64_t **) state;
	uint64_t **in2_64 = (uint64_t **) in2;

	count /= 2;
	for (uint8_t i = 0; i < 2; i++) {
		for (uint32_t j = 0; j < count; j++) {
			state_64[i][j] = state_64[i][j] ^ in2_64[i][j];
		}
	}
}

#ifdef WITH_AVX
void mpc_xor_vec3(__m256i state[3], __m256i val[3], __m256i output[3]) {
	for (uint8_t i = 0; i < 3; i++) {
		output[i] = _mm256_xor_si256(state[i], val[i]);
	}
}

void mpc_xor_vec2(__m256i state[2], __m256i val[2], __m256i output[2]) {
	for (uint8_t i = 0; i < 2; i++) {
		output[i] = _mm256_xor_si256(state[i], val[i]);
	}
}

/* Having specialized versions of these xor functions gives better performance */
void mpc_xor4_vec3(__m256i temp_vec[3], __m256i *bits_vec[4][3]) {
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			temp_vec[i] = _mm256_xor_si256(temp_vec[i], *bits_vec[j][i]);
		}
	}
}

void mpc_xor4_vec2(__m256i temp_vec[2], __m256i *bits_vec[4][2]) {
	for (uint8_t i = 0; i < 2; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			temp_vec[i] = _mm256_xor_si256(temp_vec[i], *bits_vec[j][i]);
		}
	}
}
#endif

void Commit(const uint8_t seed[PRG_SEED_LENGTH], const view_t view,
            uint8_t hash[SHA256_DIGEST_LENGTH], lowmcparams_t *params) {
	SHA256_CTX ctx;
	uint8_t prefix = H_FOR_COMMITMENT_PREFIX;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &prefix, sizeof(uint8_t));
	SHA256_Update(&ctx, seed, PRG_SEED_LENGTH);
	SHA256_Update(&ctx, view.inputShare, params->stateSizeBytes);
	SHA256_Update(&ctx, view.communicatedBits, params->numAndBytes);
	SHA256_Update(&ctx, view.outputShare, params->stateSizeBytes);
	SHA256_Final(hash, &ctx);
}

void generateAesIv(const uint32_t *pubKey, const uint32_t *pubInput, uint8_t iv[16], lowmcparams_t *params) {
	SHA256_CTX ctx;
	uint8_t hash[SHA256_DIGEST_LENGTH];

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, (const uint8_t *) pubKey, params->stateSizeBytes);
	SHA256_Update(&ctx, (const uint8_t *) pubInput, params->stateSizeBytes);
	SHA256_Final(hash, &ctx);

	memcpy(iv, hash, 16);
}

#if (UNRUH_G_WITH_INPUT_LENGTH / SHA256_DIGEST_LENGTH) > 256
#error Permutation function G will truncate when casting i from size_t to uint8_t
#endif

/* This is the random oracle "permuatation" function G for Unruh's transform */
void G(uint8_t viewNumber, const uint8_t *seed, view_t *view, uint8_t *output, lowmcparams_t *params) {
	uint8_t compressed[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	uint8_t prefix = G_PREFIX;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &prefix, sizeof(uint8_t));
	SHA256_Update(&ctx, seed, PRG_SEED_LENGTH);
	if (viewNumber == 2) {
		SHA256_Update(&ctx, view->inputShare, params->stateSizeBytes);
	}
	SHA256_Update(&ctx, view->communicatedBits, params->numAndBytes);
	SHA256_Final(compressed, &ctx);

	size_t viewUnruhLength = (viewNumber == 2) ? params->UnruhGWithInputBytes : params->UnruhGWithoutInputBytes;
	size_t numSha256Words = viewUnruhLength / SHA256_DIGEST_LENGTH;
	for (size_t i = 0; i < numSha256Words; i++) {
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, compressed, SHA256_DIGEST_LENGTH);
		SHA256_Update(&ctx, (uint8_t *) &i, sizeof(uint8_t));
		SHA256_Final(&output[i * SHA256_DIGEST_LENGTH], &ctx);
	}
	if (viewUnruhLength % SHA256_DIGEST_LENGTH != 0) {
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, compressed, SHA256_DIGEST_LENGTH);
		SHA256_Update(&ctx, (uint8_t *) &numSha256Words, sizeof(uint8_t));
		SHA256_Final(compressed, &ctx); // reusing compressed as a buffer since we no longer need the compressed value
		// copy remaining bytes from buffer into output
		memcpy(&output[numSha256Words * SHA256_DIGEST_LENGTH], compressed, viewUnruhLength % SHA256_DIGEST_LENGTH);
	}
}

void setChallenge(uint8_t *challenge, size_t round, uint8_t trit) {
	/* challenge must have length numBytes(NUM_ZKB_ROUNDS*2)
     * 0 <= index < NUM_ZKB_ROUNDS
     * trit must be in {0,1,2} */
	uint32_t roundU = (uint32_t) round;

	setBit(challenge, 2 * roundU, trit & 1);
	setBit(challenge, 2 * roundU + 1, (trit >> 1) & 1);
}

uint8_t getChallenge(const uint8_t *challenge, size_t round) {
	uint32_t roundU = (uint32_t) round;

	return (getBit(challenge, 2 * roundU + 1) << 1) | getBit(challenge, 2 * roundU);
}

void print_hex(const char *s, uint8_t *data, size_t len) {
	printf("%s: ", s);
	for (size_t i = 0; i < len; i++) {
		printf("%02X", data[i]);
	}
	printf("\n");
}

void H3(const uint32_t *circuitOutput, uint32_t *viewOutputs[NUM_ZKB_ROUNDS][3],
        commitments_t *as, uint32_t numRounds,
        uint8_t *challengeBits, const uint8_t *message, size_t messageByteLength,
        transform_t transform, unruh_permutations_t gs[NUM_ZKB_ROUNDS], lowmcparams_t *params) {
	uint8_t hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	uint8_t prefix = H_FOR_CHALLENGE_PREFIX;

	/* Depending on the number of rounds, we might not set part of the last
     * byte, make sure it's always zero. */
	challengeBits[numBytes(numRounds * 2) - 1] = 0;

	/* Hash input data */
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &prefix, sizeof(uint8_t));
	SHA256_Update(&ctx, circuitOutput, params->stateSizeBytes);
	SHA256_Update(&ctx, message, messageByteLength);
	for (uint32_t i = 0; i < numRounds; i++) {
		for (int j = 0; j < 3; j++) {
			SHA256_Update(&ctx, as[i].hashes[j], SHA256_DIGEST_LENGTH);
			SHA256_Update(&ctx, viewOutputs[i][j], params->stateSizeBytes);
			if (transform == TRANSFORM_UR) {
				// gs will be non-NULL for the Unruh transform
				size_t view3UnruhLength = (j == 2) ? params->UnruhGWithInputBytes : params->UnruhGWithoutInputBytes;
				SHA256_Update(&ctx, gs[i].G[j], view3UnruhLength);
			}
		}
	}
	SHA256_Final(hash, &ctx);

	/* Convert hash to a packed string of values in {0,1,2} */
	size_t round = 0;
	while (1) {
		for (size_t i = 0; i < sizeof(hash); i++) {
			uint8_t byte = hash[i];
			/* iterate over each pair of bits in the byte */
			for (int j = 0; j < 8; j += 2) {
				uint8_t bitPair = ((byte >> (6 - j)) & 0x03);
				if (bitPair < 3) {
					setChallenge(challengeBits, round, bitPair);
					round++;
					if (round == numRounds) {
						goto done;
					}
				}
			}
		}

		/* We need more bits; hash set hash = H(hash) */
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, &prefix, sizeof(uint8_t));
		SHA256_Update(&ctx, hash, sizeof(hash));
		SHA256_Final(hash, &ctx);
	}

done:

	return;
}

/* Caller must allocate the first parameter */
void prove(proof_t *proof, uint8_t challenge, uint8_t seeds[3][PRG_SEED_LENGTH],
           view_t views[3], commitments_t *commitments, unruh_permutations_t *gs, lowmcparams_t *params) {
	memcpy(proof->seed1, seeds[challenge], PRG_SEED_LENGTH);
	memcpy(proof->seed2, seeds[(challenge + 1) % 3], PRG_SEED_LENGTH);
	if (challenge == 1 || challenge == 2) {
		memcpy(proof->inputShare, views[2].inputShare, params->stateSizeBytes);
	}
	memcpy(proof->communicatedBits, views[(challenge + 1) % 3].communicatedBits,
	       params->numAndBytes);

	memcpy(proof->view3Commitment, commitments->hashes[(challenge + 2) % 3],
	       SHA256_DIGEST_LENGTH);
	if (gs != NULL) {
		size_t view3UnruhLength = (challenge == 0) ? params->UnruhGWithInputBytes : params->UnruhGWithoutInputBytes;
		memcpy(proof->view3UnruhG, gs->G[(challenge + 2) % 3], view3UnruhLength);
	}
}

int serializeSignature(const signature_t *sig, uint8_t *sigBytes, size_t sigBytesLen, transform_t transform, lowmcparams_t *params) {
	const proof_t *proofs = sig->proofs;
	const uint8_t *challengeBits = sig->challengeBits;

	/* Validate input buffer is large enough */
	size_t bytesRequired = numBytes(2 * NUM_ZKB_ROUNDS) +
	                       NUM_ZKB_ROUNDS * (2 * PRG_SEED_LENGTH + params->stateSizeBytes + params->numAndBytes + SHA256_DIGEST_LENGTH);

	if (transform == TRANSFORM_UR) {
		bytesRequired += params->UnruhGWithoutInputBytes * NUM_ZKB_ROUNDS;
	}

	if (sigBytesLen < bytesRequired) {
		return -1;
	}

	uint8_t *sigBytesBase = sigBytes;

	memcpy(sigBytes, challengeBits, numBytes(2 * NUM_ZKB_ROUNDS));
	sigBytes += numBytes(2 * NUM_ZKB_ROUNDS);

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		memcpy(sigBytes, proofs[i].seed1, PRG_SEED_LENGTH);
		sigBytes += PRG_SEED_LENGTH;

		memcpy(sigBytes, proofs[i].seed2, PRG_SEED_LENGTH);
		sigBytes += PRG_SEED_LENGTH;

		uint8_t challenge = getChallenge(challengeBits, i);
		if (challenge == 1 || challenge == 2) {
			memcpy(sigBytes, proofs[i].inputShare, params->stateSizeBytes);
			sigBytes += params->stateSizeBytes;
		}

		memcpy(sigBytes, proofs[i].communicatedBits, params->numAndBytes);
		sigBytes += params->numAndBytes;

		memcpy(sigBytes, proofs[i].view3Commitment, SHA256_DIGEST_LENGTH);
		sigBytes += SHA256_DIGEST_LENGTH;

		if (transform == TRANSFORM_UR) {
			size_t view3UnruhLength = (challenge == 0) ? params->UnruhGWithInputBytes : params->UnruhGWithoutInputBytes;
			memcpy(sigBytes, proofs[i].view3UnruhG, view3UnruhLength);
			sigBytes += view3UnruhLength;
		}
	}

	return (int) (sigBytes - sigBytesBase);
}

static size_t computeInputShareSize(const uint8_t *challengeBits, size_t stateSizeBytes) {
	/* When the FS transform is used, the input share is included in the proof
     * only when the challenge is 1 or 2.  When dersializing, to compute the
     * number of bytes expected, we must check how many challenge values are 1
     * or 2. The parameter stateSizeBytes is the size of an input share. */
	size_t inputShareSize = 0;

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		uint8_t challenge = getChallenge(challengeBits, i);
		if (challenge == 1 || challenge == 2) {
			inputShareSize += stateSizeBytes;
		}
	}
	return inputShareSize;
}

int deserializeSignature(signature_t *sig, const uint8_t *sigBytes,
                         size_t sigBytesLen, transform_t transform, lowmcparams_t *params) {
	proof_t *proofs = sig->proofs;
	uint8_t *challengeBits = sig->challengeBits;

	/* Validate input buffer is large enough */
	if (sigBytesLen < numBytes(2 * NUM_ZKB_ROUNDS)) { /* ensure the input has at least the challenge */
		return EXIT_FAILURE;
	}
	size_t inputShareSize = computeInputShareSize(sigBytes, params->stateSizeBytes);
	size_t bytesExpected = numBytes(2 * NUM_ZKB_ROUNDS) +
	                       NUM_ZKB_ROUNDS * (2 * PRG_SEED_LENGTH + params->numAndBytes + SHA256_DIGEST_LENGTH) + inputShareSize;
	if (transform == TRANSFORM_UR) {
		bytesExpected += params->UnruhGWithoutInputBytes * NUM_ZKB_ROUNDS;
	}
	if (sigBytesLen < bytesExpected) {
		return EXIT_FAILURE;
	}

	memcpy(challengeBits, sigBytes, numBytes(2 * NUM_ZKB_ROUNDS));
	sigBytes += numBytes(2 * NUM_ZKB_ROUNDS);

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		memcpy(proofs[i].seed1, sigBytes, PRG_SEED_LENGTH);
		sigBytes += PRG_SEED_LENGTH;

		memcpy(proofs[i].seed2, sigBytes, PRG_SEED_LENGTH);
		sigBytes += PRG_SEED_LENGTH;

		uint8_t challenge = getChallenge(challengeBits, i);
		if (challenge == 1 || challenge == 2) {
			memcpy(proofs[i].inputShare, sigBytes, params->stateSizeBytes);
			sigBytes += params->stateSizeBytes;
		}

		memcpy(proofs[i].communicatedBits, sigBytes, params->numAndBytes);
		sigBytes += params->numAndBytes;

		memcpy(proofs[i].view3Commitment, sigBytes, SHA256_DIGEST_LENGTH);
		sigBytes += SHA256_DIGEST_LENGTH;

		if (transform == TRANSFORM_UR) {
			size_t view3UnruhLength = (challenge == 0) ? params->UnruhGWithInputBytes : params->UnruhGWithoutInputBytes;
			memcpy(proofs[i].view3UnruhG, sigBytes, view3UnruhLength);
			sigBytes += view3UnruhLength;
		}
	}

	return EXIT_SUCCESS;
}

void mpc_AND_verify(uint8_t in1[2], uint8_t in2[2], uint8_t out[2],
                    randomTape_t *rand, view_t *view1, view_t *view2, uint32_t *yCount) {
	uint8_t r[2] = {getBit(rand->tape[0], rand->pos), getBit(rand->tape[1], rand->pos)};

	(rand->pos)++;
	out[0] = (in1[0] & in2[1]) ^ (in1[1] & in2[0]) ^ (in1[0] & in2[0]) ^ r[0] ^ r[1];
	setBit(view1->communicatedBits, *yCount, out[0]);
	out[1] = getBit(view2->communicatedBits, *yCount);
	(*yCount)++;
}

#ifdef WITH_AVX
void mpc_multiplyWithMatrix2(__m256i stateVec[3],
                             uint32_t round, __m256i outputVec[3],
                             __m256i temp_vec[3], lowmcparams_t *params) {
	uint32_t *state[2];
	uint32_t i;

	for (i = 0; i < 2; i++) {
		state[i] = ((uint32_t *) &stateVec[i]);
	}

	__m256i *bits_vec[4][2];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 2; k++) {
				bits_vec[j][k] = linearMatricesLookupTableVec[round][i * (sizeof(uint32_t)) + j][(state[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_vec2(temp_vec, bits_vec);
	}

	for (i = 0; i < 2; i++) {
		memcpy(&outputVec[i], &temp_vec[i], params->stateSizeBytes);
	}
}

void mpc_multiplyWithKeyMatrix2(uint32_t *key[2], uint32_t round, __m256i output[2], lowmcparams_t *params) {
	uint32_t i;
	__m256i *bits_vec[4][2];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 2; k++) {
				bits_vec[j][k] = keyMatricesLookupTableVec[round][i * (sizeof(uint32_t)) + j][(key[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_vec2(output, bits_vec);
	}
}
#else

void mpc_multiplyWithMatrix2(uint32_t *state[2], uint32_t round,
                             uint32_t *output[2], uint32_t *tmp, lowmcparams_t *params) {
	uint32_t i;
	uint32_t *temp[2];

	memset(tmp, 0, params->stateSizeBytes * 2);

	temp[0] = tmp;
	temp[1] = tmp + params->stateSizeWords;

	uint32_t *bits[4][2];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 2; k++) {
				bits[j][k] = linearMatricesLookupTable[round][i * (sizeof(uint32_t)) + j][(state[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_2(temp, bits, params);
	}

	for (i = 0; i < 2; i++) {
		memcpy(output[i], temp[i], params->stateSizeBytes);
	}
}

void mpc_multiplyWithKeyMatrix2(uint32_t *key[2], uint32_t round,
                                uint32_t *output[2], lowmcparams_t *params) {
	uint32_t i;
	uint32_t *bits[4][2];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 2; k++) {
				bits[j][k] = keyMatricesLookupTable[round][i * (sizeof(uint32_t)) + j][(key[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_2(output, bits, params);
	}
}

#endif

#ifdef WITH_AVX
void computeRoundKey2(uint32_t *key[2], __m256i output[2], uint32_t round, lowmcparams_t *params) {
	mpc_multiplyWithKeyMatrix2(key, round, output, params);
}

void mpc_substitution2(__m256i stateVec[2], randomTape_t *rand, view_t *view1,
                       view_t *view2, uint32_t *yCount, lowmcparams_t *params) {
	uint8_t a[2];
	uint8_t b[2];
	uint8_t c[2];

	uint8_t ab[2];
	uint8_t bc[2];
	uint8_t ca[2];
	uint32_t *state[2];

	for (int i = 0; i < 2; i++) {
		state[i] = ((uint32_t *) &stateVec[i]);
	}

	for (uint32_t i = 0; i < params->numSboxes * 3; i += 3) {

		for (uint8_t j = 0; j < 2; j++) {
			a[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 2);
			b[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 1);
			c[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i);
		}

		mpc_AND_verify(a, b, ab, rand, view1, view2, yCount);
		mpc_AND_verify(b, c, bc, rand, view1, view2, yCount);
		mpc_AND_verify(c, a, ca, rand, view1, view2, yCount);

		for (uint8_t j = 0; j < 2; j++) {
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 2, a[j] ^ (bc[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 1, a[j] ^ b[j] ^ (ca[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i, a[j] ^ b[j] ^ c[j] ^ (ab[j]));
		}
	}
}

#else
void computeRoundKey2(uint32_t *key[2], uint32_t *output[2], uint32_t round, lowmcparams_t *params) {
	mpc_multiplyWithKeyMatrix2(key, round, output, params);
}

void mpc_substitution2(uint32_t *state[2], randomTape_t *rand, view_t *view1,
                       view_t *view2, uint32_t *yCount, lowmcparams_t *params) {
	for (uint32_t i = 0; i < params->numSboxes * 3; i += 3) {

		uint8_t a[2];
		uint8_t b[2];
		uint8_t c[2];

		for (uint8_t j = 0; j < 2; j++) {
			a[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 2);
			b[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 1);
			c[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i);
		}

		uint8_t ab[2];
		uint8_t bc[2];
		uint8_t ca[2];

		mpc_AND_verify(a, b, ab, rand, view1, view2, yCount);
		mpc_AND_verify(b, c, bc, rand, view1, view2, yCount);
		mpc_AND_verify(c, a, ca, rand, view1, view2, yCount);

		for (uint8_t j = 0; j < 2; j++) {
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 2, a[j] ^ (bc[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 1, a[j] ^ b[j] ^ (ca[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i, a[j] ^ b[j] ^ c[j] ^ (ab[j]));
		}
	}
}
#endif

#ifdef WITH_AVX
void verifyLowMC(view_t *view1, view_t *view2,
                 randomTape_t *tapes, uint32_t *slab,
                 const uint32_t *pubInput, lowmcparams_t *params) {
	uint32_t *keyShares[2];
	__m256i roundKeyVec[2];
	__m256i stateVec[2];
	__m256i tempVec[2];

	/* The slab parameter is unused, but we must keep it so the AVX
     * and non-AVX versions of this function have the same signature */
	UNUSED_PARAMETER(slab);

	memset(&roundKeyVec[0], 0, 2 * params->stateSizeBytes);

	for (int i = 0; i < 2; i++) {
		stateVec[i] = _mm256_load_si256((__m256i *) pubInput);
	}

	uint32_t commBitCount = 0;

	keyShares[0] = view1->inputShare;
	keyShares[1] = view2->inputShare;

	computeRoundKey2(keyShares, roundKeyVec, 0, params);
	mpc_xor_vec2(stateVec, roundKeyVec, stateVec);

	__m256i roundConstantVec[2];

	for (uint32_t r = 1; r <= params->numRounds; r++) {
		/* initialize both roundkeys to 0. they are contingent */
		memset(&roundKeyVec[0], 0, 2 * params->stateSizeBytes);
		roundConstantVec[0] = roundConstantVec[1] = roundConstantsVec[r - 1][0];
		computeRoundKey2(keyShares, roundKeyVec, r, params);
		mpc_substitution2(stateVec, tapes, view1, view2, &commBitCount, params);

		memset(&tempVec[0], 0, params->stateSizeBytes);
		memset(&tempVec[1], 0, params->stateSizeBytes);

		mpc_multiplyWithMatrix2(stateVec, r - 1, stateVec, tempVec, params);

		mpc_xor_vec2(stateVec, roundConstantVec, stateVec);
		mpc_xor_vec2(stateVec, roundKeyVec, stateVec);
	}

	memcpy(view1->outputShare, &stateVec[0], params->stateSizeBytes);
	memcpy(view2->outputShare, &stateVec[1], params->stateSizeBytes);
}
#else
void verifyLowMC(view_t *view1, view_t *view2,
                 randomTape_t *tapes, uint32_t *slab,
                 const uint32_t *pubInput, lowmcparams_t *params) {
	uint32_t *state[2];
	uint32_t *keyShares[2];
	uint32_t *roundKey[2];

	roundKey[0] = slab;
	roundKey[1] = slab + params->stateSizeWords;
	state[0] = roundKey[1] + params->stateSizeWords;
	state[1] = state[0] + params->stateSizeWords;
	uint32_t *tmp = state[1] + params->stateSizeWords;

	uint32_t commBitCount = 0;

	// initialize both roundkeys to 0. they are contingent
	memset(roundKey[0], 0, 2 * params->stateSizeBytes);

	for (uint32_t i = 0; i < 2; i++) {
		memcpy(state[i], pubInput, params->stateSizeBytes);
	}

	keyShares[0] = view1->inputShare;
	keyShares[1] = view2->inputShare;

	computeRoundKey2(keyShares, roundKey, 0, params);
	mpc_xor2(state, roundKey, params->stateSizeWords);

	uint32_t *roundConstant[2];

	for (uint32_t r = 1; r <= params->numRounds; ++r) {

		// initialize both roundkeys to 0. they are contingent
		memset(roundKey[0], 0, 2 * params->stateSizeBytes);

		for (int i = 0; i < 2; i++) {
			roundConstant[i] = roundConstants[r - 1];
		}

		computeRoundKey2(keyShares, roundKey, r, params);
		mpc_substitution2(state, tapes, view1, view2, &commBitCount, params);
		mpc_multiplyWithMatrix2(state, r - 1, state, tmp, params);
		mpc_xor2(state, roundConstant, params->stateSizeWords);
		mpc_xor2(state, roundKey, params->stateSizeWords);
	}

	memcpy(view1->outputShare, state[0], params->stateSizeBytes);
	memcpy(view2->outputShare, state[1], params->stateSizeBytes);
}
#endif

void verifyProof(const proof_t *proof, view_t *view1, view_t *view2,
                 uint8_t challenge, uint32_t *slab,
                 const uint32_t *pubInput, const uint8_t iv[16], randomTape_t *tape, lowmcparams_t *params) {

	memcpy(view2->communicatedBits, proof->communicatedBits,
	       params->numAndBytes);

	tape->pos = 0;

	uint8_t *randBuff = malloc(params->stateSizeBytes + params->numAndBytes);
	if (randBuff == NULL) {
		return;
	}

	bool status = false;
	switch (challenge) {
	case 0:
		// in this case, both views' inputs are derivable from the input share

		status = getAllRandomness(proof->seed1, randBuff, params->stateSizeBytes + params->numAndBytes, iv);
		memcpy(tape->tape[0], randBuff, params->numAndBytes);
		memcpy(view1->inputShare, &randBuff[params->numAndBytes], params->stateSizeBytes);
		status = status && getAllRandomness(proof->seed2, randBuff, params->stateSizeBytes + params->numAndBytes, iv);
		if (!status) {
			break;
		}
		memcpy(tape->tape[1], randBuff, params->numAndBytes);
		memcpy(view2->inputShare, &randBuff[params->numAndBytes],
		       params->stateSizeBytes);
		break;

	case 1:
		// in this case view2's input share was already given to us explicitly as
		// it is not computable from the seed. We just need to compute view1's input from
		// its seed
		status = getAllRandomness(proof->seed1, randBuff, params->stateSizeBytes + params->numAndBytes, iv);
		memcpy(tape->tape[0], randBuff, params->numAndBytes);
		memcpy(view1->inputShare, &randBuff[params->numAndBytes], params->stateSizeBytes);
		status = status && getAllRandomness(proof->seed2, tape->tape[1], params->numAndBytes, iv);
		if (!status) {
			break;
		}
		memcpy(view2->inputShare, proof->inputShare,
		       params->stateSizeBytes);
		break;

	case 2:
		// in this case view1's input share was already given to us explicitly as
		// it is not computable from the seed. We just need to compute view2's input from
		// its seed
		status = getAllRandomness(proof->seed1, tape->tape[0], params->numAndBytes, iv);
		memcpy(view1->inputShare, proof->inputShare, params->stateSizeBytes);
		status = status && getAllRandomness(proof->seed2, randBuff, params->stateSizeBytes + params->numAndBytes, iv);
		if (!status) {
			break;
		}
		memcpy(tape->tape[1], randBuff, params->numAndBytes);
		memcpy(view2->inputShare, &randBuff[params->numAndBytes],
		       params->stateSizeBytes);
		break;

	default:
		fprintf(stderr, "%s: Invalid Challenge\n", __func__);
		break;
	}

	if (!status) {
		fprintf(stderr, "%s: Failed to generate random tapes, signature verification will fail (but signature may actually be valid)\n", __func__);
	}

	free(randBuff);

	verifyLowMC(view1, view2, tape, slab, pubInput, params);
}

int verify(signature_t *sig, const uint32_t *pubKey, const uint32_t *pubInput,
           const uint8_t *message, size_t messageByteLength, transform_t transform, lowmcparams_t *params) {

#ifdef WITH_AVX
	uint32_t *pubInput_aligned = _mm_malloc(params->stateSizeBytes, 32);
	memcpy(pubInput_aligned, pubInput, params->stateSizeBytes);
	pubInput = pubInput_aligned;
#endif

	commitments_t *as = malloc(NUM_ZKB_ROUNDS * sizeof(commitments_t));

	uint32_t *viewOutputs[NUM_ZKB_ROUNDS][3];
	const proof_t *proofs = sig->proofs;

	const uint8_t *received_challengebits = sig->challengeBits;
	int status = EXIT_SUCCESS;
	uint8_t *computed_challengebits = NULL;
	uint32_t *view3Slab = NULL;
	uint8_t iv[16];

	generateAesIv(pubKey, pubInput, iv, params);

	view_t *view1s;
	view_t *view2s;
	view_t *views = malloc(2 * NUM_ZKB_ROUNDS * sizeof(view_t));
	view1s = views;
	view2s = views + NUM_ZKB_ROUNDS;
	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		allocateView(&view1s[i], params);
		allocateView(&view2s[i], params);
	}

	uint32_t *slab = (uint32_t *) malloc((6 * params->stateSizeBytes));

	randomTape_t *tape = (randomTape_t *) malloc(sizeof(randomTape_t));
	allocateRandomTape(tape, params);

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {

		//last byte of communicated bits  will not nec get set so need to zero it out
		view1s[i].communicatedBits[params->numAndBytes - 1] = 0;

		verifyProof(&proofs[i], &view1s[i], &view2s[i],
		            getChallenge(received_challengebits, i),
		            slab, pubInput, iv, tape, params);
	}

	unruh_permutations_t *gs = NULL;
	if (transform == TRANSFORM_UR) {
		gs = malloc(NUM_ZKB_ROUNDS * sizeof(unruh_permutations_t));
		for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
			allocateUnruhPermutations(&gs[i], params);
		}
	}

	//create ordered array of commitments with order computed based on the challenge

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		// check commitments of the two opened views
		uint8_t challenge = getChallenge(received_challengebits, i);
		Commit(proofs[i].seed1, view1s[i], as[i].hashes[challenge], params);
		Commit(proofs[i].seed2, view2s[i], as[i].hashes[(challenge + 1) % 3], params);
		memcpy(as[i].hashes[(challenge + 2) % 3], proofs[i].view3Commitment,
		       SHA256_DIGEST_LENGTH);

		if (transform == TRANSFORM_UR) {
			G(challenge, proofs[i].seed1, &view1s[i], gs[i].G[challenge], params);
			G((challenge + 1) % 3, proofs[i].seed2, &view2s[i], gs[i].G[(challenge + 1) % 3], params);
			size_t view3UnruhLength = (challenge == 0) ? params->UnruhGWithInputBytes : params->UnruhGWithoutInputBytes;
			memcpy(gs[i].G[(challenge + 2) % 3], proofs[i].view3UnruhG, view3UnruhLength);
		}
	}

	/* Allocate a slab of memory for the 3rd view's output in each round */
	view3Slab = malloc(params->stateSizeBytes * NUM_ZKB_ROUNDS);
	uint32_t *view3Output = view3Slab; /* pointer into the slab to the current 3rd view */

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		uint8_t challenge = getChallenge(received_challengebits, i);
		viewOutputs[i][challenge] = view1s[i].outputShare;
		viewOutputs[i][(challenge + 1) % 3] = view2s[i].outputShare;
		for (size_t j = 0; j < params->stateSizeWords; j++) {
			view3Output[j] = view1s[i].outputShare[j] ^ view2s[i].outputShare[j] ^ pubKey[j];
		}
		viewOutputs[i][(challenge + 2) % 3] = view3Output;
		view3Output += params->stateSizeWords;
	}

	computed_challengebits = malloc(numBytes(2 * NUM_ZKB_ROUNDS));

	H3(pubKey, viewOutputs, as, NUM_ZKB_ROUNDS,
	   computed_challengebits, message, messageByteLength, transform, gs, params);

	if (computed_challengebits != NULL &&
	    memcmp(received_challengebits, computed_challengebits,
	           numBytes(2 * NUM_ZKB_ROUNDS)) != 0) {
		printf("%s: Invalid signature. Did not verify.\n", __func__);
		status = EXIT_FAILURE;
	}

#ifdef WITH_AVX
	_mm_free(pubInput_aligned);
#endif
	free(computed_challengebits);
	free(view3Slab);

	free(as);
	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		freeView(&view1s[i]);
		freeView(&view2s[i]);
	}
	free(views);
	free(slab);
	freeRandomTape(tape);
	free(tape);
	if (transform == TRANSFORM_UR) {
		for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
			freeUnruhPermutations(&gs[i]);
		}
		free(gs);
	}
	return status;
}

#ifdef WITH_AVX
void mpc_multiplyWithMatrix(__m256i stateVec[3],
                            uint32_t round, __m256i outputVec[3],
                            __m256i temp_vec[3], lowmcparams_t *params) {
	uint32_t *state[3];
	uint32_t i;

	for (i = 0; i < 3; i++) {
		state[i] = ((uint32_t *) &stateVec[i]);
	}

	__m256i *bits_vec[4][3];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 3; k++) {
				bits_vec[j][k] =
				    linearMatricesLookupTableVec[round][i * (sizeof(uint32_t)) + j][(state[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_vec3(temp_vec, bits_vec);
	}

	for (i = 0; i < 3; i++) {
		memcpy(&outputVec[i], &temp_vec[i], params->stateSizeBytes);
	}
}

void mpc_multiplyWithKeyMatrix(uint32_t *key[3], uint32_t round, __m256i output_vec[3], lowmcparams_t *params) {
	uint32_t i;
	__m256i *bits_vec[4][3];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 3; k++) {
				bits_vec[j][k] =
				    keyMatricesLookupTableVec[round][i * (sizeof(uint32_t)) + j][(key[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_vec3(output_vec, bits_vec);
	}
}
#else
void mpc_multiplyWithMatrix(uint32_t *state[3], uint32_t round,
                            uint32_t *output[3], uint32_t *tmp, lowmcparams_t *params) {
	uint32_t i;
	uint32_t *temp[3];

	memset(tmp, 0, params->stateSizeBytes * 3);

	temp[0] = tmp;
	temp[1] = tmp + params->stateSizeWords;
	temp[2] = temp[1] + params->stateSizeWords;

	uint32_t *bits[4][3];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 3; k++) {
				bits[j][k] = linearMatricesLookupTable[round][i * (sizeof(uint32_t)) + j][(state[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_3(temp, bits, params);
	}

	for (i = 0; i < 3; i++) {
		memcpy(output[i], temp[i], params->stateSizeBytes);
	}
}

void mpc_multiplyWithKeyMatrix(uint32_t *key[3], uint32_t round,
                               uint32_t *output[3], lowmcparams_t *params) {
	uint32_t i;
	uint32_t *bits[4][3];

	for (i = 0; i < params->stateSizeWords; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 3; k++) {
				bits[j][k] = keyMatricesLookupTable[round][i * (sizeof(uint32_t)) + j][(key[k][i] >> (24 - (8 * j))) & 0xFF];
			}
		}
		mpc_xor4_3(output, bits, params);
	}
}
#endif

void mpc_AND(uint8_t in1[3], uint8_t in2[3], uint8_t out[3], randomTape_t *rand,
             view_t views[3], uint32_t *yCount) {
	uint8_t r[3] = {getBit(rand->tape[0], rand->pos), getBit(rand->tape[1], rand->pos), getBit(rand->tape[2], rand->pos)};

	(rand->pos)++;

	for (uint8_t i = 0; i < 3; i++) {
		out[i] = (in1[i] & in2[(i + 1) % 3]) ^ (in1[(i + 1) % 3] & in2[i]) ^ (in1[i] & in2[i]) ^ r[i] ^ r[(i + 1) % 3];

		setBit(views[i].communicatedBits, *yCount, out[i]);
	}

	(*yCount)++;
}

#ifdef WITH_AVX
void computeRoundKey(uint32_t *key[3], __m256i output_vec[3], uint32_t round, lowmcparams_t *params) {
	mpc_multiplyWithKeyMatrix(key, round, output_vec, params);
}

void mpc_substitution(__m256i stateVec[3], randomTape_t *rand, view_t views[3],
                      uint32_t *yCount, lowmcparams_t *params) {
	uint32_t *state[3];

	for (int i = 0; i < 3; i++) {
		state[i] = ((uint32_t *) &stateVec[i]);
	}

	uint8_t a[3];
	uint8_t b[3];
	uint8_t c[3];

	uint8_t ab[3];
	uint8_t bc[3];
	uint8_t ca[3];

	for (uint32_t i = 0; i < params->numSboxes * 3; i += 3) {

		for (uint8_t j = 0; j < 3; j++) {
			a[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 2);
			b[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 1);
			c[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i);
		}

		mpc_AND(a, b, ab, rand, views, yCount);
		mpc_AND(b, c, bc, rand, views, yCount);
		mpc_AND(c, a, ca, rand, views, yCount);

		for (uint8_t j = 0; j < 3; j++) {
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 2, a[j] ^ (bc[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 1, a[j] ^ b[j] ^ (ca[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i, a[j] ^ b[j] ^ c[j] ^ (ab[j]));
		}
	}
}

#else
void computeRoundKey(uint32_t *key[3], uint32_t *output[3], uint32_t round, lowmcparams_t *params) {
	mpc_multiplyWithKeyMatrix(key, round, output, params);
}

void mpc_substitution(uint32_t *state[3], randomTape_t *rand, view_t views[3],
                      uint32_t *yCount, lowmcparams_t *params) {
	uint8_t a[3];
	uint8_t b[3];
	uint8_t c[3];

	uint8_t ab[3];
	uint8_t bc[3];
	uint8_t ca[3];

	for (uint32_t i = 0; i < params->numSboxes * 3; i += 3) {

		for (uint8_t j = 0; j < 3; j++) {
			a[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 2);
			b[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i - 1);
			c[j] = getBitFromWordArray(state[j], params->stateSizeBits - 1 - i);
		}

		mpc_AND(a, b, ab, rand, views, yCount);
		mpc_AND(b, c, bc, rand, views, yCount);
		mpc_AND(c, a, ca, rand, views, yCount);

		for (uint8_t j = 0; j < 3; j++) {
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 2, a[j] ^ (bc[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i - 1, a[j] ^ b[j] ^ (ca[j]));
			setBitInWordArray(state[j], params->stateSizeBits - 1 - i, a[j] ^ b[j] ^ c[j] ^ (ab[j]));
		}
	}
}
#endif

#ifdef WITH_AVX
void mpc_LowMC(randomTape_t *tapes, view_t views[3],
               uint32_t *countY, const uint32_t *pubInput, uint32_t *slab, lowmcparams_t *params) {
	uint32_t *keyShares[3];

	__m256i roundKey[3];
	__m256i state[3];
	__m256i temp[3];
	__m256i roundConstant[3];

	/* The slab parameter is unused, but we must keep it so the AVX
     * and non-AVX versions of this function have the same signature */
	UNUSED_PARAMETER(slab);

	memset(&roundKey[0], 0, 3 * params->stateSizeBytes);

	for (int i = 0; i < 3; i++) {
		keyShares[i] = views[i].inputShare;
		state[i] = _mm256_load_si256((__m256i *) pubInput);
	}

	computeRoundKey(keyShares, roundKey, 0, params);
	mpc_xor_vec3(state, roundKey, state);

	for (uint32_t r = 1; r <= params->numRounds; r++) {
		memset(&roundKey[0], 0, 3 * params->stateSizeBytes);

		roundConstant[0] = roundConstant[1] = roundConstant[2] = roundConstantsVec[r - 1][0];

		computeRoundKey(keyShares, roundKey, r, params);
		mpc_substitution(state, tapes, views, countY, params);
		memset(&temp[0], 0, params->stateSizeBytes);
		memset(&temp[1], 0, params->stateSizeBytes);
		memset(&temp[2], 0, params->stateSizeBytes);
		mpc_multiplyWithMatrix(state, r - 1, state, temp, params);
		mpc_xor_vec3(state, roundConstant, state);
		mpc_xor_vec3(state, roundKey, state);
	}
	for (int i = 0; i < 3; i++) {
		memcpy(views[i].outputShare, &state[i], params->stateSizeBytes);
	}
}
#else
void mpc_LowMC(randomTape_t *tapes, view_t views[3],
               uint32_t *countY, const uint32_t *pubInput, uint32_t *slab, lowmcparams_t *params) {
	uint32_t *keyShares[3];
	uint32_t *state[3];
	uint32_t *roundKey[3];
	uint32_t *roundConstant[3];

	roundKey[0] = slab;
	roundKey[1] = slab + params->stateSizeWords;
	roundKey[2] = roundKey[1] + params->stateSizeWords;
	state[0] = roundKey[2] + params->stateSizeWords;
	state[1] = state[0] + params->stateSizeWords;
	state[2] = state[1] + params->stateSizeWords;
	uint32_t *tmp = state[2] + params->stateSizeWords;

	memset(roundKey[0], 0, 3 * params->stateSizeBytes);
	for (int i = 0; i < 3; i++) {
		keyShares[i] = views[i].inputShare;
		memcpy(state[i], pubInput, params->stateSizeBytes);
	}

	computeRoundKey(keyShares, roundKey, 0, params);
	mpc_xor(state, roundKey, params->stateSizeWords);

	for (uint32_t r = 1; r <= params->numRounds; r++) {

		memset(roundKey[0], 0, 3 * params->stateSizeBytes);

		for (int i = 0; i < 3; i++) {
			roundConstant[i] = roundConstants[r - 1];
		}
		computeRoundKey(keyShares, roundKey, r, params);
		mpc_substitution(state, tapes, views, countY, params);
		mpc_multiplyWithMatrix(state, r - 1, state, tmp, params);
		mpc_xor(state, roundConstant, params->stateSizeWords);
		mpc_xor(state, roundKey, params->stateSizeWords);
	}

	for (int i = 0; i < 3; i++) {
		memcpy(views[i].outputShare, state[i], params->stateSizeBytes);
	}
}
#endif

void runMPC(view_t views[3], randomTape_t *rand,
            uint32_t *pubInput, uint32_t *slab, lowmcparams_t *params) {
	rand->pos = 0;
	uint32_t commBitCount = 0;
	mpc_LowMC(rand, views, &commBitCount, pubInput, slab, params);
}

int random_bytes_default_openssl(uint8_t *buf, size_t len) {
	/*
     * Returns 1 on success, other value on error
     * The error can be obtained with ERR_get_error()
     * https://wiki.openssl.org/index.php/Random_Numbers#Software
     */
	if (len > INT_MAX) {
		return 0;
	}
	return RAND_bytes(buf, (int) len);
}

int sign(uint32_t *privateKey, uint32_t *pubKey, uint32_t *pubInput, const uint8_t *message,
         size_t messageByteLength, signature_t *sig, transform_t transform, lowmcparams_t *params) {
#ifdef WITH_AVX
	uint32_t *pubInput_aligned = _mm_malloc(params->stateSizeBytes, 32);
	memcpy(pubInput_aligned, pubInput, params->stateSizeBytes);
	pubInput = pubInput_aligned;
#endif

	// these are 256 bit seeds to AES256 which we use as our PRG.
	uint8_t(*seeds)[3][PRG_SEED_LENGTH] = malloc(NUM_ZKB_ROUNDS * sizeof(*seeds));

	commitments_t *as = malloc(NUM_ZKB_ROUNDS * sizeof(commitments_t));

	// 3 views per round
	view_t **views = malloc(NUM_ZKB_ROUNDS * sizeof(view_t *));
	if (views == NULL) {
		fprintf(stderr, "out of memory\n");
		return EXIT_FAILURE;
	}
	for (int i = 0; i < NUM_ZKB_ROUNDS; i++) {
		views[i] = malloc(3 * sizeof(view_t));
		if (views[i] == NULL) {
			fprintf(stderr, "out of memory\n");
			return EXIT_FAILURE;
		}
	}

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		for (size_t j = 0; j < 3; j++) {
			allocateView(&views[i][j], params);

			//last byte of communiated bits  will not nec get set so need to zero it out
			views[i][j].communicatedBits[params->numAndBytes - 1] = 0;

			// Generate random seeds
			if (picnic_random_bytes(seeds[i][j], PRG_SEED_LENGTH) != 1) {
				fprintf(stderr, "%s: failed to generate random bytes\n", __func__);
				return EXIT_FAILURE;
			}
		}
	}

	//Generating randomness for mpc
	randomTape_t *tape = malloc(NUM_ZKB_ROUNDS * sizeof(randomTape_t));
	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		allocateRandomTape(&tape[i], params);
	}
	uint8_t *randBuff = malloc(params->stateSizeBytes + params->numAndBytes);
	if (randBuff == NULL) {
		return EXIT_FAILURE;
	}
	uint32_t *slab = NULL;
	uint8_t iv[16];
	generateAesIv(pubKey, pubInput, iv, params);
#if !defined(WITH_AVX)
	slab = (uint32_t *) malloc(9 * params->stateSizeBytes);
#endif

	unruh_permutations_t *gs = NULL;
	if (transform == TRANSFORM_UR) {
		gs = malloc(NUM_ZKB_ROUNDS * sizeof(unruh_permutations_t));
		for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
			allocateUnruhPermutations(&gs[i], params);
		}
	}

	bool status;
	for (int k = 0; k < NUM_ZKB_ROUNDS; k++) {
		// for first two players get all tape INCLUDING INPUT SHARE from seed
		for (int j = 0; j < 2; j++) {
			status = getAllRandomness(seeds[k][j], randBuff, params->stateSizeBytes + params->numAndBytes, iv);
			if (!status) {
				fprintf(stderr, "%s: getAllRandomness failed \n", __func__);
				return EXIT_FAILURE;
			}

			memcpy(tape[k].tape[j], randBuff, params->numAndBytes);
			memcpy(views[k][j].inputShare, &randBuff[params->numAndBytes], params->stateSizeBytes);
		}
		// now set third party's wire. We just get the random bits from the seed, but the input is
		// the XOR of other two inputs and the private key
		status = getAllRandomness(seeds[k][2], tape[k].tape[2], params->numAndBytes, iv);
		if (!status) {
			fprintf(stderr, "%s: getAllRandomness failed \n", __func__);
			return EXIT_FAILURE;
		}

		for (uint32_t j = 0; j < params->stateSizeWords; j++) {
			views[k][2].inputShare[j] = privateKey[j] ^ views[k][0].inputShare[j] ^ views[k][1].inputShare[j];
		}
	}

	//Running MPC
	for (int k = 0; k < NUM_ZKB_ROUNDS; k++) {
		runMPC(views[k], &tape[k],
		       pubInput, slab, params);
	}

	//Committing
	for (int k = 0; k < NUM_ZKB_ROUNDS; k++) {
		Commit(seeds[k][0], views[k][0], as[k].hashes[0], params);
		Commit(seeds[k][1], views[k][1], as[k].hashes[1], params);
		Commit(seeds[k][2], views[k][2], as[k].hashes[2], params);
	}

	if (transform == TRANSFORM_UR) {
		for (size_t k = 0; k < NUM_ZKB_ROUNDS; k++) {
			G(0, seeds[k][0], &views[k][0], gs[k].G[0], params);
			G(1, seeds[k][1], &views[k][1], gs[k].G[1], params);
			G(2, seeds[k][2], &views[k][2], gs[k].G[2], params);
		}
	}

	//Generating challenges
	uint32_t *viewOutputs[NUM_ZKB_ROUNDS][3];

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		for (size_t j = 0; j < 3; j++) {
			viewOutputs[i][j] = views[i][j].outputShare;
		}
	}

	uint32_t output[LOWMC_MAX_STATE_SIZE];
	for (uint32_t j = 0; j < params->stateSizeWords; j++) {
		output[j] = viewOutputs[0][0][j] ^ viewOutputs[0][1][j] ^ viewOutputs[0][2][j];
	}

	H3(output, viewOutputs, as, NUM_ZKB_ROUNDS,
	   sig->challengeBits, message, messageByteLength, transform, gs, params);

	//Packing Z

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		proof_t *proof = &sig->proofs[i];
		prove(proof, getChallenge(sig->challengeBits, i), seeds[i],
		      views[i], &as[i], (gs == NULL) ? NULL : &gs[i], params);
	}
#ifdef WITH_AVX
	_mm_free(pubInput_aligned);
#else
	free(slab);
#endif

	free(randBuff);
	free(seeds);

	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		for (size_t j = 0; j < 3; j++) {
			freeView(&views[i][j]);
		}
		free(views[i]);
	}

	free(views);
	free(as);
	for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
		freeRandomTape(&tape[i]);
	}
	free(tape);
	if (transform == TRANSFORM_UR) {
		for (size_t i = 0; i < NUM_ZKB_ROUNDS; i++) {
			freeUnruhPermutations(&gs[i]);
		}
		free(gs);
	}

	return EXIT_SUCCESS;
}
