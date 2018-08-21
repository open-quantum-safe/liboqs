/**
 * \file rand.h
 * \brief Random number generator.
 */

#ifndef __OQS_RANDOM_H
#define __OQS_RANDOM_H

#include <stdbool.h>
#define HAVE_BOOL
#include <stddef.h>
#include <stdint.h>

#include <oqs/common.h>

/*********************
 *** START NEW API ***
 *********************/
/* TODO: Consider adding NIST-KAT PRNGs from nist-branch */
/* TODO: Decide how to do statistical testing of new PRNG */

/** Algorithm identifier for system PRNG. */
#define OQS_RAND_alg_system "system"
/** Algorithm identifier for using OpenSSL's PRNG. */
#define OQS_RAND_alg_openssl "OpenSSL"
/** Algorithm identifier for NIST deterministic RNG for KATs. */
#define OQS_RAND_alg_nist_kat "NIST-KAT"
/** Algorithm identifier for using OpenSSL's PRNG. */

/**
 * Switches OQS_randombytes to use the specified algorithm.
 *
 * @param[in] algorithm The name of the algorithm to use.
 * @return OQS_SUCCESS if `algorithm` is a supported algorithm name, OQS_ERROR otherwise.
 */
OQS_STATUS OQS_randombytes_switch_algorithm(const char *algorithm);

/**
 * Switches OQS_randombytes to use the given function.
 *
 * This allows additional custom RNGs besides the provided ones.  The provided RNG
 * function must have the same signature as `OQS_randombytes`.
 *
 * @param[in] algorithm_ptr Pointer to the RNG function to use.
 */
void OQS_randombytes_custom_algorithm(void (*algorithm_ptr)(uint8_t *, size_t));

/**
 * Fills the given memory with the requested number of (pseudo)random bytes.
 *
 * This implementation uses whichever algorithm has been selected by
 * OQS_randombytes_switch_algorithm. The default is OQS_randombytes_system, which
 * reads bytes directly from `/dev/urandom`.
 *
 * The caller is responsible for providing a buffer allocated with sufficient room.
 *
 * @param[out] random_array Pointer to the memory to fill with (pseudo)random bytes
 * @param[in] bytes_to_read The number of random bytes to read into memory
 */
void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read);

/**
 * Fills the given memory with the requested number of random bytes.
 *
 * This implementation reads the bytes directly from `/dev/urandom` and will
 * block until the requested number of bytes have been read.
 *
 * The caller is responsible for providing a buffer allocated with sufficient room.
 *
 * @param[out] random_array Pointer to the memory to fill with random bytes
 * @param[in] bytes_to_read The number of random bytes to read into memory
 */
void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read);

/**
 * Initializes the NIST DRBG with a given seed.
 *
 * @param[in] entropy_input The seed; must be exactly 48 bytes
 * @param[in] personalization_string An optional personalization string; may be NULL
 * @param[in] security_strength The required security strength; must be 256
 */
void OQS_randombytes_nist_kat_init(const uint8_t *entropy_input, const uint8_t *personalization_string, int security_strength);

/********************
 *** STOP NEW API ***
 ********************/

/************************************************************
 *** START DEPRECATED CODE *** expected removal Aug. 2018 ***
 ************************************************************/

enum OQS_RAND_alg_name {
	OQS_RAND_alg_default,
	OQS_RAND_alg_urandom_chacha20,
	OQS_RAND_alg_urandom_aesctr,
};

typedef struct OQS_RAND OQS_RAND;

/**
 * OQS PRNG object
 */
struct OQS_RAND {

	/**
	 * Specifies the name of the random number function
	 */
	char *method_name;

	/**
	 * Estimated number of bits of security provided against a classical
	 * attacker
	 */
	uint16_t estimated_classical_security;

	/**
	 * Estimated number of bits of security provided against a quantum
	 * attacker
	 */
	uint16_t estimated_quantum_security;

	/**
	 * Pointer for storing the state of the PRNG
	 */
	void *ctx;

	/**
	 * Function which returns an 8-bit random unsigned integer
	 */
	uint8_t (*rand_8)(OQS_RAND *r);

	/**
	 * Function which returns an 32-bit random unsigned integer
	 */
	uint32_t (*rand_32)(OQS_RAND *r);

	/**
	 * Function which returns an 64-bit random unsigned integer
	 */
	uint64_t (*rand_64)(OQS_RAND *r);

	/**
	 * Function which generates n random 8-bit unsigned integers
	 *
	 * @param out : pointer to an array large enough to store the output integers (\f$\text{size} \geq n\f$)
	 * @param n : number of integers to generate
	 */
	void (*rand_n)(OQS_RAND *r, uint8_t *out, size_t n);

	/**
	 * Pointer to a function for freeing the allocated key exchange structure
	 *
	 * @param k : Key exchange structure
	 *
	 */
	void (*free)(OQS_RAND *r);
};

OQS_RAND *OQS_RAND_new(enum OQS_RAND_alg_name alg_name);

uint8_t OQS_RAND_8(OQS_RAND *r);
uint32_t OQS_RAND_32(OQS_RAND *r);
uint64_t OQS_RAND_64(OQS_RAND *r);
void OQS_RAND_n(OQS_RAND *r, uint8_t *out, size_t n);

void OQS_RAND_free(OQS_RAND *r);

void OQS_RAND_test_record_occurrence(const unsigned char b, unsigned long occurrences[256]);
double OQS_RAND_test_statistical_distance_from_uniform(const unsigned long occurrences[256]);

#define ZSCORE_SPARSE (999.999)
#define ZSCORE_BIGNEG (-100.0)
#define ZSCORE_BIGPOS (+100.0)
double OQS_RAND_zscore_deviation_from_uniform(const unsigned long occurrences[256]);
void OQS_RAND_report_statistics(const unsigned long occurrences[256], const char *indent);

OQS_STATUS OQS_RAND_get_system_entropy(uint8_t *buf, size_t n);

/***********************************************************
 *** STOP DEPRECATED CODE *** expected removal Aug. 2018 ***
 ***********************************************************/

#endif // __OQS_RANDOM_H
