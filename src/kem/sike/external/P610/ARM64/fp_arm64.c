/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: modular arithmetic optimized for 64-bit ARMv8 platforms for P610
*********************************************************************************************/

#include "../P610_internal.h"

/* OQS note: this file is #include'd with the defs of these consts; removed to avoid re-defs
// Global constants
extern const uint64_t p610[NWORDS_FIELD];
extern const uint64_t p610p1[NWORDS_FIELD]; 
extern const uint64_t p610x2[NWORDS_FIELD];  
extern const uint64_t p610x4[NWORDS_FIELD];
*/

__inline void mp_sub610_p2(const digit_t* a, const digit_t* b, digit_t* c)
{ // Multiprecision subtraction with correction with 2*p, c = a-b+2p. 
    
    oqs_kem_sike_mp_sub610_p2_asm(a, b, c); 
} 


__inline void mp_sub610_p4(const digit_t* a, const digit_t* b, digit_t* c)
{ // Multiprecision subtraction with correction with 4*p, c = a-b+4p. 
    
    oqs_kem_sike_mp_sub610_p4_asm(a, b, c);
}


__inline void fpadd610(const digit_t* a, const digit_t* b, digit_t* c)
{ // Modular addition, c = a+b mod p610.
  // Inputs: a, b in [0, 2*p610-1] 
  // Output: c in [0, 2*p610-1]

    oqs_kem_sike_fpadd610_asm(a, b, c);
} 


__inline void fpsub610(const digit_t* a, const digit_t* b, digit_t* c)
{ // Modular subtraction, c = a-b mod p610.
  // Inputs: a, b in [0, 2*p610-1] 
  // Output: c in [0, 2*p610-1] 

    oqs_kem_sike_fpsub610_asm(a, b, c);
}


__inline void fpneg610(digit_t* a)
{ // Modular negation, a = -a mod p610.
  // Input/output: a in [0, 2*p610-1] 
    unsigned int i, borrow = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, ((digit_t*)p610x2)[i], a[i], borrow, a[i]); 
    }
}


void fpdiv2_610(const digit_t* a, digit_t* c)
{ // Modular division by two, c = a/2 mod p610.
  // Input : a in [0, 2*p610-1] 
  // Output: c in [0, 2*p610-1] 
    unsigned int i, carry = 0;
    digit_t mask;
        
    mask = 0 - (digit_t)(a[0] & 1);    // If a is odd compute a+p521
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(carry, a[i], ((digit_t*)p610)[i] & mask, carry, c[i]); 
    }

    mp_shiftr1(c, NWORDS_FIELD);
} 


void fpcorrection610(digit_t* a)
{ // Modular correction to reduce field element a in [0, 2*p610-1] to [0, p610-1].
    unsigned int i, borrow = 0;
    digit_t mask;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, a[i], ((digit_t*)p610)[i], borrow, a[i]);
    }
    mask = 0 - (digit_t)borrow;

    borrow = 0;
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(borrow, a[i], ((digit_t*)p610)[i] & mask, borrow, a[i]);
    }
}


void mp_mul(const digit_t* a, const digit_t* b, digit_t* c, const unsigned int nwords)
{ // Multiprecision multiply, c = a*b, where lng(a) = lng(b) = nwords.

    UNREFERENCED_PARAMETER(nwords);

    oqs_kem_sike_mul610_asm(a, b, c);
}



void rdc_mont(digit_t* ma, digit_t* mc)
{ // Montgomery reduction exploiting special form of the prime.
  // mc = ma*R^-1 mod p610x2, where R = 2^512.
  // If ma < 2^512*p610, the output mc is in the range [0, 2*p610-1].
  // ma is assumed to be in Montgomery representation.
  
    oqs_kem_sike_rdc610_asm(ma, mc);
}
