/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include <string.h>

#include "random_oracle.h"
#include "instances.h"
#include "vole.h"

// H_0
void H0_init(H0_context_t* ctx, unsigned int security_param) {
  hash_init(ctx, security_param);
}

void H0_update(H0_context_t* ctx, const uint8_t* src, size_t len) {
  hash_update(ctx, src, len);
}

void H0_final(H0_context_t* ctx, uint8_t* dst, size_t len) {
  const uint8_t domain_sep_H0 = 0;
  hash_update(ctx, &domain_sep_H0, sizeof(domain_sep_H0));
  hash_final(ctx);
  hash_squeeze(ctx, dst, len);
  hash_clear(ctx);
}

// H_1
void H1_init(H1_context_t* ctx, unsigned int security_param) {
  hash_init(ctx, security_param);
}

void H1_update(H1_context_t* ctx, const uint8_t* src, size_t len) {
  hash_update(ctx, src, len);
}

void H1_final(H1_context_t* ctx, uint8_t* digest, size_t len) {
  const uint8_t domain_sep_H1 = 1;
  hash_update(ctx, &domain_sep_H1, sizeof(domain_sep_H1));
  hash_final(ctx);
  hash_squeeze(ctx, digest, len);
  hash_clear(ctx);
}

// H_2
void H2_init(H2_context_t* ctx, unsigned int security_param) {
  hash_init(ctx, security_param);
}

void H2_copy(H2_context_t* new_ctx, const H2_context_t* ctx) {
  hash_copy(new_ctx, ctx);
}

void H2_update(H2_context_t* ctx, const uint8_t* src, size_t len) {
  hash_update(ctx, src, len);
}

void H2_update_u32_le(H2_context_t* ctx, uint32_t v) {
  hash_update_uint32_le(ctx, v);
}

void H2_0_final(H2_context_t* ctx, uint8_t* digest, size_t len) {
  const uint8_t domain_sep_H2_0 = 8 + 0;
  hash_update(ctx, &domain_sep_H2_0, sizeof(domain_sep_H2_0));
  hash_final(ctx);
  hash_squeeze(ctx, digest, len);
  hash_clear(ctx);
}

void H2_1_final(H2_context_t* ctx, uint8_t* digest, size_t len) {
  const uint8_t domain_sep_H2_1 = 8 + 1;
  hash_update(ctx, &domain_sep_H2_1, sizeof(domain_sep_H2_1));
  hash_final(ctx);
  hash_squeeze(ctx, digest, len);
  hash_clear(ctx);
}

void H2_2_final(H2_context_t* ctx, uint8_t* digest, size_t len) {
  const uint8_t domain_sep_H2_2 = 8 + 2;
  hash_update(ctx, &domain_sep_H2_2, sizeof(domain_sep_H2_2));
  hash_final(ctx);
  hash_squeeze(ctx, digest, len);
  hash_clear(ctx);
}

void H2_3_final(H2_context_t* ctx, uint8_t* digest, size_t len) {
  const uint8_t domain_sep_H2_3 = 8 + 3;
  hash_update(ctx, &domain_sep_H2_3, sizeof(domain_sep_H2_3));
  hash_final(ctx);
  hash_squeeze(ctx, digest, len);
  hash_clear(ctx);
}

// H_3
void H3_init(H3_context_t* ctx, unsigned int security_param) {
  hash_init(ctx, security_param);
}

void H3_update(H3_context_t* ctx, const uint8_t* src, size_t len) {
  hash_update(ctx, src, len);
}

void H3_final(H3_context_t* ctx, uint8_t* digest, size_t len, uint8_t* iv) {
  const uint8_t domain_sep_H3 = 3;
  hash_update(ctx, &domain_sep_H3, sizeof(domain_sep_H3));
  hash_final(ctx);
  hash_squeeze(ctx, digest, len);
  hash_squeeze(ctx, iv, IV_SIZE);
  hash_clear(ctx);
}

// H_4
void H4(uint8_t* iv, const uint8_t* pre_iv, const uint8_t* mu, unsigned int security_params) {
  hash_context ctx;
  hash_init(&ctx, security_params);
  hash_update(&ctx, pre_iv, IV_SIZE);
  hash_update(&ctx, mu, (security_params * 2) / 8);
  const uint8_t domain_sep_H4 = 4;
  hash_update(&ctx, &domain_sep_H4, sizeof(domain_sep_H4));
  hash_final(&ctx);
  hash_squeeze(&ctx, iv, IV_SIZE);
  hash_clear(&ctx);
}

// H5
void H5(uint8_t* digest, const uint8_t* iv, uint32_t e, const uint8_t* L_e,
        unsigned int security_params) {
  hash_context ctx;
  hash_init(&ctx, security_params);
  hash_update(&ctx, iv, IV_SIZE);
  hash_update_uint32_le(&ctx, e);
  hash_update(&ctx, L_e, security_params / 8);
  const uint8_t domain_sep_H5 = 5;
  hash_update(&ctx, &domain_sep_H5, sizeof(domain_sep_H5));
  hash_final(&ctx);
  hash_squeeze(&ctx, digest, N_MASK_BYTES);
  hash_clear(&ctx);

#if N_MASK % 8
  digest[N_MASK_BYTES - 1] &= (UINT8_C(1) << (N_MASK % 8)) - UINT8_C(1);
#endif
}