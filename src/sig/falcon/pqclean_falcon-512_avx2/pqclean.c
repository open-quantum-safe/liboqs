#include "api.h"
#include "inner.h"
#include "randombytes.h"
#include <stddef.h>
#include <string.h>
/*
 * Wrapper for implementing the PQClean API.
 */



#define NONCELEN   40
#define SEEDLEN    48

/*
 * Encoding formats (nnnn = log of degree, 9 for Falcon-512, 10 for Falcon-1024)
 *
 *   private key:
 *      header byte: 0101nnnn
 *      private f  (6 or 5 bits by element, depending on degree)
 *      private g  (6 or 5 bits by element, depending on degree)
 *      private F  (8 bits by element)
 *
 *   public key:
 *      header byte: 0000nnnn
 *      public h   (14 bits by element)
 *
 *   signature:
 *      header byte: 0011nnnn
 *      nonce     40 bytes
 *      value     (12 bits by element)
 *
 *   message + signature:
 *      signature length   (2 bytes, big-endian)
 *      nonce              40 bytes
 *      message
 *      header byte:       0010nnnn
 *      value              (12 bits by element)
 *      (signature length is 1+len(value), not counting the nonce)
 */

/* see api.h */
int
PQCLEAN_FALCON512_AVX2_crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    union {
        uint8_t b[FALCON_KEYGEN_TEMP_9];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    int8_t f[512], g[512], F[512];
    uint16_t h[512];
    unsigned char seed[SEEDLEN];
    inner_shake256_context rng;
    size_t u, v;

    /*
     * Generate key pair.
     */
    randombytes(seed, sizeof seed);
    inner_shake256_init(&rng);
    inner_shake256_inject(&rng, seed, sizeof seed);
    inner_shake256_flip(&rng);
    PQCLEAN_FALCON512_AVX2_keygen(&rng, f, g, F, NULL, h, 9, tmp.b);
    inner_shake256_ctx_release(&rng);

    /*
     * Encode private key.
     */
    sk[0] = 0x50 + 9;
    u = 1;
    v = PQCLEAN_FALCON512_AVX2_trim_i8_encode(
            sk + u, PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES - u,
            f, 9, PQCLEAN_FALCON512_AVX2_max_fg_bits[9]);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = PQCLEAN_FALCON512_AVX2_trim_i8_encode(
            sk + u, PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES - u,
            g, 9, PQCLEAN_FALCON512_AVX2_max_fg_bits[9]);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = PQCLEAN_FALCON512_AVX2_trim_i8_encode(
            sk + u, PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES - u,
            F, 9, PQCLEAN_FALCON512_AVX2_max_FG_bits[9]);
    if (v == 0) {
        return -1;
    }
    u += v;
    if (u != PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES) {
        return -1;
    }

    /*
     * Encode public key.
     */
    pk[0] = 0x00 + 9;
    v = PQCLEAN_FALCON512_AVX2_modq_encode(
            pk + 1, PQCLEAN_FALCON512_AVX2_CRYPTO_PUBLICKEYBYTES - 1,
            h, 9);
    if (v != PQCLEAN_FALCON512_AVX2_CRYPTO_PUBLICKEYBYTES - 1) {
        return -1;
    }

    return 0;
}

/*
 * Compute the signature. nonce[] receives the nonce and must have length
 * NONCELEN bytes. sigbuf[] receives the signature value (without nonce
 * or header byte), with *sigbuflen providing the maximum value length and
 * receiving the actual value length.
 *
 * If a signature could be computed but not encoded because it would
 * exceed the output buffer size, then a new signature is computed. If
 * the provided buffer size is too low, this could loop indefinitely, so
 * the caller must provide a size that can accommodate signatures with a
 * large enough probability.
 *
 * Return value: 0 on success, -1 on error.
 */
