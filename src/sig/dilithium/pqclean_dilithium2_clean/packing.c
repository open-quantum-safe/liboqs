#include "packing.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_pack_pk
*
* Description: Bit-pack public key pk = (rho, t1).
*
* Arguments:   - uint8_t pk[]: output byte array
*              - const uint8_t rho[]: byte array containing rho
*              - const polyveck *t1: pointer to vector t1
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_pack_pk(
    uint8_t pk[PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_PUBLICKEYBYTES],
    const uint8_t rho[SEEDBYTES],
    const polyveck *t1) {
    for (size_t i = 0; i < SEEDBYTES; ++i) {
        pk[i] = rho[i];
    }
    pk += SEEDBYTES;

    for (size_t i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyt1_pack(pk + i * POLT1_SIZE_PACKED, &t1->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_unpack_pk
*
* Description: Unpack public key pk = (rho, t1).
*
* Arguments:   - const uint8_t rho[]: output byte array for rho
*              - const polyveck *t1: pointer to output vector t1
*              - uint8_t pk[]: byte array containing bit-packed pk
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_unpack_pk(
    uint8_t rho[SEEDBYTES],
    polyveck *t1,
    const uint8_t pk[PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_PUBLICKEYBYTES]) {
    for (size_t i = 0; i < SEEDBYTES; ++i) {
        rho[i] = pk[i];
    }
    pk += SEEDBYTES;

    for (size_t i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyt1_unpack(&t1->vec[i], pk + i * POLT1_SIZE_PACKED);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_pack_sk
*
* Description: Bit-pack secret key sk = (rho, key, tr, s1, s2, t0).
*
* Arguments:   - uint8_t sk[]: output byte array
*              - const uint8_t rho[]: byte array containing rho
*              - const uint8_t key[]: byte array containing key
*              - const uint8_t tr[]: byte array containing tr
*              - const polyvecl *s1: pointer to vector s1
*              - const polyveck *s2: pointer to vector s2
*              - const polyveck *t0: pointer to vector t0
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_pack_sk(
    uint8_t sk[PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_SECRETKEYBYTES],
    const uint8_t rho[SEEDBYTES],
    const uint8_t key[SEEDBYTES],
    const uint8_t tr[CRHBYTES],
    const polyvecl *s1,
    const polyveck *s2,
    const polyveck *t0) {
    for (size_t i = 0; i < SEEDBYTES; ++i) {
        sk[i] = rho[i];
    }
    sk += SEEDBYTES;

    for (size_t i = 0; i < SEEDBYTES; ++i) {
        sk[i] = key[i];
    }
    sk += SEEDBYTES;

    for (size_t i = 0; i < CRHBYTES; ++i) {
        sk[i] = tr[i];
    }
    sk += CRHBYTES;

    for (size_t i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyeta_pack(sk + i * POLETA_SIZE_PACKED, &s1->vec[i]);
    }
    sk += L * POLETA_SIZE_PACKED;

    for (size_t i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyeta_pack(sk + i * POLETA_SIZE_PACKED, &s2->vec[i]);
    }
    sk += K * POLETA_SIZE_PACKED;

    for (size_t i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyt0_pack(sk + i * POLT0_SIZE_PACKED, &t0->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_unpack_sk
*
* Description: Unpack secret key sk = (rho, key, tr, s1, s2, t0).
*
* Arguments:   - const uint8_t rho[]: output byte array for rho
*              - const uint8_t key[]: output byte array for key
*              - const uint8_t tr[]: output byte array for tr
*              - const polyvecl *s1: pointer to output vector s1
*              - const polyveck *s2: pointer to output vector s2
*              - const polyveck *r0: pointer to output vector t0
*              - uint8_t sk[]: byte array containing bit-packed sk
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_unpack_sk(
    uint8_t rho[SEEDBYTES],
    uint8_t key[SEEDBYTES],
    uint8_t tr[CRHBYTES],
    polyvecl *s1,
    polyveck *s2,
    polyveck *t0,
    const uint8_t sk[PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_SECRETKEYBYTES]) {
    for (size_t i = 0; i < SEEDBYTES; ++i) {
        rho[i] = sk[i];
    }
    sk += SEEDBYTES;

    for (size_t i = 0; i < SEEDBYTES; ++i) {
        key[i] = sk[i];
    }
    sk += SEEDBYTES;

    for (size_t i = 0; i < CRHBYTES; ++i) {
        tr[i] = sk[i];
    }
    sk += CRHBYTES;

    for (size_t i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyeta_unpack(&s1->vec[i], sk + i * POLETA_SIZE_PACKED);
    }
    sk += L * POLETA_SIZE_PACKED;

    for (size_t i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyeta_unpack(&s2->vec[i], sk + i * POLETA_SIZE_PACKED);
    }
    sk += K * POLETA_SIZE_PACKED;

    for (size_t i = 0; i < K; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyt0_unpack(&t0->vec[i], sk + i * POLT0_SIZE_PACKED);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_pack_sig
*
* Description: Bit-pack signature sig = (z, h, c).
*
* Arguments:   - uint8_t sig[]: output byte array
*              - const polyvecl *z: pointer to vector z
*              - const polyveck *h: pointer to hint vector h
*              - const poly *c: pointer to challenge polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_CLEAN_pack_sig(
    uint8_t sig[PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_SECRETKEYBYTES],
    const polyvecl *z,
    const polyveck *h,
    const poly *c) {
    size_t k;
    uint64_t signs, mask;

    for (size_t i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyz_pack(sig + i * POLZ_SIZE_PACKED, &z->vec[i]);
    }
    sig += L * POLZ_SIZE_PACKED;

    /* Encode h */
    k = 0;
    for (size_t i = 0; i < K; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (h->vec[i].coeffs[j] != 0) {
                sig[k++] = (uint8_t)j;
            }
        }

        sig[OMEGA + i] = (uint8_t)k;
    }
    while (k < OMEGA) {
        sig[k++] = 0;
    }
    sig += OMEGA + K;

    /* Encode c */
    signs = 0;
    mask = 1;
    for (size_t i = 0; i < N / 8; ++i) {
        sig[i] = 0;
        for (size_t j = 0; j < 8; ++j) {
            if (c->coeffs[8 * i + j] != 0) {
                sig[i] |= (uint8_t)(1u << j);
                if (c->coeffs[8 * i + j] == (Q - 1)) {
                    signs |= mask;
                }
                mask <<= 1;
            }
        }
    }
    sig += N / 8;
    for (size_t i = 0; i < 8; ++i) {
        sig[i] = (uint8_t)(signs >> 8u * i);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_unpack_sig
*
* Description: Unpack signature sig = (z, h, c).
*
* Arguments:   - polyvecl *z: pointer to output vector z
*              - polyveck *h: pointer to output hint vector h
*              - poly *c: pointer to output challenge polynomial
*              - const uint8_t sig[]: byte array containing
*                bit-packed signature
*
* Returns 1 in case of malformed signature; otherwise 0.
**************************************************/
int PQCLEAN_DILITHIUM2_CLEAN_unpack_sig(
    polyvecl *z,
    polyveck *h,
    poly *c,
    const uint8_t sig[PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_BYTES]) {
    size_t k;
    uint64_t signs;

    for (size_t i = 0; i < L; ++i) {
        PQCLEAN_DILITHIUM2_CLEAN_polyz_unpack(&z->vec[i], sig + i * POLZ_SIZE_PACKED);
    }
    sig += L * POLZ_SIZE_PACKED;

    /* Decode h */
    k = 0;
    for (size_t i = 0; i < K; ++i) {
        for (size_t j = 0; j < N; ++j) {
            h->vec[i].coeffs[j] = 0;
        }

        if (sig[OMEGA + i] < k || sig[OMEGA + i] > OMEGA) {
            return 1;
        }

        for (size_t j = k; j < sig[OMEGA + i]; ++j) {
            /* Coefficients are ordered for strong unforgeability */
            if (j > k && sig[j] <= sig[j - 1]) {
                return 1;
            }
            h->vec[i].coeffs[sig[j]] = 1;
        }

        k = sig[OMEGA + i];
    }

    /* Extra indices are zero for strong unforgeability */
    for (size_t j = k; j < OMEGA; ++j) {
        if (sig[j]) {
            return 1;
        }
    }

    sig += OMEGA + K;

    /* Decode c */
    for (size_t i = 0; i < N; ++i) {
        c->coeffs[i] = 0;
    }

    signs = 0;
    for (size_t i = 0; i < 8; ++i) {
        signs |= (uint64_t)sig[N / 8 + i] << 8 * i;
    }

    /* Extra sign bits are zero for strong unforgeability */
    if (signs >> 60) {
        return 1;
    }

    for (size_t i = 0; i < N / 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            if ((sig[i] >> j) & 0x01) {
                c->coeffs[8 * i + j] = 1;
                c->coeffs[8 * i + j] ^= -((int32_t) signs & 1) & (1 ^ (Q - 1));
                signs >>= 1;
            }
        }
    }

    return 0;
}
