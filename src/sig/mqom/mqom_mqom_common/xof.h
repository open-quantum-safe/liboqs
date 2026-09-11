#ifndef __XOF_H__
#define __XOF_H__

#include "mqom3_parameters.h"


/*************************************/
/********** External *****************/
/*************************************/
#if defined(MQOM3_XOF_EXTERNAL_API)

/** Here, the XOF API is defined elsewhere and provided by the user **/
#include "xof_external.h"
/**/
extern int _XOF_Init(Keccak_HashInstance *ctx);
extern int _XOF_Init_x4(Keccak_HashInstancetimes4 *ctx);
/**/
extern int _XOF_Update(Keccak_HashInstance *ctx, const uint8_t* data, size_t byte_len);
extern int _XOF_Update_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t *data[4], size_t byte_len);
/**/
extern int _XOF_Squeeze(Keccak_HashInstance *ctx, uint8_t* data, size_t byte_len);
extern int _XOF_Squeeze_x4(Keccak_HashInstancetimes4 *ctx, uint8_t *data[4], size_t byte_len);
/**/
extern int _XOF_Final(Keccak_HashInstance *ctx, const uint8_t* dummy);
extern int _XOF_Final_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t* dummy);
extern void _XOF_Release(Keccak_HashInstance *ctx);
extern void _XOF_Release_x4(Keccak_HashInstancetimes4 *ctx);

/*** No external XOF API defined, either local, MUPQ or liboqs ***/
#else
/*************************************/
/********** Local ********************/
/*************************************/
#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
#include <stdint.h>
/* Include the underlying Keccak header for hash and XOF */
#if !defined(SUPERCOP)
#include "sha3/KeccakHash.h"
#else
#include <libkeccak.a.headers/KeccakHash.h>
#endif
#include "sha3/KeccakHashtimes4.h"

/* For common helpers */
#include "common.h"

/* Depending on the parameter, the instances are different:
 *   - For 128 bits security, we use SHAKE-128 for XOF
 *   - For 192 bits security, we use SHAKE-256 for XOF
 *   - For 256 bits security, we use SHAKE-256 for XOF
 */
/* === 128 bits security === */
#if MQOM3_PARAM_SECURITY == 128
#define _XOF_Init Keccak_HashInitialize_SHAKE128
#define _XOF_Init_x4 Keccak_HashInitializetimes4_SHAKE128
/* === 192 bits security === */
#elif MQOM3_PARAM_SECURITY == 192
#define _XOF_Init Keccak_HashInitialize_SHAKE256
#define _XOF_Init_x4 Keccak_HashInitializetimes4_SHAKE256
/* === 256 bits security === */
#elif MQOM3_PARAM_SECURITY == 256
#define _XOF_Init Keccak_HashInitialize_SHAKE256
#define _XOF_Init_x4 Keccak_HashInitializetimes4_SHAKE256
#else
#error "No XOF implementation for this security level"
#endif
/* Common defines for XOF.  The MQOM wrapper is byte-oriented while XKCP is
 * bit-oriented, so keep the conversion confined to this backend adapter. */
