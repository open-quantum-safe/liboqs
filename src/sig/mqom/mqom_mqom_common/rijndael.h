#ifndef __RIJNDAEL_H__
#define __RIJNDAEL_H__

#include "rijndael_platform.h"

#if defined(RIJNDAEL_CONSTANT_TIME_REF)
#if defined(RIJNDAEL_AES_NI) || defined(RIJNDAEL_BITSLICE) || defined(RIJNDAEL_TABLE) || defined(RIJNDAEL_EXTERNAL)
#error "RIJNDAEL_CONSTANT_TIME_REF, RIJNDAEL_TABLE, RIJNDAEL_BITSLICE, RIJNDAEL_EXTERNAL and RIJNDAEL_AES_NI are exclusive!"
#endif
#endif
#if defined(RIJNDAEL_AES_NI)
#if defined(RIJNDAEL_CONSTANT_TIME_REF) || defined(RIJNDAEL_BITSLICE) || defined(RIJNDAEL_TABLE) || defined(RIJNDAEL_EXTERNAL)
#error "RIJNDAEL_CONSTANT_TIME_REF, RIJNDAEL_TABLE, RIJNDAEL_BITSLICE, RIJNDAEL_EXTERNAL and RIJNDAEL_AES_NI are exclusive!"
#endif
#endif
#if defined(RIJNDAEL_BITSLICE)
#if defined(RIJNDAEL_AES_NI) || defined(RIJNDAEL_CONSTANT_TIME_REF) || defined(RIJNDAEL_TABLE) || defined(RIJNDAEL_EXTERNAL)
#error "RIJNDAEL_CONSTANT_TIME_REF, RIJNDAEL_TABLE, RIJNDAEL_BITSLICE, RIJNDAEL_EXTERNAL and RIJNDAEL_AES_NI are exclusive!"
#endif
#endif
#if defined(RIJNDAEL_TABLE)
#if defined(RIJNDAEL_AES_NI) || defined(RIJNDAEL_BITSLICE) || defined(RIJNDAEL_CONSTANT_TIME_REF) || defined(RIJNDAEL_EXTERNAL)
#error "RIJNDAEL_CONSTANT_TIME_REF, RIJNDAEL_TABLE, RIJNDAEL_BITSLICE, RIJNDAEL_EXTERNAL and RIJNDAEL_AES_NI are exclusive!"
#endif
#endif
#if defined(RIJNDAEL_EXTERNAL)
#if defined(RIJNDAEL_AES_NI) || defined(RIJNDAEL_BITSLICE) || defined(RIJNDAEL_CONSTANT_TIME_REF) || defined(RIJNDAEL_TABLE)
#error "RIJNDAEL_CONSTANT_TIME_REF, RIJNDAEL_TABLE, RIJNDAEL_BITSLICE, RIJNDAEL_EXTERNAL and RIJNDAEL_AES_NI are exclusive!"
#endif
#endif


