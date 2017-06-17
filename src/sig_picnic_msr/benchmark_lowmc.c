/*! @file benchmark_lowmc.c
 *  @brief Benchmarking code for the Picnic and Fish signature schemes. KeyGen,
 *  sign, and verify are benchmarked both directly as well as using the
 *  signature API.
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

#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "LowMCEnc.h"
#include "LowMC.h"
#include "test_util.h"
#include <immintrin.h>

#include "picnic.h"

#define NUM_ITER 1
#define t 8

#define MESSAGE_LEN 50

#define TRANSFORM_STRING(tr) ((tr == TRANSFORM_UR) ? "UR" : "FS")

/* function defined in picnic.c, used here for testing, but not part of the Picnic API */
int picnicParamsToLowMCParams(picnic_params_t picnicParams, lowmcparams_t *lowmcParams);

/* nameOfConfig is just used to print the results nicely and can be anything, but
 * should be a recognizable name of the circuit used */
void benchmarkPicnicDirect(const char *nameOfConfig, transform_t transform, picnic_params_t params) {
	lowmcparams_t lowmcparams;

	if (picnicParamsToLowMCParams(params, &lowmcparams) != EXIT_SUCCESS) {
		exit(-1);
	}

	uint32_t *privateKey = calloc(lowmcparams.stateSizeWords, sizeof(uint32_t));
	privateKey[lowmcparams.stateSizeWords - 1] = 1;
	uint32_t *pubInput = calloc(lowmcparams.stateSizeWords, sizeof(uint32_t));
	pubInput[lowmcparams.stateSizeWords - 1] = 1;

	uint8_t message[MESSAGE_LEN];
	memset(&message, 0, sizeof(message));

	if (readLookupTables(&lowmcparams, NULL) != 0) {
		fprintf(stdout, "Initialization failed (reading look-up tables).\n");
		fflush(stdout);
		exit(-1);
	}

	uint64_t cycles_begin, cycles_end;

	/* Generate key pair */
	uint32_t *publicKey = (uint32_t *) malloc(lowmcparams.stateSizeWords * sizeof(uint32_t));
	signature_t *signature = malloc(sizeof(signature_t));
	allocateSignature(signature, &lowmcparams);

	printf("\nPicnic direct benchmark: running keyGen, sign & verify for %d iterations of %d rounds each \n",
	       NUM_ITER, NUM_ZKB_ROUNDS);

	cycles_begin = cpucycles();
	for (int i = 0; i < NUM_ITER; i++) {
		LowMCEnc(pubInput, publicKey, privateKey, &lowmcparams);
	}
	cycles_end = cpucycles();

	char label[100];
	sprintf(label, "Time for [%s-%s] keyGeneration (LowMCEnc())", nameOfConfig, TRANSFORM_STRING(transform));
	bench_print(label, (cycles_end - cycles_begin), NUM_ITER); /* The 3rd param is the number of runs*/

	cycles_begin = cpucycles();
	for (int i = 0; i < NUM_ITER; i++) {
		sign(privateKey, publicKey, pubInput, message, sizeof(message), signature, transform, &lowmcparams);
	}
	cycles_end = cpucycles();

	sprintf(label, "Time for [%s-%s] sign()", nameOfConfig, TRANSFORM_STRING(transform));
	bench_print(label, (cycles_end - cycles_begin), NUM_ITER); /* The 3rd param is the number of runs*/

	cycles_begin = cpucycles();
	for (int i = 0; i < NUM_ITER; i++) {
		verify(signature, publicKey, pubInput, message,
		       sizeof(message), transform, &lowmcparams);
	}
	cycles_end = cpucycles();

	sprintf(label, "Time for [%s-%s] verify()", nameOfConfig, TRANSFORM_STRING(transform));
	bench_print(label, (cycles_end - cycles_begin), NUM_ITER);

	freeSignature(signature);
	free(signature);
	free(publicKey);
	free(privateKey);
	free(pubInput);
	freeLookupTables();
}

void benchmarkPicnicAPI(char *nameOfConfig, transform_t transform, picnic_params_t params) {
	picnic_publickey_t pk;
	picnic_privatekey_t sk;
	uint64_t cycles_begin, cycles_end;
	char label[100];

	printf("\n Picnic API benchmark: running keyGen, sign & verify for %d iterations of %d rounds each \n",
	       NUM_ITER, NUM_ZKB_ROUNDS);

	if (picnic_init(params, NULL, 0) != 0) {
		fprintf(stdout, "Initialization failed.\n");
		fflush(stdout);
		exit(-1);
	}

	/* Key generation */
	cycles_begin = cpucycles();
	for (int i = 0; i < NUM_ITER; i++) {
		(void) picnic_keygen(params, &pk, &sk);
	}
	cycles_end = cpucycles();

	sprintf(label, "Time for [%s-%s] picnic_keygen", nameOfConfig, TRANSFORM_STRING(transform));
	bench_print(label, (cycles_end - cycles_begin), NUM_ITER);

	/* Sign */
	uint8_t message[MESSAGE_LEN];
	memset(&message, 0, sizeof(message));
	uint8_t *signature = NULL;

	size_t signature_len = picnic_signature_size(params);
	signature = (uint8_t *) malloc(signature_len);
	if (signature == NULL) {
		printf("failed to allocate signature\n");
		exit(-1);
	}

	size_t av_sig_len = 0;
	cycles_begin = cpucycles();
	for (int i = 0; i < NUM_ITER; i++) {
		/* Since the sign API outputs the number of signature bytes, re-set it
         * to the size of the buffer every iteration */
		size_t siglen = signature_len;

		(void) picnic_sign(&sk, message, sizeof(message), signature, &siglen);
		av_sig_len += siglen;
	}
	cycles_end = cpucycles();

	sprintf(label, "Time for [%s-%s] picnic_sign", nameOfConfig, TRANSFORM_STRING(transform));
	bench_print(label, (cycles_end - cycles_begin), NUM_ITER);

	/* Verify */
	cycles_begin = cpucycles();
	for (int i = 0; i < NUM_ITER; i++) {
		(void) picnic_verify(&pk, message, sizeof(message), signature,
		                     signature_len);
	}
	cycles_end = cpucycles();

	sprintf(label, "Time for [%s-%s] picnic_verify", nameOfConfig, TRANSFORM_STRING(transform));
	bench_print(label, (cycles_end - cycles_begin), NUM_ITER);

	if (transform == TRANSFORM_UR) {
		printf("  Signature size (exact): %u bytes\n", (unsigned int) (av_sig_len / NUM_ITER));
	} else if (transform == TRANSFORM_FS) {
		unsigned int av = ((unsigned int) (av_sig_len / NUM_ITER)) + ((av_sig_len % NUM_ITER) != 0);
		printf("  Signature size (average): %u bytes\n", av);
	}

	printf("\n");

	free(signature);
	picnic_shutdown(0);
}

