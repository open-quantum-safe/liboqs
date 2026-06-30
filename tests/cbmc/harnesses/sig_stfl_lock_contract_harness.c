// SPDX-License-Identifier: MIT

#include <stdint.h>

#include <oqs/oqs.h>

extern _Bool nondet_bool(void);
extern int nondet_int(void);

static unsigned int callback_calls;
static OQS_STATUS callback_status;
static void *expected_mutex;

static OQS_STATUS model_lock(void *mutex) {
	callback_calls++;
	__CPROVER_assert(mutex == expected_mutex,
	                 "lock callback receives configured mutex");
	return callback_status;
}

static OQS_STATUS model_unlock(void *mutex) {
	callback_calls++;
	__CPROVER_assert(mutex == expected_mutex,
	                 "unlock callback receives configured mutex");
	return callback_status;
}

int main(void) {
	OQS_SIG_STFL_SECRET_KEY sk = {0};
	uint8_t mutex = 0;
	_Bool use_unlock = nondet_bool();
	_Bool null_key = nondet_bool();
	_Bool configured_callback = nondet_bool();
	_Bool null_mutex = nondet_bool();
	OQS_STATUS result;

	callback_status = (OQS_STATUS) nondet_int();
	expected_mutex = &mutex;

	OQS_SIG_STFL_SECRET_KEY_SET_lock(&sk, model_lock);
	OQS_SIG_STFL_SECRET_KEY_SET_unlock(&sk, model_unlock);
	OQS_SIG_STFL_SECRET_KEY_SET_mutex(&sk, &mutex);
	__CPROVER_assert(sk.lock_key == model_lock,
	                 "lock setter stores callback");
	__CPROVER_assert(sk.unlock_key == model_unlock,
	                 "unlock setter stores callback");
	__CPROVER_assert(sk.mutex == &mutex,
	                 "mutex setter stores pointer");

	sk.lock_key = NULL;
	sk.unlock_key = NULL;
	sk.mutex = null_mutex ? NULL : &mutex;
	if (configured_callback) {
		if (use_unlock) {
			sk.unlock_key = model_unlock;
		} else {
			sk.lock_key = model_lock;
		}
	}

	OQS_SIG_STFL_SECRET_KEY *sk_ptr = null_key ? NULL : &sk;
	if (use_unlock) {
		result = OQS_SIG_STFL_SECRET_KEY_unlock(sk_ptr);
	} else {
		result = OQS_SIG_STFL_SECRET_KEY_lock(sk_ptr);
	}

	if (null_key) {
		__CPROVER_assert(result == OQS_ERROR,
		                 "lock helpers reject null key");
		__CPROVER_assert(callback_calls == 0,
		                 "lock helpers do not call through null key");
	} else if (!configured_callback) {
		__CPROVER_assert(result == OQS_SUCCESS,
		                 "unset lock callback is a no-op");
		__CPROVER_assert(callback_calls == 0,
		                 "unset lock callback is not invoked");
	} else if (null_mutex) {
		__CPROVER_assert(result == OQS_ERROR,
		                 "configured lock callback requires mutex");
		__CPROVER_assert(callback_calls == 0,
		                 "missing mutex short-circuits callback");
	} else {
		__CPROVER_assert(result == callback_status,
		                 "lock helper propagates callback status");
		__CPROVER_assert(callback_calls == 1,
		                 "lock helper invokes callback once");
	}

	return 0;
}
