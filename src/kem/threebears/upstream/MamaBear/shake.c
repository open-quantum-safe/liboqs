/* Optimized cSHAKE-256 using Keccak from KCP */
#include "shake.h"

#if VECLEN==8
    #include                <libkeccak.a.headers/KeccakP-1600-times8-SnP.h>
    #define kv                 KeccakP1600times8_PermuteAll_24rounds
    #define kv_state_size      KeccakP1600times8_statesSizeInBytes
    #define kv_state_alignment KeccakP1600times8_statesAlignment
    #define kv_add_byte        KeccakP1600times8_AddByte
    #define kv_extract_lanes   KeccakP1600times8_ExtractLanesAll
    #define kv_extract_bytes   KeccakP1600times8_ExtractBytes
    #define kv_init            KeccakP1600times8_StaticInitialize
    #define kv_broadcast       KeccakP1600times8_OverwriteLanesAll
#elif VECLEN==4
    #include                <libkeccak.a.headers/KeccakP-1600-times4-SnP.h>
    #define kv                 KeccakP1600times4_PermuteAll_24rounds
    #define kv_state_size      KeccakP1600times4_statesSizeInBytes
    #define kv_state_alignment KeccakP1600times4_statesAlignment
    #define kv_add_byte        KeccakP1600times4_AddByte
    #define kv_extract_lanes   KeccakP1600times4_ExtractLanesAll
    #define kv_extract_bytes   KeccakP1600times4_ExtractBytes
    #define kv_init            KeccakP1600times4_StaticInitialize
    #define kv_broadcast       KeccakP1600times4_OverwriteLanesAll
#elif VECLEN==2
    #define VECLEN                             2
    #include                <libkeccak.a.headers/KeccakP-1600-times2-SnP.h>
    #define kv                 KeccakP1600times2_PermuteAll_24rounds
    #define kv_state_size      KeccakP1600times2_statesSizeInBytes
    #define kv_state_alignment KeccakP1600times2_statesAlignment
    #define kv_add_byte        KeccakP1600times2_AddByte
    #define kv_extract_lanes   KeccakP1600times2_ExtractLanesAll
    #define kv_extract_bytes   KeccakP1600times2_ExtractBytes
    #define kv_init            KeccakP1600times2_StaticInitialize
    #define kv_broadcast       KeccakP1600times2_OverwriteLanesAll
#elif VECLEN==1
#else
#error "Need veclen in {1,2,4,8}"
#endif

#define HASH_STATE_BYTES (1600/8)
#define CSHAKE256_RATE (HASH_STATE_BYTES-256/4)
#define CSHAKE256_PAD 0x04 /* SHAKE would be 0x1F */

static void keccakf(hash_ctx_t ctx) {
    KeccakP1600_Permute_24rounds(ctx->state);
    ctx->position = 0;
}

void hash_update (
    struct hash_ctx_s * __restrict__ ctx,
    const uint8_t *in,
    size_t len
) {
    assert(!ctx->squeezing);
    while (len >= (unsigned)(CSHAKE256_RATE - ctx->position)) {
        size_t cando = CSHAKE256_RATE - ctx->position;
        KeccakP1600_AddBytes(ctx->state, in, ctx->position, cando);
        keccakf(ctx);
        len -= cando;
        in  += cando;
    }
    KeccakP1600_AddBytes(ctx->state, in, ctx->position, len);
    ctx->position += len;
}

void hash_output (
    hash_ctx_t ctx,
    uint8_t * __restrict__ out,
    size_t len
) {
    if (!ctx->squeezing) {
        if (ctx->position >= CSHAKE256_RATE) keccakf(ctx);
        ctx->squeezing = 1;
        KeccakP1600_AddByte(ctx->state, CSHAKE256_PAD, ctx->position);
        KeccakP1600_AddByte(ctx->state, 0x80, CSHAKE256_RATE-1);
        keccakf(ctx);
    }
    
    while (len >= (unsigned)(CSHAKE256_RATE - ctx->position)) {
        size_t cando = CSHAKE256_RATE - ctx->position;
        KeccakP1600_ExtractBytes(ctx->state, out, ctx->position, cando);
        keccakf(ctx);
        len -= cando;
        out += cando;
    }
    KeccakP1600_ExtractBytes(ctx->state, out, ctx->position, len);
    ctx->position += len;
}

#if VECLEN==1
static inline void hash_update_byte (hash_ctx_t ctx, uint8_t b) {
    assert(!ctx->squeezing);
    if (ctx->position >= CSHAKE256_RATE) keccakf(ctx);
    KeccakP1600_AddByte(ctx->state, b, ctx->position);
    ctx->position++;
}
#endif


