// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <oqs/aes.h>
#include <oqs/sha2.h>
#include <oqs/sha3.h>

#include "system_info.c"

#define BUFFER_SIZE 30000

static bool sha2_callback_called = false;

static int read_stdin(uint8_t **msg, size_t *msg_len) {
	*msg = OQS_MEM_malloc(BUFFER_SIZE);
	if (*msg == NULL) {
		return -1;
	}
	size_t bytes_read;
	bytes_read = fread(*msg, 1, BUFFER_SIZE, stdin);
	if (ferror(stdin)) {
		perror("Read from stdin failed");
		return -2;
	}
	if (bytes_read == BUFFER_SIZE && !feof(stdin)) {
		fprintf(stderr, "Input too large for buffer (%d)\n", BUFFER_SIZE);
		return -3;
	}
	*msg_len = bytes_read;
	return 0;
}

static void print_hex(uint8_t *s, size_t l) {
	for (size_t i = 0; i < l; i++) {
		printf("%02x", s[i]);
	}
	printf("\n");
}

static int do_sha256(void) {
	// read message from stdin
	uint8_t *msg;
	size_t msg_len;
	if (read_stdin(&msg, &msg_len) != 0) {
		fprintf(stderr, "ERROR reading from stdin\n");
		return -1;
	}

	// run main SHA-256 API
	uint8_t output[32];
	OQS_SHA2_sha256(output, msg, msg_len);

	// run incremental SHA-256 API
	uint8_t output_inc[32];
	uint8_t output_inc_2[32];
	OQS_SHA2_sha256_ctx state;
	OQS_SHA2_sha256_inc_init(&state);

	// clone state
	OQS_SHA2_sha256_ctx state2, state3, state4, state5, state6;
	OQS_SHA2_sha256_inc_ctx_clone(&state2, &state);
	OQS_SHA2_sha256_inc_ctx_clone(&state3, &state);
	OQS_SHA2_sha256_inc_ctx_clone(&state4, &state);
	OQS_SHA2_sha256_inc_ctx_clone(&state5, &state);
	OQS_SHA2_sha256_inc_ctx_clone(&state6, &state);

	// hash with first state
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc_blocks(&state, msg, 1);
		OQS_SHA2_sha256_inc_finalize(output_inc, &state, &msg[64], msg_len - 64);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc, &state, msg, msg_len);
	}

	if (memcmp(output, output_inc, 32) != 0) {
		fprintf(stderr, "ERROR: Incremental API does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -2;
	}
	// hash with second state
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc_blocks(&state2, msg, 1);
		OQS_SHA2_sha256_inc_finalize(output_inc, &state2, &msg[64], msg_len - 64);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc, &state2, msg, msg_len);
	}
	if (memcmp(output, output_inc, 32) != 0) {
		fprintf(stderr, "ERROR: Incremental API with cloned state does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -3;
	}

	// hash with increment 1 byte at a time
	size_t i = 0;
	for (i = 0; i < msg_len; i++) {
		OQS_SHA2_sha256_inc(&state3, &msg[i], 1);
	}
	OQS_SHA2_sha256_inc_finalize(output_inc_2, &state3, &msg[i], 0);
	if (memcmp(output, output_inc_2, 32) != 0) {
		fprintf(stderr, "ERROR: Non-block Incremental API with cloned state does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -4;
	}

	// hash increment with the entire msg len
	OQS_SHA2_sha256_inc(&state6, msg, msg_len);
	OQS_SHA2_sha256_inc_finalize(output_inc, &state6, NULL, 0);
	if (memcmp(output, output_inc, 32) != 0) {
		fprintf(stderr, "ERROR: Incremental API with the entire msg.\n");
		OQS_MEM_insecure_free(msg);
		return -3;
	}

	// hash with combination of block-size increments and non block-size increments  [64 bytes] + [n < 64 bytes]
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc_blocks(&state4, msg, 1);
		for (i = 0; i < (msg_len - 64); i++) {
			OQS_SHA2_sha256_inc(&state4, &msg[64 + i], 1);
		}
		OQS_SHA2_sha256_inc_finalize(output_inc_2, &state4, &msg[msg_len - 1], 0);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc_2, &state4, msg, msg_len);
	}
	if (memcmp(output, output_inc_2, 32) != 0) {
		fprintf(stderr, "ERROR: Combined block increments with non-block size failed to match main API\n");
		OQS_MEM_insecure_free(msg);
		return -5;
	}

	// hash with combination of non block-size and block-size [n < 64 bytes] + [64 bytes]
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc(&state5, msg, 1);
		OQS_SHA2_sha256_inc_blocks(&state5, &msg[1], 1);
		OQS_SHA2_sha256_inc_finalize(output_inc_2, &state5, &msg[65], msg_len - 65);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc_2, &state5, msg, msg_len);
	}
	if (memcmp(output, output_inc_2, 32) != 0) {
		fprintf(stderr, "ERROR: Combined non-block size and block increments failed to match main API\n");
		OQS_MEM_insecure_free(msg);
		return -5;
	}
	//Test inc API
	print_hex(output, 32);
	OQS_MEM_insecure_free(msg);
	return 0;
}

