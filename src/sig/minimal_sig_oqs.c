/*
 * minimal_sig_oqs.c
 *
 * Minimal example of a post-quantum signature method implemented in liboqs.
 *
*/
#include <stdio.h>
#include <stdlib.h>

#include <oqs/common.h>
#include <oqs/rand.h>
#include <oqs/sig.h>

/* Displays hexadecimal strings */
void print_hex_string(const char *label, uint8_t *str, size_t len);

/* Partially displays hexadecimal strings */
void print_partial_hex_string(const char *label, uint8_t *str, size_t len,
                              size_t sub_len);

/* Cleaning up memory etc */
void cleanup(uint8_t *msg, size_t msg_len, uint8_t *sig, size_t sig_len,
             uint8_t *pub, uint8_t *priv, OQS_SIG *s, OQS_RAND *rnd);

int main(void) {
	uint8_t *priv = NULL; // Private key
	uint8_t *pub = NULL;  // Public key
	uint8_t *msg = NULL;  // Message
	size_t msg_len = 0;   // Message's length
	uint8_t *sig = NULL;  // Signature
	size_t sig_len = 0;   // Signature's length

	enum OQS_SIG_algid alg_name = OQS_SIG_picnic_default; // Algorithm name
	// Equivalent to OQS_SIG_picnic_L1_FS

	OQS_RAND *rnd = NULL; // Source of randomness
	OQS_SIG *s = NULL;    // OQS_SIG structure

	/* Setup the source of randomness */
	rnd = OQS_RAND_new(OQS_RAND_alg_urandom_chacha20);
	if (rnd == NULL) {
		eprintf("ERROR: Setting up the randomness source!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Populate the OQS_SIG structure, here's where liboqs sets up
     * the specific details of the selected SIG implementation */
	s = OQS_SIG_new(rnd, alg_name);
	if (s == NULL) {
		eprintf("ERROR: OQS_SIG_new failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Proceed with the signature generation */
	printf("====================================\n");
	printf("Post-quantum signature: %s\n", s->method_name);
	printf("====================================\n");

	/* Private key memory allocation */
	priv = malloc(s->priv_key_len);
	if (priv == NULL) {
		eprintf("ERROR: priv malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Public key memory generation */
	pub = malloc(s->pub_key_len);
	if (pub == NULL) {
		eprintf("ERROR: pub malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Generates the signature key pair */
	int success = OQS_SIG_keygen(s, priv, pub);
	if (success != OQS_SUCCESS) {
		eprintf("ERROR: OQS_SIG_keygen failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	print_hex_string("Private key", priv, s->priv_key_len);
	print_hex_string("Public key", pub, s->pub_key_len);

	/* Allocates the memory for the message to sign */
	msg_len = 64; // TODO: randomize based on scheme's max length
	msg = malloc(msg_len);
	if (msg == NULL) {
		eprintf("ERROR: msg malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Generates a random message to sign */
	OQS_RAND_n(rnd, msg, msg_len);
	print_hex_string("Message", msg, msg_len);

	/* Allocates memory for the signature */
	sig_len = s->max_sig_len;
	sig = malloc(sig_len);
	if (sig == NULL) {
		eprintf("ERROR: sig malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Signs the message */
	success = OQS_SIG_sign(s, priv, msg, msg_len, sig, &sig_len);
	if (success != OQS_SUCCESS) {
		eprintf("ERROR: OQS_SIG_sign failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	if (sig_len > 40) {
		// only print the parts of the sig if too long
		print_partial_hex_string("Signature", sig, sig_len, 20);
	}

	/* Verification */
	success = OQS_SIG_verify(s, pub, msg, msg_len, sig, sig_len);
	if (success != OQS_SUCCESS) {
		eprintf("ERROR: OQS_SIG_verify failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

		return EXIT_FAILURE;
	}

	/* Success and clean-up */
	printf("Signature is valid.\n");
	cleanup(msg, msg_len, sig, sig_len, pub, priv, s, rnd);

	return EXIT_SUCCESS;
}

void print_hex_string(const char *label, uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", ((unsigned char *) (str))[i]);
	}
	printf("\n");
}

void print_partial_hex_string(const char *label, uint8_t *str, size_t len,
                              size_t sub_len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (sub_len); i++) {
		printf("%02X", ((unsigned char *) (str))[i]);
	}
	printf("...");
	for (size_t i = 0; i < (sub_len); i++) {
		printf("%02X", ((unsigned char *) (str))[len - sub_len + i]);
	}
	printf("\n");
}

/* Cleaning up memory etc */
void cleanup(uint8_t *msg, size_t msg_len, uint8_t *sig, size_t sig_len,
             uint8_t *pub, uint8_t *priv, OQS_SIG *s, OQS_RAND *rnd) {
	OQS_MEM_secure_free(msg, msg_len);
	OQS_MEM_secure_free(sig, sig_len);
	OQS_MEM_secure_free(pub, s->pub_key_len);
	OQS_MEM_secure_free(priv, s->priv_key_len);
	OQS_SIG_free(s);
	OQS_RAND_free(rnd);
}
