#ifndef __XOF_H__
#define __XOF_H__

#include "mqom2_parameters.h"

/*************************************/
/********** Local ********************/
/*************************************/
#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
#include <stdint.h>
/* Include the underlying Keccak header for hash and XOF */
#include "sha3/KeccakHash.h"
#include "sha3/KeccakHashtimes4.h"

/* For common helpers */
#include "common.h"

/* Depending on the parameter, the instances are different:
 *   - For 128 bits security, we use SHAKE-128 for XOF
 *   - For 192 bits security, we use SHAKE-256 for XOF
 *   - For 256 bits security, we use SHAKE-256 for XOF
 */
/* === 128 bits security === */
#if MQOM2_PARAM_SECURITY == 128
#define _XOF_Init Keccak_HashInitialize_SHAKE128
#define _XOF_Init_x4 Keccak_HashInitializetimes4_SHAKE128
/* === 192 bits security === */
#elif MQOM2_PARAM_SECURITY == 192
#define _XOF_Init Keccak_HashInitialize_SHAKE256
#define _XOF_Init_x4 Keccak_HashInitializetimes4_SHAKE256
/* === 256 bits security === */
#elif MQOM2_PARAM_SECURITY == 256
#define _XOF_Init Keccak_HashInitialize_SHAKE256
#define _XOF_Init_x4 Keccak_HashInitializetimes4_SHAKE256
#else
#error "No XOF implementation for this security level"
#endif
/* Common defines for XOF */
#define _XOF_Update Keccak_HashUpdate
#define _XOF_Update_x4 Keccak_HashUpdatetimes4
#define _XOF_Final Keccak_HashFinal
#define _XOF_Final_x4 Keccak_HashFinaltimes4
#define _XOF_Squeeze Keccak_HashSqueeze
#define _XOF_Squeeze_x4 Keccak_HashSqueezetimes4
static inline void _XOF_Release(Keccak_HashInstance *ctx) {
	(void)ctx;
	return;
}
static inline void _XOF_Release_x4(Keccak_HashInstancetimes4 *ctx) {
	(void)ctx;
	return;
}

/*************************************/
/********** MUPQ API *****************/
/*************************************/
#elif defined(MQOM2_FOR_MUPQ)
#include "fips202.h"
/* For common helpers */
#include "common.h"
/* Depending on the parameter, the instances are different:
 *   - For 128 bits security, we use SHAKE-128 for XOF
 *   - For 192 bits security, we use SHAKE-256 for XOF
 *   - For 256 bits security, we use SHAKE-256 for XOF
 */
/* === 128 bits security === */
#if MQOM2_PARAM_SECURITY == 128
#define Keccak_HashInstance shake128incctx
#define __XOF_Init shake128_inc_init
#define __XOF_Update shake128_inc_absorb
#define __XOF_Final shake128_inc_finalize
#define __XOF_Squeeze shake128_inc_squeeze
/* === 192 bits security === */
#elif MQOM2_PARAM_SECURITY == 192
#define Keccak_HashInstance shake256incctx
#define __XOF_Init shake256_inc_init
#define __XOF_Update shake256_inc_absorb
#define __XOF_Final shake256_inc_finalize
#define __XOF_Squeeze shake256_inc_squeeze
/* === 256 bits security === */
#elif MQOM2_PARAM_SECURITY == 256
#define Keccak_HashInstance shake256incctx
#define __XOF_Init shake256_inc_init
#define __XOF_Update shake256_inc_absorb
#define __XOF_Final shake256_inc_finalize
#define __XOF_Squeeze shake256_inc_squeeze
#else
#error "No XOF implementation for this security level"
#endif

typedef struct {
	Keccak_HashInstance ctx[4];
} Keccak_HashInstancetimes4;