/* === Constant time ref case */
#if defined(RIJNDAEL_CONSTANT_TIME_REF)
#include "rijndael_ref.h"
#define rijndael_ctx_aes128 rijndael_ref_ctx_aes128
#define rijndael_ctx_aes256 rijndael_ref_ctx_aes256
#define rijndael_ctx_rijndael256 rijndael_ref_ctx_rijndael256
#define aes128_setkey_enc aes128_ref_setkey_enc
#define aes256_setkey_enc aes256_ref_setkey_enc
#define rijndael256_setkey_enc rijndael256_ref_setkey_enc
#define aes128_enc aes128_ref_enc
#define aes256_enc aes256_ref_enc
#define rijndael256_enc rijndael256_ref_enc
#define aes128_enc_x2 aes128_ref_enc_x2
#define aes128_enc_x4 aes128_ref_enc_x4
#define aes128_enc_x8 aes128_ref_enc_x8
#define aes256_enc_x2 aes256_ref_enc_x2
#define aes256_enc_x4 aes256_ref_enc_x4
#define aes256_enc_x8 aes256_ref_enc_x8
#define rijndael256_enc_x2 rijndael256_ref_enc_x2
#define rijndael256_enc_x4 rijndael256_ref_enc_x4
#define rijndael256_enc_x8 rijndael256_ref_enc_x8
/**/
#define rijndael_ctx_aes128_x2 rijndael_ref_ctx_aes128_x2
#define rijndael_ctx_aes256_x2 rijndael_ref_ctx_aes256_x2
#define rijndael_ctx_rijndael256_x2 rijndael_ref_ctx_rijndael256_x2
#define aes128_setkey_enc_x2 aes128_ref_setkey_enc_x2
#define aes256_setkey_enc_x2 aes256_ref_setkey_enc_x2
#define rijndael256_setkey_enc_x2 rijndael256_ref_setkey_enc_x2
#define rijndael_ctx_aes128_x4 rijndael_ref_ctx_aes128_x4
#define rijndael_ctx_aes256_x4 rijndael_ref_ctx_aes256_x4
#define rijndael_ctx_rijndael256_x4 rijndael_ref_ctx_rijndael256_x4
#define aes128_setkey_enc_x4 aes128_ref_setkey_enc_x4
#define aes256_setkey_enc_x4 aes256_ref_setkey_enc_x4
#define rijndael256_setkey_enc_x4 rijndael256_ref_setkey_enc_x4
#define rijndael_ctx_aes128_x8 rijndael_ref_ctx_aes128_x8
#define rijndael_ctx_aes256_x8 rijndael_ref_ctx_aes256_x8
#define rijndael_ctx_rijndael256_x8 rijndael_ref_ctx_rijndael256_x8
#define aes128_setkey_enc_x8 aes128_ref_setkey_enc_x8
#define aes256_setkey_enc_x8 aes256_ref_setkey_enc_x8
#define rijndael256_setkey_enc_x8 rijndael256_ref_setkey_enc_x8
#define aes128_enc_x2_x2 aes128_ref_enc_x2_x2
#define aes128_enc_x4_x4 aes128_ref_enc_x4_x4
#define aes128_enc_x8_x8 aes128_ref_enc_x8_x8
#define aes256_enc_x2_x2 aes256_ref_enc_x2_x2
#define aes256_enc_x4_x4 aes256_ref_enc_x4_x4
#define aes256_enc_x8_x8 aes256_ref_enc_x8_x8
#define rijndael256_enc_x2_x2 rijndael256_ref_enc_x2_x2
#define rijndael256_enc_x4_x4 rijndael256_ref_enc_x4_x4
#define rijndael256_enc_x8_x8 rijndael256_ref_enc_x8_x8
static const char rijndael_conf[] = "Rijndael ref (constant time, slow)";
/**/
#if defined(RIJNDAEL_PRIV_PUB_COMMON)
#define rijndael_ctx_aes128_pub rijndael_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_setkey_enc
#define aes256_setkey_enc_pub aes256_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_setkey_enc
#define aes128_enc_pub aes128_enc
#define aes256_enc_pub aes256_enc
#define rijndael256_enc_pub rijndael256_enc
#define aes128_enc_x2_pub aes128_enc_x2
#define aes128_enc_x4_pub aes128_enc_x4
#define aes128_enc_x8_pub aes128_enc_x8
#define aes256_enc_x2_pub aes256_enc_x2
#define aes256_enc_x4_pub aes256_enc_x4
#define aes256_enc_x8_pub aes256_enc_x8
#define rijndael256_enc_x2_pub rijndael256_enc_x2
#define rijndael256_enc_x4_pub rijndael256_enc_x4
#define rijndael256_enc_x8_pub rijndael256_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub ref";
#else
#include "rijndael_table.h"
#define rijndael_ctx_aes128_pub rijndael_table_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_table_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_table_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_table_setkey_enc
#define aes256_setkey_enc_pub aes256_table_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_table_setkey_enc
#define aes128_enc_pub aes128_table_enc
#define aes256_enc_pub aes256_table_enc
#define rijndael256_enc_pub rijndael256_table_enc
#define aes128_enc_x2_pub aes128_table_enc_x2
#define aes128_enc_x4_pub aes128_table_enc_x4
#define aes128_enc_x8_pub aes128_table_enc_x8
#define aes256_enc_x2_pub aes256_table_enc_x2
#define aes256_enc_x4_pub aes256_table_enc_x4
#define aes256_enc_x8_pub aes256_table_enc_x8
#define rijndael256_enc_x2_pub rijndael256_table_enc_x2
#define rijndael256_enc_x4_pub rijndael256_table_enc_x4
#define rijndael256_enc_x8_pub rijndael256_table_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_table_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_table_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_table_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_table_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_table_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_table_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_table_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_table_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_table_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_table_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_table_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_table_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_table_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_table_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_table_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_table_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_table_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_table_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_table_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_table_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_table_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_table_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_table_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_table_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_table_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_table_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_table_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub table";
#endif
#endif

