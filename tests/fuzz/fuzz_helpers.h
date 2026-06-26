// SPDX-License-Identifier: MIT

#ifndef OQS_TESTS_FUZZ_HELPERS_H
#define OQS_TESTS_FUZZ_HELPERS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define OQS_FUZZ_HEADER_SIZE 5
#define OQS_FUZZ_MAX_MESSAGE_LEN 1024
#define OQS_FUZZ_MAX_CTX_LEN 256
#define OQS_FUZZ_MAX_SERIALIZED_KEY_LEN 65536
#define OQS_FUZZ_SIGNATURE_SLACK 32

typedef struct {
	uint8_t version;
	uint16_t algorithm_index;
	uint8_t mode;
	uint8_t flags;
} oqs_fuzz_header;

typedef struct {
	const uint8_t *data;
	size_t size;
	size_t offset;
} oqs_fuzz_cursor;

typedef struct {
	uint8_t *public_key;
	size_t public_key_len;
	uint8_t *secret_key;
	size_t secret_key_len;
	uint8_t *message;
	size_t message_len;
	uint8_t *signature;
	size_t signature_len;
} oqs_fuzz_stfl_kat;

void oqs_fuzz_init(void);
void oqs_fuzz_seed_prng(uint64_t seed);
uint64_t oqs_fuzz_hash(const uint8_t *data, size_t data_len, uint64_t seed);

bool oqs_fuzz_read_header(const uint8_t *data, size_t data_len, oqs_fuzz_header *header, oqs_fuzz_cursor *cursor);
size_t oqs_fuzz_remaining(const oqs_fuzz_cursor *cursor);
uint8_t oqs_fuzz_take_u8(oqs_fuzz_cursor *cursor, uint8_t fallback);
uint32_t oqs_fuzz_take_u32(oqs_fuzz_cursor *cursor, uint32_t fallback);
size_t oqs_fuzz_take_size(oqs_fuzz_cursor *cursor, size_t max_value, size_t fallback);
void oqs_fuzz_copy_baseline(uint8_t *dst, size_t dst_len, const uint8_t *baseline, size_t baseline_len);
void oqs_fuzz_patch(uint8_t *dst, size_t dst_len, size_t start, oqs_fuzz_cursor *cursor, size_t max_bytes);

bool oqs_fuzz_load_stfl_kat(const char *method_name, oqs_fuzz_stfl_kat *kat);
void oqs_fuzz_free_stfl_kat(oqs_fuzz_stfl_kat *kat);
bool oqs_fuzz_make_lms_secret_key(const char *method_name, uint8_t *secret_key, size_t secret_key_len);

#endif
