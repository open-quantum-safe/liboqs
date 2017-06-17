/*! @file picnic.c
 *  @brief Implementation of the Picnic signature API
 *
 *  This file is part of the reference implementation of the Picnic and Fish
 *  signature schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha
 *  Cryptology ePrint Archive: Report 2017/279
 *  http://eprint.iacr.org/2017/279
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *
 */

#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include "LowMCEnc.h"
#include "LowMC.h"
#include "picnic.h"
#include "platform.h"

static int g_initialized = 0;

#define ENSURE_LIBRARY_INITIALIZED()         \
	if (!g_initialized) {                    \
		printf("Library not initialized\n"); \
		return -1;                           \
	}

static int is_valid_params(picnic_params_t params) {
	if (params > 0 && params < PARAMETER_SET_MAX_INDEX) {
		return 1;
	}

	return 0;
}

transform_t get_transform(picnic_params_t parameters) {
	switch (parameters) {
	case LowMC_256_256_42_14_FS:
	case LowMC_256_256_1_316_FS:
	case LowMC_256_256_10_38_FS:
		return TRANSFORM_FS;
	case LowMC_256_256_42_14_UR:
	case LowMC_256_256_1_316_UR:
	case LowMC_256_256_10_38_UR:
		return TRANSFORM_UR;

	default:
		return TRANSFORM_INVALID;
	}
}

const char *picnic_get_param_name(picnic_params_t parameters) {
	switch (parameters) {
	case LowMC_256_256_42_14_FS:
		return "LowMC_256_256_42_14_FS";
	case LowMC_256_256_1_316_FS:
		return "LowMC_256_256_1_316_FS";
	case LowMC_256_256_10_38_FS:
		return "LowMC_256_256_10_38_FS";
	case LowMC_256_256_42_14_UR:
		return "LowMC_256_256_42_14_UR";
	case LowMC_256_256_1_316_UR:
		return "LowMC_256_256_1_316_UR";
	case LowMC_256_256_10_38_UR:
		return "LowMC_256_256_10_38_UR";

	default:
		return "Unknown parameter set";
	}
}

int picnicParamsToLowMCParams(picnic_params_t picnicParams, lowmcparams_t *lowmcParams) {
	memset(lowmcParams, 0, sizeof(lowmcparams_t));

	switch (picnicParams) {
	case LowMC_256_256_42_14_FS:
		lowmcParams->stateSizeBytes = 32;
		lowmcParams->numSboxes = 42;
		lowmcParams->numRounds = 14;
		lowmcParams->numAndBytes = numBytes(lowmcParams->numSboxes * 3 * lowmcParams->numRounds);
		break;
	case LowMC_256_256_42_14_UR:
		lowmcParams->stateSizeBytes = 32;
		lowmcParams->numSboxes = 42;
		lowmcParams->numRounds = 14;
		lowmcParams->numAndBytes = numBytes(lowmcParams->numSboxes * 3 * lowmcParams->numRounds);
		lowmcParams->UnruhGWithoutInputBytes = PRG_SEED_LENGTH + lowmcParams->numAndBytes;
		lowmcParams->UnruhGWithInputBytes = lowmcParams->UnruhGWithoutInputBytes + lowmcParams->stateSizeBytes;
		break;
	case LowMC_256_256_1_316_FS:
		lowmcParams->stateSizeBytes = 32;
		lowmcParams->numSboxes = 1;
		lowmcParams->numRounds = 316;
		lowmcParams->numAndBytes = numBytes(lowmcParams->numSboxes * 3 * lowmcParams->numRounds);
		break;
	case LowMC_256_256_1_316_UR:
		lowmcParams->stateSizeBytes = 32;
		lowmcParams->numSboxes = 1;
		lowmcParams->numRounds = 316;
		lowmcParams->numAndBytes = numBytes(lowmcParams->numSboxes * 3 * lowmcParams->numRounds);
		lowmcParams->UnruhGWithoutInputBytes = PRG_SEED_LENGTH + lowmcParams->numAndBytes;
		lowmcParams->UnruhGWithInputBytes = lowmcParams->UnruhGWithoutInputBytes + lowmcParams->stateSizeBytes;
		break;
	case LowMC_256_256_10_38_FS:
		lowmcParams->stateSizeBytes = 32;
		lowmcParams->numSboxes = 10;
		lowmcParams->numRounds = 38;
		lowmcParams->numAndBytes = numBytes(lowmcParams->numSboxes * 3 * lowmcParams->numRounds);
		break;
	case LowMC_256_256_10_38_UR:
		lowmcParams->stateSizeBytes = 32;
		lowmcParams->numSboxes = 10;
		lowmcParams->numRounds = 38;
		lowmcParams->numAndBytes = numBytes(lowmcParams->numSboxes * 3 * lowmcParams->numRounds);
		lowmcParams->UnruhGWithoutInputBytes = PRG_SEED_LENGTH + lowmcParams->numAndBytes;
		lowmcParams->UnruhGWithInputBytes = lowmcParams->UnruhGWithoutInputBytes + lowmcParams->stateSizeBytes;
		break;
	default:
		fprintf(stderr, "%s: Unsupported LowMC parameter set (%d). \n", __func__, picnicParams);
		return -1;
	}

	lowmcParams->stateSizeBits = lowmcParams->stateSizeBytes * 8;
	lowmcParams->stateSizeWords = lowmcParams->stateSizeBits / WORD_SIZE_BITS;
	return EXIT_SUCCESS;
}

