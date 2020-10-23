#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sha256avx.h"

// Transpose 8 vectors containing 32-bit values
void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_transpose(u256 s[8]) {
    u256 tmp0[8];
    u256 tmp1[8];
    tmp0[0] = _mm256_unpacklo_epi32(s[0], s[1]);
    tmp0[1] = _mm256_unpackhi_epi32(s[0], s[1]);
    tmp0[2] = _mm256_unpacklo_epi32(s[2], s[3]);
    tmp0[3] = _mm256_unpackhi_epi32(s[2], s[3]);
    tmp0[4] = _mm256_unpacklo_epi32(s[4], s[5]);
    tmp0[5] = _mm256_unpackhi_epi32(s[4], s[5]);
    tmp0[6] = _mm256_unpacklo_epi32(s[6], s[7]);
    tmp0[7] = _mm256_unpackhi_epi32(s[6], s[7]);
    tmp1[0] = _mm256_unpacklo_epi64(tmp0[0], tmp0[2]);
    tmp1[1] = _mm256_unpackhi_epi64(tmp0[0], tmp0[2]);
    tmp1[2] = _mm256_unpacklo_epi64(tmp0[1], tmp0[3]);
    tmp1[3] = _mm256_unpackhi_epi64(tmp0[1], tmp0[3]);
    tmp1[4] = _mm256_unpacklo_epi64(tmp0[4], tmp0[6]);
    tmp1[5] = _mm256_unpackhi_epi64(tmp0[4], tmp0[6]);
    tmp1[6] = _mm256_unpacklo_epi64(tmp0[5], tmp0[7]);
    tmp1[7] = _mm256_unpackhi_epi64(tmp0[5], tmp0[7]);
    s[0] = _mm256_permute2x128_si256(tmp1[0], tmp1[4], 0x20);
    s[1] = _mm256_permute2x128_si256(tmp1[1], tmp1[5], 0x20);
    s[2] = _mm256_permute2x128_si256(tmp1[2], tmp1[6], 0x20);
    s[3] = _mm256_permute2x128_si256(tmp1[3], tmp1[7], 0x20);
    s[4] = _mm256_permute2x128_si256(tmp1[0], tmp1[4], 0x31);
    s[5] = _mm256_permute2x128_si256(tmp1[1], tmp1[5], 0x31);
    s[6] = _mm256_permute2x128_si256(tmp1[2], tmp1[6], 0x31);
    s[7] = _mm256_permute2x128_si256(tmp1[3], tmp1[7], 0x31);
}

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_clone_statex8(sha256ctxx8 *outctx, const sha256ctxx8 *inctx) {
    memcpy(outctx, inctx, sizeof(sha256ctxx8));
}

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_init8x(sha256ctxx8 *ctx) {
    ctx->s[0] = _mm256_set_epi32((int)0x6a09e667, (int)0x6a09e667, (int)0x6a09e667, (int)0x6a09e667, (int)0x6a09e667, (int)0x6a09e667, (int)0x6a09e667, (int)0x6a09e667);
    ctx->s[1] = _mm256_set_epi32((int)0xbb67ae85, (int)0xbb67ae85, (int)0xbb67ae85, (int)0xbb67ae85, (int)0xbb67ae85, (int)0xbb67ae85, (int)0xbb67ae85, (int)0xbb67ae85);
    ctx->s[2] = _mm256_set_epi32((int)0x3c6ef372, (int)0x3c6ef372, (int)0x3c6ef372, (int)0x3c6ef372, (int)0x3c6ef372, (int)0x3c6ef372, (int)0x3c6ef372, (int)0x3c6ef372);
    ctx->s[3] = _mm256_set_epi32((int)0xa54ff53a, (int)0xa54ff53a, (int)0xa54ff53a, (int)0xa54ff53a, (int)0xa54ff53a, (int)0xa54ff53a, (int)0xa54ff53a, (int)0xa54ff53a);
    ctx->s[4] = _mm256_set_epi32((int)0x510e527f, (int)0x510e527f, (int)0x510e527f, (int)0x510e527f, (int)0x510e527f, (int)0x510e527f, (int)0x510e527f, (int)0x510e527f);
    ctx->s[5] = _mm256_set_epi32((int)0x9b05688c, (int)0x9b05688c, (int)0x9b05688c, (int)0x9b05688c, (int)0x9b05688c, (int)0x9b05688c, (int)0x9b05688c, (int)0x9b05688c);
    ctx->s[6] = _mm256_set_epi32((int)0x1f83d9ab, (int)0x1f83d9ab, (int)0x1f83d9ab, (int)0x1f83d9ab, (int)0x1f83d9ab, (int)0x1f83d9ab, (int)0x1f83d9ab, (int)0x1f83d9ab);
    ctx->s[7] = _mm256_set_epi32((int)0x5be0cd19, (int)0x5be0cd19, (int)0x5be0cd19, (int)0x5be0cd19, (int)0x5be0cd19, (int)0x5be0cd19, (int)0x5be0cd19, (int)0x5be0cd19);

    ctx->datalen = 0;
    ctx->msglen = 0;
}

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_update8x(sha256ctxx8 *ctx,
        const unsigned char *d0,
        const unsigned char *d1,
        const unsigned char *d2,
        const unsigned char *d3,
        const unsigned char *d4,
        const unsigned char *d5,
        const unsigned char *d6,
        const unsigned char *d7,
        unsigned long long len) {
    size_t i = 0;
    size_t bytes_to_copy;

    while (i < len) {
        bytes_to_copy = (size_t)len - i;
        if (bytes_to_copy > 64) {
            bytes_to_copy = 64;
        }
        memcpy(&ctx->msgblocks[64 * 0], d0 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 1], d1 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 2], d2 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 3], d3 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 4], d4 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 5], d5 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 6], d6 + i, bytes_to_copy);
        memcpy(&ctx->msgblocks[64 * 7], d7 + i, bytes_to_copy);
        ctx->datalen += (unsigned int)bytes_to_copy;
        i += bytes_to_copy;
        if (ctx->datalen == 64) {
            PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_transform8x(ctx, ctx->msgblocks);
            ctx->msglen += 512;
            ctx->datalen = 0;
        }
    }
}

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_final8x(sha256ctxx8 *ctx,
        unsigned char *out0,
        unsigned char *out1,
        unsigned char *out2,
        unsigned char *out3,
        unsigned char *out4,
        unsigned char *out5,
        unsigned char *out6,
        unsigned char *out7) {
    unsigned int i, curlen;

    // Padding
    if (ctx->datalen < 56) {
        for (i = 0; i < 8; ++i) {
            curlen = ctx->datalen;
            ctx->msgblocks[64 * i + curlen++] = 0x80;
            while (curlen < 64) {
                ctx->msgblocks[64 * i + curlen++] = 0x00;
            }
        }
    } else {
        for (i = 0; i < 8; ++i) {
            curlen = ctx->datalen;
            ctx->msgblocks[64 * i + curlen++] = 0x80;
            while (curlen < 64) {
                ctx->msgblocks[64 * i + curlen++] = 0x00;
            }
        }
        PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_transform8x(ctx, ctx->msgblocks);
        memset(ctx->msgblocks, 0, 8 * 64);
    }

    // Add length of the message to each block
    ctx->msglen += ctx->datalen * 8;
    for (i = 0; i < 8; i++) {
        ctx->msgblocks[64 * i + 63] = (unsigned char)ctx->msglen;
        ctx->msgblocks[64 * i + 62] = (unsigned char)(ctx->msglen >> 8);
        ctx->msgblocks[64 * i + 61] = (unsigned char)(ctx->msglen >> 16);
        ctx->msgblocks[64 * i + 60] = (unsigned char)(ctx->msglen >> 24);
        ctx->msgblocks[64 * i + 59] = (unsigned char)(ctx->msglen >> 32);
        ctx->msgblocks[64 * i + 58] = (unsigned char)(ctx->msglen >> 40);
        ctx->msgblocks[64 * i + 57] = (unsigned char)(ctx->msglen >> 48);
        ctx->msgblocks[64 * i + 56] = (unsigned char)(ctx->msglen >> 56);
    }
    PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_transform8x(ctx, ctx->msgblocks);

    // Compute final hash output
    PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_transpose(ctx->s);

    // Store Hash value
    STORE(out0, BYTESWAP(ctx->s[0]));
    STORE(out1, BYTESWAP(ctx->s[1]));
    STORE(out2, BYTESWAP(ctx->s[2]));
    STORE(out3, BYTESWAP(ctx->s[3]));
    STORE(out4, BYTESWAP(ctx->s[4]));
    STORE(out5, BYTESWAP(ctx->s[5]));
    STORE(out6, BYTESWAP(ctx->s[6]));
    STORE(out7, BYTESWAP(ctx->s[7]));
}

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_sha256_transform8x(sha256ctxx8 *ctx, const unsigned char *data) {
    u256 s[8], w[64], T0, T1;
    int i;

    // Load words and transform data correctly
    for (i = 0; i < 8; i++) {
        w[i] = BYTESWAP(LOAD(data + 64 * i));
        w[i + 8] = BYTESWAP(LOAD(data + 32 + 64 * i));
    }

    PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_transpose(w);
    PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_transpose(w + 8);

    // Initial State
    s[0] = ctx->s[0];
    s[1] = ctx->s[1];
    s[2] = ctx->s[2];
    s[3] = ctx->s[3];
    s[4] = ctx->s[4];
    s[5] = ctx->s[5];
    s[6] = ctx->s[6];
    s[7] = ctx->s[7];

    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 0, w[0]);
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 1, w[1]);
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 2, w[2]);
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 3, w[3]);
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 4, w[4]);
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 5, w[5]);
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 6, w[6]);
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 7, w[7]);
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 8, w[8]);
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 9, w[9]);
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 10, w[10]);
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 11, w[11]);
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 12, w[12]);
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 13, w[13]);
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 14, w[14]);
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 15, w[15]);
    w[16] = ADD4_32(WSIGMA1_AVX(w[14]), w[0], w[9], WSIGMA0_AVX(w[1]));
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 16, w[16]);
    w[17] = ADD4_32(WSIGMA1_AVX(w[15]), w[1], w[10], WSIGMA0_AVX(w[2]));
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 17, w[17]);
    w[18] = ADD4_32(WSIGMA1_AVX(w[16]), w[2], w[11], WSIGMA0_AVX(w[3]));
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 18, w[18]);
    w[19] = ADD4_32(WSIGMA1_AVX(w[17]), w[3], w[12], WSIGMA0_AVX(w[4]));
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 19, w[19]);
    w[20] = ADD4_32(WSIGMA1_AVX(w[18]), w[4], w[13], WSIGMA0_AVX(w[5]));
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 20, w[20]);
    w[21] = ADD4_32(WSIGMA1_AVX(w[19]), w[5], w[14], WSIGMA0_AVX(w[6]));
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 21, w[21]);
    w[22] = ADD4_32(WSIGMA1_AVX(w[20]), w[6], w[15], WSIGMA0_AVX(w[7]));
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 22, w[22]);
    w[23] = ADD4_32(WSIGMA1_AVX(w[21]), w[7], w[16], WSIGMA0_AVX(w[8]));
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 23, w[23]);
    w[24] = ADD4_32(WSIGMA1_AVX(w[22]), w[8], w[17], WSIGMA0_AVX(w[9]));
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 24, w[24]);
    w[25] = ADD4_32(WSIGMA1_AVX(w[23]), w[9], w[18], WSIGMA0_AVX(w[10]));
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 25, w[25]);
    w[26] = ADD4_32(WSIGMA1_AVX(w[24]), w[10], w[19], WSIGMA0_AVX(w[11]));
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 26, w[26]);
    w[27] = ADD4_32(WSIGMA1_AVX(w[25]), w[11], w[20], WSIGMA0_AVX(w[12]));
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 27, w[27]);
    w[28] = ADD4_32(WSIGMA1_AVX(w[26]), w[12], w[21], WSIGMA0_AVX(w[13]));
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 28, w[28]);
    w[29] = ADD4_32(WSIGMA1_AVX(w[27]), w[13], w[22], WSIGMA0_AVX(w[14]));
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 29, w[29]);
    w[30] = ADD4_32(WSIGMA1_AVX(w[28]), w[14], w[23], WSIGMA0_AVX(w[15]));
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 30, w[30]);
    w[31] = ADD4_32(WSIGMA1_AVX(w[29]), w[15], w[24], WSIGMA0_AVX(w[16]));
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 31, w[31]);
    w[32] = ADD4_32(WSIGMA1_AVX(w[30]), w[16], w[25], WSIGMA0_AVX(w[17]));
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 32, w[32]);
    w[33] = ADD4_32(WSIGMA1_AVX(w[31]), w[17], w[26], WSIGMA0_AVX(w[18]));
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 33, w[33]);
    w[34] = ADD4_32(WSIGMA1_AVX(w[32]), w[18], w[27], WSIGMA0_AVX(w[19]));
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 34, w[34]);
    w[35] = ADD4_32(WSIGMA1_AVX(w[33]), w[19], w[28], WSIGMA0_AVX(w[20]));
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 35, w[35]);
    w[36] = ADD4_32(WSIGMA1_AVX(w[34]), w[20], w[29], WSIGMA0_AVX(w[21]));
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 36, w[36]);
    w[37] = ADD4_32(WSIGMA1_AVX(w[35]), w[21], w[30], WSIGMA0_AVX(w[22]));
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 37, w[37]);
    w[38] = ADD4_32(WSIGMA1_AVX(w[36]), w[22], w[31], WSIGMA0_AVX(w[23]));
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 38, w[38]);
    w[39] = ADD4_32(WSIGMA1_AVX(w[37]), w[23], w[32], WSIGMA0_AVX(w[24]));
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 39, w[39]);
    w[40] = ADD4_32(WSIGMA1_AVX(w[38]), w[24], w[33], WSIGMA0_AVX(w[25]));
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 40, w[40]);
    w[41] = ADD4_32(WSIGMA1_AVX(w[39]), w[25], w[34], WSIGMA0_AVX(w[26]));
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 41, w[41]);
    w[42] = ADD4_32(WSIGMA1_AVX(w[40]), w[26], w[35], WSIGMA0_AVX(w[27]));
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 42, w[42]);
    w[43] = ADD4_32(WSIGMA1_AVX(w[41]), w[27], w[36], WSIGMA0_AVX(w[28]));
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 43, w[43]);
    w[44] = ADD4_32(WSIGMA1_AVX(w[42]), w[28], w[37], WSIGMA0_AVX(w[29]));
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 44, w[44]);
    w[45] = ADD4_32(WSIGMA1_AVX(w[43]), w[29], w[38], WSIGMA0_AVX(w[30]));
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 45, w[45]);
    w[46] = ADD4_32(WSIGMA1_AVX(w[44]), w[30], w[39], WSIGMA0_AVX(w[31]));
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 46, w[46]);
    w[47] = ADD4_32(WSIGMA1_AVX(w[45]), w[31], w[40], WSIGMA0_AVX(w[32]));
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 47, w[47]);
    w[48] = ADD4_32(WSIGMA1_AVX(w[46]), w[32], w[41], WSIGMA0_AVX(w[33]));
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 48, w[48]);
    w[49] = ADD4_32(WSIGMA1_AVX(w[47]), w[33], w[42], WSIGMA0_AVX(w[34]));
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 49, w[49]);
    w[50] = ADD4_32(WSIGMA1_AVX(w[48]), w[34], w[43], WSIGMA0_AVX(w[35]));
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 50, w[50]);
    w[51] = ADD4_32(WSIGMA1_AVX(w[49]), w[35], w[44], WSIGMA0_AVX(w[36]));
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 51, w[51]);
    w[52] = ADD4_32(WSIGMA1_AVX(w[50]), w[36], w[45], WSIGMA0_AVX(w[37]));
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 52, w[52]);
    w[53] = ADD4_32(WSIGMA1_AVX(w[51]), w[37], w[46], WSIGMA0_AVX(w[38]));
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 53, w[53]);
    w[54] = ADD4_32(WSIGMA1_AVX(w[52]), w[38], w[47], WSIGMA0_AVX(w[39]));
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 54, w[54]);
    w[55] = ADD4_32(WSIGMA1_AVX(w[53]), w[39], w[48], WSIGMA0_AVX(w[40]));
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 55, w[55]);
    w[56] = ADD4_32(WSIGMA1_AVX(w[54]), w[40], w[49], WSIGMA0_AVX(w[41]));
    SHA256ROUND_AVX(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], 56, w[56]);
    w[57] = ADD4_32(WSIGMA1_AVX(w[55]), w[41], w[50], WSIGMA0_AVX(w[42]));
    SHA256ROUND_AVX(s[7], s[0], s[1], s[2], s[3], s[4], s[5], s[6], 57, w[57]);
    w[58] = ADD4_32(WSIGMA1_AVX(w[56]), w[42], w[51], WSIGMA0_AVX(w[43]));
    SHA256ROUND_AVX(s[6], s[7], s[0], s[1], s[2], s[3], s[4], s[5], 58, w[58]);
    w[59] = ADD4_32(WSIGMA1_AVX(w[57]), w[43], w[52], WSIGMA0_AVX(w[44]));
    SHA256ROUND_AVX(s[5], s[6], s[7], s[0], s[1], s[2], s[3], s[4], 59, w[59]);
    w[60] = ADD4_32(WSIGMA1_AVX(w[58]), w[44], w[53], WSIGMA0_AVX(w[45]));
    SHA256ROUND_AVX(s[4], s[5], s[6], s[7], s[0], s[1], s[2], s[3], 60, w[60]);
    w[61] = ADD4_32(WSIGMA1_AVX(w[59]), w[45], w[54], WSIGMA0_AVX(w[46]));
    SHA256ROUND_AVX(s[3], s[4], s[5], s[6], s[7], s[0], s[1], s[2], 61, w[61]);
    w[62] = ADD4_32(WSIGMA1_AVX(w[60]), w[46], w[55], WSIGMA0_AVX(w[47]));
    SHA256ROUND_AVX(s[2], s[3], s[4], s[5], s[6], s[7], s[0], s[1], 62, w[62]);
    w[63] = ADD4_32(WSIGMA1_AVX(w[61]), w[47], w[56], WSIGMA0_AVX(w[48]));
    SHA256ROUND_AVX(s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[0], 63, w[63]);

    // Feed Forward
    ctx->s[0] = ADD32(s[0], ctx->s[0]);
    ctx->s[1] = ADD32(s[1], ctx->s[1]);
    ctx->s[2] = ADD32(s[2], ctx->s[2]);
    ctx->s[3] = ADD32(s[3], ctx->s[3]);
    ctx->s[4] = ADD32(s[4], ctx->s[4]);
    ctx->s[5] = ADD32(s[5], ctx->s[5]);
    ctx->s[6] = ADD32(s[6], ctx->s[6]);
    ctx->s[7] = ADD32(s[7], ctx->s[7]);
}
