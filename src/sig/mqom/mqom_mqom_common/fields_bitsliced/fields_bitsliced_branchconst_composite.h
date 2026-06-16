#ifndef __FIELDS_BITSLICED_BRANCHCONST_H__
#define __FIELDS_BITSLICED_BRANCHCONST_H__

#include "fields_handling.h"

__attribute__((weak)) const uint8_t GF256_TO_COMPOSITE_TABLE[256] = {
	0, 1, 65, 64, 102, 103, 39, 38, 108, 109, 45, 44, 10, 11, 75, 74, 86, 87, 23, 22, 48, 49, 113, 112, 58, 59, 123, 122, 92, 93, 29, 28, 154, 155, 219, 218, 252, 253, 189, 188, 246, 247, 183, 182, 144, 145, 209, 208, 204, 205, 141, 140, 170, 171, 235, 234, 160, 161, 225, 224, 198, 199, 135, 134, 88, 89, 25, 24, 62, 63, 127, 126, 52, 53, 117, 116, 82, 83, 19, 18, 14, 15, 79, 78, 104, 105, 41, 40, 98, 99, 35, 34, 4, 5, 69, 68, 194, 195, 131, 130, 164, 165, 229, 228, 174, 175, 239, 238, 200, 201, 137, 136, 148, 149, 213, 212, 242, 243, 179, 178, 248, 249, 185, 184, 158, 159, 223, 222, 196, 197, 133, 132, 162, 163, 227, 226, 168, 169, 233, 232, 206, 207, 143, 142, 146, 147, 211, 210, 244, 245, 181, 180, 254, 255, 191, 190, 152, 153, 217, 216, 94, 95, 31, 30, 56, 57, 121, 120, 50, 51, 115, 114, 84, 85, 21, 20, 8, 9, 73, 72, 110, 111, 47, 46, 100, 101, 37, 36, 2, 3, 67, 66, 156, 157, 221, 220, 250, 251, 187, 186, 240, 241, 177, 176, 150, 151, 215, 214, 202, 203, 139, 138, 172, 173, 237, 236, 166, 167, 231, 230, 192, 193, 129, 128, 6, 7, 71, 70, 96, 97, 33, 32, 106, 107, 43, 42, 12, 13, 77, 76, 80, 81, 17, 16, 54, 55, 119, 118, 60, 61, 125, 124, 90, 91, 27, 26
};

static inline void gf256_bitslice32_vect_pack_post_composite(uint32_t x_bitsliced_[8]) {
	/* XXX: NOTE: the 'volatile' madness below is here because gcc-15 seems to use an unsound optimization in 'O3' ... */
	volatile uint32_t *x_bitsliced = (volatile uint32_t*)x_bitsliced_;
	uint32_t y0 = x_bitsliced[0], y1 = x_bitsliced[1], y2 = x_bitsliced[2], y3 = x_bitsliced[3];
	uint32_t y4 = x_bitsliced[4], y5 = x_bitsliced[5], y6 = x_bitsliced[6], y7 = x_bitsliced[7];
	x_bitsliced[0] = y0 ^ y1; // 1
	x_bitsliced[1] = y2 ^ y4 ^ y5; // a
	x_bitsliced[2] = y2 ^ y3 ^ y4 ^ y7; // b
	x_bitsliced[3] = y3 ^ y5 ^ y6; // b*a
	x_bitsliced[4] = y4 ^ y5 ^ y6; // c
	x_bitsliced[5] = y2 ^ y3; // c*a
	x_bitsliced[6] = y1 ^ y2 ^ y3 ^ y4 ^ y6 ^ y7; // c*b
	x_bitsliced[7] = y5 ^ y7; // c*b*a
}