int picnic_init(picnic_params_t params, const char *path, unsigned int flags) {
	/* NOTE: Not threadsafe, init_EVP may end up being called multiple times.
     * and readLookupTables will leak if called multiple times. */
	if (!g_initialized) {

		/* Do not initialize OpenSSL when flags == 1 */
		if (flags == 0) {
			init_EVP();
		}

		lowmcparams_t lowmcparams;

		int ret = picnicParamsToLowMCParams(params, &lowmcparams);
		if (ret != EXIT_SUCCESS) {
			return ret;
		}

		ret = readLookupTables(&lowmcparams, path);
		if (ret != 0) {
			return ret;
		}

		g_initialized = 1;
	}

	return 0;
}

void picnic_shutdown(unsigned int flags) {
	if (g_initialized) {

		/* Do not shutdown OpenSSL when flags == 1 */
		if (flags == 0) {
			cleanup_EVP();
		}
		freeLookupTables();
		g_initialized = 0;
	}
}

int picnic_keygen(picnic_params_t parameters, picnic_publickey_t *pk,
                  picnic_privatekey_t *sk) {
	ENSURE_LIBRARY_INITIALIZED();

	if (!is_valid_params(parameters)) {
		printf("Invalid parameter set\n");
		return -1;
	}

	if (pk == NULL) {
		printf("public key is NULL\n");
		return -1;
	}

	if (sk == NULL) {
		printf("private key is NULL\n");
		return -1;
	}

	memset(pk, 0x00, sizeof(picnic_publickey_t));
	memset(sk, 0x00, sizeof(picnic_privatekey_t));

	lowmcparams_t lowmcparams;
	int ret = picnicParamsToLowMCParams(parameters, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to initialize LowMC parameters\n");
		fflush(stderr);
		return ret;
	}

	/* Generate a private key */
	sk->params = parameters;
	if (!picnic_random_bytes(sk->data, lowmcparams.stateSizeBytes)) {
		printf("Failed to generate private key\n");
		return -1;
	}

	/* Generate a random plaintext block */
	pk->params = parameters;
	if (!picnic_random_bytes(pk->pubInput, lowmcparams.stateSizeBytes)) {
		printf("Failed to generate private key\n");
		return -1;
	}
	sk->pk = pk;

	/* Compute the ciphertext */
	LowMCEnc((uint32_t *) pk->pubInput, (uint32_t *) pk->ciphertext,
	         (uint32_t *) sk->data, &lowmcparams);

	return 0;
}

