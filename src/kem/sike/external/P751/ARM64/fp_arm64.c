/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: modular arithmetic optimized for 64-bit ARMv8 platforms for P751
*********************************************************************************************/

#include "../P751_internal.h"

/* OQS note: this file is #include'd with the defs of these consts; removed to avoid re-defs
// Global constants
extern const uint64_t p751[NWORDS_FIELD];
extern const uint64_t p751p1[NWORDS_FIELD]; 
extern const uint64_t p751x2[NWORDS_FIELD];
extern const uint64_t p751x4[NWORDS_FIELD];
*/

inline void mp_sub751_p2(const digit_t* a, const digit_t* b, digit_t* c)
{ // Multiprecision subtraction with correction with 2*p, c = a-b+2p. 
    
    oqs_kem_sike_mp_sub751_p2_asm(a, b, c); 
} 


inline void mp_sub751_p4(const digit_t* a, const digit_t* b, digit_t* c)
{ // Multiprecision subtraction with correction with 4*p, c = a-b+4p. 
    
    oqs_kem_sike_mp_sub751_p4_asm(a, b, c);
}

inline void fpadd751(const digit_t *a, const digit_t *b, digit_t *c) { // Modular addition, c = a+b mod p751.
	// Inputs: a, b in [0, 2*p751-1]
	// Output: c in [0, 2*p751-1]

	oqs_kem_sike_fpadd751_asm(a, b, c);
}

inline void fpsub751(const digit_t *a, const digit_t *b, digit_t *c) { // Modular subtraction, c = a-b mod p751.
	// Inputs: a, b in [0, 2*p751-1]
	// Output: c in [0, 2*p751-1]

	oqs_kem_sike_fpsub751_asm(a, b, c);
}

inline void fpneg751(digit_t *a) { // Modular negation, a = -a mod p751.
	// Input/output: a in [0, 2*p751-1]
	unsigned int i, borrow = 0;

	for (i = 0; i < NWORDS_FIELD; i++) {
		SUBC(borrow, ((digit_t *) p751x2)[i], a[i], borrow, a[i]);
	}
}

void fpdiv2_751(const digit_t *a, digit_t *c) { // Modular division by two, c = a/2 mod p751.
	// Input : a in [0, 2*p751-1]
	// Output: c in [0, 2*p751-1]
	unsigned int i, carry = 0;
	digit_t mask;

	mask = 0 - (digit_t)(a[0] & 1); // If a is odd compute a+p521
	for (i = 0; i < NWORDS_FIELD; i++) {
		ADDC(carry, a[i], ((digit_t *) p751)[i] & mask, carry, c[i]);
	}

	mp_shiftr1(c, NWORDS_FIELD);
}

void fpcorrection751(digit_t *a) { // Modular correction to reduce field element a in [0, 2*p751-1] to [0, p751-1].
	unsigned int i, borrow = 0;
	digit_t mask;

	for (i = 0; i < NWORDS_FIELD; i++) {
		SUBC(borrow, a[i], ((digit_t *) p751)[i], borrow, a[i]);
	}
	mask = 0 - (digit_t) borrow;

	borrow = 0;
	for (i = 0; i < NWORDS_FIELD; i++) {
		ADDC(borrow, a[i], ((digit_t *) p751)[i] & mask, borrow, a[i]);
	}
}

void mp_mul(const digit_t *a, const digit_t *b, digit_t *c, const unsigned int nwords) { // Multiprecision multiply, c = a*b, where lng(a) = lng(b) = nwords.

	UNREFERENCED_PARAMETER(nwords);

	oqs_kem_sike_mul751_asm(a, b, c);
}

void rdc_mont(digit_t *ma, digit_t *mc) { // Montgomery reduction exploiting special form of the prime.
	// mc = ma*R^-1 mod p751x2, where R = 2^768.
	// If ma < 2^768*p751, the output mc is in the range [0, 2*p751-1].
	// ma is assumed to be in Montgomery representation.

	oqs_kem_sike_rdc751_asm(ma, mc);
}
