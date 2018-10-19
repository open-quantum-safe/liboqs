/**
 * \file sig.h
 * \brief Signature schemes
 *
 * The file `src/sig/example_sig.c` contains two examples on using the OQS_SIG API.
 *
 * The first example uses the individual scheme's algorithms directly and uses
 * no dynamic memory allocation -- all buffers are allocated on the stack, with
 * sizes indicated using preprocessor macros.  Since algorithms can be disabled at
 * compile-time, the programmer should wrap the code in \#ifdefs.
 *
 * The second example uses an OQS_SIG object to use an algorithm specified at
 * runtime.  Therefore it uses dynamic memory allocation -- all buffers must be
 * malloc'ed by the programmer, with sizes indicated using the corresponding length
 * member of the OQS_SIG object in question.  Since algorithms can be disabled at
 * compile-time, the programmer should check that the OQS_SIG object is not `NULL`.
 */

#ifndef __OQS_SIG_H
#define __OQS_SIG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

/** Algorithm identifier for default SIG algorithm. */
#define OQS_SIG_alg_default "DEFAULT"
/** Algorithm identifier for qTESLA_I */
#define OQS_SIG_alg_qTESLA_I "qTESLA_I"
/** Algorithm identifier for qTESLA_III_size */
#define OQS_SIG_alg_qTESLA_III_size "qTESLA_III_size"
/** Algorithm identifier for qTESLA_III_speed */
#define OQS_SIG_alg_qTESLA_III_speed "qTESLA_III_speed"
/** Algorithm identifier for qTESLA_p_I */
#define OQS_SIG_alg_qTESLA_p_I "qTESLA_p_I"
/** Algorithm identifier for qTESLA_p_III */
#define OQS_SIG_alg_qTESLA_p_III "qTESLA_p_III"
/** Algorithm identifier for picnic_L1_FS */
#define OQS_SIG_alg_picnic_L1_FS "picnic_L1_FS"
/** Algorithm identifier for picnic_L1_UR */
#define OQS_SIG_alg_picnic_L1_UR "picnic_L1_UR"
/** Algorithm identifier for picnic_L3_FS */
#define OQS_SIG_alg_picnic_L3_FS "picnic_L3_FS"
/** Algorithm identifier for Picnic_L3_UR */
#define OQS_SIG_alg_picnic_L3_UR "picnic_L3_UR"
/** Algorithm identifier for Picnic_L5_FS */
#define OQS_SIG_alg_picnic_L5_FS "picnic_L5_FS"
/** Algorithm identifier for Picnic_L5_FS */
#define OQS_SIG_alg_picnic_L5_UR "picnic_L5_UR"
/** Algorithm identifier for Dilithium_II_medium */
#define OQS_SIG_alg_Dilithium_II_medium "Dilithium_II_medium"
/** Algorithm identifier for Dilithium_III_recommended */
#define OQS_SIG_alg_Dilithium_III_recommended "Dilithium_III_recommended"
/** Algorithm identifier for Dilithium_IV_very_high */
#define OQS_SIG_alg_Dilithium_IV_very_high "Dilithium_IV_very_high"

// EDIT-WHEN-ADDING-SIG
/** Number of algorithm identifiers above (including default). */
#define OQS_SIG_algs_length 15

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
 * Signature schemes object
 */
typedef struct OQS_SIG {

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
	/** The (maximum) length, in bytes, of secret keys for this signature scheme. */
	size_t length_secret_key;
	/** The (maximum) length, in bytes, of signature overheads for this signature scheme. */
	size_t length_sig_overhead;

	// clang-format off

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
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);

	/**
	 * Signature generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `signed_message`,
	 * based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_SIG_*_length_*`.
	 *
	 * @param[out] signed_message The signed message represented as a byte string.
	 * @param[out] signed_message_len The length of the signed message.
	 * @param[in] message The message to sign represented as a byte string.
	 * @param[in] message_len The length of the message to sign.
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*sign)(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);

	/**
	 * Signature verification algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `message`, based
	 * on the `length_*` members in this object or the per-scheme compile-time macros
	 * `OQS_SIG_*_length_*`.
	 *
	 * @param[out] message The message represented as a byte string.
	 * @param[out] message_len The length of the message.
	 * @param[in] signed_message The signed message represented as a byte string.
	 * @param[in] signed_message_len The length of the signed message.
	 * @param[in] public_key The public key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*sign_open)(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);
	// clang-format on

} OQS_SIG;

/**
 * Constructs an OQS_SIG object for a particular algorithm.
 *
 * Callers should always check whether the return value is `NULL`, which indicates either than an
 * invalid algorithm name was provided, or that the requested algorithm was disabled at compile-time.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_SIG_algs`.
 * @return An OQS_SIG for the particular algorithm, or `NULL` if the algorithm has been disabled at compile-time.
 */
OQS_API OQS_SIG *OQS_SIG_new(const char *method_name);

/**
 * Keypair generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `public_key` and
 * `secret_key`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_SIG_*_length_*`.
 *
 * @param[in] sig The OQS_SIG object representing the signature scheme.
 * @param[out] public_key The public key represented as a byte string.
 * @param[out] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_keypair(const OQS_SIG *sig, uint8_t *public_key, uint8_t *secret_key);

/**
 * Signature generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `signed_message`,
 * based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_SIG_*_length_*`.
 *
 * @param[in] sig The OQS_SIG object representing the signature scheme.
 * @param[out] signed_message The signed message represented as a byte string.
 * @param[out] signed_message_len The length of the signed message.
 * @param[in] message The message to sign represented as a byte string.
 * @param[in] message_len The length of the message to sign.
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_sign(const OQS_SIG *sig, uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);

/**
 * Signature verification algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `message`, based
 * on the `length_*` members in this object or the per-scheme compile-time macros
 * `OQS_SIG_*_length_*`.
 *
 * @param[in] sig The OQS_SIG object representing the signature scheme.
 * @param[out] message The message represented as a byte string.
 * @param[out] message_len The length of the message.
 * @param[in] signed_message The signed message represented as a byte string.
 * @param[in] signed_message_len The length of the signed message.
 * @param[in] public_key The public key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_sign_open(const OQS_SIG *sig, uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

/**
 * Frees an OQS_SIG object that was constructed by OQS_SIG_new.
 *
 * @param[in] sig The OQS_SIG object to free.
 */
OQS_API void OQS_SIG_free(OQS_SIG *sig);

#include <oqs/sig_qtesla.h>
#include <oqs/sig_picnic.h>
#include <oqs/sig_dilithium.h>
// EDIT-WHEN-ADDING-SIG

#endif // __OQS_SIG_H
