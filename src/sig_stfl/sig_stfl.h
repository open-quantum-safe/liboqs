/**
 * \file sig_stfl.h
 * \brief Stateful Signature schemes
 *
 * The file `tests/example_sig_stfl.c` contains an example on using the OQS_SIG_STFL API.
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

#define OQS_SIG_STFL_algs_length 1

/**
 * Returns identifiers for available signature schemes in liboqs.  Used with OQS_SIG_STFL_new.
 *
 * Note that algorithm identifiers are present in this list even when the algorithm is disabled
 * at compile time.
 *
 * @param[in] i Index of the algorithm identifier to return, 0 <= i < OQS_SIG_algs_length
 * @return Algorithm identifier as a string, or NULL.
 */
OQS_API const char *OQS_SIG_STFL_alg_identifier(size_t i);

/**
 * Returns the number of signature mechanisms in liboqs.  They can be enumerated with
 * OQS_SIG_STFL_alg_identifier.
 *
 * Note that some mechanisms may be disabled at compile time.
 *
 * @return The number of signature mechanisms.
 */
OQS_API int OQS_SIG_STFL_alg_count(void);

/**
 * Indicates whether the specified algorithm was enabled at compile-time or not.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_SIG_STFL_algs`.
 * @return 1 if enabled, 0 if disabled or not found
 */
OQS_API int OQS_SIG_STFL_alg_is_enabled(const char *method_name);

/**
 * Stateful signature scheme object
 */
