// SPDX-License-Identifier: MIT

#include "fuzz_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#ifndef OQS_FUZZ_SOURCE_DIR
#define OQS_FUZZ_SOURCE_DIR "."
#endif

#define OQS_FUZZ_LMS_PRIVATE_KEY_LEN 64
#define OQS_FUZZ_LMS_PARAM_OFFSET 8
#define OQS_FUZZ_LMS_PARAM_SLOTS 8

static uint64_t oqs_fuzz_prng_state = UINT64_C(0x9e3779b97f4a7c15);

static void oqs_fuzz_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	for (size_t i = 0; i < bytes_to_read; i++) {
		oqs_fuzz_prng_state ^= oqs_fuzz_prng_state >> 12;
		oqs_fuzz_prng_state ^= oqs_fuzz_prng_state << 25;
		oqs_fuzz_prng_state ^= oqs_fuzz_prng_state >> 27;
		oqs_fuzz_prng_state *= UINT64_C(0x2545f4914f6cdd1d);
		random_array[i] = (uint8_t)(oqs_fuzz_prng_state >> 56);
	}
}

void oqs_fuzz_init(void) {
	OQS_init();
	OQS_randombytes_custom_algorithm(&oqs_fuzz_randombytes);
}

void oqs_fuzz_seed_prng(uint64_t seed) {
	if (seed == 0) {
		seed = UINT64_C(0x9e3779b97f4a7c15);
	}
	oqs_fuzz_prng_state = seed;
}

uint64_t oqs_fuzz_hash(const uint8_t *data, size_t data_len, uint64_t seed) {
	uint64_t hash = seed ^ UINT64_C(0xcbf29ce484222325);
	for (size_t i = 0; i < data_len; i++) {
		hash ^= data[i];
		hash *= UINT64_C(0x100000001b3);
	}
	return hash;
}

bool oqs_fuzz_read_header(const uint8_t *data, size_t data_len, oqs_fuzz_header *header, oqs_fuzz_cursor *cursor) {
	if (data == NULL || header == NULL || cursor == NULL || data_len < OQS_FUZZ_HEADER_SIZE) {
		return false;
	}

	header->version = data[0];
	header->algorithm_index = (uint16_t)((uint16_t)data[1] | ((uint16_t)data[2] << 8));
	header->mode = data[3];
	header->flags = data[4];

	cursor->data = data;
	cursor->size = data_len;
	cursor->offset = OQS_FUZZ_HEADER_SIZE;
	return true;
}

size_t oqs_fuzz_remaining(const oqs_fuzz_cursor *cursor) {
	if (cursor == NULL || cursor->offset >= cursor->size) {
		return 0;
	}
	return cursor->size - cursor->offset;
}

uint8_t oqs_fuzz_take_u8(oqs_fuzz_cursor *cursor, uint8_t fallback) {
	if (oqs_fuzz_remaining(cursor) < 1) {
		return fallback;
	}
	return cursor->data[cursor->offset++];
}

uint32_t oqs_fuzz_take_u32(oqs_fuzz_cursor *cursor, uint32_t fallback) {
	if (oqs_fuzz_remaining(cursor) < 4) {
		return fallback;
	}

	uint32_t value = (uint32_t)cursor->data[cursor->offset];
	value |= (uint32_t)cursor->data[cursor->offset + 1] << 8;
	value |= (uint32_t)cursor->data[cursor->offset + 2] << 16;
	value |= (uint32_t)cursor->data[cursor->offset + 3] << 24;
	cursor->offset += 4;
	return value;
}

size_t oqs_fuzz_take_size(oqs_fuzz_cursor *cursor, size_t max_value, size_t fallback) {
	if (max_value == 0) {
		return 0;
	}
	if (oqs_fuzz_remaining(cursor) < 4) {
		return fallback <= max_value ? fallback : max_value;
	}

	uint32_t value = oqs_fuzz_take_u32(cursor, 0);
	return (size_t)value % (max_value + 1);
}

void oqs_fuzz_copy_baseline(uint8_t *dst, size_t dst_len, const uint8_t *baseline, size_t baseline_len) {
	if (dst == NULL || dst_len == 0) {
		return;
	}

	memset(dst, 0, dst_len);
	if (baseline != NULL && baseline_len > 0) {
		size_t copy_len = baseline_len < dst_len ? baseline_len : dst_len;
		memcpy(dst, baseline, copy_len);
	}
}

