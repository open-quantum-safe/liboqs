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

#include "system_info.c"

/* Displays hexadecimal strings */
void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", str[i]);
	}
	printf("\n");
}

void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
	size_t i;
	fprintf(fp, "%s", S);
	for (i = 0; i < L; i++) {
		fprintf(fp, "%02X", A[i]);
	}
	if (L == 0) {
		fprintf(fp, "00");
	}
	fprintf(fp, "\n");
}

static inline uint32_t UINT32_TO_LE(const uint32_t x) {
	union {
		uint32_t val;
		uint8_t bytes[4];
	} y;
	y.bytes[0] = x & 0xFF;
	y.bytes[1] = (x >> 8) & 0xFF;
	y.bytes[2] = (x >> 16) & 0xFF;
	y.bytes[3] = (x >> 24) & 0xFF;
	return y.val;
}

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
	} else if ((0 == strcmp(sig->method_name, "picnic_L1_FS")) || (0 == strcmp(sig->method_name, "picnic_L1_UR")) || (0 == strcmp(sig->method_name, "picnic_L1_full")) || (0 == strcmp(sig->method_name, "picnic_L3_FS")) || (0 == strcmp(sig->method_name, "picnic_L3_UR")) || (0 == strcmp(sig->method_name, "picnic_L3_full")) || (0 == strcmp(sig->method_name, "picnic_L5_FS")) || (0 == strcmp(sig->method_name, "picnic_L5_UR")) || (0 == strcmp(sig->method_name, "picnic_L5_full")) || (0 == strcmp(sig->method_name, "picnic3_L1")) || (0 == strcmp(sig->method_name, "picnic3_L3")) || (0 == strcmp(sig->method_name, "picnic3_L5"))) {
		// signed_msg = 4 byte signature length in little endian || msg || signature
		*signed_msg_len = 4 + signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		uint32_t signature_len_le = UINT32_TO_LE((uint32_t) signature_len);
		memcpy(*signed_msg, &signature_len_le, 4);
		memcpy(*signed_msg + 4, msg, msg_len);
		memcpy(*signed_msg + 4 + msg_len, signature, signature_len);
		return OQS_SUCCESS;
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_COMBINE_MESSAGE_SIGNATURE_START
	} else if (0 == strcmp(sig->method_name, "DILITHIUM_2")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "DILITHIUM_3")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "DILITHIUM_4")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
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
		*signed_msg = malloc(*signed_msg_len);
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
		*signed_msg = malloc(*signed_msg_len);
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
	} else if (0 == strcmp(sig->method_name, "Rainbow-Ia-Classic")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-Ia-Cyclic")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-Ia-Cyclic-Compressed")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-IIIc-Classic")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-IIIc-Cyclic")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-IIIc-Cyclic-Compressed")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-Vc-Classic")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-Vc-Cyclic")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "Rainbow-Vc-Cyclic-Compressed")) {
		// signed_msg = msg || signature
		*signed_msg_len = msg_len + signature_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, msg, msg_len);
		memcpy(*signed_msg + msg_len, signature, signature_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-128f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-128f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-128s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-128s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-192f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-192f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-192s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-192s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-256f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-256f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-256s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-Haraka-256s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-128f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-128f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-128s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-128s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-192f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-192f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-192s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-192s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-256f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-256f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-256s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHA256-256s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-128f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-128f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-128s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-128s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-192f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-192f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-192s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-192s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-256f-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-256f-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-256s-robust")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
	} else if (0 == strcmp(sig->method_name, "SPHINCS+-SHAKE256-256s-simple")) {
		// signed_msg = signature || msg
		*signed_msg_len = signature_len + msg_len;
		*signed_msg = malloc(*signed_msg_len);
		if (*signed_msg == NULL) {
			return OQS_ERROR;
		}
		memcpy(*signed_msg, signature, signature_len);
		memcpy(*signed_msg + signature_len, msg, msg_len);
		return OQS_SUCCESS;
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_COMBINE_MESSAGE_SIGNATURE_END
	} else {
		return OQS_ERROR;
	}
}

OQS_STATUS sig_kat(const char *method_name) {

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
	// int rv;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[sig_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	for (uint8_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS) {
		goto err;
	}
	OQS_randombytes_nist_kat_init_256bit(entropy_input, NULL);

	fh = stdout;

	fprintf(fh, "count = 0\n");
	OQS_randombytes(seed, 48);
	fprintBstr(fh, "seed = ", seed, 48);

	msg_len = 33 * (0 + 1);
	fprintf(fh, "mlen = %zu\n", msg_len);

	msg = malloc(msg_len);
	OQS_randombytes(msg, msg_len);
	fprintBstr(fh, "msg = ", msg, msg_len);

	OQS_randombytes_nist_kat_init_256bit(seed, NULL);

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	signature = malloc(sig->length_signature);
	if ((public_key == NULL) || (secret_key == NULL) || (signature == NULL)) {
		fprintf(stderr, "[kat_sig] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "pk = ", public_key, sig->length_public_key);
	fprintBstr(fh, "sk = ", secret_key, sig->length_secret_key);

	rc = OQS_SIG_sign(sig, signature, &signature_len, msg, msg_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_sign failed!\n", method_name);
		goto err;
	}
	rc = combine_message_signature(&signed_msg, &signed_msg_len, msg, msg_len, signature, signature_len, sig);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: combine_message_signature failed!\n", method_name);
		goto err;
	}
	fprintf(fh, "smlen = %zu\n", signed_msg_len);
	fprintBstr(fh, "sm = ", signed_msg, signed_msg_len);

	rc = OQS_SIG_verify(sig, msg, msg_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_sig] %s ERROR: OQS_SIG_verify failed!\n", method_name);
		goto err;
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
		OQS_MEM_secure_free(signed_msg, signed_msg_len);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signature);
	OQS_MEM_insecure_free(msg);
	OQS_SIG_free(sig);
	return ret;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Usage: kat_sig algname\n");
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
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	OQS_STATUS rc = sig_kat(alg_name);
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
