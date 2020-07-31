/* These functions should help with endianness-safe conversions */

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC2561CCA2_LEAKTIME_store8(unsigned char *out, uint64_t in);
uint64_t PQCLEAN_HQC2561CCA2_LEAKTIME_load8(const unsigned char *in);
void PQCLEAN_HQC2561CCA2_LEAKTIME_load8_arr(uint64_t *out64, size_t outlen, const uint8_t *in8, size_t inlen);
void PQCLEAN_HQC2561CCA2_LEAKTIME_store8_arr(uint8_t *out8, size_t outlen, const uint64_t *in64, size_t inlen);
