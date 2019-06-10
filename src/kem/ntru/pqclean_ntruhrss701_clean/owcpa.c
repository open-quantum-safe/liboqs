#include "owcpa.h"
#include "poly.h"
#include "sample.h"

static int owcpa_check_r(const poly *r) {
    /* Check that r is in message space. */
    /* Note: Assumes that r has coefficients in {0, 1, ..., q-1} */
    int i;
    uint64_t t = 0;
    uint16_t c;
    for (i = 0; i < NTRU_N; i++) {
        c = MODQ(r->coeffs[i] + 1);
        t |= c & (NTRU_Q - 4); /* 0 if c is in {0,1,2,3} */
        t |= (c + 1) & 0x4;   /* 0 if c is in {0,1,2} */
    }
    t |= r->coeffs[NTRU_N - 1]; /* Coefficient n-1 must be zero */
    t = (~t + 1); // two's complement
    t >>= 63;
    return (int) t;
}

void PQCLEAN_NTRUHRSS701_CLEAN_owcpa_samplemsg(unsigned char msg[NTRU_OWCPA_MSGBYTES],
        const unsigned char seed[NTRU_SAMPLE_RM_BYTES]) {
    poly r, m;

    PQCLEAN_NTRUHRSS701_CLEAN_sample_rm(&r, &m, seed);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(msg, &r);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(msg + NTRU_PACK_TRINARY_BYTES, &m);
}

void PQCLEAN_NTRUHRSS701_CLEAN_owcpa_keypair(unsigned char *pk,
        unsigned char *sk,
        const unsigned char seed[NTRU_SAMPLE_FG_BYTES]) {
    int i;

    poly x1, x2, x3, x4, x5;

    poly *f = &x1, *invf_mod3 = &x2;
    poly *g = &x3, *G = &x2;
    poly *Gf = &x3, *invGf = &x4, *tmp = &x5;
    poly *invh = &x3, *h = &x3;

    PQCLEAN_NTRUHRSS701_CLEAN_sample_fg(f, g, seed);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_inv(invf_mod3, f);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(sk, f);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(sk + NTRU_PACK_TRINARY_BYTES, invf_mod3);

    /* Lift coeffs of f and g from Z_p to Z_q */
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Z3_to_Zq(f);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Z3_to_Zq(g);

    /* G = 3*(x-1)*g */
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul_x_minus_1(G, g);
    for (i = 0; i < NTRU_N; i++) {
        G->coeffs[i] = MODQ(3 * G->coeffs[i]);
    }

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(Gf, G, f);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_inv(invGf, Gf);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(tmp, invGf, f);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_mul(invh, tmp, f);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_tobytes(sk + 2 * NTRU_PACK_TRINARY_BYTES, invh);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(tmp, invGf, G);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(h, tmp, G);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_sum_zero_tobytes(pk, h);
}


void PQCLEAN_NTRUHRSS701_CLEAN_owcpa_enc(unsigned char *c,
        const unsigned char *rm,
        const unsigned char *pk) {
    int i;
    poly x1, x2, x3;
    poly *h = &x1, *liftm = &x1;
    poly *r = &x2, *m = &x2;
    poly *ct = &x3;

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_sum_zero_frombytes(h, pk);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_frombytes(r, rm);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Z3_to_Zq(r);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(ct, r, h);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_frombytes(m, rm + NTRU_PACK_TRINARY_BYTES);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_lift(liftm, m);
    for (i = 0; i < NTRU_N; i++) {
        ct->coeffs[i] = MODQ(ct->coeffs[i] + liftm->coeffs[i]);
    }

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_sum_zero_tobytes(c, ct);
}

int PQCLEAN_NTRUHRSS701_CLEAN_owcpa_dec(unsigned char *rm,
                                        const unsigned char *ciphertext,
                                        const unsigned char *secretkey) {
    int i;
    int fail;
    poly x1, x2, x3, x4;

    poly *c = &x1, *f = &x2, *cf = &x3;
    poly *mf = &x2, *finv3 = &x3, *m = &x4;
    poly *liftm = &x2, *invh = &x3, *r = &x4;
    poly *b = &x1;

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_sum_zero_frombytes(c, ciphertext);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_frombytes(f, secretkey);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Z3_to_Zq(f);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(cf, c, f);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_to_S3(mf, cf);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_frombytes(finv3, secretkey + NTRU_PACK_TRINARY_BYTES);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_mul(m, mf, finv3);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(rm + NTRU_PACK_TRINARY_BYTES, m);

    /* NOTE: For the IND-CCA2 KEM we must ensure that c = Enc(h, (r,m)).       */
    /* We can avoid re-computing r*h + Lift(m) as long as we check that        */
    /* r (defined as b/h mod (q, Phi_n)) and m are in the message space.       */
    /* (m can take any value in S3 in NTRU_HRSS) */
    fail = 0;

    /* b = c - Lift(m) mod (q, x^n - 1) */
    PQCLEAN_NTRUHRSS701_CLEAN_poly_lift(liftm, m);
    for (i = 0; i < NTRU_N; i++) {
        b->coeffs[i] = MODQ(c->coeffs[i] - liftm->coeffs[i]);
    }

    /* r = b / h mod (q, Phi_n) */
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_frombytes(invh, secretkey + 2 * NTRU_PACK_TRINARY_BYTES);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_mul(r, b, invh);

    /* NOTE: Our definition of r as b/h mod (q, Phi_n) follows Figure 4 of     */
    /*   [Sch18] https://eprint.iacr.org/2018/1174/20181203:032458.            */
    /* This differs from Figure 10 of Saito--Xagawa--Yamakawa                  */
    /*   [SXY17] https://eprint.iacr.org/2017/1005/20180516:055500             */
    /* where r gets a final reduction modulo p.                                */
    /* We need this change to use Proposition 1 of [Sch18].                    */

    /* Proposition 1 of [Sch18] shows that re-encryption with (r,m) yields c.  */
    /* if and only if fail==0 after the following call to owcpa_check_r        */
    /* The procedure given in Fig. 8 of [Sch18] can be skipped because we have */
    /* c(1) = 0 due to the use of poly_Rq_sum_zero_{to,from}bytes.             */
    fail |= owcpa_check_r(r);

    PQCLEAN_NTRUHRSS701_CLEAN_poly_trinary_Zq_to_Z3(r);
    PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(rm, r);

    return fail;
}
