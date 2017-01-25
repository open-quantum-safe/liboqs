/**
 * \file sig.h
 * \brief Header defining the API for generic OQS Signature
 */

#ifndef __OQS_SIG_H
#define __OQS_SIG_H

#include <stddef.h>
#include <stdint.h>

#include <oqs/rand.h>

enum OQS_SIG_alg_name {
	OQS_SIG_alg_default,
};

typedef struct OQS_SIG OQS_SIG;

/**
 * OQS signature object
 */
typedef struct OQS_SIG {

	/**
	 * PRNG
	 */
	OQS_RAND *rand;

	/**
	 * Specifies the name of the sig method
	 */
	char *method_name;

	/**
	 * Classical security in terms of the number of bits provided by the
	 * signature method.
	 */
	uint16_t estimated_classical_security;

	/**
	 *  Equivalent quantum security in terms of the number of bits provided by the
	 *  signature method.
	 */
	uint16_t estimated_quantum_security;

	/**
	 *  Private key length.
	 */
	uint16_t priv_key_len;

	/**
	 *  Public key length.
	 */
	uint16_t pub_key_len;

	/**
	 *  Maximum signature length.
	 */
	uint16_t max_sig_len;

	/**
	 * Named parameters for this key exchange method instance, if any.
	 */
	char *named_parameters;

	/**
	 * Opaque pointer for passing around instance-specific data
	 */
	void *params;

	/**
	 * Opaque pointer for passing around any computation context
	 */
	void *ctx;

	/**
	 * Pointer to a function for public and private signature key generation.
	 *
	 * @param s                Signature structure
	 * @param priv             The private key
	 * @param pub              The public key
	 * @return                 1 on success, or 0 on failure
	 */
	int (*keygen)(const OQS_SIG *s, uint8_t **priv, uint8_t **pub);

	/**
	 * Pointer to a function for signature generation.
	 *
	 * @param s                Signature structure
	 * @param priv             The signer's private key
	 * @param msg              The message to sign
	 * @param msg_len          Length of the message to sign
	 * @param sig              The generated signature
	 * @param sig_len          Length of the generated signature
	 * @return                 1 on success, or 0 on failure
	 */
	int (*sign)(const OQS_SIG *s, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t **sig, size_t *sig_len);

	/**
	 * Pointer to a function for signature verification.
	 *
	 * @param s                Signature structure
	 * @param pub              The signer's public key
	 * @param msg              The signed message
	 * @param msg_len          Length of the signed message
	 * @param sig              The signature to verify
	 * @param sig_len          Length of the signature to verify
	 * @return                 1 on success, or 0 on failure
	 */
	int (*verify)(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len);

	/**
	 * Pointer to a function for freeing the allocated signature structure
	 *
	 * @param s                Signature structure
	 */
	void (*free)(OQS_SIG *s);

} OQS_SIG;

/**
 * Allocate a new signature object.
 *
 * @param rand               Random number generator.
 * @param alg_name           Algorithm to be instantiated
 * @param named_parameters   Name or description of method-specific parameters
 *                           to use for this instance (as a NULL-terminated C string),
 *                           if any, or NULL.
 * @return                   The object on success, or NULL on failure.
 */
OQS_SIG *OQS_SIG_new(OQS_RAND *rand, enum OQS_SIG_alg_name alg_name, const char *named_parameters);
int OQS_SIG_keygen(const OQS_SIG *s, uint8_t **priv, uint8_t **pub);
int OQS_SIG_sign(const OQS_SIG *s, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t **sig, size_t *sig_len);
int OQS_SIG_verify(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len);
void OQS_SIG_free(OQS_SIG *s);

#endif
