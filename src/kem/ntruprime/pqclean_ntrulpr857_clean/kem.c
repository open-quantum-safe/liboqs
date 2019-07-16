#include <stdint.h>

#include "api.h"

#include "params.h"

#include "aes.h"
#include "randombytes.h"
#include "sha2.h"

#include "decode.h"
#include "encode.h"
#include "int32.h"
#include "uint32.h"

/* ----- masks */

/* return -1 if x<0; otherwise return 0 */
static int int16_negative_mask(int16_t x) {
    uint16_t u = x;
    u >>= 15;
    return -(int) u;
    /* alternative with gcc -fwrapv: */
    /* x>>15 compiles to CPU's arithmetic right shift */
}

/* ----- arithmetic mod 3 */

/* F3 is always represented as -1,0,1 */
/* so ZZ_fromF3 is a no-op */

/* x must not be close to top int16 */
static int8_t F3_freeze(int16_t x) {
    return (int8_t) (PQCLEAN_NTRULPR857_CLEAN_int32_mod_uint14(x + 1, 3) - 1);
}

/* ----- arithmetic mod q */

#define q12 ((q-1)/2)
typedef int16_t Fq;
/* always represented as -q12...q12 */
/* so ZZ_fromFq is a no-op */

/* x must not be close to top int32 */
static Fq Fq_freeze(int32_t x) {
    return PQCLEAN_NTRULPR857_CLEAN_int32_mod_uint14(x + q12, q) - q12;
}

/* ----- Top and Right */

#define tau 16

static int8_t Top(Fq C) {
    return (tau1 * (int32_t)(C + tau0) + 16384) >> 15;
}

static Fq Right(int8_t T) {
    return Fq_freeze(tau3 * (int32_t)T - tau2);
}

/* ----- polynomials mod q */

/* h = f*g in the ring Rq */
static void Rq_mult_small(Fq *h, const Fq *f, const int8_t *g) {
    Fq fg[p + p - 1];
    Fq result;
    int i, j;

    for (i = 0; i < p; ++i) {
        result = 0;
        for (j = 0; j <= i; ++j) {
            result = Fq_freeze(result + f[j] * (int32_t)g[i - j]);
        }
        fg[i] = result;
    }
    for (i = p; i < p + p - 1; ++i) {
        result = 0;
        for (j = i - p + 1; j < p; ++j) {
            result = Fq_freeze(result + f[j] * (int32_t)g[i - j]);
        }
        fg[i] = result;
    }

    for (i = p + p - 2; i >= p; --i) {
        fg[i - p] = Fq_freeze(fg[i - p] + fg[i]);
        fg[i - p + 1] = Fq_freeze(fg[i - p + 1] + fg[i]);
    }

    for (i = 0; i < p; ++i) {
        h[i] = fg[i];
    }
}

/* ----- rounded polynomials mod q */

static void Round(Fq *out, const Fq *a) {
    int i;
    for (i = 0; i < p; ++i) {
        out[i] = a[i] - F3_freeze(a[i]);
    }
}

/* ----- sorting to generate short polynomial */

static void Short_fromlist(int8_t *out, const uint32_t *in) {
    uint32_t L[p];
    int i;

    for (i = 0; i < w; ++i) {
        L[i] = in[i] & (uint32_t) -2;
    }
    for (i = w; i < p; ++i) {
        L[i] = (in[i] & (uint32_t) -3) | 1;
    }
    PQCLEAN_NTRULPR857_CLEAN_uint32_sort(L, p);
    for (i = 0; i < p; ++i) {
        out[i] = (L[i] & 3) - 1;
    }
}

/* ----- underlying hash function */

#define Hash_bytes 32

static void Hash(unsigned char *out, int b, const unsigned char *in, int inlen);

/* ----- higher-level randomness */

static uint32_t urandom32(void) {
    unsigned char c[4];
    uint32_t out[4];

    randombytes(c, 4);
    out[0] = (uint32_t)c[0];
    out[1] = ((uint32_t)c[1]) << 8;
    out[2] = ((uint32_t)c[2]) << 16;
    out[3] = ((uint32_t)c[3]) << 24;
    return out[0] + out[1] + out[2] + out[3];
}

static void Short_random(int8_t *out) {
    uint32_t L[p];
    int i;

    for (i = 0; i < p; ++i) {
        L[i] = urandom32();
    }
    Short_fromlist(out, L);
}

