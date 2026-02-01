#ifndef __ENC_LIBOQS_H__
#define __ENC_LIBOQS_H__

/****************************************/
/********** libOQS API ******************/
/****************************************/
/* For 128 bits security for libOQS when local Rijndael is not defined, we use the AES-128 dedicated API */
/* XXX: this is somehow suboptimal because of how rigid is the API wrt x2, x4 and x8 ... But we accept to lose
 * a little performance here (and actually gain performance on non-AES-NI platforms) */
#include <oqs/aes.h>

#define ENC_CTX_INIT_MAGIC 0xfeedbabebadf000dULL

typedef struct {
	uint64_t magic;
	void *sched_keys;
} enc_ctx;
typedef struct {
	enc_ctx ctx1;
	enc_ctx ctx2;
} enc_ctx_x2;
typedef struct {
	enc_ctx_x2 ctx1;
	enc_ctx_x2 ctx2;
} enc_ctx_x4;
typedef struct {
	enc_ctx_x4 ctx1;
	enc_ctx_x4 ctx2;
} enc_ctx_x8;

static inline int enc_key_sched(enc_ctx *ctx, const uint8_t key[16]) {
	if ((ctx != NULL) && (ctx->magic == ENC_CTX_INIT_MAGIC) && (ctx->sched_keys != NULL)) {
		/* Free the context if it has already been previously allocated */
		OQS_AES128_free_schedule(ctx->sched_keys);
		ctx->magic = 0;
	}
	OQS_AES128_ECB_load_schedule(key, &ctx->sched_keys);
	ctx->magic = ENC_CTX_INIT_MAGIC;
	return 0;
}
static inline int enc_key_sched_x2(enc_ctx_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret;
	ret  = enc_key_sched(&ctx->ctx1, key1);
	ret |= enc_key_sched(&ctx->ctx2, key2);
	return ret;
}
static inline int enc_key_sched_x4(enc_ctx_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret;
	ret  = enc_key_sched_x2(&ctx->ctx1, key1, key2);
	ret |= enc_key_sched_x2(&ctx->ctx2, key3, key4);
	return ret;
}
static inline int enc_key_sched_x8(enc_ctx_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret;
	ret  = enc_key_sched_x4(&ctx->ctx1, key1, key2, key3, key4);
	ret |= enc_key_sched_x4(&ctx->ctx2, key5, key6, key7, key8);
	return ret;
}

static inline int enc_encrypt(const enc_ctx *ctx, const uint8_t pt[16], uint8_t ct[16]) {
	if ((ctx == NULL) || (ctx->magic != ENC_CTX_INIT_MAGIC)) {
		return -1;
	}
	OQS_AES128_ECB_enc_sch(pt, 16, ctx->sched_keys, ct);
	return 0;
}

static inline int enc_encrypt_x2(const enc_ctx *ctx1, const enc_ctx *ctx2, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret = 0;

	ret  = enc_encrypt(ctx1, pt1, ct1);
	ret |= enc_encrypt(ctx2, pt2, ct2);

	return ret;
}
static inline int enc_encrypt_x2_x2(const enc_ctx_x2 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret = 0;

	ret  = enc_encrypt(&ctx->ctx1, pt1, ct1);
	ret |= enc_encrypt(&ctx->ctx2, pt2, ct2);

	return ret;
}

static inline int enc_encrypt_x4(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret = 0;

	ret  = enc_encrypt_x2(ctx1, ctx2, pt1, pt2, ct1, ct2);
	ret |= enc_encrypt_x2(ctx3, ctx4, pt3, pt4, ct3, ct4);

	return ret;
}
static inline int enc_encrypt_x4_x4(const enc_ctx_x4 *ctx,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret = 0;

	ret  = enc_encrypt_x2_x2(&ctx->ctx1, pt1, pt2, ct1, ct2);
	ret |= enc_encrypt_x2_x2(&ctx->ctx2, pt3, pt4, ct3, ct4);

	return ret;
}

