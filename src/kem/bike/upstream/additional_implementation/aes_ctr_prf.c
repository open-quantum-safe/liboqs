/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#include "aes_ctr_prf.h"
#include "string.h"

OQS_STATUS init_aes_ctr_prf_state(OUT aes_ctr_prf_state_t *s,
                                  IN const uint32_t max_invokations,
                                  IN const seed_t *seed) {
	if (0 == max_invokations) {
		return OQS_ERR_KEM_BIKE_AES_CTR_PRF_INIT;
	}

	// Set the key schedule (from seed).
	// Make sure the size matches the AES256 key size
	aes256_key_t key;
	static_assert(sizeof(seed->u) == sizeof(key.raw),
	              seed_size_equals_ky_size);
	memcpy(key.raw, seed->u.raw, sizeof(key.raw));

	aes256_key_expansion(&s->ks, &key);

	// Initialize buffer and counter
	s->ctr.u.qw[0] = 0;
	s->ctr.u.qw[1] = 0;

	s->pos = AES256_BLOCK_SIZE;
	s->rem_invokations = max_invokations;

	SEDMSG("    Init aes_prf_ctr state:\n");
	SEDMSG("      s.pos = %d\n", s->pos);
	SEDMSG("      s.rem_invokations = %u\n", s->rem_invokations);
	SEDMSG("      s.ctr = 0x");

	OQS_MEM_cleanse(key.raw, sizeof(key));

	return OQS_SUCCESS;
}

_INLINE_ OQS_STATUS perform_aes(OUT uint8_t *ct, IN OUT aes_ctr_prf_state_t *s) {
	// Ensure that the CTR is big enough
	static_assert(((sizeof(s->ctr.u.qw[0]) == 8) && (BIT(33) >= MAX_AES_INVOKATION)),
	              ctr_size_is_too_small);

	if (0 == s->rem_invokations) {
		return OQS_ERR_KEM_BIKE_AES_OVER_USED;
	}

	aes256_enc_intrinsic(ct, s->ctr.u.bytes, &s->ks);

	s->ctr.u.qw[0]++;
	s->rem_invokations--;

	return OQS_SUCCESS;
}

OQS_STATUS aes_ctr_prf(OUT uint8_t *a,
                       IN aes_ctr_prf_state_t *s,
                       IN const uint32_t len) {
	OQS_STATUS res = OQS_SUCCESS;

	// When Len is smaller then whats left in the buffer
	// there is no need in additional AES.
	if ((len + s->pos) <= AES256_BLOCK_SIZE) {
		memcpy(a, &s->buffer.u.bytes[s->pos], len);
		s->pos += len;

		return res;
	}

	// If s.pos != AES256_BLOCK_SIZE then copy whats left in the buffer.
	// Else copy zero bytes.
	uint32_t idx = AES256_BLOCK_SIZE - s->pos;
	memcpy(a, &s->buffer.u.bytes[s->pos], idx);

	// Init s.pos;
	s->pos = 0;

	// Copy full AES blocks.
	while ((len - idx) >= AES256_BLOCK_SIZE) {
		res = perform_aes(&a[idx], s);
		CHECK_STATUS(res);
		idx += AES256_BLOCK_SIZE;
	}

	res = perform_aes(s->buffer.u.bytes, s);
	CHECK_STATUS(res);

	// Copy the tail.
	s->pos = len - idx;
	memcpy(&a[idx], s->buffer.u.bytes, s->pos);

EXIT:
	return res;
}
