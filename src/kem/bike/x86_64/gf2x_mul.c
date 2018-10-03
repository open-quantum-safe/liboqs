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

#include "gf2x_mul.h"
#include "stdlib.h"

// Prototypes of ASM functions
extern void gf2_muladd_4x4(OUT uint64_t *res,
                           IN const uint64_t *a,
                           IN const uint64_t *b);

extern void karatzuba_add1(OUT const uint64_t *res,
                           IN const uint64_t *a,
                           IN const uint64_t *b,
                           IN const uint64_t n_half,
                           IN uint64_t *alah);

extern void karatzuba_add2(OUT const uint64_t *res,
                           IN const uint64_t *res1,
                           IN const uint64_t *res2,
                           IN const uint64_t *tmp,
                           IN const uint64_t n_half);

extern void red_asm(uint64_t *res);

// All the temporary data (which might hold secrets)
// will be stored on a secure buffer, so we can easily clean it later.
// The secure buffer required is: 3n/2 (alah|blbh|tmp) in a recursive way
// 3n/2 + 3n/4 + 3n/8 = 3(n/2 + n/4 + n/8) < 3n
#define SECURE_BUFFER_SIZE (3 * R_PADDED_SIZE)

// This functions assumes that n is even.
_INLINE_ void karatzuba(OUT uint64_t *res,
                        IN const uint64_t *a,
                        IN const uint64_t *b,
                        IN const uint64_t n,
                        uint64_t *secure_buf) {
	// If n=4 then calculate 256bitx256bit (4*64)
	// in schoolbook mode.
	if (4 == n) {
		gf2_muladd_4x4(res, a, b);
		return;
	}

	static_assert((n % 2 == 0), karatzuba_n_is_odd);
	const uint64_t half_n = n >> 1;

	// Define pointers for the middle of each parameter
	// sepearting a=a_low and a_high (same for ba nd res)
	const uint64_t *a_high = a + half_n;
	const uint64_t *b_high = b + half_n;

	// Divide res into 4 parts res3|res2|res1|res in size n/2
	uint64_t *res1 = res + half_n;
	uint64_t *res2 = res1 + half_n;

	// All three parameters below are allocated on the secure buffer
	// All of them are in size half n
	uint64_t *alah = secure_buf;
	uint64_t *blbh = alah + half_n;
	uint64_t *tmp = blbh + half_n;

	// Place the secure buffer ptr on the first free location
	// so the recursive function can use it
	secure_buf = tmp + half_n;

	// Calculate Z0 and store the result in res(low)
	karatzuba(res, a, b, half_n, secure_buf);

	// Calculate Z2 and store the result in res(high)
	karatzuba(res2, a_high, b_high, half_n, secure_buf);

	// Accomulate the results
	karatzuba_add1(res, a, b, half_n, alah);

	// (a_low + a_high)(b_low + b_high) --> res1
	karatzuba(res1, alah, blbh, half_n, secure_buf);

	karatzuba_add2(res, res1, res2, tmp, half_n);
}

void gf2x_mod_mul(OUT uint64_t *res,
                  IN const uint64_t *a,
                  IN const uint64_t *b) {
	uint64_t *secure_buffer = (uint64_t *) malloc(SECURE_BUFFER_SIZE);
	karatzuba(res, a, b, R_PADDED_QW, secure_buffer);

	// This function implicitly assumes that the size of res is 2*R_PADDED_QW
	red_asm(res);

	//Free also clear the memory! this prevents secrets from being exposed
	OQS_MEM_secure_free(secure_buffer, 3 * R_PADDED_SIZE);
}
