#ifndef __RIJNDAEL_CT64_H__
#define __RIJNDAEL_CT64_H__

#include "rijndael_common.h"

#include <stdbool.h>

/* Deal with namespacing */
#define aes128_ct64_setkey_enc MQOM_NAMESPACE(aes128_ct64_setkey_enc)
#define aes256_ct64_setkey_enc MQOM_NAMESPACE(aes256_ct64_setkey_enc)
#define rijndael256_ct64_setkey_enc MQOM_NAMESPACE(rijndael256_ct64_setkey_enc)
#define aes128_ct64_enc MQOM_NAMESPACE(aes128_ct64_enc)
#define aes256_ct64_enc MQOM_NAMESPACE(aes256_ct64_enc)
#define rijndael256_ct64_enc MQOM_NAMESPACE(rijndael256_ct64_enc)
#define aes128_ct64_enc_x2 MQOM_NAMESPACE(aes128_ct64_enc_x2)
#define aes128_ct64_enc_x4 MQOM_NAMESPACE(aes128_ct64_enc_x4)
#define aes128_ct64_enc_x8 MQOM_NAMESPACE(aes128_ct64_enc_x8)
#define aes256_ct64_enc_x2 MQOM_NAMESPACE(aes256_ct64_enc_x2)
#define aes256_ct64_enc_x4 MQOM_NAMESPACE(aes256_ct64_enc_x4)
#define aes256_ct64_enc_x8 MQOM_NAMESPACE(aes256_ct64_enc_x8)
#define rijndael256_ct64_enc_x2 MQOM_NAMESPACE(rijndael256_ct64_enc_x2)
#define rijndael256_ct64_enc_x4 MQOM_NAMESPACE(rijndael256_ct64_enc_x4)
#define rijndael256_ct64_enc_x8 MQOM_NAMESPACE(rijndael256_ct64_enc_x8)
/**/
#define aes128_ct64_setkey_enc_x2 MQOM_NAMESPACE(aes128_ct64_setkey_enc_x2)
#define aes256_ct64_setkey_enc_x2 MQOM_NAMESPACE(aes256_ct64_setkey_enc_x2)
#define rijndael256_ct64_setkey_enc_x2 MQOM_NAMESPACE(rijndael256_ct64_setkey_enc_x2)
#define aes128_ct64_setkey_enc_x4 MQOM_NAMESPACE(aes128_ct64_setkey_enc_x4)
#define aes256_ct64_setkey_enc_x4 MQOM_NAMESPACE(aes256_ct64_setkey_enc_x4)
#define rijndael256_ct64_setkey_enc_x4 MQOM_NAMESPACE(rijndael256_ct64_setkey_enc_x4)
#define aes128_ct64_setkey_enc_x8 MQOM_NAMESPACE(aes128_ct64_setkey_enc_x8)
#define aes256_ct64_setkey_enc_x8 MQOM_NAMESPACE(aes256_ct64_setkey_enc_x8)
#define rijndael256_ct64_setkey_enc_x8 MQOM_NAMESPACE(rijndael256_ct64_setkey_enc_x8)
#define aes128_ct64_enc_x2_x2 MQOM_NAMESPACE(aes128_ct64_enc_x2_x2)
#define aes128_ct64_enc_x4_x4 MQOM_NAMESPACE(aes128_ct64_enc_x4_x4)
#define aes128_ct64_enc_x8_x8 MQOM_NAMESPACE(aes128_ct64_enc_x8_x8)
#define aes256_ct64_enc_x2_x2 MQOM_NAMESPACE(aes256_ct64_enc_x2_x2)
#define aes256_ct64_enc_x4_x4 MQOM_NAMESPACE(aes256_ct64_enc_x4_x4)
#define aes256_ct64_enc_x8_x8 MQOM_NAMESPACE(aes256_ct64_enc_x8_x8)
#define rijndael256_ct64_enc_x2_x2 MQOM_NAMESPACE(rijndael256_ct64_enc_x2_x2)
#define rijndael256_ct64_enc_x4_x4 MQOM_NAMESPACE(rijndael256_ct64_enc_x4_x4)
#define rijndael256_ct64_enc_x8_x8 MQOM_NAMESPACE(rijndael256_ct64_enc_x8_x8)

