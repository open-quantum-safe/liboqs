#include "indcpa.h"

#include "ntt.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "symmetric.h"

#include <stdint.h>

/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   unsigned char *r:          pointer to the output serialized public key
*              const poly *pk:            pointer to the input public-key polynomial
*              const unsigned char *seed: pointer to the input public seed
**************************************************/
static void pack_pk(unsigned char *r, polyvec *pk, const unsigned char *seed) {
    int i;
    PQCLEAN_KYBER1024_CLEAN_polyvec_tobytes(r, pk);
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        r[i + KYBER_POLYVECBYTES] = seed[i];
    }
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk:                   pointer to output public-key vector of polynomials
*              - unsigned char *seed:           pointer to output seed to generate matrix A
*              - const unsigned char *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk(polyvec *pk, unsigned char *seed, const unsigned char *packedpk) {
    int i;
    PQCLEAN_KYBER1024_CLEAN_polyvec_frombytes(pk, packedpk);
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        seed[i] = packedpk[i + KYBER_POLYVECBYTES];
    }
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - unsigned char *r:  pointer to output serialized secret key
*              - const polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk(unsigned char *r, polyvec *sk) {
    PQCLEAN_KYBER1024_CLEAN_polyvec_tobytes(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key;
*              inverse of pack_sk
*
* Arguments:   - polyvec *sk:                   pointer to output vector of polynomials (secret key)
*              - const unsigned char *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk(polyvec *sk, const unsigned char *packedsk) {
    PQCLEAN_KYBER1024_CLEAN_polyvec_frombytes(sk, packedsk);
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   unsigned char *r:          pointer to the output serialized ciphertext
*              const poly *pk:            pointer to the input vector of polynomials b
*              const unsigned char *seed: pointer to the input polynomial v
**************************************************/
static void pack_ciphertext(unsigned char *r, polyvec *b, poly *v) {
    PQCLEAN_KYBER1024_CLEAN_polyvec_compress(r, b);
    PQCLEAN_KYBER1024_CLEAN_poly_compress(r + KYBER_POLYVECCOMPRESSEDBYTES, v);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b:             pointer to the output vector of polynomials b
*              - poly *v:                pointer to the output polynomial v
*              - const unsigned char *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext(polyvec *b, poly *v, const unsigned char *c) {
    PQCLEAN_KYBER1024_CLEAN_polyvec_decompress(b, c);
    PQCLEAN_KYBER1024_CLEAN_poly_decompress(v, c + KYBER_POLYVECCOMPRESSEDBYTES);
}

/*************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   - int16_t *r:               pointer to output buffer
*              - unsigned int len:         requested number of 16-bit integers (uniform mod q)
*              - const unsigned char *buf: pointer to input buffer (assumed to be uniform random bytes)
*              - unsigned int buflen:      length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
**************************************************/
static unsigned int rej_uniform(int16_t *r, unsigned int len, const unsigned char *buf, unsigned int buflen) {
    unsigned int ctr, pos;
    uint16_t val;

    ctr = pos = 0;
    while (ctr < len && pos + 2 <= buflen) {
        val = buf[pos] | ((uint16_t)buf[pos + 1] << 8);
        pos += 2;

        if (val < 19 * KYBER_Q) {
            val -= (val >> 12) * KYBER_Q; // Barrett reduction
            r[ctr++] = (int16_t)val;
        }
    }

    return ctr;
}

#define gen_a(A,B)  PQCLEAN_KYBER1024_CLEAN_gen_matrix(A,B,0)
#define gen_at(A,B) PQCLEAN_KYBER1024_CLEAN_gen_matrix(A,B,1)

#define GENMATRIX_MAXNBLOCKS ((530 + XOF_BLOCKBYTES) / XOF_BLOCKBYTES) /* 530 is expected number of required bytes */


/*************************************************
* Name:        gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   - polyvec *a:                pointer to ouptput matrix A
*              - const unsigned char *seed: pointer to input seed
*              - int transposed:            boolean deciding whether A or A^T is generated
**************************************************/
static void PQCLEAN_KYBER1024_CLEAN_gen_matrix(polyvec *a, const unsigned char *seed, int transposed) {
    unsigned int ctr;
    unsigned char i, j;
    unsigned char buf[XOF_BLOCKBYTES * GENMATRIX_MAXNBLOCKS + 1];
    xof_state state;

    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_K; j++) {
            if (transposed) {
                xof_absorb(&state, seed, i, j);
            } else {
                xof_absorb(&state, seed, j, i);
            }

            xof_squeezeblocks(buf, GENMATRIX_MAXNBLOCKS, &state);
            ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, GENMATRIX_MAXNBLOCKS * XOF_BLOCKBYTES);

            while (ctr < KYBER_N) {
                xof_squeezeblocks(buf, 1, &state);
                ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, XOF_BLOCKBYTES);
            }
        }
    }
}

