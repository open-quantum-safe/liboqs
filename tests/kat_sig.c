// This KAT test only generates a subset of the NIST KAT files.
// To extract the subset from a submission file, use the command:
//     cat PQCsignKAT_whatever.rsp | head -n 10 | tail -n 8

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>

/* Displays hexadecimal strings */
void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", ((unsigned char *) (str))[i]);
	}
	printf("\n");
}

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

OQS_STATUS combine_message_signature(uint8_t **signed_msg, size_t *signed_msg_len, const uint8_t *msg, size_t msg_len, const uint8_t *signature, size_t signature_len, const OQS_SIG *sig) {
	if (0) {
	} else if (0 == strcmp(sig->method_name, "DILITHIUM_2")) {
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
}

OQS_STATUS sig_kat(const char *method_name) {

	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	uint8_t *msg = NULL;
	size_t msg_len = 0;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *signature = NULL;
	uint8_t *signed_msg = NULL;
	size_t signature_len = 0;
	size_t signed_msg_len = 0;
	OQS_STATUS rc, ret = OQS_ERROR;
	// int rv;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[sig_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	for (size_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS) {
		goto err;
	}
	OQS_randombytes_nist_kat_init(entropy_input, NULL, 256);

	fh = stdout;

	fprintf(fh, "count = 0\n");
	OQS_randombytes(seed, 48);
	fprintBstr(fh, "seed = ", seed, 48);

	msg_len = 33 * (0 + 1);
	fprintf(fh, "mlen = %zu\n", msg_len);
	
	msg = malloc(msg_len);
	OQS_randombytes(msg, msg_len);
	fprintBstr(fh, "msg = ", msg, msg_len);

	OQS_randombytes_nist_kat_init(seed, NULL, 256);

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	signature = malloc(sig->length_signature);
	if ((public_key == NULL) || (secret_key == NULL) || (signature == NULL)) {
		fprintf(stderr, "[kat_sig] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}
	
	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "pk = ", public_key, sig->length_public_key);
	fprintBstr(fh, "sk = ", secret_key, sig->length_secret_key);
	
	rc = OQS_SIG_sign(sig, signature, &signature_len, msg, msg_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_sign failed!\n", method_name);
		goto err;
	}
	rc = combine_message_signature(&signed_msg, &signed_msg_len, msg, msg_len, signature, signature_len, sig);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: combine_message_signature failed!\n", method_name);
		goto err;
	}
	fprintf(fh, "smlen = %zu\n", signed_msg_len);
	fprintBstr(fh, "sm = ", signed_msg, signed_msg_len);
	
	rc = OQS_SIG_verify(sig, msg, msg_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_verify failed!\n", method_name);
		goto err;
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
		OQS_MEM_secure_free(signed_msg, signed_msg_len);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signature);
	OQS_MEM_insecure_free(msg);
	OQS_SIG_free(sig);
	return ret;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Usage: kat_sig algname\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	OQS_STATUS rc = sig_kat(alg_name);
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
