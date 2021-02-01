#include "align.h"
#include "fips202.h"
#include "packing.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "sign.h"
#include "symmetric.h"
#include <stdint.h>
#include <string.h>

static inline void polyvec_matrix_expand_row(polyvecl **row, polyvecl buf[2], const uint8_t rho[SEEDBYTES], unsigned int i) {
    switch (i) {
    case 0:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row0(buf, buf + 1, rho);
        *row = buf;
        break;
    case 1:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row1(buf + 1, buf, rho);
        *row = buf + 1;
        break;
    case 2:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row2(buf, buf + 1, rho);
        *row = buf;
        break;
    case 3:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row3(buf + 1, buf, rho);
        *row = buf + 1;
        break;
    case 4:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row4(buf, buf + 1, rho);
        *row = buf;
        break;
    case 5:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row5(buf + 1, buf, rho);
        *row = buf + 1;
        break;
    case 6:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row6(buf, buf + 1, rho);
        *row = buf;
        break;
    case 7:
        PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand_row7(buf + 1, buf, rho);
        *row = buf + 1;
        break;
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM5_AVX2_crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - uint8_t *pk: pointer to output public key (allocated
*                             array of PQCLEAN_DILITHIUM5_AVX2_CRYPTO_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key (allocated
*                             array of PQCLEAN_DILITHIUM5_AVX2_CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_DILITHIUM5_AVX2_crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
    unsigned int i;
    uint8_t seedbuf[3 * SEEDBYTES];
    const uint8_t *rho, *rhoprime, *key;
    polyvecl rowbuf[2];
    polyvecl s1, *row = rowbuf;
    polyveck s2;
    poly t1, t0;

    /* Get randomness for rho, rhoprime and key */
    randombytes(seedbuf, SEEDBYTES);
    shake256(seedbuf, 3 * SEEDBYTES, seedbuf, SEEDBYTES);
    rho = seedbuf;
    rhoprime = seedbuf + SEEDBYTES;
    key = seedbuf + 2 * SEEDBYTES;

    /* Store rho, key */
    memcpy(pk, rho, SEEDBYTES);
    memcpy(sk, rho, SEEDBYTES);
    memcpy(sk + SEEDBYTES, key, SEEDBYTES);

    /* Sample short vectors s1 and s2 */
    PQCLEAN_DILITHIUM5_AVX2_poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime, 0, 1, 2, 3);
    PQCLEAN_DILITHIUM5_AVX2_poly_uniform_eta_4x(&s1.vec[4], &s1.vec[5], &s1.vec[6], &s2.vec[0], rhoprime, 4, 5, 6, 7);
    PQCLEAN_DILITHIUM5_AVX2_poly_uniform_eta_4x(&s2.vec[1], &s2.vec[2], &s2.vec[3], &s2.vec[4], rhoprime, 8, 9, 10, 11);
    PQCLEAN_DILITHIUM5_AVX2_poly_uniform_eta_4x(&s2.vec[5], &s2.vec[6], &s2.vec[7], &t0, rhoprime, 12, 13, 14, 15);

    /* Pack secret vectors */
    for (i = 0; i < L; i++) {
        PQCLEAN_DILITHIUM5_AVX2_polyeta_pack(sk + 2 * SEEDBYTES + CRHBYTES + i * POLYETA_PACKEDBYTES, &s1.vec[i]);
    }
    for (i = 0; i < K; i++) {
        PQCLEAN_DILITHIUM5_AVX2_polyeta_pack(sk + 2 * SEEDBYTES + CRHBYTES + (L + i)*POLYETA_PACKEDBYTES, &s2.vec[i]);
    }

    /* Transform s1 */
    PQCLEAN_DILITHIUM5_AVX2_polyvecl_ntt(&s1);


    for (i = 0; i < K; i++) {
        /* Expand matrix row */
        polyvec_matrix_expand_row(&row, rowbuf, rho, i);

        /* Compute inner-product */
        PQCLEAN_DILITHIUM5_AVX2_polyvecl_pointwise_acc_montgomery(&t1, row, &s1);
        PQCLEAN_DILITHIUM5_AVX2_poly_invntt_tomont(&t1);

        /* Add error polynomial */
        PQCLEAN_DILITHIUM5_AVX2_poly_add(&t1, &t1, &s2.vec[i]);

        /* Round t and pack t1, t0 */
        PQCLEAN_DILITHIUM5_AVX2_poly_caddq(&t1);
        PQCLEAN_DILITHIUM5_AVX2_poly_power2round(&t1, &t0, &t1);
        PQCLEAN_DILITHIUM5_AVX2_polyt1_pack(pk + SEEDBYTES + i * POLYT1_PACKEDBYTES, &t1);
        PQCLEAN_DILITHIUM5_AVX2_polyt0_pack(sk + 2 * SEEDBYTES + CRHBYTES + (L + K)*POLYETA_PACKEDBYTES + i * POLYT0_PACKEDBYTES, &t0);
    }

    /* Compute CRH(rho, t1) and store in secret key */
    crh(sk + 2 * SEEDBYTES, pk, PQCLEAN_DILITHIUM5_AVX2_CRYPTO_PUBLICKEYBYTES);

    return 0;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM5_AVX2_crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - uint8_t *sig: pointer to output signature (of length PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES)
*              - size_t *siglen: pointer to output length of signature
*              - uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_DILITHIUM5_AVX2_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk) {
    unsigned int i, n, pos;
    uint8_t seedbuf[2 * SEEDBYTES + 3 * CRHBYTES];
    uint8_t *rho, *tr, *key, *mu, *rhoprime;
    uint8_t hintbuf[N];
    uint8_t *hint = sig + SEEDBYTES + L * POLYZ_PACKEDBYTES;
    uint64_t nonce = 0;
    polyvecl mat[K], s1, z;
    polyveck t0, s2, w1;
    poly c, tmp;
    union {
        polyvecl y;
        polyveck w0;
    } tmpv;
    shake256incctx state;

    rho = seedbuf;
    tr = rho + SEEDBYTES;
    key = tr + CRHBYTES;
    mu = key + SEEDBYTES;
    rhoprime = mu + CRHBYTES;
    PQCLEAN_DILITHIUM5_AVX2_unpack_sk(rho, tr, key, &t0, &s1, &s2, sk);

    /* Compute CRH(tr, msg) */
    shake256_inc_init(&state);
    shake256_inc_absorb(&state, tr, CRHBYTES);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(mu, CRHBYTES, &state);
    shake256_inc_ctx_release(&state);

    crh(rhoprime, key, SEEDBYTES + CRHBYTES);

    /* Expand matrix and transform vectors */
    PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_expand(mat, rho);
    PQCLEAN_DILITHIUM5_AVX2_polyvecl_ntt(&s1);
    PQCLEAN_DILITHIUM5_AVX2_polyveck_ntt(&s2);
    PQCLEAN_DILITHIUM5_AVX2_polyveck_ntt(&t0);


rej:
    /* Sample intermediate vector y */
    PQCLEAN_DILITHIUM5_AVX2_poly_uniform_gamma1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &z.vec[3],
            rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
    PQCLEAN_DILITHIUM5_AVX2_poly_uniform_gamma1_4x(&z.vec[4], &z.vec[5], &z.vec[6], &tmp,
            rhoprime, nonce + 4, nonce + 5, nonce + 6, 0);
    nonce += 7;

    /* Matrix-vector product */
    tmpv.y = z;
    PQCLEAN_DILITHIUM5_AVX2_polyvecl_ntt(&tmpv.y);
    PQCLEAN_DILITHIUM5_AVX2_polyvec_matrix_pointwise_montgomery(&w1, mat, &tmpv.y);
    PQCLEAN_DILITHIUM5_AVX2_polyveck_invntt_tomont(&w1);

    /* Decompose w and call the random oracle */
    PQCLEAN_DILITHIUM5_AVX2_polyveck_caddq(&w1);
    PQCLEAN_DILITHIUM5_AVX2_polyveck_decompose(&w1, &tmpv.w0, &w1);
    PQCLEAN_DILITHIUM5_AVX2_polyveck_pack_w1(sig, &w1);

    shake256_inc_init(&state);
    shake256_inc_absorb(&state, mu, CRHBYTES);
    shake256_inc_absorb(&state, sig, K * POLYW1_PACKEDBYTES);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(sig, SEEDBYTES, &state);
    shake256_inc_ctx_release(&state);
    PQCLEAN_DILITHIUM5_AVX2_poly_challenge(&c, sig);
    PQCLEAN_DILITHIUM5_AVX2_poly_ntt(&c);

    /* Compute z, reject if it reveals secret */
    for (i = 0; i < L; i++) {
        PQCLEAN_DILITHIUM5_AVX2_poly_pointwise_montgomery(&tmp, &c, &s1.vec[i]);
        PQCLEAN_DILITHIUM5_AVX2_poly_invntt_tomont(&tmp);
        PQCLEAN_DILITHIUM5_AVX2_poly_add(&z.vec[i], &z.vec[i], &tmp);
        PQCLEAN_DILITHIUM5_AVX2_poly_reduce(&z.vec[i]);
        if (PQCLEAN_DILITHIUM5_AVX2_poly_chknorm(&z.vec[i], GAMMA1 - BETA)) {
            goto rej;
        }
    }

    /* Zero hint vector in signature */
    pos = 0;
    memset(hint, 0, OMEGA);

    for (i = 0; i < K; i++) {
        /* Check that subtracting cs2 does not change high bits of w and low bits
         * do not reveal secret information */
        PQCLEAN_DILITHIUM5_AVX2_poly_pointwise_montgomery(&tmp, &c, &s2.vec[i]);
        PQCLEAN_DILITHIUM5_AVX2_poly_invntt_tomont(&tmp);
        PQCLEAN_DILITHIUM5_AVX2_poly_sub(&tmpv.w0.vec[i], &tmpv.w0.vec[i], &tmp);
        PQCLEAN_DILITHIUM5_AVX2_poly_reduce(&tmpv.w0.vec[i]);
        if (PQCLEAN_DILITHIUM5_AVX2_poly_chknorm(&tmpv.w0.vec[i], GAMMA2 - BETA)) {
            goto rej;
        }

        /* Compute hints */
        PQCLEAN_DILITHIUM5_AVX2_poly_pointwise_montgomery(&tmp, &c, &t0.vec[i]);
        PQCLEAN_DILITHIUM5_AVX2_poly_invntt_tomont(&tmp);
        PQCLEAN_DILITHIUM5_AVX2_poly_reduce(&tmp);
        if (PQCLEAN_DILITHIUM5_AVX2_poly_chknorm(&tmp, GAMMA2)) {
            goto rej;
        }

        PQCLEAN_DILITHIUM5_AVX2_poly_add(&tmpv.w0.vec[i], &tmpv.w0.vec[i], &tmp);
        n = PQCLEAN_DILITHIUM5_AVX2_poly_make_hint(hintbuf, &tmpv.w0.vec[i], &w1.vec[i]);
        if (pos + n > OMEGA) {
            goto rej;
        }

        /* Store hints in signature */
        memcpy(&hint[pos], hintbuf, n);
        hint[OMEGA + i] = pos = pos + n;
    }

    /* Pack z into signature */
    for (i = 0; i < L; i++) {
        PQCLEAN_DILITHIUM5_AVX2_polyz_pack(sig + SEEDBYTES + i * POLYZ_PACKEDBYTES, &z.vec[i]);
    }

    *siglen = PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES;
    return 0;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM5_AVX2_crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   - uint8_t *sm: pointer to output signed message (allocated
*                             array with PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES + mlen bytes),
*                             can be equal to m
*              - size_t *smlen: pointer to output length of signed
*                               message
*              - const uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - const uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_DILITHIUM5_AVX2_crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen, const uint8_t *sk) {
    size_t i;

    for (i = 0; i < mlen; ++i) {
        sm[PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
    }
    PQCLEAN_DILITHIUM5_AVX2_crypto_sign_signature(sm, smlen, sm + PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES, mlen, sk);
    *smlen += mlen;
    return 0;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM5_AVX2_crypto_sign_verify
*
* Description: Verifies signature.
*
* Arguments:   - uint8_t *m: pointer to input signature
*              - size_t siglen: length of signature
*              - const uint8_t *m: pointer to message
*              - size_t mlen: length of message
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
**************************************************/
int PQCLEAN_DILITHIUM5_AVX2_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk) {
    unsigned int i, j, pos = 0;
    /* PQCLEAN_DILITHIUM5_AVX2_polyw1_pack writes additional 14 bytes */
    ALIGNED_UINT8(K * POLYW1_PACKEDBYTES + 14) buf;
    uint8_t mu[CRHBYTES];
    const uint8_t *hint = sig + SEEDBYTES + L * POLYZ_PACKEDBYTES;
    polyvecl rowbuf[2];
    polyvecl *row = rowbuf;
    polyvecl z;
    poly c, w1, h;
    shake256incctx state;

    if (siglen != PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES) {
        return -1;
    }

    /* Compute CRH(CRH(rho, t1), msg) */
    crh(mu, pk, PQCLEAN_DILITHIUM5_AVX2_CRYPTO_PUBLICKEYBYTES);
    shake256_inc_init(&state);
    shake256_inc_absorb(&state, mu, CRHBYTES);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(mu, CRHBYTES, &state);
    shake256_inc_ctx_release(&state);

    /* Expand PQCLEAN_DILITHIUM5_AVX2_challenge */
    PQCLEAN_DILITHIUM5_AVX2_poly_challenge(&c, sig);
    PQCLEAN_DILITHIUM5_AVX2_poly_ntt(&c);

    /* Unpack z; shortness follows from unpacking */
    for (i = 0; i < L; i++) {
        PQCLEAN_DILITHIUM5_AVX2_polyz_unpack(&z.vec[i], sig + SEEDBYTES + i * POLYZ_PACKEDBYTES);
        PQCLEAN_DILITHIUM5_AVX2_poly_ntt(&z.vec[i]);
    }


    for (i = 0; i < K; i++) {
        /* Expand matrix row */
        polyvec_matrix_expand_row(&row, rowbuf, pk, i);

        /* Compute i-th row of Az - c2^Dt1 */
        PQCLEAN_DILITHIUM5_AVX2_polyvecl_pointwise_acc_montgomery(&w1, row, &z);

        PQCLEAN_DILITHIUM5_AVX2_polyt1_unpack(&h, pk + SEEDBYTES + i * POLYT1_PACKEDBYTES);
        PQCLEAN_DILITHIUM5_AVX2_poly_shiftl(&h);
        PQCLEAN_DILITHIUM5_AVX2_poly_ntt(&h);
        PQCLEAN_DILITHIUM5_AVX2_poly_pointwise_montgomery(&h, &c, &h);

        PQCLEAN_DILITHIUM5_AVX2_poly_sub(&w1, &w1, &h);
        PQCLEAN_DILITHIUM5_AVX2_poly_reduce(&w1);
        PQCLEAN_DILITHIUM5_AVX2_poly_invntt_tomont(&w1);

        /* Get hint polynomial and reconstruct w1 */
        memset(h.vec, 0, sizeof(poly));
        if (hint[OMEGA + i] < pos || hint[OMEGA + i] > OMEGA) {
            return -1;
        }

        for (j = pos; j < hint[OMEGA + i]; ++j) {
            /* Coefficients are ordered for strong unforgeability */
            if (j > pos && hint[j] <= hint[j - 1]) {
                return -1;
            }
            h.coeffs[hint[j]] = 1;
        }
        pos = hint[OMEGA + i];

        PQCLEAN_DILITHIUM5_AVX2_poly_caddq(&w1);
        PQCLEAN_DILITHIUM5_AVX2_poly_use_hint(&w1, &w1, &h);
        PQCLEAN_DILITHIUM5_AVX2_polyw1_pack(buf.coeffs + i * POLYW1_PACKEDBYTES, &w1);
    }

    /* Extra indices are zero for strong unforgeability */
    for (j = pos; j < OMEGA; ++j) {
        if (hint[j]) {
            return -1;
        }
    }

    /* Call random oracle and verify PQCLEAN_DILITHIUM5_AVX2_challenge */
    shake256_inc_init(&state);
    shake256_inc_absorb(&state, mu, CRHBYTES);
    shake256_inc_absorb(&state, buf.coeffs, K * POLYW1_PACKEDBYTES);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(buf.coeffs, SEEDBYTES, &state);
    shake256_inc_ctx_release(&state);
    for (i = 0; i < SEEDBYTES; ++i) {
        if (buf.coeffs[i] != sig[i]) {
            return -1;
        }
    }

    return 0;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM5_AVX2_crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   - uint8_t *m: pointer to output message (allocated
*                            array with smlen bytes), can be equal to sm
*              - size_t *mlen: pointer to output length of message
*              - const uint8_t *sm: pointer to signed message
*              - size_t smlen: length of signed message
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
**************************************************/
int PQCLEAN_DILITHIUM5_AVX2_crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen, const uint8_t *pk) {
    size_t i;

    if (smlen < PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES) {
        goto badsig;
    }

    *mlen = smlen - PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES;
    if (PQCLEAN_DILITHIUM5_AVX2_crypto_sign_verify(sm, PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES, sm + PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES, *mlen, pk)) {
        goto badsig;
    } else {
        /* All good, copy msg, return 0 */
        for (i = 0; i < *mlen; ++i) {
            m[i] = sm[PQCLEAN_DILITHIUM5_AVX2_CRYPTO_BYTES + i];
        }
        return 0;
    }

badsig:
    /* Signature verification failed */
    *mlen = -1;
    for (i = 0; i < smlen; ++i) {
        m[i] = 0;
    }

    return -1;
}
