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
#define OQS_SIG_STFL_alg_xmss_shake256_h10 "XMSS-SHAKE_10_256"
/* Algorithm identifier for XMSS-SHAKE_16_256 */
#define OQS_SIG_STFL_alg_xmss_shake256_h16 "XMSS-SHAKE_16_256"
/* Algorithm identifier for XMSS-SHAKE_20_256 */
#define OQS_SIG_STFL_alg_xmss_shake256_h20 "XMSS-SHAKE_20_256"
/* Algorithm identifier for XMSS-SHAKE_10_512 */
#define OQS_SIG_STFL_alg_xmss_shake512_h10 "XMSS-SHAKE_10_512"
/* Algorithm identifier for XMSS-SHAKE_16_512 */
#define OQS_SIG_STFL_alg_xmss_shake512_h16 "XMSS-SHAKE_16_512"
/* Algorithm identifier for XMSS-SHAKE_20_512 */
#define OQS_SIG_STFL_alg_xmss_shake512_h20 "XMSS-SHAKE_20_512"

/**
 * LMS will be added later. So far its only XMSS 
 * 
 */
#define OQS_SIG_STFL_algs_length 12

/**
 * Returns identifiers for available signature schemes in liboqs.  Used with OQS_SIG_new.
 *
 * Note that algorithm identifiers are present in this list even when the algorithm is disabled
 * at compile time.
 *
 * @param[in] i Index of the algorithm identifier to return, 0 <= i < OQS_SIG_algs_length
 * @return Algorithm identifier as a string, or NULL.
 */
const char *OQS_SIG_STFL_alg_identifier(size_t i);

/**
 * Returns the number of signature mechanisms in liboqs.  They can be enumerated with
 * OQS_SIG_alg_identifier.
 *
 * Note that some mechanisms may be disabled at compile time.
 *
 * @return The number of signature mechanisms.
 */
int OQS_SIG_STFL_alg_count(void);

/**
 * Indicates whether the specified algorithm was enabled at compile-time or not.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_SIG_algs`.
 * @return 1 if enabled, 0 if disabled or not found
 */
int OQS_SIG_STFL_alg_is_enabled(const char *method_name);


/**
 * @brief Secret Kye object for stateful signature schemes * 
 */

typedef struct OQS_SECRET_KEY OQS_SECRET_KEY;

typedef struct OQS_SECRET_KEY {
	
	/** The (maximum) length, in bytes, of secret keys for this signature scheme. */
	unsigned long long length_secret_key;

	uint32_t oid;

	bool is_xmssmt;

	/** The physical secret key stored in memory as an array of bytes*/
	uint8_t *secret_key;

	unsigned long long (*sigs_total)(const OQS_SECRET_KEY *secret_key);

	unsigned long long (*sigs_left)(const OQS_SECRET_KEY *secret_key);

	int (*lock_key)(OQS_SECRET_KEY *sk);
	int (*oqs_save_updated_sk_key)(const OQS_SECRET_KEY *sk);
	int (*release_key)(OQS_SECRET_KEY *sk);

} OQS_SECRET_KEY;

OQS_SECRET_KEY *OQS_SECRET_KEY_new(const char *method_name);

void OQS_SECRET_KEY_free(OQS_SECRET_KEY *sk);	



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


	/**
	 * Keypair generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `public_key` and
	 * `secret_key`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_SIG_*_length_*`.
	 *
	 * @param[out] public_key The public key represented as a byte string.
	 * @param[out] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	int (*keypair)(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

	/**
	 * Signature generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `signature`,
	 * based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_SIG_*_length_*`.
	 *
	 * @param[out] signature The signature on the message represented as a byte string.
	 * @param[out] signature_len The length of the signature.
	 * @param[in] message The message to sign represented as a byte string.
	 * @param[in] message_len The length of the message to sign.
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	int (*sign)(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key);

	/**
	 * Signature verification algorithm.
	 *
	 * @param[in] message The message represented as a byte string.
	 * @param[in] message_len The length of the message.
	 * @param[in] signature The signature on the message represented as a byte string.
	 * @param[in] signature_len The length of the signature.
	 * @param[in] public_key The public key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	int (*verify)(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

} OQS_SIG_STFL;

OQS_SIG_STFL *OQS_SIG_STFL_new(const char *method_name);

int OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *pk, OQS_SECRET_KEY *sk);

int OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key);

int OQS_SIG_STFL_verify(const OQS_SIG_STFL *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

void OQS_SIG_STFL_free(OQS_SIG_STFL *sig);

#if defined(__cplusplus)
} // extern "C"
#endif


#endif /* OQS_SIG_STATEFUL_H */