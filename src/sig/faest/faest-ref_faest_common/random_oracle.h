/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_RANDOM_ORACLE_H
#define FAEST_RANDOM_ORACLE_H

#include "hash_shake.h"
#include "macros.h"

FAEST_BEGIN_C_DECL

// implementation of H_0

typedef hash_context H0_context_t;

void H0_init(H0_context_t* H0_ctx, unsigned int security_param);
void H0_update(H0_context_t* H0_ctx, const uint8_t* src, size_t len);
void H0_final(H0_context_t* H0_ctx, uint8_t* dst, size_t len);

// implementation of H_1

typedef hash_context H1_context_t;

void H1_init(H1_context_t* H1_ctx, unsigned int security_param);
void H1_update(H1_context_t* H1_ctx, const uint8_t* src, size_t len);
void H1_final(H1_context_t* H1_ctx, uint8_t* digest, size_t len);

// implementation of H_2

typedef hash_context H2_context_t;

void H2_init(H2_context_t* ctx, unsigned int security_param);
void H2_copy(H2_context_t* new_ctx, const H2_context_t* ctx);
void H2_update(H2_context_t* ctx, const uint8_t* src, size_t len);
void H2_update_u32_le(H2_context_t* ctx, uint32_t v);
void H2_0_final(H2_context_t* ctx, uint8_t* digest, size_t len);
void H2_1_final(H2_context_t* ctx, uint8_t* digest, size_t len);
void H2_2_final(H2_context_t* ctx, uint8_t* digest, size_t len);
void H2_3_final(H2_context_t* ctx, uint8_t* digest, size_t len);

// implementation for H_3

typedef hash_context H3_context_t;

void H3_init(H3_context_t* ctx, unsigned int security_param);
void H3_update(H3_context_t* ctx, const uint8_t* src, size_t len);
void H3_final(H3_context_t* ctx, uint8_t* digest, size_t len, uint8_t* iv);

// implementation for H_4

void H4(uint8_t* iv, const uint8_t* pre_iv, const uint8_t* mu, unsigned int security_params);

// implementation for H_5

void H5(uint8_t* digest, const uint8_t* iv, uint32_t e, const uint8_t* L_e,
        unsigned int security_params);

FAEST_END_C_DECL

#endif
