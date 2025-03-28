/**
 * functions
 */

#include <stddef.h>
#include <assert.h>

#include "snova.h"

#if defined(SNOVA_LIBOQS)
#include <aes.h>

int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen) {
	assert(inputByteLen > 0);
	const uint8_t iv[16] = {0};

	void *state;
	OQS_AES128_CTR_inc_init(input, &state);
	OQS_AES128_CTR_inc_stream_iv(iv, 12, state, output, outputByteLen);
	OQS_AES128_free_schedule(state);
	return (int)outputByteLen;

}

void AES_256_ECB(const unsigned char *key, const uint8_t *input, unsigned char *output) {
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule);
	OQS_AES256_ECB_enc_sch(input, 16, schedule, output);
	OQS_AES256_free_schedule(schedule);
}

#else
#include "aes/aes_local.h"

#if __AVX2__

int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen) {
	assert(inputByteLen > 0);
	const uint8_t iv[16] = {0};
	void *schedule = NULL;
	oqs_aes128_load_schedule_ni(input, &schedule);
	oqs_aes128_ctr_enc_sch_ni(iv, 16, schedule, output, outputByteLen);
	oqs_aes128_free_schedule_ni(schedule);
	return (int)outputByteLen;
}

void AES_256_ECB(const unsigned char *key, const uint8_t *input, unsigned char *output) {
	void *schedule = NULL;
	oqs_aes256_load_schedule_ni(key, &schedule);
	oqs_aes256_ecb_enc_sch_ni(input, 16, schedule, output);
	oqs_aes256_free_schedule_ni(schedule);
}

#elif __ARM_ARCH

int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen) {
	assert(inputByteLen > 0);
	const uint8_t iv[16] = {0};
	void *schedule = NULL;
	oqs_aes128_load_schedule_no_bitslice(input, &schedule);
	oqs_aes128_ctr_enc_sch_armv8(iv, 16, schedule, output, outputByteLen);
	oqs_aes128_free_schedule_no_bitslice(schedule);
	return (int)outputByteLen;
}

void AES_256_ECB(const unsigned char *key, const uint8_t *input, unsigned char *output) {
	void *schedule = NULL;
	oqs_aes256_load_schedule_no_bitslice(key, &schedule);
	oqs_aes256_ecb_enc_sch_armv8(input, 16, schedule, output);
	oqs_aes256_free_schedule_no_bitslice(schedule);
}

#else

int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen) {
	assert(inputByteLen > 0);
	const uint8_t iv[16] = {0};
	void *schedule = NULL;
	oqs_aes128_load_schedule_c(input, &schedule);
	oqs_aes128_ctr_enc_sch_c(iv, 16, schedule, output, outputByteLen);
	oqs_aes128_free_schedule_c(schedule);
	return (int)outputByteLen;
}

void AES_256_ECB(const unsigned char *key, const uint8_t *input, unsigned char *output) {
	void *schedule = NULL;
	oqs_aes256_load_schedule_c(key, &schedule);
	oqs_aes256_ecb_enc_sch_c(input, 16, schedule, output);
	oqs_aes256_free_schedule_c(schedule);
}

#endif
#endif

