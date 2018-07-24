#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>

void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
	size_t i;
	fprintf(fp, "%s", S);
	for (i = 0; i < L; i++) {
		fprintf(fp, "%02X", A[i]);
	}
	if (L == 0) {
		fprintf(fp, "00");
	}
	fprintf(fp, "\n");
}

static OQS_STATUS sig_kat(const char *method_name) {

	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	size_t mlen = 33, smlen;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signed_message = NULL;
	char filename[200];
	OQS_STATUS rc, ret = OQS_ERROR;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[sig_kat] %s was not enabled at compile-time.\n", method_name);
		goto err;
	}

	for (size_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS) {
		goto err;
	}
	OQS_randombytes_nist_kat_init(entropy_input, NULL, 256);

	sprintf(filename, "kat_sig_rsp/%s.kat", method_name);

	fh = fopen(filename, "w");
	if (fh == NULL) {
		goto err;
	}

	fprintf(fh, "count = 0\n");
	OQS_randombytes(seed, 48);
	fprintBstr(fh, "seed = ", seed, 48);

	fprintf(fh, "mlen = %lu\n", mlen);
	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	message = malloc(mlen + sig->length_sig_overhead);
	signed_message = malloc(mlen + sig->length_sig_overhead);
	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signed_message == NULL)) {
		fprintf(stderr, "[kat_sig] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}
	OQS_randombytes(message, mlen);
	fprintBstr(fh, "msg = ", message, mlen);

	OQS_randombytes_nist_kat_init(seed, NULL, 256);

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "pk = ", public_key, sig->length_public_key);
	fprintBstr(fh, "sk = ", secret_key, sig->length_secret_key);

	rc = OQS_SIG_sign(sig, signed_message, &smlen, message, mlen, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_sign failed!\n", method_name);
		goto err;
	}
	fprintf(fh, "smlen = %lu\n", smlen);
	fprintBstr(fh, "sm = ", signed_message, smlen);

	rc = OQS_SIG_sign_open(sig, message, &mlen, signed_message, smlen, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_sign_open failed!\n", method_name);
		goto err;
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (fh != NULL) {
		fclose(fh);
	}
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(signed_message);
	OQS_SIG_free(sig);
	return ret;
}

int main() {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	int status;
	status = mkdir("kat_sig_rsp", S_IRWXU);
	if (!((status == 0) || (errno == EEXIST))) {
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
		rc = sig_kat(OQS_SIG_alg_identifier(i));
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	}

	return ret;
}
