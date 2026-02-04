// SPDX-License-Identifier: MIT

// This KAT test only generates a subset of the NIST KAT files.
// To extract the subset from a submission file, use the command:
//     cat PQCsignKAT_whatever.rsp | head -n 10 | tail -n 8

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>
#include <oqs/rand_nist.h>

#include "test_helpers.h"

#include "system_info.c"

static inline uint16_t UINT16_TO_BE(const uint16_t x) {
	union {
		uint16_t val;
		uint8_t bytes[2];
	} y;
	y.bytes[0] = (x >> 8) & 0xFF;
	y.bytes[1] = x & 0xFF;
	return y.val;
}

OQS_STATUS combine_message_signature(uint8_t **signed_msg, size_t *signed_msg_len, const uint8_t *msg, size_t msg_len, const uint8_t *signature, size_t signature_len, const OQS_SIG *sig) {
	if (0) {
		///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_COMBINE_MESSAGE_SIGNATURE_START
	} else if (0 == strcmp(sig->method_name, "ML-DSA-44")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "ML-DSA-65")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "ML-DSA-87")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Falcon-512")) {
		// signed_msg = sig_len (2 bytes, big endian) || nonce (40 bytes) || msg || 0x29 || sig
		const uint16_t signature_len_uint16 = (uint16_t)signature_len;
		*signed_msg_len = 2 + signature_len_uint16 + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		const uint8_t *falc_salt = &signature[1];
		const uint8_t *falc_sig = &signature[41];
		union {
			uint16_t val;
			uint8_t bytes[2];
		} signature_len_be = {.val = UINT16_TO_BE(signature_len_uint16 - 40)};
		memcpy(*signed_msg, &signature_len_be, 2);
		memcpy(*signed_msg + 2, falc_salt, 40);
		memcpy(*signed_msg + 42, msg, msg_len);
		(*signed_msg)[42 + msg_len] = 0x29;
		memcpy(*signed_msg + 42 + msg_len + 1, falc_sig, signature_len - 41);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Falcon-1024")) {
		// signed_msg = sig_len (2 bytes, big endian) || nonce (40 bytes) || msg || 0x2A || sig
		const uint16_t signature_len_uint16 = (uint16_t)signature_len;
		*signed_msg_len = 2 + signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		const uint8_t *falc_salt = &signature[1];
		const uint8_t *falc_sig = &signature[41];
		union {
			uint16_t val;
			uint8_t bytes[2];
		} signature_len_be = {.val = UINT16_TO_BE(signature_len_uint16 - 40)};
		memcpy(*signed_msg, &signature_len_be, 2);
		memcpy(*signed_msg + 2, falc_salt, 40);
		memcpy(*signed_msg + 42, msg, msg_len);
		(*signed_msg)[42 + msg_len] = 0x2A;
		memcpy(*signed_msg + 42 + msg_len + 1, falc_sig, signature_len - 41);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Falcon-padded-512")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Falcon-padded-1024")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "MAYO-1")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "MAYO-2")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "MAYO-3")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "MAYO-5")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-128-balanced")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-128-fast")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-128-small")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-192-balanced")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-192-fast")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-192-small")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-256-balanced")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-256-fast")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdp-256-small")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-128-balanced")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-128-fast")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-128-small")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-192-balanced")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-192-fast")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-192-small")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-256-balanced")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-256-fast")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "cross-rsdpg-256-small")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-Is")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-Ip")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-III")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-V")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-Is-pkc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-Ip-pkc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-III-pkc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-V-pkc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-Is-pkc-skc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-Ip-pkc-skc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-III-pkc-skc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "OV-V-pkc-skc")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_24_5_4")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_24_5_4_SHAKE")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_24_5_4_esk")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_24_5_4_SHAKE_esk")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_37_17_2")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_25_8_3")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_56_25_2")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_49_11_3")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_37_8_4")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_24_5_5")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_60_10_4")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SNOVA_29_6_5")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = OQS_MEM_malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
		///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_COMBINE_MESSAGE_SIGNATURE_END
	} else {
		return OQS_ERROR;
	}
}

OQS_STATUS sig_kat(const char *method_name, bool all) {

	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	uint8_t *msg = NULL;
	size_t msg_len = 0;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *signature = NULL;
	uint8_t *signed_msg = NULL;
	size_t signature_len = 0;
	size_t signed_msg_len = 0;
	OQS_STATUS rc, ret = OQS_ERROR;
	OQS_KAT_PRNG *prng = NULL;
	size_t max_count;

	prng = OQS_KAT_PRNG_new(method_name);
	if (prng == NULL) {
		goto err;
	}

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[sig_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	for (uint8_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	OQS_KAT_PRNG_seed(prng, entropy_input, NULL);

	fh = stdout;

	max_count = all ? prng->max_kats : 1;

	public_key = OQS_MEM_malloc(sig->length_public_key);
	secret_key = OQS_MEM_malloc(sig->length_secret_key);
	signature = OQS_MEM_malloc(sig->length_signature);
	// allocate maximum length for msg
	msg = OQS_MEM_malloc(33 * max_count);
	if ((public_key == NULL) || (secret_key == NULL) || (signature == NULL) || (msg == NULL)) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	for (size_t count = 0; count < max_count; ++count) {
		fprintf(fh, "count = %zu\n", count);
		OQS_randombytes(seed, 48);
		OQS_fprintBstr(fh, "seed = ", seed, 48);

		msg_len = 33 * (count + 1);
		fprintf(fh, "mlen = %zu\n", msg_len);

		OQS_randombytes(msg, msg_len);
		OQS_fprintBstr(fh, "msg = ", msg, msg_len);

		OQS_KAT_PRNG_save_state(prng);
		OQS_KAT_PRNG_seed(prng, seed, NULL);

		rc = OQS_SIG_keypair(sig, public_key, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_keypair failed!\n", method_name);
			goto err;
		}
		OQS_fprintBstr(fh, "pk = ", public_key, sig->length_public_key);
		OQS_fprintBstr(fh, "sk = ", secret_key, sig->length_secret_key);

		rc = OQS_SIG_sign(sig, signature, &signature_len, msg, msg_len, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_sign failed!\n", method_name);
			goto err;
		}
		rc = combine_message_signature(&signed_msg, &signed_msg_len, msg, msg_len, signature, signature_len, sig);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_sig] %s ERROR: combine_message_signature failed!\n", method_name);
			OQS_MEM_secure_free(signed_msg, signed_msg_len);
			goto err;
		}
		fprintf(fh, "smlen = %zu\n", signed_msg_len);
		OQS_fprintBstr(fh, "sm = ", signed_msg, signed_msg_len);

		OQS_MEM_secure_free(signed_msg, signed_msg_len);

		// The NIST program generates KAT response files with a trailing newline.
		if (count != max_count - 1) {
			fprintf(fh, "\n");
		}

		rc = OQS_SIG_verify(sig, msg, msg_len, signature, signature_len, public_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_verify failed!\n", method_name);
			goto err;
		}

		OQS_KAT_PRNG_restore_state(prng);
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signature);
	OQS_MEM_insecure_free(msg);
	OQS_SIG_free(sig);
	OQS_KAT_PRNG_free(prng);
	return ret;
}

int main(int argc, char **argv) {
	OQS_init();

	if (argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[2], "--all"))) {
		fprintf(stderr, "Usage: kat_sig algname [--all]\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	bool all = (argc == 3);
	OQS_STATUS rc = sig_kat(alg_name, all);
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
