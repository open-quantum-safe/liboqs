// SPDX-License-Identifier: MIT

// This KAT test only generates a subset of the NIST KAT files.
// To extract the subset from a submission file, use the command:
//     cat PQCkemKAT_whatever.rsp | head -n 8 | tail -n 6

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>
#include <oqs/rand_nist.h>
#include <oqs/sha3.h>

#include "test_helpers.h"

#include "system_info.c"

static OQS_STATUS kem_kat(const char *method_name, bool all) {

	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;
	size_t max_count;
	OQS_KAT_PRNG *prng;

	prng = OQS_KAT_PRNG_new(method_name);
	if (prng == NULL) {
		goto err;
	}

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[kem_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	for (uint8_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	OQS_KAT_PRNG_seed(prng, entropy_input, NULL);

	fh = stdout;

	public_key = OQS_MEM_malloc(kem->length_public_key);
	secret_key = OQS_MEM_malloc(kem->length_secret_key);
	ciphertext = OQS_MEM_malloc(kem->length_ciphertext);
	shared_secret_e = OQS_MEM_malloc(kem->length_shared_secret);
	shared_secret_d = OQS_MEM_malloc(kem->length_shared_secret);
	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "[kat_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	max_count = all ? prng->max_kats : 1;

	for (size_t count = 0; count < max_count; ++count) {
		fprintf(fh, "count = %zu\n", count);
		OQS_randombytes(seed, 48);
		OQS_fprintBstr(fh, "seed = ", seed, 48);

		OQS_KAT_PRNG_save_state(prng);
		OQS_KAT_PRNG_seed(prng, seed, NULL);

		rc = OQS_KEM_keypair(kem, public_key, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
			goto err;
		}
		OQS_fprintBstr(fh, "pk = ", public_key, kem->length_public_key);
		OQS_fprintBstr(fh, "sk = ", secret_key, kem->length_secret_key);

		rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
			goto err;
		}
		OQS_fprintBstr(fh, "ct = ", ciphertext, kem->length_ciphertext);
		OQS_fprintBstr(fh, "ss = ", shared_secret_e, kem->length_shared_secret);

		// The NIST program generates KAT response files with a trailing newline.
		if (count != max_count - 1) {
			fprintf(fh, "\n");
		}

		rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_decaps failed!\n", method_name);
			goto err;
		}

		rv = memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret);
		if (rv != 0) {
			fprintf(stderr, "[kat_kem] %s ERROR: shared secrets are not equal\n", method_name);
			OQS_print_hex_string("shared_secret_e", shared_secret_e, kem->length_shared_secret);
			OQS_print_hex_string("shared_secret_d", shared_secret_d, kem->length_shared_secret);
			goto err;
		}

		OQS_KAT_PRNG_restore_state(prng);
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
		OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ciphertext);
	OQS_KEM_free(kem);
	OQS_KAT_PRNG_free(prng);
	return ret;
}

int main(int argc, char **argv) {

	OQS_init();
	if (argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[2], "--all"))) {
		fprintf(stderr, "Usage: kat_kem algname [--all]\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	bool all = (argc == 3);
	OQS_STATUS rc = kem_kat(alg_name, all);
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
