/**
 *  ff.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef ff_h
#define ff_h

#include <stdint.h>

typedef uint16_t ff_unit;

/**
 *  Finite field F_{2^m}
 *
 *  @note
 *  The addition, multiplication, squaring and inversion operations
 *  are done without the use of any look-up tables. Only m = {12,13}
 *  cases are implemented at this stage.
 **/
typedef struct FF2m {
    /**
     *  Dimension of extension over F_2
     **/
    int m;
    
    /**
     *  Add two finite-field elements over F_{2^m}
     *
     *  @param[in] ff2m  Pointer to FF2m instance
     *  @param[in] a     Operand a
     *  @param[in] b     Operand b
     *  @return Sum of a and b in the field F_{2^m}
     **/
    ff_unit (*ff_add)(const struct FF2m* ff2m, ff_unit a, ff_unit b);
    
    /**
     *  Multiply two finite-field elements over F_{2^m}
     *
     *  @param[in] ff2m  Pointer to FF2m instance
     *  @param[in] a     Operand a
     *  @param[in] b     Operand b
     *  @return Product of a and b in the field F_{2^m}
     **/
    ff_unit (*ff_mul)(const struct FF2m* ff2m, ff_unit a, ff_unit b);
    
    /**
     *  Squaring of an element over F_{2^m}
     *
     *  @param[in] ff2m  Pointer to FF2m instance
     *  @param[in] a     Operand a
     *  @return a^2 in the field F_{2^m}
     **/
    ff_unit (*ff_sqr)(const struct FF2m* ff2m, ff_unit a);
    
    /**
     *  Inversion of an element over F_{2^m}
     *
     *  @param[in] ff2m  Pointer to FF2m instance
     *  @param[in] a     Operand a
     *  @return a^{-1} in the field F_{2^m}
     **/
    ff_unit (*ff_inv)(const struct FF2m* ff2m, ff_unit a);
    
    /**
     *  Basis
     **/
    ff_unit* basis;

} FF2m;

/**
 *  Create an instance of FF2m object
 *  
 *  @return FF2m instance on success, otherwise NULL
 **/
FF2m* PQCLEAN_NTSKEM1264_CLEAN_ff_create(void);

/**
 *  Release an FF2m instance created by {@see ff_create}.
 *
 *  @param[in] ff2m  Instance to an FF2m object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_ff_release(FF2m* ff2m);

#endif /* ff_h */
