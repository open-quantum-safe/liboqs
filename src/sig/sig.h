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

#ifndef OQS_SIG_H
#define OQS_SIG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** Algorithm identifier for default SIG algorithm. */
#define OQS_SIG_alg_default "DEFAULT"
/** Algorithm identifier for picnic_L1_FS */
#define OQS_SIG_alg_picnic_L1_FS "picnic_L1_FS"
/** Algorithm identifier for picnic_L1_UR */
#define OQS_SIG_alg_picnic_L1_UR "picnic_L1_UR"
/** Algorithm identifier for picnic_L3_FS */
#define OQS_SIG_alg_picnic_L3_FS "picnic_L3_FS"
/** Algorithm identifier for picnic_L3_UR */
#define OQS_SIG_alg_picnic_L3_UR "picnic_L3_UR"
/** Algorithm identifier for picnic_L5_FS */
#define OQS_SIG_alg_picnic_L5_FS "picnic_L5_FS"
/** Algorithm identifier for picnic_L5_FS */
#define OQS_SIG_alg_picnic_L5_UR "picnic_L5_UR"
/** Algorithm identifier for picnic2_L1_FS */
#define OQS_SIG_alg_picnic2_L1_FS "picnic2_L1_FS"
/** Algorithm identifier for picnic2_L3_FS */
#define OQS_SIG_alg_picnic2_L3_FS "picnic2_L3_FS"
/** Algorithm identifier for picnic2_L5_FS */
#define OQS_SIG_alg_picnic2_L5_FS "picnic2_L5_FS"
/** Algorithm identifier for qTesla-p-I */
#define OQS_SIG_alg_qTesla_p_I "qTesla-p-I"
/** Algorithm identifier for qTesla-p-III */
#define OQS_SIG_alg_qTesla_p_III "qTesla-p-III"
///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALG_IDENTIFIER_START
/** Algorithm identifier for DILITHIUM_2 */
#define OQS_SIG_alg_dilithium_2 "DILITHIUM_2"
/** Algorithm identifier for DILITHIUM_3 */
#define OQS_SIG_alg_dilithium_3 "DILITHIUM_3"
/** Algorithm identifier for DILITHIUM_4 */
#define OQS_SIG_alg_dilithium_4 "DILITHIUM_4"
/** Algorithm identifier for MQDSS-31-48 */
#define OQS_SIG_alg_mqdss_31_48 "MQDSS-31-48"
/** Algorithm identifier for MQDSS-31-64 */
#define OQS_SIG_alg_mqdss_31_64 "MQDSS-31-64"
/** Algorithm identifier for SPHINCS+-Haraka-128f-robust */
#define OQS_SIG_alg_sphincs_haraka_128f_robust "SPHINCS+-Haraka-128f-robust"
/** Algorithm identifier for SPHINCS+-Haraka-128f-simple */
#define OQS_SIG_alg_sphincs_haraka_128f_simple "SPHINCS+-Haraka-128f-simple"
/** Algorithm identifier for SPHINCS+-Haraka-128s-robust */
#define OQS_SIG_alg_sphincs_haraka_128s_robust "SPHINCS+-Haraka-128s-robust"
/** Algorithm identifier for SPHINCS+-Haraka-128s-simple */
#define OQS_SIG_alg_sphincs_haraka_128s_simple "SPHINCS+-Haraka-128s-simple"
/** Algorithm identifier for SPHINCS+-Haraka-192f-robust */
#define OQS_SIG_alg_sphincs_haraka_192f_robust "SPHINCS+-Haraka-192f-robust"
/** Algorithm identifier for SPHINCS+-Haraka-192f-simple */
#define OQS_SIG_alg_sphincs_haraka_192f_simple "SPHINCS+-Haraka-192f-simple"
/** Algorithm identifier for SPHINCS+-Haraka-192s-robust */
#define OQS_SIG_alg_sphincs_haraka_192s_robust "SPHINCS+-Haraka-192s-robust"
/** Algorithm identifier for SPHINCS+-Haraka-192s-simple */
#define OQS_SIG_alg_sphincs_haraka_192s_simple "SPHINCS+-Haraka-192s-simple"
/** Algorithm identifier for SPHINCS+-Haraka-256f-robust */
#define OQS_SIG_alg_sphincs_haraka_256f_robust "SPHINCS+-Haraka-256f-robust"
/** Algorithm identifier for SPHINCS+-Haraka-256f-simple */
#define OQS_SIG_alg_sphincs_haraka_256f_simple "SPHINCS+-Haraka-256f-simple"
/** Algorithm identifier for SPHINCS+-Haraka-256s-robust */
#define OQS_SIG_alg_sphincs_haraka_256s_robust "SPHINCS+-Haraka-256s-robust"
/** Algorithm identifier for SPHINCS+-Haraka-256s-simple */
#define OQS_SIG_alg_sphincs_haraka_256s_simple "SPHINCS+-Haraka-256s-simple"
/** Algorithm identifier for SPHINCS+-SHA256-128f-robust */
#define OQS_SIG_alg_sphincs_sha256_128f_robust "SPHINCS+-SHA256-128f-robust"
/** Algorithm identifier for SPHINCS+-SHA256-128f-simple */
#define OQS_SIG_alg_sphincs_sha256_128f_simple "SPHINCS+-SHA256-128f-simple"
/** Algorithm identifier for SPHINCS+-SHA256-128s-robust */
#define OQS_SIG_alg_sphincs_sha256_128s_robust "SPHINCS+-SHA256-128s-robust"
/** Algorithm identifier for SPHINCS+-SHA256-128s-simple */
#define OQS_SIG_alg_sphincs_sha256_128s_simple "SPHINCS+-SHA256-128s-simple"
/** Algorithm identifier for SPHINCS+-SHA256-192f-robust */
#define OQS_SIG_alg_sphincs_sha256_192f_robust "SPHINCS+-SHA256-192f-robust"
/** Algorithm identifier for SPHINCS+-SHA256-192f-simple */
#define OQS_SIG_alg_sphincs_sha256_192f_simple "SPHINCS+-SHA256-192f-simple"
/** Algorithm identifier for SPHINCS+-SHA256-192s-robust */
#define OQS_SIG_alg_sphincs_sha256_192s_robust "SPHINCS+-SHA256-192s-robust"
/** Algorithm identifier for SPHINCS+-SHA256-192s-simple */
#define OQS_SIG_alg_sphincs_sha256_192s_simple "SPHINCS+-SHA256-192s-simple"
/** Algorithm identifier for SPHINCS+-SHA256-256f-robust */
#define OQS_SIG_alg_sphincs_sha256_256f_robust "SPHINCS+-SHA256-256f-robust"
/** Algorithm identifier for SPHINCS+-SHA256-256f-simple */
#define OQS_SIG_alg_sphincs_sha256_256f_simple "SPHINCS+-SHA256-256f-simple"
/** Algorithm identifier for SPHINCS+-SHA256-256s-robust */
#define OQS_SIG_alg_sphincs_sha256_256s_robust "SPHINCS+-SHA256-256s-robust"
/** Algorithm identifier for SPHINCS+-SHA256-256s-simple */
#define OQS_SIG_alg_sphincs_sha256_256s_simple "SPHINCS+-SHA256-256s-simple"
/** Algorithm identifier for SPHINCS+-SHAKE256-128f-robust */
#define OQS_SIG_alg_sphincs_shake256_128f_robust "SPHINCS+-SHAKE256-128f-robust"
/** Algorithm identifier for SPHINCS+-SHAKE256-128f-simple */
#define OQS_SIG_alg_sphincs_shake256_128f_simple "SPHINCS+-SHAKE256-128f-simple"
/** Algorithm identifier for SPHINCS+-SHAKE256-128s-robust */
#define OQS_SIG_alg_sphincs_shake256_128s_robust "SPHINCS+-SHAKE256-128s-robust"
/** Algorithm identifier for SPHINCS+-SHAKE256-128s-simple */
#define OQS_SIG_alg_sphincs_shake256_128s_simple "SPHINCS+-SHAKE256-128s-simple"
/** Algorithm identifier for SPHINCS+-SHAKE256-192f-robust */
#define OQS_SIG_alg_sphincs_shake256_192f_robust "SPHINCS+-SHAKE256-192f-robust"
/** Algorithm identifier for SPHINCS+-SHAKE256-192f-simple */
#define OQS_SIG_alg_sphincs_shake256_192f_simple "SPHINCS+-SHAKE256-192f-simple"
/** Algorithm identifier for SPHINCS+-SHAKE256-192s-robust */
#define OQS_SIG_alg_sphincs_shake256_192s_robust "SPHINCS+-SHAKE256-192s-robust"
/** Algorithm identifier for SPHINCS+-SHAKE256-192s-simple */
#define OQS_SIG_alg_sphincs_shake256_192s_simple "SPHINCS+-SHAKE256-192s-simple"
/** Algorithm identifier for SPHINCS+-SHAKE256-256f-robust */
#define OQS_SIG_alg_sphincs_shake256_256f_robust "SPHINCS+-SHAKE256-256f-robust"
/** Algorithm identifier for SPHINCS+-SHAKE256-256f-simple */
#define OQS_SIG_alg_sphincs_shake256_256f_simple "SPHINCS+-SHAKE256-256f-simple"
/** Algorithm identifier for SPHINCS+-SHAKE256-256s-robust */
#define OQS_SIG_alg_sphincs_shake256_256s_robust "SPHINCS+-SHAKE256-256s-robust"
/** Algorithm identifier for SPHINCS+-SHAKE256-256s-simple */
#define OQS_SIG_alg_sphincs_shake256_256s_simple "SPHINCS+-SHAKE256-256s-simple"
///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALG_IDENTIFIER_END
// EDIT-WHEN-ADDING-SIG
///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALGS_LENGTH_START
/** Number of algorithm identifiers above (including default). */
#define OQS_SIG_algs_length 53
///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALGS_LENGTH_END

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
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);

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
	OQS_STATUS (*sign)(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);

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
 * Caller is responsible for allocating sufficient memory for `signnature`,
 * based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_SIG_*_length_*`.
 *
 * @param[in] sig The OQS_SIG object representing the signature scheme.
 * @param[out] signature The signature on the message represented as a byte string.
 * @param[out] signature_len The length of the signature.
 * @param[in] message The message to sign represented as a byte string.
 * @param[in] message_len The length of the message to sign.
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_sign(const OQS_SIG *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);

/**
 * Signature verification algorithm.
 *
 * @param[in] sig The OQS_SIG object representing the signature scheme.
 * @param[in] message The message represented as a byte string.
 * @param[in] message_len The length of the message.
 * @param[in] signature The signature on the message represented as a byte string.
 * @param[in] signature_len The length of the signature.
 * @param[in] public_key The public key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_verify(const OQS_SIG *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

/**
 * Frees an OQS_SIG object that was constructed by OQS_SIG_new.
 *
 * @param[in] sig The OQS_SIG object to free.
 */
OQS_API void OQS_SIG_free(OQS_SIG *sig);

#ifdef OQS_ENABLE_SIG_PICNIC
#include <oqs/sig_picnic.h>
#endif /* OQS_ENABLE_SIG_PICNIC */
#ifdef OQS_ENABLE_SIG_QTESLA
#include <oqs/sig_qtesla.h>
#endif /* OQS_ENABLE_SIG_QTESLA */
///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_INCLUDE_START
#ifdef OQS_ENABLE_SIG_DILITHIUM
#include <oqs/sig_dilithium.h>
#endif /* OQS_ENABLE_SIG_DILITHIUM */
#ifdef OQS_ENABLE_SIG_MQDSS
#include <oqs/sig_mqdss.h>
#endif /* OQS_ENABLE_SIG_MQDSS */
#ifdef OQS_ENABLE_SIG_SPHINCS
#include <oqs/sig_sphincs.h>
#endif /* OQS_ENABLE_SIG_SPHINCS */
///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_INCLUDE_END
// EDIT-WHEN-ADDING-SIG

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_SIG_H
