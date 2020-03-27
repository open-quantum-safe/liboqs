#include "gf.h"

//// gf4 := gf2[x]/x^2+x+1
static inline uint8_t gf4_mul_2(uint8_t a) {
    uint8_t r = (uint8_t)(a << 1);
    r ^= (uint8_t)((a >> 1) * 7);
    return r;
}

static inline uint8_t gf4_mul(uint8_t a, uint8_t b) {
    uint8_t r = (uint8_t)(a * (b & 1));
    return r ^ (uint8_t)(gf4_mul_2(a) * (b >> 1));
}

static inline uint8_t gf4_squ(uint8_t a) {
    return a ^ (a >> 1);
}

//// gf16 := gf4[y]/y^2+y+x
uint8_t PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_mul(uint8_t a, uint8_t b) {
    uint8_t a0 = a & 3;
    uint8_t a1 = (a >> 2);
    uint8_t b0 = b & 3;
    uint8_t b1 = (b >> 2);
    uint8_t a0b0 = gf4_mul(a0, b0);
    uint8_t a1b1 = gf4_mul(a1, b1);
    uint8_t a0b1_a1b0 = gf4_mul(a0 ^ a1, b0 ^ b1) ^ a0b0 ^ a1b1;
    uint8_t a1b1_x2 = gf4_mul_2(a1b1);
    return (uint8_t)((a0b1_a1b0 ^ a1b1) << 2 ^ a0b0 ^ a1b1_x2);
}

static inline uint8_t gf16_squ(uint8_t a) {
    uint8_t a0 = a & 3;
    uint8_t a1 = (a >> 2);
    a1 = gf4_squ(a1);
    uint8_t a1squ_x2 = gf4_mul_2(a1);
    return (uint8_t)((a1 << 2) ^ a1squ_x2 ^ gf4_squ(a0));
}

uint8_t PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_is_nonzero(uint8_t a) {
    unsigned int a4 = a & 0xf;
    unsigned int r = ((unsigned int)0) - a4;
    r >>= 4;
    return r & 1;
}

uint8_t PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_inv(uint8_t a) {
    uint8_t a2 = gf16_squ(a);
    uint8_t a4 = gf16_squ(a2);
    uint8_t a8 = gf16_squ(a4);
    uint8_t a6 = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_mul(a4, a2);
    return PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_mul(a8, a6);
}

