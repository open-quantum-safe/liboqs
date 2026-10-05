// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_WIN32)
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <oqs/oqs.h>

#if defined(OQS_ALLOW_LMS_KEY_AND_SIG_GEN) && \
    defined(OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1) && \
    defined(OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8)
#define OQS_LMS_REGRESSIONS_ENABLED 1
#endif

static int failures = 0;

#define CHECK(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL: %s\n", msg); \
        failures++; \
    } \
} while (0)

#define REQUIRE(cond, msg) do { \
    if (!(cond)) { \
        CHECK(false, msg); \
        goto cleanup; \
    } \
} while (0)

#ifdef OQS_LMS_REGRESSIONS_ENABLED
/* context points to a size_t counting how many times the callback ran */
static OQS_STATUS store_success(uint8_t *sk_buf, size_t sk_buf_len, void *context) {
	(void)sk_buf;
	(void)sk_buf_len;
	(*(size_t *)context)++;
	return OQS_SUCCESS;
}

static OQS_STATUS store_failure(uint8_t *sk_buf, size_t sk_buf_len, void *context) {
	(void)sk_buf;
	(void)sk_buf_len;
	(*(size_t *)context)++;
	return OQS_ERROR;
}

static bool all_bytes_equal(const uint8_t *buf, size_t len, uint8_t value) {
	for (size_t i = 0; i < len; i++) {
		if (buf[i] != value) {
			return false;
		}
	}
	return true;
}

#if !defined(_WIN32)
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

typedef struct {
	uint8_t *mapping;
	size_t mapping_len;
	uint8_t *buf;
} guarded_buffer;

