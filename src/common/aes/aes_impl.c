// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "aes.h"
#include "aes_local.h"

/*
 * The AES backend (portable C, x86-64 AES-NI, ARMv8 crypto extensions) is
 * selected once, either at compile time or, for dist builds, in OQS_AES_init()
 * after CPU feature detection. The result is stored in aes_default_callbacks,
 * so no per-call dispatch is needed.
 */
#if defined(OQS_DIST_X86_64_BUILD)
#define OQS_AES_HAVE_C 1
#define OQS_AES_HAVE_NI 1
#define OQS_AES_HAVE_ARM 0
#elif defined(OQS_DIST_ARM64_V8_BUILD)
#define OQS_AES_HAVE_C 1
#define OQS_AES_HAVE_NI 0
#define OQS_AES_HAVE_ARM 1
#elif defined(OQS_USE_AES_INSTRUCTIONS)
#define OQS_AES_HAVE_C 0
#define OQS_AES_HAVE_NI 1
#define OQS_AES_HAVE_ARM 0
#elif defined(OQS_USE_ARM_AES_INSTRUCTIONS)
#define OQS_AES_HAVE_C 0
#define OQS_AES_HAVE_NI 0
#define OQS_AES_HAVE_ARM 1
#else
#define OQS_AES_HAVE_C 1
#define OQS_AES_HAVE_NI 0
#define OQS_AES_HAVE_ARM 0
#endif

#define OQS_AES_DEFINE_ECB_ENC(name, load, enc_sch, free_sch) \
    static void name(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) { \
        void *schedule = NULL; \
        load(key, &schedule); \
        enc_sch(plaintext, plaintext_len, schedule, ciphertext); \
        free_sch(schedule); \
    }

#if OQS_AES_HAVE_C
OQS_AES_DEFINE_ECB_ENC(aes128_ecb_enc_c, oqs_aes128_load_schedule_c, oqs_aes128_ecb_enc_sch_c, oqs_aes128_free_schedule_c)
OQS_AES_DEFINE_ECB_ENC(aes256_ecb_enc_c, oqs_aes256_load_schedule_c, oqs_aes256_ecb_enc_sch_c, oqs_aes256_free_schedule_c)
OQS_AES_DEFINE_ECB_ENC(aes192_ecb_enc_c, oqs_aes192_load_schedule_c, oqs_aes192_ecb_enc_sch_c, oqs_aes192_free_schedule_c)

#define OQS_AES_C_CALLBACKS { \
    .AES128_ECB_load_schedule = oqs_aes128_load_schedule_c, \
    .AES128_CTR_inc_init = oqs_aes128_load_schedule_c, \
    .AES128_CTR_inc_iv = oqs_aes128_load_iv_c, \
    .AES128_CTR_inc_ivu64 = oqs_aes128_load_iv_u64_c, \
    .AES128_free_schedule = oqs_aes128_free_schedule_c, \
    .AES128_ECB_enc = aes128_ecb_enc_c, \
    .AES128_ECB_enc_sch = oqs_aes128_ecb_enc_sch_c, \
    .AES128_CTR_inc_stream_iv = oqs_aes128_ctr_enc_sch_c, \
    .AES256_ECB_load_schedule = oqs_aes256_load_schedule_c, \
    .AES256_CTR_inc_init = oqs_aes256_load_schedule_c, \
    .AES256_CTR_inc_iv = oqs_aes256_load_iv_c, \
    .AES256_CTR_inc_ivu64 = oqs_aes256_load_iv_u64_c, \
    .AES256_free_schedule = oqs_aes256_free_schedule_c, \
    .AES256_ECB_enc = aes256_ecb_enc_c, \
    .AES256_ECB_enc_sch = oqs_aes256_ecb_enc_sch_c, \
    .AES256_CTR_inc_stream_iv = oqs_aes256_ctr_enc_sch_c, \
    .AES256_CTR_inc_stream_blks = oqs_aes256_ctr_enc_sch_upd_blks_c, \
    .AES192_ECB_load_schedule = oqs_aes192_load_schedule_c, \
    .AES192_free_schedule = oqs_aes192_free_schedule_c, \
    .AES192_ECB_enc = aes192_ecb_enc_c, \
    .AES192_ECB_enc_sch = oqs_aes192_ecb_enc_sch_c, \
}
#endif

#if OQS_AES_HAVE_NI
OQS_AES_DEFINE_ECB_ENC(aes128_ecb_enc_ni, oqs_aes128_load_schedule_ni, oqs_aes128_ecb_enc_sch_ni, oqs_aes128_free_schedule_ni)
OQS_AES_DEFINE_ECB_ENC(aes256_ecb_enc_ni, oqs_aes256_load_schedule_ni, oqs_aes256_ecb_enc_sch_ni, oqs_aes256_free_schedule_ni)
OQS_AES_DEFINE_ECB_ENC(aes192_ecb_enc_ni, oqs_aes192_load_schedule_ni, oqs_aes192_ecb_enc_sch_ni, oqs_aes192_free_schedule_ni)

