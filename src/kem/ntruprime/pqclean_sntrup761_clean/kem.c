#include "api.h"
#include "crypto_declassify.h"
#include "crypto_sort_uint32.h"
#include "params.h"
#include "randombytes.h"
#include "sha2.h"



#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define uint16 uint16_t
#define uint32 uint32_t

/* ----- arithmetic mod 3 */

typedef int8 small;
/* F3 is always represented as -1,0,1 */

/* ----- arithmetic mod q */

typedef int16 Fq;
/* always represented as -(q-1)/2...(q-1)/2 */

/* ----- small polynomials */

/* R3_fromR(R_fromRq(r)) */
static void R3_fromRq(small *out, const Fq *r) {
    crypto_encode_pxfreeze3((unsigned char *) out, (unsigned char *) r);
}

/* h = f*g in the ring R3 */
static void R3_mult(small *h, const small *f, const small *g) {
    crypto_core_mult3((unsigned char *) h, (const unsigned char *) f, (const unsigned char *) g);
}

/* ----- polynomials mod q */

/* h = h*g in the ring Rq */
static void Rq_mult_small(Fq *h, const small *g) {
    crypto_encode_pxint16((unsigned char *) h, h);
    crypto_core_mult((unsigned char *) h, (const unsigned char *) h, (const unsigned char *) g);
    crypto_decode_pxint16(h, (const unsigned char *) h);
}

/* h = 3f in Rq */
static void Rq_mult3(Fq *h, const Fq *f) {
    crypto_encode_pxint16((unsigned char *) h, f);
    crypto_core_scale3((unsigned char *) h, (const unsigned char *) h);
    crypto_decode_pxint16(h, (const unsigned char *) h);
}

/* out = 1/(3*in) in Rq */
/* caller must have 2p+1 bytes free in out, not just 2p */
static void Rq_recip3(Fq *out, const small *in) {
    crypto_core_inv((unsigned char *) out, (const unsigned char *) in);
    /* could check byte 2*p for failure; but, in context, inv always works */
    crypto_decode_pxint16(out, (unsigned char *) out);
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
    uint32 L[ppadsort];
    int i;

    randombytes((unsigned char *) L, 4 * p);
    crypto_decode_pxint32(L, (unsigned char *) L);
    for (i = 0; i < w; ++i) {
        L[i] = L[i] & (uint32) - 2;
    }
    for (i = w; i < p; ++i) {
        L[i] = (L[i] & (uint32) - 3) | 1;
    }
    for (i = p; i < ppadsort; ++i) {
        L[i] = 0xffffffff;
    }
    PQCLEAN_SNTRUP761_CLEAN_crypto_sort_uint32(L, ppadsort);
    for (i = 0; i < p; ++i) {
        out[i] = (small) ((L[i] & 3) - 1);
    }
}

static void Small_random(small *out) {
    uint32 L[p];
    int i;

    randombytes((unsigned char *) L, sizeof L);
    crypto_decode_pxint32(L, (unsigned char *) L);
    for (i = 0; i < p; ++i) {
        out[i] = (small) ((((L[i] & 0x3fffffff) * 3) >> 30) - 1);
    }
}

/* ----- Streamlined NTRU Prime */

typedef small Inputs[p]; /* passed by reference */
#define Ciphertexts_bytes Rounded_bytes
#define SecretKeys_bytes (2*Small_bytes)
#define PublicKeys_bytes Rq_bytes
#define Confirm_bytes 32

/* c,r_enc[1:] = Hide(r,pk,cache); cache is Hash4(pk) */
/* also set r_enc[0]=3 */
/* also set x[0]=2, and x[1:1+Hash_bytes] = Hash3(r_enc) */
/* also overwrite x[1+Hash_bytes:1+2*Hash_bytes] */
static void Hide(unsigned char *x, unsigned char *c, unsigned char *r_enc, const Inputs r, const unsigned char *pk, const unsigned char *cache) {
    Fq h[p];
    int i;

    Small_encode(r_enc + 1, r);
    Rq_decode(h, pk);
    Rq_mult_small(h, r);
    Round_and_encode(c, h);
    r_enc[0] = 3;
    Hash(x + 1, r_enc, 1 + Small_bytes);
    for (i = 0; i < Hash_bytes; ++i) {
        x[1 + Hash_bytes + i] = cache[i];
    }
    x[0] = 2;
    Hash(c + Ciphertexts_bytes, x, 1 + Hash_bytes * 2);
}


