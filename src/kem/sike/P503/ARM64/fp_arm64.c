/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: modular arithmetic optimized for 64-bit ARMv8 platforms for P503
*********************************************************************************************/

#include "../P503_internal.h"

// Global constants
extern const uint64_t p503[NWORDS_FIELD];
extern const uint64_t p503x2[NWORDS_FIELD]; 


__inline void fpadd503(const digit_t* a, const digit_t* b, digit_t* c)
{ // Modular addition, c = a+b mod p503.
  // Inputs: a, b in [0, 2*p503-1] 
  // Output: c in [0, 2*p503-1]

    fpadd503_asm(a, b, c);
} 


__inline void fpsub503(const digit_t* a, const digit_t* b, digit_t* c)
{ // Modular subtraction, c = a-b mod p503.
  // Inputs: a, b in [0, 2*p503-1] 
  // Output: c in [0, 2*p503-1] 

    fpsub503_asm(a, b, c);
}


__inline void fpneg503(digit_t* a)
{ // Modular negation, a = -a mod p503.
  // Input/output: a in [0, 2*p503-1] 
    unsigned int i, borrow = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, ((digit_t*)p503x2)[i], a[i], borrow, a[i]); 
    }
}


void fpdiv2_503(const digit_t* a, digit_t* c)
{ // Modular division by two, c = a/2 mod p503.
  // Input : a in [0, 2*p503-1] 
  // Output: c in [0, 2*p503-1] 
    unsigned int i, carry = 0;
    digit_t mask;
        
    mask = 0 - (digit_t)(a[0] & 1);    // If a is odd compute a+p521
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(carry, a[i], ((digit_t*)p503)[i] & mask, carry, c[i]); 
    }

    mp_shiftr1(c, NWORDS_FIELD);
} 


void fpcorrection503(digit_t* a)
{ // Modular correction to reduce field element a in [0, 2*p503-1] to [0, p503-1].
    unsigned int i, borrow = 0;
    digit_t mask;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, a[i], ((digit_t*)p503)[i], borrow, a[i]);
    }
    mask = 0 - (digit_t)borrow;

    borrow = 0;
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(borrow, a[i], ((digit_t*)p503)[i] & mask, borrow, a[i]);
    }
}


void mp_mul(const digit_t* a, const digit_t* b, digit_t* c, const unsigned int nwords)
{ // Multiprecision multiply, c = a*b, where lng(a) = lng(b) = nwords.

    UNREFERENCED_PARAMETER(nwords);

    mul503_asm(a, b, c);
}



void rdc_mont(const digit_t* ma, digit_t* mc)
{ // Montgomery reduction exploiting special form of the prime.
  // mc = ma*R^-1 mod p503x2, where R = 2^512.
  // If ma < 2^512*p503, the output mc is in the range [0, 2*p503-1].
  // ma is assumed to be in Montgomery representation.
  
    rdc503_asm(ma, mc);
}
