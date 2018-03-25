/**
 * \file kem.h
 * \brief Key encapsulation mechanisms
 *
 * The file `src/kem/example_kem.c` contains two examples on using the OQS_KEM API.
 *
 * The first example uses the individual scheme's algorithms directly and uses
 * no dynamic memory allocation -- all buffers are allocated on the stack, with
 * sizes indicated using preprocessor macros.  Since algorithms can be disabled at
 * compile-time, the programmer should wrap the code in \#ifdefs.
 *
 * The second example uses an OQS_KEM object to use an algorithm specified at
 * runtime.  Therefore it uses dynamic memory allocation -- all buffers must be
 * malloc'ed by the programmer, with sizes indicated using the corresponding length
 * member of the OQS_KEM object in question.  Since algorithms can be disabled at
 * compile-time, the programmer should check that the OQS_KEM object is not `NULL`.
 */

#ifndef __OQS_KEM_H
#define __OQS_KEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

/**
 * Identifiers for available key encapsulation mechanisms in liboqs.  Used with OQS_KEM_new.
 *
 * Note that algorithm identifiers are present in this enum even when the algorithm is disabled
 * at compile time.
 *
 * The order and numbers of the identifiers (except `OQS_KEM_alg_last`) should remain fixed
 * for binary compability across versions, even if an algorithm is removed.
 *
 * New algorithms should have identifiers added immediately before `OQS_KEM_alg_last`.
 */
enum OQS_KEM_alg_name {
	OQS_KEM_alg_default = 0,
	OQS_KEM_alg_dummy1 = 1,
	OQS_KEM_alg_dummy2 = 2,
	OQS_KEM_alg_frodokem_640_aes = 3,
	OQS_KEM_alg_frodokem_976_aes = 4,
	OQS_KEM_alg_frodokem_640_cshake = 5,
	OQS_KEM_alg_frodokem_976_cshake = 6,
	OQS_KEM_alg_newhope_512_cca_kem = 7,
	OQS_KEM_alg_newhope_1024_cca_kem = 8,
	// EDIT-WHEN-ADDING-KEM
	OQS_KEM_alg_last = 9
};

/**
 * Key encapsulation mechanism object
 */
typedef struct OQS_KEM {

	/** Printable string representing the name of the key encapsulation mechanism. */
	char *method_name;

	/** The NIST security level (1, 2, 3, 4, 5) claimed in this algorithm's original NIST submission. */
	uint8_t claimed_nist_level;

	/** Whether the KEM offers IND-CCA security (TRUE) or IND-CPA security (FALSE). */
	bool ind_cca;

	/** The (maximum) length, in bytes, of public keys for this KEM. */
	size_t length_public_key;
	/** The (maximum) length, in bytes, of secret keys for this KEM. */
	size_t length_secret_key;
	/** The (maximum) length, in bytes, of ciphertexts for this KEM. */
	size_t length_ciphertext;
	/** The (maximum) length, in bytes, of shared secrets for this KEM. */
	size_t length_shared_secret;

	// clang-format off

	/**
	 * Keypair generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `public_key` and
	 * `secret_key`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_KEM_*_length_*`.
	 *
	 * @param[out] public_key The public key represented as a byte string.
	 * @param[out] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);

	/**
	 * Encapsulation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `ciphertext` and
	 * `shared_secret`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_KEM_*_length_*`.
	 *
	 * @param[out] ciphertext The ciphertext (encapsulation) represented as a byte string.
	 * @param[out] shared_secret The shared secret represented as a byte string.
	 * @param[in] public_key The public key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*encaps)(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);

	/**
	 * Decapsulation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `shared_secret`, based
	 * on the `length_*` members in this object or the per-scheme compile-time macros
	 * `OQS_KEM_*_length_*`.
	 *
	 * @param[out] shared_secret The shared secret represented as a byte string.
	 * @param[in] ciphertext The ciphertext (encapsulation) represented as a byte string.
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*decaps)(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
	// clang-format on

} OQS_KEM;

/**
 * Consturcts an OQS_KEM object for a particular algorithm.
 *
 * Callers should always check whether the return value is `NULL`, which indicates either than an
 * invalid algorithm identifier was provided, or that the requested algorithm was disabled at compile-time.
 *
 * @param[in] alg_name Identifier of the desired KEM algorithm.
 * @return An OQS_KEM for the particular algorithm, or `NULL` if the algorithm has been disabled at compile-time.
 */
OQS_KEM *OQS_KEM_new(enum OQS_KEM_alg_name alg_name);

/**
 * Keypair generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `public_key` and
 * `secret_key`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] public_key The public key represented as a byte string.
 * @param[out] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key);

/**
 * Encapsulation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `ciphertext` and
 * `shared_secret`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] ciphertext The ciphertext (encapsulation) represented as a byte string.
 * @param[out] shared_secret The shared secret represented as a byte string.
 * @param[in] public_key The public key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);

/**
 * Decapsulation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `shared_secret`, based
 * on the `length_*` members in this object or the per-scheme compile-time macros
 * `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] shared_secret The shared secret represented as a byte string.
 * @param[in] ciphertext The ciphertext (encapsulation) represented as a byte string.
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

/**
 * Frees an OQS_KEM object that was constructed by OQS_KEM_new.
 *
 * @param[in] kem The OQS_KEM object to free.
 */
void OQS_KEM_free(OQS_KEM *kem);

#include <oqs/kem_dummy1.h>
#include <oqs/kem_dummy2.h>
#include <oqs/kem_frodokem.h>
#include <oqs/kem_newhopenist.h>
// EDIT-WHEN-ADDING-KEM

#endif // __OQS_KEM_H
