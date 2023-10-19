#include <stdint.h>
#include <string.h>

#include "hash.h"

#include "address.h"
#include "fips202.h"
#include "params.h"
#include "utils.h"

/*
 * Computes PRF(pk_seed, sk_seed, addr)
 */
void prf_addr(unsigned char *out, const spx_ctx *ctx,
              const uint32_t addr[8]) {
    unsigned char buf[2 * SPX_N + SPX_ADDR_BYTES];

    memcpy(buf, ctx->pub_seed, SPX_N);
    memcpy(buf + SPX_N, addr, SPX_ADDR_BYTES);
    memcpy(buf + SPX_N + SPX_ADDR_BYTES, ctx->sk_seed, SPX_N);

    shake256(out, SPX_N, buf, 2 * SPX_N + SPX_ADDR_BYTES);
}

/**
 * Computes the message-dependent randomness R, using a secret seed and an
 * optional randomization value as well as the message.
 */
void gen_message_random(unsigned char *R, const unsigned char *sk_prf,
                        const unsigned char *optrand,
                        const unsigned char *m, size_t mlen,
                        const spx_ctx *ctx) {
    (void)ctx;
    shake256incctx s_inc;

    shake256_inc_init(&s_inc);
    shake256_inc_absorb(&s_inc, sk_prf, SPX_N);
    shake256_inc_absorb(&s_inc, optrand, SPX_N);
    shake256_inc_absorb(&s_inc, m, mlen);
    shake256_inc_finalize(&s_inc);
    shake256_inc_squeeze(R, SPX_N, &s_inc);
    shake256_inc_ctx_release(&s_inc);
}

/**
 * Computes the message hash using R, the public key, and the message.
 * Outputs the message digest and the index of the leaf. The index is split in
 * the tree index and the leaf index, for convenient copying to an address.
 */
void hash_message(unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
                  const unsigned char *R, const unsigned char *pk,
                  const unsigned char *m, size_t mlen,
                  const spx_ctx *ctx) {
    (void)ctx;
#define SPX_TREE_BITS (SPX_TREE_HEIGHT * (SPX_D - 1))
#define SPX_TREE_BYTES ((SPX_TREE_BITS + 7) / 8)
#define SPX_LEAF_BITS SPX_TREE_HEIGHT
#define SPX_LEAF_BYTES ((SPX_LEAF_BITS + 7) / 8)
#define SPX_DGST_BYTES (SPX_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)

    unsigned char buf[SPX_DGST_BYTES];
    unsigned char *bufp = buf;
    shake256incctx s_inc;

    shake256_inc_init(&s_inc);
    shake256_inc_absorb(&s_inc, R, SPX_N);
    shake256_inc_absorb(&s_inc, pk, SPX_PK_BYTES);
    shake256_inc_absorb(&s_inc, m, mlen);
    shake256_inc_finalize(&s_inc);
    shake256_inc_squeeze(buf, SPX_DGST_BYTES, &s_inc);
    shake256_inc_ctx_release(&s_inc);

    memcpy(digest, bufp, SPX_FORS_MSG_BYTES);
    bufp += SPX_FORS_MSG_BYTES;

    *tree = bytes_to_ull(bufp, SPX_TREE_BYTES);
    *tree &= (~(uint64_t)0) >> (64 - SPX_TREE_BITS);
    bufp += SPX_TREE_BYTES;

    *leaf_idx = (uint32_t)bytes_to_ull(bufp, SPX_LEAF_BYTES);
    *leaf_idx &= (~(uint32_t)0) >> (32 - SPX_LEAF_BITS);
}