static inline void gf256_bitslice32_vect_unpack_pre_composite(uint32_t x_bitsliced_[8]) {
	/* XXX: NOTE: the 'volatile' madness below is here because gcc-15 seems to use an unsound optimization in 'O3' ... */
	volatile uint32_t *x_bitsliced = (volatile uint32_t*)x_bitsliced_;
	uint32_t y0 = x_bitsliced[0], y1 = x_bitsliced[1], y2 = x_bitsliced[2], y3 = x_bitsliced[3];
	uint32_t y4 = x_bitsliced[4], y5 = x_bitsliced[5], y6 = x_bitsliced[6], y7 = x_bitsliced[7];
	x_bitsliced[0] = y0 ^ y4 ^ y5 ^ y6 ^ y7;
	x_bitsliced[1] = y4 ^ y5 ^ y6 ^ y7;
	x_bitsliced[2] = y1 ^ y2 ^ y5 ^ y7;
	x_bitsliced[3] = y1 ^ y2 ^ y7;
	x_bitsliced[4] = y1 ^ y2 ^ y3 ^ y4 ^ y7;
	x_bitsliced[5] = y1 ^ y3 ^ y4 ^ y5;
	x_bitsliced[6] = y2 ^ y4 ^ y5 ^ y7;
	x_bitsliced[7] = y1 ^ y3 ^ y4 ^ y5 ^ y7;
}

static inline void gf256_bitslice32_vect_pack_gf2_composite(uint8_t x, uint32_t x_bitsliced[8], uint32_t index32) {
	x_bitsliced[0] |= (x << index32);
}

static inline void gf256_bitslice32_vect_pack_gf16_composite(uint8_t x, uint32_t x_bitsliced[8], uint32_t index32) {
	uint8_t b0 = x & 0x01, b1 = (x >> 1) & 0x01, b2 = (x >> 2) & 0x01, b3 = (x >> 3) & 0x01;
	x_bitsliced[0] |= ((b0 ^ b2) << index32);
	//x_bitsliced[1] |= ((0) << index32);
	x_bitsliced[2] |= ((b2) << index32);
	x_bitsliced[3] |= ((b2) << index32);
	x_bitsliced[4] |= ((b2 ^ b3) << index32);
	x_bitsliced[5] |= ((b1 ^ b3) << index32);
	x_bitsliced[6] |= ((b1 ^ b2) << index32);
	x_bitsliced[7] |= ((b1 ^ b3) << index32);
}

static inline void gf256_bitslice32_vect_pack_gf256_composite(uint8_t x, uint32_t x_bitsliced[8], uint32_t index32) {
	for (uint32_t k = 0; k < 8; k++) {
		uint32_t b = (x >> k) & 0x01;
		x_bitsliced[k] |= (b << index32);
	}
}

static inline uint8_t gf256_bitslice32_vect_unpack_gf256_composite(const uint32_t x_bitsliced[8], uint32_t index32) {
	uint8_t x = 0;
	for (uint32_t k = 0; k < 8; k++) {
		uint8_t b = (x_bitsliced[k] >> index32) & 0x01;
		x |= (b << k);
	}
	return x;
}

static inline void gf256_bitslice32_add_composite(uint32_t z_bitsliced[8], const uint32_t x_bitsliced[8], const uint32_t y_bitsliced[8]) {
	for (uint32_t k = 0; k < 8; k++) {
		z_bitsliced[k] = x_bitsliced[k] ^ y_bitsliced[k];
	}
}

#define GF4_MUL(r0, r1, x0, x1, y0, y1)                 \
do {                                                    \
    uint32_t _x1y1 = (x1) & (y1);                         \
    (r0) = ((x0) & (y0)) ^ _x1y1;                       \
    (r1) = ((x0) & (y1)) ^ ((x1) & (y0)) ^ _x1y1;       \
} while (0)

#define GF16_MUL(r0, r1, r2, r3, x0, x1, x2, x3, y0, y1, y2, y3)     \
do {                                                    \
    uint32_t _r00, _r01, _r10, _r11, _r20, _r21;        \
    uint32_t _xs0 = (x0) ^ (x2);                \
    uint32_t _xs1 = (x1) ^ (x3);                \
    uint32_t _ys0 = (y0) ^ (y2);                        \
    uint32_t _ys1 = (y1) ^ (y3);                        \
    GF4_MUL(_r00, _r01, _xs0, _xs1, _ys0, _ys1);         /* (x0+x1)*(yl+yh) */ \
    GF4_MUL(_r10, _r11, (x0), (x1), (y0), (y1));         /* x0*yl           */ \
    GF4_MUL(_r20, _r21, (x2), (x3), (y2), (y3));         /* x1*yh           */ \
    (r0) = _r10 ^ _r21;                                 \
    (r1) = _r11 ^ _r20 ^ _r21;                          \
    (r2) = _r00 ^ _r10;                                 \
    (r3) = _r01 ^ _r11;                                 \
} while (0)

