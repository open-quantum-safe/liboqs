#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/rand.h>
#include <oqs/kex.h>

#define DEBUG

#define PRINT_HEX_STRING(label, str, len) { \
	printf("%-20s (%4zu bytes):  ", (label), (size_t) (len)); \
	for (size_t i = 0; i < (len); i++) { \
		printf("%02X", ((unsigned char *) (str)) [i]); \
	} \
	printf("\n"); \
}

int main(void) {

	int ret;
	int rc;

	OQS_RAND *rand = NULL;
	OQS_KEX *kex = NULL;

	void *alice_priv = NULL;
	uint8_t *alice_msg = NULL;
	size_t alice_msg_len;
	uint8_t *alice_key = NULL;
	size_t alice_key_len;

	uint8_t *bob_msg = NULL;
	size_t bob_msg_len;
	uint8_t *bob_key = NULL;
	size_t bob_key_len;

	/* setup RAND */
	rand = OQS_RAND_new();
	if (rand == NULL) goto err;

	/* setup KEX */
	kex = OQS_KEX_new(rand, NULL, 0);
	if (kex == NULL) goto err;

	/* Alice's initial message */
	rc = OQS_KEX_alice_0(kex, &alice_priv, &alice_msg, &alice_msg_len);
	if (rc != 1) goto err;

#ifdef DEBUG
	PRINT_HEX_STRING("Alice message", alice_msg, alice_msg_len)
#endif

	/* Bob's response */
	rc = OQS_KEX_bob(kex, alice_msg, alice_msg_len, &bob_msg, &bob_msg_len, &bob_key, &bob_key_len);
	if (rc != 1) goto err;

#ifdef DEBUG
	PRINT_HEX_STRING("Bob message", bob_msg, bob_msg_len)
	PRINT_HEX_STRING("Bob session key", bob_key, bob_key_len)
#endif

	/* Alice processes Bob's response */
	rc = OQS_KEX_alice_1(kex, alice_priv, bob_msg, bob_msg_len, &alice_key, &alice_key_len);
	if (rc != 1) goto err;

#ifdef DEBUG
	PRINT_HEX_STRING("Alice session key", alice_key, alice_key_len)
#endif

	/* compare session key lengths and values */
	if (alice_key_len != bob_key_len) {
		fprintf(stderr, "Alice's session key and Bob's session key are different lengths (%zu vs %zu)\n", alice_key_len, bob_key_len);
		goto err;
	}
	rc = memcmp(alice_key, bob_key, alice_key_len);
	if (rc != 0) {
		PRINT_HEX_STRING("Alice session key", alice_key, alice_key_len)
		PRINT_HEX_STRING("Bob session key", bob_key, bob_key_len)
		goto err;
	}

	ret = 1;
	printf("Success!\n");
	goto cleanup;

err:
	ret = 0;
	fprintf(stderr, "ERROR!\n");
cleanup:
	free(alice_msg);
	free(alice_key);
	free(bob_msg);
	free(bob_key);
	OQS_KEX_alice_priv_free(kex, alice_priv);
	OQS_KEX_free(kex);
	OQS_RAND_free(rand);

	return ret;

}
