/*
 * \file sig_stateful.h
 * \brief Stateful Signature schemes
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_SIG_STATEFUL_H
#define OQS_SIG_STATEFUL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Algorithm identifier for XMSS-SHA2_10_256 */
#define OQS_SIG_STFL_alg_xmss_sha256_h10 "XMSS-SHA2_10_256"
/* Algorithm identifier for XMSS-SHA2_16_256 */
#define OQS_SIG_STFL_alg_xmss_sha256_h16 "XMSS-SHA2_16_256"
/* Algorithm identifier for XMSS-SHA2_20_256 */
#define OQS_SIG_STFL_alg_xmss_sha256_h20 "XMSS-SHA2_20_256"
/* Algorithm identifier for XMSS-SHA2_10_512 */
#define OQS_SIG_STFL_alg_xmss_sha512_h10 "XMSS-SHA2_10_512"
/* Algorithm identifier for XMSS-SHA2_16_512 */
#define OQS_SIG_STFL_alg_xmss_sha512_h16 "XMSS-SHA2_16_512"
/* Algorithm identifier for XMSS-SHA2_20_256 */
#define OQS_SIG_STFL_alg_xmss_sha512_h20 "XMSS-SHA2_20_512"
/* Algorithm identifier for XMSS-SHAKE_10_256 */
#define OQS_SIG_STFL_alg_xmss_shake128_h10 "XMSS-SHAKE_10_256"
/* Algorithm identifier for XMSS-SHAKE_16_256 */
#define OQS_SIG_STFL_alg_xmss_shake128_h16 "XMSS-SHAKE_16_256"
/* Algorithm identifier for XMSS-SHAKE_20_256 */
#define OQS_SIG_STFL_alg_xmss_shake128_h20 "XMSS-SHAKE_20_256"
/* Algorithm identifier for XMSS-SHAKE_10_512 */
#define OQS_SIG_STFL_alg_xmss_shake256_h10 "XMSS-SHAKE_10_512"
/* Algorithm identifier for XMSS-SHAKE_16_512 */
#define OQS_SIG_STFL_alg_xmss_shake256_h16 "XMSS-SHAKE_16_512"
/* Algorithm identifier for XMSS-SHAKE_20_512 */
#define OQS_SIG_STFL_alg_xmss_shake256_h20 "XMSS-SHAKE_20_512"


/**
 * LMS will be added later. So far its only XMSS and XMSS^MT
 * 
 */


#define OQS_SIG_algs_length 12


/**
 * Returns identifiers for available signature schemes in liboqs.  Used with OQS_SIG_new.
 *
 * Note that algorithm identifiers are present in this list even when the algorithm is disabled
 * at compile time.
 *
 * @param[in] i Index of the algorithm identifier to return, 0 <= i < OQS_SIG_algs_length
 * @return Algorithm identifier as a string, or NULL.
 */
OQS_API const char *OQS_SIG_alg_identifier(size_t i);

/**
 * Returns the number of signature mechanisms in liboqs.  They can be enumerated with
 * OQS_SIG_alg_identifier.
 *
 * Note that some mechanisms may be disabled at compile time.
 *
 * @return The number of signature mechanisms.
 */
OQS_API int OQS_SIG_alg_count(void);

/**
 * Indicates whether the specified algorithm was enabled at compile-time or not.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_SIG_algs`.
 * @return 1 if enabled, 0 if disabled or not found
 */
OQS_API int OQS_SIG_alg_is_enabled(const char *method_name);

/**
 * Signature schemes object
 */
typedef struct OQS_SIG_STFL {

    /** Printable string representing the name of the signature scheme. */
	const char *method_name;

	/**
	 * Printable string representing the version of the cryptographic algorithm.
	 *
	 * Implementations with the same method_name and same alg_version will be interoperable.
	 * See README.md for information about algorithm compatibility.
	 */
	const char *alg_version;

	/** The NIST security level (1, 2, 3, 4, 5) claimed in this algorithm's original NIST submission. */
	uint8_t claimed_nist_level;

	/** Whether the signature offers EUF-CMA security (TRUE) or not (FALSE). */
	bool euf_cma;

	/** The (maximum) length, in bytes, of public keys for this signature scheme. */
	size_t length_public_key;
	
	/** The (maximum) length, in bytes, of signatures for this signature scheme. */
	size_t length_signature;

} OQS_SIG_STFL;


typedef struct OQS_SECRET_KEY {
	
	/** The (maximum) length, in bytes, of secret keys for this signature scheme. */
	size_t length_secret_key;

	/** The physical secret key stored in memory as an array of bytes*/
	uint8_t *secret_key;

	unsigned long long signatures_completed;

	unsigned long long signatures_left;


};	

#if defined(__cplusplus)
} // extern "C"
#endif


#endif // OQS_SIG_STATEFUL_H