#define __XOF_Update_bits Keccak_HashUpdate
#define __XOF_Update_x4_bits Keccak_HashUpdatetimes4
#define _XOF_Final Keccak_HashFinal
#define _XOF_Final_x4 Keccak_HashFinaltimes4
#define __XOF_Squeeze_bits Keccak_HashSqueeze
#define __XOF_Squeeze_x4_bits Keccak_HashSqueezetimes4
static inline int _XOF_Update(Keccak_HashInstance *ctx, const uint8_t *data, size_t byte_len) {
	if (byte_len > (SIZE_MAX / 8)) {
		return -1;
	}
	return __XOF_Update_bits(ctx, data, byte_len * 8);
}
static inline int _XOF_Update_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t *data[4], size_t byte_len) {
	if (byte_len > (SIZE_MAX / 8)) {
		return -1;
	}
	return __XOF_Update_x4_bits(ctx, data, byte_len * 8);
}
static inline int _XOF_Squeeze(Keccak_HashInstance *ctx, uint8_t *data, size_t byte_len) {
	if (byte_len > (SIZE_MAX / 8)) {
		return -1;
	}
	return __XOF_Squeeze_bits(ctx, data, byte_len * 8);
}
static inline int _XOF_Squeeze_x4(Keccak_HashInstancetimes4 *ctx, uint8_t *data[4], size_t byte_len) {
	if (byte_len > (SIZE_MAX / 8)) {
		return -1;
	}
	return __XOF_Squeeze_x4_bits(ctx, data, byte_len * 8);
}
static inline void _XOF_Release(Keccak_HashInstance *ctx) {
	(void)ctx;
	return;
}
static inline void _XOF_Release_x4(Keccak_HashInstancetimes4 *ctx) {
	(void)ctx;
	return;
}
/* Native x8 path: native KeccakP-1600-times8 kernel (AVX-512 only) */
#ifdef XKCP_has_KeccakP1600times8
#include "sha3/KeccakHashtimes8.h"
#if MQOM3_PARAM_SECURITY == 128
#define _XOF_Init_x8  Keccak_HashInitializetimes8_SHAKE128
#elif MQOM3_PARAM_SECURITY == 192 || MQOM3_PARAM_SECURITY == 256
#define _XOF_Init_x8  Keccak_HashInitializetimes8_SHAKE256
#endif
#define __XOF_Update_x8_bits Keccak_HashUpdatetimes8
#define _XOF_Final_x8    Keccak_HashFinaltimes8
#define __XOF_Squeeze_x8_bits Keccak_HashSqueezetimes8
static inline int _XOF_Update_x8(Keccak_HashInstancetimes8 *ctx, const uint8_t *data[8], size_t byte_len) {
	if (byte_len > (SIZE_MAX / 8)) {
		return -1;
	}
	return __XOF_Update_x8_bits(ctx, data, byte_len * 8);
}
static inline int _XOF_Squeeze_x8(Keccak_HashInstancetimes8 *ctx, uint8_t *data[8], size_t byte_len) {
	if (byte_len > (SIZE_MAX / 8)) {
		return -1;
	}
	return __XOF_Squeeze_x8_bits(ctx, data, byte_len * 8);
}
static inline void _XOF_Release_x8(Keccak_HashInstancetimes8 *ctx) {
	(void)ctx;
}
#endif /* XKCP_has_KeccakP1600times8 */

/*************************************/
/********** MUPQ API *****************/
/*************************************/
#elif defined(MQOM3_FOR_MUPQ)
#include "fips202.h"
/* For common helpers */
#include "common.h"
/* Depending on the parameter, the instances are different:
 *   - For 128 bits security, we use SHAKE-128 for XOF
 *   - For 192 bits security, we use SHAKE-256 for XOF
 *   - For 256 bits security, we use SHAKE-256 for XOF
 */
