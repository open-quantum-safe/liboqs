/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: noise sampling functions
*********************************************************************************************/

#include "sha3/fips202.h"


void frodo_sample_n(uint16_t *s, const size_t n, const uint8_t *seed, const size_t s_seed, const uint16_t ctr) 
{ // Fills vector s with n samples from the noise distribution which requires 16 bits to sample. 
  // The distribution is specified by its CDF.
    unsigned int i, j;

    cshake((uint8_t*)s, n*sizeof(uint16_t), ctr, seed, (unsigned long long)s_seed);

    for (i = 0; i < n; ++i) {
        uint8_t sample = 0;
        uint16_t prnd = s[i] >> 1;    // Drop the least significant bit
        uint8_t sign = s[i] & 0x1;    // Pick the least significant bit

        // No need to compare with the last value.
        for (j = 0; j < (unsigned int)(CDF_TABLE_LEN - 1); j++) {
            // Constant time comparison: 1 if CDF_TABLE[j] < s, 0 otherwise. Uses the fact that CDF_TABLE[j] and s fit in 15 bits.
            sample += (uint16_t)(CDF_TABLE[j] - prnd) >> 15;
        }
        // Assuming that sign is either 0 or 1, flips sample iff sign = 1
        s[i] = ((-sign) ^ sample) + sign;
    }
}
