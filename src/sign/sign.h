/**
 * \file sign.h
 * \brief Header defining the API for generic OQS Signature
 */

#ifndef __OQS_SIGN_H
#define __OQS_SIGN_H

#include <stddef.h>
#include <stdint.h>

#include <oqs/rand.h>

typedef struct OQS_SIGN OQS_SIGN;

/**
 * OQS signature object
 */
typedef struct OQS_SIGN {

	/**
	 * PRNG
	 */
	OQS_RAND *rand;

	/**
	 * Specifies the name of the signature scheme.
	 */
	char *method_name;

	/**
	 * Classical security in terms of the number of bits provided by the signature 
	 * scheme.
	 */
	uint16_t estimated_classical_security;

	/**
	 *  Equivalent quantum security in terms of the number of bits provided by the 
	 *  signature scheme.
	 */
	uint16_t estimated_quantum_security;

	/**
	 * An instance-specific seed, if any.
	 */
	const uint8_t *seed;

	/**
	 * Size of instance-specific seed, if any.
	 */
	size_t seed_len;

	/**
	 * Named parameters for this signature scheme instance, if any.
	 */
	const char *named_parameters;

	/**
	 * Opaque pointer for passing around instance-specific data.
	 */
	void *params;

	/**
	 * Opaque pointer for passing around any computation context.
	 */
	void *ctx;

	/**
	 * Pointer to a function for private and verification key generation.
	 *
	 * @param s                Signature structure
	 * @param priv             Private key
	 * @param verif            Verification key
	 * @param verif_len        Verification key length
	 * @return                 1 on success, or 0 on failure
	 */
	int (*keygen)(OQS_SIGN *s, void **priv, uint8_t **verif, size_t *verif_len);

	/**
	 * Pointer to a function for signing messages.
	 *
	 * @param s                Signature structure
	 * @param priv             Private key
	 * @param msg              Message
	 * @param msg_len          Message length
	 * @param sig              Signature
	 * @param sig_len          Signature length
	 * @return                 1 on success, or 0 on failure
	 */
	int (*sign)(OQS_SIGN *s, void *priv, uint8_t *msg, const size_t msg_len, uint8_t **sig, size_t *sig_len);

	/**
	 * Pointer to a function for verifying a signature.
	 *
	 * @param s                Signature structure
	 * @param verif            Verification key
	 * @param verif_len        Verification key length
	 * @param msg              Message
	 * @param msg_len          Message length
	 * @param sig              Signature
	 * @param sig_len          Signature length
	 * @return                 1 on success, or 0 on failure
	 */
	int (*verify)(OQS_SIGN *s, uint8_t *verif, const size_t verif_len, uint8_t *msg, const size_t msg_len, uint8_t *sig, const size_t sig_len);

	/**
	 * Pointer to a function for freeing the private key.
	 *
	 * @param s                Signature structure
	 * @param priv             Private key
	 */
	void (*priv_free)(OQS_SIGN *s, void *priv);

	/**
	 * Pointer to a function for freeing the allocated signature structure.
	 *
	 * @param s                Signature structure
	 */
	void (*free)(OQS_SIGN *s);

} OQS_SIGN;

/**
 * Allocate a new signature object.
 *
 * @param rand               Random number generator.
 * @param seed               An instance-specific seed, if any, or NULL.
 * @param seed_len           The length of seed, or 0.
 * @param named_parameters   Name or description of method-specific parameters to use for this instance (as a NULL-terminated C string), if any, or NULL.
 * @return                   The object on success, or NULL on failure.
 */
OQS_SIGN *OQS_SIGN_new(OQS_RAND *rand, const uint8_t *seed, const size_t seed_len, const char *named_parameters);

int OQS_SIGN_keygen(OQS_SIGN *s, void **priv, uint8_t **verif, size_t *verif_len);
int OQS_SIGN_sign(OQS_SIGN *s, void *priv, uint8_t *msg, const size_t msg_len, uint8_t **sig, size_t *sig_len);
int OQS_SIGN_verify(OQS_SIGN *s, uint8_t *verif, const size_t verif_len, uint8_t *msg, const size_t msg_len, uint8_t *sig, const size_t sig_len);

void OQS_SIGN_priv_free(OQS_SIGN *s, void *priv);
void OQS_SIGN_free(OQS_SIGN *s);

#endif