/* Hash instances for ThreeBears */
void threebears_cshake_init(hash_ctx_t ctx) {
    KeccakP1600_StaticInitialize();
    const uint8_t precomputed[HASH_STATE_BYTES] = {
        0xd4,0xd9,0x0c,0x1d,0xc8,0x67,0x0e,0xf6,
        0x61,0x0f,0x96,0xc6,0xa5,0x92,0xb3,0xe1,
        0x65,0xf3,0xd2,0x13,0xc5,0x15,0x8e,0x34,
        0xb8,0x10,0xa2,0x0e,0x94,0xf1,0x21,0xff,
        0xd2,0x9a,0x43,0x6e,0x84,0x22,0xe0,0x16,
        0xcd,0x37,0xc9,0x4c,0x21,0x9d,0x73,0xe8,
        0x01,0x84,0x23,0x8e,0x3d,0xeb,0x94,0x90,
        0xff,0x92,0x41,0xd9,0x0a,0x42,0x0c,0xb1,
        0xb2,0x17,0x6e,0x45,0xa1,0x2c,0x84,0x5f,
        0x48,0x10,0x89,0xfe,0x19,0xec,0xda,0x86,
        0x49,0xa9,0xf8,0xf6,0x45,0x34,0x5c,0xfd,
        0x54,0x3a,0x72,0x38,0x8f,0x84,0x06,0x7e,
        0xbc,0x5d,0xda,0x45,0x3d,0x76,0x15,0x10,
        0x2b,0x23,0x57,0x75,0x9c,0x4d,0x0b,0x90,
        0x0e,0x16,0xcd,0x45,0x3e,0xa3,0xda,0x86,
        0xa0,0x4b,0x62,0xf0,0xb5,0xe9,0xd0,0xc1,
        0xcd,0xbb,0x84,0x16,0xac,0x62,0x24,0x74,
        0xb0,0x4e,0x2a,0xf9,0x9c,0xa7,0xc1,0xc9,
        0xbb,0x18,0x8f,0xd7,0x2a,0xd0,0xac,0xfb,
        0xb1,0x11,0x6f,0x7b,0x5c,0x47,0x19,0xb5,
        0xf8,0x3c,0x85,0xbf,0x03,0x43,0x09,0x55,
        0x9d,0x9f,0x12,0xf1,0xce,0x12,0x42,0xd4,
        0x46,0x49,0x7e,0x20,0x95,0xf6,0x47,0x12,
        0xbf,0x2a,0x58,0x00,0x5c,0x7f,0xc1,0x1d,
        0xa1,0xb1,0xf3,0x75,0xa9,0xcc,0xa9,0x20
    };
    KeccakP1600_OverwriteBytes(ctx->state, precomputed, 0, sizeof(precomputed));
    ctx->position = ctx->squeezing = 0;
}

#if VECLEN > 1
/************ Vectorized CSHAKE ************/
void hash_times_n(
    uint8_t *output,
    unsigned outlen,
    unsigned stride,
    const hash_ctx_t ctx0,
    uint8_t iv,
    unsigned n
) {
    kv_init();
    unsigned ctx_position = ctx0->position;
    uint8_t state_transfer[HASH_STATE_BYTES] __attribute__((aligned(8)));
    KeccakP1600_ExtractBytes(ctx0->state, state_transfer, 0, HASH_STATE_BYTES);
    uint8_t ctxv[kv_state_size] __attribute__((aligned(kv_state_alignment)));
    
    unsigned i,j;
    for (j=0; j<n; j+=VECLEN) {
        kv_broadcast(ctxv, state_transfer, 25, 0);
        for (i=0; i<VECLEN; i++) {
            kv_add_byte(ctxv, i, j+i+iv, ctx_position);
        }
        
        /* Do the input padding */
        unsigned position=ctx_position+1;
        if (ctx_position >= CSHAKE256_RATE) {
            kv(ctxv);
            position=0;
        }
        for (unsigned i=0; i<VECLEN; i++) {
            kv_add_byte(ctxv, i, CSHAKE256_PAD, position);
            kv_add_byte(ctxv, i, 0x80, CSHAKE256_RATE-1);
        }
    
        /* Extract */
        for (unsigned done=0; done<outlen; done+=CSHAKE256_RATE) {
            kv(ctxv);
            unsigned cando = CSHAKE256_RATE;
	    if (cando > outlen-done) cando = outlen-done;
            if (stride%8==0 && cando%8==0 && n-j>=VECLEN) {
                kv_extract_lanes(ctxv, output+stride*j+done, cando/8, stride/8);
            } else for (unsigned i=j; i<n; i++) {
                kv_extract_bytes(ctxv, i-j, output+done+stride*i, 0, cando);
            }
        }
    }
    secure_bzero(state_transfer,sizeof(state_transfer));
    secure_bzero(ctxv,sizeof(ctxv));
}
#else /* !(VECLEN > 1) */
void hash_times_n (
    uint8_t *output,
    unsigned outlen,
    unsigned stride,
    const hash_ctx_t ctx0,
    uint8_t iv,
    unsigned n
) {
    for (unsigned i=0; i<n; i++) {
        hash_ctx_t ctx;
        memcpy(ctx,ctx0,sizeof(ctx));
        hash_update_byte(ctx,iv+i);
        hash_output(ctx,output+i*stride,outlen);
        hash_destroy(ctx);
    }
}
#endif /* __HAVE_VECTORIZED_SHAKE__ */