void oqs_fuzz_patch(uint8_t *dst, size_t dst_len, size_t start, oqs_fuzz_cursor *cursor, size_t max_bytes) {
	if (dst == NULL || dst_len == 0 || cursor == NULL || max_bytes == 0) {
		return;
	}

	size_t remaining = oqs_fuzz_remaining(cursor);
	size_t patch_len = remaining < max_bytes ? remaining : max_bytes;
	if (patch_len > dst_len) {
		patch_len = dst_len;
	}
	if (patch_len == 0) {
		return;
	}

	size_t offset = start % dst_len;
	for (size_t i = 0; i < patch_len; i++) {
		dst[(offset + i) % dst_len] = cursor->data[cursor->offset + i];
	}
	cursor->offset += patch_len;
}

static bool oqs_fuzz_build_stfl_kat_path(const char *method_name, char *path, size_t path_len) {
	if (method_name == NULL || path == NULL || path_len == 0) {
		return false;
	}

	const char *family = strncmp(method_name, "LMS_", 4) == 0 ? "lms" : "xmss";
	char filename[128];
	size_t method_len = strlen(method_name);
	if (method_len + 5 >= sizeof(filename)) {
		return false;
	}

	for (size_t i = 0; i < method_len; i++) {
		filename[i] = method_name[i] == '/' ? '-' : method_name[i];
	}
	memcpy(filename + method_len, ".rsp", 5);

	int written = snprintf(path, path_len, "%s/tests/KATs/sig_stfl/%s/%s", OQS_FUZZ_SOURCE_DIR, family, filename);
	return written > 0 && (size_t)written < path_len;
}

static bool oqs_fuzz_read_file(const char *path, char **contents, size_t *contents_len) {
	if (path == NULL || contents == NULL || contents_len == NULL) {
		return false;
	}

	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return false;
	}
	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		return false;
	}
	long file_len = ftell(file);
	if (file_len < 0 || fseek(file, 0, SEEK_SET) != 0) {
		fclose(file);
		return false;
	}

	char *buffer = OQS_MEM_malloc((size_t)file_len + 1);
	if (buffer == NULL) {
		fclose(file);
		return false;
	}

	size_t read_len = fread(buffer, 1, (size_t)file_len, file);
	fclose(file);
	if (read_len != (size_t)file_len) {
		OQS_MEM_insecure_free(buffer);
		return false;
	}

	buffer[read_len] = '\0';
	*contents = buffer;
	*contents_len = read_len;
	return true;
}

static int oqs_fuzz_hex_nibble(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'a' && c <= 'f') {
		return 10 + c - 'a';
	}
	if (c >= 'A' && c <= 'F') {
		return 10 + c - 'A';
	}
	return -1;
}

static bool oqs_fuzz_extract_hex_field(const char *contents, const char *field, uint8_t **value, size_t *value_len) {
	if (contents == NULL || field == NULL || value == NULL || value_len == NULL) {
		return false;
	}

	char marker[32];
	int marker_len = snprintf(marker, sizeof(marker), "%s = ", field);
	if (marker_len <= 0 || (size_t)marker_len >= sizeof(marker)) {
		return false;
	}

	const char *start = strstr(contents, marker);
	if (start == NULL) {
		return false;
	}
	start += marker_len;

	const char *end = start;
	while (*end != '\0' && *end != '\n' && *end != '\r') {
		end++;
	}
	size_t hex_len = (size_t)(end - start);
	if (hex_len == 0 || (hex_len % 2) != 0) {
		return false;
	}

	size_t output_len = hex_len / 2;
	uint8_t *output = OQS_MEM_malloc(output_len);
	if (output == NULL) {
		return false;
	}

	for (size_t i = 0; i < output_len; i++) {
		int high = oqs_fuzz_hex_nibble(start[2 * i]);
		int low = oqs_fuzz_hex_nibble(start[2 * i + 1]);
		if (high < 0 || low < 0) {
			OQS_MEM_insecure_free(output);
			return false;
		}
		output[i] = (uint8_t)((high << 4) | low);
	}

	*value = output;
	*value_len = output_len;
	return true;
}

