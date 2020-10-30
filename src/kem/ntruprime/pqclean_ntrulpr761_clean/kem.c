#include "api.h"
#include "crypto_sort_uint32.h"
#include "crypto_stream_aes256ctr.h"
#include "params.h"
#include "randombytes.h"
#include "sha2.h"



#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t

/* ----- masks */

/* return -1 if x<0; otherwise return 0 */
static int int16_negative_mask(int16 x) {
    uint16 u = (uint16) x;
    u >>= 15;
    return -(int) u;
    /* alternative with gcc -fwrapv: */
    /* x>>15 compiles to CPU's arithmetic right shift */
}

/* ----- arithmetic mod 3 */

typedef int8 small;
/* F3 is always represented as -1,0,1 */

/* ----- arithmetic mod q */

#define q12 ((q-1)/2)
typedef int16 Fq;

/* works for -14000000 < x < 14000000 if q in 4591, 4621, 5167 */
/* assumes twos complement; use, e.g., gcc -fwrapv */
static Fq Fq_freeze(int32 x) {
    x -= (int32) (q * ((q18 * x) >> 18));
    x -= (int32) (q * ((q27 * x + 67108864) >> 27));
    return (Fq) x;
}

/* works for all uint32 x */
static Fq Fq_bigfreeze(uint32 x) {
    x -= (uint32) (q * ((x * (uint64)q31) >> 31));
    x -= (uint32) (q * ((x * (uint64)q31) >> 31));
    x -= q;
    x += (~(x >> 31) + 1) & (uint32)q;
    return (Fq) x;
}

/* ----- Top and Right */

static int8 Top(Fq C) {
    return (int8) ((tau1 * (int32)(C + tau0) + 16384) >> 15);
}

static Fq Right(int8 T) {
    return Fq_freeze(tau3 * (int32)T - tau2);
}

/* ----- polynomials mod q */

/* h = h*g in the ring Rq */
static void Rq_mult_small(Fq *h, const small *g) {
    crypto_encode_pxint16((unsigned char *) h, h);
    crypto_core_mult((unsigned char *) h, (const unsigned char *) h, (const unsigned char *) g);
    crypto_decode_pxint16(h, (const unsigned char *) h);
}

/* ----- sorting to generate short polynomial */

static void Short_fromlist(small *out, const uint32 *in) {
    uint32 L[ppadsort];
    int i;

    for (i = 0; i < w; ++i) {
        L[i] = in[i] & (uint32) - 2;
    }
    for (i = w; i < p; ++i) {
        L[i] = (in[i] & (uint32) - 3) | 1;
    }
    for (i = p; i < ppadsort; ++i) {
        L[i] = 0xffffffff;
    }
    PQCLEAN_NTRULPR761_CLEAN_crypto_sort_uint32(L, ppadsort);
    for (i = 0; i < p; ++i) {
        out[i] = (small) ((L[i] & 3) - 1);
    }
}

/* ----- underlying hash function */

#define Hash_bytes 32

static void Hash(unsigned char *out, const unsigned char *in, int inlen) {
    unsigned char h[64];
    int i;
    sha512(h, in, (size_t) inlen);
    for (i = 0; i < 32; ++i) {
        out[i] = h[i];
    }
}

/* ----- higher-level randomness */

static void Short_random(small *out) {
    uint32 L[p];

    randombytes((unsigned char *) L, sizeof L);
    crypto_decode_pxint32(L, (unsigned char *) L);
    Short_fromlist(out, L);
}

/* ----- Inputs, Generator */

typedef int8 Inputs[I]; /* passed by reference */

static const unsigned char aes_nonce[16] = {0};

/* G = Generator(pk) */
static void Generator(Fq *G, const unsigned char *pk) {
    uint32 L[p];
    int i;

    PQCLEAN_NTRULPR761_CLEAN_crypto_stream_aes256ctr((unsigned char *) L, 4 * p, aes_nonce, pk);
    crypto_decode_pxint32(L, (unsigned char *) L);
    for (i = 0; i < p; ++i) {
        G[i] = Fq_bigfreeze(L[i]) - q12;
    }
}

/* ----- NTRU LPRime */

#define Seeds_bytes 32
#define Ciphertexts_bytes (Rounded_bytes+Top_bytes)
#define SecretKeys_bytes Small_bytes
#define PublicKeys_bytes (Seeds_bytes+Rounded_bytes)
#define Confirm_bytes 32

