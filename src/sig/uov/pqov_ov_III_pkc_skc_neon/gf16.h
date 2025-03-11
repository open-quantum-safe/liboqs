// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf16.h
/// @brief Library for arithmetics in GF(16) and GF(256)
///

#ifndef _GF16_H_
#define _GF16_H_

#include <stdint.h>




///////////////////////////////////////////
//
//  Arithmetics for one field element
//
//////////////////////////////////////////




static inline uint8_t gf16_is_nonzero(uint8_t a) {
    unsigned a4 = a & 0xf;
    unsigned r = ((unsigned) 0) - a4;
    r >>= 4;
    return r & 1;
}




// gf16 := gf2[x]/(x^4+x+1)
static inline uint8_t gf16_mul(uint8_t a, uint8_t b) {
    uint8_t r8 = (a & 1) * b;
    r8 ^= (a & 2) * b;
    r8 ^= (a & 4) * b;
    r8 ^= (a & 8) * b;

    // reduction
    uint8_t r4 = r8 ^ (((r8 >> 4) & 5) * 3); // x^4 = x+1  , x^6 = x^3 + x^2
    r4 ^= (((r8 >> 5) & 1) * 6);       // x^5 = x^2 + x
    return (r4 & 0xf);
}

static inline uint8_t gf16_squ(uint8_t a) {
    uint8_t r4 = a & 1;  // constant term
    r4 ^= (a << 1) & 4;  // x -> x^2
    r4 ^= ((a >> 2) & 1) * 3; // x^2 -> x^4 -> x+1
    r4 ^= ((a >> 3) & 1) * 12; // x^3 -> x^6 -> x^3+x^2
    return r4;
}


static inline uint8_t gf16_inv(uint8_t a) {
    // fermat inversion
    uint8_t a2 = gf16_squ(a);
    uint8_t a4 = gf16_squ(a2);
    uint8_t a8 = gf16_squ(a4);
    uint8_t a6 = gf16_mul(a4, a2);
    return gf16_mul(a8, a6);
}



////////////



static inline uint8_t gf256_is_nonzero(uint8_t a) {
    unsigned a8 = a;
    unsigned r = ((unsigned) 0) - a8;
    r >>= 8;
    return r & 1;
}



// gf256 := gf2[X]/ (x^8+x^4+x^3+x+1)   // 0x11b , AES field
static inline uint8_t gf256_mul(uint8_t a, uint8_t b) {
    uint8_t r = a * (b & 1);

    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 1) & 1);
    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 2) & 1);
    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 3) & 1);
    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 4) & 1);
    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 5) & 1);
    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 6) & 1);
    a = (a << 1) ^ ((a >> 7) * 0x1b);
    r ^= a * ((b >> 7) & 1);
    return r;
}


static inline uint8_t gf256_squ(uint8_t a) {
    uint8_t r8 = a & 1;
    r8 ^= (a << 1) & 4;    // x^1 -> x^2
    r8 ^= (a << 2) & (1 << 4); // x^2 -> x^4
    r8 ^= (a << 3) & (1 << 6); // x^3 -> x^6

    r8 ^= ((a >> 4) & 1) * 0x1b; // x^4 -> x^8  --> 0x1b
    r8 ^= ((a >> 5) & 1) * (0x1b << 2); // x^5 -> x^10  --> (0x1b<<2)
    r8 ^= ((a >> 6) & 1) * (0xab); // x^6 -> x^12  --> (0xab)
    r8 ^= ((a >> 7) & 1) * (0x9a); // x^7 -> x^14  --> (0x9a)

    return r8;
}

#define _GFINV_EXTGCD_

static inline uint8_t gf256_inv(uint8_t a) {
    #ifdef _GFINV_EXTGCD_
    // faster
    // extended GCD
    uint16_t f = 0x11b;
    uint16_t g = ((uint16_t)a) << 1;
    int16_t delta = 1;

    uint16_t r = 0x100;
    uint16_t v = 0;

    for (int i = 0; i < 15; i++) {
        uint16_t g0 = (g >> 8) & 1;
        uint16_t minus_delta = -delta;
        uint16_t swap = (minus_delta >> 15) & g0; // >>15 -> get sign bit
        //uint16_t f0g0 = g0;  // f0 is always 1

        // update delta
        delta = swap * (minus_delta << 1) + delta + 1;

        // update f, g, v, r
        uint16_t f_g = (f ^ g);
        g ^= (f * g0);
        f ^= (f_g) * swap;

        uint16_t v_r = (v ^ r);
        r ^= (v * g0);
        v ^= (v_r) * swap;

        g <<= 1;
        v >>= 1;
    }
    return v & 0xff;

    #else  //     #ifdef _GFINV_EXTGCD_
    // fermat inversion
    // 128+64+32+16+8+4+2 = 254
    uint8_t a2 = gf256_squ(a);
    uint8_t a4 = gf256_squ(a2);
    uint8_t a8 = gf256_squ(a4);
    uint8_t a4_2 = gf256_mul(a4, a2);
    uint8_t a8_4_2 = gf256_mul(a4_2, a8);
    uint8_t a64_ = gf256_squ(a8_4_2);
    a64_ = gf256_squ(a64_);
    a64_ = gf256_squ(a64_);
    uint8_t a64_2 = gf256_mul(a64_, a8_4_2);
    uint8_t a128_ = gf256_squ(a64_2);
    return gf256_mul(a2, a128_);
    #endif  //     #ifdef _GFINV_EXTGCD_
}






