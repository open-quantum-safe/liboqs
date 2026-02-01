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

typedef enum {
	AES128 = 0, /* Actually Rijndael_128_128 */
	AES256 = 1, /* Actually Rijndael_128_256  */
	RIJNDAEL_256_256 = 2,
} rijndael_type;

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


#endif /* __RIJNDAEL_COMMON_H__ */
