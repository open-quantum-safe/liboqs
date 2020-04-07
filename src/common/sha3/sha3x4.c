#include <immintrin.h>
#include <stdint.h>
#include <assert.h>

#define SHAKE128_RATE 168

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64 - offset)))

static uint64_t load64(const unsigned char *x) {
	unsigned long long r = 0, i;

	for (i = 0; i < 8; ++i) {
		r |= (unsigned long long) x[i] << 8 * i;
	}
	return r;
}

static void store64(uint8_t *x, uint64_t u) {
	union {
		uint64_t val;
		uint8_t bytes[8];
	} v;
	v.val = u;
	for (unsigned int i = 0; i < 8; ++i) {
		x[i] = v.bytes[i];
	}
}

/* Use implementation from the Keccak Code Package */
#include "keccak4x/KeccakP-1600-times4-SIMD256.c"
#define KeccakF1600_StatePermute4x KeccakP1600times4_PermuteAll_24rounds

static void keccak_absorb4x(__m256i *s, unsigned int r, const unsigned char *m0, const unsigned char *m1, const unsigned char *m2, const unsigned char *m3,
                            unsigned long long int mlen, unsigned char p) {
	unsigned long long i;
	unsigned char t0[200];
	unsigned char t1[200];
	unsigned char t2[200];
	unsigned char t3[200];
	unsigned long long *ss = (unsigned long long *) s;

	while (mlen >= r) {
		for (i = 0; i < r / 8; ++i) {
			ss[4 * i + 0] ^= load64(m0 + 8 * i);
			ss[4 * i + 1] ^= load64(m1 + 8 * i);
			ss[4 * i + 2] ^= load64(m2 + 8 * i);
			ss[4 * i + 3] ^= load64(m3 + 8 * i);
		}

		KeccakF1600_StatePermute4x(s);
		mlen -= r;
		m0 += r;
		m1 += r;
		m2 += r;
		m3 += r;
	}

	for (i = 0; i < r; ++i) {
		t0[i] = 0;
		t1[i] = 0;
		t2[i] = 0;
		t3[i] = 0;
	}
	for (i = 0; i < mlen; ++i) {
		t0[i] = m0[i];
		t1[i] = m1[i];
		t2[i] = m2[i];
		t3[i] = m3[i];
	}

	t0[i] = p;
	t1[i] = p;
	t2[i] = p;
	t3[i] = p;

	t0[r - 1] |= 128;
	t1[r - 1] |= 128;
	t2[r - 1] |= 128;
	t3[r - 1] |= 128;

	for (i = 0; i < r / 8; ++i) {
		ss[4 * i + 0] ^= load64(t0 + 8 * i);
		ss[4 * i + 1] ^= load64(t1 + 8 * i);
		ss[4 * i + 2] ^= load64(t2 + 8 * i);
		ss[4 * i + 3] ^= load64(t3 + 8 * i);
	}
}

static void keccak_squeezeblocks4x(unsigned char *h0, unsigned char *h1, unsigned char *h2, unsigned char *h3, unsigned long long int nblocks, __m256i *s, unsigned int r) {
	unsigned int i;
	unsigned long long *ss = (unsigned long long *) s;

	while (nblocks > 0) {
		KeccakF1600_StatePermute4x(s);
		for (i = 0; i < (r >> 3); i++) {
			store64(h0 + 8 * i, ss[4 * i + 0]);
			store64(h1 + 8 * i, ss[4 * i + 1]);
			store64(h2 + 8 * i, ss[4 * i + 2]);
			store64(h3 + 8 * i, ss[4 * i + 3]);
		}
		h0 += r;
		h1 += r;
		h2 += r;
		h3 += r;
		nblocks--;
	}
}

/********** SHAKE128 ***********/

static void shake128_absorb4x(__m256i *s, const unsigned char *in0, const unsigned char *in1, const unsigned char *in2, const unsigned char *in3, unsigned long long inlen) {
	unsigned int i;

	for (i = 0; i < 25; i++) {
		s[i] = _mm256_xor_si256(s[i], s[i]);    // zero state
	}

	/* Absorb input */
	keccak_absorb4x(s, SHAKE128_RATE, in0, in1, in2, in3, inlen, 0x1F);
}