#define GF16_MUL_CST(r0, r1, r2, r3, y0, y1, y2, y3)     \
do {                                                    \
    (r0) = (y2) ^ (y3);                                 \
    (r1) = (y3) ^ (r0);                          \
    (r2) = (y1) ^ (y3);                                 \
    (r3) = (y0) ^ (y2) ^ (r2);                                 \
} while (0)

static inline void gf256_bitslice32_mult_composite(uint32_t res[8], const uint32_t x[8], const uint32_t y[8]) {
	/* Charge toutes les tranches en registres (évite rereads SRAM) */
	const uint32_t x0 = x[0], x1 = x[1], x2 = x[2], x3 = x[3];
	const uint32_t x4 = x[4], x5 = x[5], x6 = x[6], x7 = x[7];
	const uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	const uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	/* Sommes xl ^ xh (en regs) */
	const uint32_t xs0 = x0 ^ x4;
	const uint32_t xs1 = x1 ^ x5;
	const uint32_t xs2 = x2 ^ x6;
	const uint32_t xs3 = x3 ^ x7;

	/* Sommes yl ^ yh (en regs) */
	const uint32_t ys0 = y0 ^ y4;
	const uint32_t ys1 = y1 ^ y5;
	const uint32_t ys2 = y2 ^ y6;
	const uint32_t ys3 = y3 ^ y7;

	/* Trois produits GF(16) enregs: r0 = (x0+x1)*(yl+yh), r1 = x0*yl, r2 = x1*yh */
	uint32_t r0_0, r0_1, r0_2, r0_3;
	uint32_t r1_0, r1_1, r1_2, r1_3;
	uint32_t r2_0, r2_1, r2_2, r2_3;

	GF16_MUL(r0_0, r0_1, r0_2, r0_3, xs0, xs1, xs2, xs3, ys0, ys1, ys2, ys3);
	GF16_MUL(r1_0, r1_1, r1_2, r1_3, x0, x1, x2, x3, y0, y1, y2, y3);
	GF16_MUL(r2_0, r2_1, r2_2, r2_3, x4, x5, x6, x7, y4, y5, y6, y7);

	/* Réduction quadratique (même motif que niveau inférieur), tout en regs */
	uint32_t o0, o1, o2, o3;
	GF16_MUL_CST(o0, o1, o2, o3, r2_0, r2_1, r2_2, r2_3);
	o0 ^= r1_0;
	o1 ^= r1_1;
	o2 ^= r1_2;
	o3 ^= r1_3;

	uint32_t o4 = r0_0 ^ r1_0;
	uint32_t o5 = r0_1 ^ r1_1;
	uint32_t o6 = r0_2 ^ r1_2;
	uint32_t o7 = r0_3 ^ r1_3;

	/* Un seul burst d'écritures (8 words) */
	res[0] = o0;
	res[1] = o1;
	res[2] = o2;
	res[3] = o3;
	res[4] = o4;
	res[5] = o5;
	res[6] = o6;
	res[7] = o7;
}

#define GF4_MUL_HYBRID(r0, r1, x, y0, y1)               \
do {                                                    \
    uint32_t _mx0 = (uint32_t)(0u - ((x) & 1u));        \
    uint32_t _mx1 = (uint32_t)(0u - (((x) >> 1) & 1u)); \
    uint32_t _x1y1 = _mx1 & (y1);                       \
    (r0) = (_mx0 & (y0)) ^ _x1y1;                       \
    (r1) = (_mx0 & (y1)) ^ (_mx1 & (y0)) ^ _x1y1;       \
} while (0)

