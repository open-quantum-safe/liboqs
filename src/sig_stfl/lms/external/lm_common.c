// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the tree part of the LMS hash
 * based signatures
 */
#include <string.h>
#include "lm_common.h"
#include "hash.h"
#include "common_defs.h"
#include "lm_ots_common.h"

/*
 * Internal utility to convert encoded parameter sets into what they represent
 */
bool lm_look_up_parameter_set(param_set_t parameter_set,
     unsigned *h, unsigned *n, unsigned *height) {
    unsigned v_h, v_n, v_height;
    switch (parameter_set) {
    case LMS_SHA256_N32_H5:
        v_h = HASH_SHA256; v_n = 32; v_height = 5; break;
    case LMS_SHA256_N32_H10:
        v_h = HASH_SHA256; v_n = 32; v_height = 10; break;
    case LMS_SHA256_N32_H15:
        v_h = HASH_SHA256; v_n = 32; v_height = 15; break;
    case LMS_SHA256_N32_H20:
        v_h = HASH_SHA256; v_n = 32; v_height = 20; break;
    case LMS_SHA256_N32_H25:
        v_h = HASH_SHA256; v_n = 32; v_height = 25; break;
    default: return false;
    }

    if (h) *h = v_h;
    if (n) *n = v_n;
    if (height) *height = v_height;

    return true;
}

/* The LM public key consists of: */
#define LM_PUB_PARM_SET    0       /* The parameter set (4 bytes) */
#define LM_PUB_OTS_PARM_SET 4      /* The OTS parameter set (4 bytes) */
#define LM_PUB_I            8      /* Our nonce (I) value (16 bytes) */
/* The root value comes here */

/*
 * XDR requires us to pad the I value out to a multiple of 4
 * This computes how long the field will be after padding
 * That is, it rounds len_I up to the next multiple of 4
 */
#define padded_length(len_I) (((len_I) + 3) & ~3)

/* The public key just consists of the parameter sets, plus I, plus root hash */
size_t lm_get_public_key_len(param_set_t lm_type) {
    unsigned n;
    if (!lm_look_up_parameter_set( lm_type, 0, &n, 0))
        return 0;

    return LM_PUB_I + padded_length(I_LEN) + n;
}

/*
 * The amount of space we use for signature
 */
size_t lm_get_signature_len(param_set_t lm_type,
                            param_set_t lm_ots_type) {
    unsigned n, height;
    if (!lm_look_up_parameter_set( lm_type, 0, &n, &height ))
        return 0;

    int ots_sig_len = (int)lm_ots_get_signature_len(lm_ots_type);
    if (ots_sig_len == 0)
        return 0;

    /*
     * The LM signature consists of the type code, the diversification factor,
     * the LM-OTS signature (which includes the OTS type code), and the
     * authentication path (which is an array of height hashes)
     */
    return 4 + 4 + ots_sig_len + n*height;
}