/* N is assumed to be empty; S is assumed to have at most 2 characters */
void OQS_SHA3_shake128_4x(unsigned char *output0, unsigned char *output1, unsigned char *output2, unsigned char *output3, unsigned long long outlen,
                          const unsigned char *in0, const unsigned char *in1, const unsigned char *in2, const unsigned char *in3, unsigned long long inlen) {
	__m256i s[25];
	unsigned char t0[SHAKE128_RATE];
	unsigned char t1[SHAKE128_RATE];
	unsigned char t2[SHAKE128_RATE];
	unsigned char t3[SHAKE128_RATE];
	unsigned int i;

	shake128_absorb4x(s, in0, in1, in2, in3, inlen);

	/* Squeeze output */
	keccak_squeezeblocks4x(output0, output1, output2, output3, outlen / SHAKE128_RATE, s, SHAKE128_RATE);
	output0 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
	output1 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
	output2 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
	output3 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;

	if (outlen % SHAKE128_RATE) {
		keccak_squeezeblocks4x(t0, t1, t2, t3, 1, s, SHAKE128_RATE);
		for (i = 0; i < outlen % SHAKE128_RATE; i++) {
			output0[i] = t0[i];
			output1[i] = t1[i];
			output2[i] = t2[i];
			output3[i] = t3[i];
		}
	}
}

/********** cSHAKE128 ***********/

static void cshake128_simple_absorb4x(__m256i *s, uint16_t cstm0, uint16_t cstm1, uint16_t cstm2, uint16_t cstm3, const unsigned char *in, unsigned long long inlen) {
	unsigned char *sep = (unsigned char *) s;
	unsigned int i;

	for (i = 0; i < 25; i++) {
		s[i] = _mm256_xor_si256(s[i], s[i]);    // zero state
	}

	/* Absorb customization (domain-separation) string */
	for (i = 0; i < 4; i++) {
		sep[8 * i + 0] = 0x01;
		sep[8 * i + 1] = 0xa8;
		sep[8 * i + 2] = 0x01;
		sep[8 * i + 3] = 0x00;
		sep[8 * i + 4] = 0x01;
		sep[8 * i + 5] = 16;
	}
	sep[6] = cstm0 & 0xff;
	sep[7] = cstm0 >> 8;
	sep[14] = cstm1 & 0xff;
	sep[15] = cstm1 >> 8;
	sep[22] = cstm2 & 0xff;
	sep[23] = cstm2 >> 8;
	sep[30] = cstm3 & 0xff;
	sep[31] = cstm3 >> 8;

	KeccakF1600_StatePermute4x(s);

	/* Absorb input */
	keccak_absorb4x(s, SHAKE128_RATE, in, in, in, in, inlen, 0x04);
}

/* N is assumed to be empty; S is assumed to have at most 2 characters */
void OQS_SHA3_cshake128_simple4x(unsigned char *output0, unsigned char *output1, unsigned char *output2, unsigned char *output3, unsigned long long outlen,
                                 uint16_t cstm0, uint16_t cstm1, uint16_t cstm2, uint16_t cstm3, const unsigned char *in, unsigned long long inlen) {
	__m256i s[25];
	unsigned char t0[SHAKE128_RATE];
	unsigned char t1[SHAKE128_RATE];
	unsigned char t2[SHAKE128_RATE];
	unsigned char t3[SHAKE128_RATE];
	unsigned int i;

	cshake128_simple_absorb4x(s, cstm0, cstm1, cstm2, cstm3, in, inlen);

	/* Squeeze output */
	keccak_squeezeblocks4x(output0, output1, output2, output3, outlen / SHAKE128_RATE, s, SHAKE128_RATE);
	output0 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
	output1 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
	output2 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
	output3 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;

	if (outlen % SHAKE128_RATE) {
		keccak_squeezeblocks4x(t0, t1, t2, t3, 1, s, SHAKE128_RATE);
		for (i = 0; i < outlen % SHAKE128_RATE; i++) {
			output0[i] = t0[i];
			output1[i] = t1[i];
			output2[i] = t2[i];
			output3[i] = t3[i];
		}
	}
}
