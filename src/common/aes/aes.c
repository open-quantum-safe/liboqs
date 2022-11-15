// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "aes.h"
#include "aes_local.h"

#if defined(OQS_DIST_X86_64_BUILD)
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
   do { \
      if (OQS_CPU_has_extension(OQS_CPU_EXT_AES)) { \
          stmt_ni; \
      } else { \
          stmt_c; \
      } \
   } while(0)
#elif defined(OQS_DIST_ARM64_V8_BUILD)
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    do { \
        if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_AES)) {  \
            stmt_arm; \
        } else { \
            stmt_c; \
        } \
    } while(0)
#elif defined(OQS_USE_AES_INSTRUCTIONS)
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    stmt_ni
#elif defined(OQS_USE_ARM_AES_INSTRUCTIONS)
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    stmt_arm
#else
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    stmt_c
#endif

void OQS_AES128_ECB_load_schedule(const uint8_t *key, void **_schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes128_load_schedule_c(key, _schedule),
	    oqs_aes128_load_schedule_ni(key, _schedule),
	    oqs_aes128_load_schedule_no_bitslice(key, _schedule)
	);
}

void OQS_AES128_free_schedule(void *schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes128_free_schedule_c(schedule),
	    oqs_aes128_free_schedule_ni(schedule),
	    oqs_aes128_free_schedule_no_bitslice(schedule)
	);
}

void OQS_AES256_ECB_load_schedule(const uint8_t *key, void **_schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_load_schedule_c(key, _schedule),
	    oqs_aes256_load_schedule_ni(key, _schedule),
	    oqs_aes256_load_schedule_no_bitslice(key, _schedule)
	);
}

void OQS_AES256_CTR_inc_init(const uint8_t *key, void **_schedule) {
	OQS_AES256_ECB_load_schedule(key, _schedule);
}

void OQS_AES256_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *_schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_load_iv_c(iv, iv_len, _schedule),
	    oqs_aes256_load_iv_ni(iv, iv_len, _schedule),
	    oqs_aes256_load_iv_armv8(iv, iv_len, _schedule)
	);
}

void OQS_AES256_CTR_inc_ivu64(uint64_t iv, void *_schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_load_iv_u64_c(iv, _schedule),
	    oqs_aes256_load_iv_u64_ni(iv, _schedule),
	    (void) iv; (void) _schedule
	);
}

void OQS_AES256_free_schedule(void *schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_free_schedule_c(schedule),
	    oqs_aes256_free_schedule_ni(schedule),
	    oqs_aes256_free_schedule_no_bitslice(schedule)
	);
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(key, &schedule);
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	C_OR_NI_OR_ARM(
	    oqs_aes128_ecb_enc_sch_c(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes128_ecb_enc_sch_ni(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes128_ecb_enc_sch_armv8(plaintext, plaintext_len, schedule, ciphertext)
	);
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule);
	OQS_AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_ecb_enc_sch_c(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes256_ecb_enc_sch_ni(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes256_ecb_enc_sch_armv8(plaintext, plaintext_len, schedule, ciphertext)
	);
}

void OQS_AES256_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_ctr_enc_sch_c(iv, iv_len, schedule, out, out_len),
	    oqs_aes256_ctr_enc_sch_ni(iv, iv_len, schedule, out, out_len),
	    oqs_aes256_ctr_enc_sch_armv8(iv, iv_len, schedule, out, out_len)
	);
}

void OQS_AES256_CTR_inc_stream_blks(void *schedule, uint8_t *out, size_t out_blks) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_ctr_enc_sch_upd_blks_c(schedule, out, out_blks),
	    oqs_aes256_ctr_enc_sch_upd_blks_ni(schedule, out, out_blks),
	    oqs_aes256_ctr_enc_sch_upd_blks_armv8(schedule, out, out_blks)
	);
}
