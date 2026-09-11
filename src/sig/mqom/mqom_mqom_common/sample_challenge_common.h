#ifndef __SAMPLE_CHALLENGE_COMMON_H__
#define __SAMPLE_CHALLENGE_COMMON_H__

/* Grinding: AES-based (spec), the only supported algorithm - the earlier
 * Keccak/XOF-based grinding option has been removed. */

#include "mqom3_parameters.h"
#include "common.h"
#include "enc.h"
#include "xof.h"
#include "domain_separation.h"

/* Nonce length in bits; the signature format stores 4 bytes (32 bits).
 * The signing loop uses uint32_t with defined unsigned wrapping to cover all 2^GRIND_L nonces.
 * GRIND_L must be a multiple of 8: a non-multiple would cause the top bits of the last nonce
 * byte to silently overlap with the fixed plaintext part g_j, breaking the spec encoding. */
#define MQOM3_PARAM_GRIND_L           32
#if (MQOM3_PARAM_GRIND_L % 8) != 0
#error "MQOM3_PARAM_GRIND_L must be a multiple of 8 (byte-aligned nonce)"
#endif
#define MQOM3_PARAM_GRIND_NONCE_BYTES ((MQOM3_PARAM_GRIND_L + 7) / 8)

/* Fixed part of each AES plaintext: one full block minus the nonce bytes. */
#define MQOM3_PARAM_GRIND_PTXT_BYTES   (MQOM3_PARAM_SEED_SIZE - MQOM3_PARAM_GRIND_NONCE_BYTES)
/* Total XOF_5 output: g0 | g1 | k0_raw | k1_raw = 4*lambda - 2*ell bits.
 * k0_raw and k1_raw are full lambda-bit AES keys; their 2 top bits (bits 7:6 of the
 * last byte) are NOT from the XOF but are overwritten by the per-cipher selector bits,
 * so each key carries lambda-2 bits of XOF randomness.  This avoids the non-byte-aligned
 * (lambda-ell-2)-bit formulation of the earlier spec draft. */
#define MQOM3_PARAM_GRIND_MATERIAL_BYTES \
    (2 * MQOM3_PARAM_GRIND_PTXT_BYTES + 2 * MQOM3_PARAM_SEED_SIZE)
/* Bytes needed to hold (W-1) truncation bits used in the AES grinding check (spec: Truncate_{w-1}).
 * This is independent from GRIND_L: ell controls nonce size; w-1 controls grinding hardness. */
#define MQOM3_PARAM_GRIND_TRUNC_BYTES  ((MQOM3_PARAM_W - 1 + 7) / 8)

/* W == 0 would make the (W - 1) shift below undefined; the code guards that case
 * explicitly, but no parameter set has any business using it. */
#if MQOM3_PARAM_W < 1
#error "MQOM3_PARAM_W must be at least 1"
#endif
/* Sanity check of W upper bound */
#if (MQOM3_PARAM_W - 1) > 24
#error "MQOM3_PARAM_W - 1 must be <= 24"
#endif

/* Precomputed AES grinding material, derived once per signing / verification from sig_id.
 * g0 and g1 are the fixed halves of the two AES plaintexts; the nonce is appended each
 * iteration.  k0_ctx and k1_ctx hold separate expanded schedules for k0||01 and k1||11,
 * stored as independent contexts to allow instruction-level interleaving (enc_x2_pub / enc_x4_pub). */
typedef struct {
    uint8_t g0[MQOM3_PARAM_GRIND_PTXT_BYTES];
    uint8_t g1[MQOM3_PARAM_GRIND_PTXT_BYTES];
    enc_ctx_pub k0_ctx;
    enc_ctx_pub k1_ctx;
} grinding_material_t;

/* Derive grinding material from sig_id via XOF_5(sig_id):
 *   g0 | g1 | k0_raw | k1_raw   (sizes: ptxt | ptxt | seed | seed)
 * The top 2 bits (7:6) of the last byte of each raw key are overwritten with the
 * per-cipher selector (k0: ...01, k1: ...11) and both key schedules are expanded.
 * Must be called once before the nonce iteration (Sign) or nonce check (Verify).
 * Returns 0 on success. */
static int grinding_aes_derive(const uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE],
                               grinding_material_t *mat)
{
    int ret;
    xof_context xof_ctx = { 0 };
    uint8_t buf[MQOM3_PARAM_GRIND_MATERIAL_BYTES];
    uint8_t k0_key[MQOM3_PARAM_SEED_SIZE];
    uint8_t k1_key[MQOM3_PARAM_SEED_SIZE];
    const uint8_t *ptr;

    ret = xof_init(&xof_ctx);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_GRINDING, 1);
    ERR(ret, err);
    ret = xof_update(&xof_ctx, sig_id, MQOM3_PARAM_DIGEST_SIZE);
    ERR(ret, err);
    ret = xof_squeeze(&xof_ctx, buf, MQOM3_PARAM_GRIND_MATERIAL_BYTES);
    ERR(ret, err);

    /* Parse: g0 | g1 | k0_raw | k1_raw */
    ptr = buf;
    memcpy(mat->g0, ptr, MQOM3_PARAM_GRIND_PTXT_BYTES); ptr += MQOM3_PARAM_GRIND_PTXT_BYTES;
    memcpy(mat->g1, ptr, MQOM3_PARAM_GRIND_PTXT_BYTES); ptr += MQOM3_PARAM_GRIND_PTXT_BYTES;
    memcpy(k0_key,  ptr, MQOM3_PARAM_SEED_SIZE);        ptr += MQOM3_PARAM_SEED_SIZE;
    memcpy(k1_key,  ptr, MQOM3_PARAM_SEED_SIZE);

    /* Inject the 2-bit selector, per spec: key = Truncate_{lambda-2}(k) || 01 for k0
     * and || 11 for k1.
     *
     * Where those two trailing bits land in the last byte is fixed by the bit-string
     * serialization of: a bit-string (s_1, ..., s_lambda) is laid
     * out as B(s_1..s_8) || B(s_9..s_16) || ... with B(b_0, ..., b_7) = sum 2^i * b_i.
     */
    k0_key[MQOM3_PARAM_SEED_SIZE - 1] = (k0_key[MQOM3_PARAM_SEED_SIZE - 1] & 0x3fU) | 0x80U;
    k1_key[MQOM3_PARAM_SEED_SIZE - 1] = (k1_key[MQOM3_PARAM_SEED_SIZE - 1] & 0x3fU) | 0xc0U;

    ret = enc_key_sched_pub(&mat->k0_ctx, k0_key);
    ERR(ret, err);
    ret = enc_key_sched_pub(&mat->k1_ctx, k1_key);

err:
    xof_clean_ctx(&xof_ctx);
    mqom_cleanse(buf, sizeof(buf));
    mqom_cleanse(k0_key, sizeof(k0_key));
    mqom_cleanse(k1_key, sizeof(k1_key));
    return ret;
}

/* Clean grinding material (g0, g1, and key schedules). */
static inline void grinding_material_clean(grinding_material_t *mat) {
    enc_clean_ctx_pub(&mat->k0_ctx);
    enc_clean_ctx_pub(&mat->k1_ctx);
    mqom_cleanse(mat, sizeof(*mat));
}

#endif /* __SAMPLE_CHALLENGE_COMMON_H__ */
