/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 * http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 * The license is detailed in the file LICENSE.md, and applies to this file.
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 */

#pragma once
#include <oqs/common.h>

#include "types.h"
#include <string.h>

/* Runs _thecleanup function on _thealloc once _thealloc went out of scope */
#define DEFER_CLEANUP(_thealloc, _thecleanup) \
	__attribute__((cleanup(_thecleanup))) _thealloc

/* Creates cleanup function for pointers from function func which accepts a
 * pointer. This is useful for DEFER_CLEANUP as it passes &_thealloc into
 * _thecleanup function,
 * so if _thealloc is a pointer _thecleanup will receive a pointer to a
 * pointer.*/
#define DEFINE_POINTER_CLEANUP_FUNC(type, func)  \
	static inline void func##_pointer(type *p) { \
		if (p && *p) {                           \
			func(*p);                            \
		}                                        \
	}                                            \
	struct __useless_struct_to_allow_trailing_semicolon__

// len is bytes length of in
#define secure_clean OQS_MEM_cleanse

_INLINE_ void
r_cleanup(IN OUT r_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
e_cleanup(IN OUT e_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
padded_r_cleanup(IN OUT padded_r_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
padded_e_cleanup(IN OUT padded_e_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
split_e_cleanup(IN OUT split_e_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
pad_sk_cleanup(IN OUT pad_sk_t *o) {
	secure_clean((uint8_t *) o[0], sizeof(*o));
}

_INLINE_ void
pad_ct_cleanup(IN OUT pad_ct_t *o) {
	secure_clean((uint8_t *) o[0], sizeof(*o));
}

_INLINE_ void
dbl_pad_ct_cleanup(IN OUT dbl_pad_ct_t *o) {
	secure_clean((uint8_t *) o[0], sizeof(*o));
}

_INLINE_ void
seed_cleanup(IN OUT seed_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
syndrome_cleanup(IN OUT syndrome_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
dbl_pad_syndrome_cleanup(IN OUT dbl_pad_syndrome_t *o) {
	secure_clean((uint8_t *) o[0], sizeof(*o));
}

_INLINE_ void
compressed_idx_dv_ar_cleanup(IN OUT compressed_idx_dv_ar_t *o) {
	for (int i = 0; i < N0; i++) {
		secure_clean((uint8_t *) &(*o)[i], sizeof((*o)[0]));
	}
}

_INLINE_ void
generic_param_n_cleanup(IN OUT generic_param_n_t *o) {
	secure_clean((uint8_t *) o, sizeof(*o));
}

_INLINE_ void
seeds_cleanup(IN OUT seeds_t *o) {
	for (int i = 0; i < NUM_OF_SEEDS; i++) {
		seed_cleanup(&(o->seed[i]));
	}
}
