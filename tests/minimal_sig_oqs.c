/*
 * minimal_sig_oqs.c
 *
 * Minimal example of a post-quantum signature method implemented in liboqs.
 *
*/
#include <stdio.h>
#include <stdlib.h>

#include <oqs/oqs.h>

/* Cleaning up memory etc */
void cleanup(uint8_t *msg, size_t msg_len, uint8_t *sig, size_t sig_len,
             uint8_t *pub, uint8_t *priv, OQS_SIG *s);

#ifdef ENABLE_SIG_PICNIC
int main(void) {
	uint8_t *priv = NULL; // Private key
	uint8_t *pub = NULL;  // Public key
	uint8_t *msg = NULL;  // Message
	size_t msg_len = 0;   // Message's length
	uint8_t *sig = NULL;  // Signature
	size_t sig_len = 0;   // Signature's length

	enum OQS_SIG_algid alg_name = OQS_SIG_default; // Algorithm name
	// Equivalent to OQS_SIG_picnic_L1_FS

	OQS_SIG *s = NULL;    // OQS_SIG structure

	/* Populate the OQS_SIG structure, here's where liboqs sets up
	 * the specific details of the selected SIG implementation */
	s = OQS_SIG_new(alg_name);
	if (s == NULL) {
		eprintf("ERROR: OQS_SIG_new failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	/* Proceed with the signature generation */
	printf("====================================\n");
	printf("Post-quantum signature: %s\n", s->method_name);
	printf("====================================\n");

	/* Private key memory allocation */
	priv = malloc(s->length_secret_key);
	if (priv == NULL) {
		eprintf("ERROR: priv malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	/* Public key memory generation */
	pub = malloc(s->length_public_key);
	if (pub == NULL) {
		eprintf("ERROR: pub malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	/* Generates the signature key pair */
	OQS_STATUS success = OQS_SIG_keygen(s, priv, pub);
	if (success != OQS_SUCCESS) {
		eprintf("ERROR: OQS_SIG_keygen failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	OQS_print_hex_string("Private key", priv, s->length_secret_key);
	OQS_print_hex_string("Public key", pub, s->length_public_key);

	/* Allocates the memory for the message to sign */
	msg_len = 64; // TODO: randomize based on scheme's max length
	msg = malloc(msg_len);
	if (msg == NULL) {
		eprintf("ERROR: msg malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	/* Generates a random message to sign */
	OQS_randombytes(msg, msg_len);
	OQS_print_hex_string("Message", msg, msg_len);

	/* Allocates memory for the signature */
	sig_len = s->max_length_signature;
	sig = malloc(sig_len);
	if (sig == NULL) {
		eprintf("ERROR: sig malloc failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	/* Signs the message */
	success = OQS_SIG_sign(s, priv, msg, msg_len, sig, &sig_len);
	if (success != OQS_SUCCESS) {
		eprintf("ERROR: OQS_SIG_sign failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	if (sig_len > 40) {
		// only print the parts of the sig if too long
		OQS_print_part_hex_string("Signature", sig, sig_len, 20);
	}

	/* Verification */
	success = OQS_SIG_verify(s, pub, msg, msg_len, sig, sig_len);
	if (success != OQS_SUCCESS) {
		eprintf("ERROR: OQS_SIG_verify failed!\n");
		cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

		return EXIT_FAILURE;
	}

	/* Success and clean-up */
	printf("Signature is valid.\n");
	cleanup(msg, msg_len, sig, sig_len, pub, priv, s);

	return EXIT_SUCCESS;
}
#else // !ENABLE_SIG_PICNIC
int main(void) {
	printf("Picnic not available. Make sure configure was run properly (see Readme.md) or change this code to use a different algorithm.\n");
	return EXIT_FAILURE;
}
#endif

/* Cleaning up memory etc */
void cleanup(uint8_t *msg, size_t msg_len, uint8_t *sig, size_t sig_len,
             uint8_t *pub, uint8_t *priv, OQS_SIG *s) {
	OQS_MEM_secure_free(msg, msg_len);
	OQS_MEM_secure_free(sig, sig_len);
	OQS_MEM_secure_free(pub, s->length_public_key);
	OQS_MEM_secure_free(priv, s->length_secret_key);
	OQS_SIG_free(s);
}
