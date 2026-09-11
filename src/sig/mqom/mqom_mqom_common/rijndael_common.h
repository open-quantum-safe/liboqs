#ifndef __RIJNDAEL_COMMON_H__
#define __RIJNDAEL_COMMON_H__

/* Common includes for all the implementation variants of
 * Rijndael */
#include <stdint.h>
#include <string.h>

/* Namespacing with the appropriate prefix */
#ifndef MQOM_NAMESPACE
#ifdef APPLY_NAMESPACE
#ifndef concat2
#define _concat2(a, b) a ## b
#define concat2(a, b) _concat2(a, b)
#endif
#define MQOM_NAMESPACE(s) concat2(APPLY_NAMESPACE, s)
#else
#define MQOM_NAMESPACE(s) s
#endif
#endif

/* Alignment macro */
#ifdef ALIGN
#undef ALIGN
#endif
#if defined(__GNUC__)
#define ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#define ALIGN(x) __declspec(align(x))
#elif defined(__ARMCC_VERSION)
#define ALIGN(x) __align(x)
#else
#define ALIGN(x)
#endif

/* Packing macro */
#ifdef PACKED_BEGIN
#undef PACKED_BEGIN
#endif
#ifdef PACKED_END
#undef PACKED_END
#endif
#if defined(_MSC_VER)
/* MSVC uses pragmas */
#define PACKED_BEGIN __pragma(pack(push, 1))
#define PACKED_END   __pragma(pack(pop))
#elif defined(__GNUC__) || defined(__clang__)
/* GCC / Clang */
#define PACKED_BEGIN
#define PACKED_END   __attribute__((packed))
#elif defined(__ARMCC_VERSION)
/* ARM Compiler */
#define PACKED_BEGIN
#define PACKED_END   __packed
#else
/* Fallback */
#define PACKED_BEGIN
#define PACKED_END
#endif


typedef enum {
	AES128 = 0, /* Actually Rijndael_128_128 */
	AES256 = 1, /* Actually Rijndael_128_256  */
	RIJNDAEL_256_256 = 2,
} rijndael_type;

/* Wipe a buffer, defeating dead-store elimination via a volatile function
 * pointer (same technique as mqom_cleanse in common.h). Self-contained here
 * so the rijndael/ backends do not need to depend on the higher-level
 * common.h just to erase intermediate round-key material. */
static inline void rijndael_cleanse(void *ptr, size_t len) {
	static void *(*volatile rijndael_memset_ptr)(void*, int, size_t) = memset;
	if (ptr != NULL) {
		rijndael_memset_ptr(ptr, 0, len);
	}
}

/* Macros to ease dealing with automatic x2, x4 and x8 contexts */
#define MAKE_GENERIC_CTX_XX(aes_alg, aes_impl) \
typedef struct { \
	rijndael_ ## aes_impl ## _ctx_ ## aes_alg ctx[2]; \
} \
rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2; \
typedef struct { \
	rijndael_ ## aes_impl ## _ctx_ ## aes_alg ctx[4]; \
} \
rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4; \
typedef struct { \
	rijndael_ ## aes_impl ## _ctx_ ## aes_alg ctx[8]; \
} \
rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8; \

#define MAKE_GENERIC_FUNCS_XX_DECL(aes_alg, aes_impl, szkey, sztext) \
int aes_alg ## _ ## aes_impl ## _setkey_enc_x2(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey]); \
int aes_alg ## _ ## aes_impl ## _setkey_enc_x4(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey], const uint8_t key3[szkey], const uint8_t key4[szkey]); \
int aes_alg ## _ ## aes_impl ## _setkey_enc_x8(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey], const uint8_t key3[szkey], const uint8_t key4[szkey], const uint8_t key5[szkey], const uint8_t key6[szkey], const uint8_t key7[szkey], const uint8_t key8[szkey]); \
int aes_alg ## _ ## aes_impl ## _enc_x2_x2(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], uint8_t cipherText1[sztext], uint8_t cipherText2[sztext]); \
int aes_alg ## _ ## aes_impl ## _enc_x4_x4(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], const uint8_t plainText3[sztext], const uint8_t plainText4[sztext], \
                  uint8_t cipherText1[sztext], uint8_t cipherText2[sztext], uint8_t cipherText3[sztext], uint8_t cipherText4[sztext]); \
