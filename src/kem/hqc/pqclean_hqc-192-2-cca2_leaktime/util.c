#include "util.h"
#include "stddef.h"

#include "assert.h"

/* These functions should help with endianness-safe conversions
 *
 * load8 and store8 are copied from the McEliece implementations,
 * which are in the public domain.
 */


void PQCLEAN_HQC1922CCA2_LEAKTIME_store8(unsigned char *out, uint64_t in) {
    out[0] = (in >> 0x00) & 0xFF;
    out[1] = (in >> 0x08) & 0xFF;
    out[2] = (in >> 0x10) & 0xFF;
    out[3] = (in >> 0x18) & 0xFF;
    out[4] = (in >> 0x20) & 0xFF;
    out[5] = (in >> 0x28) & 0xFF;
    out[6] = (in >> 0x30) & 0xFF;
    out[7] = (in >> 0x38) & 0xFF;
}


uint64_t PQCLEAN_HQC1922CCA2_LEAKTIME_load8(const unsigned char *in) {
    uint64_t ret = in[7];

    for (int8_t i = 6; i >= 0; i--) {
        ret <<= 8;
        ret |= in[i];
    }

    return ret;
}

void PQCLEAN_HQC1922CCA2_LEAKTIME_load8_arr(uint64_t *out64, size_t outlen, const uint8_t *in8, size_t inlen) {
    size_t index_in = 0;
    size_t index_out = 0;

    // first copy by 8 bytes
    if (inlen >= 8 && outlen >= 1) {
        while (index_out < outlen && index_in + 8 <= inlen) {
            out64[index_out] = PQCLEAN_HQC1922CCA2_LEAKTIME_load8(in8 + index_in);

            index_in += 8;
            index_out += 1;
        }
    }

    // we now need to do the last 7 bytes if necessary
    if (index_in >= inlen || index_out >= outlen) {
        return;
    }
    out64[index_out] = in8[inlen - 1];
    for (int8_t i = (int8_t)(inlen - index_in) - 2; i >= 0; i--) {
        out64[index_out] <<= 8;
        out64[index_out] |= in8[index_in + i];
    }
}

void PQCLEAN_HQC1922CCA2_LEAKTIME_store8_arr(uint8_t *out8, size_t outlen, const uint64_t *in64, size_t inlen) {
    for (size_t index_out = 0, index_in = 0; index_out < outlen && index_in < inlen;) {
        out8[index_out] = (in64[index_in] >> ((index_out % 8) * 8)) & 0xFF;
        index_out++;
        if (index_out % 8 == 0) {
            index_in++;
        }
    }
}
