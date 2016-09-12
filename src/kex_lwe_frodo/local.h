#ifndef _OQS_KEX_LWE_FRODO_LOCAL_H_
#define _OQS_KEX_LWE_FRODO_LOCAL_H_

#include <stdint.h>

#include <oqs/rand.h>

struct oqs_kex_lwe_frodo_params {
	uint8_t *seed;
	size_t seed_len;
	char *param_name;
	uint16_t log2_q;
	uint16_t q;
	uint16_t n;
	uint16_t extracted_bits;
	uint16_t nbar;
	uint16_t key_bits;
	uint16_t noise_dist;
	uint16_t rec_hint_length;
	uint32_t pub_length;
};

void oqs_kex_lwe_frodo_round2(unsigned char *out, uint16_t *in, struct oqs_kex_lwe_frodo_params *param);
void oqs_kex_lwe_frodo_crossround2(unsigned char *out, const uint16_t *in, struct oqs_kex_lwe_frodo_params *param);
void oqs_kex_lwe_frodo_reconcile(unsigned char *out, uint16_t *w, const unsigned char *hint, struct oqs_kex_lwe_frodo_params *param);

void oqs_kex_lwe_frodo_key_derive_server(uint16_t *out, const uint16_t *b, const uint16_t *s, struct oqs_kex_lwe_frodo_params *param);
void oqs_kex_lwe_frodo_key_round(uint16_t *vec, const size_t length, const int b);
void oqs_kex_lwe_frodo_key_round_hints(uint16_t *vec, const size_t length, const int b, const unsigned char *hint);
void oqs_kex_lwe_frodo_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb);
void oqs_kex_lwe_frodo_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb);

#endif /* _OQS_KEX_RLWE_BCNS15_LOCAL_H_ */
