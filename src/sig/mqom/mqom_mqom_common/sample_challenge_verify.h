#ifndef __SAMPLE_CHALLENGE_VERIFY_H__
#define __SAMPLE_CHALLENGE_VERIFY_H__

#include "sample_challenge_common.h"
#include "blc.h"

/* Given precomputed material and a candidate nonce, check the AES grinding condition:
 *   Truncate_{W-1}(Enc(k0||01, g0||nonce) XOR Enc(k1||11, g1||nonce)) == 0
 * Used only by the scalar DeriveChallenge below: the batched DeriveChallenge_x2
 * (sample_challenge_sign.h) does its own interleaved encryption and check inline. */
static int grinding_aes_check_nonce(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                    const uint8_t nonce[4],
                                    const grinding_material_t *mat,
                                    uint16_t i_star[MQOM3_PARAM_TAU])
{
    int ret;
    uint8_t pt0[MQOM3_PARAM_SEED_SIZE], pt1[MQOM3_PARAM_SEED_SIZE];
    uint8_t ct0[MQOM3_PARAM_SEED_SIZE], ct1[MQOM3_PARAM_SEED_SIZE];
    uint32_t xor_trunc;
    int bi;
    xof_context xof_ctx = { 0 };
    uint8_t tmp[MQOM3_PARAM_TAU * 2];
    int e;

    /* Plaintexts: g_j (fixed part) || Bits_ell(nonce) (GRIND_NONCE_BYTES bytes, LE). */
    memcpy(pt0, mat->g0, MQOM3_PARAM_GRIND_PTXT_BYTES);
    memcpy(pt0 + MQOM3_PARAM_GRIND_PTXT_BYTES, nonce, MQOM3_PARAM_GRIND_NONCE_BYTES);
    memcpy(pt1, mat->g1, MQOM3_PARAM_GRIND_PTXT_BYTES);
    memcpy(pt1 + MQOM3_PARAM_GRIND_PTXT_BYTES, nonce, MQOM3_PARAM_GRIND_NONCE_BYTES);

    /* Encrypt c0 = Enc(k0||01, pt0) and c1 = Enc(k1||11, pt1) with interleaved AES rounds. */
    ret = enc_encrypt_x2_pub(&mat->k0_ctx, &mat->k1_ctx, pt0, pt1, ct0, ct1);
    ERR(ret, err);

    /* Truncate_{w-1}(c0 XOR c1) == 0: check the first (W-1) bits of the XOR. */
    xor_trunc = 0;
    for (bi = 0; bi < MQOM3_PARAM_GRIND_TRUNC_BYTES; bi++) {
        xor_trunc |= (uint32_t)(ct0[bi] ^ ct1[bi]) << (8 * bi);
    }
    /* W == 0: GRIND_TRUNC_BYTES is already 0, so the loop above never ran and
     * xor_trunc is still 0 - the mask below would be a no-op anyway, but its
     * (W - 1) shift count is undefined behavior for W == 0, so skip it. */
#if (MQOM3_PARAM_W > 0)
    xor_trunc &= ((uint32_t)1u << (MQOM3_PARAM_W - 1)) - 1u;
#endif
    if (xor_trunc != 0) { ret = -1; goto err; }

    /* Nonce passes the AES check; derive i* via XOF_6(sig_id, nonce, c0, c1) if requested. */
    if (i_star == NULL) { ret = 0; goto err; }

    ret = xof_init(&xof_ctx);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_CHALLENGE, 1);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, sig_id, MQOM3_PARAM_DIGEST_SIZE);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, nonce, MQOM3_PARAM_GRIND_NONCE_BYTES);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, ct0, MQOM3_PARAM_SEED_SIZE);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, ct1, MQOM3_PARAM_SEED_SIZE);
    ERR(ret, err);
    ret = xof_squeeze(&xof_ctx, tmp, MQOM3_PARAM_TAU * 2);
    ERR(ret, err);
    for (e = 0; e < MQOM3_PARAM_TAU; e++) {
        i_star[e] = ((uint16_t)tmp[2 * e] + (uint16_t)tmp[2 * e + 1] * 256)
                  & (uint16_t)((1 << MQOM3_PARAM_NB_EVALS_LOG) - 1);
    }
    ret = 0;

err:
    xof_clean_ctx(&xof_ctx);
    return ret;
}

/* DeriveChallenge (spec, AES path): run the AES grinding check for one nonce value.
 * Implements the algorithm from the spec: Truncate_{w-1}(c0 XOR c1) = 0,
 * then derive i* via XOF_6 and verify BLC validity.
 * Returns 0 and fills i_star if i* != bot; -1 otherwise.
 */
static int DeriveChallenge(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                           const uint8_t nonce[4],
                           uint16_t i_star[MQOM3_PARAM_TAU],
                           grinding_material_t *mat,
                           int *mat_precomputed)
{
    int v;
    if (!(*mat_precomputed)) {
        /* -2, not -1: a failure to derive the grinding material is a hard error */
        if (grinding_aes_derive(sig_id, mat) != 0) { return -2; }
        *mat_precomputed = 1;
    }
    v = grinding_aes_check_nonce(sig_id, nonce, mat, i_star);
    return ((v == 0) && BLC_IsValidChallenge(i_star)) ? 0 : -1;
}

#endif /* __SAMPLE_CHALLENGE_VERIFY_H__ */
