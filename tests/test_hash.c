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

static int read_stdin(uint8_t **msg, size_t *msg_len) {
	*msg = malloc(BUFFER_SIZE);
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
	OQS_SHA2_sha256_ctx state;
	OQS_SHA2_sha256_inc_init(&state);
	// clone state
	OQS_SHA2_sha256_ctx state2;
	OQS_SHA2_sha256_inc_ctx_clone(&state2, &state);
	// hash with first state
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc_blocks(&state, msg, 1);
		OQS_SHA2_sha256_inc_finalize(output_inc, &state, &msg[64], msg_len - 64);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc, &state, msg, msg_len);
	}
	if (memcmp(output, output_inc, 32) != 0) {
		fprintf(stderr, "ERROR: Incremental API does not match main API\n");
		free(msg);
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
		free(msg);
		return -3;
	}
	print_hex(output, 32);
	free(msg);
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
		free(msg);
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
		free(msg);
		return -3;
	}
	print_hex(output, 48);
	free(msg);
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
		free(msg);
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
		free(msg);
		return -3;
	}
	print_hex(output, 64);
	free(msg);
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
	uint8_t *output = malloc(hash_len);
	hash(output, msg, msg_len);
	print_hex(output, hash_len);
	free(output);
	free(msg);
	return 0;
}

int main(int argc, char **argv) {
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
		return do_sha256();
	} else if (strcmp(hash_alg, "sha384inc") == 0) {
		return do_sha384();
	} else if (strcmp(hash_alg, "sha512inc") == 0) {
		return do_sha512();
	} else if (strcmp(hash_alg, "sha256") == 0) {
		return do_arbitrary_hash(&OQS_SHA2_sha256, 32);
	} else if (strcmp(hash_alg, "sha384") == 0) {
		return do_arbitrary_hash(&OQS_SHA2_sha384, 48);
	} else if (strcmp(hash_alg, "sha512") == 0) {
		return do_arbitrary_hash(&OQS_SHA2_sha512, 64);
	} else if (strcmp(hash_alg, "sha3_256") == 0) {
		return do_arbitrary_hash(&OQS_SHA3_sha3_256, 32);
	} else if (strcmp(hash_alg, "sha3_384") == 0) {
		return do_arbitrary_hash(&OQS_SHA3_sha3_384, 48);
	} else if (strcmp(hash_alg, "sha3_512") == 0) {
		return do_arbitrary_hash(&OQS_SHA3_sha3_512, 64);
	} else {
		fprintf(stderr, "ERROR: Test not implemented\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}
}