/* c,r_enc[1:] = Hide(r,pk,cache); cache is Hash4(pk) */
static void Hide(unsigned char *c, unsigned char *r_enc, const Inputs r, const unsigned char *pk, const unsigned char *cache) {
    small b[p];
    int i;

    Inputs_encode(r_enc + 1, r);
    {
        unsigned char h[Hash_bytes];
        uint32 L[p];
        {
            unsigned char s[1 + Inputs_bytes];
            Inputs_encode(s + 1, r);
            s[0] = 5;
            Hash(h, s, sizeof s);
        }
        PQCLEAN_NTRULPR761_CLEAN_crypto_stream_aes256ctr((unsigned char *) L, 4 * p, aes_nonce, h);
        crypto_decode_pxint32(L, (unsigned char *) L);
        Short_fromlist(b, L);
    }
    {
        Fq bG[p];
        Generator(bG, pk);
        Rq_mult_small(bG, b);
        Round_and_encode(c, bG);
        c += Rounded_bytes;
    }
    {
        Fq bA[p];
        int8 T[I];
        Rounded_decode(bA, pk + Seeds_bytes);
        Rq_mult_small(bA, b);
        for (i = 0; i < I; ++i) {
            T[i] = Top(Fq_freeze(bA[i] + r[i] * q12));
        }
        Top_encode(c, T);
        c += Top_bytes;
    }
    {
        unsigned char x[1 + Inputs_bytes + Hash_bytes];
        for (i = 0; i < Inputs_bytes; ++i) {
            x[1 + i] = r_enc[1 + i];
        }
        for (i = 0; i < Hash_bytes; ++i) {
            x[1 + Inputs_bytes + i] = cache[i];
        }
        x[0] = 2;
        Hash(c, x, sizeof x);
    }
}


int PQCLEAN_NTRULPR761_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    Fq aG[p];
    int i;
    randombytes(pk, Seeds_bytes);
    Generator(aG, pk);
    {
        small a[p];
        Short_random(a);
        Rq_mult_small(aG, a);
        Small_encode(sk, a);
    }
    Round_and_encode(pk + Seeds_bytes, aG);
    {
        unsigned char sksave = sk[SecretKeys_bytes - 1];
        for (i = 0; i < PublicKeys_bytes; ++i) {
            sk[SecretKeys_bytes + i] = pk[i];
        }
        sk[SecretKeys_bytes - 1] = 4;
        Hash(sk + SecretKeys_bytes + PublicKeys_bytes + Inputs_bytes, sk + SecretKeys_bytes - 1, 1 + PublicKeys_bytes);
        sk[SecretKeys_bytes - 1] = sksave;
        randombytes(sk + SecretKeys_bytes + PublicKeys_bytes, Inputs_bytes);
    }
    return 0;
}

int PQCLEAN_NTRULPR761_CLEAN_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk) {
    int i;
    unsigned char cache[Hash_bytes];
    {
        unsigned char y[1 + PublicKeys_bytes];
        for (i = 0; i < PublicKeys_bytes; ++i) {
            y[1 + i] = pk[i];
        }
        y[0] = 4;
        Hash(cache, y, sizeof y);
    }
    Inputs r;
    {
        unsigned char s[Inputs_bytes];
        randombytes(s, sizeof s);
        Inputs_decode(r, s);
    }
    {
        unsigned char x[1 + Inputs_bytes + Ciphertexts_bytes + Confirm_bytes];
        Hide(c, x, r, pk, cache);
        for (i = 0; i < Ciphertexts_bytes + Confirm_bytes; ++i) {
            x[1 + Inputs_bytes + i] = c[i];
        }
        x[0] = 1;
        Hash(k, x, sizeof x);
    }
    return 0;
}

int PQCLEAN_NTRULPR761_CLEAN_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk) {
    const unsigned char *pk = sk + SecretKeys_bytes;
    const unsigned char *rho = pk + PublicKeys_bytes;
    const unsigned char *cache = rho + Inputs_bytes;
    Inputs r;
    int i;
    {
        Fq aB[p];
        Rounded_decode(aB, c);
        {
            small a[p];
            Small_decode(a, sk);
            Rq_mult_small(aB, a);
        }
        {
            int8 T[I];
            Top_decode(T, c + Rounded_bytes);
            for (i = 0; i < I; ++i) {
                r[i] = (int8) - int16_negative_mask(Fq_freeze(Right(T[i]) - aB[i] + 4 * w + 1));
            }
        }
    }
    {
        unsigned char cnew[Ciphertexts_bytes + Confirm_bytes];
        int mask;
        unsigned char x[1 + Inputs_bytes + Ciphertexts_bytes + Confirm_bytes];
        Hide(cnew, x, r, pk, cache);
        mask = crypto_verify_clen(c, cnew);
        for (i = 0; i < Inputs_bytes; ++i) {
            x[1 + i] ^= (unsigned char) (mask & (x[1 + i] ^ rho[i]));
        }
        for (i = 0; i < Ciphertexts_bytes + Confirm_bytes; ++i) {
            x[1 + Inputs_bytes + i] = c[i];
        }
        x[0] = (unsigned char) (1 + mask);
        Hash(k, x, sizeof x);
    }
    return 0;
}
