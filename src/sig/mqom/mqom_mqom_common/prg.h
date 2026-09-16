#ifndef __PRG_H__
#define __PRG_H__

#include "seed_expand.h"

/* Deal with namespacing */
#define PRG MQOM_NAMESPACE(PRG)
#define PRG_memopt MQOM_NAMESPACE(PRG_memopt)
#define PRG_memopt_with_ctx MQOM_NAMESPACE(PRG_memopt_with_ctx)

/* v3 spec PRG(seed, n_bytes): key=seed, ptx=Bits_lambda(counter), Enc = the native
 * cipher for the security level (AES-128 for cat1, Rijndael-256 truncated/full for
 * cat3/cat5). Used for ExpandMQ (seed_eq is lambda bytes). No salt, no e, no TweakSalt. */
#define MQOM3_PARAM_SEED_EQ_SIZE MQOM3_PARAM_SEED_SIZE
int PRG(const uint8_t seed[MQOM3_PARAM_SEED_EQ_SIZE], uint32_t nbytes, uint8_t *out_data);
int PRG_memopt(const uint8_t seed[MQOM3_PARAM_SEED_EQ_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_EQ_SIZE]);
/* Variant of PRG_memopt with a pre-computed key schedule, avoiding a redundant enc_key_sched
 * when the same seed is used across many consecutive calls (e.g. ExpandEquations_memopt). */
int PRG_memopt_with_ctx(const enc_ctx_pub_ecb *ctx, uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_EQ_SIZE]);

#endif /* __PRG_H__ */
