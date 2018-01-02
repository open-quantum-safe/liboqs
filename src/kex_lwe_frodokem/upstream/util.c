/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: additional functions for FrodoKEM
*********************************************************************************************/

#include <string.h>
#include "frodo_macrify.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))


void frodo_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb) 
{ // Pack the input uint16 vector into a char output vector, copying lsb bits from each input element. 
  // If inlen * lsb / 8 > outlen, only outlen * 8 bits are copied.
    memset(out, 0, outlen);

    size_t i = 0;            // whole bytes already filled in
    size_t j = 0;            // whole uint16_t already copied
    uint16_t w = 0;          // the leftover, not yet copied
    unsigned char bits = 0;  // the number of lsb in w

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
        unsigned char b = 0;  // bits in out[i] already filled in
        while (b < 8) {
            int nbits = min(8 - b, bits);
            uint16_t mask = (1 << nbits) - 1;
            unsigned char t = (w >> (bits - nbits)) & mask;  // the bits to copy from w to out
            out[i] = out[i] + (t << (8 - b - nbits));
            b += nbits;
            bits -= nbits;
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


void frodo_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb) 
{ // Unpack the input char vector into a uint16_t output vector, copying lsb bits
  // for each output element from input. outlen must be at least ceil(inlen * 8 / lsb).
    memset(out, 0, outlen * sizeof(uint16_t));

    size_t i = 0;            // whole uint16_t already filled in
    size_t j = 0;            // whole bytes already copied
    unsigned char w = 0;     // the leftover, not yet copied
    unsigned char bits = 0;  // the number of lsb bits of w

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
        unsigned char b = 0;  // bits in out[i] already filled in
        while (b < lsb) {
            int nbits = min(lsb - b, bits);
            uint16_t mask = (1 << nbits) - 1;
            unsigned char t = (w >> (bits - nbits)) & mask;  // the bits to copy from w to out
            out[i] = out[i] + (t << (lsb - b - nbits));
            b += nbits;
            bits -= nbits;
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


void clear_words(void* mem, unsigned int nwords)
{ // Clear 32-bit words from memory. "nwords" indicates the number of words to be zeroed.
  // This function uses the volatile type qualifier to inform the compiler not to optimize out the memory clearing.
    volatile uint32_t *v = mem; 

    for (unsigned int i = 0; i < nwords; i++) {
        v[i] = 0;
    }
}
