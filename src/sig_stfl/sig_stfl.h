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

/*
 * Developer's Notes:
 * Stateful signatures are based on one-time use of a secret key. A pool of secret keys are created for this purpose.
 * The state of these keys are tracked to ensure that they are used only once to generate a signature.
 *
 * As such, product specific environments do play a role in ensuring the safety of the keys.
 * Secret keys must be store securely.
 * The key index/counter must be updated after each signature generation.
 * Secret key must be protected in a thread-save manner.
 *
 * Application therefore are required to provide environment specific callback functions to
 *  - store private key
 *  - lock/unlock private key
 *
 *  See below for details
 *  OQS_SIG_STFL_SECRET_KEY_SET_lock
 *  OQS_SIG_STFL_SECRET_KEY_SET_unlock
 *  OQS_SIG_STFL_SECRET_KEY_SET_mutex
 *  OQS_SIG_STFL_SECRET_KEY_SET_store_cb
 *
 */

#if defined(__cplusplus)
extern "C" {
#endif

/* Algorithm identifier for XMSS-SHA2_10_256 */
#define OQS_SIG_STFL_alg_xmss_sha256_h10 "XMSS-SHA2_10_256"
#define OQS_SIG_STFL_alg_xmss_sha256_h16 "XMSS-SHA2_16_256"
#define OQS_SIG_STFL_alg_xmss_sha256_h20 "XMSS-SHA2_20_256"
#define OQS_SIG_STFL_alg_xmss_shake128_h10 "XMSS-SHAKE_10_256"
#define OQS_SIG_STFL_alg_xmss_shake128_h16 "XMSS-SHAKE_16_256"
#define OQS_SIG_STFL_alg_xmss_shake128_h20 "XMSS-SHAKE_20_256"
#define OQS_SIG_STFL_alg_xmss_sha512_h10 "XMSS-SHA2_10_512"
#define OQS_SIG_STFL_alg_xmss_sha512_h16 "XMSS-SHA2_16_512"
#define OQS_SIG_STFL_alg_xmss_sha512_h20 "XMSS-SHA2_20_512"
#define OQS_SIG_STFL_alg_xmss_shake256_h10 "XMSS-SHAKE_10_512"
#define OQS_SIG_STFL_alg_xmss_shake256_h16 "XMSS-SHAKE_16_512"
#define OQS_SIG_STFL_alg_xmss_shake256_h20 "XMSS-SHAKE_20_512"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_2 "XMSSMT-SHA2_20/2_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_4 "XMSSMT-SHA2_20/4_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_2 "XMSSMT-SHA2_40/2_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_4 "XMSSMT-SHA2_40/4_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_8 "XMSSMT-SHA2_40/8_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_3 "XMSSMT-SHA2_60/3_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_6 "XMSSMT-SHA2_60/6_256"
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_12 "XMSSMT-SHA2_60/12_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_2 "XMSSMT-SHAKE_20/2_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_4 "XMSSMT-SHAKE_20/4_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_2 "XMSSMT-SHAKE_40/2_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_4 "XMSSMT-SHAKE_40/4_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_8 "XMSSMT-SHAKE_40/8_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_3 "XMSSMT-SHAKE_60/3_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_6 "XMSSMT-SHAKE_60/6_256"
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_12 "XMSSMT-SHAKE_60/12_256"

/* Defined LMS parameter identifiers */
#define OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1 "LMS_SHA256_H5_W1" //"5/1"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2 "LMS_SHA256_H5_W2" //"5/2"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4 "LMS_SHA256_H5_W4" //"5/4"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8 "LMS_SHA256_H5_W8" //"5/8"

#define OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1 "LMS_SHA256_H10_W1" //"10/1"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2 "LMS_SHA256_H10_W2" //"10/2"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4 "LMS_SHA256_H10_W4" //"10/4"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8 "LMS_SHA256_H10_W8" //"10/8"

#define OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1 "LMS_SHA256_H15_W1" //"15/1"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2 "LMS_SHA256_H15_W2" //"15/2"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4 "LMS_SHA256_H15_W4" //"15/4"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8 "LMS_SHA256_H15_W8" //"15/8"

#define OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1 "LMS_SHA256_H20_W1" //"20/1"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2 "LMS_SHA256_H20_W2" //"20/2"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4 "LMS_SHA256_H20_W4" //"20/4"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8 "LMS_SHA256_H20_W8" //"20/8"

#define OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1 "LMS_SHA256_H25_W1" //"25/1"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2 "LMS_SHA256_H25_W2" //"25/2"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4 "LMS_SHA256_H25_W4" //"25/4"
#define OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8 "LMS_SHA256_H25_W8" //"25/8"

#define OQS_SIG_STFL_algs_length 48

/* Defined LM parameter identifiers */
/* Algorithm identifier for LMS-SHA256_N32_H5 */
#define OQS_SIG_STFL_alg_lms_sha256_n32_h5 "LMS-SHA256_N32_H5"   //0x00000005

typedef struct OQS_SIG_STFL_SECRET_KEY OQS_SIG_STFL_SECRET_KEY;

/**
 * Application provided function to securely store data
 * @param[in] sk_buf pointer to the data to be saved
 * @param[in] buf_len length of the the data to be store
 * @param[out] context pointer to application relevant data.
 * return OQS_SUCCESS if successful, otherwise OQS_ERROR
 */
typedef OQS_STATUS (*secure_store_sk)(uint8_t *sk_buf, size_t buf_len, void *context);

/**
 * Application provided function to lock secret key object serialize access
 * @param[in] sk pointer to secret key object to lock
 * @param[in] mutex pointer to mutex struct
 * return OQS_SUCCESS if successful, otherwise OQS_ERROR
 */
typedef OQS_STATUS (*lock_key)(void *mutex);

/**
 * Application provided function to unlock secret key object
 * @param[in] sk pointer to secret key object to unlock
 * @param[in] mutex pointer to mutex struct
 * return OQS_SUCCESS if successful, otherwise OQS_ERROR
 */
typedef OQS_STATUS (*unlock_key)(void *mutex);

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

	/** A local ordinal representing the OID parameter of the signature scheme. */
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
	OQS_STATUS (*keypair)(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

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
	OQS_STATUS (*sign)(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);

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
	OQS_STATUS (*sigs_remaining)(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);

	/**
	 * Total number of signatures
	 *
	 * @param[out] total The total number of signatures
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*sigs_total)(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

} OQS_SIG_STFL;

/**
 * @brief OQS_SIG_STFL_SECRET_KEY object for stateful signature schemes
 */

typedef struct OQS_SIG_STFL_SECRET_KEY {

	/** Associated signature object */
	OQS_SIG_STFL *sig;

	/* The (maximum) length, in bytes, of secret keys for this signature scheme. */
	size_t length_secret_key;

	/* The variant specific secret key data */
	void *secret_key_data;

	/* mutual exclusion struct */
	void *mutex;

	/* file storage handle */
	void *context;

	/**
	 * Secret Key retrieval Function
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @param[out] sk_len length of private key as a byte stream
	 * @param[out] sk_buf_ptr pointer to private key data as a byte stream
	 * @returns length of key material data available
	 * Caller deletes the buffer if memory was allocated.
	 */
	OQS_STATUS (*serialize_key)(const OQS_SIG_STFL_SECRET_KEY *sk, size_t *sk_len, uint8_t **sk_buf_ptr);

	/**
	 * Secret Key to internal structure Function
	 *
	 * @param[in] sk OQS_SIG_STFL_SECRET_KEY object
	 * @param[in] key_len length of the returned byte string
	 * @param[in] sk_buf The secret key data to populate key object
	 * @param[in] context application specific data
	 * @returns  status of the operation populated with key material none-zero length. Caller
	 * deletes the buffer. if sk_buf is NULL the function returns the length
	 */
	OQS_STATUS (*deserialize_key)(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf, void *context);

	/**
	 * Secret Key Locking Function
	 *
	 * @param[in] mutex application defined mutex
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*lock_key)(void *mutex);

	/**
	 * Secret Key Unlocking / Releasing Function
	 *
	 * @param[in]  mutex application defined mutex
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*unlock_key)(void *mutex);

	/**
	 * Store Secret Key Function
	 * Callback function used to securely store key data
	 * @param[in] sk_buf The serialized secret key data to secure store
	 * @param[in] buf_len length of data to secure
	 * @param[in] context aides the secure writing of data
	 *
	 * @return OQS_SUCCESS or OQS_ERROR
	 * Idealy written to secure device
	 */
	OQS_STATUS (*secure_store_scrt_key)(uint8_t *sk_buf, size_t buf_len, void *context);

	/**
	 * Secret Key free internal variant specific data
	 *
	 * @param[in] sk The secret key represented as OQS_SIG_STFL_SECRET_KEY object
	 * @return none
	 */
	void (*free_key)(OQS_SIG_STFL_SECRET_KEY *sk);

	/**
	 * Set Secret Key store callback Function
	 *
	 * @param[in] sk secret key pointer to be updated
	 * @param[in] store_cb callback pointer
	 * @param[in] context secret key specific data/identifier
	 */
	void (*set_scrt_key_store_cb)(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);
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
OQS_API OQS_STATUS OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

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
OQS_API OQS_STATUS OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);

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
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_sigs_remaining(const OQS_SIG_STFL *sig, unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);

/**
 * * Total number of signatures
 *
 * @param[in] sig The OQS_SIG_STFL object representing the signature scheme.
 * @param[out] max The number of remaining signatures
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_SIG_STFL_sigs_total(const OQS_SIG_STFL *sig, unsigned long long *max, const OQS_SIG_STFL_SECRET_KEY *secret_key);

/**
 * Frees an OQS_SIG_STFL object that was constructed by OQS_SIG_STFL_new.
 *
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
 * Frees an OQS_SIG_STFL_SECRET_KEY **inner** data that was constructed by OQS_SECRET_KEY_new.
 *
 * @param[in] sig The OQS_SIG_STFL_SECRET_KEY object to free.
 * @return OQS_SUCCESS if successful, or OQS_ERROR if the object could not be freed.
 */
void OQS_SECRET_KEY_XMSS_free(OQS_SIG_STFL_SECRET_KEY *sk);

/**
 * Frees an OQS_SIG_STFL_SECRET_KEY object that was constructed by OQS_SECRET_KEY_new.
 *
 * @param[in] sig The OQS_SIG_STFL_SECRET_KEY object to free.
 * @return OQS_SUCCESS if successful, or OQS_ERROR if the object could not be freed.
 */
OQS_API void OQS_SIG_STFL_SECRET_KEY_free(OQS_SIG_STFL_SECRET_KEY *sk);

/**
 * OQS_SIG_STFL_SECRET_KEY_SET_lock .
 *
 * Sets function to prevent multiple processes from using the sk at the same time.
 *
 * @param[in] sk secret key pointer to be updated
 * @param[in] lock function pointer
 *
 */
void OQS_SIG_STFL_SECRET_KEY_SET_lock(OQS_SIG_STFL_SECRET_KEY *sk, lock_key lock);

/**
 * OQS_SIG_STFL_SECRET_KEY_SET_unlock .
 *
 * Sets function to prevent multiple processes from using the sk at the same time.
 *
 * @param[in] sk secret key pointer to be updated
 * @param[in] unlock function pointer
 *
 */
void OQS_SIG_STFL_SECRET_KEY_SET_unlock(OQS_SIG_STFL_SECRET_KEY *sk, unlock_key unlock);

/**
 * OQS_SIG_STFL_SECRET_KEY_SET_mutex .
 *
 * Sets function to prevent multiple processes from using the sk at the same time.
 *
 * @param[in] sk secret key pointer to be updated
 * @param[in] mutex function pointer
 *
 */
void OQS_SIG_STFL_SECRET_KEY_SET_mutex(OQS_SIG_STFL_SECRET_KEY *sk, void *mutex);

/**
 * OQS_SIG_STFL_SECRET_KEY_lock .
 *
 * Locks sk so only one application that holds the lock can access it.
 *
 * @param[in] sk secret key pointer to be locked
 * @return OQS_SUCCESS if successful, or OQS_ERROR if the object fails to apply the lock
 *
 */
OQS_STATUS OQS_SIG_STFL_SECRET_KEY_lock(OQS_SIG_STFL_SECRET_KEY *sk);

/**
 * OQS_SIG_STFL_SECRET_KEY_unlock .
 *
 * Unlocks the resouces so that th enext process can access it.
 *
 * @param[in] sk secret key pointer
 * @return OQS_SUCCESS if successful, or OQS_ERROR if the object fails to release the lock
 *
 */
OQS_STATUS OQS_SIG_STFL_SECRET_KEY_unlock(OQS_SIG_STFL_SECRET_KEY *sk);

/**
 * OQS_SIG_STFL_SECRET_KEY_SET_store_cb .
 *
 * Can be called after creating a new stateful secret key has been generated.
 * Allows the lib to securely store and update secret key after a sign operation.
 *
 * @param[in] sk secret key pointer to be updated
 * @param[in] store_cb callback pointer
 * @param[in] context secret key specific data/identifier
 *
 */
void OQS_SIG_STFL_SECRET_KEY_SET_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);

OQS_API OQS_STATUS OQS_SECRET_KEY_STFL_serialize_key(const OQS_SIG_STFL_SECRET_KEY *sk,  size_t *sk_len, uint8_t **sk_buf);

/* Insert lms byte string in an LMS secret key object */
OQS_API OQS_STATUS OQS_SECRET_KEY_STFL_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, size_t key_len, const uint8_t *sk_buf, void *context);

#if defined(__cplusplus)
} // extern "C"
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS
#include <oqs/sig_stfl_xmss.h>
#endif // OQS_ENABLE_SIG_STFL_XMSS

#ifdef OQS_ENABLE_SIG_STFL_LMS
#include <oqs/sig_stfl_lms.h>
#endif // OQS_ENABLE_SIG_STFL_LMS

#endif /* OQS_SIG_STATEFUL_H */
