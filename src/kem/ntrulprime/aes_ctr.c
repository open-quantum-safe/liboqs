/*
 * Implementation of counter-mode AES, using liboqs in-built ECB mode.
 */

#include <string.h>
#include <oqs/aes.h>
#include "ref/crypto_stream_aes256ctr.h"

static void increment_nonce(unsigned char *nonce);

extern int crypto_stream_aes256ctr_dolbeau_aesenc_int(unsigned char *stream,
                                                      unsigned long long streamLen, const unsigned char *startingNonce,
                                                      const unsigned char *key) {

	void *schedule;
	OQS_AES256_load_schedule(key, &schedule, 1);

	unsigned char nonce[crypto_stream_aes256ctr_dolbeau_aesenc_int_NONCEBYTES];
	memcpy(nonce, startingNonce, crypto_stream_aes256ctr_dolbeau_aesenc_int_NONCEBYTES);

	// Process whole blocks directly into the output buffer
	for (unsigned long long i = 0; i < streamLen / 16; i++) {
		OQS_AES256_ECB_enc_sch(nonce, crypto_stream_aes256ctr_dolbeau_aesenc_int_NONCEBYTES, schedule, &stream[i * 16]);
		increment_nonce(nonce);
	}

	unsigned remain = streamLen % 16;
	if (remain) {
		unsigned char buf[16];
		OQS_AES256_ECB_enc_sch(nonce, crypto_stream_aes256ctr_dolbeau_aesenc_int_NONCEBYTES, schedule, buf);
		memcpy(&stream[streamLen - remain], buf, remain);
	}

	OQS_AES256_free_schedule(schedule);
	return 0;
}

static void increment_nonce(unsigned char *nonce) {
	for (int i = crypto_stream_aes256ctr_dolbeau_aesenc_int_NONCEBYTES - 1;
	     i >= 0; i--) {
		if (++nonce[i]) {
			return;
		}
	}
}