static int guarded_buffer_init(guarded_buffer *guard) {
	long page_size = sysconf(_SC_PAGESIZE);

	if (page_size <= 0) {
		return 0;
	}
	guard->mapping_len = (size_t)page_size * 2U;
	guard->mapping = mmap(NULL, guard->mapping_len, PROT_READ | PROT_WRITE,
	                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (guard->mapping == MAP_FAILED) {
		guard->mapping = NULL;
		return 0;
	}
	if (mprotect(guard->mapping + page_size, (size_t)page_size, PROT_NONE) != 0) {
		munmap(guard->mapping, guard->mapping_len);
		guard->mapping = NULL;
		return 0;
	}
	guard->buf = guard->mapping + page_size - 1;
	guard->buf[0] = 0;
	return 1;
}

static void guarded_buffer_free(guarded_buffer *guard) {
	if (guard->mapping != NULL) {
		munmap(guard->mapping, guard->mapping_len);
	}
}
#endif
#endif /* OQS_LMS_REGRESSIONS_ENABLED */

#ifdef OQS_LMS_REGRESSIONS_ENABLED
static void test_lms_contracts(void) {
	static const uint8_t message[] = "LMS security regression";
	OQS_SIG_STFL *large_sig = NULL, *small_sig = NULL;
	OQS_SIG_STFL_SECRET_KEY *large_sk = NULL, *small_sk = NULL;
	OQS_SIG_STFL_SECRET_KEY *error_sk = NULL, *exhaustion_sk = NULL;
	uint8_t *large_pk = NULL, *large_signature = NULL;
	uint8_t *small_pk = NULL, *small_signature = NULL;
	uint8_t *serialized = NULL;
	size_t large_signature_len = 0, small_signature_len = 0, serialized_len = 0;
	unsigned long long total = 0;
	size_t store_success_calls = 0, store_failure_calls = 0;
#if !defined(_WIN32)
	guarded_buffer guard = {0};
#endif

	large_sig = OQS_SIG_STFL_new(OQS_SIG_STFL_alg_lms_sha256_h5_w1);
	small_sig = OQS_SIG_STFL_new(OQS_SIG_STFL_alg_lms_sha256_h5_w8);
	large_sk = OQS_SIG_STFL_SECRET_KEY_new(OQS_SIG_STFL_alg_lms_sha256_h5_w1);
	small_sk = OQS_SIG_STFL_SECRET_KEY_new(OQS_SIG_STFL_alg_lms_sha256_h5_w8);
	error_sk = OQS_SIG_STFL_SECRET_KEY_new(OQS_SIG_STFL_alg_lms_sha256_h5_w8);
	exhaustion_sk = OQS_SIG_STFL_SECRET_KEY_new(OQS_SIG_STFL_alg_lms_sha256_h5_w8);
	REQUIRE(large_sig != NULL && small_sig != NULL && large_sk != NULL &&
	        small_sk != NULL && error_sk != NULL && exhaustion_sk != NULL,
	        "LMS object construction");

	large_pk = OQS_MEM_malloc(large_sig->length_public_key);
	large_signature = OQS_MEM_malloc(large_sig->length_signature);
	small_pk = OQS_MEM_malloc(small_sig->length_public_key);
	small_signature = OQS_MEM_malloc(small_sig->length_signature + 32U);
	REQUIRE(large_pk != NULL && large_signature != NULL && small_pk != NULL &&
	        small_signature != NULL, "LMS buffer allocation");

	REQUIRE(OQS_SIG_STFL_keypair(large_sig, large_pk, large_sk) == OQS_SUCCESS,
	        "LMS large keypair");
	REQUIRE(OQS_SIG_STFL_SECRET_KEY_serialize(&serialized, &serialized_len, large_sk) == OQS_SUCCESS,
	        "LMS large key serialization");
	CHECK(OQS_SIG_STFL_SECRET_KEY_deserialize(small_sk, serialized, serialized_len, NULL) == OQS_ERROR,
	      "LMS rejects cross-parameter secret-key deserialize");

	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(large_sk, store_success, &store_success_calls);
	REQUIRE(OQS_SIG_STFL_sign(large_sig, large_signature, &large_signature_len,
	                          message, sizeof(message) - 1U, large_sk) == OQS_SUCCESS,
	        "LMS large sign");
	CHECK(store_success_calls == 1, "LMS stores the updated key once per signature");
	CHECK(OQS_SIG_STFL_verify(large_sig, message, sizeof(message) - 1U,
	                          large_signature, large_signature_len, large_pk) == OQS_SUCCESS,
	      "LMS self verification");
	CHECK(OQS_SIG_STFL_verify(small_sig, message, sizeof(message) - 1U,
	                          large_signature, large_signature_len, large_pk) == OQS_ERROR,
	      "LMS rejects cross-parameter verification");

	REQUIRE(OQS_SIG_STFL_keypair(small_sig, small_pk, small_sk) == OQS_SUCCESS,
	        "LMS short-signature keypair");
#if !defined(_WIN32)
	REQUIRE(guarded_buffer_init(&guard), "LMS guard-page allocation");
	CHECK(OQS_SIG_STFL_verify(small_sig, message, sizeof(message) - 1U,
	                          guard.buf, 1U, small_pk) == OQS_ERROR,
	      "LMS rejects short signatures before reading");
	guarded_buffer_free(&guard);
	guard.mapping = NULL;
#endif

	REQUIRE(OQS_SIG_STFL_keypair(small_sig, small_pk, error_sk) == OQS_SUCCESS,
	        "LMS error-path keypair");
	memset(small_signature, 0xA5, small_sig->length_signature + 32U);
	small_signature_len = small_sig->length_signature + 32U;
	CHECK(OQS_SIG_STFL_sign(small_sig, small_signature, &small_signature_len,
	                        message, sizeof(message) - 1U, error_sk) == OQS_ERROR,
	      "LMS sign without secure store fails");
	CHECK(small_signature_len == 0, "LMS early error zeroes output length");
	CHECK(all_bytes_equal(small_signature + small_sig->length_signature, 32U, 0xA5),
	      "LMS early error does not cleanse beyond output capacity");

	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(error_sk, store_failure, &store_failure_calls);
	memset(small_signature, 0xA5, small_sig->length_signature + 32U);
	small_signature_len = small_sig->length_signature + 32U;
	CHECK(OQS_SIG_STFL_sign(small_sig, small_signature, &small_signature_len,
	                        message, sizeof(message) - 1U, error_sk) == OQS_ERROR,
	      "LMS sign with failing secure store fails");
	CHECK(store_failure_calls == 1, "LMS invokes failing secure store once");
	CHECK(small_signature_len == 0, "LMS store failure zeroes output length");
	CHECK(all_bytes_equal(small_signature, small_sig->length_signature, 0),
	      "LMS store failure cleanses produced signature");
	CHECK(all_bytes_equal(small_signature + small_sig->length_signature, 32U, 0xA5),
	      "LMS store failure does not cleanse beyond output capacity");

	REQUIRE(OQS_SIG_STFL_keypair(small_sig, small_pk, exhaustion_sk) == OQS_SUCCESS,
	        "LMS exhaustion keypair");
	store_success_calls = 0;
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(exhaustion_sk, store_success, &store_success_calls);
	REQUIRE(OQS_SIG_STFL_sigs_total(small_sig, &total, exhaustion_sk) == OQS_SUCCESS,
	        "LMS sigs_total");
	CHECK(total == 32ULL, "LMS H5 reports all 32 usable leaves");
	for (unsigned long long i = 0; i < total; i++) {
		small_signature_len = 0;
		REQUIRE(OQS_SIG_STFL_sign(small_sig, small_signature, &small_signature_len,
		                          message, sizeof(message) - 1U, exhaustion_sk) == OQS_SUCCESS,
		        "LMS signs every advertised leaf");
	}
	CHECK(store_success_calls == total, "LMS stores the updated key after every signature");
	small_signature_len = small_sig->length_signature;
	CHECK(OQS_SIG_STFL_sign(small_sig, small_signature, &small_signature_len,
	                        message, sizeof(message) - 1U, exhaustion_sk) == OQS_ERROR,
	      "LMS exhausted key returns error");
	CHECK(small_signature_len == 0, "LMS exhausted key zeroes output length");
	CHECK(store_success_calls == total, "LMS exhausted key does not store");

cleanup:
#if !defined(_WIN32)
	guarded_buffer_free(&guard);
#endif
	OQS_MEM_secure_free(serialized, serialized_len);
	OQS_MEM_insecure_free(small_signature);
	OQS_MEM_insecure_free(small_pk);
	OQS_MEM_insecure_free(large_signature);
	OQS_MEM_insecure_free(large_pk);
	OQS_SIG_STFL_SECRET_KEY_free(exhaustion_sk);
	OQS_SIG_STFL_SECRET_KEY_free(error_sk);
	OQS_SIG_STFL_SECRET_KEY_free(small_sk);
	OQS_SIG_STFL_SECRET_KEY_free(large_sk);
	OQS_SIG_STFL_free(small_sig);
	OQS_SIG_STFL_free(large_sig);
}
#endif

int main(void) {
	OQS_init();
#ifdef OQS_LMS_REGRESSIONS_ENABLED
	test_lms_contracts();
#endif
	OQS_destroy();
	return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