/* === Constant time bitslice case */
#if defined(RIJNDAEL_BITSLICE)
#include "rijndael_ct64.h"
#define rijndael_ctx_aes128 rijndael_ct64_ctx_aes128
#define rijndael_ctx_aes256 rijndael_ct64_ctx_aes256
#define rijndael_ctx_rijndael256 rijndael_ct64_ctx_rijndael256
#define aes128_setkey_enc aes128_ct64_setkey_enc
#define aes256_setkey_enc aes256_ct64_setkey_enc
#define rijndael256_setkey_enc rijndael256_ct64_setkey_enc
#define aes128_enc aes128_ct64_enc
#define aes256_enc aes256_ct64_enc
#define rijndael256_enc rijndael256_ct64_enc
#define aes128_enc_x2 aes128_ct64_enc_x2
#define aes128_enc_x4 aes128_ct64_enc_x4
#define aes128_enc_x8 aes128_ct64_enc_x8
#define aes256_enc_x2 aes256_ct64_enc_x2
#define aes256_enc_x4 aes256_ct64_enc_x4
#define aes256_enc_x8 aes256_ct64_enc_x8
#define rijndael256_enc_x2 rijndael256_ct64_enc_x2
#define rijndael256_enc_x4 rijndael256_ct64_enc_x4
#define rijndael256_enc_x8 rijndael256_ct64_enc_x8
/**/
#define rijndael_ctx_aes128_x2 rijndael_ct64_ctx_aes128_x2
#define rijndael_ctx_aes256_x2 rijndael_ct64_ctx_aes256_x2
#define rijndael_ctx_rijndael256_x2 rijndael_ct64_ctx_rijndael256_x2
#define aes128_setkey_enc_x2 aes128_ct64_setkey_enc_x2
#define aes256_setkey_enc_x2 aes256_ct64_setkey_enc_x2
#define rijndael256_setkey_enc_x2 rijndael256_ct64_setkey_enc_x2
#define rijndael_ctx_aes128_x4 rijndael_ct64_ctx_aes128_x4
#define rijndael_ctx_aes256_x4 rijndael_ct64_ctx_aes256_x4
#define rijndael_ctx_rijndael256_x4 rijndael_ct64_ctx_rijndael256_x4
#define aes128_setkey_enc_x4 aes128_ct64_setkey_enc_x4
#define aes256_setkey_enc_x4 aes256_ct64_setkey_enc_x4
#define rijndael256_setkey_enc_x4 rijndael256_ct64_setkey_enc_x4
#define rijndael_ctx_aes128_x8 rijndael_ct64_ctx_aes128_x8
#define rijndael_ctx_aes256_x8 rijndael_ct64_ctx_aes256_x8
#define rijndael_ctx_rijndael256_x8 rijndael_ct64_ctx_rijndael256_x8
#define aes128_setkey_enc_x8 aes128_ct64_setkey_enc_x8
#define aes256_setkey_enc_x8 aes256_ct64_setkey_enc_x8
#define rijndael256_setkey_enc_x8 rijndael256_ct64_setkey_enc_x8
#define aes128_enc_x2_x2 aes128_ct64_enc_x2_x2
#define aes128_enc_x4_x4 aes128_ct64_enc_x4_x4
#define aes128_enc_x8_x8 aes128_ct64_enc_x8_x8
#define aes256_enc_x2_x2 aes256_ct64_enc_x2_x2
#define aes256_enc_x4_x4 aes256_ct64_enc_x4_x4
#define aes256_enc_x8_x8 aes256_ct64_enc_x8_x8
#define rijndael256_enc_x2_x2 rijndael256_ct64_enc_x2_x2
#define rijndael256_enc_x4_x4 rijndael256_ct64_enc_x4_x4
#define rijndael256_enc_x8_x8 rijndael256_ct64_enc_x8_x8
#if defined(RIJNDAEL_OPT_ARMV7M)
static const char rijndael_conf[] = "Rijndael bitslice (constant time) (Variant ASM optimized for ARMv7M micro-architecture)";
#else
static const char rijndael_conf[] = "Rijndael bitslice (constant time) (pure C)";
#endif
/**/
#if defined(RIJNDAEL_PRIV_PUB_COMMON)
#define rijndael_ctx_aes128_pub rijndael_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_setkey_enc
#define aes256_setkey_enc_pub aes256_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_setkey_enc
#define aes128_enc_pub aes128_enc
#define aes256_enc_pub aes256_enc
#define rijndael256_enc_pub rijndael256_enc
#define aes128_enc_x2_pub aes128_enc_x2
#define aes128_enc_x4_pub aes128_enc_x4
#define aes128_enc_x8_pub aes128_enc_x8
#define aes256_enc_x2_pub aes256_enc_x2
#define aes256_enc_x4_pub aes256_enc_x4
#define aes256_enc_x8_pub aes256_enc_x8
#define rijndael256_enc_x2_pub rijndael256_enc_x2
#define rijndael256_enc_x4_pub rijndael256_enc_x4
#define rijndael256_enc_x8_pub rijndael256_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub bitslice";
#else
#include "rijndael_table.h"
#define rijndael_ctx_aes128_pub rijndael_table_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_table_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_table_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_table_setkey_enc
#define aes256_setkey_enc_pub aes256_table_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_table_setkey_enc
#define aes128_enc_pub aes128_table_enc
#define aes256_enc_pub aes256_table_enc
#define rijndael256_enc_pub rijndael256_table_enc
#define aes128_enc_x2_pub aes128_table_enc_x2
#define aes128_enc_x4_pub aes128_table_enc_x4
#define aes128_enc_x8_pub aes128_table_enc_x8
#define aes256_enc_x2_pub aes256_table_enc_x2
#define aes256_enc_x4_pub aes256_table_enc_x4
#define aes256_enc_x8_pub aes256_table_enc_x8
#define rijndael256_enc_x2_pub rijndael256_table_enc_x2
#define rijndael256_enc_x4_pub rijndael256_table_enc_x4
#define rijndael256_enc_x8_pub rijndael256_table_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_table_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_table_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_table_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_table_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_table_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_table_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_table_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_table_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_table_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_table_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_table_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_table_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_table_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_table_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_table_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_table_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_table_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_table_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_table_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_table_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_table_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_table_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_table_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_table_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_table_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_table_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_table_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub table";
#endif
#endif