#define GF16_MUL_HYBRID(r0, r1, r2, r3, x, y0, y1, y2, y3)\
do {                                                     \
    uint32_t _r00, _r01, _r10, _r11, _r20, _r21;         \
    uint8_t  _x0 = (uint8_t)((x) & 0x3u);                \
    uint8_t  _x1 = (uint8_t)(((x) >> 2) & 0x3u);         \
    uint8_t  _sx = (uint8_t)(_x0 ^ _x1);                 \
    uint32_t _ys0 = (y0) ^ (y2);                         \
    uint32_t _ys1 = (y1) ^ (y3);                         \
    GF4_MUL_HYBRID(_r00, _r01, _sx, _ys0, _ys1);        /* (x0+x1)*(yl+yh) */ \
    GF4_MUL_HYBRID(_r10, _r11, _x0, (y0), (y1));        /* x0*yl           */ \
    GF4_MUL_HYBRID(_r20, _r21, _x1, (y2), (y3));        /* x1*yh           */ \
    (r0) = _r10 ^ _r21;                                  \
    (r1) = _r11 ^ _r20 ^ _r21;                           \
    (r2) = _r00 ^ _r10;                                  \
    (r3) = _r01 ^ _r11;                                  \
} while (0)

static inline void gf256_bitslice32_mult_hybrid_composite(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	const uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	const uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	uint8_t b0 =  x       & 0x01, b1 = (x >> 1) & 0x01, b2 = (x >> 2) & 0x01, b3 = (x >> 3) & 0x01;
	uint8_t b4 = (x >> 4) & 0x01, b5 = (x >> 5) & 0x01, b6 = (x >> 6) & 0x01, b7 = (x >> 7) & 0x01;
	const uint8_t _x = b0 ^ ((-b1) & 0x41) ^ ((-b2) & 0x66) ^ ((-b3) & 0x6c) ^ ((-b4) & 0x56) ^ ((-b5) & 0x9a) ^ ((-b6) & 0x58) ^ ((-b7) & 0xc4) ;
	const uint8_t x0 = (uint8_t)(_x & 0x0Fu);
	const uint8_t x1 = (uint8_t)((_x >> 4) & 0x0Fu);
	const uint8_t sx = (uint8_t)(x0 ^ x1);

	const uint32_t ys0 = y0 ^ y4;
	const uint32_t ys1 = y1 ^ y5;
	const uint32_t ys2 = y2 ^ y6;
	const uint32_t ys3 = y3 ^ y7;

	uint32_t r0_0, r0_1, r0_2, r0_3;
	uint32_t r1_0, r1_1, r1_2, r1_3;
	uint32_t r2_0, r2_1, r2_2, r2_3;

	GF16_MUL_HYBRID(r0_0, r0_1, r0_2, r0_3, sx, ys0, ys1, ys2, ys3);
	GF16_MUL_HYBRID(r1_0, r1_1, r1_2, r1_3, x0, y0, y1, y2, y3);
	GF16_MUL_HYBRID(r2_0, r2_1, r2_2, r2_3, x1, y4, y5, y6, y7);

	uint32_t o0, o1, o2, o3;
	GF16_MUL_HYBRID(o0, o1, o2, o3, 0x08, r2_0, r2_1, r2_2, r2_3);
	o0 ^= r1_0;
	o1 ^= r1_1;
	o2 ^= r1_2;
	o3 ^= r1_3;

	uint32_t o4 = r0_0 ^ r1_0;
	uint32_t o5 = r0_1 ^ r1_1;
	uint32_t o6 = r0_2 ^ r1_2;
	uint32_t o7 = r0_3 ^ r1_3;

	res[0] = o0;
	res[1] = o1;
	res[2] = o2;
	res[3] = o3;
	res[4] = o4;
	res[5] = o5;
	res[6] = o6;
	res[7] = o7;
}

static inline void gf256_bitslice32_mult_hybrid_gf2_composite(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	gf256_bitslice32_mult_hybrid_composite(res, x, y);
}

static inline void gf256_bitslice32_mult_hybrid_gf16_composite(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	x = (x & 1) ^ (-((x >> 1) & 1) & 0xE0) ^ (-((x >> 2) & 1) & 0x5D) ^ (-(x >> 3) & 0xB0);
	gf256_bitslice32_mult_hybrid_composite(res, x, y);
}

