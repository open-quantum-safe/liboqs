#ifndef __ENC_H__
#define __ENC_H__

/* MQOM2 parameters */
#include "mqom2_parameters.h"
/* Common helpers */
#include "common.h"


#if defined(MQOM2_FOR_LIBOQS) && defined(MQOM2_FOR_MUPQ)
#error "MQOM2_FOR_LIBOQS and MQOM2_FOR_MUPQ are not compatible options ..."
#endif

/* Rijndael external, MQOM2_FOR_MUPQ and MQOM2_FOR_LIBOQS cat1 are not compatible */
#if defined(RIJNDAEL_EXTERNAL) && defined(MQOM2_FOR_LIBOQS) && (MQOM2_PARAM_SECURITY == 128)
#error "Error: RIJNDAEL_EXTERNAL and MQOM2_FOR_LIBOQS with MQOM2_PARAM_SECURITY = 128 are NOT compatible."
#endif
#if defined(RIJNDAEL_EXTERNAL) && defined(MQOM2_FOR_MUPQ) && (MQOM2_PARAM_SECURITY == 128)
#error "Error: RIJNDAEL_EXTERNAL and MQOM2_FOR_MUPQ with MQOM2_PARAM_SECURITY = 128 are NOT compatible."
#endif

/* Counting cycles for symmetric crypto in MUPQ */
#if defined(MQOM2_FOR_MUPQ) && defined(PROFILE_HASHING)
#include "hal.h"
extern unsigned long long hash_cycles;
#define MQOM2_SYM_MEASURE_PRE \
        uint64_t t0 = hal_get_time();

#define MQOM2_SYM_MEASURE_POST \
        uint64_t t1 = hal_get_time(); \
        hash_cycles += (t1 - t0); \

#else
#define MQOM2_SYM_MEASURE_PRE
#define MQOM2_SYM_MEASURE_POST
#endif


/****************************************/
/********** libOQS API ******************/
/****************************************/
#if defined(MQOM2_FOR_LIBOQS) && !defined(MQOM2_FOR_LIBOQS_LOCAL_RIJNDAEL) && (MQOM2_PARAM_SECURITY == 128)
#define MQOM2_SEC128_OVERRIDE /* Tell the local layer that cat1 is overriden */
#include "enc_liboqs.h"
#endif

/*************************************/
/********** MUPQ API *****************/
/*************************************/
#if defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_MUPQ_LOCAL_RIJNDAEL) && (MQOM2_PARAM_SECURITY == 128)
#define MQOM2_SEC128_OVERRIDE /* Tell the local layer that cat1 is overriden */
#include "enc_mupq.h"
#endif


/*************************************/
/********** Local ********************/
/*************************************/
#include "enc_local.h"


/*** Contexts cleaning functions ****/
/* Common encryption context cleaning function, except for libOQS cat 1 since we have a dedicated
 * freeing function for this case, and except for external Rijndael where we expect that the use
 * provides theses functions */
