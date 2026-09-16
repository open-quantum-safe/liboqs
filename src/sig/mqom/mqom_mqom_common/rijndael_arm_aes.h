#ifndef __RIJNDAEL_ARM_AES_H__
#define __RIJNDAEL_ARM_AES_H__

/* Check for ARM Crypto Extension (AES) support */
#if !defined(__ARM_FEATURE_CRYPTO) && !defined(__ARM_FEATURE_AES)
#error "Error: asking for RIJNDAEL_ARM_AES while ARM Crypto Extension support is not detected. Check your platform and compilation options (e.g. -march=armv8-a+crypto)!"
#endif

#include "rijndael_common.h"
#include <arm_neon.h>

/* Namespacing */
#define aes128_arm_aes_setkey_enc MQOM_NAMESPACE(aes128_arm_aes_setkey_enc)
#define aes256_arm_aes_setkey_enc MQOM_NAMESPACE(aes256_arm_aes_setkey_enc)
#define rijndael256_arm_aes_setkey_enc MQOM_NAMESPACE(rijndael256_arm_aes_setkey_enc)
#define aes128_arm_aes_enc MQOM_NAMESPACE(aes128_arm_aes_enc)
#define aes256_arm_aes_enc MQOM_NAMESPACE(aes256_arm_aes_enc)
#define rijndael256_arm_aes_enc MQOM_NAMESPACE(rijndael256_arm_aes_enc)
#define aes128_arm_aes_enc_x2 MQOM_NAMESPACE(aes128_arm_aes_enc_x2)
#define aes128_arm_aes_enc_x4 MQOM_NAMESPACE(aes128_arm_aes_enc_x4)
#define aes128_arm_aes_enc_x8 MQOM_NAMESPACE(aes128_arm_aes_enc_x8)
#define aes256_arm_aes_enc_x2 MQOM_NAMESPACE(aes256_arm_aes_enc_x2)
#define aes256_arm_aes_enc_x4 MQOM_NAMESPACE(aes256_arm_aes_enc_x4)
#define aes256_arm_aes_enc_x8 MQOM_NAMESPACE(aes256_arm_aes_enc_x8)
#define rijndael256_arm_aes_enc_x2 MQOM_NAMESPACE(rijndael256_arm_aes_enc_x2)
#define rijndael256_arm_aes_enc_x4 MQOM_NAMESPACE(rijndael256_arm_aes_enc_x4)
#define rijndael256_arm_aes_enc_x8 MQOM_NAMESPACE(rijndael256_arm_aes_enc_x8)
/**/
#define aes128_arm_aes_setkey_enc_x2 MQOM_NAMESPACE(aes128_arm_aes_setkey_enc_x2)
#define aes256_arm_aes_setkey_enc_x2 MQOM_NAMESPACE(aes256_arm_aes_setkey_enc_x2)
#define rijndael256_arm_aes_setkey_enc_x2 MQOM_NAMESPACE(rijndael256_arm_aes_setkey_enc_x2)
#define aes128_arm_aes_setkey_enc_x4 MQOM_NAMESPACE(aes128_arm_aes_setkey_enc_x4)
#define aes256_arm_aes_setkey_enc_x4 MQOM_NAMESPACE(aes256_arm_aes_setkey_enc_x4)
#define rijndael256_arm_aes_setkey_enc_x4 MQOM_NAMESPACE(rijndael256_arm_aes_setkey_enc_x4)
#define aes128_arm_aes_setkey_enc_x8 MQOM_NAMESPACE(aes128_arm_aes_setkey_enc_x8)
#define aes256_arm_aes_setkey_enc_x8 MQOM_NAMESPACE(aes256_arm_aes_setkey_enc_x8)
#define rijndael256_arm_aes_setkey_enc_x8 MQOM_NAMESPACE(rijndael256_arm_aes_setkey_enc_x8)
#define aes128_arm_aes_enc_x2_x2 MQOM_NAMESPACE(aes128_arm_aes_enc_x2_x2)
#define aes128_arm_aes_enc_x4_x4 MQOM_NAMESPACE(aes128_arm_aes_enc_x4_x4)
#define aes128_arm_aes_enc_x8_x8 MQOM_NAMESPACE(aes128_arm_aes_enc_x8_x8)
#define aes256_arm_aes_enc_x2_x2 MQOM_NAMESPACE(aes256_arm_aes_enc_x2_x2)
#define aes256_arm_aes_enc_x4_x4 MQOM_NAMESPACE(aes256_arm_aes_enc_x4_x4)
#define aes256_arm_aes_enc_x8_x8 MQOM_NAMESPACE(aes256_arm_aes_enc_x8_x8)
#define rijndael256_arm_aes_enc_x2_x2 MQOM_NAMESPACE(rijndael256_arm_aes_enc_x2_x2)
#define rijndael256_arm_aes_enc_x4_x4 MQOM_NAMESPACE(rijndael256_arm_aes_enc_x4_x4)
#define rijndael256_arm_aes_enc_x8_x8 MQOM_NAMESPACE(rijndael256_arm_aes_enc_x8_x8)
/**/
#define aes128_arm_aes_setkey_enc_ecb MQOM_NAMESPACE(aes128_arm_aes_setkey_enc_ecb)
#define aes256_arm_aes_setkey_enc_ecb MQOM_NAMESPACE(aes256_arm_aes_setkey_enc_ecb)
#define rijndael256_arm_aes_setkey_enc_ecb MQOM_NAMESPACE(rijndael256_arm_aes_setkey_enc_ecb)
#define aes128_arm_aes_enc_ecb MQOM_NAMESPACE(aes128_arm_aes_enc_ecb)
#define aes256_arm_aes_enc_ecb MQOM_NAMESPACE(aes256_arm_aes_enc_ecb)
#define rijndael256_arm_aes_enc_ecb MQOM_NAMESPACE(rijndael256_arm_aes_enc_ecb)

