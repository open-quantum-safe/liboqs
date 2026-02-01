#ifndef __RIJNDAEL_REF_H__
#define __RIJNDAEL_REF_H__

#include "rijndael_common.h"

/* Deal with namespacing */
#define aes128_ref_setkey_enc MQOM_NAMESPACE(aes128_ref_setkey_enc)
#define aes256_ref_setkey_enc MQOM_NAMESPACE(aes256_ref_setkey_enc)
#define rijndael256_ref_setkey_enc MQOM_NAMESPACE(rijndael256_ref_setkey_enc)
#define aes128_ref_enc MQOM_NAMESPACE(aes128_ref_enc)
#define aes256_ref_enc MQOM_NAMESPACE(aes256_ref_enc)
#define rijndael256_ref_enc MQOM_NAMESPACE(rijndael256_ref_enc)
#define aes128_ref_enc_x2 MQOM_NAMESPACE(aes128_ref_enc_x2)
#define aes128_ref_enc_x4 MQOM_NAMESPACE(aes128_ref_enc_x4)
#define aes128_ref_enc_x8 MQOM_NAMESPACE(aes128_ref_enc_x8)
#define aes256_ref_enc_x2 MQOM_NAMESPACE(aes256_ref_enc_x2)
#define aes256_ref_enc_x4 MQOM_NAMESPACE(aes256_ref_enc_x4)
#define aes256_ref_enc_x8 MQOM_NAMESPACE(aes256_ref_enc_x8)
#define rijndael256_ref_enc_x2 MQOM_NAMESPACE(rijndael256_ref_enc_x2)
#define rijndael256_ref_enc_x4 MQOM_NAMESPACE(rijndael256_ref_enc_x4)
#define rijndael256_ref_enc_x8 MQOM_NAMESPACE(rijndael256_ref_enc_x8)
/**/
#define aes128_ref_setkey_enc_x2 MQOM_NAMESPACE(aes128_ref_setkey_enc_x2)
#define aes256_ref_setkey_enc_x2 MQOM_NAMESPACE(aes256_ref_setkey_enc_x2)
#define rijndael256_ref_setkey_enc_x2 MQOM_NAMESPACE(rijndael256_ref_setkey_enc_x2)
#define aes128_ref_setkey_enc_x4 MQOM_NAMESPACE(aes128_ref_setkey_enc_x4)
#define aes256_ref_setkey_enc_x4 MQOM_NAMESPACE(aes256_ref_setkey_enc_x4)
#define rijndael256_ref_setkey_enc_x4 MQOM_NAMESPACE(rijndael256_ref_setkey_enc_x4)
#define aes128_ref_setkey_enc_x8 MQOM_NAMESPACE(aes128_ref_setkey_enc_x8)
#define aes256_ref_setkey_enc_x8 MQOM_NAMESPACE(aes256_ref_setkey_enc_x8)
#define rijndael256_ref_setkey_enc_x8 MQOM_NAMESPACE(rijndael256_ref_setkey_enc_x8)
#define aes128_ref_enc_x2_x2 MQOM_NAMESPACE(aes128_ref_enc_x2_x2)
#define aes128_ref_enc_x4_x4 MQOM_NAMESPACE(aes128_ref_enc_x4_x4)
#define aes128_ref_enc_x8_x8 MQOM_NAMESPACE(aes128_ref_enc_x8_x8)
#define aes256_ref_enc_x2_x2 MQOM_NAMESPACE(aes256_ref_enc_x2_x2)
#define aes256_ref_enc_x4_x4 MQOM_NAMESPACE(aes256_ref_enc_x4_x4)
#define aes256_ref_enc_x8_x8 MQOM_NAMESPACE(aes256_ref_enc_x8_x8)
#define rijndael256_ref_enc_x2_x2 MQOM_NAMESPACE(rijndael256_ref_enc_x2_x2)
#define rijndael256_ref_enc_x4_x4 MQOM_NAMESPACE(rijndael256_ref_enc_x4_x4)
#define rijndael256_ref_enc_x8_x8 MQOM_NAMESPACE(rijndael256_ref_enc_x8_x8)

