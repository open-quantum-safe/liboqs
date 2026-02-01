#ifndef __PRG_H__
#define __PRG_H__

/* MQOM2 parameters */
#include "mqom2_parameters.h"
/* Our PRG is based on a block cipher defined in the following header */
#include "enc.h"
/* PRG cache for context x1 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 1
#include "prg_cache.h"
/* PRG cache for public context x1 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _pub
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_pub
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 1
#include "prg_cache.h"
/* PRG cache for context x2 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _x2
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_x2
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 2
#include "prg_cache.h"
/* PRG cache for public context x2 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _pub_x2
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_pub_x2
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 2
#include "prg_cache.h"
/* PRG cache for context x4 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _x4
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_x4
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 4
#include "prg_cache.h"
/* PRG cache for public context x4 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _pub_x4
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_pub_x4
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 4
#include "prg_cache.h"
/* PRG cache for context x8 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _x8
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_x8
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 8
#include "prg_cache.h"
/* PRG cache for public context x8 */
#undef __PRG_CACHE_H__
#undef PRG_CACHE_SUFFIX
#define PRG_CACHE_SUFFIX _pub_x8
#undef PRG_ENC_CTX_TYPE
#define PRG_ENC_CTX_TYPE enc_ctx_pub_x8
#undef PRG_CACHE_NB_PARALLELS
#define PRG_CACHE_NB_PARALLELS 8
#include "prg_cache.h"

/* Deal with namespacing */
#define PRG_memopt MQOM_NAMESPACE(PRG_memopt)
#define PRG_memopt_pub MQOM_NAMESPACE(PRG_memopt_pub)
#define PRG MQOM_NAMESPACE(PRG)
#define PRG_pub MQOM_NAMESPACE(PRG_pub)
#define PRG_x2 MQOM_NAMESPACE(PRG_x2)
#define PRG_x2_pub MQOM_NAMESPACE(PRG_x2_pub)
#define PRG_x4 MQOM_NAMESPACE(PRG_x4)
#define PRG_x4_pub MQOM_NAMESPACE(PRG_x4_pub)
#define PRG_x8 MQOM_NAMESPACE(PRG_x8)
#define PRG_x8_pub MQOM_NAMESPACE(PRG_x8_pub)

int PRG_memopt(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM2_PARAM_SEED_SIZE], prg_key_sched_cache *cache);
int PRG_memopt_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM2_PARAM_SEED_SIZE], prg_key_sched_cache_pub *cache);

int PRG(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, prg_key_sched_cache *cache);
int PRG_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, prg_key_sched_cache_pub *cache);

int PRG_x2(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[2][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[2], prg_key_sched_cache_x2* cache, uint8_t nb_contexts);
int PRG_x2_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[2][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[2], prg_key_sched_cache_pub_x2* cache, uint8_t nb_contexts);

int PRG_x4(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[4][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[4], prg_key_sched_cache_x4* cache, uint8_t nb_contexts);
int PRG_x4_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[4][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[4], prg_key_sched_cache_pub_x4* cache, uint8_t nb_contexts);

int PRG_x8(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], prg_key_sched_cache_x8* cache, uint8_t nb_contexts);
int PRG_x8_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], prg_key_sched_cache_pub_x8* cache, uint8_t nb_contexts);

#endif /* __PRG_H__ */
