// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_u32.h
/// @brief Inlined functions for implementing basic linear algebra functions for uint32 arch.
///

#ifndef _BLAS_U32_H_
#define _BLAS_U32_H_

#include "gf16.h"

#include <string.h>
#include <stdint.h>



static inline void _gf256v_add_u32_aligned(uint8_t *accu_b, const uint8_t *a, unsigned _num_byte) {
    while ( _num_byte >= 4 ) {
        uint32_t *bx = (uint32_t *)accu_b;
        uint32_t *ax = (uint32_t *)a;
        bx[0] ^= ax[0];
        a += 4;
        accu_b += 4;
        _num_byte -= 4;
    }
    while ( _num_byte ) {
        _num_byte--;
        accu_b[_num_byte] ^= a[_num_byte];
    }
}


static inline void _gf256v_add_u32(uint8_t *accu_b, const uint8_t *a, unsigned _num_byte) {
    uintptr_t bp = (uintptr_t)(const void *)accu_b;
    uintptr_t ap = (uintptr_t)(const void *)a;
    if ( !((bp & 3) || (ap & 3) || (_num_byte < 8)) ) {
        _gf256v_add_u32_aligned(accu_b, a, _num_byte);
        return;
    }

    while ( _num_byte ) {
        _num_byte--;
        accu_b[_num_byte] ^= a[_num_byte];
    }
}


////////////////


static inline void _gf256v_conditional_add_u32_aligned(uint8_t *accu_b, uint8_t condition, const uint8_t *a, unsigned _num_byte) {
    uint32_t pr_u32 = ((uint32_t) 0) - ((uint32_t) condition);
    uint8_t pr_u8 = pr_u32 & 0xff;

    while ( _num_byte >= 4 ) {
        uint32_t *bx = (uint32_t *)accu_b;
        uint32_t *ax = (uint32_t *)a;
        bx[0] ^= ax[0] & pr_u32;
        a += 4;
        accu_b += 4;
        _num_byte -= 4;
    }
    while ( _num_byte ) {
        _num_byte--;
        accu_b[_num_byte] ^= a[_num_byte] & pr_u8;
    }
}


static inline void _gf256v_conditional_add_u32(uint8_t *accu_b, uint8_t condition, const uint8_t *a, unsigned _num_byte) {
    uintptr_t bp = (uintptr_t)(const void *)accu_b;
    uintptr_t ap = (uintptr_t)(const void *)a;

    if ( !((bp & 3) || (ap & 3) || (_num_byte < 8)) ) {
        _gf256v_conditional_add_u32_aligned(accu_b, condition, a, _num_byte);
        return;
    }

    uint8_t pr_u8 = ((uint8_t)0) - condition;
    while ( _num_byte ) {
        _num_byte--;
        accu_b[_num_byte] ^= a[_num_byte] & pr_u8;
    }
}


///////////////////////////////////////////////////

#if defined(_VALGRIND_)
#include "valgrind/memcheck.h"
#endif


