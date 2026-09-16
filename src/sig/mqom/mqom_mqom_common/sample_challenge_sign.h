#ifndef __SAMPLE_CHALLENGE_SIGN_H__
#define __SAMPLE_CHALLENGE_SIGN_H__

#include "sample_challenge_common.h"

#ifdef SUPERCOP
#include "crypto_declassify.h"
#endif

#define SampleChallenge MQOM_NAMESPACE(SampleChallenge)

/* Only the scalar SampleChallenge fallback (no X4 batching) calls DeriveChallenge
 * directly; the batched DeriveChallenge_x2 below is self-contained on top of
 * sample_challenge_common.h and never needs it. Including it unconditionally
 * would leave DeriveChallenge unused (and warned about) in sign-only translation
 * units built with the default X4 batching. */
#if !defined(AES_GRINDING_X4)
#include "sample_challenge_verify.h"
#endif

#if defined(AES_GRINDING_X4)
#include "blc.h"

/* Three variants below, selected at compile time, all with the same contract:
 * try K nonces per call (K interleaved-AES-batched truncation checks), and if
 * at least one passes, derive i* for it via XOF_6 and validate with
 * BLC_IsValidChallenge. Returns the index in [0,K) of the first valid nonce,
 * -1 if none valid, <-1 on error (-2). That distinction is what stops the
 * SampleChallenge loop from retrying a deterministic failure across the
 * whole nonce space.
 *
 * For CT, BLC_IsValidChallenge is a trivial always-true stub (see blc_ct.h),
 * so the AES truncation check alone gates the search and XOF_6 only ever runs
 * once per SampleChallenge call: DeriveChallenge_x2 (AES-only batching) is
 * the right - and only - variant for CT, unaffected by the two below.
 *
 * For OT, OT_BLC_IsValidChallenge (blc_ot.h) has a real failure probability
 * (it checks the sampled indices form a valid opening set in the shared large
 * GGM tree), so at low w the AES check barely filters anything and the search
 * ends up gated almost entirely by repeated XOF_6 + validity checks. Batching
 * the XOF_6 derivation itself - not just the AES pre-filter - across a group
 * of 4 or 8 nonces (via xof_x4/xof_x8 amortizes the per-call overhead
 * instead of paying it once per candidate nonce, which is where the actual
 * win is at low w. */

#if (MQOM3_PARAM_OT_VARIANT == 1) && defined(USE_XOF_X8)

/* DeriveChallenge_x8: OT, XOF x8-batched. 8 nonces per call: 16 interleaved
 * AES encryptions (two enc_encrypt_x8_pub calls) for the truncation check,
 * then one xof_x8 call to derive i* for all 8 whenever at least one passes. */
