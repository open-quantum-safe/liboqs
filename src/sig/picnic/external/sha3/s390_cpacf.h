/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef SHA3_S390_CPACF_H
#define SHA3_S390_CPACF_H

#include <string.h>
#include <stdint.h>

#include "macros.h"

typedef struct {
  uint8_t ctx[200]; /* param block */
  uint8_t data_block[168]; /* buffer for input and output data */
  uint8_t func; /* function: SHAKE128 or SHKAE256 */
  uint8_t data_block_size; /* block size */
  uint8_t pos; /* current position in data_block */
} hash_context ATTR_ALIGNED(32);

static inline void hash_init(hash_context* ctx, size_t digest_size) {
  memset(ctx->ctx, 0, sizeof(ctx->ctx));
  memset(ctx->data_block, 0, sizeof(ctx->data_block));
  if (digest_size == 32) {
    /* SHAKE128 */
    ctx->func = 0x24;
    ctx->data_block_size = 168;
  } else {
    /* SHAKE256 */
    ctx->func = 0x25;
    ctx->data_block_size = 136;
  }
  ctx->pos = 0;
}

/**
 * Perform KIMD instruction (hash multiple blocks of 168 (SHAKE128) or 136 (SHAKE256) bytes
 */
static inline void hash_update_kimd(hash_context* ctx, const uint8_t* data, size_t size) {
  /* function code in GR 0 */
  register long func asm("0") = ctx->func;
  /* param block in GR 1 */
  register uint8_t* param asm("1") = ctx->ctx;
  /* input data in an even numbered GR (goes into R2) */
  register const uint8_t* src asm("2") = data;
  /* size of input data (goes into R2+1); needs to be a multiple of the data block size */
  register size_t src_size asm("3") = size;

  asm volatile("0:  .insn rre,0xb93e0000,0,%[src]\n\t" /* KIMD opcode */
               "    brc   1,0b\n\t"                    /* handle partial completion */
               : [src] "+a"(src), "+d"(src_size)
               : "d"(func), "a"(param)
               : "cc", "memory");
}

/**
 * Perform KLMD instruction (hash and pad the last block of < 168 (SHAKE128) or 136 (SHAKE256) bytes
 * and produce XOF output of a block size)
 */
static inline void hash_update_klmd(hash_context* ctx, uint8_t* buffer, size_t buffer_size, const uint8_t* data, size_t size) {
  /* function code in GR 0 */
  register long func asm("0") = ctx->func;
  /* param block in GR 1 */
  register uint8_t* param asm("1") = ctx->ctx;
  /* input data in an even numbered GR (goes into R2) */
  register const uint8_t* src asm("2") = data;
  /* size of input data (goes into R2+1) */
  register size_t src_size asm("3") = size;
  /* output buffer in an even numbered GR (goes into R1) */
  register unsigned char* dst asm("4") = buffer;
  /* size of output buffer (goes into R1+1); needs to be a multiple of the data block size */
  register long dst_size asm("5") = buffer_size;

  asm volatile("0:  .insn rre,0xb93f0000,%[dst],%[src]\n\t" /* KLMD opcode */
               "    brc   1,0b\n\t"                         /* handle partial completion */
               : [src] "+a"(src), "+d"(src_size), [dst] "+a"(dst), "+d"(dst_size)
               : "d"(func), "a"(param)
               : "cc", "memory");
}

/**
 * Perform KLMD instruction (produce XOF output of a block size)
 */
static inline void hash_squeeze_kmld(hash_context* ctx, uint8_t* buffer, size_t size) {
  hash_update_klmd(ctx, buffer, size, NULL, 0);
}

static inline void hash_update(hash_context* ctx, const uint8_t* data, size_t size) {
  /* process buffered data */
  if (ctx->pos) {
    const size_t gap       = ctx->data_block_size - ctx->pos;
    const size_t copy_size = MIN(gap, size);

    memcpy(ctx->data_block + ctx->pos, data, copy_size);
    ctx->pos += copy_size;
    data += copy_size;
    size -= copy_size;

    if (ctx->pos == ctx->data_block_size) {
      hash_update_kimd(ctx, ctx->data_block, ctx->data_block_size);
      ctx->pos = 0;
    }
  }

  /* process as many full blocks as possible */
  if (size > ctx->data_block_size) {
    const size_t copy_size = size - (size % ctx->data_block_size);
    hash_update_kimd(ctx, data, copy_size);
    data += copy_size;
    size -= copy_size;
  }

  /* buffer remaining data */
  if (size) {
    memcpy(ctx->data_block, data, size);
    ctx->pos = size;
  }
}

static inline void hash_final(hash_context* ctx) {
  /* process remaining input (if available) */
  hash_update_klmd(ctx, ctx->data_block, ctx->data_block_size, ctx->data_block, ctx->pos);
  ctx->pos = 0;
}

static inline void hash_squeeze(hash_context* ctx, uint8_t* buffer, size_t buflen) {
  /* process buffered output */
  if (ctx->pos < ctx->data_block_size) {
    const size_t gap       = ctx->data_block_size - ctx->pos;
    const size_t copy_size = MIN(gap, buflen);

    memcpy(buffer, ctx->data_block + ctx->pos, copy_size);
    ctx->pos += copy_size;
    buffer += copy_size;
    buflen -= copy_size;
  }
  /* either ctx->pos == ctx->data_block_size or buflen == 0 */

  /* process as many full blocks as possible */
  if (buflen > ctx->data_block_size) {
    const size_t copy_size = buflen - (buflen % ctx->data_block_size);
    hash_squeeze_kmld(ctx, buffer, buflen);
    buffer += copy_size;
    buflen -= copy_size;
  }

  if (buflen) {
    hash_squeeze_kmld(ctx, ctx->data_block, ctx->data_block_size);
    memcpy(buffer, ctx->data_block, buflen);
    ctx->pos = buflen;
  }
}

#endif