/* === Non-constant time table case */
#if defined(RIJNDAEL_TABLE)
#include "rijndael_table.h"
#define rijndael_ctx_aes128 rijndael_table_ctx_aes128
#define rijndael_ctx_aes256 rijndael_table_ctx_aes256
#define rijndael_ctx_rijndael256 rijndael_table_ctx_rijndael256
#define aes128_setkey_enc aes128_table_setkey_enc
#define aes256_setkey_enc aes256_table_setkey_enc
#define rijndael256_setkey_enc rijndael256_table_setkey_enc
#define aes128_enc aes128_table_enc
#define aes256_enc aes256_table_enc
#define rijndael256_enc rijndael256_table_enc
#define aes128_enc_x2 aes128_table_enc_x2
#define aes128_enc_x4 aes128_table_enc_x4
#define aes128_enc_x8 aes128_table_enc_x8
#define aes256_enc_x2 aes256_table_enc_x2
#define aes256_enc_x4 aes256_table_enc_x4
#define aes256_enc_x8 aes256_table_enc_x8
#define rijndael256_enc_x2 rijndael256_table_enc_x2
#define rijndael256_enc_x4 rijndael256_table_enc_x4
#define rijndael256_enc_x8 rijndael256_table_enc_x8
/**/
#define rijndael_ctx_aes128_x2 rijndael_table_ctx_aes128_x2
#define rijndael_ctx_aes256_x2 rijndael_table_ctx_aes256_x2
#define rijndael_ctx_rijndael256_x2 rijndael_table_ctx_rijndael256_x2
#define aes128_setkey_enc_x2 aes128_table_setkey_enc_x2
#define aes256_setkey_enc_x2 aes256_table_setkey_enc_x2
#define rijndael256_setkey_enc_x2 rijndael256_table_setkey_enc_x2
#define rijndael_ctx_aes128_x4 rijndael_table_ctx_aes128_x4
#define rijndael_ctx_aes256_x4 rijndael_table_ctx_aes256_x4
#define rijndael_ctx_rijndael256_x4 rijndael_table_ctx_rijndael256_x4
#define aes128_setkey_enc_x4 aes128_table_setkey_enc_x4
#define aes256_setkey_enc_x4 aes256_table_setkey_enc_x4
#define rijndael256_setkey_enc_x4 rijndael256_table_setkey_enc_x4
#define rijndael_ctx_aes128_x8 rijndael_table_ctx_aes128_x8
#define rijndael_ctx_aes256_x8 rijndael_table_ctx_aes256_x8
#define rijndael_ctx_rijndael256_x8 rijndael_table_ctx_rijndael256_x8
#define aes128_setkey_enc_x8 aes128_table_setkey_enc_x8
#define aes256_setkey_enc_x8 aes256_table_setkey_enc_x8
#define rijndael256_setkey_enc_x8 rijndael256_table_setkey_enc_x8
#define aes128_enc_x2_x2 aes128_table_enc_x2_x2
#define aes128_enc_x4_x4 aes128_table_enc_x4_x4
#define aes128_enc_x8_x8 aes128_table_enc_x8_x8
#define aes256_enc_x2_x2 aes256_table_enc_x2_x2
#define aes256_enc_x4_x4 aes256_table_enc_x4_x4
#define aes256_enc_x8_x8 aes256_table_enc_x8_x8
#define rijndael256_enc_x2_x2 rijndael256_table_enc_x2_x2
#define rijndael256_enc_x4_x4 rijndael256_table_enc_x4_x4
#define rijndael256_enc_x8_x8 rijndael256_table_enc_x8_x8
#if defined(RIJNDAEL_OPT_ARMV7M)
static const char rijndael_conf[] = "Rijndael table (usually NON constant time, constant time on embedded platforms without cache to SRAM) (Variant ASM optimized for ARMv7M micro-architecture)";
#else
static const char rijndael_conf[] = "Rijndael table (usually NON constant time, constant time on embedded platforms without cache to SRAM) (pure C)";
#endif
/**/
#define rijndael_ctx_aes128_pub rijndael_table_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_table_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_table_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_table_setkey_enc
#define aes256_setkey_enc_pub aes256_table_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_table_setkey_enc
#define aes128_enc_pub aes128_table_enc
#define aes256_enc_pub aes256_table_enc
#define rijndael256_enc_pub rijndael256_table_enc
#define aes128_enc_x2_pub aes128_table_enc_x2
#define aes128_enc_x4_pub aes128_table_enc_x4
#define aes128_enc_x8_pub aes128_table_enc_x8
#define aes256_enc_x2_pub aes256_table_enc_x2
#define aes256_enc_x4_pub aes256_table_enc_x4
#define aes256_enc_x8_pub aes256_table_enc_x8
#define rijndael256_enc_x2_pub rijndael256_table_enc_x2
#define rijndael256_enc_x4_pub rijndael256_table_enc_x4
#define rijndael256_enc_x8_pub rijndael256_table_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_table_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_table_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_table_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_table_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_table_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_table_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_table_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_table_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_table_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_table_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_table_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_table_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_table_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_table_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_table_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_table_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_table_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_table_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_table_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_table_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_table_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_table_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_table_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_table_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_table_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_table_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_table_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub table";
#endif

