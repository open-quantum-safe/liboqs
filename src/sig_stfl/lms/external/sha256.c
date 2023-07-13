/*
 * SHA-256
 * Implementation derived from LibTomCrypt (Tom St Denis)
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtomcrypt.org
 */

#include <string.h>
#include "sha256.h"
#include "endian.h"

#if !USE_OPENSSL && !defined(EXT_SHA256_H)

/* If we don't have OpenSSL, here's a SHA256 implementation */
#define SHA256_FINALCOUNT_SIZE  8
#define SHA256_K_SIZE	        64
static const unsigned long K[SHA256_K_SIZE] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
    0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
    0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
    0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
    0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
    0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
    0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
    0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
    0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Various logical functions */

/* Rotate x right by rot bits */
static unsigned long RORc(unsigned long x, int rot) {
    rot &= 31; if (rot == 0) return x;
    unsigned long right = ((x&0xFFFFFFFFUL)>>rot );
    unsigned long left  = ((x&0xFFFFFFFFUL)<<(32-rot) );
    return (right|left) & 0xFFFFFFFFUL;
}
#define Ch(x,y,z)       (z ^ (x & (y ^ z)))
#define Maj(x,y,z)      (((x | y) & z) | (x & y))
#define S(x, n)         RORc((x),(n))
#define R(x, n)         (((x)&0xFFFFFFFFUL)>>(n))
#define Sigma0(x)       (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x)       (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x)       (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x)       (S(x, 17) ^ S(x, 19) ^ R(x, 10))

static void sha256_compress (SHA256_CTX * ctx, const void *buf)
{
    unsigned long S0, S1, S2, S3, S4, S5, S6, S7, W[SHA256_K_SIZE], t0, t1, t;
    int i;
    const unsigned char *p;

    /* copy state into S */
    S0 = ctx->h[0];
    S1 = ctx->h[1];
    S2 = ctx->h[2];
    S3 = ctx->h[3];
    S4 = ctx->h[4];
    S5 = ctx->h[5];
    S6 = ctx->h[6];
    S7 = ctx->h[7];

    /*
     * We've been asked to perform the hash computation on this 512-bit string.
     * SHA256 interprets that as an array of 16 bigendian 32 bit numbers; copy
     * it, and convert it into 16 unsigned long's of the CPU's native format
     */
    p = buf;
    for (i=0; i<16; i++) {
        W[i] = get_bigendian( p, 4 );
        p += 4;
    }

    /* fill W[16..63] */
    for (i = 16; i < SHA256_K_SIZE; i++) {
        W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
    }

    /* Compress */
#define RND(a,b,c,d,e,f,g,h,i)                         \
     t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];   \
     t1 = Sigma0(a) + Maj(a, b, c);                    \
     d += t0;                                          \
     h  = t0 + t1;

     for (i = 0; i < SHA256_K_SIZE; ++i) {
         RND(S0,S1,S2,S3,S4,S5,S6,S7,i);
         t = S7; S7 = S6; S6 = S5; S5 = S4;
         S4 = S3; S3 = S2; S2 = S1; S1 = S0; S0 = t;
     }
#undef RND

    /* feedback */
    ctx->h[0] += S0;
    ctx->h[1] += S1;
    ctx->h[2] += S2;
    ctx->h[3] += S3;
    ctx->h[4] += S4;
    ctx->h[5] += S5;
    ctx->h[6] += S6;
    ctx->h[7] += S7;
}

void SHA256_Init (SHA256_CTX *ctx)
{
    ctx->Nl = 0;
    ctx->Nh = 0;
    ctx->num = 0;
    ctx->h[0] = 0x6A09E667UL;
    ctx->h[1] = 0xBB67AE85UL;
    ctx->h[2] = 0x3C6EF372UL;
    ctx->h[3] = 0xA54FF53AUL;
    ctx->h[4] = 0x510E527FUL;
    ctx->h[5] = 0x9B05688CUL;
    ctx->h[6] = 0x1F83D9ABUL;
    ctx->h[7] = 0x5BE0CD19UL;
}

void SHA256_Update (SHA256_CTX *ctx, const void *src, unsigned int count)
{
    unsigned new_count = (ctx->Nl + (count << 3)) & 0xffffffff;
    if (new_count < ctx->Nl) {
        ctx->Nh += 1;
    }
    ctx->Nl = new_count;

    while (count) {
        unsigned int this_step = 64 - ctx->num;
        if (this_step > count) this_step = count;
        memcpy( ctx->data + ctx->num, src, this_step);

        if (this_step + ctx->num < 64) {
            ctx->num += this_step;
            break;
        }

        src = (const unsigned char *)src + this_step;
        count -= this_step;
        ctx->num = 0;

        sha256_compress( ctx, ctx->data );
    }
}

/*
 * Add padding and return the message digest.
 */
void SHA256_Final (unsigned char *digest, SHA256_CTX *ctx)
{
    unsigned int i;
    unsigned char finalcount[SHA256_FINALCOUNT_SIZE];

    put_bigendian( &finalcount[0], ctx->Nh, 4 );
    put_bigendian( &finalcount[4], ctx->Nl, 4 );

    SHA256_Update(ctx, "\200", 1);

    if (ctx->num > 56) {
        SHA256_Update(ctx, "\0\0\0\0\0\0\0\0", 8);
    }
    memset( ctx->data + ctx->num, 0, 56 - ctx->num );
    ctx->num = 56;
    SHA256_Update(ctx, finalcount, SHA256_FINALCOUNT_SIZE);  /* Should cause a sha256_compress() */

    /*
     * The final state is an array of unsigned long's; place them as a series
     * of bigendian 4-byte words onto the output
     */
    for (i=0; i<8; i++) {
        put_bigendian( digest + 4*i, ctx->h[i], 4 );
    }
}
#endif