/* ----- NTRU LPRime Core */

/* (G,A),a = KeyGen(G); leaves G unchanged */
static void KeyGen(Fq *A, int8_t *a, const Fq *G) {
    Fq aG[p];

    Short_random(a);
    Rq_mult_small(aG, G, a);
    Round(A, aG);
}

/* B,T = Encrypt(r,(G,A),b) */
static void Encrypt(Fq *B, int8_t *T, const int8_t *r, const Fq *G, const Fq *A, const int8_t *b) {
    Fq bG[p];
    Fq bA[p];
    int i;

    Rq_mult_small(bG, G, b);
    Round(B, bG);
    Rq_mult_small(bA, A, b);
    for (i = 0; i < I; ++i) {
        T[i] = Top(Fq_freeze(bA[i] + r[i] * q12));
    }
}

/* r = Decrypt((B,T),a) */
static void Decrypt(int8_t *r, const Fq *B, const int8_t *T, const int8_t *a) {
    Fq aB[p];
    int i;

    Rq_mult_small(aB, B, a);
    for (i = 0; i < I; ++i) {
        r[i] = (int8_t) (-int16_negative_mask(Fq_freeze(Right(T[i]) - aB[i] + 4 * w + 1)));
    }
}

/* ----- encoding I-bit inputs */

#define Inputs_bytes (I/8)
typedef int8_t Inputs[I]; /* passed by reference */

static void Inputs_encode(unsigned char *s, const Inputs r) {
    int i;
    for (i = 0; i < Inputs_bytes; ++i) {
        s[i] = 0;
    }
    for (i = 0; i < I; ++i) {
        s[i >> 3] |= r[i] << (i & 7);
    }
}

/* ----- Expand */

static const unsigned char aes_nonce[16] = {0};

static uint32_t le_to_uint32(uint32_t n) {
    return (((uint8_t *) &n)[0] | (((uint8_t *) &n)[1] << 8) | (((uint8_t *) &n)[2] << 16) | (((uint8_t *) &n)[3] << 24));
}

static void Expand(uint32_t *L, const unsigned char *k) {
    aes256ctx ctx;
    aes256_keyexp(&ctx, k);
    aes256_ctr((unsigned char *) L, 4 * p, aes_nonce, &ctx);
    for (size_t i = 0; i < p; i++) {
        L[i] = le_to_uint32(L[i]);
    }
}

/* ----- Seeds */

#define Seeds_bytes 32

static void Seeds_random(unsigned char *s) {
    randombytes(s, Seeds_bytes);
}

/* ----- Generator, HashShort */

/* G = Generator(k) */
static void Generator(Fq *G, const unsigned char *k) {
    uint32_t L[p];
    int i;

    Expand(L, k);
    for (i = 0; i < p; ++i) {
        G[i] = PQCLEAN_NTRULPR857_CLEAN_uint32_mod_uint14(L[i], q) - q12;
    }
}

/* out = HashShort(r) */
static void HashShort(int8_t *out, const Inputs r) {
    unsigned char s[Inputs_bytes];
    unsigned char h[Hash_bytes];
    uint32_t L[p];

    Inputs_encode(s, r);
    Hash(h, 5, s, sizeof s);
    Expand(L, h);
    Short_fromlist(out, L);
}

/* ----- NTRU LPRime Expand */

/* (S,A),a = XKeyGen() */
static void XKeyGen(unsigned char *S, Fq *A, int8_t *a) {
    Fq G[p];

    Seeds_random(S);
    Generator(G, S);
    KeyGen(A, a, G);
}

/* B,T = XEncrypt(r,(S,A)) */
static void XEncrypt(Fq *B, int8_t *T, const int8_t *r, const unsigned char *S, const Fq *A) {
    Fq G[p];
    int8_t b[p];

    Generator(G, S);
    HashShort(b, r);
    Encrypt(B, T, r, G, A, b);
}

#define XDecrypt Decrypt

/* ----- encoding small polynomials (including short polynomials) */

#define Small_bytes ((p+3)/4)

/* these are the only functions that rely on p mod 4 = 1 */

static void Small_encode(unsigned char *s, const int8_t *f) {
    int8_t x;
    int i;

    for (i = 0; i < p / 4; ++i) {
        x = *f++ + 1;
        x += (*f++ + 1) << 2;
        x += (*f++ + 1) << 4;
        x += (*f++ + 1) << 6;
        *s++ = x;
    }
    x = *f++ + 1;
    *s++ = x;
}