typedef struct OQS_SIG_STFL {

	/** A local ordinal representing the LMS parameter of the signature scheme. */
	uint32_t oid;

	/** Printable string representing the name of the signature scheme. */
	const char *method_name;

	/**
	 * Printable string representing the version of the cryptographic algorithm.
	 *
	 * Implementations with the same method_name and same alg_version will be interoperable.
	 * See README.md for information about algorithm compatibility.
	 */
	const char *alg_version;

	/** Whether the signature offers EUF-CMA security (TRUE) or not (FALSE). */
	bool euf_cma;

	/** The (maximum) length, in bytes, of public keys for this signature scheme. */
	size_t length_public_key;
	/** The (maximum) length, in bytes, of secret keys for this signature scheme. */
	size_t length_secret_key;
	/** The (maximum) length, in bytes, of signatures for this signature scheme. */
	size_t length_signature;


	/**
	 * Keypair generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `public_key`
	 * based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_SIG_STFL_*_length_*`.
	 *
	 * @param[out] public_key The public key represented as a byte string.
	 * @param[out] secret_key The secret key represented as a byt string
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);

	/**
	 * Signature generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `signature`,
	 * based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_SIG_STFL_*_length_*`.
	 *
	 * @param[out] signature The signature on the message represented as a byte string.
	 * @param[out] signature_len The length of the signature.
	 * @param[in] message The message to sign represented as a byte string.
	 * @param[in] message_len The length of the message to sign.
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*sign)(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, uint8_t *secret_key);

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
	OQS_STATUS (*verify)(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

	/**
	 * Query number of remaining signatures
	 *
	 * @param[out] remain The number of remaining signatures
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*sigs_remaining)(size_t *remain, const uint8_t *secret_key);

	/**
	 * Total number of signatures
	 *
	 * @param[out] total The total number of signatures
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*sigs_total)(size_t *total, const uint8_t *secret_key);

} OQS_SIG_STFL;

/**
 * @brief OQS_SIG_STFL_SECRET_KEY object for stateful signature schemes
 */
typedef struct OQS_SIG_STFL_SECRET_KEY OQS_SIG_STFL_SECRET_KEY;

typedef struct OQS_SIG_STFL_SECRET_KEY {

	/** Associated signature object */
	OQS_SIG_STFL *sig;

	/* The (maximum) length, in bytes, of secret keys for this signature scheme. */
	size_t length_secret_key;

	/* The variant specific secret key data */
	void *secret_key_data;

	/* Function that returns the total number of signatures for the secret key */
	unsigned long long (*sigs_total)(const OQS_SIG_STFL_SECRET_KEY *secret_key);

	/* Function that returns the number of signatures left for the secret key */
	unsigned long long (*sigs_left)(const OQS_SIG_STFL_SECRET_KEY *secret_key);

	/**
	 * Secret Key retrieval Function
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @param[out] key_len length of the returned byte string
	 * @returns newly created pointer to ley byte string if none-zero length. Caller
	 * deletes the buffer.
	 */
	uint8_t *(*serialize_key)(OQS_SIG_STFL_SECRET_KEY *sk, size_t key_len);

	/**
	 * set Secret Key to internal structure Function
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @param[out] key_len length of the returned byte string
	 * @returns newly created pointer to ley byte string if none-zero length. Caller
	 * deletes the buffer.
	 */
	uint8_t *(*deserialize_key)(OQS_SIG_STFL_SECRET_KEY *sk, size_t key_len, uint8_t *sk_key);

	/**
	 * Secret Key Locking Function
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*lock_key)(OQS_SIG_STFL_SECRET_KEY *sk);

	/**
	 * Secret Key Unlocking / Releasing Function
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*unlock_key)(OQS_SIG_STFL_SECRET_KEY *sk);

	/**
	 * Secret Key Saving Function
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*save_secret_key)(const OQS_SIG_STFL_SECRET_KEY *sk);

	/**
	 * Secret Key free internal variant specific data
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @return none
	 */
	void (*free_key)(OQS_SIG_STFL_SECRET_KEY *sk);
} OQS_SIG_STFL_SECRET_KEY;

/**
 * Constructs an OQS_SIG_STFL object for a particular algorithm.
 *
 * Callers should always check whether the return value is `NULL`, which indicates either than an
 * invalid algorithm name was provided, or that the requested algorithm was disabled at compile-time.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_SIG_STFL_algs`.
 * @return An OQS_SIG_STFL for the particular algorithm, or `NULL` if the algorithm has been disabled at compile-time.
 */
OQS_API OQS_SIG_STFL *OQS_SIG_STFL_new(const char *method_name);

/**
 * Keypair generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `public_key` based
 * on the `length_*` members in this object or the per-scheme compile-time macros
 * `OQS_SIG_STFL_*_length_*`. Caller is also responsible for initializing
 * `secret_key` using the OQS_SIG_STFL_SECRET_KEY(*) function
 *
 * @param[in] sig The OQS_SIG_STFL object representing the signature scheme.
 * @param[out] public_key The public key represented as a byte string.
 * @param[out] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *pk, uint8_t *sk);

/**
 * Signature generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `signature`,
 * based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_SIG_STFL_*_length_*`.
 *
 * @param[in] sig The OQS_SIG_STFL object representing the signature scheme.
 * @param[out] signature The signature on the message represented as a byte string.
 * @param[out] signature_len The length of the signature.
 * @param[in] message The message to sign represented as a byte string.
 * @param[in] message_len The length of the message to sign.
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, uint8_t *secret_key);

/**
 * Signature verification algorithm.
 *
 * @param[in] sig The OQS_SIG_STFL object representing the signature scheme.
 * @param[in] message The message represented as a byte string.
 * @param[in] message_len The length of the message.
 * @param[in] signature The signature on the message represented as a byte string.
 * @param[in] signature_len The length of the signature.
 * @param[in] public_key The public key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_verify(const OQS_SIG_STFL *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

/**
 * Query number of remaining signatures
 *
 * @param[in] sig The OQS_SIG_STFL object representing the signature scheme.
 * @param[out] remain The number of remaining signatures
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_sigs_remaining(const OQS_SIG_STFL *sig, size_t *remain, const uint8_t *secret_key);

/**
 * * Total number of signatures
 *
 * @param[in] sig The OQS_SIG_STFL object representing the signature scheme.
 * @param[out] max The number of remaining signatures
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_sigs_total(const OQS_SIG_STFL *sig, size_t *max, const uint8_t *secret_key);

/**
 * Frees an OQS_SIG_STFL object that was constructed by OQS_SIG_STFL_new.
 *
 * @param[in] sig The OQS_SIG_STFL object to free.
 */
OQS_API void OQS_SIG_STFL_free(OQS_SIG_STFL *sig);

/**
 * Constructs an OQS_SIG_STFL_SECRET_KEY object for a particular algorithm.
 *
 * Callers should always check whether the return value is `NULL`, which indicates either than an
 * invalid algorithm name was provided, or that the requested algorithm was disabled at compile-time.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_SIG_STFL_algs`.
 * @return An OQS_SIG_STFL_SECRET_KEY for the particular algorithm, or `NULL` if the algorithm has been disabled at compile-time.
 */
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SIG_STFL_SECRET_KEY_new(const char *method_name);

/**
 * Frees an OQS_SIG_STFL_SECRET_KEY object that was constructed by OQS_SECRET_KEY_new.
 *
 * @param[in] sig The OQS_SIG_STFL_SECRET_KEY object to free.
 * @return OQS_SUCCESS if successful, or OQS_ERROR if the object could not be freed.
 */
OQS_API void OQS_SIG_STFL_SECRET_KEY_free(OQS_SIG_STFL_SECRET_KEY *sk);

#if defined(__cplusplus)
} // extern "C"
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS
#include <oqs/sig_stfl_xmss.h>
#endif // OQS_ENABLE_SIG_STFL_XMSS

#endif /* OQS_SIG_STATEFUL_H */
