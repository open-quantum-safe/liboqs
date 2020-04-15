#ifndef PQCLEAN_MQDSS48_CLEAN_API_H
#define PQCLEAN_MQDSS48_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_MQDSS48_CLEAN_CRYPTO_ALGNAME "MQDSS-48"

#define PQCLEAN_MQDSS48_CLEAN_CRYPTO_SECRETKEYBYTES 16
#define PQCLEAN_MQDSS48_CLEAN_CRYPTO_PUBLICKEYBYTES 46
#define PQCLEAN_MQDSS48_CLEAN_CRYPTO_BYTES 28400

/*
 * Generates an MQDSS key pair.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_keypair(
    uint8_t *pk, uint8_t *sk);

/**
 * Returns an array containing a detached signature.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

/**
 * Verifies a detached signature and message under a given public key.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

/**
 * Returns an array containing the signature followed by the message.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

/**
 * Verifies a given signature-message pair under a given public key.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk);

#endif