void parseOptions(int argc, char **argv, transform_t *transform) {
	for (int i = 1; i < argc; i++) {
		if (strcmp("-u", argv[i]) == 0) {
			*transform = TRANSFORM_UR;
		}
	}
}

int serializationTest(transform_t transform, picnic_params_t params) {
	int ret = -1;
	lowmcparams_t lowmcparams = {0};
	signature_t signature;
	signature_t signature2;
	uint32_t *privateKey = NULL;
	uint32_t *pubInput = NULL;

	if (picnicParamsToLowMCParams(params, &lowmcparams) != EXIT_SUCCESS) {
		return -1;
	}

	uint32_t *publicKey = (uint32_t *) malloc(lowmcparams.stateSizeWords * sizeof(uint32_t));
	uint8_t message[MESSAGE_LEN];
	memset(&message, 0, sizeof(message));

	if (readLookupTables(&lowmcparams, NULL) != 0) {
		fprintf(stdout, "Initialization failed (reading look-up tables).\n");
		ret = -1;
		goto Exit;
	}
	/* Generate key pair */
	privateKey = calloc(lowmcparams.stateSizeWords, sizeof(uint32_t));
	pubInput = calloc(lowmcparams.stateSizeWords, sizeof(uint32_t));

	LowMCEnc(pubInput, publicKey, privateKey, &lowmcparams);

	/* Create a signature to serialize */
	allocateSignature(&signature, &lowmcparams);
	allocateSignature(&signature2, &lowmcparams);
	sign(privateKey, publicKey, pubInput, message, sizeof(message), &signature, transform, &lowmcparams);

	uint8_t sigBytes[PICNIC_MAX_SIGNATURE_SIZE];
	memset(sigBytes, 0, sizeof(sigBytes));
	ret = serializeSignature(&signature, sigBytes, sizeof(sigBytes), transform, &lowmcparams);
	if (ret == -1) {
		fprintf(stderr, "Failed to serialize signature\n");
		ret = -1;
		goto Exit;
	}

	ret = deserializeSignature(&signature2, sigBytes, sizeof(sigBytes), transform, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to deserialize signature\n");
		ret = -1;
		goto Exit;
	}

	uint8_t sigBytes2[sizeof(sigBytes)];
	memset(sigBytes2, 0, sizeof(sigBytes2));
	ret = serializeSignature(&signature2, sigBytes2, sizeof(sigBytes2), transform, &lowmcparams);
	if (ret == -1) {
		fprintf(stderr, "Failed to serialize signature 2\n");
		ret = -1;
		goto Exit;
	}

	if (memcmp(sigBytes, sigBytes2, sizeof(sigBytes2)) != 0) {
		fprintf(stderr, "Serialization roundtrip failed\n");
		ret = -1;
		goto Exit;
	}

	ret = verify(&signature2, publicKey, pubInput, message, sizeof(message), transform, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to verify deserialized signature\n");
		ret = -1;
		goto Exit;
	}

	ret = 0; /* Success */

Exit:
	freeSignature(&signature);
	freeSignature(&signature2);
	free(publicKey);
	free(privateKey);
	free(pubInput);
	freeLookupTables();

	return ret;
}

void runBenchmarks(picnic_params_t params, transform_t transform) {

	printf("\nBenchmarking Picnic with %s\n", picnic_get_param_name(params));

	if (serializationTest(transform, params) != 0) {
		printf("Failed serialization test\n");
		exit(-1);
	}

	/* Benchmark Picnic by calling the sign/verify functions directly (as
     * opposed to using the public API) */
	benchmarkPicnicDirect("LowMC", transform, params);

	/* Benchmark functions in the public API */
	benchmarkPicnicAPI("LowMC", transform, params);
}

int main(int argc, char **argv) {
	transform_t transform = TRANSFORM_FS;

	parseOptions(argc, argv, &transform);

	printf("Running benchmarks\n");

	// benchmark the given transform with each of the 3 supported LowMC parameter sets
	picnic_params_t params;
	params = (transform == TRANSFORM_UR) ? LowMC_256_256_42_14_UR : LowMC_256_256_42_14_FS;
	runBenchmarks(params, transform);

	params = (transform == TRANSFORM_UR) ? LowMC_256_256_1_316_UR : LowMC_256_256_1_316_FS;
	runBenchmarks(params, transform);

	params = (transform == TRANSFORM_UR) ? LowMC_256_256_10_38_UR : LowMC_256_256_10_38_FS;
	runBenchmarks(params, transform);
}