/**/
static inline int _XOF_Init(Keccak_HashInstance *ctx) {
	__XOF_Init(ctx);
	return 0;
}
static inline int _XOF_Init_x4(Keccak_HashInstancetimes4 *ctx) {
	unsigned int i;
	for (i = 0; i < 4; i++) {
		__XOF_Init(&ctx->ctx[i]);
	}
	return 0;
}
/**/
static inline int _XOF_Update(Keccak_HashInstance *ctx, const uint8_t* data, size_t byte_len) {
	__XOF_Update(ctx, data, byte_len >> 3);
	return 0;
}
static inline int _XOF_Update_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t *data[4], size_t byte_len) {
	unsigned int i;
	for (i = 0; i < 4; i++) {
		__XOF_Update(&ctx->ctx[i], data[i], byte_len >> 3);
	}
	return 0;
}
/**/
static inline int _XOF_Squeeze(Keccak_HashInstance *ctx, uint8_t* data, size_t byte_len) {
	__XOF_Squeeze(data, byte_len >> 3, ctx);
	return 0;
}
static inline int _XOF_Squeeze_x4(Keccak_HashInstancetimes4 *ctx, uint8_t *data[4], size_t byte_len) {
	unsigned int i;
	for (i = 0; i < 4; i++) {
		__XOF_Squeeze(data[i], byte_len >> 3, &ctx->ctx[i]);
	}
	return 0;
}
/**/
static inline int _XOF_Final(Keccak_HashInstance *ctx, const uint8_t* dummy) {
	(void)dummy;
	__XOF_Final(ctx);
	return 0;
}
static inline int _XOF_Final_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t* dummy) {
	unsigned int i;
	(void)dummy;
	for (i = 0; i < 4; i++) {
		__XOF_Final(&ctx->ctx[i]);
	}
	return 0;
}
static inline void _XOF_Release(Keccak_HashInstance *ctx) {
	(void)ctx;
	return;
}
static inline void _XOF_Release_x4(Keccak_HashInstancetimes4 *ctx) {
	(void)ctx;
	return;
}

/***************************************/
/********** libOQS API *****************/
/***************************************/
#elif defined(MQOM2_FOR_LIBOQS)
#include <oqs/sha3.h>
#include <oqs/sha3x4.h>
/* For common helpers */
#include "common.h"
/* Depending on the parameter, the instances are different:
 *   - For 128 bits security, we use SHAKE-128 for XOF
 *   - For 192 bits security, we use SHAKE-256 for XOF
 *   - For 256 bits security, we use SHAKE-256 for XOF
 */