/* === Constant time AES-NI case (supposes AES-NI x86 support) */
#if defined(RIJNDAEL_AES_NI)
#include "rijndael_aes_ni.h"
#define rijndael_ctx_aes128 rijndael_aes_ni_ctx_aes128
#define rijndael_ctx_aes256 rijndael_aes_ni_ctx_aes256
#define rijndael_ctx_rijndael256 rijndael_aes_ni_ctx_rijndael256
#define aes128_setkey_enc aes128_aes_ni_setkey_enc
#define aes256_setkey_enc aes256_aes_ni_setkey_enc
#define rijndael256_setkey_enc rijndael256_aes_ni_setkey_enc
#define aes128_enc aes128_aes_ni_enc
#define aes256_enc aes256_aes_ni_enc
#define rijndael256_enc rijndael256_aes_ni_enc
#define aes128_enc_x2 aes128_aes_ni_enc_x2
#define aes128_enc_x4 aes128_aes_ni_enc_x4
#define aes128_enc_x8 aes128_aes_ni_enc_x8
#define aes256_enc_x2 aes256_aes_ni_enc_x2
#define aes256_enc_x4 aes256_aes_ni_enc_x4
#define aes256_enc_x8 aes256_aes_ni_enc_x8
#define rijndael256_enc_x2 rijndael256_aes_ni_enc_x2
#define rijndael256_enc_x4 rijndael256_aes_ni_enc_x4
#define rijndael256_enc_x8 rijndael256_aes_ni_enc_x8
/**/
#define rijndael_ctx_aes128_x2 rijndael_aes_ni_ctx_aes128_x2
#define rijndael_ctx_aes256_x2 rijndael_aes_ni_ctx_aes256_x2
#define rijndael_ctx_rijndael256_x2 rijndael_aes_ni_ctx_rijndael256_x2
#define aes128_setkey_enc_x2 aes128_aes_ni_setkey_enc_x2
#define aes256_setkey_enc_x2 aes256_aes_ni_setkey_enc_x2
#define rijndael256_setkey_enc_x2 rijndael256_aes_ni_setkey_enc_x2
#define rijndael_ctx_aes128_x4 rijndael_aes_ni_ctx_aes128_x4
#define rijndael_ctx_aes256_x4 rijndael_aes_ni_ctx_aes256_x4
#define rijndael_ctx_rijndael256_x4 rijndael_aes_ni_ctx_rijndael256_x4
#define aes128_setkey_enc_x4 aes128_aes_ni_setkey_enc_x4
#define aes256_setkey_enc_x4 aes256_aes_ni_setkey_enc_x4
#define rijndael256_setkey_enc_x4 rijndael256_aes_ni_setkey_enc_x4
#define rijndael_ctx_aes128_x8 rijndael_aes_ni_ctx_aes128_x8
#define rijndael_ctx_aes256_x8 rijndael_aes_ni_ctx_aes256_x8
#define rijndael_ctx_rijndael256_x8 rijndael_aes_ni_ctx_rijndael256_x8
#define aes128_setkey_enc_x8 aes128_aes_ni_setkey_enc_x8
#define aes256_setkey_enc_x8 aes256_aes_ni_setkey_enc_x8
#define rijndael256_setkey_enc_x8 rijndael256_aes_ni_setkey_enc_x8
#define aes128_enc_x2_x2 aes128_aes_ni_enc_x2_x2
#define aes128_enc_x4_x4 aes128_aes_ni_enc_x4_x4
#define aes128_enc_x8_x8 aes128_aes_ni_enc_x8_x8
#define aes256_enc_x2_x2 aes256_aes_ni_enc_x2_x2
#define aes256_enc_x4_x4 aes256_aes_ni_enc_x4_x4
#define aes256_enc_x8_x8 aes256_aes_ni_enc_x8_x8
#define rijndael256_enc_x2_x2 rijndael256_aes_ni_enc_x2_x2
#define rijndael256_enc_x4_x4 rijndael256_aes_ni_enc_x4_x4
#define rijndael256_enc_x8_x8 rijndael256_aes_ni_enc_x8_x8
static const char rijndael_conf[] = "Rijndael AES-NI (constant time, x86 dedicated)";
/**/
#define rijndael_ctx_aes128_pub rijndael_aes_ni_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_aes_ni_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_aes_ni_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_aes_ni_setkey_enc
#define aes256_setkey_enc_pub aes256_aes_ni_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_aes_ni_setkey_enc
#define aes128_enc_pub aes128_aes_ni_enc
#define aes256_enc_pub aes256_aes_ni_enc
#define rijndael256_enc_pub rijndael256_aes_ni_enc
#define aes128_enc_x2_pub aes128_aes_ni_enc_x2
#define aes128_enc_x4_pub aes128_aes_ni_enc_x4
#define aes128_enc_x8_pub aes128_aes_ni_enc_x8
#define aes256_enc_x2_pub aes256_aes_ni_enc_x2
#define aes256_enc_x4_pub aes256_aes_ni_enc_x4
#define aes256_enc_x8_pub aes256_aes_ni_enc_x8
#define rijndael256_enc_x2_pub rijndael256_aes_ni_enc_x2
#define rijndael256_enc_x4_pub rijndael256_aes_ni_enc_x4
#define rijndael256_enc_x8_pub rijndael256_aes_ni_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub aes_ni";
#endif

