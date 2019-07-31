#include "gf.h"

//// gf4 := gf2[x]/x^2+x+1
static inline uint8_t gf4_mul_2(uint8_t a) {
    uint8_t r = (uint8_t) (a << 1);
    r ^= (uint8_t) ((a >> 1) * 7);
    return r;
}

static inline uint8_t gf4_mul(uint8_t a, uint8_t b) {
    uint8_t r = (uint8_t) (a * (b & 1));
    return r ^ (uint8_t)(gf4_mul_2(a) * (b >> 1));
}

static inline uint8_t gf4_squ(uint8_t a) {
    return a ^ (a >> 1);
}

static inline uint32_t gf4v_mul_2_u32(uint32_t a) {
    uint32_t bit0 = a & 0x55555555;
    uint32_t bit1 = a & 0xaaaaaaaa;
    return (bit0 << 1) ^ bit1 ^ (bit1 >> 1);
}

static inline uint32_t gf4v_mul_u32(uint32_t a, uint8_t b) {
    uint32_t bit0_b = ((uint32_t) 0) - ((uint32_t)(b & 1));
    uint32_t bit1_b = ((uint32_t) 0) - ((uint32_t)((b >> 1) & 1));
    return (a & bit0_b) ^ (bit1_b & gf4v_mul_2_u32(a));
}

//// gf16 := gf4[y]/y^2+y+x
static inline uint8_t gf16_mul(uint8_t a, uint8_t b) {
    uint8_t a0 = a & 3;
    uint8_t a1 = (a >> 2);
    uint8_t b0 = b & 3;
    uint8_t b1 = (b >> 2);
    uint8_t a0b0 = gf4_mul(a0, b0);
    uint8_t a1b1 = gf4_mul(a1, b1);
    uint8_t a0b1_a1b0 = gf4_mul(a0 ^ a1, b0 ^ b1) ^ a0b0 ^ a1b1;
    uint8_t a1b1_x2 = gf4_mul_2(a1b1);
    return (uint8_t) ((a0b1_a1b0 ^ a1b1) << 2 ^ a0b0 ^ a1b1_x2);
}

static inline uint8_t gf16_squ(uint8_t a) {
    uint8_t a0 = a & 3;
    uint8_t a1 = (a >> 2);
    a1 = gf4_squ(a1);
    uint8_t a1squ_x2 = gf4_mul_2(a1);
    return (uint8_t)((a1 << 2) ^ a1squ_x2 ^ gf4_squ(a0));
}

// gf16 := gf4[y]/y^2+y+x
uint32_t PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf16v_mul_u32(uint32_t a, uint8_t b) {
    uint32_t axb0 = gf4v_mul_u32(a, b);
    uint32_t axb1 = gf4v_mul_u32(a, b >> 2);
    uint32_t a0b1 = (axb1 << 2) & 0xcccccccc;
    uint32_t a1b1 = axb1 & 0xcccccccc;
    uint32_t a1b1_2 = a1b1 >> 2;

    return axb0 ^ a0b1 ^ a1b1 ^ gf4v_mul_2_u32(a1b1_2);
}


uint8_t PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256_is_nonzero(uint8_t a) {
    unsigned a8 = a;
    unsigned r = ((unsigned) 0) - a8;
    r >>= 8;
    return r & 1;
}

static inline uint8_t gf4_mul_3(uint8_t a) {
    uint8_t msk = (uint8_t) ((a - 2) >> 1);
    return (uint8_t) ((msk & ((int)a * 3)) | ((~msk) & ((int)a - 1)));
}
static inline uint8_t gf16_mul_8(uint8_t a) {
    uint8_t a0 = a & 3;
    uint8_t a1 = a >> 2;
    return (uint8_t) ((gf4_mul_2(a0 ^ a1) << 2) | gf4_mul_3(a1));
}

// gf256 := gf16[X]/X^2+X+xy
static inline uint8_t gf256_mul(uint8_t a, uint8_t b) {
    uint8_t a0 = a & 15;
    uint8_t a1 = (a >> 4);
    uint8_t b0 = b & 15;
    uint8_t b1 = (b >> 4);
    uint8_t a0b0 = gf16_mul(a0, b0);
    uint8_t a1b1 = gf16_mul(a1, b1);
    uint8_t a0b1_a1b0 = gf16_mul(a0 ^ a1, b0 ^ b1) ^ a0b0 ^ a1b1;
    uint8_t a1b1_x8 = gf16_mul_8(a1b1);
    return (uint8_t)((a0b1_a1b0 ^ a1b1) << 4 ^ a0b0 ^ a1b1_x8);
}

static inline uint8_t gf256_squ(uint8_t a) {
    uint8_t a0 = a & 15;
    uint8_t a1 = (a >> 4);
    a1 = gf16_squ(a1);
    uint8_t a1squ_x8 = gf16_mul_8(a1);
    return (uint8_t)((a1 << 4) ^ a1squ_x8 ^ gf16_squ(a0));
}

uint8_t PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256_inv(uint8_t a) {
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
}

static inline uint32_t gf4v_mul_3_u32(uint32_t a) {
    uint32_t bit0 = a & 0x55555555;
    uint32_t bit1 = a & 0xaaaaaaaa;
    return (bit0 << 1) ^ bit0 ^ (bit1 >> 1);
}
static inline uint32_t gf16v_mul_8_u32(uint32_t a) {
    uint32_t a1 = a & 0xcccccccc;
    uint32_t a0 = (a << 2) & 0xcccccccc;
    return gf4v_mul_2_u32(a0 ^ a1) | gf4v_mul_3_u32(a1 >> 2);
}
uint32_t PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256v_mul_u32(uint32_t a, uint8_t b) {
    uint32_t axb0 = PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf16v_mul_u32(a, b);
    uint32_t axb1 = PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf16v_mul_u32(a, b >> 4);
    uint32_t a0b1 = (axb1 << 4) & 0xf0f0f0f0;
    uint32_t a1b1 = axb1 & 0xf0f0f0f0;
    uint32_t a1b1_4 = a1b1 >> 4;

    return axb0 ^ a0b1 ^ a1b1 ^ gf16v_mul_8_u32(a1b1_4);
}
