/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: additional functions for FrodoKEM
*********************************************************************************************/

#include <stdint.h>
#include <string.h>

#include "api.h"
#include "common.h"
#include "params.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

uint16_t PQCLEAN_FRODOKEM976AES_CLEAN_LE_TO_UINT16(uint16_t n) {
    return (((uint8_t *) &n)[0] | (((uint8_t *) &n)[1] << 8));
}

uint16_t PQCLEAN_FRODOKEM976AES_CLEAN_UINT16_TO_LE(uint16_t n) {
    uint16_t y;
    uint8_t *z = (uint8_t *) &y;
    z[0] = n & 0xFF;
    z[1] = (n & 0xFF00) >> 8;
    return y;
}

void PQCLEAN_FRODOKEM976AES_CLEAN_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s) {
    // Multiply by s on the right
    // Inputs: b (N_BAR x N), s (N x N_BAR)
    // Output: out = b*s (N_BAR x N_BAR)
    int i, j, k;

    for (i = 0; i < PARAMS_NBAR; i++) {
        for (j = 0; j < PARAMS_NBAR; j++) {
            out[i * PARAMS_NBAR + j] = 0;
            for (k = 0; k < PARAMS_N; k++) {
                out[i * PARAMS_NBAR + j] += b[i * PARAMS_N + k] * s[j * PARAMS_N + k];
            }
            out[i * PARAMS_NBAR + j] = (uint32_t)(out[i * PARAMS_NBAR + j]) & ((1 << PARAMS_LOGQ) - 1);
        }
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e) {
    // Multiply by s on the left
    // Inputs: b (N x N_BAR), s (N_BAR x N), e (N_BAR x N_BAR)
    // Output: out = s*b + e (N_BAR x N_BAR)
    int i, j, k;

    for (k = 0; k < PARAMS_NBAR; k++) {
        for (i = 0; i < PARAMS_NBAR; i++) {
            out[k * PARAMS_NBAR + i] = e[k * PARAMS_NBAR + i];
            for (j = 0; j < PARAMS_N; j++) {
                out[k * PARAMS_NBAR + i] += s[k * PARAMS_N + j] * b[j * PARAMS_NBAR + i];
            }
            out[k * PARAMS_NBAR + i] = (uint32_t)(out[k * PARAMS_NBAR + i]) & ((1 << PARAMS_LOGQ) - 1);
        }
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_add(uint16_t *out, const uint16_t *a, const uint16_t *b) {
    // Add a and b
    // Inputs: a, b (N_BAR x N_BAR)
    // Output: c = a + b

    for (size_t i = 0; i < (PARAMS_NBAR * PARAMS_NBAR); i++) {
        out[i] = (a[i] + b[i]) & ((1 << PARAMS_LOGQ) - 1);
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_sub(uint16_t *out, const uint16_t *a, const uint16_t *b) {
    // Subtract a and b
    // Inputs: a, b (N_BAR x N_BAR)
    // Output: c = a - b

    for (size_t i = 0; i < (PARAMS_NBAR * PARAMS_NBAR); i++) {
        out[i] = (a[i] - b[i]) & ((1 << PARAMS_LOGQ) - 1);
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_key_encode(uint16_t *out, const uint16_t *in) {
    // Encoding
    unsigned int i, j, npieces_word = 8;
    unsigned int nwords = (PARAMS_NBAR * PARAMS_NBAR) / 8;
    uint64_t temp, mask = ((uint64_t)1 << PARAMS_EXTRACTED_BITS) - 1;
    uint16_t *pos = out;

    for (i = 0; i < nwords; i++) {
        temp = 0;
        for (j = 0; j < PARAMS_EXTRACTED_BITS; j++) {
            temp |= ((uint64_t)((uint8_t *)in)[i * PARAMS_EXTRACTED_BITS + j]) << (8 * j);
        }
        for (j = 0; j < npieces_word; j++) {
            *pos = (uint16_t)((temp & mask) << (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS));
            temp >>= PARAMS_EXTRACTED_BITS;
            pos++;
        }
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_key_decode(uint16_t *out, const uint16_t *in) {
    // Decoding
    unsigned int i, j, index = 0, npieces_word = 8;
    unsigned int nwords = (PARAMS_NBAR * PARAMS_NBAR) / 8;
    uint16_t temp, maskex = ((uint16_t)1 << PARAMS_EXTRACTED_BITS) - 1, maskq = ((uint16_t)1 << PARAMS_LOGQ) - 1;
    uint8_t  *pos = (uint8_t *)out;
    uint64_t templong;

    for (i = 0; i < nwords; i++) {
        templong = 0;
        for (j = 0; j < npieces_word; j++) {  // temp = floor(in*2^{-11}+0.5)
            temp = ((in[index] & maskq) + (1 << (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS - 1))) >> (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS);
            templong |= ((uint64_t)(temp & maskex)) << (PARAMS_EXTRACTED_BITS * j);
            index++;
        }
        for (j = 0; j < PARAMS_EXTRACTED_BITS; j++) {
            pos[i * PARAMS_EXTRACTED_BITS + j] = (templong >> (8 * j)) & 0xFF;
        }
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_pack(uint8_t *out, size_t outlen, const uint16_t *in, size_t inlen, uint8_t lsb) {
    // Pack the input uint16 vector into a char output vector, copying lsb bits from each input element.
    // If inlen * lsb / 8 > outlen, only outlen * 8 bits are copied.
    memset(out, 0, outlen);

    size_t i = 0;            // whole bytes already filled in
    size_t j = 0;            // whole uint16_t already copied
    uint16_t w = 0;          // the leftover, not yet copied
    uint8_t bits = 0;        // the number of lsb in w

    while (i < outlen && (j < inlen || ((j == inlen) && (bits > 0)))) {
        /*
        in: |        |        |********|********|
                              ^
                              j
        w : |   ****|
                ^
               bits
        out:|**|**|**|**|**|**|**|**|* |
                                    ^^
                                    ib
        */
        uint8_t b = 0;  // bits in out[i] already filled in
        while (b < 8) {
            int nbits = min(8 - b, bits);
            uint16_t mask = (1 << nbits) - 1;
            uint8_t t = (uint8_t) ((w >> (bits - nbits)) & mask);  // the bits to copy from w to out
            out[i] = out[i] + (t << (8 - b - nbits));
            b += (uint8_t) nbits;
            bits -= (uint8_t) nbits;
            w &= ~(mask << bits);  // not strictly necessary; mostly for debugging

            if (bits == 0) {
                if (j < inlen) {
                    w = in[j];
                    bits = lsb;
                    j++;
                } else {
                    break;  // the input vector is exhausted
                }
            }
        }
        if (b == 8) {  // out[i] is filled in
            i++;
        }
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_unpack(uint16_t *out, size_t outlen, const uint8_t *in, size_t inlen, uint8_t lsb) {
    // Unpack the input char vector into a uint16_t output vector, copying lsb bits
    // for each output element from input. outlen must be at least ceil(inlen * 8 / lsb).
    memset(out, 0, outlen * sizeof(uint16_t));

    size_t i = 0;            // whole uint16_t already filled in
    size_t j = 0;            // whole bytes already copied
    uint8_t w = 0;           // the leftover, not yet copied
    uint8_t bits = 0;        // the number of lsb bits of w

    while (i < outlen && (j < inlen || ((j == inlen) && (bits > 0)))) {
        /*
        in: |  |  |  |  |  |  |**|**|...
                              ^
                              j
        w : | *|
              ^
              bits
        out:|   *****|   *****|   ***  |        |...
                              ^   ^
                              i   b
        */
        uint8_t b = 0;  // bits in out[i] already filled in
        while (b < lsb) {
            int nbits = min(lsb - b, bits);
            uint16_t mask = (1 << nbits) - 1;
            uint8_t t = (w >> (bits - nbits)) & mask;  // the bits to copy from w to out
            out[i] = out[i] + (t << (lsb - b - nbits));
            b += (uint8_t) nbits;
            bits -= (uint8_t) nbits;
            w &= ~(mask << bits);  // not strictly necessary; mostly for debugging

            if (bits == 0) {
                if (j < inlen) {
                    w = in[j];
                    bits = 8;
                    j++;
                } else {
                    break;  // the input vector is exhausted
                }
            }
        }
        if (b == lsb) {  // out[i] is filled in
            i++;
        }
    }
}


void PQCLEAN_FRODOKEM976AES_CLEAN_clear_bytes(uint8_t *mem, size_t n) {
    // Clear 8-bit bytes from memory. "n" indicates the number of bytes to be zeroed.
    // This function uses the volatile type qualifier to inform the compiler not to optimize out the memory clearing.
    volatile uint8_t *v = mem;

    for (size_t i = 0; i < n; i++) {
        v[i] = 0;
    }
}
