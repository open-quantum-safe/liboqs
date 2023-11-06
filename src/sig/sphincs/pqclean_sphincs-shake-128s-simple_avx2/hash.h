#ifndef SPX_HASH_H
#define SPX_HASH_H

#include <stddef.h>
#include <stdint.h>

#include "context.h"
#include "params.h"

#define prf_addr SPX_NAMESPACE(prf_addr)
void prf_addr(unsigned char *out, const spx_ctx *ctx,
              const uint32_t addr[8]);

#define gen_message_random SPX_NAMESPACE(gen_message_random)
void gen_message_random(unsigned char *R, const unsigned char *sk_prf,
                        const unsigned char *optrand,
                        const unsigned char *m, size_t mlen,
                        const spx_ctx *ctx);

#define hash_message SPX_NAMESPACE(hash_message)
void hash_message(unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
                  const unsigned char *R, const unsigned char *pk,
                  const unsigned char *m, size_t mlen,
                  const spx_ctx *ctx);

#endif
