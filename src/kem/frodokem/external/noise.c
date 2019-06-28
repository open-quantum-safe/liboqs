/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: noise sampling functions
*********************************************************************************************/

void frodo_sample_n(uint16_t *s, const size_t n) 
{ // Fills vector s with n samples from the noise distribution which requires 16 bits to sample. 
  // The distribution is specified by its CDF.
  // Input: pseudo-random values (2*n bytes) passed in s. The input is overwritten by the output.
    unsigned int i, j;
    uint16_t cdf_table[CDF_TABLE_LEN] = CDF_TABLE;

    for (i = 0; i < n; ++i) {
        uint16_t sample = 0;
        uint16_t prnd = s[i] >> 1;    // Drop the least significant bit
        uint16_t sign = s[i] & 0x1;    // Pick the least significant bit

        // No need to compare with the last value.
        for (j = 0; j < (unsigned int)(CDF_TABLE_LEN - 1); j++) {
            // Constant time comparison: 1 if CDF_TABLE[j] < s, 0 otherwise. Uses the fact that CDF_TABLE[j] and s fit in 15 bits.
            sample += (uint16_t)(cdf_table[j] - prnd) >> 15;
        }
        // Assuming that sign is either 0 or 1, flips sample iff sign = 1
        s[i] = ((-sign) ^ sample) + sign;
    }
}
