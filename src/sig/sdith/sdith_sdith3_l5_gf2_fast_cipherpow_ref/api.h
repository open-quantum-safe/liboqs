#ifndef API_H
#define API_H

#include <stddef.h>

// This header wraps the SDiTH signature API for use in liboqs

#include "sdith_signature.h"
#include "commons.h"

#include <oqs/common.h>

/* Public API */
#ifndef SDITH_PUBLIC_API_NAMESPACE
#ifdef APPLY_PUBLIC_API_NAMESPACE
#ifndef concat2
#define _concat2(a, b) a ## b
#define concat2(a, b) _concat2(a, b)
#endif
#define SDITH_PUBLIC_API_NAMESPACE(s) concat2(APPLY_PUBLIC_API_NAMESPACE, s)
#else
#define SDITH_PUBLIC_API_NAMESPACE(s) s
#endif
#endif

#define crypto_sign_keypair SDITH_PUBLIC_API_NAMESPACE(crypto_sign_keypair)
#define crypto_sign_sign SDITH_PUBLIC_API_NAMESPACE(crypto_sign_sign)
#define crypto_sign_verify SDITH_PUBLIC_API_NAMESPACE(crypto_sign_verify)

/**
 * Generates the public and private key
 * @param public_key    [out] public key
 * @param secret_key    [out] secret key
 * @return              OQS_SUCCESS on success, OQS_FAILURE on failure
 */
EXPORT OQS_STATUS crypto_sign_keypair(uint8_t* public_key,
	                            uint8_t* secret_key);

/**
 * Signs a message with the private key
 * @param signature     [out] signature
 * @param signature_len [out] length of the signature
 * @param message       [in] message to sign
 * @param message_len   [in] length of the message
 * @param secret_key    [in] secret key
 */
EXPORT OQS_STATUS crypto_sign_sign(uint8_t *signature,
                             size_t *signature_len,
                             const uint8_t *message,
                             size_t message_len,
                             const uint8_t *secret_key);

/**
 * Verifies a signature with the public key
 * @param message       [in] message to verify
 * @param message_len   [in] length of the message
 * @param signature     [in] signature to verify
 * @param signature_len [in] length of the signature
 * @param public_key    [in] public key
 * @return              OQS_SUCCESS on successful verification, OQS_FAILURE on failure
 */
EXPORT OQS_STATUS crypto_sign_verify(const uint8_t *signature,
                                     size_t signature_len,
                                     const uint8_t *message,
                                     size_t message_len,
                                     const uint8_t *public_key);

#endif  // API_H