/* The general Rijndael core context structure */
PACKED_BEGIN
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	ALIGN(4) uint32_t rk[44];  /* Round keys */
}
rijndael_ct64_ctx_aes128 PACKED_END;
PACKED_BEGIN
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	ALIGN(4) uint32_t rk[60];  /* Round keys */
} rijndael_ct64_ctx_aes256 PACKED_END;
PACKED_BEGIN
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	ALIGN(4) uint32_t rk[120];  /* Round keys */
} rijndael_ct64_ctx_rijndael256 PACKED_END;

/* x2, x4 and x8 contexts */
/*** AES-128 ***/
#if defined(RIJNDAEL_OPT_ARMV7M)
PACKED_BEGIN
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 4 bytes to avoid unaligned accesses */
	ALIGN(4) uint32_t interleaved_rkeys[2*44]; /* Bitsliced keys */
} rijndael_ct64_ctx_aes128_x2 PACKED_END;
typedef struct {
        rijndael_ct64_ctx_aes128_x2 ctx1;
        rijndael_ct64_ctx_aes128_x2 ctx2;
} rijndael_ct64_ctx_aes128_x4;
typedef struct {
        rijndael_ct64_ctx_aes128_x4 ctx1;
        rijndael_ct64_ctx_aes128_x4 ctx2;
} rijndael_ct64_ctx_aes128_x8;
#else
PACKED_BEGIN
typedef struct  {
	rijndael_type rtype; /* Type of Rijndael */
	uint32_t Nr; /* Number of rounds */
	uint32_t Nk; /* Number of words in the key */
	uint32_t Nb; /* Number of words in the block*/
	/* Round keys aligned on 8 bytes to avoid unaligned accesses */
	ALIGN(4) uint64_t interleaved_rkeys[2*44]; /* Bitsliced keys */
} rijndael_ct64_ctx_aes128_x2 PACKED_END;
typedef rijndael_ct64_ctx_aes128_x2 rijndael_ct64_ctx_aes128_x4;
typedef struct {
	rijndael_ct64_ctx_aes128_x4 ctx1;
	rijndael_ct64_ctx_aes128_x4 ctx2;
} rijndael_ct64_ctx_aes128_x8;
#endif
/*** AES-256 ***/
PACKED_BEGIN
typedef struct {
        rijndael_type rtype; /* Type of Rijndael */
        uint32_t Nr; /* Number of rounds */
        uint32_t Nk; /* Number of words in the key */
        uint32_t Nb; /* Number of words in the block*/
        /* Round keys aligned on 8 bytes to avoid unaligned accesses */
        ALIGN(8) uint64_t interleaved_rkeys[2*60]; /* Bitsliced keys */
} rijndael_ct64_ctx_aes256_x2 PACKED_END;
typedef rijndael_ct64_ctx_aes256_x2 rijndael_ct64_ctx_aes256_x4;
typedef struct {
	rijndael_ct64_ctx_aes256_x4 ctx1;
	rijndael_ct64_ctx_aes256_x4 ctx2;
} rijndael_ct64_ctx_aes256_x8;
/*** RIJNDAEL-256-256 ***/
PACKED_BEGIN
typedef struct {
        rijndael_type rtype; /* Type of Rijndael */
        uint32_t Nr; /* Number of rounds */
        uint32_t Nk; /* Number of words in the key */
        uint32_t Nb; /* Number of words in the block*/
        /* Round keys aligned on  bytes to avoid unaligned accesses */
        ALIGN(8) uint64_t interleaved_rkeys[120]; /* Bitsliced keys */
} rijndael_ct64_ctx_rijndael256_x2 PACKED_END;
typedef struct {
	rijndael_ct64_ctx_rijndael256_x2 ctx1;
	rijndael_ct64_ctx_rijndael256_x2 ctx2;
} rijndael_ct64_ctx_rijndael256_x4;
typedef struct {
	rijndael_ct64_ctx_rijndael256_x2 ctx1;
	rijndael_ct64_ctx_rijndael256_x2 ctx2;
	rijndael_ct64_ctx_rijndael256_x2 ctx3;
	rijndael_ct64_ctx_rijndael256_x2 ctx4;
} rijndael_ct64_ctx_rijndael256_x8;