/* Context structures using uint8x16_t for natural NEON alignment */
typedef struct {
	rijndael_type rtype;
	uint8x16_t rk[11]; /* AES-128: 11 round keys */
} rijndael_arm_aes_ctx_aes128;

typedef struct {
	rijndael_type rtype;
	uint8x16_t rk[15]; /* AES-256: 15 round keys */
} rijndael_arm_aes_ctx_aes256;

typedef struct {
	rijndael_type rtype;
	/* Rijndael-256: 30 half-block (128-bit) round keys = 15 pairs for a 256-bit block */
	uint8x16_t rk[30];
} rijndael_arm_aes_ctx_rijndael256;

/* x2, x4 and x8 contexts */
MAKE_GENERIC_CTX_XX(aes128, arm_aes)
MAKE_GENERIC_CTX_XX(aes256, arm_aes)
MAKE_GENERIC_CTX_XX(rijndael256, arm_aes)
/* ECB contexts (typedef of the single context) */
MAKE_GENERIC_CTX_ECB(aes128, arm_aes)
MAKE_GENERIC_CTX_ECB(aes256, arm_aes)
MAKE_GENERIC_CTX_ECB(rijndael256, arm_aes)

/* ==== Public API ==== */
int aes128_arm_aes_setkey_enc(rijndael_arm_aes_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_arm_aes_setkey_enc(rijndael_arm_aes_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_arm_aes_setkey_enc(rijndael_arm_aes_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_arm_aes_enc(const rijndael_arm_aes_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_arm_aes_enc(const rijndael_arm_aes_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_arm_aes_enc(const rijndael_arm_aes_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2 encryption APIs */
int aes128_arm_aes_enc_x2(const rijndael_arm_aes_ctx_aes128 *ctx1, const rijndael_arm_aes_ctx_aes128 *ctx2,
                           const uint8_t pt1[16], const uint8_t pt2[16],
                           uint8_t ct1[16], uint8_t ct2[16]);
int aes256_arm_aes_enc_x2(const rijndael_arm_aes_ctx_aes256 *ctx1, const rijndael_arm_aes_ctx_aes256 *ctx2,
                           const uint8_t pt1[16], const uint8_t pt2[16],
                           uint8_t ct1[16], uint8_t ct2[16]);
int rijndael256_arm_aes_enc_x2(const rijndael_arm_aes_ctx_rijndael256 *ctx1, const rijndael_arm_aes_ctx_rijndael256 *ctx2,
                                const uint8_t pt1[32], const uint8_t pt2[32],
                                uint8_t ct1[32], uint8_t ct2[32]);
/* x4 encryption APIs */
int aes128_arm_aes_enc_x4(const rijndael_arm_aes_ctx_aes128 *ctx1, const rijndael_arm_aes_ctx_aes128 *ctx2,
                           const rijndael_arm_aes_ctx_aes128 *ctx3, const rijndael_arm_aes_ctx_aes128 *ctx4,
                           const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                           uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]);
int aes256_arm_aes_enc_x4(const rijndael_arm_aes_ctx_aes256 *ctx1, const rijndael_arm_aes_ctx_aes256 *ctx2,
                           const rijndael_arm_aes_ctx_aes256 *ctx3, const rijndael_arm_aes_ctx_aes256 *ctx4,
                           const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                           uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]);
int rijndael256_arm_aes_enc_x4(const rijndael_arm_aes_ctx_rijndael256 *ctx1, const rijndael_arm_aes_ctx_rijndael256 *ctx2,
                                const rijndael_arm_aes_ctx_rijndael256 *ctx3, const rijndael_arm_aes_ctx_rijndael256 *ctx4,
                                const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32]);
/* x8 encryption APIs */
int aes128_arm_aes_enc_x8(const rijndael_arm_aes_ctx_aes128 *ctx1, const rijndael_arm_aes_ctx_aes128 *ctx2,
                           const rijndael_arm_aes_ctx_aes128 *ctx3, const rijndael_arm_aes_ctx_aes128 *ctx4,
                           const rijndael_arm_aes_ctx_aes128 *ctx5, const rijndael_arm_aes_ctx_aes128 *ctx6,
                           const rijndael_arm_aes_ctx_aes128 *ctx7, const rijndael_arm_aes_ctx_aes128 *ctx8,
                           const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                           const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                           uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                           uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]);
int aes256_arm_aes_enc_x8(const rijndael_arm_aes_ctx_aes256 *ctx1, const rijndael_arm_aes_ctx_aes256 *ctx2,
                           const rijndael_arm_aes_ctx_aes256 *ctx3, const rijndael_arm_aes_ctx_aes256 *ctx4,
                           const rijndael_arm_aes_ctx_aes256 *ctx5, const rijndael_arm_aes_ctx_aes256 *ctx6,
                           const rijndael_arm_aes_ctx_aes256 *ctx7, const rijndael_arm_aes_ctx_aes256 *ctx8,
                           const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                           const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                           uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                           uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]);
int rijndael256_arm_aes_enc_x8(const rijndael_arm_aes_ctx_rijndael256 *ctx1, const rijndael_arm_aes_ctx_rijndael256 *ctx2,
                                const rijndael_arm_aes_ctx_rijndael256 *ctx3, const rijndael_arm_aes_ctx_rijndael256 *ctx4,
                                const rijndael_arm_aes_ctx_rijndael256 *ctx5, const rijndael_arm_aes_ctx_rijndael256 *ctx6,
                                const rijndael_arm_aes_ctx_rijndael256 *ctx7, const rijndael_arm_aes_ctx_rijndael256 *ctx8,
                                const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                const uint8_t pt5[32], const uint8_t pt6[32], const uint8_t pt7[32], const uint8_t pt8[32],
                                uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32],
                                uint8_t ct5[32], uint8_t ct6[32], uint8_t ct7[32], uint8_t ct8[32]);

/* Generic setkey_xN and enc_xN_xN boilerplate */
MAKE_GENERIC_FUNCS_XX_DECL(aes128, arm_aes, 16, 16)
MAKE_GENERIC_FUNCS_XX_DECL(aes256, arm_aes, 32, 16)
MAKE_GENERIC_FUNCS_XX_DECL(rijndael256, arm_aes, 32, 32)
/* Generic ECB boilerplate */
MAKE_GENERIC_FUNCS_ECB_DECL(aes128, arm_aes, 16, 16)
MAKE_GENERIC_FUNCS_ECB_DECL(aes256, arm_aes, 32, 16)
MAKE_GENERIC_FUNCS_ECB_DECL(rijndael256, arm_aes, 32, 32)

#endif /* __RIJNDAEL_ARM_AES_H__ */
