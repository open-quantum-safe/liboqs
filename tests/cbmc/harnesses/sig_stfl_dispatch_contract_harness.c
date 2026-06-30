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
static uint8_t *expected_public_key;
static OQS_SIG_STFL_SECRET_KEY *expected_secret_key;
static unsigned long long *expected_counter;
static uint8_t **expected_serialized_ptr;
static size_t *expected_serialized_len;
static const uint8_t *expected_serialized_input;
static size_t expected_serialized_input_len;
static void *expected_context;

static OQS_STATUS model_keypair(uint8_t *public_key,
                                OQS_SIG_STFL_SECRET_KEY *secret_key) {
	callback_calls++;
	__CPROVER_assert(public_key == expected_public_key,
	                 "stfl keypair forwards public key pointer");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "stfl keypair forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_sign(uint8_t *signature, size_t *signature_len,
                             const uint8_t *message, size_t message_len,
                             OQS_SIG_STFL_SECRET_KEY *secret_key) {
	callback_calls++;
	__CPROVER_assert(signature == expected_signature,
	                 "stfl sign forwards signature pointer");
	__CPROVER_assert(signature_len == expected_signature_len,
	                 "stfl sign forwards signature length pointer");
	__CPROVER_assert(message == expected_message,
	                 "stfl sign forwards message pointer");
	__CPROVER_assert(message_len == expected_message_len,
	                 "stfl sign forwards message length");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "stfl sign forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_verify(const uint8_t *message, size_t message_len,
                               const uint8_t *signature, size_t signature_len,
                               const uint8_t *public_key) {
	callback_calls++;
	__CPROVER_assert(message == expected_message,
	                 "stfl verify forwards message pointer");
	__CPROVER_assert(message_len == expected_message_len,
	                 "stfl verify forwards message length");
	__CPROVER_assert(signature == expected_signature,
	                 "stfl verify forwards signature pointer");
	__CPROVER_assert(signature_len == *expected_signature_len,
	                 "stfl verify forwards signature length");
	__CPROVER_assert(public_key == expected_public_key,
	                 "stfl verify forwards public key pointer");
	return callback_status;
}

static OQS_STATUS model_sigs_remaining(unsigned long long *remain,
                                       const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	callback_calls++;
	__CPROVER_assert(remain == expected_counter,
	                 "stfl sigs_remaining forwards counter pointer");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "stfl sigs_remaining forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_sigs_total(unsigned long long *total,
                                   const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	callback_calls++;
	__CPROVER_assert(total == expected_counter,
	                 "stfl sigs_total forwards counter pointer");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "stfl sigs_total forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_serialize(uint8_t **serialized, size_t *serialized_len,
                                  const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	callback_calls++;
	__CPROVER_assert(serialized == expected_serialized_ptr,
	                 "stfl serialize forwards output pointer");
	__CPROVER_assert(serialized_len == expected_serialized_len,
	                 "stfl serialize forwards output length pointer");
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "stfl serialize forwards secret key pointer");
	return callback_status;
}

static OQS_STATUS model_deserialize(OQS_SIG_STFL_SECRET_KEY *secret_key,
                                    const uint8_t *serialized,
                                    const size_t serialized_len,
                                    void *context) {
	callback_calls++;
	__CPROVER_assert(secret_key == expected_secret_key,
	                 "stfl deserialize forwards secret key pointer");
	__CPROVER_assert(serialized == expected_serialized_input,
	                 "stfl deserialize forwards input pointer");
	__CPROVER_assert(serialized_len == expected_serialized_input_len,
	                 "stfl deserialize forwards input length");
	__CPROVER_assert(context == expected_context,
	                 "stfl deserialize forwards context pointer");
	return callback_status;
}

