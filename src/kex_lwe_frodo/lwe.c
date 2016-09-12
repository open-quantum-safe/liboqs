#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

// [.]_2
void oqs_kex_lwe_frodo_round2(unsigned char *out, uint16_t *in, struct oqs_kex_lwe_frodo_params *params) {
	oqs_kex_lwe_frodo_key_round(in, params->nbar * params->nbar, params->log2_q - params->extracted_bits);
	int i;
	for (i = 0; i < params->nbar * params->nbar; i++) {
		in[i] >>= params->log2_q - params->extracted_bits;  // drop bits that were zeroed out
	}

	// out should have enough space for the key
	oqs_kex_lwe_frodo_pack(out, params->key_bits / 8, in, params->nbar * params->nbar, params->extracted_bits);
}

void oqs_kex_lwe_frodo_crossround2(unsigned char *out, const uint16_t *in, struct oqs_kex_lwe_frodo_params *params) {
	int i;
	// out should have enough space for N_BAR * N_BAR bits
	memset((unsigned char *)out, 0, params->rec_hint_length);

	uint16_t whole = 1 << (params->log2_q - params->extracted_bits);
	uint16_t half = whole >> 1;
	uint16_t mask = whole - 1;

	for (i = 0; i < params->nbar * params->nbar; i++) {
		uint16_t remainder = in[i] & mask;
		out[i / 8] += (remainder >= half) << (i % 8);
	}
}

void oqs_kex_lwe_frodo_reconcile(unsigned char *out, uint16_t *w, const unsigned char *hint, struct oqs_kex_lwe_frodo_params *params) {
	oqs_kex_lwe_frodo_key_round_hints(w, params->nbar * params->nbar, params->log2_q - params->extracted_bits, hint);
	int i;
	for (i = 0; i < params->nbar * params->nbar; i++) {
		w[i] >>= params->log2_q - params->extracted_bits;  // drop bits that were zeroed out
	}
	oqs_kex_lwe_frodo_pack(out, params->key_bits / 8, w, params->nbar * params->nbar, params->extracted_bits);
}

// multiply by s on the right
void oqs_kex_lwe_frodo_key_derive_server(uint16_t *out, const uint16_t *b, const uint16_t *s, struct oqs_kex_lwe_frodo_params *params) {
	// b (N_BAR x N)
	// s (N x N_BAR)
	// out = bs
	int i, j, k;
	for (i = 0; i < params->nbar; i++) {
		for (j = 0; j < params->nbar; j++) {
			out[i * params->nbar + j] = 0;
			for (k = 0; k < params->n; k++) {
				out[i * params->nbar + j] += b[i * params->n + k] * s[k * params->nbar + j];
			}
			out[i * params->nbar + j] %= params->q;  // not really necessary since LWE_Q is a power of 2.
		}
	}
}

// round all elements of a vector to the nearest multiple of 2^b
void oqs_kex_lwe_frodo_key_round(uint16_t *vec, const size_t length, const int b) {
	size_t i;
	uint16_t negmask = ~((1 << b) - 1);
	uint16_t half = b > 0 ? 1 << (b - 1) : 0;
	for (i = 0; i < length; i++) vec[i] = (vec[i] + half) & negmask;
}

// Round all elements of a vector to the multiple of 2^b, with a hint for the
// direction of rounding when close to the boundary.
void oqs_kex_lwe_frodo_key_round_hints(uint16_t *vec, const size_t length, const int b, const unsigned char *hint) {
	size_t i;
	uint16_t whole = 1 << b;
	uint16_t mask = whole - 1;
	uint16_t negmask = ~mask;
	uint16_t half = 1 << (b - 1);
	uint16_t quarter = 1 << (b - 2);

	for (i = 0; i < length; i++) {
		uint16_t remainder = vec[i] & mask;
		uint16_t use_hint = ((remainder + quarter) >> (b - 1)) & 0x1;

		unsigned char h = (hint[i / 8] >> (i % 8)) % 2;  // the hint
		uint16_t shift = use_hint * (2 * h - 1) * quarter;

		// if use_hint = 1 and h = 0, adding -quarter forces rounding down
		//                     h = 1, adding quarter forces rounding up

		vec[i] = (vec[i] + half + shift) & negmask;
	}
}

// Pack the input uint16 vector into a char output vector, copying lsb bits
// from each input element. If inlen * lsb / 8 > outlen, only outlen * 8 bits
// are copied.
void oqs_kex_lwe_frodo_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb) {
	memset((unsigned char *)out, 0, outlen);

	size_t i = 0;            // whole bytes already filled in
	size_t j = 0;            // whole uint16_t already copied
	uint16_t w = 0;          // the leftover, not yet copied
	unsigned char bits = 0;  // the number of lsb in w
	while (i < outlen && (j < inlen || ((j == inlen) && (bits > 0)))) {
		/*
		in: |        |        |********|********|
		                      ^
		                      j
		w : |   ****|
		        ^
		       bits
		out:|**|**|**|**|**|**|**|**|* |
		                            ^^
		                            ib
		*/
		unsigned char b = 0;  // bits in out[i] already filled in
		while (b < 8) {
			int nbits = min(8 - b, bits);
			uint16_t mask = (1 << nbits) - 1;
			unsigned char t = (w >> (bits - nbits)) & mask;  // the bits to copy from w to out
			out[i] = out[i] + (t << (8 - b - nbits));
			b += nbits;
			bits -= nbits;
			w &= ~(mask << bits);  // not strictly necessary; mostly for debugging

			if (bits == 0) {
				if (j < inlen) {
					w = in[j];
					bits = lsb;
					j++;
				} else {
					break;  // the input vector is exhausted
				}
			}
		}
		if (b == 8) {  // out[i] is filled in
			i++;
			b = 0;
		}
	}
}

// Unpack the input char vector into a uint16_t output vector, copying lsb bits
// for each output element from input. outlen must be at least ceil(inlen * 8 /
// lsb).
void oqs_kex_lwe_frodo_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb) {
	memset(out, 0, outlen * sizeof(uint16_t));

	size_t i = 0;            // whole uint16_t already filled in
	size_t j = 0;            // whole bytes already copied
	unsigned char w = 0;     // the leftover, not yet copied
	unsigned char bits = 0;  // the number of lsb bits of w
	while (i < outlen && (j < inlen || ((j == inlen) && (bits > 0)))) {
		/*
		in: |  |  |  |  |  |  |**|**|...
		                      ^
		                      j
		w : | *|
		      ^
		      bits
		out:|   *****|   *****|   ***  |        |...
		                      ^   ^
		                      i   b
		*/
		unsigned char b = 0;  // bits in out[i] already filled in
		while (b < lsb) {
			int nbits = min(lsb - b, bits);
			uint16_t mask = (1 << nbits) - 1;
			unsigned char t = (w >> (bits - nbits)) & mask;  // the bits to copy from w to out
			out[i] = out[i] + (t << (lsb - b - nbits));
			b += nbits;
			bits -= nbits;
			w &= ~(mask << bits);  // not strictly necessary; mostly for debugging

			if (bits == 0) {
				if (j < inlen) {
					w = in[j];
					bits = 8;
					j++;
				} else {
					break;  // the input vector is exhausted
				}
			}
		}
		if (b == lsb) {  // out[i] is filled in
			i++;
			b = 0;
		}
	}
}