int PQCLEAN_SNTRUP761_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    small g[p];
    for (;;) {
        Small_random(g);
        {
            small v[p + 1];
            small vp;
            crypto_core_inv3((unsigned char *) v, (const unsigned char *) g);
            vp = v[p];
            crypto_declassify(&vp, sizeof vp);
            if (vp == 0) {
                Small_encode(sk + Small_bytes, v);
                break;
            }
        }
    }
    {
        small f[p];
        Short_random(f);
        Small_encode(sk, f);
        {
            Fq h[p + 1];
            Rq_recip3(h, f); /* always works */
            Rq_mult_small(h, g);
            Rq_encode(pk, h);
        }
    }
    {
        int i;
        unsigned char sksave = sk[SecretKeys_bytes - 1];
        for (i = 0; i < PublicKeys_bytes; ++i) {
            sk[SecretKeys_bytes + i] = pk[i];
        }
        sk[SecretKeys_bytes - 1] = 4;
        Hash(sk + SecretKeys_bytes + PublicKeys_bytes + Small_bytes, sk + SecretKeys_bytes - 1, 1 + PublicKeys_bytes);
        sk[SecretKeys_bytes - 1] = sksave;
        randombytes(sk + SecretKeys_bytes + PublicKeys_bytes, Small_bytes);
    }
    return 0;
}

int PQCLEAN_SNTRUP761_CLEAN_crypto_kem_enc(uint8_t *c, uint8_t *k, const uint8_t *pk) {
    unsigned char cache[Hash_bytes];
    int i;
    {
        unsigned char y[1 + PublicKeys_bytes]; /* XXX: can eliminate with incremental hashing */
        for (i = 0; i < PublicKeys_bytes; ++i) {
            y[1 + i] = pk[i];
        }
        y[0] = 4;
        Hash(cache, y, sizeof y);
    }
    {
        Inputs r;
        Short_random(r);
        {
            unsigned char r_enc[Small_bytes + 1];
            unsigned char x[1 + Hash_bytes + Ciphertexts_bytes + Confirm_bytes];
            Hide(x, c, r_enc, r, pk, cache);
            for (i = 0; i < Ciphertexts_bytes + Confirm_bytes; ++i) {
                x[1 + Hash_bytes + i] = c[i];
            }
            x[0] = 1;
            Hash(k, x, sizeof x);
        }
    }
    return 0;
}

int PQCLEAN_SNTRUP761_CLEAN_crypto_kem_dec(uint8_t *k, const uint8_t *c, const uint8_t *sk) {
    const unsigned char *pk = sk + SecretKeys_bytes;
    const unsigned char *rho = pk + PublicKeys_bytes;
    const unsigned char *cache = rho + Small_bytes;
    int mask, i;
    Inputs r;
    {
        Fq d[p];
        Rounded_decode(d, c);
        {
            small f[p];
            Small_decode(f, sk);
            Rq_mult_small(d, f);
            Rq_mult3(d, d);
        }
        {
            small e[p];
            small v[p];
            R3_fromRq(e, d);
            Small_decode(v, sk + Small_bytes);
            R3_mult(r, e, v);
        }
        crypto_core_wforce((unsigned char *) r, (unsigned char *) r);
    }
    {
        unsigned char r_enc[1 + Small_bytes];
        unsigned char cnew[Ciphertexts_bytes + Confirm_bytes];
        unsigned char x[1 + Hash_bytes + Ciphertexts_bytes + Confirm_bytes];
        /* XXX: can use incremental hashing to reduce x size */

        Hide(x, cnew, r_enc, r, pk, cache);
        mask = crypto_verify_clen(c, cnew);
        for (i = 0; i < Small_bytes; ++i) {
            r_enc[i + 1] ^= (unsigned char) (mask & (r_enc[i + 1] ^ rho[i]));
        }
        Hash(x + 1, r_enc, 1 + Small_bytes); /* XXX: can instead do cmov on cached hash of rho */
        for (i = 0; i < Ciphertexts_bytes + Confirm_bytes; ++i) {
            x[1 + Hash_bytes + i] = c[i];
        }
        x[0] = (unsigned char) (1 + mask);
        Hash(k, x, sizeof x);
    }
    return 0;
}