int aes_alg ## _ ## aes_impl ## _enc_x8_x8(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8 *ctx, \
                  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], const uint8_t plainText3[sztext], const uint8_t plainText4[sztext], \
                  const uint8_t plainText5[sztext], const uint8_t plainText6[sztext], const uint8_t plainText7[sztext], const uint8_t plainText8[sztext], \
                  uint8_t cipherText1[sztext], uint8_t cipherText2[sztext], uint8_t cipherText3[sztext], uint8_t cipherText4[sztext], \
                  uint8_t cipherText5[sztext], uint8_t cipherText6[sztext], uint8_t cipherText7[sztext], uint8_t cipherText8[sztext]);

#define MAKE_GENERIC_FUNCS_XX_IMPL(aes_alg, aes_impl, szkey, sztext) \
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_x2(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey]){ \
	int ret = 0; \
	ret =  aes_alg ## _ ## aes_impl ## _setkey_enc((rijndael_ ## aes_impl ## _ctx_ ## aes_alg*)&ctx->ctx[0], key1); \
	ret |= aes_alg ## _ ## aes_impl ## _setkey_enc((rijndael_ ## aes_impl ## _ctx_ ## aes_alg*)&ctx->ctx[1], key2); \
	return ret; \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_x4(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey], const uint8_t key3[szkey], const uint8_t key4[szkey]){ \
	int ret = 0; \
	ret =  aes_alg ## _ ## aes_impl ## _setkey_enc_x2((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2*)&ctx->ctx[0], key1, key2); \
	ret |= aes_alg ## _ ## aes_impl ## _setkey_enc_x2((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2*)&ctx->ctx[2], key3, key4); \
	return ret; \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_x8(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey], const uint8_t key3[szkey], const uint8_t key4[szkey], const uint8_t key5[szkey], const uint8_t key6[szkey], const uint8_t key7[szkey], const uint8_t key8[szkey]){ \
	int ret = 0; \
	ret =  aes_alg ## _ ## aes_impl ## _setkey_enc_x4((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4*)&ctx->ctx[0], key1, key2, key3, key4); \
	ret |= aes_alg ## _ ## aes_impl ## _setkey_enc_x4((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4*)&ctx->ctx[4], key5, key6, key7, key8); \
	return ret; \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _enc_x2_x2(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], uint8_t cipherText1[sztext], uint8_t cipherText2[sztext]){ \
	int ret = 0; \
	ret =  aes_alg ## _ ## aes_impl ## _enc((rijndael_ ## aes_impl ## _ctx_ ## aes_alg*)&ctx->ctx[0], plainText1, cipherText1); \
	ret |= aes_alg ## _ ## aes_impl ## _enc((rijndael_ ## aes_impl ## _ctx_ ## aes_alg*)&ctx->ctx[1], plainText2, cipherText2); \
	return ret; \
}\
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _enc_x4_x4(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], const uint8_t plainText3[sztext], const uint8_t plainText4[sztext], \
                  uint8_t cipherText1[sztext], uint8_t cipherText2[sztext], uint8_t cipherText3[sztext], uint8_t cipherText4[sztext]){ \
	int ret = 0; \
	ret =  aes_alg ## _ ## aes_impl ## _enc_x2_x2((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2*)&ctx->ctx[0], plainText1, plainText2, cipherText1, cipherText2); \
	ret |= aes_alg ## _ ## aes_impl ## _enc_x2_x2((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2*)&ctx->ctx[2], plainText3, plainText4, cipherText3, cipherText4); \
	return ret; \
}\
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _enc_x8_x8(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8 *ctx, \
                  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], const uint8_t plainText3[sztext], const uint8_t plainText4[sztext], \
                  const uint8_t plainText5[sztext], const uint8_t plainText6[sztext], const uint8_t plainText7[sztext], const uint8_t plainText8[sztext], \
                  uint8_t cipherText1[sztext], uint8_t cipherText2[sztext], uint8_t cipherText3[sztext], uint8_t cipherText4[sztext], \
                  uint8_t cipherText5[sztext], uint8_t cipherText6[sztext], uint8_t cipherText7[sztext], uint8_t cipherText8[sztext]) { \
	int ret = 0; \
	ret =  aes_alg ## _ ## aes_impl ## _enc_x4_x4((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4*)&ctx->ctx[0], plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4); \
	ret |= aes_alg ## _ ## aes_impl ## _enc_x4_x4((rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4*)&ctx->ctx[4], plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8); \
	return ret; \
}

