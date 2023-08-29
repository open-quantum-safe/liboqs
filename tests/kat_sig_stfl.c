// SPDX-License-Identifier: MIT

// This KAT test only generates a subset of the NIST KAT files.
// To extract the subset from a submission file, use the command:

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include <oqs/oqs.h>

#include "system_info.c"

#define MAX_MARKER_LEN 50

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int FindMarker(FILE *infile, const char *marker) {
	char line[MAX_MARKER_LEN];
	unsigned long i, len;
	int curr_line;

	len = strlen(marker);
	if (len > MAX_MARKER_LEN - 1) {
		len = MAX_MARKER_LEN - 1;
	}

	for (i = 0; i < len; i++) {
		curr_line = fgetc(infile);
		line[i] = (char)curr_line;
		if (curr_line == EOF) {
			return 0;
		}
	}
	line[len] = '\0';

	while (1) {
		if (!strncmp(line, marker, len)) {
			return 1;
		}

		for (i = 0; i < len - 1; i++) {
			line[i] = line[i + 1];
		}
		curr_line = fgetc(infile);
		line[len - 1] = (char)curr_line;
		if (curr_line == EOF) {
			return 0;
		}
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int ReadHex(FILE *infile, unsigned char *a, unsigned long Length, char *str) {
	int ch, started;
	unsigned long i;
	unsigned char ich;

	if (Length == 0) {
		a[0] = 0x00;
		return 1;
	}
	memset(a, 0x00, Length);
	started = 0;
	if (FindMarker(infile, str))
		while ((ch = fgetc(infile)) != EOF) {
			if (!isxdigit(ch)) {
				if (!started) {
					if (ch == '\n') {
						break;
					} else {
						continue;
					}
				} else {
					break;
				}
			}
			started = 1;
			if ((ch >= '0') && (ch <= '9')) {
				ich = (unsigned char)ch - '0';
			} else if ((ch >= 'A') && (ch <= 'F')) {
				ich = (unsigned char)ch - 'A' + 10;
			} else if ((ch >= 'a') && (ch <= 'f')) {
				ich = (unsigned char)ch - 'a' + 10;
			} else {
				// shouldn't ever get here
				ich = 0;
			}

			for (i = 0; i < Length - 1; i++) {
				a[i] = (unsigned char) (a[i] << 4) | (unsigned char) (a[i + 1] >> 4);
			}
			a[Length - 1] = (unsigned char) (a[Length - 1] << 4) | (unsigned char) ich;
		} else {
		return 0;
	}

	return 1;
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

OQS_STATUS sig_stfl_kat(const char *method_name, const char *katfile) {

	uint8_t seed[48];
	FILE *fh = NULL;
	FILE *fp_rsp = NULL;
	OQS_SIG_STFL *sig = NULL;
	uint8_t *msg = NULL, *msg_rand = NULL;
	size_t msg_len = 0;
	uint8_t *public_key = NULL;
	OQS_SIG_STFL_SECRET_KEY *secret_key = NULL;
	uint8_t *signature = NULL, *signature_kat = NULL;
	uint8_t *signed_msg = NULL;
	size_t signature_len = 0;
	size_t signed_msg_len = 0;
	unsigned long long sigs_remain = 0;
	unsigned long long sigs_maximum = 0;
	OQS_STATUS rc, ret = OQS_ERROR;

	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "[sig_stfl_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS) {
		goto err;
	}

	if ((fp_rsp = fopen(katfile, "r")) == NULL) {
		fprintf(stderr, "Couldn't open <%s> for read\n", katfile);
		return OQS_ERROR;
	}

	// Grab the pk and sk from KAT file
	public_key = malloc(sig->length_public_key);
	secret_key = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);
	signature = calloc(sig->length_signature, sizeof(uint8_t));
	signature_kat = calloc(sig->length_signature, sizeof(uint8_t));

	if ((public_key == NULL) || (secret_key == NULL) || (signature == NULL)) {
		fprintf(stderr, "[kat_stfl_sig] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}

	if (!ReadHex(fp_rsp, public_key, sig->length_public_key, "pk = ")) {
		fprintf(stderr, "ERROR: unable to read 'pk' from <%s>\n", katfile);
		goto err;
	}

	if (!ReadHex(fp_rsp, secret_key->secret_key_data, sig->length_secret_key, "sk = ")) {
		fprintf(stderr, "ERROR: unable to read 'sk' from <%s>\n", katfile);
		goto err;
	}

	fh = stdout;
	fprintf(fh, "# %s\n\n", sig->method_name);

	fprintBstr(fh, "pk = ", public_key, sig->length_public_key);
	fprintBstr(fh, "sk = ", secret_key->secret_key_data, sig->length_secret_key);
	fprintf(fh, "\n\n");

	fprintf(fh, "count = 0\n");
	if (!ReadHex(fp_rsp, seed, 48, "seed = ")) {
		fprintf(stderr, "ERROR: unable to read 'seed' from <%s>\n", katfile);
		goto err;
	}

	fprintBstr(fh, "seed = ", seed, 48);
	OQS_randombytes_nist_kat_init_256bit(seed, NULL);

	msg_len = 33 * (0 + 1);
	fprintf(fh, "mlen = %zu\n", msg_len);
	msg = malloc(msg_len);
	msg_rand = malloc(msg_len);

	if (!ReadHex(fp_rsp, msg, msg_len, "msg = ")) {
		fprintf(stderr, "ERROR: unable to read 'msg' from <%s>\n", katfile);
		goto err;
	}

	OQS_randombytes(msg_rand, msg_len);

	if (memcmp(msg_rand, msg, msg_len)) {
		fprintf(stderr, "randombytes data unaligned\n");
		fprintBstr(fh, "m = ", msg, msg_len);
		fprintBstr(fh, "m_rand = ", msg_rand, msg_len);
		goto err;
	}

	fprintBstr(fh, "msg = ", msg, msg_len);

	rc = OQS_SIG_STFL_sign(sig, signature, &signature_len, msg, msg_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_stfl_sig] %s ERROR: OQS_SIG_STFL_sign failed!\n", method_name);
		goto err;
	}
	fprintf(fh, "smlen = %zu\n", signature_len);
	fprintBstr(fh, "sm = ", signature, signature_len);

	if (signature_len != sig->length_signature) {
		fprintf(stderr, "[kat_stfl_sig] %s ERROR: OQS_SIG_STFL_sign incorrect length of signature!\n", method_name);
		goto err;
	}

	if (!ReadHex(fp_rsp, signature_kat, signature_len, "sm = ")) {
		fprintf(stderr, "ERROR: unable to read 'msg' from <%s>\n", katfile);
		goto err;
	}

	if (memcmp(signature, signature_kat, signature_len)) {
		fprintBstr(fh, "sm_kat = ", signature_kat, signature_len);
		fprintf(stderr, "Incorrect signature output\n");
		goto err;
	}

	rc = OQS_SIG_STFL_verify(sig, msg, msg_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_stfl_sig] %s ERROR: OQS_SIG_STFL_verify failed!\n", method_name);
		goto err;
	}

	rc = OQS_SIG_STFL_sigs_remaining(sig, &sigs_remain, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_stfl_sig] %s ERROR: OQS_SIG_STFL_sigs_remaining failed!\n", method_name);
		goto err;
	}
	fprintf(fh, "remain = %llu\n", sigs_remain);

	rc = OQS_SIG_STFL_sigs_total(sig, &sigs_maximum, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_stfl_sig] %s ERROR: OQS_SIG_STFL_sigs_total failed!\n", method_name);
		goto err;
	}
	fprintf(fh, "max = %llu", sigs_maximum);

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(signed_msg, signed_msg_len);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_SIG_STFL_SECRET_KEY_free(secret_key);
	OQS_MEM_insecure_free(signature);
	OQS_MEM_insecure_free(signature_kat);
	OQS_MEM_insecure_free(msg);
	OQS_MEM_insecure_free(msg_rand);
	OQS_SIG_STFL_free(sig);
	fclose(fp_rsp);
	return ret;
}

int main(int argc, char **argv) {
	OQS_init();

	if (argc != 3) {
		fprintf(stderr, "Usage: kat_stfl_sig algname katfile\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_STFL_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	char *katfile = argv[2];
	OQS_STATUS rc = sig_stfl_kat(alg_name, katfile);
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
