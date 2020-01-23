/* Melas forward error correction, reference code (as implemented in the paper) */
#include "melas_fec.h"

/* Return s/2^n mod R */
static fec_gf_t step(size_t n, fec_gf_t R, fec_gf_t s) {
    for (; n; n--) {
        s = (s ^ ((s & 1) * R)) >> 1;
    }
    return s;
}

/* Compute syndrome(data), where data has length len */
#define syndrome18(data,len) s18update(0,data,len)
static fec_gf_t s18update(fec_gf_t r, const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        r = step(8, 0x46231, r ^ data[i]);
    }
    return r;
}

/* Append 3 bytes of FEC(data) to data, so that the FEC becomes 0 */
void PQCLEAN_PAPABEAR_CLEAN_melas_fec_set(
    uint8_t out[MELAS_FEC_BYTES],
    const uint8_t *data,
    size_t len
) {
    fec_gf_t fec = syndrome18(data, len);
    for (size_t i = 0; i < MELAS_FEC_BYTES; i++, fec >>= 8) {
        out[i] = (uint8_t)fec;
    }
}

/* Return a*b mod Q */
static fec_gf_t mul(fec_gf_t a, fec_gf_t b) {
    fec_gf_t r = 0;
    for (size_t i = 0; i < 9; i++) {
        r ^= ((b >> (8 - i)) & 1) * a;
        a = step(1, Q, a);
    }
    return r;
}

/* Reverse an 18-bit number x */
static fec_gf_t reverse18(fec_gf_t x) {
    fec_gf_t ret = 0;
    for (size_t i = 0; i < 18; i++) {
        ret ^= ((x >> i) & 1) << (17 - i);
    }
    return ret;
}

/* Correct data to have the given FEC */
void PQCLEAN_PAPABEAR_CLEAN_melas_fec_correct (
    uint8_t *data,
    size_t len,
    const uint8_t fec[MELAS_FEC_BYTES]
) {

    fec_gf_t a = s18update(syndrome18(data, len), fec, MELAS_FEC_BYTES);
    fec_gf_t c, r, htr;
    size_t i;
    const uint8_t table[9] = {36, 10, 43, 215, 52, 11, 116, 244, 0};
    fec_gf_t e0, e1;

    /* Form a quadratic equation from the syndrome */
    c = mul(step(9, Q, a), step(9, Q, reverse18(a)));
    for (i = 0, r = 0x100; i < 510; i++) {
        r = mul(r, c);
    }
    r = step(17, Q, r);
    a = step(511 - (len + MELAS_FEC_BYTES) * 8, Q, a);

    /* Solve using the half trace */
    for (i = 0, htr = 0; i < 9; i++) {
        htr ^= ((r >> i) & 1) * table[i];
    }
    e0 = mul(a, htr);
    e1 = e0 ^ a;

    /* Correct the errors using the locators */
    for (i = 0; i < len; i++) {
        data[i] ^= (uint8_t)(e0 & (((e0 & (e0 - 1)) - 1) >> 9));
        data[i] ^= (uint8_t)(e1 & (((e1 & (e1 - 1)) - 1) >> 9));
        e0 = step(8, Q, e0);
        e1 = step(8, Q, e1);
    }
}
