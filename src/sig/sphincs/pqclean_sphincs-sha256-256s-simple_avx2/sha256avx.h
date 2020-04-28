#ifndef SHA256AVX_H
#define SHA256AVX_H

#include <immintrin.h>
#include <stdint.h>

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

typedef struct SHA256state {
    u256 s[8];
    uint8_t msgblocks[8 * 64];
    unsigned int datalen;
    uint64_t msglen;
} sha256ctxx8;


void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_transpose(u256 s[8]);
void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_sha256_init_frombytes_x8(sha256ctxx8 *ctx, const uint8_t *s, unsigned long long msglen);
void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_sha256_init8x(sha256ctxx8 *ctx);
void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_sha256_update8x(sha256ctxx8 *ctx,
        const unsigned char *d0,
        const unsigned char *d1,
        const unsigned char *d2,
        const unsigned char *d3,
        const unsigned char *d4,
        const unsigned char *d5,
        const unsigned char *d6,
        const unsigned char *d7,
        unsigned long long len);
void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_sha256_final8x(sha256ctxx8 *ctx,
        unsigned char *out0,
        unsigned char *out1,
        unsigned char *out2,
        unsigned char *out3,
        unsigned char *out4,
        unsigned char *out5,
        unsigned char *out6,
        unsigned char *out7);

void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_sha256_transform8x(sha256ctxx8 *ctx, const unsigned char *data);

void PQCLEAN_SPHINCSSHA256256SSIMPLE_AVX2_sha256_clone_statex8(sha256ctxx8 *outctx, const sha256ctxx8 *inctx);


#endif