bool oqs_fuzz_load_stfl_kat(const char *method_name, oqs_fuzz_stfl_kat *kat) {
	if (method_name == NULL || kat == NULL) {
		return false;
	}

	memset(kat, 0, sizeof(*kat));
	char path[512];
	if (!oqs_fuzz_build_stfl_kat_path(method_name, path, sizeof(path))) {
		return false;
	}

	char *contents = NULL;
	size_t contents_len = 0;
	if (!oqs_fuzz_read_file(path, &contents, &contents_len)) {
		return false;
	}
	(void)contents_len;

	(void)oqs_fuzz_extract_hex_field(contents, "pk", &kat->public_key, &kat->public_key_len);
	(void)oqs_fuzz_extract_hex_field(contents, "sk", &kat->secret_key, &kat->secret_key_len);
	(void)oqs_fuzz_extract_hex_field(contents, "msg", &kat->message, &kat->message_len);
	(void)oqs_fuzz_extract_hex_field(contents, "sm", &kat->signature, &kat->signature_len);
	OQS_MEM_insecure_free(contents);

	return kat->public_key != NULL || kat->secret_key != NULL || kat->message != NULL || kat->signature != NULL;
}

void oqs_fuzz_free_stfl_kat(oqs_fuzz_stfl_kat *kat) {
	if (kat == NULL) {
		return;
	}

	OQS_MEM_insecure_free(kat->public_key);
	OQS_MEM_secure_free(kat->secret_key, kat->secret_key_len);
	OQS_MEM_insecure_free(kat->message);
	OQS_MEM_insecure_free(kat->signature);
	memset(kat, 0, sizeof(*kat));
}

static uint8_t oqs_fuzz_lms_height_code(unsigned long height) {
	switch (height) {
	case 5:
		return 5;
	case 10:
		return 6;
	case 15:
		return 7;
	case 20:
		return 8;
	case 25:
		return 9;
	default:
		return 0;
	}
}

static uint8_t oqs_fuzz_lms_width_code(unsigned long width) {
	switch (width) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 4:
		return 3;
	case 8:
		return 4;
	default:
		return 0;
	}
}

bool oqs_fuzz_make_lms_secret_key(const char *method_name, uint8_t *secret_key, size_t secret_key_len) {
	if (method_name == NULL || secret_key == NULL || secret_key_len < OQS_FUZZ_LMS_PRIVATE_KEY_LEN || strncmp(method_name, "LMS_SHA256_", 11) != 0) {
		return false;
	}

	memset(secret_key, 0, secret_key_len);
	memset(secret_key + OQS_FUZZ_LMS_PARAM_OFFSET, 0xff, OQS_FUZZ_LMS_PARAM_SLOTS);
	for (size_t i = OQS_FUZZ_LMS_PARAM_OFFSET + OQS_FUZZ_LMS_PARAM_SLOTS; i < OQS_FUZZ_LMS_PRIVATE_KEY_LEN; i++) {
		secret_key[i] = (uint8_t)(0xa5u ^ (uint8_t)i);
	}

	const char *cursor = method_name + 11;
	size_t level = 0;
	while (*cursor != '\0' && level < OQS_FUZZ_LMS_PARAM_SLOTS) {
		if (cursor[0] != 'H') {
			return false;
		}
		char *end = NULL;
		unsigned long height = strtoul(cursor + 1, &end, 10);
		if (end == NULL || end[0] != '_' || end[1] != 'W') {
			return false;
		}
		unsigned long width = strtoul(end + 2, &end, 10);
		uint8_t height_code = oqs_fuzz_lms_height_code(height);
		uint8_t width_code = oqs_fuzz_lms_width_code(width);
		if (height_code == 0 || width_code == 0) {
			return false;
		}
		secret_key[OQS_FUZZ_LMS_PARAM_OFFSET + level] = (uint8_t)((height_code << 4) | width_code);
		level++;
		if (*end == '\0') {
			break;
		}
		if (*end != '_') {
			return false;
		}
		cursor = end + 1;
	}

	return level > 0;
}
