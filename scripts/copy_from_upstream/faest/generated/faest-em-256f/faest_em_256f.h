/*
 *  SPDX-License-Identifier: MIT
 */

// clang-format off
#ifndef FAEST_EM_256F_H
#define FAEST_EM_256F_H
// clang-format on

#include "faest_defines.h"

#include <stddef.h>
#include <stdint.h>

FAEST_BEGIN_C_DECL

// clang-format off

/**
 * Size of the public key in bytes.
 */
#define FAEST_EM_256F_PUBLIC_KEY_SIZE 64
/**
 * Size of the private key in bytes.
 */
#define FAEST_EM_256F_PRIVATE_KEY_SIZE 64
/**
 * Size of the signature in bytes.
 */
#define FAEST_EM_256F_SIGNATURE_SIZE 18084

/**
 * Unpacked private key with pre-computed OWF output and witness.
 */
typedef struct {
  uint8_t owf_input[32];
  uint8_t owf_key[256 / 8];
  uint8_t owf_output[32];
  uint8_t witness[1792 / 8];
} faest_em_256f_unpacked_private_key_t;

/* Signature API */

/**
 * Key generation function.
 * Generates a public and private key pair, for the specified parameter set.
 *
 * @param[out] pk         The new public key.
 * @param[out] sk         The new private key.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_keygen(uint8_t* pk, uint8_t* sk);

/**
 * Unpack a private key key.
 * Compute the internal witness for faster signing operations.
 *
 * @param[out] unpacked_sk  The unpacked private key.
 * @param[in] sk            The private key.
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_unpack_private_key(faest_em_256f_unpacked_private_key_t* unpacked_sk, const uint8_t* sk);

/**
 * Signature function.
 * Signs a message with the private key. Samples rho internally.
 *
 * @param[in] sk      The signer's private key.
 * @param[in] message The message to be signed.
 * @param[in] message_len The length of the message, in bytes.
 * @param[out] signature A buffer to hold the signature. The specific max number of
 * bytes required for a parameter set is given by FAEST_EM_256F_SIGNATURE_SIZE.
 * @param[in,out] signature_len The length of the provided signature buffer.
 * On success, this is set to the number of bytes written to the signature buffer.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 *
 * @see faest_em_256f_verify(), faest_em_256f_keygen()
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_sign(const uint8_t* sk, const uint8_t* message, size_t message_len, uint8_t* signature, size_t* signature_len);

/**
 * Signature function (with custom randomness input).
 * Signs a message with the private key.
 *
 * @param[in] sk      The signer's private key.
 * @param[in] message The message to be signed.
 * @param[in] message_len The length of the message, in bytes.
 * @param[in] rho     Additonal randomness; providing randomness renders the signature non-determinstic
 * @param[in] rho_len Length of rho, in bytes.
 * @param[out] signature A buffer to hold the signature. The specific max number of
 * bytes required for a parameter set is given by FAEST_EM_256F_SIGNATURE_SIZE.
 * @param[in,out] signature_len The length of the provided signature buffer.
 * On success, this is set to the number of bytes written to the signature buffer.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 *
 * @see faest_em_256f_verify(), faest_em_256f_keygen()
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_sign_with_randomness(const uint8_t* sk, const uint8_t* message, size_t message_len, const uint8_t* rho, size_t rho_len, uint8_t* signature, size_t* signature_len);

/**
 * Signature function.
 * Signs a message with the given private key. Samples rho internally.
 *
 * @param[in] unpacked_sk The signer's private key.
 * @param[in] message The message to be signed.
 * @param[in] message_len The length of the message, in bytes.
 * @param[out] signature A buffer to hold the signature. The specific max number of
 * bytes required for a parameter set is given by FAEST_EM_256F_SIGNATURE_SIZE.
 * @param[in,out] signature_len The length of the provided signature buffer.
 * On success, this is set to the number of bytes written to the signature buffer.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 *
 * @see faest_em_256f_verify(), faest_em_256f_keygen(), faest_em_256f_unpack_private_key()
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_unpacked_sign(const faest_em_256f_unpacked_private_key_t* unpacked_sk, const uint8_t* message, size_t message_len, uint8_t* signature, size_t* signature_len);

/**
 * Signature function (with custom randomness input).
 * Signs a message with the given private key.
 *
 * @param[in] unpacked_sk The signer's private key.
 * @param[in] message The message to be signed.
 * @param[in] message_len The length of the message, in bytes.
 * @param[in] rho     Additonal randomness; providing randomness renders the signature non-determinstic
 * @param[in] rho_len Length of rho, in bytes.
 * @param[out] signature A buffer to hold the signature. The specific max number of
 * bytes required for a parameter set is given by FAEST_EM_256F_SIGNATURE_SIZE.
 * @param[in,out] signature_len The length of the provided signature buffer.
 * On success, this is set to the number of bytes written to the signature buffer.
 *
 * @return Returns 0 for success, or a nonzero value indicating an error.
 *
 * @see faest_em_256f_verify(), faest_em_256f_keygen(), faest_em_256f_unpack_private_key()
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_unpacked_sign_with_randomness(const faest_em_256f_unpacked_private_key_t* unpacked_sk, const uint8_t* message, size_t message_len, const uint8_t* rho, size_t rho_len, uint8_t* signature, size_t* signature_len);

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
 * @see faest_em_256f_sign(), faest_em_256f_keygen()
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_verify(const uint8_t* pk, const uint8_t* message, size_t message_len, const uint8_t* signature, size_t signature_len);

/**
 * Check that a key pair is valid.
 *
 * @param[in] sk The private key to check.
 * @param[in] pk The public key to check.
 *
 * @return Returns 0 if the key pair is valid, or a nonzero value indicating an error
 */
FAEST_EXPORT int FAEST_CALLING_CONVENTION faest_em_256f_validate_keypair(const uint8_t* pk, const uint8_t* sk);

/**
 * Clear data of a private key.
 *
 * @param[out] key The private key to clear.
 */
FAEST_EXPORT void FAEST_CALLING_CONVENTION faest_em_256f_clear_private_key(uint8_t* key);

/**
 * Clear data of an unpacked private key.
 *
 * @param[out] unpacked_sk The private key to clear.
 */
FAEST_EXPORT void FAEST_CALLING_CONVENTION faest_em_256f_clear_unpacked_private_key(faest_em_256f_unpacked_private_key_t* unpacked_sk);

// clang-format on

FAEST_END_C_DECL

#endif

// vim: ft=c
