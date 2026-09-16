// SPDX-License-Identifier: Apache-2.0

#ifndef MAYO_H
#define MAYO_H

#include <stdint.h>
#include <stdlib.h>

#include <mayo_constants.h>

/**
 * Struct defining MAYO parameters
 */
#ifdef ENABLE_PARAMS_DYNAMIC
typedef struct mayo_params_t {
    int m;
    int n;
    int o;
    int k;
    int q;
    const unsigned char *f_tail;
    const unsigned char *whip_pos;
    int m_bytes;
    int O_bytes;
    int v_bytes;
    int r_bytes;
    int P1_bytes;
    int P2_bytes;
    int P3_bytes;
    int csk_bytes;
    int cpk_bytes;
    int sig_bytes;
    int salt_bytes;
    int sk_seed_bytes;
    int digest_bytes;
    int pk_seed_bytes;
    int m_vec_limbs;
    const char *name;
} mayo_params_t;

/**
 * MAYO parameter sets
 */
extern const mayo_params_t MAYO_1;
extern const mayo_params_t MAYO_2;
extern const mayo_params_t MAYO_3;
extern const mayo_params_t MAYO_5;
#else
typedef struct mayo_params_t mayo_params_t;
#endif

typedef struct sk_t {
    uint64_t p[P1_LIMBS_MAX + P2_LIMBS_MAX + MAYO_MVEC_SLACK];
    uint8_t O[V_MAX*O_MAX];
    uint8_t sk_seed[SK_SEED_BYTES_MAX]; // seed_sk, needed for salt derivation in Sign(esk)
} sk_t;

typedef struct pk_t {
    uint64_t p[P1_LIMBS_MAX + P2_LIMBS_MAX + P3_LIMBS_MAX + MAYO_MVEC_SLACK];
} pk_t;

/**
 * Status codes
 */
#define MAYO_OK 0
#define MAYO_ERR -1

/**
 * Mayo keypair generation.
 *
 * The implementation corresponds to Mayo.CompactKeyGen() in the Mayo spec.
 * The caller is responsible to allocate sufficient memory to hold pk and sk.
 *
 * @param[in] p Mayo parameter set
 * @param[out] pk Mayo public key
 * @param[out] sk Mayo secret key
 * @return int status code
 */
#define mayo_keypair MAYO_NAMESPACE(mayo_keypair)
int mayo_keypair(const mayo_params_t *p, unsigned char *pk, unsigned char *sk);

/**
 * MAYO signature generation.
 *
 * The implementation performs Mayo.expandSK() + Mayo.sign() in the Mayo spec.
 * Outputs the detached signature; use mayo_sign for signature-plus-message.
 * The caller is responsible to allocate sufficient memory to hold sig.
 *
 * @param[in] p Mayo parameter set
 * @param[out] sig Signature
 * @param[out] siglen Pointer to the length of sig
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] csk Compacted secret key
 * @return int status code
 */
#define mayo_sign_signature MAYO_NAMESPACE(mayo_sign_signature)
int mayo_sign_signature(const mayo_params_t *p, unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const unsigned char *csk);

/**
 * MAYO signature generation from an expanded secret key.
 *
 * Corresponds to Mayo.sign() in the Mayo spec: takes an already expanded secret
 * key (see mayo_expand_sk), so a caller can sign many messages without redoing
 * the key expansion.
 *
 * @param[in] p Mayo parameter set
 * @param[out] sig Signature
 * @param[out] siglen Pointer to the length of sig
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] esk Expanded secret key (from mayo_expand_sk)
 * @return int status code
 */
#define mayo_sign_signature_esk MAYO_NAMESPACE(mayo_sign_signature_esk)
int mayo_sign_signature_esk(const mayo_params_t *p, unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const sk_t *esk);

/**
 * MAYO signature generation from an expanded secret key (signed message output).
 *
 * Like mayo_sign but takes an already expanded secret key (see mayo_expand_sk).
 * Outputs the signature concatenated with the message in sm.
 *
 * @param[in]  p      Mayo parameter set
 * @param[out] sm     Signature concatenated with message
 * @param[out] smlen  Pointer to the length of sm
 * @param[in]  m      Message to be signed
 * @param[in]  mlen   Message length
 * @param[in]  esk    Expanded secret key (from mayo_expand_sk)
 * @return int status code
 */
#define mayo_sign_esk MAYO_NAMESPACE(mayo_sign_esk)
int mayo_sign_esk(const mayo_params_t *p, unsigned char *sm,
                  size_t *smlen, const unsigned char *m,
                  size_t mlen, const sk_t *esk);

/**
 * MAYO signature generation.
 *
 * The implementation performs Mayo.expandSK() + Mayo.sign() in the Mayo spec.
 * Keys provided is a compacted secret keys.
 * The caller is responsible to allocate sufficient memory to hold sm.
 *
 * @param[in] p Mayo parameter set
 * @param[out] sm Signature concatenated with message
 * @param[out] smlen Pointer to the length of sm
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] sk Compacted secret key
 * @return int status code
 */