/* Setkey-only subset of MAKE_GENERIC_FUNCS_XX_IMPL; use when enc_xN_xN are
 * overridden manually (e.g. AES-NI with truly interleaved enc_xN variants). */
#define MAKE_GENERIC_SETKEY_XX_IMPL(aes_alg, aes_impl, szkey, sztext) \
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_x2(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey]){ \
	int ret; \
	if (ctx == NULL) { return -1; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[0], key1); \
	if (ret) { return ret; } \
	return aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[1], key2); \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_x4(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey], const uint8_t key3[szkey], const uint8_t key4[szkey]){ \
	int ret; \
	if (ctx == NULL) { return -1; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[0], key1); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[1], key2); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[2], key3); \
	if (ret) { return ret; } \
	return aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[3], key4); \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_x8(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8 *ctx, const uint8_t key1[szkey], const uint8_t key2[szkey], const uint8_t key3[szkey], const uint8_t key4[szkey], const uint8_t key5[szkey], const uint8_t key6[szkey], const uint8_t key7[szkey], const uint8_t key8[szkey]){ \
	int ret; \
	if (ctx == NULL) { return -1; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[0], key1); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[1], key2); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[2], key3); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[3], key4); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[4], key5); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[5], key6); \
	if (ret) { return ret; } \
	ret = aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[6], key7); \
	if (ret) { return ret; } \
	return aes_alg ## _ ## aes_impl ## _setkey_enc(&ctx->ctx[7], key8); \
}

/* Compound-context encryption wrappers for implementations providing true
 * parallel enc_x2/enc_x4/enc_x8 cores. */
#define MAKE_PARALLEL_FUNCS_XX_IMPL(aes_alg, aes_impl, sztext) \
WEAK int aes_alg ## _ ## aes_impl ## _enc_x2_x2(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x2 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], uint8_t cipherText1[sztext], uint8_t cipherText2[sztext]){ \
	if (ctx == NULL) { return -1; } \
	return aes_alg ## _ ## aes_impl ## _enc_x2(&ctx->ctx[0], &ctx->ctx[1], \
		plainText1, plainText2, cipherText1, cipherText2); \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _enc_x4_x4(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x4 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], const uint8_t plainText3[sztext], const uint8_t plainText4[sztext], \
		  uint8_t cipherText1[sztext], uint8_t cipherText2[sztext], uint8_t cipherText3[sztext], uint8_t cipherText4[sztext]){ \
	if (ctx == NULL) { return -1; } \
	return aes_alg ## _ ## aes_impl ## _enc_x4(&ctx->ctx[0], &ctx->ctx[1], &ctx->ctx[2], &ctx->ctx[3], \
		plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4); \
} \
/**/\
WEAK int aes_alg ## _ ## aes_impl ## _enc_x8_x8(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _x8 *ctx, \
		  const uint8_t plainText1[sztext], const uint8_t plainText2[sztext], const uint8_t plainText3[sztext], const uint8_t plainText4[sztext], \
		  const uint8_t plainText5[sztext], const uint8_t plainText6[sztext], const uint8_t plainText7[sztext], const uint8_t plainText8[sztext], \
		  uint8_t cipherText1[sztext], uint8_t cipherText2[sztext], uint8_t cipherText3[sztext], uint8_t cipherText4[sztext], \
		  uint8_t cipherText5[sztext], uint8_t cipherText6[sztext], uint8_t cipherText7[sztext], uint8_t cipherText8[sztext]) { \
	if (ctx == NULL) { return -1; } \
	return aes_alg ## _ ## aes_impl ## _enc_x8(&ctx->ctx[0], &ctx->ctx[1], &ctx->ctx[2], &ctx->ctx[3], \
		&ctx->ctx[4], &ctx->ctx[5], &ctx->ctx[6], &ctx->ctx[7], \
		plainText1, plainText2, plainText3, plainText4, plainText5, plainText6, plainText7, plainText8, \
		cipherText1, cipherText2, cipherText3, cipherText4, cipherText5, cipherText6, cipherText7, cipherText8); \
}


/* Macros to ease dealing with automatic ECB contexts */
#define MAKE_GENERIC_CTX_ECB(aes_alg, aes_impl) \
typedef rijndael_ ## aes_impl ## _ctx_ ## aes_alg rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb; \

#define MAKE_GENERIC_FUNCS_ECB_DECL(aes_alg, aes_impl, szkey, sztext) \
int aes_alg ## _ ## aes_impl ## _setkey_enc_ecb(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb *ctx, const uint8_t key[szkey]); \
int aes_alg ## _ ## aes_impl ## _enc_ecb(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb *ctx, uint32_t nblocks, const uint8_t* in, uint8_t* out);