int main(void) {
	OQS_SIG_STFL sig = {0};
	OQS_SIG_STFL_SECRET_KEY secret_key = {0};
	uint8_t signature = 0;
	uint8_t message = 0;
	uint8_t public_key = 0;
	uint8_t serialized_input = 0;
	uint8_t context = 0;
	uint8_t *serialized_output = NULL;
	size_t signature_len = nondet_size_t();
	size_t serialized_output_len = nondet_size_t();
	unsigned long long counter = 0;
	unsigned int operation = nondet_uint();
	_Bool null_object = nondet_bool();
	_Bool null_callback = nondet_bool();
	_Bool null_buffer_arg = nondet_bool();
	_Bool null_length_arg = nondet_bool();
	OQS_STATUS result;

	__CPROVER_assume(operation <= 6);

	callback_status = (OQS_STATUS) nondet_int();
	expected_signature = &signature;
	expected_signature_len = &signature_len;
	expected_message = &message;
	expected_message_len = nondet_size_t();
	expected_public_key = &public_key;
	expected_secret_key = &secret_key;
	expected_counter = &counter;
	expected_serialized_ptr = &serialized_output;
	expected_serialized_len = &serialized_output_len;
	expected_serialized_input = &serialized_input;
	expected_serialized_input_len = nondet_size_t();
	expected_context = &context;

	if (!null_callback) {
		switch (operation) {
		case 0:
			sig.keypair = model_keypair;
			break;
		case 1:
			sig.sign = model_sign;
			break;
		case 2:
			sig.verify = model_verify;
			break;
		case 3:
			sig.sigs_remaining = model_sigs_remaining;
			break;
		case 4:
			sig.sigs_total = model_sigs_total;
			break;
		case 5:
			secret_key.serialize_key = model_serialize;
			break;
		default:
			secret_key.deserialize_key = model_deserialize;
			break;
		}
	}

	const OQS_SIG_STFL *sig_ptr = null_object ? NULL : &sig;
	OQS_SIG_STFL_SECRET_KEY *secret_key_ptr = null_object ? NULL : &secret_key;
	uint8_t **serialized_output_arg =
	    null_buffer_arg ? NULL : &serialized_output;
	size_t *serialized_output_len_arg =
	    null_length_arg ? NULL : &serialized_output_len;
	const uint8_t *serialized_input_arg =
	    null_buffer_arg ? NULL : &serialized_input;

	switch (operation) {
	case 0:
		result = OQS_SIG_STFL_keypair(sig_ptr, &public_key, &secret_key);
		break;
	case 1:
		result = OQS_SIG_STFL_sign(sig_ptr, &signature, &signature_len, &message,
		                           expected_message_len, &secret_key);
		break;
	case 2:
		result = OQS_SIG_STFL_verify(sig_ptr, &message, expected_message_len,
		                             &signature, signature_len, &public_key);
		break;
	case 3:
		result = OQS_SIG_STFL_sigs_remaining(sig_ptr, &counter, &secret_key);
		break;
	case 4:
		result = OQS_SIG_STFL_sigs_total(sig_ptr, &counter, &secret_key);
		break;
	case 5:
		result = OQS_SIG_STFL_SECRET_KEY_serialize(serialized_output_arg,
		         serialized_output_len_arg,
		         secret_key_ptr);
		break;
	default:
		result = OQS_SIG_STFL_SECRET_KEY_deserialize(secret_key_ptr,
		         serialized_input_arg,
		         expected_serialized_input_len,
		         &context);
		break;
	}

	_Bool invalid_wrapper_input =
	    null_object || null_callback ||
	    (operation == 5 && (null_buffer_arg || null_length_arg)) ||
	    (operation == 6 && null_buffer_arg);

	if (invalid_wrapper_input) {
		__CPROVER_assert(result == OQS_ERROR,
		                 "stfl dispatch rejects missing wrapper input");
		__CPROVER_assert(callback_calls == 0,
		                 "stfl dispatch short-circuits before callback");
	} else if (operation <= 4) {
		OQS_STATUS expected_result =
		    callback_status == OQS_SUCCESS ? OQS_SUCCESS : OQS_ERROR;
		__CPROVER_assert(result == expected_result,
		                 "stfl operation dispatch normalizes callback status");
		__CPROVER_assert(callback_calls == 1,
		                 "stfl operation dispatch invokes callback once");
	} else {
		__CPROVER_assert(result == callback_status,
		                 "stfl key serialization dispatch preserves callback status");
		__CPROVER_assert(callback_calls == 1,
		                 "stfl key serialization dispatch invokes callback once");
	}

	return 0;
}