static int
do_sign(uint8_t *nonce, uint8_t *sigbuf, size_t *sigbuflen,
        const uint8_t *m, size_t mlen, const uint8_t *sk) {
    union {
        uint8_t b[72 * 512];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    int8_t f[512], g[512], F[512], G[512];
    union {
        int16_t sig[512];
        uint16_t hm[512];
    } r;
    unsigned char seed[SEEDLEN];
    inner_shake256_context sc;
    size_t u, v;

    /*
     * Decode the private key.
     */
    if (sk[0] != 0x50 + 9) {
        return -1;
    }
    u = 1;
    v = PQCLEAN_FALCON512_AVX2_trim_i8_decode(
            f, 9, PQCLEAN_FALCON512_AVX2_max_fg_bits[9],
            sk + u, PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = PQCLEAN_FALCON512_AVX2_trim_i8_decode(
            g, 9, PQCLEAN_FALCON512_AVX2_max_fg_bits[9],
            sk + u, PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = PQCLEAN_FALCON512_AVX2_trim_i8_decode(
            F, 9, PQCLEAN_FALCON512_AVX2_max_FG_bits[9],
            sk + u, PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    if (u != PQCLEAN_FALCON512_AVX2_CRYPTO_SECRETKEYBYTES) {
        return -1;
    }
    if (!PQCLEAN_FALCON512_AVX2_complete_private(G, f, g, F, 9, tmp.b)) {
        return -1;
    }

    /*
     * Create a random nonce (40 bytes).
     */
    randombytes(nonce, NONCELEN);

    /*
     * Hash message nonce + message into a vector.
     */
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, nonce, NONCELEN);
    inner_shake256_inject(&sc, m, mlen);
    inner_shake256_flip(&sc);
    PQCLEAN_FALCON512_AVX2_hash_to_point_ct(&sc, r.hm, 9, tmp.b);
    inner_shake256_ctx_release(&sc);

    /*
     * Initialize a RNG.
     */
    randombytes(seed, sizeof seed);
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, seed, sizeof seed);
    inner_shake256_flip(&sc);

    /*
     * Compute and return the signature. This loops until a signature
     * value is found that fits in the provided buffer.
     */
    for (;;) {
        PQCLEAN_FALCON512_AVX2_sign_dyn(r.sig, &sc, f, g, F, G, r.hm, 9, tmp.b);
        v = PQCLEAN_FALCON512_AVX2_comp_encode(sigbuf, *sigbuflen, r.sig, 9);
        if (v != 0) {
            inner_shake256_ctx_release(&sc);
            *sigbuflen = v;
            return 0;
        }
    }
}

/*
 * Verify a sigature. The nonce has size NONCELEN bytes. sigbuf[]
 * (of size sigbuflen) contains the signature value, not including the
 * header byte or nonce. Return value is 0 on success, -1 on error.
 */
static int
do_verify(
    const uint8_t *nonce, const uint8_t *sigbuf, size_t sigbuflen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {
    union {
        uint8_t b[2 * 512];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    uint16_t h[512], hm[512];
    int16_t sig[512];
    inner_shake256_context sc;

    /*
     * Decode public key.
     */
    if (pk[0] != 0x00 + 9) {
        return -1;
    }
    if (PQCLEAN_FALCON512_AVX2_modq_decode(h, 9,
                                           pk + 1, PQCLEAN_FALCON512_AVX2_CRYPTO_PUBLICKEYBYTES - 1)
            != PQCLEAN_FALCON512_AVX2_CRYPTO_PUBLICKEYBYTES - 1) {
        return -1;
    }
    PQCLEAN_FALCON512_AVX2_to_ntt_monty(h, 9);

    /*
     * Decode signature.
     */
    if (sigbuflen == 0) {
        return -1;
    }
    if (PQCLEAN_FALCON512_AVX2_comp_decode(sig, 9, sigbuf, sigbuflen) != sigbuflen) {
        return -1;
    }

    /*
     * Hash nonce + message into a vector.
     */
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, nonce, NONCELEN);
    inner_shake256_inject(&sc, m, mlen);
    inner_shake256_flip(&sc);
    PQCLEAN_FALCON512_AVX2_hash_to_point_ct(&sc, hm, 9, tmp.b);
    inner_shake256_ctx_release(&sc);

    /*
     * Verify signature.
     */
    if (!PQCLEAN_FALCON512_AVX2_verify_raw(hm, sig, h, 9, tmp.b)) {
        return -1;
    }
    return 0;
}

/* see api.h */
int
PQCLEAN_FALCON512_AVX2_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    /*
     * The PQCLEAN_FALCON512_AVX2_CRYPTO_BYTES constant is used for
     * the signed message object (as produced by PQCLEAN_FALCON512_AVX2_crypto_sign())
     * and includes a two-byte length value, so we take care here
     * to only generate signatures that are two bytes shorter than
     * the maximum. This is done to ensure that PQCLEAN_FALCON512_AVX2_crypto_sign()
     * and PQCLEAN_FALCON512_AVX2_crypto_sign_signature() produce the exact same signature
     * value, if used on the same message, with the same private key,
     * and using the same output from randombytes() (this is for
     * reproducibility of tests).
     */
    size_t vlen;

    vlen = PQCLEAN_FALCON512_AVX2_CRYPTO_BYTES - NONCELEN - 3;
    if (do_sign(sig + 1, sig + 1 + NONCELEN, &vlen, m, mlen, sk) < 0) {
        return -1;
    }
    sig[0] = 0x30 + 9;
    *siglen = 1 + NONCELEN + vlen;
    return 0;
}

/* see api.h */
int
PQCLEAN_FALCON512_AVX2_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {
    if (siglen < 1 + NONCELEN) {
        return -1;
    }
    if (sig[0] != 0x30 + 9) {
        return -1;
    }
    return do_verify(sig + 1,
                     sig + 1 + NONCELEN, siglen - 1 - NONCELEN, m, mlen, pk);
}

/* see api.h */
int
PQCLEAN_FALCON512_AVX2_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    uint8_t *pm, *sigbuf;
    size_t sigbuflen;

    /*
     * Move the message to its final location; this is a memmove() so
     * it handles overlaps properly.
     */
    memmove(sm + 2 + NONCELEN, m, mlen);
    pm = sm + 2 + NONCELEN;
    sigbuf = pm + 1 + mlen;
    sigbuflen = PQCLEAN_FALCON512_AVX2_CRYPTO_BYTES - NONCELEN - 3;
    if (do_sign(sm + 2, sigbuf, &sigbuflen, pm, mlen, sk) < 0) {
        return -1;
    }
    pm[mlen] = 0x20 + 9;
    sigbuflen ++;
    sm[0] = (uint8_t)(sigbuflen >> 8);
    sm[1] = (uint8_t)sigbuflen;
    *smlen = mlen + 2 + NONCELEN + sigbuflen;
    return 0;
}

/* see api.h */
int
PQCLEAN_FALCON512_AVX2_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk) {
    const uint8_t *sigbuf;
    size_t pmlen, sigbuflen;

    if (smlen < 3 + NONCELEN) {
        return -1;
    }
    sigbuflen = ((size_t)sm[0] << 8) | (size_t)sm[1];
    if (sigbuflen < 2 || sigbuflen > (smlen - NONCELEN - 2)) {
        return -1;
    }
    sigbuflen --;
    pmlen = smlen - NONCELEN - 3 - sigbuflen;
    if (sm[2 + NONCELEN + pmlen] != 0x20 + 9) {
        return -1;
    }
    sigbuf = sm + 2 + NONCELEN + pmlen + 1;

    /*
     * The 2-byte length header and the one-byte signature header
     * have been verified. Nonce is at sm+2, followed by the message
     * itself. Message length is in pmlen. sigbuf/sigbuflen point to
     * the signature value (excluding the header byte).
     */
    if (do_verify(sm + 2, sigbuf, sigbuflen,
                  sm + 2 + NONCELEN, pmlen, pk) < 0) {
        return -1;
    }

    /*
     * Signature is correct, we just have to copy/move the message
     * to its final destination. The memmove() properly handles
     * overlaps.
     */
    memmove(m, sm + 2 + NONCELEN, pmlen);
    *mlen = pmlen;
    return 0;
}