/* === External Rijndael case */
#if defined(RIJNDAEL_EXTERNAL)
#include "rijndael_external.h"
#define rijndael_ctx_aes128 rijndael_external_ctx_aes128
#define rijndael_ctx_aes256 rijndael_external_ctx_aes256
#define rijndael_ctx_rijndael256 rijndael_external_ctx_rijndael256
#define aes128_setkey_enc aes128_external_setkey_enc
#define aes256_setkey_enc aes256_external_setkey_enc
#define rijndael256_setkey_enc rijndael256_external_setkey_enc
#define aes128_enc aes128_external_enc
#define aes256_enc aes256_external_enc
#define rijndael256_enc rijndael256_external_enc
#define aes128_enc_x2 aes128_external_enc_x2
#define aes128_enc_x4 aes128_external_enc_x4
#define aes128_enc_x8 aes128_external_enc_x8
#define aes256_enc_x2 aes256_external_enc_x2
#define aes256_enc_x4 aes256_external_enc_x4
#define aes256_enc_x8 aes256_external_enc_x8
#define rijndael256_enc_x2 rijndael256_external_enc_x2
#define rijndael256_enc_x4 rijndael256_external_enc_x4
#define rijndael256_enc_x8 rijndael256_external_enc_x8
/**/
#define rijndael_ctx_aes128_x2 rijndael_external_ctx_aes128_x2
#define rijndael_ctx_aes256_x2 rijndael_external_ctx_aes256_x2
#define rijndael_ctx_rijndael256_x2 rijndael_external_ctx_rijndael256_x2
#define aes128_setkey_enc_x2 aes128_external_setkey_enc_x2
#define aes256_setkey_enc_x2 aes256_external_setkey_enc_x2
#define rijndael256_setkey_enc_x2 rijndael256_external_setkey_enc_x2
#define rijndael_ctx_aes128_x4 rijndael_external_ctx_aes128_x4
#define rijndael_ctx_aes256_x4 rijndael_external_ctx_aes256_x4
#define rijndael_ctx_rijndael256_x4 rijndael_external_ctx_rijndael256_x4
#define aes128_setkey_enc_x4 aes128_external_setkey_enc_x4
#define aes256_setkey_enc_x4 aes256_external_setkey_enc_x4
#define rijndael256_setkey_enc_x4 rijndael256_external_setkey_enc_x4
#define rijndael_ctx_aes128_x8 rijndael_external_ctx_aes128_x8
#define rijndael_ctx_aes256_x8 rijndael_external_ctx_aes256_x8
#define rijndael_ctx_rijndael256_x8 rijndael_external_ctx_rijndael256_x8
#define aes128_setkey_enc_x8 aes128_external_setkey_enc_x8
#define aes256_setkey_enc_x8 aes256_external_setkey_enc_x8
#define rijndael256_setkey_enc_x8 rijndael256_external_setkey_enc_x8
#define aes128_enc_x2_x2 aes128_external_enc_x2_x2
#define aes128_enc_x4_x4 aes128_external_enc_x4_x4
#define aes128_enc_x8_x8 aes128_external_enc_x8_x8
#define aes256_enc_x2_x2 aes256_external_enc_x2_x2
#define aes256_enc_x4_x4 aes256_external_enc_x4_x4
#define aes256_enc_x8_x8 aes256_external_enc_x8_x8
#define rijndael256_enc_x2_x2 rijndael256_external_enc_x2_x2
#define rijndael256_enc_x4_x4 rijndael256_external_enc_x4_x4
#define rijndael256_enc_x8_x8 rijndael256_external_enc_x8_x8
static const char rijndael_conf[] = "Rijndael external (user provided)";
/**/
#define rijndael_ctx_aes128_pub rijndael_ctx_aes128
#define rijndael_ctx_aes256_pub rijndael_ctx_aes256
#define rijndael_ctx_rijndael256_pub rijndael_ctx_rijndael256
#define aes128_setkey_enc_pub aes128_setkey_enc
#define aes256_setkey_enc_pub aes256_setkey_enc
#define rijndael256_setkey_enc_pub rijndael256_setkey_enc
#define aes128_enc_pub aes128_enc
#define aes256_enc_pub aes256_enc
#define rijndael256_enc_pub rijndael256_enc
#define aes128_enc_x2_pub aes128_enc_x2
#define aes128_enc_x4_pub aes128_enc_x4
#define aes128_enc_x8_pub aes128_enc_x8
#define aes256_enc_x2_pub aes256_enc_x2
#define aes256_enc_x4_pub aes256_enc_x4
#define aes256_enc_x8_pub aes256_enc_x8
#define rijndael256_enc_x2_pub rijndael256_enc_x2
#define rijndael256_enc_x4_pub rijndael256_enc_x4
#define rijndael256_enc_x8_pub rijndael256_enc_x8
/**/
#define rijndael_ctx_aes128_pub_x2 rijndael_ctx_aes128_x2
#define rijndael_ctx_aes256_pub_x2 rijndael_ctx_aes256_x2
#define rijndael_ctx_rijndael256_pub_x2 rijndael_ctx_rijndael256_x2
#define rijndael_ctx_aes128_pub_x4 rijndael_ctx_aes128_x4
#define rijndael_ctx_aes256_pub_x4 rijndael_ctx_aes256_x4
#define rijndael_ctx_rijndael256_pub_x4 rijndael_ctx_rijndael256_x4
#define rijndael_ctx_aes128_pub_x8 rijndael_ctx_aes128_x8
#define rijndael_ctx_aes256_pub_x8 rijndael_ctx_aes256_x8
#define rijndael_ctx_rijndael256_pub_x8 rijndael_ctx_rijndael256_x8
#define aes128_setkey_enc_pub_x2 aes128_setkey_enc_x2
#define aes256_setkey_enc_pub_x2 aes256_setkey_enc_x2
#define rijndael256_setkey_enc_pub_x2 rijndael256_setkey_enc_x2
#define aes128_setkey_enc_pub_x4 aes128_setkey_enc_x4
#define aes256_setkey_enc_pub_x4 aes256_setkey_enc_x4
#define rijndael256_setkey_enc_pub_x4 rijndael256_setkey_enc_x4
#define aes128_setkey_enc_pub_x8 aes128_setkey_enc_x8
#define aes256_setkey_enc_pub_x8 aes256_setkey_enc_x8
#define rijndael256_setkey_enc_pub_x8 rijndael256_setkey_enc_x8
#define aes128_enc_x2_pub_x2 aes128_enc_x2_x2
#define aes128_enc_x4_pub_x4 aes128_enc_x4_x4
#define aes128_enc_x8_pub_x8 aes128_enc_x8_x8
#define aes256_enc_x2_pub_x2 aes256_enc_x2_x2
#define aes256_enc_x4_pub_x4 aes256_enc_x4_x4
#define aes256_enc_x8_pub_x8 aes256_enc_x8_x4
#define rijndael256_enc_x2_pub_x2 rijndael256_enc_x2_x2
#define rijndael256_enc_x4_pub_x4 rijndael256_enc_x4_x4
#define rijndael256_enc_x8_pub_x8 rijndael256_enc_x8_x8
static const char rijndael_conf_pub[] = "Rijndael pub external (user provided)";
#endif