/*************************************************
* Name:        indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - unsigned char *pk: pointer to output public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec a[KYBER_K], e, pkpv, skpv;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    int i;
    unsigned char nonce = 0;

    randombytes(buf, KYBER_SYMBYTES);
    hash_g(buf, buf, KYBER_SYMBYTES);

    gen_a(a, publicseed);

    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_getnoise(skpv.vec + i, noiseseed, nonce++);
    }
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_getnoise(e.vec + i, noiseseed, nonce++);
    }

    PQCLEAN_KYBER1024_CLEAN_polyvec_ntt(&skpv);
    PQCLEAN_KYBER1024_CLEAN_polyvec_ntt(&e);

    // matrix-vector multiplication
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_polyvec_pointwise_acc(&pkpv.vec[i], &a[i], &skpv);
        PQCLEAN_KYBER1024_CLEAN_poly_frommont(&pkpv.vec[i]);
    }

    PQCLEAN_KYBER1024_CLEAN_polyvec_add(&pkpv, &pkpv, &e);
    PQCLEAN_KYBER1024_CLEAN_polyvec_reduce(&pkpv);

    pack_sk(sk, &skpv);
    pack_pk(pk, &pkpv, publicseed);
}

/*************************************************
* Name:        indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - unsigned char *c:          pointer to output ciphertext (of length KYBER_INDCPA_BYTES bytes)
*              - const unsigned char *m:    pointer to input message (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const unsigned char *pk:   pointer to input public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - const unsigned char *coin: pointer to input random coins used as seed (of length KYBER_SYMBYTES bytes)
*                                           to deterministically generate all randomness
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_indcpa_enc(unsigned char *c,
                                        const unsigned char *m,
                                        const unsigned char *pk,
                                        const unsigned char *coins) {
    polyvec sp, pkpv, ep, at[KYBER_K], bp;
    poly v, k, epp;
    unsigned char seed[KYBER_SYMBYTES];
    int i;
    unsigned char nonce = 0;

    unpack_pk(&pkpv, seed, pk);
    PQCLEAN_KYBER1024_CLEAN_poly_frommsg(&k, m);
    gen_at(at, seed);

    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_getnoise(sp.vec + i, coins, nonce++);
    }
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_getnoise(ep.vec + i, coins, nonce++);
    }
    PQCLEAN_KYBER1024_CLEAN_poly_getnoise(&epp, coins, nonce++);

    PQCLEAN_KYBER1024_CLEAN_polyvec_ntt(&sp);

    // matrix-vector multiplication
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_polyvec_pointwise_acc(&bp.vec[i], &at[i], &sp);
    }

    PQCLEAN_KYBER1024_CLEAN_polyvec_pointwise_acc(&v, &pkpv, &sp);

    PQCLEAN_KYBER1024_CLEAN_polyvec_invntt(&bp);
    PQCLEAN_KYBER1024_CLEAN_poly_invntt(&v);

    PQCLEAN_KYBER1024_CLEAN_polyvec_add(&bp, &bp, &ep);
    PQCLEAN_KYBER1024_CLEAN_poly_add(&v, &v, &epp);
    PQCLEAN_KYBER1024_CLEAN_poly_add(&v, &v, &k);
    PQCLEAN_KYBER1024_CLEAN_polyvec_reduce(&bp);
    PQCLEAN_KYBER1024_CLEAN_poly_reduce(&v);

    pack_ciphertext(c, &bp, &v);
}

/*************************************************
* Name:        indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - unsigned char *m:        pointer to output decrypted message (of length KYBER_INDCPA_MSGBYTES)
*              - const unsigned char *c:  pointer to input ciphertext (of length KYBER_INDCPA_BYTES)
*              - const unsigned char *sk: pointer to input secret key (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_indcpa_dec(unsigned char *m,
                                        const unsigned char *c,
                                        const unsigned char *sk) {
    polyvec bp, skpv;
    poly v, mp;

    unpack_ciphertext(&bp, &v, c);
    unpack_sk(&skpv, sk);

    PQCLEAN_KYBER1024_CLEAN_polyvec_ntt(&bp);
    PQCLEAN_KYBER1024_CLEAN_polyvec_pointwise_acc(&mp, &skpv, &bp);
    PQCLEAN_KYBER1024_CLEAN_poly_invntt(&mp);

    PQCLEAN_KYBER1024_CLEAN_poly_sub(&mp, &v, &mp);
    PQCLEAN_KYBER1024_CLEAN_poly_reduce(&mp);

    PQCLEAN_KYBER1024_CLEAN_poly_tomsg(m, &mp);
}
