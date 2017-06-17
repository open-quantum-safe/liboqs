/*! @file LowMC.h
 *  @brief Header file for LowMC.c, the file where all the LowMC
 *  proof-of-knowledge operations are done.
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
#ifndef LOWMC_H
#define LOWMC_H

#include <openssl/sha.h>
#include "LowMCConstants.h"

#define PRG_SEED_LENGTH 32

#define NUM_ZKB_ROUNDS 438

typedef struct proof_t {
	uint8_t seed1[PRG_SEED_LENGTH];
	uint8_t seed2[PRG_SEED_LENGTH];
	uint32_t *inputShare; // Input share of the party which does not derive it from the seed (not included if challenge is 0)
	uint8_t *communicatedBits;
	uint8_t view3Commitment[SHA256_DIGEST_LENGTH];
	uint8_t *view3UnruhG; // we include the max length, but we will only serialize the bytes we use
} proof_t;

typedef struct signature_t {
	proof_t proofs[NUM_ZKB_ROUNDS];
	uint8_t challengeBits[((2 * NUM_ZKB_ROUNDS) - 1) / 8 + 1]; // there are 2 bits per round for the challenge.
} signature_t;

typedef enum {
	TRANSFORM_FS = 0,
	TRANSFORM_UR = 1,
	TRANSFORM_INVALID = 255
} transform_t;

int sign(uint32_t *privateKey, uint32_t *pubKey, uint32_t *pubInput, const uint8_t *message, size_t messageByteLength, signature_t *sig, transform_t transform, lowmcparams_t *params);
int verify(signature_t *sig, const uint32_t *pubKey, const uint32_t *pubInput, const uint8_t *message, size_t messageByteLength, transform_t transform, lowmcparams_t *params);

void allocateSignature(signature_t *sig, lowmcparams_t *params);
void freeSignature(signature_t *sig);

/* Returns the number of bytes written on success, or -1 on error */
int serializeSignature(const signature_t *sig, uint8_t *sigBytes, size_t sigBytesLen, transform_t transform, lowmcparams_t *params);
/* Returns EXIT_SUCCESS on success or EXIT_FAILURE on error */
int deserializeSignature(signature_t *sig, const uint8_t *sigBytes, size_t sigBytesLen, transform_t transform, lowmcparams_t *params);

/* Functions to initialize OpenSSL */
void init_EVP();
void cleanup_EVP();

/*
 * Fill buf with len random bytes.
 * Returns 1 on success, 0 on failure
 */
int random_bytes_default_openssl(uint8_t *buf, size_t len);

/* Return the number of bytes required to represent the given number of bits */
uint32_t numBytes(uint32_t numBits);

/* Get a bit from a byte array */
uint8_t getBit(const uint8_t *array, uint32_t bitNumber);
uint8_t getBitFromWordArray(const uint32_t *array, uint32_t bitNumber);

/* Set a bit in a byte/word array */
void setBit(uint8_t *bytes, uint32_t bitNumber, uint8_t val);
void setBitInWordArray(uint32_t *bytes, uint32_t bitNumber, uint8_t val);

#endif /* LOWMC_H */