#include "mqom2_parameters.h"
/* === 128 bits security === */
#if MQOM2_PARAM_SECURITY == 128
#define XOF_CTX_INIT_MAGIC 0xf00dbabebadfeeedULL
typedef struct {
	uint64_t magic;
	OQS_SHA3_shake128_inc_ctx ctx;
} Keccak_HashInstance;
#define XOF_CTX_X4_INIT_MAGIC 0xf11dbabebadf999dULL
typedef struct {
	uint64_t magic;
	OQS_SHA3_shake128_x4_inc_ctx ctx;
} Keccak_HashInstancetimes4;
#define __XOF_Init OQS_SHA3_shake128_inc_init
#define __XOF_Update OQS_SHA3_shake128_inc_absorb
#define __XOF_Final OQS_SHA3_shake128_inc_finalize
#define __XOF_Squeeze OQS_SHA3_shake128_inc_squeeze
#define __XOF_Release OQS_SHA3_shake128_inc_ctx_release
#define __XOF_Reset OQS_SHA3_shake128_inc_ctx_reset
#define __XOF_Init_x4 OQS_SHA3_shake128_x4_inc_init
#define __XOF_Update_x4 OQS_SHA3_shake128_x4_inc_absorb
#define __XOF_Final_x4 OQS_SHA3_shake128_x4_inc_finalize
#define __XOF_Squeeze_x4 OQS_SHA3_shake128_x4_inc_squeeze
#define __XOF_Release_x4 OQS_SHA3_shake128_x4_inc_ctx_release
#define __XOF_Reset_x4 OQS_SHA3_shake128_x4_inc_ctx_reset
/* === 192 bits security === */
#elif MQOM2_PARAM_SECURITY == 192
#define XOF_CTX_INIT_MAGIC 0xf00dbabebadfeeedULL
typedef struct {
	uint64_t magic;
	OQS_SHA3_shake256_inc_ctx ctx;
} Keccak_HashInstance;
#define XOF_CTX_X4_INIT_MAGIC 0xf11dbabebadf999dULL
typedef struct {
	uint64_t magic;
	OQS_SHA3_shake256_x4_inc_ctx ctx;
} Keccak_HashInstancetimes4;
#define __XOF_Init OQS_SHA3_shake256_inc_init
#define __XOF_Update OQS_SHA3_shake256_inc_absorb
#define __XOF_Final OQS_SHA3_shake256_inc_finalize
#define __XOF_Squeeze OQS_SHA3_shake256_inc_squeeze
#define __XOF_Release OQS_SHA3_shake256_inc_ctx_release
#define __XOF_Reset OQS_SHA3_shake256_inc_ctx_reset
#define __XOF_Init_x4 OQS_SHA3_shake256_x4_inc_init
#define __XOF_Update_x4 OQS_SHA3_shake256_x4_inc_absorb
#define __XOF_Final_x4 OQS_SHA3_shake256_x4_inc_finalize
#define __XOF_Squeeze_x4 OQS_SHA3_shake256_x4_inc_squeeze
#define __XOF_Release_x4 OQS_SHA3_shake256_x4_inc_ctx_release
#define __XOF_Reset_x4 OQS_SHA3_shake256_x4_inc_ctx_reset
/* === 256 bits security === */
#elif MQOM2_PARAM_SECURITY == 256
#define XOF_CTX_INIT_MAGIC 0xf00dbabebadfeeedULL
typedef struct {
	uint64_t magic;
	OQS_SHA3_shake256_inc_ctx ctx;
} Keccak_HashInstance;
#define XOF_CTX_X4_INIT_MAGIC 0xf11dbabebadf999dULL
typedef struct {
	uint64_t magic;
	OQS_SHA3_shake256_x4_inc_ctx ctx;
} Keccak_HashInstancetimes4;
#define __XOF_Init OQS_SHA3_shake256_inc_init
#define __XOF_Update OQS_SHA3_shake256_inc_absorb
#define __XOF_Final OQS_SHA3_shake256_inc_finalize
#define __XOF_Squeeze OQS_SHA3_shake256_inc_squeeze
#define __XOF_Release OQS_SHA3_shake256_inc_ctx_release
#define __XOF_Reset OQS_SHA3_shake256_inc_ctx_reset
#define __XOF_Init_x4 OQS_SHA3_shake256_x4_inc_init
#define __XOF_Update_x4 OQS_SHA3_shake256_x4_inc_absorb
#define __XOF_Final_x4 OQS_SHA3_shake256_x4_inc_finalize
#define __XOF_Squeeze_x4 OQS_SHA3_shake256_x4_inc_squeeze
#define __XOF_Release_x4 OQS_SHA3_shake256_x4_inc_ctx_release
#define __XOF_Reset_x4 OQS_SHA3_shake256_x4_inc_ctx_reset
#else
#error "No XOF implementation for this security level"
#endif

