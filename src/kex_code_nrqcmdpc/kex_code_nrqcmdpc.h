#ifndef KEX_CODE_NRQCMDPC_H
#define KEX_CODE_NRQCMDPC_H

/**
 * \file oqs_kex.h
 * \brief Header for code-based key exchange using protocol NRQCMDPC.
 *
 * NRQCMDPC stands for the Niederreiter crypto system, using Quasi-Cyclic
 * Medium-Density Parity-Check codes.
 */

#include <stddef.h>
#include <stdint.h>

#include <oqs/kex.h>
#include <oqs/rand.h>

#define KEY_BYTES 32

OQS_KEX *OQS_KEX_code_nrqcmdpc_new(
    OQS_RAND *rand);

int OQS_KEX_code_nrqcmdpc_alice_0(
    OQS_KEX *k,
    void **alice_priv,
    uint8_t **alice_msg,
    size_t *alice_msg_len);

int OQS_KEX_code_nrqcmdpc_bob(
    OQS_KEX *k,
    const uint8_t *alice_msg,
    const size_t alice_msg_len,
    uint8_t **bob_msg,
    size_t *bob_msg_len,
    uint8_t **key,
    size_t *key_len);

int OQS_KEX_code_nrqcmdpc_alice_1(
    OQS_KEX *k,
    const void *alice_priv,
    const uint8_t *bob_msg,
    const size_t bob_msg_len,
    uint8_t **key,
    size_t *key_len);

void OQS_KEX_code_nrqcmdpc_alice_priv_free(
    OQS_KEX *k,
    void *alice_priv);

void OQS_KEX_code_nrqcmdpc_free(
    OQS_KEX *k);

#endif /* KEX_CODE_NRQCMDPC_H */