static int do_sha384(void) {
	// read message from stdin
	uint8_t *msg;
	size_t msg_len;
	if (read_stdin(&msg, &msg_len) != 0) {
		fprintf(stderr, "ERROR reading from stdin\n");
		return -1;
	}
	// run main SHA-384 API
	uint8_t output[48];
	OQS_SHA2_sha384(output, msg, msg_len);
	// run incremental SHA-384 API
	uint8_t output_inc[48];
	OQS_SHA2_sha384_ctx state;
	OQS_SHA2_sha384_inc_init(&state);
	// clone state
	OQS_SHA2_sha384_ctx state2;
	OQS_SHA2_sha384_inc_ctx_clone(&state2, &state);
	// hash with first state
	if (msg_len > 128) {
		OQS_SHA2_sha384_inc_blocks(&state, msg, 1);
		OQS_SHA2_sha384_inc_finalize(output_inc, &state, &msg[128], msg_len - 128);
	} else {
		OQS_SHA2_sha384_inc_finalize(output_inc, &state, msg, msg_len);
	}
	if (memcmp(output, output_inc, 48) != 0) {
		fprintf(stderr, "ERROR: Incremental API does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -2;
	}
	// hash with second state
	if (msg_len > 128) {
		OQS_SHA2_sha384_inc_blocks(&state2, msg, 1);
		OQS_SHA2_sha384_inc_finalize(output_inc, &state2, &msg[128], msg_len - 128);
	} else {
		OQS_SHA2_sha384_inc_finalize(output_inc, &state2, msg, msg_len);
	}
	if (memcmp(output, output_inc, 48) != 0) {
		fprintf(stderr, "ERROR: Incremental API with cloned state does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -3;
	}
	print_hex(output, 48);
	OQS_MEM_insecure_free(msg);
	return 0;
}

static int do_sha512(void) {
	// read message from stdin
	uint8_t *msg;
	size_t msg_len;
	if (read_stdin(&msg, &msg_len) != 0) {
		fprintf(stderr, "ERROR reading from stdin\n");
		return -1;
	}
	// run main SHA-512 API
	uint8_t output[64];
	OQS_SHA2_sha512(output, msg, msg_len);
	// run incremental SHA-512 API
	uint8_t output_inc[64];
	OQS_SHA2_sha512_ctx state;
	OQS_SHA2_sha512_inc_init(&state);
	// clone state
	OQS_SHA2_sha512_ctx state2;
	OQS_SHA2_sha512_inc_ctx_clone(&state2, &state);
	// hash with first state
	if (msg_len > 128) {
		OQS_SHA2_sha512_inc_blocks(&state, msg, 1);
		OQS_SHA2_sha512_inc_finalize(output_inc, &state, &msg[128], msg_len - 128);
	} else {
		OQS_SHA2_sha512_inc_finalize(output_inc, &state, msg, msg_len);
	}
	if (memcmp(output, output_inc, 64) != 0) {
		fprintf(stderr, "ERROR: Incremental API does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -2;
	}
	// hash with second state
	if (msg_len > 128) {
		OQS_SHA2_sha512_inc_blocks(&state2, msg, 1);
		OQS_SHA2_sha512_inc_finalize(output_inc, &state2, &msg[128], msg_len - 128);
	} else {
		OQS_SHA2_sha512_inc_finalize(output_inc, &state2, msg, msg_len);
	}
	if (memcmp(output, output_inc, 64) != 0) {
		fprintf(stderr, "ERROR: Incremental API with cloned state does not match main API\n");
		OQS_MEM_insecure_free(msg);
		return -3;
	}
	print_hex(output, 64);
	OQS_MEM_insecure_free(msg);
	return 0;
}

static int do_arbitrary_hash(void (*hash)(uint8_t *, const uint8_t *, size_t), size_t hash_len) {
	// read message from stdin
	uint8_t *msg;
	size_t msg_len;
	if (read_stdin(&msg, &msg_len) != 0) {
		fprintf(stderr, "ERROR reading from stdin\n");
		return -1;
	}
	// run main SHA-256 API
	uint8_t *output = OQS_MEM_malloc(hash_len);
	hash(output, msg, msg_len);
	print_hex(output, hash_len);
	OQS_MEM_insecure_free(output);
	OQS_MEM_insecure_free(msg);
	return 0;
}

extern struct OQS_SHA2_callbacks sha2_default_callbacks;

static void override_SHA2_sha256_inc_init(OQS_SHA2_sha256_ctx *state) {
	sha2_callback_called = true;
	sha2_default_callbacks.SHA2_sha256_inc_init(state);
}

int main(int argc, char **argv) {
	int ret;
	struct OQS_SHA2_callbacks sha2_callbacks = sha2_default_callbacks;

	sha2_callbacks.SHA2_sha256_inc_init = override_SHA2_sha256_inc_init;
	OQS_SHA2_set_callbacks(&sha2_callbacks);

	OQS_init();
	if (argc != 2) {
		fprintf(stderr, "Usage: test_hash algname\n");
		fprintf(stderr, "  algname: sha256, sha384, sha512, sha256inc, sha384inc, sha512inc\n");
		fprintf(stderr, "           sha3_256, sha3_384, sha3_512\n");
		fprintf(stderr, "  test_hash reads input from stdin and outputs hash value as hex string to stdout");
		printf("\n");
		print_system_info();
		return EXIT_FAILURE;
	}

	char *hash_alg = argv[1];

	if (strcmp(hash_alg, "sha256inc") == 0) {
		ret = do_sha256();
	} else if (strcmp(hash_alg, "sha384inc") == 0) {
		ret = do_sha384();
	} else if (strcmp(hash_alg, "sha512inc") == 0) {
		ret = do_sha512();
	} else if (strcmp(hash_alg, "sha256") == 0) {
		ret = do_arbitrary_hash(&OQS_SHA2_sha256, 32);
	} else if (strcmp(hash_alg, "sha384") == 0) {
		ret = do_arbitrary_hash(&OQS_SHA2_sha384, 48);
	} else if (strcmp(hash_alg, "sha512") == 0) {
		ret = do_arbitrary_hash(&OQS_SHA2_sha512, 64);
	} else if (strcmp(hash_alg, "sha3_256") == 0) {
		ret = do_arbitrary_hash(&OQS_SHA3_sha3_256, 32);
	} else if (strcmp(hash_alg, "sha3_384") == 0) {
		ret = do_arbitrary_hash(&OQS_SHA3_sha3_384, 48);
	} else if (strcmp(hash_alg, "sha3_512") == 0) {
		ret = do_arbitrary_hash(&OQS_SHA3_sha3_512, 64);
	} else {
		fprintf(stderr, "ERROR: Test not implemented\n");
		ret = EXIT_FAILURE;
	}

	if (strcmp(hash_alg, "sha256inc") == 0 && !sha2_callback_called) {
		fprintf(stderr, "ERROR: SHA2 callback was not called\n");
		ret = EXIT_FAILURE;
	}

	OQS_destroy();
	return ret;
}