static void Small_decode(int8_t *f, const unsigned char *s) {
    unsigned char x;
    int i;

    for (i = 0; i < p / 4; ++i) {
        x = *s++;
        *f++ = ((int8_t)(x & 3)) - 1;
        x >>= 2;
        *f++ = ((int8_t)(x & 3)) - 1;
        x >>= 2;
        *f++ = ((int8_t)(x & 3)) - 1;
        x >>= 2;
        *f++ = ((int8_t)(x & 3)) - 1;
    }
    x = *s++;
    *f++ = ((int8_t)(x & 3)) - 1;
}

/* ----- encoding rounded polynomials */

static void Rounded_encode(unsigned char *s, const Fq *r) {
    uint16_t R[p], M[p];
    int i;

    for (i = 0; i < p; ++i) {
        R[i] = ((r[i] + q12) * 10923) >> 15;
    }
    for (i = 0; i < p; ++i) {
        M[i] = (q + 2) / 3;
    }
    PQCLEAN_NTRULPR857_CLEAN_Encode(s, R, M, p);
}

static void Rounded_decode(Fq *r, const unsigned char *s) {
    uint16_t R[p], M[p];
    int i;

    for (i = 0; i < p; ++i) {
        M[i] = (q + 2) / 3;
    }
    PQCLEAN_NTRULPR857_CLEAN_Decode(R, s, M, p);
    for (i = 0; i < p; ++i) {
        r[i] = R[i] * 3 - q12;
    }
}

/* ----- encoding top polynomials */

#define Top_bytes (I/2)

static void Top_encode(unsigned char *s, const int8_t *T) {
    int i;
    for (i = 0; i < Top_bytes; ++i) {
        s[i] = T[2 * i] + (T[2 * i + 1] << 4);
    }
}

static void Top_decode(int8_t *T, const unsigned char *s) {
    int i;
    for (i = 0; i < Top_bytes; ++i) {
        T[2 * i] = s[i] & 15;
        T[2 * i + 1] = s[i] >> 4;
    }
}

/* ----- NTRU LPRime Expand plus encoding */

#define Ciphertexts_bytes (Rounded_bytes+Top_bytes)
#define SecretKeys_bytes Small_bytes
#define PublicKeys_bytes (Seeds_bytes+Rounded_bytes)

static void Inputs_random(Inputs r) {
    unsigned char s[Inputs_bytes];
    int i;

    randombytes(s, sizeof s);
    for (i = 0; i < I; ++i) {
        r[i] = 1 & (s[i >> 3] >> (i & 7));
    }
}

/* pk,sk = ZKeyGen() */
static void ZKeyGen(unsigned char *pk, unsigned char *sk) {
    Fq A[p];
    int8_t a[p];

    XKeyGen(pk, A, a);
    pk += Seeds_bytes;
    Rounded_encode(pk, A);
    Small_encode(sk, a);
}

/* c = ZEncrypt(r,pk) */
static void ZEncrypt(unsigned char *c, const Inputs r, const unsigned char *pk) {
    Fq A[p];
    Fq B[p];
    int8_t T[I];

    Rounded_decode(A, pk + Seeds_bytes);
    XEncrypt(B, T, r, pk, A);
    Rounded_encode(c, B);
    c += Rounded_bytes;
    Top_encode(c, T);
}

/* r = ZDecrypt(C,sk) */
static void ZDecrypt(Inputs r, const unsigned char *c, const unsigned char *sk) {
    int8_t a[p];
    Fq B[p];
    int8_t T[I];

    Small_decode(a, sk);
    Rounded_decode(B, c);
    Top_decode(T, c + Rounded_bytes);
    XDecrypt(r, B, T, a);
}

/* ----- confirmation hash */

#define Confirm_bytes 32

/* h = HashConfirm(r,pk,cache); cache is Hash4(pk) */
static void HashConfirm(unsigned char *h, const unsigned char *r, const unsigned char *cache) {
    unsigned char x[Inputs_bytes + Hash_bytes];
    int i;

    for (i = 0; i < Inputs_bytes; ++i) {
        x[i] = r[i];
    }
    for (i = 0; i < Hash_bytes; ++i) {
        x[Inputs_bytes + i] = cache[i];
    }
    Hash(h, 2, x, sizeof x);
}

