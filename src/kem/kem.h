#ifndef __OQS_KEM_H__
#define __OQS_KEM_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

// This enum is assumed to be in increasing order: 0, 1, 2, .., last
enum OQS_KEM_alg_name {
	OQS_KEM_alg_default = 0,
	OQS_KEM_alg_dummy1,
	OQS_KEM_alg_dummy2,
	OQS_KEM_alg_frodokem_640_aes,
	OQS_KEM_alg_frodokem_976_aes,
	OQS_KEM_alg_frodokem_640_cshake,
	OQS_KEM_alg_frodokem_976_cshake,
	OQS_KEM_alg_newhope_512_cca_kem,
	OQS_KEM_alg_newhope_1024_cca_kem,
	OQS_KEM_alg_BIG_QUAKE_1,
	// EDIT-WHEN-ADDING-KEM
	OQS_KEM_alg_last
};

typedef struct OQS_KEM {

	char *method_name;

	uint8_t claimed_nist_level;
	bool ind_cca;

	size_t length_public_key;
	size_t length_secret_key;
	size_t length_ciphertext;
	size_t length_shared_secret;

	// clang-format off
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);
	OQS_STATUS (*encaps)(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
	OQS_STATUS (*decaps)(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
	// clang-format on

} OQS_KEM;

OQS_KEM *OQS_KEM_new(enum OQS_KEM_alg_name alg_name);

OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

void OQS_KEM_free(OQS_KEM *kem);

#include <oqs/kem_dummy1.h>
#include <oqs/kem_dummy2.h>
#include <oqs/kem_frodokem.h>
#include <oqs/kem_newhopenist.h>
#include <oqs/kem_BIGQUAKE.h>
// EDIT-WHEN-ADDING-KEM

#endif