static inline void gf256_bitslice32_mult_hybrid_gf256_composite(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	gf256_bitslice32_mult_hybrid_composite(res, x, y);
}

#if defined(FIELDS_BITSLICE_PUBLIC_JUMP)
#define GF16_BRANCH_MUL(r0, r1, r2, r3, x, y0, y1, y2, y3)     \
do {                                                    \
    if(x < 8) { \
        if(x < 4) { \
            if(x < 2) { \
                if(x == 0) { /* x == 0 */ \
                    (r0) = 0; \
                    (r1) = 0; \
                    (r2) = 0; \
                    (r3) = 0; \
                } else { /* x == 1 */ \
                    (r0) = (y0); \
                    (r1) = (y1); \
                    (r2) = (y2); \
                    (r3) = (y3); \
                } \
            } else { \
                if(x == 2) { /* x == 2 */ \
                    (r0) = (y1); \
                    (r1) = (y1) ^ (y0); \
                    (r2) = (y3); \
                    (r3) = (y2) ^ (y3); \
                } else { /* x == 3 */ \
                    (r0) = (y1) ^ (y0); \
                    (r1) = (y0); \
                    (r2) = (y3) ^ (y2); \
                    (r3) = (y2); \
                } \
            } \
        } else { \
            if(x < 6) { \
                if(x == 4) { /* x == 4 */ \
                    (r0) = (y3); \
                    (r1) = (y2) ^ (y3); \
                    (r2) = (y0) ^ (y2); \
                    (r3) = (y1) ^ (y3); \
                } else { /* x == 5 */ \
                    (r0) = (y0) ^ (y3); \
                    (r1) = (y1) ^ (y2) ^ (y3); \
                    (r2) = (y0); \
                    (r3) = (y1); \
                } \
            } else { \
                if(x == 6) { /* x == 6 */ \
                    (r0) = (y1) ^ (y3); \
                    /*(r1) = (y0) ^ (y1) ^ (y2) ^ (y3);*/ \
                    (r2) = (y0) ^ (y2) ^ (y3); \
                    (r1) = (r2) ^ (y1); \
                    (r3) = (y1) ^ (y2); \
                } else { /* x == 7 */ \
                    /*(r0) = (y0) ^ (y1) ^ (y3);*/ \
                    /*(r1) = (y0) ^ (y2) ^ (y3);*/ \
                    (r2) = (y0) ^ (y3); \
                    (r0) = (r2) ^ (y1); \
                    (r1) = (r2) ^ (y2); \
                    (r3) = (y1) ^ (y2) ^ (y3); \
                } \
            } \
        } \
    } else { \
        if(x < 12) { \
            if(x < 10) { \
                if(x == 8) { /* x == 8 */ \
                    (r0) = (y2) ^ (y3); \
                    (r1) = (y2); \
                    (r2) = (y1) ^ (y3); \
                    /*(r3) = (y0) ^ (y1) ^ (y2) ^ (y3);*/ \
                    (r3) = (r0) ^ (y0) ^ (y1); \
                } else { /* x == 9 */ \
                    uint32_t tmp = (y2) ^ (y3); \
                    (r0) = (y0) ^ tmp; \
                    (r1) = (y1) ^ (y2); \
                    (r2) = (y1) ^ tmp; \
                    /*(r3) = (y0) ^ (y1) ^ (y2);*/ \
                    (r3) = (y0) ^ (r1); \
                } \
            } else { \
                if(x == 10) { /* x == 10 */ \
                    (r0) = (y1) ^ (y2) ^ (y3); \
                    /*(r1) = (y0) ^ (y1) ^ (y2);*/ \
                    (r2) = (y1); \
                    (r3) = (y0) ^ (y1); \
                    (r1) = (r3) ^ (y2); \
                } else { /* x == 11 */ \
                    /*(r0) = (y0) ^ (y1) ^ (y2) ^ (y3);*/ \
                    (r1) = (y0) ^ (y2); \
                    (r2) = (y1) ^ (y2); \
                    (r3) = (y0) ^ (y1) ^ (y3); \
                    (r0) = (r3) ^ (y2); \
                } \
            } \
        } else { \
            if(x < 14) { \
                if(x == 12) { /* x == 12 */ \
                    (r0) = (y2); \
                    (r1) = (y3); \
                    /*(r2) = (y0) ^ (y1) ^ (y2) ^ (y3);*/ \
                    (r3) = (y0) ^ (y2); \
                    (r2) = (r3) ^ (y1) ^ (y3); \
                } else { /* x == 13 */ \
                    (r0) = (y0) ^ (y2); \
                    (r1) = (y1) ^ (y3); \
                    /*(r2) = (y0) ^ (y1) ^ (y3);*/ \
                    (r2) = (y0) ^ (r1); \
                    /*(r3) = (y0) ^ (y2) ^ (y3);*/ \
                    (r3) = (r0) ^ (y3); \
                } \
            } else { \
                if(x == 14) { /* x == 14 */ \
                    (r0) = (y1) ^ (y2); \
                    /*(r1) = (y0) ^ (y1) ^ (y3);*/ \
                    /*(r2) = (y0) ^ (y1) ^ (y2);*/ \
                    (r2) = (y0) ^ (r0); \
                    (r3) = (y0) ^ (y3); \
                    (r1) = (r3) ^ (y1); \
                } else { /* x == 15 */ \
                    /*(r0) = (y0) ^ (y1) ^ (y2);*/ \
                    (r1) = (y0) ^ (y3); \
                    (r2) = (y0) ^ (y1); \
                    (r3) = (y0); \
                    (r0) = (r2) ^ (y2); \
                } \
            } \
        } \
    } \
} while (0)