#define mayo_sign MAYO_NAMESPACE(mayo_sign)
int mayo_sign(const mayo_params_t *p, unsigned char *sm,
              size_t *smlen, const unsigned char *m,
              size_t mlen, const unsigned char *sk);

/**
 * Mayo open signature.
 *
 * The implementation performs Mayo.verify(). If the signature verification succeeded, the original message is stored in m.
 * Keys provided is a compact public key.
 * The caller is responsible to allocate sufficient memory to hold m.
 *
 * @param[in] p Mayo parameter set
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
#define mayo_open MAYO_NAMESPACE(mayo_open)
int mayo_open(const mayo_params_t *p, unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk);

/**
 * Mayo compact keypair generation.
 *
 * The implementation corresponds to Mayo.CompactKeyGen() in the Mayo spec.
 * The caller is responsible to allocate sufficient memory to hold pk and sk.
 *
 * outputs a pair (csk, cpk) \in B^{csk_bytes} x B^{cpk_bytes}, where csk and
 * cpk are compact representations of a Mayo secret key and public key
 *
 * @param[in] p Mayo parameter set
 * @param[out] cpk Mayo compacted public key
 * @param[out] csk Mayo compacted secret key
 * @return int status code
 */
#define mayo_keypair_compact MAYO_NAMESPACE(mayo_keypair_compact)
int mayo_keypair_compact(const mayo_params_t *p, unsigned char *cpk,
                         unsigned char *csk);

/**
 * Mayo expand public key.
 *
 * The implementation corresponds to Mayo.expandPK() in the Mayo spec.
 * The caller is responsible to allocate sufficient memory to hold epk.
 *
 * @param[in] p Mayo parameter set
 * @param[in] cpk Compacted public key.
 * @param[out] epk Expanded public key.
 * @return int return code
 */
#define mayo_expand_pk MAYO_NAMESPACE(mayo_expand_pk)
int mayo_expand_pk(const mayo_params_t *p, const unsigned char *cpk,
                   uint64_t *epk);

/**
 * Mayo expand secret key.
 *
 * The implementation corresponds to Mayo.expandSK() in the Mayo spec.
 * The caller is responsible to allocate sufficient memory to hold esk.
 *
 * @param[in] p Mayo parameter set
 * @param[in] csk Compacted secret key.
 * @param[out] esk Expanded secret key.
 * @return int return code
 */
#define mayo_expand_sk MAYO_NAMESPACE(mayo_expand_sk)
int mayo_expand_sk(const mayo_params_t *p, const unsigned char *csk,
                   sk_t *esk);

/**
 * Mayo verify signature.
 *
 * The implementation performs Mayo.verify(). If the signature verification succeeded, returns 0, otherwise 1.
 * Keys provided is a compact public key.
 *
 * @param[in] p Mayo parameter set
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
#define mayo_verify MAYO_NAMESPACE(mayo_verify)
int mayo_verify(const mayo_params_t *p, const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                const unsigned char *pk);

/**
 * Mayo verify signature from an expanded public key.
 *
 * The implementation performs Mayo.verify() using an already expanded public
 * key (see mayo_expand_pk), so a caller can verify many signatures without
 * redoing the key expansion.
 *
 * @param[in] p Mayo parameter set
 * @param[in] m Message
 * @param[in] mlen Message length
 * @param[in] sig Signature
 * @param[in] epk Expanded public key (from mayo_expand_pk)
 * @return int 0 if verification succeeded, 1 otherwise.
 */
#define mayo_verify_epk MAYO_NAMESPACE(mayo_verify_epk)
int mayo_verify_epk(const mayo_params_t *p, const unsigned char *m,
                    size_t mlen, const unsigned char *sig,
                    const uint64_t *epk);

/**
 * Mayo open signature from an expanded public key.
 *
 * Like mayo_open but takes an already expanded public key (see mayo_expand_pk),
 * so a caller can verify many signatures without redoing the key expansion.
 *
 * @param[in]  p     Mayo parameter set
 * @param[out] m     Message stored if verification succeeds
 * @param[out] mlen  Pointer to the length of m
 * @param[in]  sm    Signature concatenated with message
 * @param[in]  smlen Length of sm
 * @param[in]  epk   Expanded public key (from mayo_expand_pk)
 * @return int 0 if verification succeeded, 1 otherwise.
 */
#define mayo_open_epk MAYO_NAMESPACE(mayo_open_epk)
int mayo_open_epk(const mayo_params_t *p, unsigned char *m,
                  size_t *mlen, const unsigned char *sm,
                  size_t smlen, const uint64_t *epk);

#endif