#define OQS_AES_NI_CALLBACKS { \
    .AES128_ECB_load_schedule = oqs_aes128_load_schedule_ni, \
    .AES128_CTR_inc_init = oqs_aes128_load_schedule_ni, \
    .AES128_CTR_inc_iv = oqs_aes128_load_iv_ni, \
    .AES128_CTR_inc_ivu64 = oqs_aes128_load_iv_u64_ni, \
    .AES128_free_schedule = oqs_aes128_free_schedule_ni, \
    .AES128_ECB_enc = aes128_ecb_enc_ni, \
    .AES128_ECB_enc_sch = oqs_aes128_ecb_enc_sch_ni, \
    .AES128_CTR_inc_stream_iv = oqs_aes128_ctr_enc_sch_ni, \
    .AES256_ECB_load_schedule = oqs_aes256_load_schedule_ni, \
    .AES256_CTR_inc_init = oqs_aes256_load_schedule_ni, \
    .AES256_CTR_inc_iv = oqs_aes256_load_iv_ni, \
    .AES256_CTR_inc_ivu64 = oqs_aes256_load_iv_u64_ni, \
    .AES256_free_schedule = oqs_aes256_free_schedule_ni, \
    .AES256_ECB_enc = aes256_ecb_enc_ni, \
    .AES256_ECB_enc_sch = oqs_aes256_ecb_enc_sch_ni, \
    .AES256_CTR_inc_stream_iv = oqs_aes256_ctr_enc_sch_ni, \
    .AES256_CTR_inc_stream_blks = oqs_aes256_ctr_enc_sch_upd_blks_ni, \
    .AES192_ECB_load_schedule = oqs_aes192_load_schedule_ni, \
    .AES192_free_schedule = oqs_aes192_free_schedule_ni, \
    .AES192_ECB_enc = aes192_ecb_enc_ni, \
    .AES192_ECB_enc_sch = oqs_aes192_ecb_enc_sch_ni, \
}
#endif

#if OQS_AES_HAVE_ARM
/* The ARMv8 backend has never provided a 64-bit IV loader; keep it a no-op. */
static void aes_armv8_load_iv_u64_noop(uint64_t iv, void *schedule) {
	(void) iv;
	(void) schedule;
}

OQS_AES_DEFINE_ECB_ENC(aes128_ecb_enc_armv8, oqs_aes128_load_schedule_no_bitslice, oqs_aes128_ecb_enc_sch_armv8, oqs_aes128_free_schedule_no_bitslice)
OQS_AES_DEFINE_ECB_ENC(aes256_ecb_enc_armv8, oqs_aes256_load_schedule_no_bitslice, oqs_aes256_ecb_enc_sch_armv8, oqs_aes256_free_schedule_no_bitslice)
OQS_AES_DEFINE_ECB_ENC(aes192_ecb_enc_armv8, oqs_aes192_load_schedule_no_bitslice, oqs_aes192_ecb_enc_sch_armv8, oqs_aes192_free_schedule_no_bitslice)

#define OQS_AES_ARMV8_CALLBACKS { \
    .AES128_ECB_load_schedule = oqs_aes128_load_schedule_no_bitslice, \
    .AES128_CTR_inc_init = oqs_aes128_load_schedule_no_bitslice, \
    .AES128_CTR_inc_iv = oqs_aes128_load_iv_armv8, \
    .AES128_CTR_inc_ivu64 = aes_armv8_load_iv_u64_noop, \
    .AES128_free_schedule = oqs_aes128_free_schedule_no_bitslice, \
    .AES128_ECB_enc = aes128_ecb_enc_armv8, \
    .AES128_ECB_enc_sch = oqs_aes128_ecb_enc_sch_armv8, \
    .AES128_CTR_inc_stream_iv = oqs_aes128_ctr_enc_sch_armv8, \
    .AES256_ECB_load_schedule = oqs_aes256_load_schedule_no_bitslice, \
    .AES256_CTR_inc_init = oqs_aes256_load_schedule_no_bitslice, \
    .AES256_CTR_inc_iv = oqs_aes256_load_iv_armv8, \
    .AES256_CTR_inc_ivu64 = aes_armv8_load_iv_u64_noop, \
    .AES256_free_schedule = oqs_aes256_free_schedule_no_bitslice, \
    .AES256_ECB_enc = aes256_ecb_enc_armv8, \
    .AES256_ECB_enc_sch = oqs_aes256_ecb_enc_sch_armv8, \
    .AES256_CTR_inc_stream_iv = oqs_aes256_ctr_enc_sch_armv8, \
    .AES256_CTR_inc_stream_blks = oqs_aes256_ctr_enc_sch_upd_blks_armv8, \
    .AES192_ECB_load_schedule = oqs_aes192_load_schedule_no_bitslice, \
    .AES192_free_schedule = oqs_aes192_free_schedule_no_bitslice, \
    .AES192_ECB_enc = aes192_ecb_enc_armv8, \
    .AES192_ECB_enc_sch = oqs_aes192_ecb_enc_sch_armv8, \
}
#endif

/*
 * Statically initialised with the compile-time choice. Dist builds start with
 * the portable C backend and are upgraded in OQS_AES_init() once the CPU
 * features are known.
 */
#if defined(OQS_DIST_X86_64_BUILD) || defined(OQS_DIST_ARM64_V8_BUILD)
struct OQS_AES_callbacks aes_default_callbacks = OQS_AES_C_CALLBACKS;
#elif OQS_AES_HAVE_NI
struct OQS_AES_callbacks aes_default_callbacks = OQS_AES_NI_CALLBACKS;
#elif OQS_AES_HAVE_ARM
struct OQS_AES_callbacks aes_default_callbacks = OQS_AES_ARMV8_CALLBACKS;
#else
struct OQS_AES_callbacks aes_default_callbacks = OQS_AES_C_CALLBACKS;
#endif

void OQS_AES_init(void) {
#if defined(OQS_DIST_X86_64_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AES)) {
		aes_default_callbacks = (struct OQS_AES_callbacks) OQS_AES_NI_CALLBACKS;
	}
#elif defined(OQS_DIST_ARM64_V8_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_AES)) {
		aes_default_callbacks = (struct OQS_AES_callbacks) OQS_AES_ARMV8_CALLBACKS;
	}
#endif
}
