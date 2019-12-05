#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <oqs/aes.h>
#include <oqs/sha2.h>
#include <oqs/sha3.h>

#include "system_info.c"

#define BUFFER_SIZE 10000

static int read_stdin(uint8_t **msg, size_t *msg_len) {
	*msg = malloc(BUFFER_SIZE);
	if (*msg == NULL) {
		return -1;
	}
	uint8_t *msg_next_read = *msg;
	size_t bytes_read;
	*msg_len = 0;
	while (1) {
		bytes_read = read(0, msg_next_read, BUFFER_SIZE);
		*msg_len += bytes_read;
		if (bytes_read < BUFFER_SIZE) {
			break;
		} else {
			uint8_t *msgprime = malloc(*msg_len + BUFFER_SIZE);
			if (msgprime == NULL) {
				return -1;
			}
			memcpy(msgprime, *msg, *msg_len);
			free(*msg);
			*msg = msgprime;
			msg_next_read = &((*msg)[*msg_len]);
		}
	}
	return 0;
}

static void print_hex(uint8_t *s, size_t l) {
	for (size_t i = 0; i < l; i++) {
		printf("%02x", s[i]);
	}
	printf("\n");
}

int do_sha256(void) {
	// read message from stdin
	uint8_t *msg;
	size_t msg_len;
	if (read_stdin(&msg, &msg_len) != 0) {
		fprintf(stderr, "ERROR: malloc failure\n");
		return -1;
	}
	// run main SHA-256 API
	uint8_t output[32];
	OQS_SHA2_sha256(output, msg, msg_len);
	// run incremental SHA-256 API
	uint8_t output_inc[32];
	void *state;
	OQS_SHA2_sha256_inc_init(&state);
	// clone state
	void *state2;
	OQS_SHA2_sha256_inc_clone_state(&state2, state);
	// hash with first state
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc_blocks(state, msg, 1);
		OQS_SHA2_sha256_inc_finalize(output_inc, state, &msg[64], msg_len - 64);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc, state, msg, msg_len);
	}
	if (memcmp(output, output_inc, 32) != 0) {
		free(msg);
		fprintf(stderr, "ERROR: Incremental API does not match main API\n");
		return -2;
	}
	// hash with second state
	if (msg_len > 64) {
		OQS_SHA2_sha256_inc_blocks(state2, msg, 1);
		OQS_SHA2_sha256_inc_finalize(output_inc, state2, &msg[64], msg_len - 64);
	} else {
		OQS_SHA2_sha256_inc_finalize(output_inc, state2, msg, msg_len);
	}
	free(msg);
	if (memcmp(output, output_inc, 32) != 0) {
		fprintf(stderr, "ERROR: Incremental API with cloned state does not match main API\n");
		return -3;
	}
	print_hex(output, 32);
	return 0;
}

int do_arbitrary_hash(void (*hash)(uint8_t *, const uint8_t *, size_t), size_t hash_len) {
	// read message from stdin
	uint8_t *msg;
	size_t msg_len;
	if (read_stdin(&msg, &msg_len) != 0) {
		fprintf(stderr, "ERROR: malloc failure\n");
		return -1;
	}
	// run main SHA-256 API
	uint8_t *output = malloc(hash_len);
	hash(output, msg, msg_len);
	print_hex(output, hash_len);
	free(output);
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: test_hash algname\n");
		fprintf(stderr, "  algname: sha256, sha384, sha512\n");
		fprintf(stderr, "  test_hash reads input from stdin and outputs hash value as hex string to stdout");
		printf("\n");
		print_system_info();
		return EXIT_FAILURE;
	}

	char *hash_alg = argv[1];

	if (strcmp(hash_alg, "sha256") == 0) {
		return do_sha256();
	} else if (strcmp(hash_alg, "sha384") == 0) {
		return do_arbitrary_hash(&OQS_SHA2_sha384, 48);
	} else if (strcmp(hash_alg, "sha512") == 0) {
		return do_arbitrary_hash(&OQS_SHA2_sha512, 64);
	} else {
		fprintf(stderr, "ERROR: Test not implemented\n");
		return EXIT_FAILURE;
	}
}