static inline void gf256_bitslice32_mult_hybrid_public_composite(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	const uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	const uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	const uint8_t _x = GF256_TO_COMPOSITE_TABLE[x];
	const uint8_t x0 = (uint8_t)(_x & 0x0Fu);
	const uint8_t x1 = (uint8_t)((_x >> 4) & 0x0Fu);
	const uint8_t sx = (uint8_t)(x0 ^ x1);

	const uint32_t ys0 = y0 ^ y4;
	const uint32_t ys1 = y1 ^ y5;
	const uint32_t ys2 = y2 ^ y6;
	const uint32_t ys3 = y3 ^ y7;

	uint32_t r0_0, r0_1, r0_2, r0_3;
	uint32_t r1_0, r1_1, r1_2, r1_3;
	uint32_t r2_0, r2_1, r2_2, r2_3;

	GF16_BRANCH_MUL(r0_0, r0_1, r0_2, r0_3, sx, ys0, ys1, ys2, ys3);
	GF16_BRANCH_MUL(r1_0, r1_1, r1_2, r1_3, x0, y0, y1, y2, y3);
	GF16_BRANCH_MUL(r2_0, r2_1, r2_2, r2_3, x1, y4, y5, y6, y7);

	uint32_t o0, o1, o2, o3;
	GF16_BRANCH_MUL(o0, o1, o2, o3, 0x08, r2_0, r2_1, r2_2, r2_3);
	o0 ^= r1_0;
	o1 ^= r1_1;
	o2 ^= r1_2;
	o3 ^= r1_3;

	uint32_t o4 = r0_0 ^ r1_0;
	uint32_t o5 = r0_1 ^ r1_1;
	uint32_t o6 = r0_2 ^ r1_2;
	uint32_t o7 = r0_3 ^ r1_3;

	res[0] = o0;
	res[1] = o1;
	res[2] = o2;
	res[3] = o3;
	res[4] = o4;
	res[5] = o5;
	res[6] = o6;
	res[7] = o7;
}
#else
static inline void gf256_bitslice32_mult_hybrid_public_composite(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	const uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	const uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	const uint8_t _x = GF256_TO_COMPOSITE_TABLE[x];
	const uint8_t x0 = (uint8_t)(_x & 0x0Fu);
	const uint8_t x1 = (uint8_t)((_x >> 4) & 0x0Fu);
	const uint8_t sx = (uint8_t)(x0 ^ x1);

	const uint32_t ys0 = y0 ^ y4;
	const uint32_t ys1 = y1 ^ y5;
	const uint32_t ys2 = y2 ^ y6;
	const uint32_t ys3 = y3 ^ y7;

	uint32_t r0_0, r0_1, r0_2, r0_3;
	uint32_t r1_0, r1_1, r1_2, r1_3;
	uint32_t r2_0, r2_1, r2_2, r2_3;

	GF16_MUL_HYBRID(r0_0, r0_1, r0_2, r0_3, sx, ys0, ys1, ys2, ys3);
	GF16_MUL_HYBRID(r1_0, r1_1, r1_2, r1_3, x0, y0, y1, y2, y3);
	GF16_MUL_HYBRID(r2_0, r2_1, r2_2, r2_3, x1, y4, y5, y6, y7);

	uint32_t o0, o1, o2, o3;
	GF16_MUL_HYBRID(o0, o1, o2, o3, 0x08, r2_0, r2_1, r2_2, r2_3);
	o0 ^= r1_0;
	o1 ^= r1_1;
	o2 ^= r1_2;
	o3 ^= r1_3;

	uint32_t o4 = r0_0 ^ r1_0;
	uint32_t o5 = r0_1 ^ r1_1;
	uint32_t o6 = r0_2 ^ r1_2;
	uint32_t o7 = r0_3 ^ r1_3;

	res[0] = o0;
	res[1] = o1;
	res[2] = o2;
	res[3] = o3;
	res[4] = o4;
	res[5] = o5;
	res[6] = o6;
	res[7] = o7;
}
#endif

