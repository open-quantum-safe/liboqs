#include <string.h>
#include <assert.h>

#include "snova_common.h"

alignas(32) uint8_t mt4b[256] = {0};
alignas(32) uint8_t inv4b[16] = {0};

int ct_is_negative(int val) {
	return ((val >> 31) & 1);
}

// Constant time version of: (val != 0)
uint32_t ct_gf16_is_not_zero(uint8_t val) {
	return (val | (val >> 1) | (val >> 2) | (val >> 3)) & 1;
}

uint32_t ct_xgf16_is_not_zero(uint32_t val) {
	return (val | (val >> 3) | (val >> 6) | (val >> 9)) & 1;
}

void snova_set_zero(void *ptr, size_t size) {
	memset(ptr, 0, size);
}

/**
 * shake256
 * @param pt_seed_array - Pointer to the hash input.
 * @param input_bytes - hash lenth.
 * @param pt_output_array - Pointer to the hash output.
 * @param output_bytes - hash input.
 */
void shake256(const uint8_t *pt_seed_array, int input_bytes, uint8_t *pt_output_array, int output_bytes) {
	Keccak_HashInstance hashInstance;
	Keccak_HashInitialize_SHAKE256(&hashInstance);
	Keccak_HashUpdate(&hashInstance, pt_seed_array, 8 * input_bytes);
	Keccak_HashFinal(&hashInstance, NULL);
	Keccak_HashSqueeze(&hashInstance, pt_output_array, 8 * output_bytes);
}

/**
 * Convert one byte of data to GF16 representation (using only half of the
 * byte). Example: <bytes 12 34 56 78 9a bc> -> <bytes 02 01 04 03 05 ..... 0c
 * 0b>
 * @param byte_array - input (bytes)
 * @param gf16_array - output (GF16)
 * @param num_of_GF16s - GF16 amount
 */
void convert_bytes_to_GF16s(const uint8_t *byte_array, uint8_t *gf16_array, int num_of_GF16s) {
	int i;
	int pairs = num_of_GF16s >> 1;

	// Convert each byte into two GF16 values
	for (i = 0; i < pairs; ++i) {
		gf16_array[i * 2] = byte_array[i] & 0x0F;
		gf16_array[i * 2 + 1] = (byte_array[i] >> 4) & 0x0F;
	}

	// Handle the last GF16 value if num_of_GF16s is odd
	if (num_of_GF16s % 2 == 1) {
		gf16_array[num_of_GF16s - 1] = byte_array[pairs] & 0x0F;
	}
}

/**
 * Convert two GF16 values to one byte.
 * Example:
 *  <bytes 02 01 04 03 05 ..... 0c 0b> -> <bytes 12 34 56 78 9a bc>
 * @param byte_array - output (bytes)
 * @param gf16_array - input (GF16)
 * @param num_of_GF16s - GF16 amount
 */
void convert_GF16s_to_bytes(uint8_t *byte_array, const uint8_t *gf16_array, int num_of_GF16s) {
	int i;
	int pairs = num_of_GF16s >> 1;

	// Convert pairs of GF16 values into one byte
	for (i = 0; i < pairs; ++i) {
		byte_array[i] = gf16_array[i * 2] | (gf16_array[i * 2 + 1] << 4);
	}

	// Handle the last GF16 value if num_of_GF16s is odd
	if (num_of_GF16s % 2 == 1) {
		byte_array[pairs] = gf16_array[num_of_GF16s - 1];
	}
}

/**
 * Convert one byte of data to GF16 representation (using only half of the
 * byte). cut_in_half Example: <bytes 12 34 56 78 9a bc> -> <bytes 02 04 06 08
 * 0a 0c 01 03 05 07 09 0b>
 * @param byte_array - input (bytes)
 * @param gf16_array - output (GF16)
 * @param num_of_GF16s - GF16 amount
 */
void convert_bytes_to_GF16s_cut_in_half(const uint8_t *byte_array, uint8_t *gf16_array, int num_of_GF16s) {
	int half_GF16s = (num_of_GF16s + 1) >> 1;
	int i;

	// Extract the lower 4 bits of each byte to the first half of gf16_array
	for (i = 0; i < half_GF16s; ++i) {
		gf16_array[i] = byte_array[i] & 0x0F;
	}

	// Extract the upper 4 bits of each byte to the second half of gf16_array
	for (i = 0; i < (num_of_GF16s >> 1); ++i) {
		gf16_array[i + half_GF16s] = byte_array[i] >> 4;
	}
}

/**
 * Convert two GF16 values to one byte.
 * Example:
 *  <bytes 02 04 06 08 0a 0c 01 03 05 07 09 0b> -> <bytes 12 34 56 78 9a bc>
 * @param byte_array - output (bytes)
 * @param gf16_array - input (GF16)
 * @param num_of_GF16s - GF16 amount
 */
void convert_GF16s_to_bytes_merger_in_half(uint8_t *byte_array, uint8_t *gf16_array, int num_of_GF16s) {
	int half_GF16s = (num_of_GF16s + 1) >> 1;
	int i;

	// Combine pairs of GF16 values into one byte
	for (i = 0; i < (num_of_GF16s >> 1); ++i) {
		byte_array[i] = gf16_array[i] | (gf16_array[i + half_GF16s] << 4);
	}

	// If num_of_GF16s is odd, handle the last GF16 value separately
	if (num_of_GF16s & 1) {
		byte_array[i] = gf16_array[i];
	}
}
