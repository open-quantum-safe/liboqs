/*! @file picnic.h
 *  @brief Public API for the Picnic signature scheme.
 *
 *  This file is part of the reference implementation of the Picnic and Fish
 *  signature schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives <br/>
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha <br/>
 *  Cryptology ePrint Archive: Report 2017/279 <br/>
 *  http://eprint.iacr.org/2017/279 <br/>
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *
 */

// Doxygen mainpage:
/** @mainpage
 *
 *  This is a reference implementation of the Picnic and Fish signature
 *  schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives <br/>
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha <br/>
 *  Cryptology ePrint Archive: Report 2017/279 <br/>
 *  http://eprint.iacr.org/2017/279 <br/>
 *
 *  The library API is documented in \ref picnic.h.
 *
 *  Authors: Steven Goldfeder and Greg Zaverucha <br/>
 *  May 2017
 */

#ifndef PICNIC_H
#define PICNIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/* Maximum lengths in bytes */
#define PICNIC_MAX_PUBLICKEY_SIZE 64     /**< Largest public key size, in bytes */
#define PICNIC_MAX_PRIVATEKEY_SIZE 64    /**< Largest private key size, in bytes */
#define PICNIC_MAX_SIGNATURE_SIZE 263786 /**< Largest signature size, in bytes */

/** Parameter sets */
typedef enum picnic_params_t {
	PARAMETER_SET_INVALID = 0,
	LowMC_256_256_42_14_FS = 1, /**< LowMC with Fiat-Shamir balanced number of s-boxes (42) and rounds (14). */
	LowMC_256_256_42_14_UR = 2, /**< LowMC with Unruh balanced number of s-boxes (42) and rounds (14). */
	LowMC_256_256_1_316_FS = 3, /**< LowMC with Fiat-Shamir and a small number of s-boxes (1) with a large number of rounds (316). */
	LowMC_256_256_1_316_UR = 4, /**< LowMC with Unruh and a small number of s-boxes (1) with a large number of rounds (316). */
	LowMC_256_256_10_38_FS = 5, /**< LowMC with Fiat-Shamir balanced number of s-boxes (10) and rounds (38). */
	LowMC_256_256_10_38_UR = 6, /**< LowMC with Unruh balanced number of s-boxes (10) and rounds (38). */
	PARAMETER_SET_MAX_INDEX = 7
} picnic_params_t;

/** Public key */
typedef struct {
	picnic_params_t params;                            /**< The parameter set used with this public key. */
	uint8_t pubInput[PICNIC_MAX_PUBLICKEY_SIZE / 2];   /**< The input plaintext block to LowMC. */
	uint8_t ciphertext[PICNIC_MAX_PUBLICKEY_SIZE / 2]; /**< The encryption of pubInput under the private key. */
} picnic_publickey_t;

/** Private key */
typedef struct {
	picnic_params_t params;                   /**< The parameter set used with this private key. */
	uint8_t data[PICNIC_MAX_PRIVATEKEY_SIZE]; /**< The private key data. */
	picnic_publickey_t *pk;                   /**< The corresponding public key.  */
} picnic_privatekey_t;

/**
 * Library initialization.
 * Must be called before calling any of the other functions.
 *
 * Used to initialize OpenSSL and load LowMC constants and precomputed tables.
 * Initialization is specific to a parameter set.
 *
 * @param[in] params The parameter set that will be used.
 * @param[in] path   (optional) a path where pre-computed data is stored. If
 * provided, must contain the trailing slash character(s) ('/' or '\\').
 * @param[in] flags  (optional) If flags is 1, OpenSSL is not initialized. This
 * is useful if the application using picnic.h also uses OpenSSL, and does not
 * want the picnic_init to perform OpenSSL initialization.  If flags is 1,
 * fucntions in picnic.h assume that OpenSSL has been initialized, and that the
 * SHA-256, and AES algorithms are available.
 *
 * @return 0 on success, nonzero for error
 *
 * @see picnic_shutdown()
 */
int picnic_init(picnic_params_t params, const char *path, unsigned int flags);

/**
 * Free resources allocated by picnic_init().
 *
 * @param[in] flags  If flags is 1, the OpenSSL shutdown steps are not
 * performed. This is useful if the application using picnic.h also uses
 * OpenSSL, and does not want picnic_shutdown to shutdown OpenSSL.
 *
 * @see picnic_init()
 */
void picnic_shutdown(unsigned int flags);

/**
 * Get a string representation of the parameter set.
 *
 * @param parameters A parameter set
 *
 * @return A null-terminated string describing the parameter set.
 */
const char *picnic_get_param_name(picnic_params_t parameters);

/* Signature API */

/**
 * Key generation function.
 * Generates a public and private key pair, for the specified parameter set.
 *
 * @param[in]  parameters The parameter set to use when generating a key.
 * @param[out] pk         The new public key.
 * @param[out] sk         The new private key.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 *
 * @see picnic_verify(), picnic_sign()
 */
int picnic_keygen(picnic_params_t parameters, picnic_publickey_t *pk,
                  picnic_privatekey_t *sk);

/**
 * Signature function.
 * Signs a message with the given keypair.
 *
 * @param[in] sk      The signer's private key.
 * @param[in] message The message to be signed.
 * @param[in] message_len The length of the message, in bytes.
 * @param[out] signature A buffer to hold the signature. The required size does
 * not exceed PICNIC_MAX_SIGNATURE_SIZE bytes.  The specific max number of
 * bytes required for a parameter set is given by picnic_signature_size(). Note
 * that the length of each signature varies slightly, for the parameter sets
 * using the FS transform.  The parameter sets using the Unruh transform have a
 * fixed length.
 * @param[in,out] signature_len The length of the provided signature buffer.
 * On success, this is set to the number of bytes written to the signature buffer.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 *
 * @see picnic_verify(), picnic_keygen(), picnic_signature_size()
 */
