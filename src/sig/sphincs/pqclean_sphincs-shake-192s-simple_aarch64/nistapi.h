#ifndef SPX_API_H
#define SPX_API_H

#include <stddef.h>
#include <stdint.h>

#include "params.h"

#define CRYPTO_ALGNAME "SPHINCS+"

#define CRYPTO_SECRETKEYBYTES SPX_SK_BYTES
#define CRYPTO_PUBLICKEYBYTES SPX_PK_BYTES
#define CRYPTO_BYTES SPX_BYTES
#define CRYPTO_SEEDBYTES (3*SPX_N)

/*
 * Returns the length of a secret key, in bytes
 */
#define crypto_sign_secretkeybytes SPX_NAMESPACE(crypto_sign_secretkeybytes)
size_t crypto_sign_secretkeybytes(void);

/*
 * Returns the length of a public key, in bytes
 */
#define crypto_sign_publickeybytes SPX_NAMESPACE(crypto_sign_publickeybytes)
size_t crypto_sign_publickeybytes(void);

/*
 * Returns the length of a signature, in bytes
 */
#define crypto_sign_bytes SPX_NAMESPACE(crypto_sign_bytes)
size_t crypto_sign_bytes(void);

/*
 * Returns the length of the seed required to generate a key pair, in bytes
 */
#define crypto_sign_seedbytes SPX_NAMESPACE(crypto_sign_seedbytes)
size_t crypto_sign_seedbytes(void);

/*
 * Generates a SPHINCS+ key pair given a seed.
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [root || PUB_SEED]
 */
#define crypto_sign_seed_keypair SPX_NAMESPACE(crypto_sign_seed_keypair)
int crypto_sign_seed_keypair(uint8_t *pk, uint8_t *sk,
                             const uint8_t *seed);

/*
 * Generates a SPHINCS+ key pair.
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [root || PUB_SEED]
 */
#define crypto_sign_keypair SPX_NAMESPACE(crypto_sign_keypair)
int crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

/**
 * Returns an array containing a detached signature.
 */
#define crypto_sign_signature SPX_NAMESPACE(crypto_sign_signature)
int crypto_sign_signature(uint8_t *sig, size_t *siglen,
                          const uint8_t *m, size_t mlen, const uint8_t *sk);

/**
 * Verifies a detached signature and message under a given public key.
 */
#define crypto_sign_verify SPX_NAMESPACE(crypto_sign_verify)
int crypto_sign_verify(const uint8_t *sig, size_t siglen,
                       const uint8_t *m, size_t mlen, const uint8_t *pk);

/**
 * Returns an array containing the signature followed by the message.
 */
#define crypto_sign SPX_NAMESPACE(crypto_sign)
int crypto_sign(uint8_t *sm, size_t *smlen,
                const uint8_t *m, size_t mlen,
                const uint8_t *sk);

/**
 * Verifies a given signature-message pair under a given public key.
 */
#define crypto_sign_open SPX_NAMESPACE(crypto_sign_open)
int crypto_sign_open(uint8_t *m, size_t *mlen,
                     const uint8_t *sm, size_t smlen,
                     const uint8_t *pk);

#endif