/* The general Rijndael core context structure */
typedef struct  __attribute__((packed)) {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	/* NOTE: alignment to allow uint32_t dereference on platforms where unalignment might be an issue */
	__attribute__((aligned(4))) uint8_t rk[176];  /* Round keys    */
}
rijndael_ref_ctx_aes128;
typedef struct  __attribute__((packed)) {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	/* NOTE: alignment to allow uint32_t dereference on platforms where unalignment might be an issue */
	__attribute__((aligned(4))) uint8_t rk[240];  /* Round keys    */
}
rijndael_ref_ctx_aes256;
typedef struct  __attribute__((packed)) {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	/* NOTE: alignment to allow uint32_t dereference on platforms where unalignment might be an issue */
	__attribute__((aligned(4))) uint8_t rk[480];  /* Round keys    */
}
rijndael_ref_ctx_rijndael256;

/* x2, x4 and x8 contexts */
MAKE_GENERIC_CTX_XX(aes128, ref)
MAKE_GENERIC_CTX_XX(aes256, ref)
MAKE_GENERIC_CTX_XX(rijndael256, ref)

/* ==== Public API ==== */
int aes128_ref_setkey_enc(rijndael_ref_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_ref_setkey_enc(rijndael_ref_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_ref_setkey_enc(rijndael_ref_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_ref_enc(const rijndael_ref_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_ref_enc(const rijndael_ref_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_ref_enc(const rijndael_ref_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2 and x4 encryption APIs */
int aes128_ref_enc_x2(const rijndael_ref_ctx_aes128 *ctx1, const rijndael_ref_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_ref_enc_x4(const rijndael_ref_ctx_aes128 *ctx1, const rijndael_ref_ctx_aes128 *ctx2, const rijndael_ref_ctx_aes128 *ctx3, const rijndael_ref_ctx_aes128 *ctx4,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_ref_enc_x8(const rijndael_ref_ctx_aes128 *ctx1, const rijndael_ref_ctx_aes128 *ctx2, const rijndael_ref_ctx_aes128 *ctx3, const rijndael_ref_ctx_aes128 *ctx4,
                      const rijndael_ref_ctx_aes128 *ctx5, const rijndael_ref_ctx_aes128 *ctx6, const rijndael_ref_ctx_aes128 *ctx7, const rijndael_ref_ctx_aes128 *ctx8,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                      uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_ref_enc_x2(const rijndael_ref_ctx_aes256 *ctx1, const rijndael_ref_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_ref_enc_x4(const rijndael_ref_ctx_aes256 *ctx1, const rijndael_ref_ctx_aes256 *ctx2, const rijndael_ref_ctx_aes256 *ctx3, const rijndael_ref_ctx_aes256 *ctx4,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_ref_enc_x8(const rijndael_ref_ctx_aes256 *ctx1, const rijndael_ref_ctx_aes256 *ctx2, const rijndael_ref_ctx_aes256 *ctx3, const rijndael_ref_ctx_aes256 *ctx4,
                      const rijndael_ref_ctx_aes256 *ctx5, const rijndael_ref_ctx_aes256 *ctx6, const rijndael_ref_ctx_aes256 *ctx7, const rijndael_ref_ctx_aes256 *ctx8,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                      uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int rijndael256_ref_enc_x2(const rijndael_ref_ctx_rijndael256 *ctx1, const rijndael_ref_ctx_rijndael256 *ctx2,
                           const uint8_t plainText1[32], const uint8_t plainText2[32],
                           uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_ref_enc_x4(const rijndael_ref_ctx_rijndael256 *ctx1, const rijndael_ref_ctx_rijndael256 *ctx2, const rijndael_ref_ctx_rijndael256 *ctx3, const rijndael_ref_ctx_rijndael256 *ctx4,
                           const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                           uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_ref_enc_x8(const rijndael_ref_ctx_rijndael256 *ctx1, const rijndael_ref_ctx_rijndael256 *ctx2, const rijndael_ref_ctx_rijndael256 *ctx3, const rijndael_ref_ctx_rijndael256 *ctx4,
                           const rijndael_ref_ctx_rijndael256 *ctx5, const rijndael_ref_ctx_rijndael256 *ctx6, const rijndael_ref_ctx_rijndael256 *ctx7, const rijndael_ref_ctx_rijndael256 *ctx8,
                           const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                           const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                           uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                           uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);

MAKE_GENERIC_FUNCS_XX_DECL(aes128, ref, 16, 16)
MAKE_GENERIC_FUNCS_XX_DECL(aes256, ref, 32, 16)
MAKE_GENERIC_FUNCS_XX_DECL(rijndael256, ref, 32, 32)

#endif /* __RIJNDAEL_REF_H__ */
