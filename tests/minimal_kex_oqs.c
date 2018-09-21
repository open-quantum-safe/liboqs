/*
 * minimal_kex_oqs.c
 *
 * Minimal example of a Diffie-Hellman post-quantum key exchange method
 * implemented in liboqs.
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

/* Cleaning up memory etc */
void cleanup(uint8_t *alice_msg, size_t alice_msg_len, uint8_t *alice_key,
             size_t alice_key_len, uint8_t *bob_msg, size_t bob_msg_len,
             uint8_t *bob_key, size_t bob_key_len, void *alice_priv,
             OQS_KEX *kex, OQS_RAND *rnd);


int main(void) {
	printf("KEX algorithm not available. Make sure configure was run properly; see Readme.md.\n");
	return EXIT_FAILURE;
}


void cleanup(uint8_t *alice_msg, size_t alice_msg_len, uint8_t *alice_key,
             size_t alice_key_len, uint8_t *bob_msg, size_t bob_msg_len,
             uint8_t *bob_key, size_t bob_key_len, void *alice_priv,
             OQS_KEX *kex, OQS_RAND *rnd) {
	/* Secure cleaning */
	OQS_MEM_secure_free(alice_msg, alice_msg_len);
	OQS_MEM_secure_free(alice_key, alice_key_len);
	OQS_MEM_secure_free(bob_msg, bob_msg_len);
	OQS_MEM_secure_free(bob_key, bob_key_len);
	OQS_KEX_alice_priv_free(kex, alice_priv);
	OQS_KEX_free(kex);
	OQS_RAND_free(rnd);
}
