/*
 * dump_alg_info.c
 *
 * Print algorithm information in YAML format; used in conjunction with test_alg_info.py.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

int main(void) {

	OQS_init();
	// iterate through KEMs and print info
	printf("KEMs:\n");
	for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
		const char *kem_name = OQS_KEM_alg_identifier(i);
		printf("  %s:\n", kem_name);
		OQS_KEM *kem = OQS_KEM_new(kem_name);
		if (kem == NULL) {
			printf("    isnull: true\n");
			continue;
		}
		printf("    isnull: false\n");
		printf("    claimed-nist-level: %d\n", kem->claimed_nist_level);
		printf("    claimed-security: %s\n", kem->ind_cca ? "IND-CCA2" : "IND-CPA");
		printf("    length-public-key: %zu\n", kem->length_public_key);
		printf("    length-ciphertext: %zu\n", kem->length_ciphertext);
		printf("    length-secret-key: %zu\n", kem->length_secret_key);
		printf("    length-shared-secret: %zu\n", kem->length_shared_secret);
		OQS_KEM_free(kem);
	}

	// iterate through signature schemes and print info
	printf("SIGs:\n");
	for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
		const char *sig_name = OQS_SIG_alg_identifier(i);
		printf("  %s:\n", sig_name);
		OQS_SIG *sig = OQS_SIG_new(sig_name);
		if (sig == NULL) {
			printf("    isnull: true\n");
			continue;
		}
		printf("    isnull: false\n");
		printf("    claimed-nist-level: %d\n", sig->claimed_nist_level);
		printf("    claimed-security: %s\n", sig->suf_cma ? "SUF-CMA" : (sig->euf_cma ? "EUF-CMA" : "none"));
		printf("    length-public-key: %zu\n", sig->length_public_key);
		printf("    length-secret-key: %zu\n", sig->length_secret_key);
		printf("    length-signature: %zu\n", sig->length_signature);
		OQS_SIG_free(sig);
	}

	// iterate through stateful signature schemes and print info
	printf("SIG_STFLs:\n");
	for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
		const char *sig_name = OQS_SIG_STFL_alg_identifier(i);
		printf("  %s:\n", sig_name);
		OQS_SIG_STFL *sig = OQS_SIG_STFL_new(sig_name);
		if (sig == NULL) {
			printf("    isnull: true\n");
			continue;
		}
		printf("    isnull: false\n");
		printf("    claimed-security: %s\n", sig->suf_cma ? "SUF-CMA" : (sig->euf_cma ? "EUF-CMA" : "none"));
		printf("    length-public-key: %zu\n", sig->length_public_key);
		printf("    length-secret-key: %zu\n", sig->length_secret_key);
		printf("    length-signature: %zu\n", sig->length_signature);
		OQS_SIG_STFL_free(sig);
	}

	OQS_destroy();
}