static inline int DeriveChallenge_x8(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                     const uint8_t nonces[8][4],
                                     uint16_t i_star[MQOM3_PARAM_TAU],
                                     grinding_material_t *mat,
                                     int *mat_precomputed)
{
    int ret;
    uint8_t pt[16][MQOM3_PARAM_SEED_SIZE];
    uint8_t ct[16][MQOM3_PARAM_SEED_SIZE];
    uint32_t xor_trunc[8];
    int bi, n, e, any_pass;
    xof_context_x8 xof_ctx_x8 = { 0 };
    uint8_t tmp[8][MQOM3_PARAM_TAU * 2];
    uint8_t *tmp_ptr[8] = { tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7] };
    const uint8_t *tag_ptr[8] = { (const uint8_t*) DOMAIN_SEP_CHALLENGE, (const uint8_t*) DOMAIN_SEP_CHALLENGE,
                                   (const uint8_t*) DOMAIN_SEP_CHALLENGE, (const uint8_t*) DOMAIN_SEP_CHALLENGE,
                                   (const uint8_t*) DOMAIN_SEP_CHALLENGE, (const uint8_t*) DOMAIN_SEP_CHALLENGE,
                                   (const uint8_t*) DOMAIN_SEP_CHALLENGE, (const uint8_t*) DOMAIN_SEP_CHALLENGE };
    const uint8_t *sigid_ptr[8] = { sig_id, sig_id, sig_id, sig_id, sig_id, sig_id, sig_id, sig_id };
    const uint8_t *nonce_ptr[8];
    const uint8_t *ct0_ptr[8];
    const uint8_t *ct1_ptr[8];

    if (!(*mat_precomputed)) {
        if (grinding_aes_derive(sig_id, mat) != 0) { return -2; }
        *mat_precomputed = 1;
    }

    /* Plaintexts: pt[2*n] = g0||nonce[n], pt[2*n+1] = g1||nonce[n] for n in [0, 8). */
    for (n = 0; n < 8; n++) {
        memcpy(pt[2 * n],     mat->g0, MQOM3_PARAM_GRIND_PTXT_BYTES);
        memcpy(pt[2 * n] + MQOM3_PARAM_GRIND_PTXT_BYTES, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
        memcpy(pt[2 * n + 1], mat->g1, MQOM3_PARAM_GRIND_PTXT_BYTES);
        memcpy(pt[2 * n + 1] + MQOM3_PARAM_GRIND_PTXT_BYTES, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
    }

    /* 16 interleaved encryptions with keys [k0, k1]x8, as two x8 batches. */
    ret = enc_encrypt_x8_pub(&mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              &mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              pt[0], pt[1], pt[2], pt[3], pt[4], pt[5], pt[6], pt[7],
                              ct[0], ct[1], ct[2], ct[3], ct[4], ct[5], ct[6], ct[7]);
    ERR(ret, err);
    ret = enc_encrypt_x8_pub(&mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              &mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              pt[8], pt[9], pt[10], pt[11], pt[12], pt[13], pt[14], pt[15],
                              ct[8], ct[9], ct[10], ct[11], ct[12], ct[13], ct[14], ct[15]);
    ERR(ret, err);

    any_pass = 0;
    for (n = 0; n < 8; n++) {
        xor_trunc[n] = 0;
        for (bi = 0; bi < MQOM3_PARAM_GRIND_TRUNC_BYTES; bi++) {
            xor_trunc[n] |= (uint32_t)(ct[2 * n][bi] ^ ct[2 * n + 1][bi]) << (8 * bi);
        }
        /* W == 0: GRIND_TRUNC_BYTES is already 0, so the loop above never ran and
         * xor_trunc[n] is still 0 - the mask below would be a no-op anyway, but its
         * (W - 1) shift count is undefined behavior for W == 0, so skip it. */
#if (MQOM3_PARAM_W > 0)
        xor_trunc[n] &= ((uint32_t)1u << (MQOM3_PARAM_W - 1)) - 1u;
#endif
        if (xor_trunc[n] == 0) { any_pass = 1; }
        nonce_ptr[n] = nonces[n];
        ct0_ptr[n] = ct[2 * n];
        ct1_ptr[n] = ct[2 * n + 1];
    }
    if (!any_pass) { return -1; }

    /* At least one of the 8 nonces passed the AES check: batch-derive i* for
     * all 8 via XOF_6(sig_id, nonces[n], c0, c1) in one x8 call. */
    ret = xof_init_x8(&xof_ctx_x8);
    ERR(ret, err);
    ret = xof_update_x8(&xof_ctx_x8, tag_ptr, 1);
    ERR(ret, err);
    ret = xof_update_x8(&xof_ctx_x8, sigid_ptr, MQOM3_PARAM_DIGEST_SIZE);
    ERR(ret, err);
    ret = xof_update_x8(&xof_ctx_x8, nonce_ptr, MQOM3_PARAM_GRIND_NONCE_BYTES);
    ERR(ret, err);
    ret = xof_update_x8(&xof_ctx_x8, ct0_ptr, MQOM3_PARAM_SEED_SIZE);
    ERR(ret, err);
    ret = xof_update_x8(&xof_ctx_x8, ct1_ptr, MQOM3_PARAM_SEED_SIZE);
    ERR(ret, err);
    ret = xof_squeeze_x8(&xof_ctx_x8, tmp_ptr, MQOM3_PARAM_TAU * 2);
    ERR(ret, err);
    xof_clean_ctx_x8(&xof_ctx_x8);

    for (n = 0; n < 8; n++) {
        if (xor_trunc[n] != 0) { continue; }
        for (e = 0; e < MQOM3_PARAM_TAU; e++) {
            i_star[e] = ((uint16_t)tmp[n][2 * e] + (uint16_t)tmp[n][2 * e + 1] * 256)
                      & (uint16_t)((1 << MQOM3_PARAM_NB_EVALS_LOG) - 1);
        }
        if (BLC_IsValidChallenge(i_star)) { return n; }
    }
    return -1;

err:
    xof_clean_ctx_x8(&xof_ctx_x8);
    /* -2: a XOF failure is a hard error, not "no valid nonce in this batch".
     * Returning ret (always -1 here) made the caller grind on. */
    (void)ret;
    return -2;
}

/* SampleChallenge (OT, XOF x8-batched path): process 8 nonces per call. */
static inline int SampleChallenge(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                  uint16_t i_star[MQOM3_PARAM_TAU],
                                  uint8_t nonce[4])
{
    uint32_t nonce_int[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    uint8_t nonces[8][4];
    unsigned int i;
    int k;
    grinding_material_t mat = { 0 };
    int mat_precomputed = 0;

#ifdef SUPERCOP
    /* The whole grinding computation is public */
    crypto_declassify(sig_id, MQOM3_PARAM_DIGEST_SIZE);
#endif
    do {
        for (i = 0; i < 8; i++) {
            nonces[i][0] = (uint8_t)(nonce_int[i] & 0xffU);
            nonces[i][1] = (uint8_t)((nonce_int[i] >>  8) & 0xffU);
            nonces[i][2] = (uint8_t)((nonce_int[i] >> 16) & 0xffU);
            nonces[i][3] = (uint8_t)((nonce_int[i] >> 24) & 0xffU);
        }
        k = DeriveChallenge_x8(sig_id, (const uint8_t (*)[4])nonces, i_star, &mat, &mat_precomputed);
        /* Only -1 means "try the next batch"; -2 is a hard error and retrying
         * it would reproduce the same failure for the whole nonce space. */
        if (k == -1) {
            for (i = 0; i < 8; i++) { nonce_int[i] += 8; }
        }
    /* nonce_int[0] wraps to 0 only after all 2^32 nonces are exhausted. */
    } while ((k == -1) && (nonce_int[0] != 0));

    grinding_material_clean(&mat);
    if (k < 0) { return -1; }
    memcpy(nonce, nonces[k], 4);
    return 0;
}

#elif (MQOM3_PARAM_OT_VARIANT == 1) && defined(USE_XOF_X4)

/* DeriveChallenge_x4: OT, XOF x4-batched. 4 nonces per call: 8 interleaved
 * AES encryptions (one enc_encrypt_x8_pub call) for the truncation check,
 * then one xof_x4 call to derive i* for all 4 whenever at least one passes. */
static inline int DeriveChallenge_x4(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                     const uint8_t nonces[4][4],
                                     uint16_t i_star[MQOM3_PARAM_TAU],
                                     grinding_material_t *mat,
                                     int *mat_precomputed)
{
    int ret;
    uint8_t pt[8][MQOM3_PARAM_SEED_SIZE];
    uint8_t ct[8][MQOM3_PARAM_SEED_SIZE];
    uint32_t xor_trunc[4];
    int bi, n, e, any_pass;
    xof_context_x4 xof_ctx_x4 = { 0 };
    uint8_t tmp[4][MQOM3_PARAM_TAU * 2];
    uint8_t *tmp_ptr[4] = { tmp[0], tmp[1], tmp[2], tmp[3] };
    const uint8_t *tag_ptr[4] = { (const uint8_t*) DOMAIN_SEP_CHALLENGE, (const uint8_t*) DOMAIN_SEP_CHALLENGE,
                                   (const uint8_t*) DOMAIN_SEP_CHALLENGE, (const uint8_t*) DOMAIN_SEP_CHALLENGE };
    const uint8_t *sigid_ptr[4] = { sig_id, sig_id, sig_id, sig_id };
    const uint8_t *nonce_ptr[4];
    const uint8_t *ct0_ptr[4];
    const uint8_t *ct1_ptr[4];

    if (!(*mat_precomputed)) {
        if (grinding_aes_derive(sig_id, mat) != 0) { return -2; }
        *mat_precomputed = 1;
    }

    /* Plaintexts: pt[2*n] = g0||nonce[n], pt[2*n+1] = g1||nonce[n] for n in [0, 4). */
    for (n = 0; n < 4; n++) {
        memcpy(pt[2 * n],     mat->g0, MQOM3_PARAM_GRIND_PTXT_BYTES);
        memcpy(pt[2 * n] + MQOM3_PARAM_GRIND_PTXT_BYTES, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
        memcpy(pt[2 * n + 1], mat->g1, MQOM3_PARAM_GRIND_PTXT_BYTES);
        memcpy(pt[2 * n + 1] + MQOM3_PARAM_GRIND_PTXT_BYTES, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
    }

    /* 8 interleaved encryptions with keys [k0, k1, k0, k1, k0, k1, k0, k1]. */
    ret = enc_encrypt_x8_pub(&mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              &mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              pt[0], pt[1], pt[2], pt[3], pt[4], pt[5], pt[6], pt[7],
                              ct[0], ct[1], ct[2], ct[3], ct[4], ct[5], ct[6], ct[7]);
    ERR(ret, err);

    any_pass = 0;
    for (n = 0; n < 4; n++) {
        xor_trunc[n] = 0;
        for (bi = 0; bi < MQOM3_PARAM_GRIND_TRUNC_BYTES; bi++) {
            xor_trunc[n] |= (uint32_t)(ct[2 * n][bi] ^ ct[2 * n + 1][bi]) << (8 * bi);
        }
        /* W == 0: GRIND_TRUNC_BYTES is already 0, so the loop above never ran and
         * xor_trunc[n] is still 0 - the mask below would be a no-op anyway, but its
         * (W - 1) shift count is undefined behavior for W == 0, so skip it. */
#if (MQOM3_PARAM_W > 0)
        xor_trunc[n] &= ((uint32_t)1u << (MQOM3_PARAM_W - 1)) - 1u;
#endif
        if (xor_trunc[n] == 0) { any_pass = 1; }
        nonce_ptr[n] = nonces[n];
        ct0_ptr[n] = ct[2 * n];
        ct1_ptr[n] = ct[2 * n + 1];
    }
    if (!any_pass) { return -1; }

    /* At least one of the 4 nonces passed the AES check: batch-derive i* for
     * all 4 via XOF_6(sig_id, nonces[n], c0, c1) in one x4 call. */
    ret = xof_init_x4(&xof_ctx_x4);
    ERR(ret, err);
    ret = xof_update_x4(&xof_ctx_x4, tag_ptr, 1);
    ERR(ret, err);
    ret = xof_update_x4(&xof_ctx_x4, sigid_ptr, MQOM3_PARAM_DIGEST_SIZE);
    ERR(ret, err);
    ret = xof_update_x4(&xof_ctx_x4, nonce_ptr, MQOM3_PARAM_GRIND_NONCE_BYTES);
    ERR(ret, err);
    ret = xof_update_x4(&xof_ctx_x4, ct0_ptr, MQOM3_PARAM_SEED_SIZE);
    ERR(ret, err);
    ret = xof_update_x4(&xof_ctx_x4, ct1_ptr, MQOM3_PARAM_SEED_SIZE);
    ERR(ret, err);
    ret = xof_squeeze_x4(&xof_ctx_x4, tmp_ptr, MQOM3_PARAM_TAU * 2);
    ERR(ret, err);
    xof_clean_ctx_x4(&xof_ctx_x4);

    for (n = 0; n < 4; n++) {
        if (xor_trunc[n] != 0) { continue; }
        for (e = 0; e < MQOM3_PARAM_TAU; e++) {
            i_star[e] = ((uint16_t)tmp[n][2 * e] + (uint16_t)tmp[n][2 * e + 1] * 256)
                      & (uint16_t)((1 << MQOM3_PARAM_NB_EVALS_LOG) - 1);
        }
        if (BLC_IsValidChallenge(i_star)) { return n; }
    }
    return -1;

err:
    xof_clean_ctx_x4(&xof_ctx_x4);
    /* -2: a XOF failure is a hard error, not "no valid nonce in this batch".
     * Returning ret (always -1 here) made the caller grind on. */
    (void)ret;
    return -2;
}

/* SampleChallenge (OT, XOF x4-batched path): process 4 nonces per call. */
static inline int SampleChallenge(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                  uint16_t i_star[MQOM3_PARAM_TAU],
                                  uint8_t nonce[4])
{
    uint32_t nonce_int[4] = { 0, 1, 2, 3 };
    uint8_t nonces[4][4];
    unsigned int i;
    int k;
    grinding_material_t mat = { 0 };
    int mat_precomputed = 0;

#ifdef SUPERCOP
    /* The whole grinding computation is public */
    crypto_declassify(sig_id, MQOM3_PARAM_DIGEST_SIZE);
#endif
    do {
        for (i = 0; i < 4; i++) {
            nonces[i][0] = (uint8_t)(nonce_int[i] & 0xffU);
            nonces[i][1] = (uint8_t)((nonce_int[i] >>  8) & 0xffU);
            nonces[i][2] = (uint8_t)((nonce_int[i] >> 16) & 0xffU);
            nonces[i][3] = (uint8_t)((nonce_int[i] >> 24) & 0xffU);
        }
        k = DeriveChallenge_x4(sig_id, (const uint8_t (*)[4])nonces, i_star, &mat, &mat_precomputed);
        /* Only -1 means "try the next batch"; -2 is a hard error and retrying
         * it would reproduce the same failure for the whole nonce space. */
        if (k == -1) {
            for (i = 0; i < 4; i++) { nonce_int[i] += 4; }
        }
    /* nonce_int[0] wraps to 0 only after all 2^32 nonces are exhausted. */
    } while ((k == -1) && (nonce_int[0] != 0));

    grinding_material_clean(&mat);
    if (k < 0) { return -1; }
    memcpy(nonce, nonces[k], 4);
    return 0;
}

#else /* CT, or OT without XOF x4/x8 available */

/* DeriveChallenge_x2: process 2 nonces with 4 interleaved AES encryptions.
 * Calls enc_encrypt_x4_pub with contexts [k0, k1, k0, k1], hiding round latency
 * across 4 independent chains (DO_ENC_BLOCK_128_128_x4 / DO_ENC_BLOCK_256_256_x4).
 * Returns 0/1 (index of first valid nonce, i_star filled), -1 if none valid, <-1 on error. */
static inline int DeriveChallenge_x2(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                     const uint8_t nonces[2][4],
                                     uint16_t i_star[MQOM3_PARAM_TAU],
                                     grinding_material_t *mat,
                                     int *mat_precomputed)
{
    int ret;
    uint8_t pt[4][MQOM3_PARAM_SEED_SIZE];
    uint8_t ct[4][MQOM3_PARAM_SEED_SIZE];
    uint32_t xor_trunc;
    int bi, n, e;
    xof_context xof_ctx = { 0 };
    uint8_t tmp[MQOM3_PARAM_TAU * 2];

    if (!(*mat_precomputed)) {
        if (grinding_aes_derive(sig_id, mat) != 0) { return -2; }
        *mat_precomputed = 1;
    }

    /* Plaintexts: pt[2*n] = g0||nonce[n], pt[2*n+1] = g1||nonce[n] for n in {0, 1}. */
    for (n = 0; n < 2; n++) {
        memcpy(pt[2 * n],     mat->g0, MQOM3_PARAM_GRIND_PTXT_BYTES);
        memcpy(pt[2 * n] + MQOM3_PARAM_GRIND_PTXT_BYTES, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
        memcpy(pt[2 * n + 1], mat->g1, MQOM3_PARAM_GRIND_PTXT_BYTES);
        memcpy(pt[2 * n + 1] + MQOM3_PARAM_GRIND_PTXT_BYTES, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
    }

    /* 4 interleaved encryptions with keys [k0, k1, k0, k1]. */
    ret = enc_encrypt_x4_pub(&mat->k0_ctx, &mat->k1_ctx, &mat->k0_ctx, &mat->k1_ctx,
                              pt[0], pt[1], pt[2], pt[3],
                              ct[0], ct[1], ct[2], ct[3]);
    ERR(ret, err);

    /* Check each nonce; return the index of the first valid one. */
    for (n = 0; n < 2; n++) {
        xor_trunc = 0;
        for (bi = 0; bi < MQOM3_PARAM_GRIND_TRUNC_BYTES; bi++) {
            xor_trunc |= (uint32_t)(ct[2 * n][bi] ^ ct[2 * n + 1][bi]) << (8 * bi);
        }
        /* W == 0: GRIND_TRUNC_BYTES is already 0, so the loop above never ran and
         * xor_trunc is still 0 - the mask below would be a no-op anyway, but its
         * (W - 1) shift count is undefined behavior for W == 0, so skip it. */
#if (MQOM3_PARAM_W > 0)
        xor_trunc &= ((uint32_t)1u << (MQOM3_PARAM_W - 1)) - 1u;
#endif
        if (xor_trunc != 0) { continue; }

        /* XOR check passed: derive i* via XOF_6(sig_id, nonces[n], c0, c1). */
        ret = xof_init(&xof_ctx);
        ERR(ret, err);
        ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_CHALLENGE, 1);
        ERR(ret, err);
        ret = xof_update(&xof_ctx, sig_id, MQOM3_PARAM_DIGEST_SIZE);
        ERR(ret, err);
        ret = xof_update(&xof_ctx, nonces[n], MQOM3_PARAM_GRIND_NONCE_BYTES);
        ERR(ret, err);
        ret = xof_update(&xof_ctx, ct[2 * n], MQOM3_PARAM_SEED_SIZE);
        ERR(ret, err);
        ret = xof_update(&xof_ctx, ct[2 * n + 1], MQOM3_PARAM_SEED_SIZE);
        ERR(ret, err);
        ret = xof_squeeze(&xof_ctx, tmp, MQOM3_PARAM_TAU * 2);
        ERR(ret, err);
        for (e = 0; e < MQOM3_PARAM_TAU; e++) {
            i_star[e] = ((uint16_t)tmp[2 * e] + (uint16_t)tmp[2 * e + 1] * 256)
                      & (uint16_t)((1 << MQOM3_PARAM_NB_EVALS_LOG) - 1);
        }
        xof_clean_ctx(&xof_ctx);
        if (BLC_IsValidChallenge(i_star)) { return n; }
    }
    return -1;

err:
    xof_clean_ctx(&xof_ctx);
    /* -2: a XOF failure is a hard error, not "no valid nonce in this batch".
     * Returning ret (always -1 here) made the caller grind on. */
    (void)ret;
    return -2;
}

/* SampleChallenge (AES x4 path): process 2 nonces per call. */
static inline int SampleChallenge(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                  uint16_t i_star[MQOM3_PARAM_TAU],
                                  uint8_t nonce[4])
{
    uint32_t nonce_int[2] = { 0, 1 };
    uint8_t nonces[2][4];
    unsigned int i;
    int k;
    grinding_material_t mat = { 0 };
    int mat_precomputed = 0;

#ifdef SUPERCOP
    /* The whole grinding computation is public */
    crypto_declassify(sig_id, MQOM3_PARAM_DIGEST_SIZE);
#endif
    do {
        for (i = 0; i < 2; i++) {
            nonces[i][0] = (uint8_t)(nonce_int[i] & 0xffU);
            nonces[i][1] = (uint8_t)((nonce_int[i] >>  8) & 0xffU);
            nonces[i][2] = (uint8_t)((nonce_int[i] >> 16) & 0xffU);
            nonces[i][3] = (uint8_t)((nonce_int[i] >> 24) & 0xffU);
        }
        k = DeriveChallenge_x2(sig_id, (const uint8_t (*)[4])nonces, i_star, &mat, &mat_precomputed);
        /* Only -1 means "try the next batch"; -2 is a hard error and retrying
         * it would reproduce the same failure for the whole nonce space. */
        if (k == -1) {
            nonce_int[0] += 2;
            nonce_int[1] += 2;
        }
    /* nonce_int[0] wraps to 0 only after all 2^32 nonces are exhausted. */
    } while ((k == -1) && (nonce_int[0] != 0));

    grinding_material_clean(&mat);
    if (k < 0) { return -1; }
    memcpy(nonce, nonces[k], 4);
    return 0;
}

#endif /* MQOM3_PARAM_OT_VARIANT == 1 && (USE_XOF_X8 || USE_XOF_X4) */

#else /* AES_GRINDING_X4 not set: scalar path */
/* SampleChallenge (AES scalar path): one nonce per call. */
static inline int SampleChallenge(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                                  uint16_t i_star[MQOM3_PARAM_TAU],
                                  uint8_t nonce[4])
{
    int ret = -1;
    grinding_material_t mat = { 0 };
    int mat_precomputed = 0;
    uint32_t nonce_int = 0;
    int v;

#ifdef SUPERCOP
    /* The whole grinding computation is public */
    crypto_declassify(sig_id, MQOM3_PARAM_DIGEST_SIZE);
#endif
    do {
        nonce[0] = (uint8_t)(nonce_int & 0xffU);
        nonce[1] = (uint8_t)((nonce_int >>  8) & 0xffU);
        nonce[2] = (uint8_t)((nonce_int >> 16) & 0xffU);
        nonce[3] = (uint8_t)((nonce_int >> 24) & 0xffU);
        v = DeriveChallenge(sig_id, nonce, i_star, &mat, &mat_precomputed);
        if (v == 0) { ret = 0; break; }
        /* v < -1: hard error from DeriveChallenge (grinding material), not a
         * rejected nonce - stop instead of grinding the whole nonce space. */
        if (v < -1) { break; }
    } while (++nonce_int != 0);
    grinding_material_clean(&mat);
    return ret;
}
#endif /* AES_GRINDING_X4 */

#endif /* __SAMPLE_CHALLENGE_SIGN_H__ */