////////////////////////////////////////
//
//  library 32 bit vectors
//
////////////////////////////////////////



// gf16 := gf2[x]/(x^4+x+1)

static inline uint32_t gf16v_mul_u32(uint32_t a, uint8_t b) {
    uint32_t a_msb;
    uint32_t a32 = a;
    uint32_t b32 = b;
    uint32_t r32 = a32 * (b32 & 1);

    a_msb = a32 & 0x88888888; // MSB, 3rd bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 3) * 3);
    r32 ^= (a32) * ((b32 >> 1) & 1);

    a_msb = a32 & 0x88888888; // MSB, 3rd bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 3) * 3);
    r32 ^= (a32) * ((b32 >> 2) & 1);

    a_msb = a32 & 0x88888888; // MSB, 3rd bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 3) * 3);
    r32 ^= (a32) * ((b32 >> 3) & 1);

    return r32;

}


static inline uint32_t gf16v_squ_u32(uint32_t a) {
    uint32_t a01 = (a & 0x11111111) + ((a << 1) & 0x44444444);
    uint32_t a23 = (((a >> 2) & 0x11111111) + ((a >> 1) & 0x44444444)) * 3;
    return a01 ^ a23;
}



static inline uint32_t _gf16v_mul_u32_u32(uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t b0, uint32_t b1, uint32_t b2, uint32_t b3) {
    uint32_t c0 = a0 & b0;
    uint32_t c1 = (a0 & b1) ^ (a1 & b0);
    uint32_t c2 = (a0 & b2) ^ (a1 & b1) ^ (a2 & b0);
    uint32_t c3 = (a0 & b3) ^ (a1 & b2) ^ (a2 & b1) ^ (a3 & b0);
    uint32_t c4 = (a1 & b3) ^ (a2 & b2) ^ (a3 & b1);
    uint32_t c5 = (a2 & b3) ^ (a3 & b2);
    uint32_t c6 = a3 & b3;

    return c0 ^ (c1 << 1) ^ (c2 << 2) ^ (c3 << 3) ^ (c4 * 3) ^ (c5 * 6) ^ (c6 * 12);
}

static inline uint32_t gf16v_mul_u32_u32(uint32_t a, uint32_t b) {
    uint32_t a0 = a & 0x11111111;
    uint32_t a1 = (a >> 1) & 0x11111111;
    uint32_t a2 = (a >> 2) & 0x11111111;
    uint32_t a3 = (a >> 3) & 0x11111111;
    uint32_t b0 = b & 0x11111111;
    uint32_t b1 = (b >> 1) & 0x11111111;
    uint32_t b2 = (b >> 2) & 0x11111111;
    uint32_t b3 = (b >> 3) & 0x11111111;

    return _gf16v_mul_u32_u32(a0, a1, a2, a3, b0, b1, b2, b3);
}



/////////////////////////////



// gf256 := gf2[X]/ (x^8+x^4+x^3+x+1)   // 0x11b , AES field


static inline uint32_t gf256v_mul_u32(uint32_t a, uint8_t b) {
    uint32_t a_msb;
    uint32_t a32 = a;
    uint32_t b32 = b;
    uint32_t r32 = a32 * (b32 & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 1) & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 2) & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 3) & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 4) & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 5) & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 6) & 1);

    a_msb = a32 & 0x80808080; // MSB, 7th bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 7) * 0x1b);
    r32 ^= (a32) * ((b32 >> 7) & 1);

    return r32;
}

static inline uint32_t gf256v_squ_u32(uint32_t a) {

    uint32_t r32 = a & 0x01010101;
    r32 ^= (a << 1)   & 0x04040404; // x^1 -> x^2
    r32 ^= (a << 2)   & 0x10101010; // x^2 -> x^4
    r32 ^= (a << 3)   & 0x40404040; // x^3 -> x^6

    r32 ^= ((a >> 4) & 0x01010101) * 0x1b; // x^4 -> x^8  --> 0x1b
    r32 ^= ((a >> 5) & 0x01010101) * (0x1b << 2); // x^5 -> x^10  --> (0x1b<<2)
    r32 ^= ((a >> 6) & 0x01010101) * (0xab); // x^6 -> x^12  --> (0xab)
    r32 ^= ((a >> 7) & 0x01010101) * (0x9a); // x^7 -> x^14  --> (0x9a)

    return r32;
}





//////////////////////////////



//  return v[0]^v[1]^v[2]^v[3]
static inline uint8_t gf256v_reduce_u32(uint32_t a) {
    uint16_t *aa = (uint16_t *) (&a);
    uint16_t r = aa[0] ^ aa[1];
    uint8_t *rr = (uint8_t *) (&r);
    return rr[0] ^ rr[1];
}


static inline uint8_t gf16v_reduce_u32(uint32_t a) {
    uint8_t r256 = gf256v_reduce_u32(a);
    return (r256 & 0xf) ^ (r256 >> 4);
}










#endif // _GF16_H_