/* ==== Public API ==== */

/** API for the private data */
int aes128_setkey_enc(rijndael_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_setkey_enc(rijndael_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_setkey_enc(rijndael_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_enc(const rijndael_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_enc(const rijndael_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_enc(const rijndael_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2, x4 and x8 encryption APIs */
int aes128_setkey_enc_x2(rijndael_ctx_aes128_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]);
int aes256_setkey_enc_x2(rijndael_ctx_aes256_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]);
int rijndael256_setkey_enc_x2(rijndael_ctx_rijndael256_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]);
int aes128_setkey_enc_x4(rijndael_ctx_aes128_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]);
int aes256_setkey_enc_x4(rijndael_ctx_aes256_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]);
int rijndael256_setkey_enc_x4(rijndael_ctx_rijndael256_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]);
int aes128_setkey_enc_x8(rijndael_ctx_aes128_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]);
int aes256_setkey_enc_x8(rijndael_ctx_aes256_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32], const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]);
int rijndael256_setkey_enc_x8(rijndael_ctx_rijndael256_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32], const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]);
/* == */
int aes128_enc_x2(const rijndael_ctx_aes128 *ctx1, const rijndael_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_enc_x4(const rijndael_ctx_aes128 *ctx1, const rijndael_ctx_aes128 *ctx2, const rijndael_ctx_aes128 *ctx3, const rijndael_ctx_aes128 *ctx4,
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_enc_x8(const rijndael_ctx_aes128 *ctx1, const rijndael_ctx_aes128 *ctx2, const rijndael_ctx_aes128 *ctx3, const rijndael_ctx_aes128 *ctx4,
                  const rijndael_ctx_aes128 *ctx5, const rijndael_ctx_aes128 *ctx6, const rijndael_ctx_aes128 *ctx7, const rijndael_ctx_aes128 *ctx8,
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                  uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes128_enc_x2_x2(const rijndael_ctx_aes128_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_enc_x4_x4(const rijndael_ctx_aes128_x4 *ctx,
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_enc_x8_x8(const rijndael_ctx_aes128_x8 *ctx, 
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                  uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);

/* == */
int aes256_enc_x2(const rijndael_ctx_aes256 *ctx1, const rijndael_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_enc_x4(const rijndael_ctx_aes256 *ctx1, const rijndael_ctx_aes256 *ctx2, const rijndael_ctx_aes256 *ctx3, const rijndael_ctx_aes256 *ctx4,
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_enc_x8(const rijndael_ctx_aes256 *ctx1, const rijndael_ctx_aes256 *ctx2, const rijndael_ctx_aes256 *ctx3, const rijndael_ctx_aes256 *ctx4,
                  const rijndael_ctx_aes256 *ctx5, const rijndael_ctx_aes256 *ctx6, const rijndael_ctx_aes256 *ctx7, const rijndael_ctx_aes256 *ctx8,
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                  uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_enc_x2_x2(const rijndael_ctx_aes256_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_enc_x4_x4(const rijndael_ctx_aes256_x4 *ctx,
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_enc_x8_x8(const rijndael_ctx_aes256_x8 *ctx, 
                  const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                  const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                  uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                  uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);

/* == */
int rijndael256_enc_x2(const rijndael_ctx_rijndael256 *ctx1, const rijndael_ctx_rijndael256 *ctx2,
                       const uint8_t plainText1[32], const uint8_t plainText2[32],
                       uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_enc_x4(const rijndael_ctx_rijndael256 *ctx1, const rijndael_ctx_rijndael256 *ctx2, const rijndael_ctx_rijndael256 *ctx3, const rijndael_ctx_rijndael256 *ctx4,
                       const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                       uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_enc_x8(const rijndael_ctx_rijndael256 *ctx1, const rijndael_ctx_rijndael256 *ctx2, const rijndael_ctx_rijndael256 *ctx3, const rijndael_ctx_rijndael256 *ctx4,
                       const rijndael_ctx_rijndael256 *ctx5, const rijndael_ctx_rijndael256 *ctx6, const rijndael_ctx_rijndael256 *ctx7, const rijndael_ctx_rijndael256 *ctx8,
                       const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                       const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                       uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                       uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);
int rijndael256_enc_x2_x2(const rijndael_ctx_rijndael256_x2 *ctx, const uint8_t plainText1[32], const uint8_t plainText2[32], uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_enc_x4_x4(const rijndael_ctx_rijndael256_x4 *ctx,
                  const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                  uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_enc_x8_x8(const rijndael_ctx_rijndael256_x8 *ctx, 
                  const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                  const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                  uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                  uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);

/** API for the public data */
int aes128_setkey_enc_pub(rijndael_ctx_aes128_pub *ctx, const uint8_t key[16]);
int aes256_setkey_enc_pub(rijndael_ctx_aes256_pub *ctx, const uint8_t key[32]);
int rijndael256_setkey_enc_pub(rijndael_ctx_rijndael256_pub *ctx, const uint8_t key[32]);
int aes128_enc_pub(const rijndael_ctx_aes128_pub *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_enc_pub(const rijndael_ctx_aes256_pub *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_enc_pub(const rijndael_ctx_rijndael256_pub *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2, x4 and x8 encryption APIs */
int aes128_setkey_enc_pub_x2(rijndael_ctx_aes128_pub_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]);
int aes256_setkey_enc_pub_x2(rijndael_ctx_aes256_pub_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]);
int rijndael256_setkey_enc_pub_x2(rijndael_ctx_rijndael256_pub_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]);
int aes128_setkey_enc_pub_x4(rijndael_ctx_aes128_pub_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]);
int aes256_setkey_enc_pub_x4(rijndael_ctx_aes256_pub_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]);
int rijndael256_setkey_enc_pub_x4(rijndael_ctx_rijndael256_pub_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]);
int aes128_setkey_enc_pub_x8(rijndael_ctx_aes128_pub_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]);
int aes256_setkey_enc_pub_x8(rijndael_ctx_aes256_pub_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32], const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]);
int rijndael256_setkey_enc_pub_x8(rijndael_ctx_rijndael256_pub_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32], const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]);
/* == */
int aes128_enc_x2_pub(const rijndael_ctx_aes128_pub *ctx1, const rijndael_ctx_aes128_pub *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_enc_x4_pub(const rijndael_ctx_aes128_pub *ctx1, const rijndael_ctx_aes128_pub *ctx2, const rijndael_ctx_aes128_pub *ctx3, const rijndael_ctx_aes128_pub *ctx4,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_enc_x8_pub(const rijndael_ctx_aes128_pub *ctx1, const rijndael_ctx_aes128_pub *ctx2, const rijndael_ctx_aes128_pub *ctx3, const rijndael_ctx_aes128_pub *ctx4,
                      const rijndael_ctx_aes128_pub *ctx5, const rijndael_ctx_aes128_pub *ctx6, const rijndael_ctx_aes128_pub *ctx7, const rijndael_ctx_aes128_pub *ctx8,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                      uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes128_enc_x2_pub_x2(const rijndael_ctx_aes128_pub_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_enc_x4_pub_x4(const rijndael_ctx_aes128_pub_x4 *ctx,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_enc_x8_pub_x8(const rijndael_ctx_aes128_pub_x8 *ctx,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                      uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
/* == */
int aes256_enc_x2_pub(const rijndael_ctx_aes256_pub *ctx1, const rijndael_ctx_aes256_pub *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_enc_x4_pub(const rijndael_ctx_aes256_pub *ctx1, const rijndael_ctx_aes256_pub *ctx2, const rijndael_ctx_aes256_pub *ctx3, const rijndael_ctx_aes256_pub *ctx4,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_enc_x8_pub(const rijndael_ctx_aes256_pub *ctx1, const rijndael_ctx_aes256_pub *ctx2, const rijndael_ctx_aes256_pub *ctx3, const rijndael_ctx_aes256_pub *ctx4,
                      const rijndael_ctx_aes256_pub *ctx5, const rijndael_ctx_aes256_pub *ctx6, const rijndael_ctx_aes256_pub *ctx7, const rijndael_ctx_aes256_pub *ctx8,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                      uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_enc_x2_pub_x2(const rijndael_ctx_aes256_pub_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_enc_x4_pub_x4(const rijndael_ctx_aes256_pub_x4 *ctx,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_enc_x8_pub_x8(const rijndael_ctx_aes256_pub_x8 *ctx,
                      const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                      const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                      uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                      uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
/* == */
int rijndael256_enc_x2_pub(const rijndael_ctx_rijndael256_pub *ctx1, const rijndael_ctx_rijndael256_pub *ctx2,
                           const uint8_t plainText1[32], const uint8_t plainText2[32],
                           uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_enc_x4_pub(const rijndael_ctx_rijndael256_pub *ctx1, const rijndael_ctx_rijndael256_pub *ctx2, const rijndael_ctx_rijndael256_pub *ctx3, const rijndael_ctx_rijndael256_pub *ctx4,
                           const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                           uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_enc_x8_pub(const rijndael_ctx_rijndael256_pub *ctx1, const rijndael_ctx_rijndael256_pub *ctx2, const rijndael_ctx_rijndael256_pub *ctx3, const rijndael_ctx_rijndael256_pub *ctx4,
                           const rijndael_ctx_rijndael256_pub *ctx5, const rijndael_ctx_rijndael256_pub *ctx6, const rijndael_ctx_rijndael256_pub *ctx7, const rijndael_ctx_rijndael256_pub *ctx8,
                           const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                           const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                           uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                           uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);
int rijndael256_enc_x2_pub_x2(const rijndael_ctx_rijndael256_pub_x2 *ctx, const uint8_t plainText1[32], const uint8_t plainText2[32], uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_enc_x4_pub_x4(const rijndael_ctx_rijndael256_pub_x4 *ctx,
                      const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                      uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_enc_x8_pub_x8(const rijndael_ctx_rijndael256_pub_x8 *ctx,
                      const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                      const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                      uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                      uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);

#endif /* __RIJNDAEL_H__ */