#define MAKE_GENERIC_FUNCS_ECB_IMPL(aes_alg, aes_impl, szkey, sztext) \
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_ecb(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb *ctx, const uint8_t key[szkey]){ \
	return aes_alg ## _ ## aes_impl ## _setkey_enc(ctx, key); \
} \
/* */\
WEAK int aes_alg ## _ ## aes_impl ## _enc_ecb(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb *ctx, uint32_t nblocks, const uint8_t* in, uint8_t* out) { \
	int ret = 0; \
	uint32_t i = 0; \
	for (; i + 8 <= nblocks; i += 8) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+0)*sztext], &out[(i+0)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+1)*sztext], &out[(i+1)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+2)*sztext], &out[(i+2)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+3)*sztext], &out[(i+3)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+4)*sztext], &out[(i+4)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+5)*sztext], &out[(i+5)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+6)*sztext], &out[(i+6)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+7)*sztext], &out[(i+7)*sztext]); \
	} \
	if (i + 4 <= nblocks) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+0)*sztext], &out[(i+0)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+1)*sztext], &out[(i+1)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+2)*sztext], &out[(i+2)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+3)*sztext], &out[(i+3)*sztext]); \
		i += 4; \
	} \
	if (i + 2 <= nblocks) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+0)*sztext], &out[(i+0)*sztext]); \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[(i+1)*sztext], &out[(i+1)*sztext]); \
		i += 2; \
	} \
	if (i < nblocks) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[i*sztext], &out[i*sztext]); \
	} \
	return ret; \
}


/* ECB using enc_x8/x4/x2 for implementations that have true SIMD parallel xN functions.
 * Passing the same ctx pointer N times gives same-key CTR-mode parallelism: the xN
 * function interleaves N independent AES pipelines, hiding per-round latency. */
#define MAKE_PARALLEL_FUNCS_ECB_IMPL(aes_alg, aes_impl, szkey, sztext) \
WEAK int aes_alg ## _ ## aes_impl ## _setkey_enc_ecb(rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb *ctx, const uint8_t key[szkey]){ \
	return aes_alg ## _ ## aes_impl ## _setkey_enc(ctx, key); \
} \
/* */\
WEAK int aes_alg ## _ ## aes_impl ## _enc_ecb(const rijndael_ ## aes_impl ## _ctx_ ## aes_alg ## _ecb *ctx, uint32_t nblocks, const uint8_t* in, uint8_t* out) { \
	int ret = 0; \
	uint32_t i = 0; \
	for (; i + 8 <= nblocks; i += 8) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc_x8(ctx, ctx, ctx, ctx, ctx, ctx, ctx, ctx, \
			&in[(i+0)*(sztext)], &in[(i+1)*(sztext)], &in[(i+2)*(sztext)], &in[(i+3)*(sztext)], \
			&in[(i+4)*(sztext)], &in[(i+5)*(sztext)], &in[(i+6)*(sztext)], &in[(i+7)*(sztext)], \
			&out[(i+0)*(sztext)], &out[(i+1)*(sztext)], &out[(i+2)*(sztext)], &out[(i+3)*(sztext)], \
			&out[(i+4)*(sztext)], &out[(i+5)*(sztext)], &out[(i+6)*(sztext)], &out[(i+7)*(sztext)]); \
	} \
	if (i + 4 <= nblocks) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc_x4(ctx, ctx, ctx, ctx, \
			&in[(i+0)*(sztext)], &in[(i+1)*(sztext)], &in[(i+2)*(sztext)], &in[(i+3)*(sztext)], \
			&out[(i+0)*(sztext)], &out[(i+1)*(sztext)], &out[(i+2)*(sztext)], &out[(i+3)*(sztext)]); \
		i += 4; \
	} \
	if (i + 2 <= nblocks) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc_x2(ctx, ctx, \
			&in[(i+0)*(sztext)], &in[(i+1)*(sztext)], \
			&out[(i+0)*(sztext)], &out[(i+1)*(sztext)]); \
		i += 2; \
	} \
	if (i < nblocks) { \
		ret |= aes_alg ## _ ## aes_impl ## _enc(ctx, &in[i*(sztext)], &out[i*(sztext)]); \
	} \
	return ret; \
}


#endif /* __RIJNDAEL_COMMON_H__ */