static inline void _gf16v_mul_scalar_u32_aligned(uint8_t *a, uint8_t gf16_b, unsigned _num_byte) {

    while ( _num_byte >= 4 ) {
        uint32_t *ax = (uint32_t *)a;
        ax[0] = gf16v_mul_u32( ax[0], gf16_b );
        a += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf16v_mul_u32(t.u32, gf16_b);
    for (unsigned i = 0; i < _num_byte; i++) {
        a[i] = t.u8[i];
    }
}



static inline void _gf16v_mul_scalar_u32(uint8_t *a, uint8_t gf16_b, unsigned _num_byte) {

    uintptr_t ap = (uintptr_t)(const void *)a;
    if ( !((ap & 3) || (_num_byte < 8)) ) {
        _gf16v_mul_scalar_u32_aligned(a, gf16_b, _num_byte);
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;

    while ( _num_byte >= 4 ) {
        t.u8[0] = a[0];
        t.u8[1] = a[1];
        t.u8[2] = a[2];
        t.u8[3] = a[3];
        t.u32 = gf16v_mul_u32(t.u32, gf16_b);
        a[0] = t.u8[0];
        a[1] = t.u8[1];
        a[2] = t.u8[2];
        a[3] = t.u8[3];
        a += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf16v_mul_u32(t.u32, gf16_b);
    for (unsigned i = 0; i < _num_byte; i++) {
        a[i] = t.u8[i];
    }

}


static inline void _gf256v_mul_scalar_u32_aligned(uint8_t *a, uint8_t b, unsigned _num_byte) {

    while ( _num_byte >= 4 ) {
        uint32_t *ax = (uint32_t *)a;
        ax[0] = gf256v_mul_u32( ax[0], b );
        a += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf256v_mul_u32(t.u32, b);
    for (unsigned i = 0; i < _num_byte; i++) {
        a[i] = t.u8[i];
    }
}




static inline void _gf256v_mul_scalar_u32(uint8_t *a, uint8_t b, unsigned _num_byte) {

    uintptr_t ap = (uintptr_t)(const void *)a;
    if ( !((ap & 3) || (_num_byte < 8)) ) {
        _gf256v_mul_scalar_u32_aligned(a, b, _num_byte);
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;

    while ( _num_byte >= 4 ) {
        t.u8[0] = a[0];
        t.u8[1] = a[1];
        t.u8[2] = a[2];
        t.u8[3] = a[3];
        t.u32 = gf256v_mul_u32(t.u32, b);
        a[0] = t.u8[0];
        a[1] = t.u8[1];
        a[2] = t.u8[2];
        a[3] = t.u8[3];
        a += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf256v_mul_u32(t.u32, b);
    for (unsigned i = 0; i < _num_byte; i++) {
        a[i] = t.u8[i];
    }

}

/////////////////////////////////////


static inline void _gf16v_madd_u32_aligned(uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned _num_byte) {
    while ( _num_byte >= 4 ) {
        const uint32_t *ax = (const uint32_t *)a;
        uint32_t *cx = (uint32_t *)accu_c;
        cx[0] ^= gf16v_mul_u32( ax[0], gf16_b );
        a += 4;
        accu_c += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf16v_mul_u32(t.u32, gf16_b);
    for (unsigned i = 0; i < _num_byte; i++) {
        accu_c[i] ^= t.u8[i];
    }
}

static inline void _gf16v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned _num_byte) {

    uintptr_t ap = (uintptr_t)(const void *)a;
    uintptr_t cp = (uintptr_t)(const void *)accu_c;
    if ( !((cp & 3) || (ap & 3) || (_num_byte < 8)) ) {
        _gf16v_madd_u32_aligned(accu_c, a, gf16_b, _num_byte);
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;

    while ( _num_byte >= 4 ) {
        t.u8[0] = a[0];
        t.u8[1] = a[1];
        t.u8[2] = a[2];
        t.u8[3] = a[3];
        t.u32 = gf16v_mul_u32(t.u32, gf16_b);
        accu_c[0] ^= t.u8[0];
        accu_c[1] ^= t.u8[1];
        accu_c[2] ^= t.u8[2];
        accu_c[3] ^= t.u8[3];
        a += 4;
        accu_c += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf16v_mul_u32(t.u32, gf16_b);
    for (unsigned i = 0; i < _num_byte; i++) {
        accu_c[i] ^= t.u8[i];
    }

}



static inline void _gf256v_madd_u32_aligned(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, unsigned _num_byte) {
    while ( _num_byte >= 4 ) {
        const uint32_t *ax = (const uint32_t *)a;
        uint32_t *cx = (uint32_t *)accu_c;
        cx[0] ^= gf256v_mul_u32( ax[0], gf256_b );
        a += 4;
        accu_c += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf256v_mul_u32(t.u32, gf256_b);
    for (unsigned i = 0; i < _num_byte; i++) {
        accu_c[i] ^= t.u8[i];
    }
}

static inline void _gf256v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, unsigned _num_byte) {

    uintptr_t ap = (uintptr_t)(const void *)a;
    uintptr_t cp = (uintptr_t)(const void *)accu_c;
    if ( !((cp & 3) || (ap & 3) || (_num_byte < 8)) ) {
        _gf256v_madd_u32_aligned(accu_c, a, gf256_b, _num_byte);
        return;
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;

    while ( _num_byte >= 4 ) {
        t.u8[0] = a[0];
        t.u8[1] = a[1];
        t.u8[2] = a[2];
        t.u8[3] = a[3];
        t.u32 = gf256v_mul_u32(t.u32, gf256_b);
        accu_c[0] ^= t.u8[0];
        accu_c[1] ^= t.u8[1];
        accu_c[2] ^= t.u8[2];
        accu_c[3] ^= t.u8[3];
        a += 4;
        accu_c += 4;
        _num_byte -= 4;
    }
    if ( 0 == _num_byte ) {
        return;
    }

    for (unsigned i = 0; i < _num_byte; i++) {
        t.u8[i] = a[i];
    }
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(&t.u32, 4);
    #endif
    t.u32 = gf256v_mul_u32(t.u32, gf256_b);
    for (unsigned i = 0; i < _num_byte; i++) {
        accu_c[i] ^= t.u8[i];
    }

}



#endif // _BLAS_U32_H_

