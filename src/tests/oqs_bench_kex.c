#include "test_cases.h"
#include "ds_benchmark.h"

#include <oqs/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define KEX_TEST_ITERATIONS 100
#define KEX_BENCH_SECONDS 1

static void cleanup_alice_0(OQS_KEX *kex, void *alice_priv, uint8_t *alice_msg) {
	free(alice_msg);
	OQS_KEX_alice_priv_free(kex, alice_priv);
}

static void cleanup_bob(uint8_t *bob_msg, uint8_t *bob_key) {
	free(bob_msg);
	free(bob_key);
}

static int kex_bench_wrapper(OQS_RAND *rand, enum OQS_KEX_alg_name alg_name, const uint8_t *seed, const size_t seed_len, const char *named_parameters, const int seconds) {

	OQS_KEX *kex = NULL;
	int rc;

	void *alice_priv = NULL;
	uint8_t *alice_msg = NULL;
	size_t alice_msg_len;
	uint8_t *alice_key = NULL;
	size_t alice_key_len;

	uint8_t *bob_msg = NULL;
	size_t bob_msg_len;
	uint8_t *bob_key = NULL;
	size_t bob_key_len;

	/* setup KEX */
	kex = OQS_KEX_new(rand, alg_name, seed, seed_len, named_parameters);
	if (kex == NULL) {
		fprintf(stderr, "new_method failed\n");
		goto err;
	}
	printf("%s\n", kex->method_name);

	TIME_OPERATION_SECONDS({ OQS_KEX_alice_0(kex, &alice_priv, &alice_msg, &alice_msg_len); cleanup_alice_0(kex, alice_priv, alice_msg); }, "alice 0", seconds);

	OQS_KEX_alice_0(kex, &alice_priv, &alice_msg, &alice_msg_len);
	TIME_OPERATION_SECONDS({ OQS_KEX_bob(kex, alice_msg, alice_msg_len, &bob_msg, &bob_msg_len, &bob_key, &bob_key_len); cleanup_bob(bob_msg, bob_key); }, "bob", seconds);

	OQS_KEX_bob(kex, alice_msg, alice_msg_len, &bob_msg, &bob_msg_len, &bob_key, &bob_key_len);
	TIME_OPERATION_SECONDS({ OQS_KEX_alice_1(kex, alice_priv, bob_msg, bob_msg_len, &alice_key, &alice_key_len); free(alice_key); }, "alice 1", seconds);
	alice_key = NULL;

	rc = 1;
	goto cleanup;

err:
	rc = 0;

cleanup:
	free(alice_msg);
	free(alice_key);
	free(bob_msg);
	free(bob_key);
	OQS_KEX_alice_priv_free(kex, alice_priv);
	OQS_KEX_free(kex);

	return rc;

}

int main(int argc, char **argv) {

	int success = 1;
	bool run_all = true;
	size_t kex_testcases_len = sizeof(kex_testcases) / sizeof(struct kex_testcase);

	if (argc > 1) {
		if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "-help") == 0) || (strcmp(argv[1], "--help") == 0)) {
			printf("Usage: ./oqs_bench_kex [algorithms]\n\n");
			printf("algorithms:\n");
			for (size_t i = 0; i < kex_testcases_len; i++) {
				printf("  %s\n", kex_testcases[i].id);
			}
			return EXIT_SUCCESS;
		}
		run_all = false;
		for (int i = 1; i < argc; i++) {
			for (size_t j = 0; j < kex_testcases_len; j++) {
				if (strcmp(argv[i], kex_testcases[j].id) == 0) {
					kex_testcases[j].run = 1;
				}
			}
		}
	}

	/* setup RAND */
	OQS_RAND *rand = OQS_RAND_new(OQS_RAND_alg_urandom_chacha20);
	if (rand == NULL) {
		goto err;
	}

	PRINT_TIMER_HEADER
	for (size_t i = 0; i < kex_testcases_len; i++) {
		if (run_all || kex_testcases[i].run == 1) {
			kex_bench_wrapper(rand, kex_testcases[i].alg_name, kex_testcases[i].seed, kex_testcases[i].seed_len, kex_testcases[i].named_parameters, KEX_BENCH_SECONDS);
		}
	}
	PRINT_TIMER_FOOTER

	success = 1;
	goto cleanup;

err:
	success = 0;
	fprintf(stderr, "ERROR!\n");

cleanup:
	OQS_RAND_free(rand);

	return (success == 1) ? EXIT_SUCCESS : EXIT_FAILURE;

}