/**/
static inline int _XOF_Init(Keccak_HashInstance *ctx) {
	if (ctx == NULL) {
		return -1;
	}
	if (ctx->magic == XOF_CTX_INIT_MAGIC) {
		__XOF_Reset(&(ctx->ctx));
	} else {
		__XOF_Init(&(ctx->ctx));
		ctx->magic = XOF_CTX_INIT_MAGIC;
	}
	return 0;
}
static inline int _XOF_Init_x4(Keccak_HashInstancetimes4 *ctx) {
	if (ctx == NULL) {
		return -1;
	}
	if (ctx->magic == XOF_CTX_X4_INIT_MAGIC) {
		__XOF_Reset_x4(&(ctx->ctx));
	} else {
		__XOF_Init_x4(&(ctx->ctx));
		ctx->magic = XOF_CTX_X4_INIT_MAGIC;
	}
	return 0;
}
/**/
static inline int _XOF_Update(Keccak_HashInstance *ctx, const uint8_t* data, size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Update(&(ctx->ctx), data, byte_len >> 3);
	return 0;
}
static inline int _XOF_Update_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t *data[4], size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_X4_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Update_x4(&(ctx->ctx), data[0], data[1], data[2], data[3], byte_len >> 3);
	return 0;
}
/**/
static inline int _XOF_Squeeze(Keccak_HashInstance *ctx, uint8_t* data, size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Squeeze(data, byte_len >> 3, &(ctx->ctx));
	return 0;
}
static inline int _XOF_Squeeze_x4(Keccak_HashInstancetimes4 *ctx, uint8_t *data[4], size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_X4_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Squeeze_x4(data[0], data[1], data[2], data[3], byte_len >> 3, &(ctx->ctx));
	return 0;
}
/**/
static inline int _XOF_Final(Keccak_HashInstance *ctx, const uint8_t* dummy) {
	(void)dummy;
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Final(&(ctx->ctx));
	return 0;
}
static inline int _XOF_Final_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t* dummy) {
	(void)dummy;
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_X4_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Final_x4(&(ctx->ctx));
	return 0;
}

static inline void _XOF_Release(Keccak_HashInstance *ctx) {
	if ((ctx != NULL) && (ctx->magic == XOF_CTX_INIT_MAGIC)) {
		__XOF_Release(&(ctx->ctx));
	}
	if(ctx != NULL){
		ctx->magic = 0;
	}
	return;
}
static inline void _XOF_Release_x4(Keccak_HashInstancetimes4 *ctx) {
	if ((ctx != NULL) && (ctx->magic == XOF_CTX_X4_INIT_MAGIC)) {
		__XOF_Release_x4(&(ctx->ctx));
	}
	if(ctx != NULL){
		ctx->magic = 0;
	}
	return;
}

#endif

/* Deal with namespacing */
#define xof_init MQOM_NAMESPACE(xof_init)
#define xof_update MQOM_NAMESPACE(xof_update)
#define xof_squeeze MQOM_NAMESPACE(xof_squeeze)
#define xof_clean_ctx MQOM_NAMESPACE(xof_clean_ctx)
#define xof_init_x4 MQOM_NAMESPACE(xof_init_x4)
#define xof_update_x4 MQOM_NAMESPACE(xof_update_x4)
#define xof_squeeze_x4 MQOM_NAMESPACE(xof_squeeze_x4)
#define xof_clean_ctx_x4 MQOM_NAMESPACE(xof_clean_ctx_x4)

/* Hash and XOF contexts are simply Keccak instances, with XOF finalization state
 * for XOF
 */
typedef struct {
	uint8_t xof_finalized;
	Keccak_HashInstance ctx;
} xof_context;

/* x4 (4 times) context */
typedef struct {
	uint8_t xof_finalized;
	Keccak_HashInstancetimes4 ctx;
} xof_context_x4;

/* Exported API for XOF, simple and x4 */
int xof_init(xof_context *ctx);
int xof_update(xof_context *ctx, const uint8_t *data, size_t byte_len);
int xof_squeeze(xof_context *ctx, uint8_t *out, uint32_t byte_len);
void xof_clean_ctx(xof_context *ctx);

int xof_init_x4(xof_context_x4 *ctx);
int xof_update_x4(xof_context_x4 *ctx, const uint8_t *data[4], size_t byte_len);
int xof_squeeze_x4(xof_context_x4 *ctx, uint8_t *out[4], uint32_t byte_len);
void xof_clean_ctx_x4(xof_context_x4 *ctx);

#endif /* __HASH_XOF_H__ */
