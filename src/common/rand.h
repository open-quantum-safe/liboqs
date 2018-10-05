/**
 * \file rand.h
 * \brief Random number generator.
 */

#ifndef __OQS_RANDOM_H
#define __OQS_RANDOM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/common.h>

/** Algorithm identifier for system PRNG. */
#define OQS_RAND_alg_system "system"
/** Algorithm identifier for NIST deterministic RNG for KATs. */
#define OQS_RAND_alg_nist_kat "NIST-KAT"
/** Algorithm identifier for using OpenSSL's PRNG. */
#define OQS_RAND_alg_openssl "OpenSSL"

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
 * Initializes the NIST DRBG with a given seed.
 *
 * @param[in] entropy_input The seed; must be exactly 48 bytes
 * @param[in] personalization_string An optional personalization string; may be NULL
 * @param[in] security_strength The required security strength; must be 256
 */
void OQS_randombytes_nist_kat_init(const uint8_t *entropy_input, const uint8_t *personalization_string, int security_strength);

/************************************************************
 *** START DEPRECATED CODE *** expected removal Sep. 2018 ***
 ************************************************************/

/** Algorithm identifiers
 *
 * @deprecated Expected removal Sep. 2018
 */
enum OQS_RAND_alg_name {
	OQS_RAND_alg_default,
	OQS_RAND_alg_urandom_chacha20,
	OQS_RAND_alg_urandom_aesctr,
};

/**
 * OQS PRNG object
 *
 * @deprecated Expected removal Sep. 2018
 */
typedef struct OQS_RAND OQS_RAND;

/**
 * OQS PRNG object
 *
 * @deprecated Expected removal Sep. 2018
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
	 *
	 * @param r PRNG object
	 */
	uint8_t (*rand_8)(OQS_RAND *r);

	/**
	 * Function which returns an 32-bit random unsigned integer
	 *
	 * @param r PRNG object
	 */
	uint32_t (*rand_32)(OQS_RAND *r);

	/**
	 * Function which returns an 64-bit random unsigned integer
	 *
	 * @param r PRNG object
	 */
	uint64_t (*rand_64)(OQS_RAND *r);

	/**
	 * Function which generates n random 8-bit unsigned integers
	 *
	 * @param r PRNG object
	 * @param out pointer to an array large enough to store the output integers (\f$\text{size} \geq n\f$)
	 * @param n number of integers to generate
	 */
	void (*rand_n)(OQS_RAND *r, uint8_t *out, size_t n);

	/**
	 * Pointer to a function for freeing the allocated key exchange structure
	 *
	 * @param r PRNG object
	 */
	void (*free)(OQS_RAND *r);
};

/**
 * Construct a new PRNG object
 *
 * @param alg_name Algorithm identifier
 * @deprecated Expected removal Sep. 2018
 */
OQS_RAND *OQS_RAND_new(enum OQS_RAND_alg_name alg_name);

/**
 * Function which returns an 8-bit random unsigned integer
 *
 * @param r PRNG object
 * @deprecated Expected removal Sep. 2018
 */
uint8_t OQS_RAND_8(OQS_RAND *r);

/**
 * Function which returns an 32-bit random unsigned integer
 *
 * @param r PRNG object
 * @deprecated Expected removal Sep. 2018
 */
uint32_t OQS_RAND_32(OQS_RAND *r);

/**
 * Function which returns an 64-bit random unsigned integer
 *
 * @param r PRNG object
 * @deprecated Expected removal Sep. 2018
 */
uint64_t OQS_RAND_64(OQS_RAND *r);

/**
 * Function which generates n random 8-bit unsigned integers
 *
 * @param r PRNG object
 * @param out pointer to an array large enough to store the output integers (\f$\text{size} \geq n\f$)
 * @param n number of integers to generate
 * @deprecated Expected removal Sep. 2018
 */
void OQS_RAND_n(OQS_RAND *r, uint8_t *out, size_t n);

/**
 * Pointer to a function for freeing the allocated key exchange structure
 *
 * @param r PRNG object
 * @deprecated Expected removal Sep. 2018
 */
void OQS_RAND_free(OQS_RAND *r);

/**
 * Internal function for statistical testing
 *
 * @param b undocumented
 * @param occurrences undocumented
 * @deprecated Expected removal Sep. 2018
 */
void OQS_RAND_test_record_occurrence(const unsigned char b, unsigned long occurrences[256]);

/**
 * Internal function for statistical testing
 *
 * @param occurrences undocumented
 * @return undocumented
 * @deprecated Expected removal Sep. 2018
 */
double OQS_RAND_test_statistical_distance_from_uniform(const unsigned long occurrences[256]);

/**
 * Parameter for statistical testing.
 *
 * @deprecated Expected removal Sep. 2018
 */
#define ZSCORE_SPARSE (999.999)

/**
 * Parameter for statistical testing.
 *
 * @deprecated Expected removal Sep. 2018
 */
#define ZSCORE_BIGNEG (-100.0)

/**
 * Parameter for statistical testing.
 *
 * @deprecated Expected removal Sep. 2018
 */
#define ZSCORE_BIGPOS (+100.0)

/**
 * Internal function for statistical testing
 *
 * @param occurrences undocumented
 * @return undocumented
 * @deprecated Expected removal Sep. 2018
 */
double OQS_RAND_zscore_deviation_from_uniform(const unsigned long occurrences[256]);

/**
 * Internal function for statistical testing
 *
 * @param occurrences undocumented
 * @param indent undocumented
 * @deprecated Expected removal Sep. 2018
 */
void OQS_RAND_report_statistics(const unsigned long occurrences[256], const char *indent);

/**
 * undocumented
 *
 * @param buf undocumented
 * @param n undocumented
 * @return undocumented
 * @deprecated Expected removal Sep. 2018
 */
OQS_STATUS OQS_RAND_get_system_entropy(uint8_t *buf, size_t n);

/***********************************************************
 *** STOP DEPRECATED CODE *** expected removal Aug. 2018 ***
 ***********************************************************/

#endif // __OQS_RANDOM_H