#if defined(RIJNDAEL_EXTERNAL)
extern void enc_clean_ctx(enc_ctx *ctx);
extern void enc_clean_ctx_x2(enc_ctx_x2 *ctx);
extern void enc_clean_ctx_x4(enc_ctx_x4 *ctx);
extern void enc_clean_ctx_x8(enc_ctx_x8 *ctx);
extern void enc_clean_ctx_pub(enc_ctx_pub *ctx);
extern void enc_clean_ctx_pub_x2(enc_ctx_pub_x2 *ctx);
extern void enc_clean_ctx_pub_x4(enc_ctx_pub_x4 *ctx);
extern void enc_clean_ctx_pub_x8(enc_ctx_pub_x8 *ctx);
extern void enc_uninit_ctx(enc_ctx *ctx);
extern void enc_uninit_ctx_x2(enc_ctx_x2 *ctx);
extern void enc_uninit_ctx_x4(enc_ctx_x4 *ctx);
extern void enc_uninit_ctx_x8(enc_ctx_x8 *ctx);
extern void enc_uninit_ctx_pub(enc_ctx_pub *ctx);
extern void enc_uninit_ctx_pub_x2(enc_ctx_pub_x2 *ctx);
extern void enc_uninit_ctx_pub_x4(enc_ctx_pub_x4 *ctx);
extern void enc_uninit_ctx_pub_x8(enc_ctx_pub_x8 *ctx);
#else
#if !defined(USE_ENC_CTX_CLEANSING)
#define enc_clean_ctx(ctx)
#define enc_clean_ctx_x2(ctx)
#define enc_clean_ctx_x4(ctx)
#define enc_clean_ctx_x8(ctx)
#define enc_clean_ctx_pub(ctx)
#define enc_clean_ctx_pub_x2(ctx)
#define enc_clean_ctx_pub_x4(ctx)
#define enc_clean_ctx_pub_x8(ctx)
#define enc_uninit_ctx(ctx)
#define enc_uninit_ctx_x2(ctx)
#define enc_uninit_ctx_x4(ctx)
#define enc_uninit_ctx_x8(ctx)
#define enc_uninit_ctx_pub(ctx)
#define enc_uninit_ctx_pub_x2(ctx)
#define enc_uninit_ctx_pub_x4(ctx)
#define enc_uninit_ctx_pub_x8(ctx)
#else
#if !defined(MQOM2_FOR_LIBOQS) || (defined(MQOM2_FOR_LIBOQS) && (MQOM2_PARAM_SECURITY != 128))
static inline void enc_clean_ctx(enc_ctx *ctx) {
        if (ctx != NULL) {
                mqom_cleanse(ctx, sizeof(enc_ctx));
        }
}
static inline void enc_clean_ctx_x2(enc_ctx_x2 *ctx) {
	if(ctx != NULL){
		mqom_cleanse(ctx, sizeof(enc_ctx_x2));
	}
}
static inline void enc_clean_ctx_x4(enc_ctx_x4 *ctx) {
	if(ctx != NULL){
		mqom_cleanse(ctx, sizeof(enc_ctx_x4));
	}
}
static inline void enc_clean_ctx_x8(enc_ctx_x8 *ctx) {
	if(ctx != NULL){
		mqom_cleanse(ctx, sizeof(enc_ctx_x8));
	}
}

static inline void enc_clean_ctx_pub(enc_ctx_pub *ctx) {
        if (ctx != NULL) {
                mqom_cleanse(ctx, sizeof(enc_ctx));
        }
}
static inline void enc_clean_ctx_pub_x2(enc_ctx_pub_x2 *ctx) {
	if(ctx != NULL){
		mqom_cleanse(ctx, sizeof(enc_ctx_pub_x2));
	}
}
static inline void enc_clean_ctx_pub_x4(enc_ctx_pub_x4 *ctx) {
	if(ctx != NULL){
		mqom_cleanse(ctx, sizeof(enc_ctx_pub_x4));
	}
}
static inline void enc_clean_ctx_pub_x8(enc_ctx_pub_x8 *ctx) {
	if(ctx != NULL){
		mqom_cleanse(ctx, sizeof(enc_ctx_pub_x8));
	}
}

static inline void enc_uninit_ctx(enc_ctx *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_x2(enc_ctx_x2 *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_x4(enc_ctx_x4 *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_x8(enc_ctx_x8 *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_pub(enc_ctx_pub *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_pub_x2(enc_ctx_pub_x2 *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_pub_x4(enc_ctx_pub_x4 *ctx) {
        (void)ctx;
}
static inline void enc_uninit_ctx_pub_x8(enc_ctx_pub_x8 *ctx) {
        (void)ctx;
}
#endif
#endif /* ENC_CTXCLEANSING */
#endif /* RIJNDAEL_EXTERNAL */

#endif /* __ENC_H__ */