int picnic_sign(picnic_privatekey_t *sk, const uint8_t *message, size_t message_len,
                uint8_t *signature, size_t *signature_len);

/**
 * Get the number of bytes required to hold a signature.
 *
 * @param[in] parameters The parameter set of the signature.
 *
 * @return The number of bytes required to hold the signature created by
 * picnic_sign
 *
 * @note The size of signatures with parameter sets using the FS transform vary
 *       slightly based on the random choices made during signing.  This function
 *       will return a suffcient number of bytes to hold a signature, and the
 *       picnic_sign() function returns the exact number used for a given signature.
 *
 * @see picnic_sign()
 */
size_t picnic_signature_size(picnic_params_t parameters);

/**
 * Verification function.
 * Verifies a signature is valid with respect to a public key and message.
 *
 * @param[in] pk      The signer's public key.
 * @param[in] message The message the signature purpotedly signs.
 * @param[in] message_len The length of the message, in bytes.
 * @param[in] signature The signature to verify.
 * @param[in] signature_len The length of the signature.
 *
 * @return Returns 0 for success, indicating a valid signature, or a nonzero
 * value indicating an error or an invalid signature.
 *
 * @see picnic_sign(), picnic_keygen()
 */
int picnic_verify(picnic_publickey_t *pk, const uint8_t *message, size_t message_len,
                  const uint8_t *signature, size_t signature_len);

/**
 * Serialize a public key.
 *
 * @param[in]  key The public key to serialize
 * @param[out] buf The buffer to write the key to.
 *                 Must have size at least PICNIC_MAX_PUBLICKEY_SIZE + 1 bytes.
 * @param[in]  buflen The length of buf, in bytes
 *
 * @return Returns the number of bytes written.
 */
int picnic_write_public_key(const picnic_publickey_t *key, uint8_t *buf, size_t buflen);

/**
 * De-serialize a public key.
 *
 * @param[out]  key The public key object to be populated.
 * @param[in] buf The buffer to read the public key from.
 *                 Must be at least PICNIC_MAX_PUBLICKEY_SIZE + 1 bytes.
 * @param[in]  buflen The length of buf, in bytes
 *
 * @return Returns the number of bytes written.
 */
int picnic_read_public_key(picnic_publickey_t *key, const uint8_t *buf, size_t buflen);

/**
 * Serialize a private key.
 *
 * @param[in]  key The private key to serialize
 * @param[out] buf The buffer to write the key to.
 *                 Must have size at least PICNIC_MAX_PRIVATEKEY_SIZE + 1 bytes.
 * @param[in]  buflen The length of buf, in bytes
 *
 * @return Returns the number of bytes written.
 */
int picnic_write_private_key(const picnic_privatekey_t *key, uint8_t *buf, size_t buflen);

/**
 * De-serialize a private key.
 *
 * @param[out]  key The private key object to be populated
 * @param[in] buf The buffer to read the key from.
 *                 Must have size at least PICNIC_MAX_PRIVATEKEY_SIZE + 1 bytes.
 * @param[in]  buflen The length of buf, in bytes
 * @param[in]  publickey The corresponding public key
 *
 * @return Returns 0 on success, or a nonzero value indicating an error.
 */
int picnic_read_private_key(picnic_privatekey_t *key, const uint8_t *buf, size_t buflen, picnic_publickey_t *publickey);

/**
 * Check that a key pair is valid.
 *
 * @param[in] privatekey The private key to check
 * @param[in] publickey The public key to check
 *
 * @return Returns 0 if the key pair is valid, or a nonzero value indicating an error
 */
int picnic_validate_keypair(const picnic_privatekey_t *privatekey, const picnic_publickey_t *publickey);

/**
 * picnic_random_bytes is used to generate random bytes in key generation and
 * signing. See the provided implementation "random_bytes_default_openssl"
 * based on OpenSSL's RAND_bytes.
 *
 * To use another RNG, make sure it has the same behavior as
 * random_bytes_default_openssl, and change the definition of
 * picnic_random_bytes.
 */
#define picnic_random_bytes random_bytes_default_openssl

/*
 * Note signing digests and streaming APIs
 *
 * Sometimes signaure APIs include "sign_digest" and "verify_digest" functions,
 * that allow the caller to hash the message themselves and provide the digest
 * to the sign/verify functions.  Example use cases for these APIs are
 * applications that must sign very large messages, which don't fit in memory,
 * or that must signa long stream data.  Applications that really need this
 * behavior could  provide a digest to the sign/verify APIs provided here.
 * This is discouraged however, because the message will then be hashed twice,
 * the challenge used in the proof will be:
 *    c = H(random, per-signature values || H(M))
 * instead of
 *    c = H(random, per-signature values || M).
 * In the former case, a collision for H immediately gives a forgery (in the
 * GMR security game, but not necessarily in all applications). For this reason
 * the latter case arguably provides better security.
 *
 * A better way to implement a streaming API (not provided by this library)
 * would be to hash the first part of the challenge, then continue hashing the
 * message as a stream, then finish computing the signature.
 *
 */

#ifdef __cplusplus
}
#endif

#endif /*PICNIC_H*/