static inline int enc_encrypt_x8(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const enc_ctx *ctx5, const enc_ctx *ctx6, const enc_ctx *ctx7, const enc_ctx *ctx8,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                 uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
	int ret = 0;

	ret  = enc_encrypt_x4(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	ret |= enc_encrypt_x4(ctx5, ctx6, ctx7, ctx8, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);

	return ret;
}
static inline int enc_encrypt_x8_x8(const enc_ctx_x8 *ctx,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                 uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
	int ret = 0;

	ret  = enc_encrypt_x4_x4(&ctx->ctx1, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	ret |= enc_encrypt_x4_x4(&ctx->ctx2, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);

	return ret;
}


/* XXX: NOTE: for liboqs, cleansing is mandatory, we always perform it */
static inline void enc_clean_ctx(enc_ctx *ctx) {
	if ((ctx != NULL) && (ctx->magic == ENC_CTX_INIT_MAGIC)) {
		OQS_AES128_free_schedule(ctx->sched_keys);
		ctx->magic = 0;
	}
	return;
}
static inline void enc_clean_ctx_x2(enc_ctx_x2 *ctx) {
	enc_clean_ctx(&ctx->ctx1);
	enc_clean_ctx(&ctx->ctx2);
	return;
}
static inline void enc_clean_ctx_x4(enc_ctx_x4 *ctx) {
	enc_clean_ctx_x2(&ctx->ctx1);
	enc_clean_ctx_x2(&ctx->ctx2);
	return;
}
static inline void enc_clean_ctx_x8(enc_ctx_x8 *ctx) {
	enc_clean_ctx_x4(&ctx->ctx1);
	enc_clean_ctx_x4(&ctx->ctx2);
	return;
}

static inline void enc_uninit_ctx(enc_ctx *ctx) {
	if (ctx != NULL) {
		ctx->magic = 0;
	}
	return;
}
static inline void enc_uninit_ctx_x2(enc_ctx_x2 *ctx) {
	enc_uninit_ctx(&ctx->ctx1);
	enc_uninit_ctx(&ctx->ctx2);
	return;
}
static inline void enc_uninit_ctx_x4(enc_ctx_x4 *ctx) {
	enc_uninit_ctx_x2(&ctx->ctx1);
	enc_uninit_ctx_x2(&ctx->ctx2);
	return;
}
static inline void enc_uninit_ctx_x8(enc_ctx_x8 *ctx) {
	enc_uninit_ctx_x4(&ctx->ctx1);
	enc_uninit_ctx_x4(&ctx->ctx2);
	return;
}


#define enc_ctx_pub enc_ctx
#define enc_ctx_pub_x2 enc_ctx_x2
#define enc_ctx_pub_x4 enc_ctx_x4
#define enc_ctx_pub_x8 enc_ctx_x8
/**/
#define enc_key_sched_pub enc_key_sched
#define enc_key_sched_pub_x2 enc_key_sched_x2
#define enc_key_sched_pub_x4 enc_key_sched_x4
#define enc_key_sched_pub_x8 enc_key_sched_x8
/**/
#define enc_encrypt_pub enc_encrypt
#define enc_encrypt_x2_pub enc_encrypt_x2
#define enc_encrypt_x4_pub enc_encrypt_x4
#define enc_encrypt_x8_pub enc_encrypt_x8
#define enc_encrypt_x2_pub_x2 enc_encrypt_x2_x2
#define enc_encrypt_x4_pub_x4 enc_encrypt_x4_x4
#define enc_encrypt_x8_pub_x8 enc_encrypt_x8_x8
/**/
#define enc_clean_ctx_pub enc_clean_ctx
#define enc_uninit_ctx_pub enc_uninit_ctx
#define enc_clean_ctx_pub_x2 enc_clean_ctx_x2
#define enc_uninit_ctx_pub_x2 enc_uninit_ctx_x2
#define enc_clean_ctx_pub_x4 enc_clean_ctx_x4
#define enc_uninit_ctx_pub_x4 enc_uninit_ctx_x4
#define enc_clean_ctx_pub_x8 enc_clean_ctx_x8
#define enc_uninit_ctx_pub_x8 enc_uninit_ctx_x8

#endif /* __ENC_LIBOQS_H__ */
