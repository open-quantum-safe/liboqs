#ifndef __SEED_EXPAND_H__
#define __SEED_EXPAND_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"
/* SeedExpand is based on a block cipher defined in the following header */
#include "enc.h"
/* SeedExpand cache for context x1 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 1
#include "seed_expand_cache.h"
/* SeedExpand cache for public context x1 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _pub
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_pub
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 1
#include "seed_expand_cache.h"
/* SeedExpand cache for context x2 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _x2
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_x2
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 2
#include "seed_expand_cache.h"
/* SeedExpand cache for public context x2 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _pub_x2
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_pub_x2
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 2
#include "seed_expand_cache.h"
/* SeedExpand cache for context x4 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _x4
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_x4
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 4
#include "seed_expand_cache.h"
/* SeedExpand cache for public context x4 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _pub_x4
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_pub_x4
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 4
#include "seed_expand_cache.h"
/* SeedExpand cache for context x8 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _x8
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_x8
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 8
#include "seed_expand_cache.h"
/* SeedExpand cache for public context x8 */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _pub_x8
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_pub_x8
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 8
#include "seed_expand_cache.h"
/* SeedExpand cache for ECB (single key, all 8 slots share the same e) */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _ecb
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_ecb
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 1
#include "seed_expand_cache.h"
/* SeedExpand cache for public ECB */
#undef __SEED_EXPAND_CACHE_H__
#undef SEED_EXPAND_SUFFIX
#define SEED_EXPAND_SUFFIX _pub_ecb
#undef SEED_EXPAND_CTX_TYPE
#define SEED_EXPAND_CTX_TYPE enc_ctx_pub_ecb
#undef SEED_EXPAND_NB_PARALLELS
#define SEED_EXPAND_NB_PARALLELS 1
#include "seed_expand_cache.h"
/* Deal with namespacing */
#define SeedExpand_memopt MQOM_NAMESPACE(SeedExpand_memopt)
#define SeedExpand_memopt_pub MQOM_NAMESPACE(SeedExpand_memopt_pub)
#define SeedExpand MQOM_NAMESPACE(SeedExpand)
#define SeedExpand_pub MQOM_NAMESPACE(SeedExpand_pub)
#define SeedExpand_x2 MQOM_NAMESPACE(SeedExpand_x2)
#define SeedExpand_x2_pub MQOM_NAMESPACE(SeedExpand_x2_pub)
#define SeedExpand_x4 MQOM_NAMESPACE(SeedExpand_x4)
#define SeedExpand_x4_pub MQOM_NAMESPACE(SeedExpand_x4_pub)
#define SeedExpand_x8 MQOM_NAMESPACE(SeedExpand_x8)
#define SeedExpand_x8_pub MQOM_NAMESPACE(SeedExpand_x8_pub)
#define SeedExpand_x8_ecb MQOM_NAMESPACE(SeedExpand_x8_ecb)
#define SeedExpand_x8_pub_ecb MQOM_NAMESPACE(SeedExpand_x8_pub_ecb)

/* SeedExpand(salt, e, seed, n_bytes): BLC leaf expansion, dispatches CT or OT at compile-time */
int SeedExpand_memopt(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_SIZE], SeedExpand_cache *cache);
int SeedExpand_memopt_pub(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_SIZE], SeedExpand_cache_pub *cache);

int SeedExpand(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, SeedExpand_cache *cache);
int SeedExpand_pub(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, SeedExpand_cache_pub *cache);

int SeedExpand_x2(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[2][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[2], SeedExpand_cache_x2* cache, uint8_t nb_contexts);
int SeedExpand_x2_pub(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[2][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[2], SeedExpand_cache_pub_x2* cache, uint8_t nb_contexts);

int SeedExpand_x4(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[4][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[4], SeedExpand_cache_x4* cache, uint8_t nb_contexts);
int SeedExpand_x4_pub(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[4][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[4], SeedExpand_cache_pub_x4* cache, uint8_t nb_contexts);

int SeedExpand_x8(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], SeedExpand_cache_x8* cache, uint8_t nb_contexts);
int SeedExpand_x8_pub(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], SeedExpand_cache_pub_x8* cache, uint8_t nb_contexts);

/* ECB variants: same key for all 8 seeds (all share the same e), uses a compact single-key cache */
int SeedExpand_x8_ecb(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[8][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], SeedExpand_cache_ecb* cache);
int SeedExpand_x8_pub_ecb(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[8][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], SeedExpand_cache_pub_ecb* cache);

#endif /* __SEED_EXPAND_H__ */