int picnic_sign(picnic_privatekey_t *sk, const uint8_t *message, size_t message_len,
                uint8_t *signature, size_t *signature_len) {
	ENSURE_LIBRARY_INITIALIZED();

	int ret;
	transform_t transform = get_transform(sk->params);
	signature_t *sig = (signature_t *) malloc(sizeof(signature_t));
	lowmcparams_t lowmcparams;

	ret = picnicParamsToLowMCParams(sk->params, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to initialize LowMC parameters\n");
		fflush(stderr);
		freeSignature(sig);
		free(sig);
		return ret;
	}

	allocateSignature(sig, &lowmcparams);
	if (sig == NULL) {
		return -1;
	}

	ret = sign((uint32_t *) sk->data, (uint32_t *) sk->pk->ciphertext, (uint32_t *) sk->pk->pubInput, message,
	           message_len, sig, transform, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to create signature\n");
		fflush(stderr);
		freeSignature(sig);
		free(sig);
		return -1;
	}

#if 0
    // self-check for debugging, try to verify the sig we just created
    ret = verify(sig, (uint32_t*)sk->pk->ciphertext, (uint32_t*)sk->pk->pubInput, message, message_len, transform, lowmcparams);
    if (ret != EXIT_SUCCESS) {
        fprintf(stderr, "Failed self-test, signature didn't verify.\n");
        fflush(stderr);
    }
#endif

	ret = serializeSignature(sig, signature, *signature_len, transform, &lowmcparams);
	if (ret == -1) {
		fprintf(stderr, "Failed to serialize signature\n");
		fflush(stderr);
		freeSignature(sig);
		free(sig);
		return -1;
	}
	*signature_len = ret;
	freeSignature(sig);
	free(sig);
	return 0;
}

size_t picnic_signature_size(picnic_params_t parameters) {

	lowmcparams_t lowmcparams;
	transform_t transform = get_transform(parameters);

	int ret = picnicParamsToLowMCParams(parameters, &lowmcparams);

	if (ret != EXIT_SUCCESS) {
		return PICNIC_MAX_SIGNATURE_SIZE;
	}

	switch (transform) {
	case TRANSFORM_FS:
		// This is the largest possible FS signature size and would result when no challenges are 0 -- which would require us to include stateSizeBytes for every ZKB round.
		return NUM_ZKB_ROUNDS * (SHA256_DIGEST_LENGTH + lowmcparams.stateSizeBytes + numBytes(3 * lowmcparams.numSboxes * lowmcparams.numRounds) + 2 * PRG_SEED_LENGTH) + numBytes(2 * NUM_ZKB_ROUNDS);
	case TRANSFORM_UR:
		return NUM_ZKB_ROUNDS * (SHA256_DIGEST_LENGTH + lowmcparams.stateSizeBytes + 2 * numBytes(3 * lowmcparams.numSboxes * lowmcparams.numRounds) + 3 * PRG_SEED_LENGTH) + numBytes(2 * NUM_ZKB_ROUNDS);
	default:
		return PICNIC_MAX_SIGNATURE_SIZE;
	}
}

int picnic_verify(picnic_publickey_t *pk, const uint8_t *message, size_t message_len,
                  const uint8_t *signature, size_t signature_len) {
	ENSURE_LIBRARY_INITIALIZED();
	int ret;

	lowmcparams_t lowmcparams;
	ret = picnicParamsToLowMCParams(pk->params, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to initialize LowMC parameters\n");
		fflush(stderr);
		return -1;
	}

	transform_t transform = get_transform(pk->params);
	signature_t *sig = (signature_t *) malloc(sizeof(signature_t));
	allocateSignature(sig, &lowmcparams);
	if (sig == NULL) {
		return -1;
	}

	ret = deserializeSignature(sig, signature, signature_len, transform, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to deserialize signature\n");
		fflush(stderr);
		freeSignature(sig);
		free(sig);
		return -1;
	}

	ret = verify(sig, (uint32_t *) pk->ciphertext,
	             (uint32_t *) pk->pubInput, message, message_len, transform, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		/* Signature is invalid, or verify function failed */
		freeSignature(sig);
		free(sig);
		return -1;
	}

	freeSignature(sig);
	free(sig);
	return 0;
}