static inline void gf256to2_bitslice32_vect_pack_post_composite(uint32_t x_bitsliced[16]) {
	gf256_bitslice32_vect_pack_post_composite(&x_bitsliced[0]);
	gf256_bitslice32_vect_pack_post_composite(&x_bitsliced[8]);
}

static inline void gf256to2_bitslice32_vect_unpack_pre_composite(uint32_t x_bitsliced[16]) {
	gf256_bitslice32_vect_unpack_pre_composite(&x_bitsliced[0]);
	gf256_bitslice32_vect_unpack_pre_composite(&x_bitsliced[8]);
}

static inline void gf256to2_bitslice32_vect_pack_gf2_composite(uint8_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	x_bitsliced[0] |= (x << index32);
}

static inline void gf256to2_bitslice32_vect_pack_gf16_composite(uint8_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	uint8_t b0 = x & 0x01, b1 = (x >> 1) & 0x01, b2 = (x >> 2) & 0x01, b3 = (x >> 3) & 0x01;
	x_bitsliced[0] |= ((b0 ^ b2) << index32);
	//x_bitsliced[1] |= ((0) << index32);
	x_bitsliced[2] |= ((b2) << index32);
	x_bitsliced[3] |= ((b2) << index32);
	x_bitsliced[4] |= ((b2 ^ b3) << index32);
	x_bitsliced[5] |= ((b1 ^ b3) << index32);
	x_bitsliced[6] |= ((b1 ^ b2) << index32);
	x_bitsliced[7] |= ((b1 ^ b3) << index32);
}

static inline void gf256to2_bitslice32_vect_pack_gf256_composite(uint8_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	for (uint32_t k = 0; k < 8; k++) {
		uint32_t b = (x >> k) & 0x01;
		x_bitsliced[k] |= (b << index32);
	}
}

static inline void gf256to2_bitslice32_vect_pack_gf256to2_composite(uint16_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	for (uint32_t k = 0; k < 16; k++) {
		uint32_t b = (x >> k) & 0x01;
		x_bitsliced[k] |= (b << index32);
	}
}

static inline uint16_t gf256to2_bitslice32_vect_unpack_gf256to2_composite(const uint32_t x_bitsliced[16], uint32_t index32) {
	uint16_t x = 0;
	for (uint32_t k = 0; k < 16; k++) {
		uint16_t b = (x_bitsliced[k] >> index32) & 0x01;
		x |= (b << k);
	}
	return x;
}

static inline void gf256to2_bitslice32_add_composite(uint32_t z_bitsliced[16], uint32_t x_bitsliced[16], uint32_t y_bitsliced[16]) {
	for (uint32_t k = 0; k < 16; k++) {
		z_bitsliced[k] = x_bitsliced[k] ^ y_bitsliced[k];
	}
}

