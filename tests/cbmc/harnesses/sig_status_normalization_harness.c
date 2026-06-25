// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

extern _Bool nondet_bool(void);
extern int nondet_int(void);
extern size_t nondet_size_t(void);
extern unsigned int nondet_uint(void);

static unsigned int callback_calls;
static OQS_STATUS callback_status;

static uint8_t *expected_signature;
static size_t *expected_signature_len;
static const uint8_t *expected_message;
static size_t expected_message_len;
static const uint8_t *expected_ctx;
static size_t expected_ctx_len;
static uint8_t *expected_public_key;
static const uint8_t *expected_secret_key;

static OQS_STATUS model_keypair(uint8_t *public_key, uint8_t *secret_key) {
	callback_calls++;
	__CPROVER_assert(public_key == expected_public_key,
	                 "keypair forwards public key pointer");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "keypair forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_sign(uint8_t *signature, size_t *signature_len,
                             const uint8_t *message, size_t message_len,
                             const uint8_t *secret_key) {
	callback_calls++;
	__CPROVER_assert(signature == expected_signature,
	                 "sign forwards signature pointer");
	__CPROVER_assert(signature_len == expected_signature_len,
	                 "sign forwards signature length pointer");
	__CPROVER_assert(message == expected_message,
	                 "sign forwards message pointer");
	__CPROVER_assert(message_len == expected_message_len,
	                 "sign forwards message length");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "sign forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_sign_with_ctx(uint8_t *signature, size_t *signature_len,
                                      const uint8_t *message, size_t message_len,
                                      const uint8_t *ctx, size_t ctx_len,
                                      const uint8_t *secret_key) {
	callback_calls++;
	__CPROVER_assert(signature == expected_signature,
	                 "sign_with_ctx forwards signature pointer");
	__CPROVER_assert(signature_len == expected_signature_len,
	                 "sign_with_ctx forwards signature length pointer");
	__CPROVER_assert(message == expected_message,
	                 "sign_with_ctx forwards message pointer");
	__CPROVER_assert(message_len == expected_message_len,
	                 "sign_with_ctx forwards message length");
	__CPROVER_assert(ctx == expected_ctx,
	                 "sign_with_ctx forwards context pointer");
	__CPROVER_assert(ctx_len == expected_ctx_len,
	                 "sign_with_ctx forwards context length");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "sign_with_ctx forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_verify(const uint8_t *message, size_t message_len,
                               const uint8_t *signature, size_t signature_len,
                               const uint8_t *public_key) {
	callback_calls++;
	__CPROVER_assert(message == expected_message,
	                 "verify forwards message pointer");
	__CPROVER_assert(message_len == expected_message_len,
	                 "verify forwards message length");
	__CPROVER_assert(signature == expected_signature,
	                 "verify forwards signature pointer");
	__CPROVER_assert(signature_len == *expected_signature_len,
	                 "verify forwards signature length");
	__CPROVER_assert(public_key == expected_public_key,
	                 "verify forwards public key pointer");
	return callback_status;
}

static OQS_STATUS model_verify_with_ctx(const uint8_t *message,
                                        size_t message_len,
                                        const uint8_t *signature,
                                        size_t signature_len,
                                        const uint8_t *ctx, size_t ctx_len,
                                        const uint8_t *public_key) {
	callback_calls++;
	__CPROVER_assert(message == expected_message,
	                 "verify_with_ctx forwards message pointer");
	__CPROVER_assert(message_len == expected_message_len,
	                 "verify_with_ctx forwards message length");
	__CPROVER_assert(signature == expected_signature,
	                 "verify_with_ctx forwards signature pointer");
	__CPROVER_assert(signature_len == *expected_signature_len,
	                 "verify_with_ctx forwards signature length");
	__CPROVER_assert(ctx == expected_ctx,
	                 "verify_with_ctx forwards context pointer");
	__CPROVER_assert(ctx_len == expected_ctx_len,
	                 "verify_with_ctx forwards context length");
	__CPROVER_assert(public_key == expected_public_key,
	                 "verify_with_ctx forwards public key pointer");
	return callback_status;
}

int main(void) {
	OQS_SIG sig = {0};
	uint8_t signature = 0;
	uint8_t message = 0;
	uint8_t ctx = 0;
	uint8_t public_key = 0;
	uint8_t secret_key = 0;
	size_t signature_len = nondet_size_t();
	unsigned int operation = nondet_uint();
	_Bool null_sig = nondet_bool();
	_Bool null_callback = nondet_bool();
	OQS_STATUS result;

	__CPROVER_assume(operation <= 4);

	callback_status = (OQS_STATUS) nondet_int();
	expected_signature = &signature;
	expected_signature_len = &signature_len;
	expected_message = &message;
	expected_message_len = nondet_size_t();
	expected_ctx = &ctx;
	expected_ctx_len = nondet_size_t();
	expected_public_key = &public_key;
	expected_secret_key = &secret_key;

	if (!null_callback) {
		switch (operation) {
		case 0:
			sig.keypair = model_keypair;
			break;
		case 1:
			sig.sign = model_sign;
			break;
		case 2:
			sig.sign_with_ctx_str = model_sign_with_ctx;
			break;
		case 3:
			sig.verify = model_verify;
			break;
		default:
			sig.verify_with_ctx_str = model_verify_with_ctx;
			break;
		}
	}

	const OQS_SIG *sig_ptr = null_sig ? NULL : &sig;

	switch (operation) {
	case 0:
		result = OQS_SIG_keypair(sig_ptr, &public_key, &secret_key);
		break;
	case 1:
		result = OQS_SIG_sign(sig_ptr, &signature, &signature_len, &message,
		                      expected_message_len, &secret_key);
		break;
	case 2:
		result = OQS_SIG_sign_with_ctx_str(sig_ptr, &signature, &signature_len,
		                                   &message, expected_message_len, &ctx,
		                                   expected_ctx_len, &secret_key);
		break;
	case 3:
		result = OQS_SIG_verify(sig_ptr, &message, expected_message_len,
		                        &signature, signature_len, &public_key);
		break;
	default:
		result = OQS_SIG_verify_with_ctx_str(sig_ptr, &message,
		                                     expected_message_len, &signature,
		                                     signature_len, &ctx,
		                                     expected_ctx_len, &public_key);
		break;
	}

	if (null_sig || null_callback) {
		__CPROVER_assert(result == OQS_ERROR,
		                 "signature dispatch rejects missing object or callback");
		__CPROVER_assert(callback_calls == 0,
		                 "signature dispatch short-circuits before callback");
	} else {
		OQS_STATUS expected_result =
		    callback_status == OQS_SUCCESS ? OQS_SUCCESS : OQS_ERROR;
		__CPROVER_assert(result == expected_result,
		                 "signature dispatch normalizes callback status");
		__CPROVER_assert(callback_calls == 1,
		                 "signature dispatch invokes callback once");
	}

	return 0;
}
