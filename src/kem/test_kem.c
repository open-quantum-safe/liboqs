#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

struct kem_testcase {
	enum OQS_KEM_alg_name alg_name;
	int iterations;
};

struct kem_testcase kem_testcases[] = {
	{OQS_KEM_alg_dummy1, 100},
	{OQS_KEM_alg_dummy2, 100},
};

static OQS_STATUS kem_test_correctness(enum OQS_KEM_alg_name alg_name) {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	kem = OQS_KEM_new(alg_name);
	if (kem == NULL) {
		return OQS_SUCCESS;
	}

	printf("================================================================================\n");
	printf("Sample computation for KEM %s\n", kem->method_name);
	printf("================================================================================\n");

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ciphertext = malloc(kem->length_ciphertext);
	shared_secret_e = malloc(kem->length_shared_secret);
	shared_secret_d = malloc(kem->length_shared_secret);

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed\n");
		goto err;
	}

	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed\n");
		goto err;
	}

	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed\n");
		goto err;
	}

	rv = memcmp(shared_secret_e, shared_secret_d, kem->length_secret_key);
	if (rc != 0) {
		fprintf(stderr, "ERROR: shared secrets are not equal\n");
		goto err;
	} else {
		printf("shared secrets are equal\n");
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	free(public_key);
	free(secret_key);
	free(ciphertext);
	free(shared_secret_e);
	free(shared_secret_d);
	OQS_KEM_free(kem);

	return ret;
}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;
	size_t num_kex_testcases = sizeof(kem_testcases) / sizeof(struct kem_testcase);

	for (size_t i = 0; i < num_kex_testcases; i++) {
		struct kem_testcase testcase = kem_testcases[i];
		rc = kem_test_correctness(testcase.alg_name);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	}

	return ret;
}
