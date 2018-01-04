#ifndef __OQS_KEM_H__
#define __OQS_KEM_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

enum OQS_KEM_alg_name {
	OQS_KEM_alg_default,
	OQS_KEM_alg_dummy1,
	OQS_KEM_alg_dummy2,
};

typedef struct OQS_KEM {

	char *method_name;

	uint16_t claimed_classical_security;
	uint16_t claimed_quantum_security;
	bool ind_cca;

	size_t length_public_key;
	size_t length_secret_key;
	size_t length_ciphertext;
	size_t length_shared_secret;

	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);
	OQS_STATUS (*encaps)(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
	OQS_STATUS (*decaps)(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

} OQS_KEM;

#include <oqs/kem_dummy1.h>
#include <oqs/kem_dummy2.h>

OQS_KEM *OQS_KEM_new(enum OQS_KEM_alg_name alg_name);

OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

void OQS_KEM_free(OQS_KEM *kem);

#endif