/* Serialize public key */
int picnic_write_public_key(const picnic_publickey_t *key, uint8_t *buf, size_t buflen) {
	if (key == NULL || buf == NULL) {
		return -1;
	}

	size_t bytesRequired = 1 + sizeof(key->pubInput) + sizeof(key->ciphertext);
	if (buflen < bytesRequired || bytesRequired > INT_MAX) {
		return -1;
	}

	buf[0] = (uint8_t) key->params;

	memcpy(buf + 1, key->pubInput, sizeof(key->pubInput));
	memcpy(buf + 1 + sizeof(key->pubInput), key->ciphertext, sizeof(key->ciphertext));

	return (int) bytesRequired;
}

int picnic_read_public_key(picnic_publickey_t *key, const uint8_t *buf, size_t buflen) {
	if (key == NULL || buf == NULL) {
		return -1;
	}

	size_t bytesExpected = 1 + sizeof(key->pubInput) + sizeof(key->ciphertext);
	if (buflen < bytesExpected) {
		return -1;
	}

	if (is_valid_params(buf[0])) {
		key->params = buf[0];
	} else {
		return -1;
	}

	memcpy(key->pubInput, buf + 1, sizeof(key->pubInput));
	memcpy(key->ciphertext, buf + 1 + sizeof(key->pubInput), sizeof(key->ciphertext));

	return 0;
}

/* Serialize a private key. */
int picnic_write_private_key(const picnic_privatekey_t *key, uint8_t *buf, size_t buflen) {
	if (key == NULL || buf == NULL) {
		return -1;
	}

	size_t bytesRequired = 1 + sizeof(key->data);
	if (buflen < bytesRequired || bytesRequired > INT_MAX) {
		return -1;
	}

	buf[0] = (uint8_t) key->params;

	memcpy(buf + 1, key->data, sizeof(key->data));

	return (int) bytesRequired;
}

/* De-serialize a private key. */
int picnic_read_private_key(picnic_privatekey_t *key, const uint8_t *buf, size_t buflen, picnic_publickey_t *publickey) {
	if (key == NULL || buf == NULL || publickey == NULL) {
		return -1;
	}

	size_t bytesExpected = 1 + sizeof(key->data);
	if (buflen < bytesExpected) {
		return -1;
	}

	if (is_valid_params(buf[0])) {
		key->params = buf[0];
	} else {
		/* Unsupported parameter set */
		return -1;
	}

	if (key->params != publickey->params) {
		return -1;
	}

	memcpy(key->data, buf + 1, sizeof(key->data));
	key->pk = publickey;

	return 0;
}

/* Check that a key pair is valid. */
int picnic_validate_keypair(const picnic_privatekey_t *privatekey, const picnic_publickey_t *publickey) {
	ENSURE_LIBRARY_INITIALIZED();

	lowmcparams_t lowmcparams;
	int ret;
	ret = picnicParamsToLowMCParams(publickey->params, &lowmcparams);
	if (ret != EXIT_SUCCESS) {
		return ret;
	}

	if (privatekey == NULL || publickey == NULL) {
		return -1;
	}

	if (privatekey->params != publickey->params) {
		return -1;
	}

	if (!is_valid_params(privatekey->params)) {
		fprintf(stderr, "Unsupported parameter set\n");
		return -1;
	}

	/* Re-compute the ciphertext and compare to the value in the public key. */
	uint8_t ciphertext[sizeof(publickey->ciphertext)];
	LowMCEnc((uint32_t *) publickey->pubInput, (uint32_t *) ciphertext, (uint32_t *) privatekey->data, &lowmcparams);
	if (memcmp(ciphertext, publickey->ciphertext, sizeof(ciphertext)) != 0) {
		return -1;
	}

	return 0;
}