static inline void gf256to2_bitslice32_mult_composite(uint32_t res[16], const uint32_t x[16], const uint32_t y[16]) {
	uint32_t x1y1[8], x0y0[8], x0_x1[8];
	gf256_bitslice32_mult_composite(x1y1, &x[8], &y[8]);
	gf256_bitslice32_mult_composite(x0y0, &x[0], &y[0]);

	gf256_bitslice32_mult_hybrid_composite(&res[0], 32, x1y1);
	gf256_bitslice32_add_composite(&res[0], &res[0], x0y0);

	gf256_bitslice32_add_composite(x0_x1, &x[0], &x[8]);
	gf256_bitslice32_add_composite(x1y1, &y[0], &y[8]); // We reuse x1y1
	gf256_bitslice32_mult_composite(&res[8], x0_x1, x1y1);
	gf256_bitslice32_add_composite(&res[8], &res[8], x0y0);
}

static inline void gf256to2_bitslice32_mult_hybrid_composite(uint32_t res[16], uint16_t x, const uint32_t y[16]) {
	uint8_t x1, x0;
	x0 = x & 0xff;
	x1 = x >> 8;

	uint32_t x1y1[8], x0y0[8];
	gf256_bitslice32_mult_hybrid_composite(x1y1, x1, &y[8]);
	gf256_bitslice32_mult_hybrid_composite(x0y0, x0, &y[0]);

	gf256_bitslice32_mult_hybrid_composite(&res[0], 32, x1y1);
	gf256_bitslice32_add_composite(&res[0], &res[0], x0y0);

	gf256_bitslice32_add_composite(x1y1, &y[0], &y[8]); // We reuse x1y1
	gf256_bitslice32_mult_hybrid_composite(&res[8], x0 ^ x1, x1y1);
	gf256_bitslice32_add_composite(&res[8], &res[8], x0y0);
}

static inline void gf256to2_bitslice32_mult_hybrid_gf2_composite(uint32_t res[16], uint8_t x, const uint32_t y[16]) {
	gf256to2_bitslice32_mult_hybrid_composite(res, (uint16_t) x, y);
}

static inline void gf256to2_bitslice32_mult_hybrid_gf16_composite(uint32_t res[16], uint8_t x, const uint32_t y[16]) {
	x = (x & 1) ^ (-((x >> 1) & 1) & 0xE0) ^ (-((x >> 2) & 1) & 0x5D) ^ (-(x >> 3) & 0xB0);
	gf256to2_bitslice32_mult_hybrid_composite(res, (uint16_t) x, y);
}

static inline void gf256to2_bitslice32_mult_hybrid_gf256_composite(uint32_t res[16], uint8_t x, const uint32_t y[16]) {
	gf256to2_bitslice32_mult_hybrid_composite(res, (uint16_t) x, y);
}

#if defined(FIELDS_BITSLICE_PUBLIC_JUMP)
static inline void gf256to2_bitslice32_mult_hybrid_public_composite(uint32_t res[16], uint16_t x, const uint32_t y[16]) {
	uint8_t x1, x0;
	x0 = x & 0xff;
	x1 = x >> 8;

	uint32_t x1y1[8], x0y0[8];
	gf256_bitslice32_mult_hybrid_public_composite(x1y1, x1, &y[8]);
	gf256_bitslice32_mult_hybrid_public_composite(x0y0, x0, &y[0]);

	gf256_bitslice32_mult_hybrid_public_composite(&res[0], 32, x1y1);
	gf256_bitslice32_add_composite(&res[0], &res[0], x0y0);

	gf256_bitslice32_add_composite(x1y1, &y[0], &y[8]); // We reuse x1y1
	gf256_bitslice32_mult_hybrid_public_composite(&res[8], x0 ^ x1, x1y1);
	gf256_bitslice32_add_composite(&res[8], &res[8], x0y0);
}
#else
static inline void gf256to2_bitslice32_mult_hybrid_public_composite(uint32_t res[16], uint16_t x, const uint32_t y[16]) {
	gf256to2_bitslice32_mult_hybrid_composite(res, x, y);
}
#endif

#endif /* __FIELDS_BITSLICED_BRANCHCONST_H__ */

