#include "rng.h"

#include <string.h> // void *memset(void *s, int c, size_t n);

#include "aes.h"
#include "qc_ldpc_parameters.h"

/*
 seedexpander_init()
 ctx            - stores the current state of an instance of the seed expander
 seed           - a 32 byte random value
 diversifier    - an 8 byte diversifier
 maxlen         - maximum number of bytes (less than 2**32) generated under this seed and diversifier
 */
static void seedexpander_init(AES_XOF_struct *ctx,
                              uint8_t *seed,
                              uint8_t *diversifier,
                              size_t maxlen) {

    ctx->length_remaining = maxlen;

    memset(ctx->key, 0, 32);
    int max_accessible_seed_len = TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH;
    memcpy(ctx->key, seed, max_accessible_seed_len);

    memcpy(ctx->ctr, diversifier, 8);
    ctx->ctr[11] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[10] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[9] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[8] = maxlen % 256;
    memset(ctx->ctr + 12, 0x00, 4);

    ctx->buffer_pos = 16;
    memset(ctx->buffer, 0x00, 16);
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander_from_trng(AES_XOF_struct *ctx,
        const uint8_t *trng_entropy
        /* TRNG_BYTE_LENGTH wide buffer */) {

    /*the NIST seedexpander will however access 32B from this buffer */
    unsigned int prng_buffer_size = TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH;
    uint8_t prng_buffer[TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH] = {0x00};
    uint8_t *diversifier = (uint8_t *)trng_entropy + 32;

    memcpy(prng_buffer,
           trng_entropy,
           TRNG_BYTE_LENGTH < prng_buffer_size ? TRNG_BYTE_LENGTH : prng_buffer_size);

    /* the required seed expansion will be quite small, set the max number of
     * bytes conservatively to 10 MiB*/
    seedexpander_init(ctx, prng_buffer, diversifier, RNG_MAXLEN);
}

/*
 seedexpander()
    ctx  - stores the current state of an instance of the seed expander
    x    - returns the XOF data
    xlen - number of bytes to return
 */
int PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander(AES_XOF_struct *ctx, uint8_t *x, size_t xlen) {
    size_t offset;
    aes256ctx ctx256;

    if ( x == NULL ) {
        return RNG_BAD_OUTBUF;
    }
    if ( xlen >= ctx->length_remaining ) {
        return RNG_BAD_REQ_LEN;
    }

    aes256_keyexp(&ctx256, ctx->key);
    ctx->length_remaining -= xlen;

    offset = 0;
    while ( xlen > 0 ) {
        if ( xlen <= (16 - ctx->buffer_pos) ) { // buffer has what we need
            memcpy(x + offset, ctx->buffer + ctx->buffer_pos, xlen);
            ctx->buffer_pos += xlen;

            return RNG_SUCCESS;
        }

        // take what's in the buffer
        memcpy(x + offset, ctx->buffer + ctx->buffer_pos, 16 - ctx->buffer_pos);
        xlen -= 16 - ctx->buffer_pos;
        offset += 16 - ctx->buffer_pos;

        aes256_ecb(ctx->buffer, ctx->ctr, 16 / AES_BLOCKBYTES, &ctx256);
        ctx->buffer_pos = 0;

        //increment the counter
        for (int i = 15; i >= 12; i--) {
            if ( ctx->ctr[i] == 0xff ) {
                ctx->ctr[i] = 0x00;
            } else {
                ctx->ctr[i]++;
                break;
            }
        }

    }
    aes256_ctx_release(&ctx256);

    return RNG_SUCCESS;
}