/* ==== Public API ==== */
int aes128_ct64_setkey_enc(rijndael_ct64_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_ct64_setkey_enc(rijndael_ct64_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_ct64_setkey_enc(rijndael_ct64_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_ct64_enc(const rijndael_ct64_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_ct64_enc(const rijndael_ct64_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_ct64_enc(const rijndael_ct64_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2 and x4 encryption APIs */
int aes128_ct64_enc_x2(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_ct64_enc_x4(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const rijndael_ct64_ctx_aes128 *ctx3, const rijndael_ct64_ctx_aes128 *ctx4,
                       const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_ct64_enc_x8(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const rijndael_ct64_ctx_aes128 *ctx3, const rijndael_ct64_ctx_aes128 *ctx4,
                       const rijndael_ct64_ctx_aes128 *ctx5, const rijndael_ct64_ctx_aes128 *ctx6, const rijndael_ct64_ctx_aes128 *ctx7, const rijndael_ct64_ctx_aes128 *ctx8,
                       const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                       const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                       uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_ct64_enc_x2(const rijndael_ct64_ctx_aes256 *ctx1, const rijndael_ct64_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_ct64_enc_x4(const rijndael_ct64_ctx_aes256 *ctx1, const rijndael_ct64_ctx_aes256 *ctx2, const rijndael_ct64_ctx_aes256 *ctx3, const rijndael_ct64_ctx_aes256 *ctx4,
                       const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_ct64_enc_x8(const rijndael_ct64_ctx_aes256 *ctx1, const rijndael_ct64_ctx_aes256 *ctx2, const rijndael_ct64_ctx_aes256 *ctx3, const rijndael_ct64_ctx_aes256 *ctx4,
                       const rijndael_ct64_ctx_aes256 *ctx5, const rijndael_ct64_ctx_aes256 *ctx6, const rijndael_ct64_ctx_aes256 *ctx7, const rijndael_ct64_ctx_aes256 *ctx8,
                       const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                       const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                       uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int rijndael256_ct64_enc_x2(const rijndael_ct64_ctx_rijndael256 *ctx1, const rijndael_ct64_ctx_rijndael256 *ctx2,
                            const uint8_t plainText1[32], const uint8_t plainText2[32],
                            uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_ct64_enc_x4(const rijndael_ct64_ctx_rijndael256 *ctx1, const rijndael_ct64_ctx_rijndael256 *ctx2, const rijndael_ct64_ctx_rijndael256 *ctx3, const rijndael_ct64_ctx_rijndael256 *ctx4,
                            const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                            uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_ct64_enc_x8(const rijndael_ct64_ctx_rijndael256 *ctx1, const rijndael_ct64_ctx_rijndael256 *ctx2, const rijndael_ct64_ctx_rijndael256 *ctx3, const rijndael_ct64_ctx_rijndael256 *ctx4,
                            const rijndael_ct64_ctx_rijndael256 *ctx5, const rijndael_ct64_ctx_rijndael256 *ctx6, const rijndael_ct64_ctx_rijndael256 *ctx7, const rijndael_ct64_ctx_rijndael256 *ctx8,
                            const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                            const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                            uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                            uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);

MAKE_GENERIC_FUNCS_XX_DECL(aes128, ct64, 16, 16)
MAKE_GENERIC_FUNCS_XX_DECL(aes256, ct64, 32, 16)
MAKE_GENERIC_FUNCS_XX_DECL(rijndael256, ct64, 32, 32)

#endif /* __RIJNDAEL_CT64_H__ */