/* === 128 bits security === */
#if MQOM3_PARAM_SECURITY == 128
#define Keccak_HashInstance shake128incctx
#define __XOF_Init shake128_inc_init
#define __XOF_Update shake128_inc_absorb
#define __XOF_Final shake128_inc_finalize
#define __XOF_Squeeze shake128_inc_squeeze
/* === 192 bits security === */
#elif MQOM3_PARAM_SECURITY == 192
#define Keccak_HashInstance shake256incctx
#define __XOF_Init shake256_inc_init
#define __XOF_Update shake256_inc_absorb
#define __XOF_Final shake256_inc_finalize
#define __XOF_Squeeze shake256_inc_squeeze
/* === 256 bits security === */
#elif MQOM3_PARAM_SECURITY == 256
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
	__XOF_Update(ctx, data, byte_len);
	return 0;
}
static inline int _XOF_Update_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t *data[4], size_t byte_len) {
	unsigned int i;
	for (i = 0; i < 4; i++) {
		__XOF_Update(&ctx->ctx[i], data[i], byte_len);
	}
	return 0;
}
/**/
static inline int _XOF_Squeeze(Keccak_HashInstance *ctx, uint8_t* data, size_t byte_len) {
	__XOF_Squeeze(data, byte_len, ctx);
	return 0;
}
static inline int _XOF_Squeeze_x4(Keccak_HashInstancetimes4 *ctx, uint8_t *data[4], size_t byte_len) {
	unsigned int i;
	for (i = 0; i < 4; i++) {
		__XOF_Squeeze(data[i], byte_len, &ctx->ctx[i]);
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
#elif defined(MQOM3_FOR_LIBOQS)
#include <oqs/sha3.h>
#include <oqs/sha3x4.h>
/* For common helpers */
#include "common.h"
/* Depending on the parameter, the instances are different:
 *   - For 128 bits security, we use SHAKE-128 for XOF
 *   - For 192 bits security, we use SHAKE-256 for XOF
 *   - For 256 bits security, we use SHAKE-256 for XOF
 */
#include "mqom3_parameters.h"
/* === 128 bits security === */
#if MQOM3_PARAM_SECURITY == 128
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
#elif MQOM3_PARAM_SECURITY == 192
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
#elif MQOM3_PARAM_SECURITY == 256
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
	__XOF_Update(&(ctx->ctx), data, byte_len);
	return 0;
}
static inline int _XOF_Update_x4(Keccak_HashInstancetimes4 *ctx, const uint8_t *data[4], size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_X4_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Update_x4(&(ctx->ctx), data[0], data[1], data[2], data[3], byte_len);
	return 0;
}
/**/
static inline int _XOF_Squeeze(Keccak_HashInstance *ctx, uint8_t* data, size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Squeeze(data, byte_len, &(ctx->ctx));
	return 0;
}
static inline int _XOF_Squeeze_x4(Keccak_HashInstancetimes4 *ctx, uint8_t *data[4], size_t byte_len) {
	if ((ctx == NULL) || (ctx->magic != XOF_CTX_X4_INIT_MAGIC)) {
		return -1;
	}
	__XOF_Squeeze_x4(data[0], data[1], data[2], data[3], byte_len, &(ctx->ctx));
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
#endif /* MQOM3_XOF_EXTERNAL_API */


/* Deal with namespacing */
#define xof_init MQOM_NAMESPACE(xof_init)
#define xof_update MQOM_NAMESPACE(xof_update)
#define xof_squeeze MQOM_NAMESPACE(xof_squeeze)
#define xof_clean_ctx MQOM_NAMESPACE(xof_clean_ctx)
#define xof_init_x4 MQOM_NAMESPACE(xof_init_x4)
#define xof_update_x4 MQOM_NAMESPACE(xof_update_x4)
#define xof_squeeze_x4 MQOM_NAMESPACE(xof_squeeze_x4)
#define xof_clean_ctx_x4 MQOM_NAMESPACE(xof_clean_ctx_x4)
#define xof_init_x8 MQOM_NAMESPACE(xof_init_x8)
#define xof_update_x8 MQOM_NAMESPACE(xof_update_x8)
#define xof_squeeze_x8 MQOM_NAMESPACE(xof_squeeze_x8)
#define xof_clean_ctx_x8 MQOM_NAMESPACE(xof_clean_ctx_x8)

/* Explicit lifecycle states make cleanup idempotent and prevent releasing a
 * backend context whose initialization failed. */
#define XOF_STATE_UNINITIALIZED 0
#define XOF_STATE_ABSORBING 1
#define XOF_STATE_SQUEEZING 2

typedef struct {
	uint8_t state;
	Keccak_HashInstance ctx;
} xof_context;

/* x4 (4 times) context */
typedef struct {
	uint8_t state;
	Keccak_HashInstancetimes4 ctx;
} xof_context_x4;

/*
 * x8 (8-way parallel) XOF.
 * On AVX-512: Keccak_HashInstancetimes8 is the native 1600-byte SIMD state,
 * and _XOF_*_x8 are thin wrappers around KeccakHashtimes8 (defined above in
 * the local-backend section when XKCP_has_KeccakP1600times8 is set).
 * On all other platforms: fallback to two sequential x4 calls.
 */
#ifndef XKCP_has_KeccakP1600times8
typedef struct {
	Keccak_HashInstancetimes4 lo;  /* instances 0..3 */
	Keccak_HashInstancetimes4 hi;  /* instances 4..7 */
} Keccak_HashInstancetimes8;

static inline int _XOF_Init_x8(Keccak_HashInstancetimes8 *ctx) {
	int r = _XOF_Init_x4(&ctx->lo);
	if (r) return r;
	r = _XOF_Init_x4(&ctx->hi);
	if (r) {
		_XOF_Release_x4(&ctx->lo);
	}
	return r;
}
static inline int _XOF_Update_x8(Keccak_HashInstancetimes8 *ctx, const uint8_t *data[8], size_t byte_len) {
	const uint8_t *lo[4] = { data[0], data[1], data[2], data[3] };
	const uint8_t *hi[4] = { data[4], data[5], data[6], data[7] };
	int r = _XOF_Update_x4(&ctx->lo, lo, byte_len);
	if (r) return r;
	return _XOF_Update_x4(&ctx->hi, hi, byte_len);
}
static inline int _XOF_Final_x8(Keccak_HashInstancetimes8 *ctx, const uint8_t *dummy) {
	int r;
	(void)dummy;
	r = _XOF_Final_x4(&ctx->lo, NULL);
	if (r) return r;
	return _XOF_Final_x4(&ctx->hi, NULL);
}
static inline int _XOF_Squeeze_x8(Keccak_HashInstancetimes8 *ctx, uint8_t *data[8], size_t byte_len) {
	uint8_t *lo[4] = { data[0], data[1], data[2], data[3] };
	uint8_t *hi[4] = { data[4], data[5], data[6], data[7] };
	int r = _XOF_Squeeze_x4(&ctx->lo, lo, byte_len);
	if (r) return r;
	return _XOF_Squeeze_x4(&ctx->hi, hi, byte_len);
}
static inline void _XOF_Release_x8(Keccak_HashInstancetimes8 *ctx) {
	_XOF_Release_x4(&ctx->lo);
	_XOF_Release_x4(&ctx->hi);
}
#endif /* !XKCP_has_KeccakP1600times8 */

/* x8 (8 times) context */
typedef struct {
	uint8_t state;
	Keccak_HashInstancetimes8 ctx;
} xof_context_x8;

/* Exported API for XOF, simple and x4 */
int xof_init(xof_context *ctx);
int xof_update(xof_context *ctx, const uint8_t *data, size_t byte_len);
int xof_squeeze(xof_context *ctx, uint8_t *out, size_t byte_len);
void xof_clean_ctx(xof_context *ctx);

int xof_init_x4(xof_context_x4 *ctx);
int xof_update_x4(xof_context_x4 *ctx, const uint8_t *data[4], size_t byte_len);
int xof_squeeze_x4(xof_context_x4 *ctx, uint8_t *out[4], size_t byte_len);
void xof_clean_ctx_x4(xof_context_x4 *ctx);

int xof_init_x8(xof_context_x8 *ctx);
int xof_update_x8(xof_context_x8 *ctx, const uint8_t *data[8], size_t byte_len);
int xof_squeeze_x8(xof_context_x8 *ctx, uint8_t *out[8], size_t byte_len);
void xof_clean_ctx_x8(xof_context_x8 *ctx);

#endif /* __HASH_XOF_H__ */
