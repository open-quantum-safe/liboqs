/**
 * \file sig.h
 * \brief Header defining the API for generic OQS Signature
 */

#ifndef __OQS_SIG_H
#define __OQS_SIG_H

#include <stddef.h>
#include <stdint.h>

#include <oqs/rand.h>

enum OQS_SIG_scheme_id {
	OQS_SIG_scheme_id_default,
};

typedef struct OQS_SIG OQS_SIG;

/**
 * OQS signature object
 */
typedef struct OQS_SIG {

	/**
	 * Specifies the unique id for the signature scheme and its parameters.
	 */
	char *scheme_id;

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
	 * Pointer to a function for public and private signature key generation.
	 *
	 * @param rand             The random number generator.
	 * @param priv             The signer's private key.
	 * @param pub              The signer's public key.
	 * @return                 1 on success, or 0 on failure
	 */
	int (*keygen)(const OQS_RAND *rand, uint8_t *priv, uint8_t *pub);

	/**
	 * Pointer to a function for signature generation.
	 *
	 * @param rand             The random number generator.
	 * @param priv             The signer's private key.
	 * @param msg              The message to sign.
	 * @param msg_len          Length of the message to sign.
	 * @param sig              The generated signature. Must be allocated by the caller, or NULL to learn how much space is needed, as returned in sig_len.
	 * @param sig_len          In: length of sig, out: length of the generated signature.
	 * @return                 1 on success, or 0 on failure.
	 */
	int (*sign)(const OQS_RAND *rand, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len);

	/**
	 * Pointer to a function for signature verification.
	 *
	 * @param pub              The signer's public key
	 * @param msg              The signed message
	 * @param msg_len          Length of the signed message
	 * @param sig              The signature to verify
	 * @param sig_len          Length of the signature to verify
	 * @return                 1 on success, or 0 on failure
	 */
	int (*verify)(const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len);

} OQS_SIG;

/**
 * Returns a new signature object.
 *
 * @param scheme_id          The id of the signature scheme to be instantiated.
 * @return                   The object on success, or NULL on failure.
 */
OQS_SIG *OQS_SIG_get(enum OQS_SIG_scheme_id scheme_id);

/**
 * Generates a new signature key pair.
 * @param s                  Pointer to the signature object.
 * @param rand               Pointer to the random number generator.
 * @param priv               Pointer where the generated private key will be stored. Caller 
 *                           must have allocated s->priv_key_len bytes.
 * @param pub                Pointer where the generated public key will be stored. Caller 
 *                           must have allocated s->pub_key_len bytes.
 * @return                   1 on success, 0 on failure.
 */
int OQS_SIG_keygen(const OQS_SIG *s, const OQS_RAND *rand, uint8_t *priv, uint8_t *pub);

/**
 * Generates a new signature.
 * @param s         Pointer to the signature object.
 * @param rand      Pointer to the random number generator.
 * @param priv      Pointer to the signer's private key, of expected length `s->priv_key_len` bytes.
 * @param msg       Pointer to the message to sign.
 * @param msg_len   Length of the message to sign `msg`.
 * @param sig       Pointer where the generated signature will be stored. Caller must have allocated `s->max_sig_len` bytes.
 * @param sig_len   Pointer to the length of the generated signature. 
 * @return          1 on success, 0 on failure.
 */
int OQS_SIG_sign(const OQS_SIG *s, const OQS_RAND *rand, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len);

/**
 * Verifies a signature.
 * @param s         Pointer to the signature object.
 * @param pub       Pointer to the signer's public key, of expected length `s->pub_key_len` bytes.
 * @param msg       Pointer to the signed message.
 * @param msg_len   Length of the signed message `msg`.
 * @param sig       Pointer to the signature.
 * @param sig_len   Length of the signature. 
 * @return          1 on success, 0 on failure.
 */
int OQS_SIG_verify(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len);

#endif
