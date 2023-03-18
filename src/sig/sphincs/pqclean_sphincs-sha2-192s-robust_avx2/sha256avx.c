#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sha256avx.h"

static const unsigned int RC[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define u32 uint32_t
#define u256 __m256i

#define XOR _mm256_xor_si256
#define OR _mm256_or_si256
#define AND _mm256_and_si256
#define ADD32 _mm256_add_epi32
#define NOT(x) _mm256_xor_si256(x, _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1))

#define LOAD(src) _mm256_loadu_si256((__m256i *)(src))
#define STORE(dest,src) _mm256_storeu_si256((__m256i *)(dest),src)

#define BYTESWAP(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(0xc,0xd,0xe,0xf,0x8,0x9,0xa,0xb,0x4,0x5,0x6,0x7,0x0,0x1,0x2,0x3,0xc,0xd,0xe,0xf,0x8,0x9,0xa,0xb,0x4,0x5,0x6,0x7,0x0,0x1,0x2,0x3))

#define SHIFTR32(x, y) _mm256_srli_epi32(x, y)
#define SHIFTL32(x, y) _mm256_slli_epi32(x, y)

#define ROTR32(x, y) OR(SHIFTR32(x, y), SHIFTL32(x, 32 - (y)))
#define ROTL32(x, y) OR(SHIFTL32(x, y), SHIFTR32(x, 32 - (y)))

#define XOR3(a, b, c) XOR(XOR(a, b), c)

#define ADD3_32(a, b, c) ADD32(ADD32(a, b), c)
#define ADD4_32(a, b, c, d) ADD32(ADD32(ADD32(a, b), c), d)
#define ADD5_32(a, b, c, d, e) ADD32(ADD32(ADD32(ADD32(a, b), c), d), e)

#define MAJ_AVX(a, b, c) XOR3(AND(a, b), AND(a, c), AND(b, c))
#define CH_AVX(a, b, c) XOR(AND(a, b), AND(NOT(a), c))

#define SIGMA1_AVX(x) XOR3(ROTR32(x, 6), ROTR32(x, 11), ROTR32(x, 25))
#define SIGMA0_AVX(x) XOR3(ROTR32(x, 2), ROTR32(x, 13), ROTR32(x, 22))

#define WSIGMA1_AVX(x) XOR3(ROTR32(x, 17), ROTR32(x, 19), SHIFTR32(x, 10))
#define WSIGMA0_AVX(x) XOR3(ROTR32(x, 7), ROTR32(x, 18), SHIFTR32(x, 3))

#define SHA256ROUND_AVX(a, b, c, d, e, f, g, h, rc, w) \
    T0 = ADD5_32(h, SIGMA1_AVX(e), CH_AVX(e, f, g), _mm256_set1_epi32((int)RC[rc]), w); \
    (d) = ADD32(d, T0); \
    T1 = ADD32(SIGMA0_AVX(a), MAJ_AVX(a, b, c)); \
    (h) = ADD32(T0, T1);

// Transpose 8 vectors containing 32-bit values
static void transpose(u256 s[8]) {
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

void sha256_ctx_clone8x(sha256x8ctx *out, const sha256x8ctx *in) {
    memcpy(out, in, sizeof(sha256x8ctx));
}

void sha256_init8x(sha256x8ctx *ctx) {
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

void sha256_final8x(sha256x8ctx *ctx,
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
        sha256_transform8x(ctx,
                           &ctx->msgblocks[64 * 0],
                           &ctx->msgblocks[64 * 1],
                           &ctx->msgblocks[64 * 2],
                           &ctx->msgblocks[64 * 3],
                           &ctx->msgblocks[64 * 4],
                           &ctx->msgblocks[64 * 5],
                           &ctx->msgblocks[64 * 6],
                           &ctx->msgblocks[64 * 7]
                          );
        memset(ctx->msgblocks, 0, 8 * 64);
    }

    // Add length of the message to each block
    ctx->msglen += ctx->datalen * 8;
    for (i = 0; i < 8; i++) {
        ctx->msgblocks[64 * i + 63] = (unsigned char)(ctx->msglen);
        ctx->msgblocks[64 * i + 62] = (unsigned char)(ctx->msglen >> 8);
        ctx->msgblocks[64 * i + 61] = (unsigned char)(ctx->msglen >> 16);
        ctx->msgblocks[64 * i + 60] = (unsigned char)(ctx->msglen >> 24);
        ctx->msgblocks[64 * i + 59] = (unsigned char)(ctx->msglen >> 32);
        ctx->msgblocks[64 * i + 58] = (unsigned char)(ctx->msglen >> 40);
        ctx->msgblocks[64 * i + 57] = (unsigned char)(ctx->msglen >> 48);
        ctx->msgblocks[64 * i + 56] = (unsigned char)(ctx->msglen >> 56);
    }
    sha256_transform8x(ctx,
                       &ctx->msgblocks[64 * 0],
                       &ctx->msgblocks[64 * 1],
                       &ctx->msgblocks[64 * 2],
                       &ctx->msgblocks[64 * 3],
                       &ctx->msgblocks[64 * 4],
                       &ctx->msgblocks[64 * 5],
                       &ctx->msgblocks[64 * 6],
                       &ctx->msgblocks[64 * 7]
                      );

    // Compute final hash output
    transpose(ctx->s);

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

void sha256_transform8x(sha256x8ctx *ctx,
                        const unsigned char *data0,
                        const unsigned char *data1,
                        const unsigned char *data2,
                        const unsigned char *data3,
                        const unsigned char *data4,
                        const unsigned char *data5,
                        const unsigned char *data6,
                        const unsigned char *data7) {
    u256 s[8], w[64], T0, T1;

    // Load words and transform data correctly
    w[0] = BYTESWAP(LOAD(data0));
    w[0 + 8] = BYTESWAP(LOAD(data0 + 32));
    w[1] = BYTESWAP(LOAD(data1));
    w[1 + 8] = BYTESWAP(LOAD(data1 + 32));
    w[2] = BYTESWAP(LOAD(data2));
    w[2 + 8] = BYTESWAP(LOAD(data2 + 32));
    w[3] = BYTESWAP(LOAD(data3));
    w[3 + 8] = BYTESWAP(LOAD(data3 + 32));
    w[4] = BYTESWAP(LOAD(data4));
    w[4 + 8] = BYTESWAP(LOAD(data4 + 32));
    w[5] = BYTESWAP(LOAD(data5));
    w[5 + 8] = BYTESWAP(LOAD(data5 + 32));
    w[6] = BYTESWAP(LOAD(data6));
    w[6 + 8] = BYTESWAP(LOAD(data6 + 32));
    w[7] = BYTESWAP(LOAD(data7));
    w[7 + 8] = BYTESWAP(LOAD(data7 + 32));

    transpose(w);
    transpose(w + 8);

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
