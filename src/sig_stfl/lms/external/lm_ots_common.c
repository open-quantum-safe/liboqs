/*
 * This is the code that implements the one-time-signature part of the LMS hash
 * based signatures
 */
#include "lm_ots_common.h"
#include "common_defs.h"
#include "hash.h"

/*
 * Convert the external name of a parameter set into the set of values we care
 * about
 */
bool lm_ots_look_up_parameter_set(param_set_t parameter_set,
     unsigned *h, unsigned *n, unsigned *w, unsigned *p, unsigned *ls) {
    unsigned v_h, v_n, v_w, v_p, v_ls;
    switch (parameter_set) {
    case LMOTS_SHA256_N32_W1:
        v_h = HASH_SHA256; v_n = 32; v_w = 1; v_p = 265; v_ls = 7; break;
    case LMOTS_SHA256_N32_W2:
        v_h = HASH_SHA256; v_n = 32; v_w = 2; v_p = 133; v_ls = 6; break;
    case LMOTS_SHA256_N32_W4:
        v_h = HASH_SHA256; v_n = 32; v_w = 4; v_p = 67; v_ls = 4; break;
    case LMOTS_SHA256_N32_W8:
        v_h = HASH_SHA256; v_n = 32; v_w = 8; v_p = 34; v_ls = 0; break;
    default: return false;
    }

    if (h) *h = v_h;
    if (n) *n = v_n;
    if (w) *w = v_w;
    if (p) *p = v_p;
    if (ls) *ls = v_ls;

    return true;
}

/* The public key just consists of the bare hash */
size_t lm_ots_get_public_key_len(param_set_t lm_ots_type) {
    unsigned n;
    if (!lm_ots_look_up_parameter_set( lm_ots_type, 0, &n, 0, 0, 0 ))
        return 0;

    return n;
}

/* Return the length of a signature */
size_t lm_ots_get_signature_len(param_set_t lm_ots_type) {
    unsigned n, p;

    if (!lm_ots_look_up_parameter_set( lm_ots_type, 0, &n, 0, &p, 0 ))
        return 0;

    return 4 + n + p*n;
}

/* Return the number of hashes we need to compute to generate a public key */
unsigned lm_ots_hashes_per_public_key(param_set_t lm_ots_type) {
    unsigned wint, num_dig;
    if (!lm_ots_look_up_parameter_set(lm_ots_type,
                               NULL, NULL, &wint, &num_dig, NULL)) {
        return 0;
    }

    /* Total number of hash invocations:
     * For each digit, we expand the seed (1), and then perform (2**wint-1)
     *    haashes to obtain the end of the chain
     * Then, we hash all the ends of the chains together
     * If we were to return the number of hash compression operations,
     * the final 1 would be a bit larger
     */
    return num_dig * (1 << wint) + 1;
}

/* Todo: some of these values depend only on w; why do we need to recompute */
/* them each time??? */
unsigned lm_ots_coef(const unsigned char *Q, unsigned i, unsigned w) {
    unsigned index = (i * w) / 8;    /* Which byte holds the coefficient */
                                     /* we want */
    unsigned digits_per_byte = 8/w;
    unsigned shift = w * (~i & (digits_per_byte-1)); /* Where in the byte */
                                     /* the coefficient is */
    unsigned mask = (1<<w) - 1;      /* How to mask off the parts we're not */
                                     /* interested in */

    return (Q[index] >> shift) & mask;
}

/* This returns the Winternitz checksum to append to the hash */
unsigned lm_ots_compute_checksum(const unsigned char *Q, unsigned Q_len,
                                 unsigned w, unsigned ls) {
    unsigned sum = 0;
    unsigned i;
    unsigned u = 8 * Q_len / w;
    unsigned max_digit = (1<<w) - 1;
    for (i=0; i<u; i++) {
        sum += max_digit - lm_ots_coef( Q, i, w );
    }
    return sum << ls;
}