/* ----- session-key hash */

/* k = HashSession(b,y,z) */
static void HashSession(unsigned char *k, int b, const unsigned char *y, const unsigned char *z) {
    unsigned char x[Inputs_bytes + Ciphertexts_bytes + Confirm_bytes];
    int i;

    for (i = 0; i < Inputs_bytes; ++i) {
        x[i] = y[i];
    }
    for (i = 0; i < Ciphertexts_bytes + Confirm_bytes; ++i) {
        x[Inputs_bytes + i] = z[i];
    }
    Hash(k, b, x, sizeof x);
}

/* ----- underlying hash function */

/* e.g., b = 0 means out = Hash0(in) */
static void Hash(unsigned char *out, int b, const unsigned char *in, int inlen) {
    unsigned char x[Inputs_bytes + Ciphertexts_bytes + Confirm_bytes + 1]; // this is the maximum size that Hash is called with
    unsigned char h[64];
    int i;

    x[0] = (unsigned char) b;
    for (i = 0; i < inlen; ++i) {
        x[i + 1] = in[i];
    }
    sha512(h, x, inlen + 1);
    for (i = 0; i < 32; ++i) {
        out[i] = h[i];
    }
}

/* ----- Streamlined NTRU Prime and NTRU LPRime */

/* pk,sk = KEM_KeyGen() */
static void KEM_KeyGen(unsigned char *pk, unsigned char *sk) {
    int i;

    ZKeyGen(pk, sk);
    sk += SecretKeys_bytes;
    for (i = 0; i < PublicKeys_bytes; ++i) {
        *sk++ = pk[i];
    }
    randombytes(sk, Inputs_bytes);
    sk += Inputs_bytes;
    Hash(sk, 4, pk, PublicKeys_bytes);
}

/* c,r_enc = Hide(r,pk,cache); cache is Hash4(pk) */
static void Hide(unsigned char *c, unsigned char *r_enc, const Inputs r, const unsigned char *pk, const unsigned char *cache) {
    Inputs_encode(r_enc, r);
    ZEncrypt(c, r, pk);
    c += Ciphertexts_bytes;
    HashConfirm(c, r_enc, cache);
}

/* c,k = Encap(pk) */
static void Encap(unsigned char *c, unsigned char *k, const unsigned char *pk) {
    Inputs r;
    unsigned char r_enc[Inputs_bytes];
    unsigned char cache[Hash_bytes];

    Hash(cache, 4, pk, PublicKeys_bytes);
    Inputs_random(r);
    Hide(c, r_enc, r, pk, cache);
    HashSession(k, 1, r_enc, c);
}

/* 0 if matching ciphertext+confirm, else -1 */
static int Ciphertexts_diff_mask(const unsigned char *c, const unsigned char *c2) {
    uint16_t differentbits = 0;
    int len = Ciphertexts_bytes + Confirm_bytes;

    while (len-- > 0) {
        differentbits |= (*c++) ^ (*c2++);
    }
    return (1 & ((differentbits - 1) >> 8)) - 1;
}

/* k = Decap(c,sk) */
static void Decap(unsigned char *k, const unsigned char *c, const unsigned char *sk) {
    const unsigned char *pk = sk + SecretKeys_bytes;
    const unsigned char *rho = pk + PublicKeys_bytes;
    const unsigned char *cache = rho + Inputs_bytes;
    Inputs r;
    unsigned char r_enc[Inputs_bytes];
    unsigned char cnew[Ciphertexts_bytes + Confirm_bytes];
    int mask;
    int i;

    ZDecrypt(r, c, sk);
    Hide(cnew, r_enc, r, pk, cache);
    mask = Ciphertexts_diff_mask(c, cnew);
    for (i = 0; i < Inputs_bytes; ++i) {
        r_enc[i] ^= mask & (r_enc[i] ^ rho[i]);
    }
    HashSession(k, 1 + mask, r_enc, c);
}

/* ----- crypto_kem API */

int PQCLEAN_NTRULPR857_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    KEM_KeyGen(pk, sk);
    return 0;
}

int PQCLEAN_NTRULPR857_CLEAN_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk) {
    Encap(c, k, pk);
    return 0;
}

int PQCLEAN_NTRULPR857_CLEAN_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk) {
    Decap(k, c, sk);
    return 0;
}
