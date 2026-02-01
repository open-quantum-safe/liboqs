#ifndef __FIELDS_BITSLICED_BRANCHCONST_H__
#define __FIELDS_BITSLICED_BRANCHCONST_H__

#include "fields_handling.h"

static inline void gf256_bitslice32_vect_pack_post_default(uint32_t x_bitsliced[8]) {
	(void) x_bitsliced;
}

static inline void gf256_bitslice32_vect_unpack_pre_default(uint32_t x_bitsliced[8]) {
	(void) x_bitsliced;
}

static inline void gf256_bitslice32_vect_pack_gf2_default(uint8_t x, uint32_t x_bitsliced[8], uint32_t index32) {
	x_bitsliced[0] |= (x << index32);
}

static inline void gf256_bitslice32_vect_pack_gf16_default(uint8_t x, uint32_t x_bitsliced[8], uint32_t index32) {
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

static inline void gf256_bitslice32_vect_pack_gf256_default(uint8_t x, uint32_t x_bitsliced[8], uint32_t index32) {
	for (uint32_t k = 0; k < 8; k++) {
		uint32_t b = (x >> k) & 0x01;
		x_bitsliced[k] |= (b << index32);
	}
}

static inline uint8_t gf256_bitslice32_vect_unpack_gf256_default(const uint32_t x_bitsliced[8], uint32_t index32) {
	uint8_t x = 0;
	for (uint32_t k = 0; k < 8; k++) {
		uint8_t b = (x_bitsliced[k] >> index32) & 0x01;
		x |= (b << k);
	}
	return x;
}

static inline void gf256_bitslice32_add_default(uint32_t z_bitsliced[8], const uint32_t x_bitsliced[8], const uint32_t y_bitsliced[8]) {
	for (uint32_t k = 0; k < 8; k++) {
		z_bitsliced[k] = x_bitsliced[k] ^ y_bitsliced[k];
	}
}

static inline void gf256_bitslice32_mult_default(uint32_t res[8], const uint32_t x[8], const uint32_t y[8]) {
	uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0;

#define STEP(BIT) do { \
        uint32_t m = x[BIT]; \
        uint32_t lead = r7; \
        uint32_t t7 = (m & y7) ^ r6; \
        uint32_t t6 = (m & y6) ^ r5; \
        uint32_t t5 = (m & y5) ^ r4; \
        uint32_t t4 = (m & y4) ^ lead ^ r3; \
        uint32_t t3 = (m & y3) ^ lead ^ r2; \
        uint32_t t2 = (m & y2) ^ r1; \
        uint32_t t1 = (m & y1) ^ lead ^ r0; \
        uint32_t t0 = (m & y0) ^ lead; \
        r7=t7; r6=t6; r5=t5; r4=t4; r3=t3; r2=t2; r1=t1; r0=t0; \
    } while(0)

	STEP(7);
	STEP(6);
	STEP(5);
	STEP(4);
	STEP(3);
	STEP(2);
	STEP(1);
	STEP(0);

	res[0] = r0;
	res[1] = r1;
	res[2] = r2;
	res[3] = r3;
	res[4] = r4;
	res[5] = r5;
	res[6] = r6;
	res[7] = r7;
#undef STEP
}

static inline void gf256_bitslice32_mult_hybrid_default(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0;

#define STEP(BIT) do { \
        uint32_t m = 0u - ((x >> (BIT)) & 1u); /* 0x00000000 ou 0xFFFFFFFF */ \
        uint32_t lead = r7; \
        uint32_t t7 = (m & y7) ^ r6; \
        uint32_t t6 = (m & y6) ^ r5; \
        uint32_t t5 = (m & y5) ^ r4; \
        uint32_t t4 = (m & y4) ^ lead ^ r3; \
        uint32_t t3 = (m & y3) ^ lead ^ r2; \
        uint32_t t2 = (m & y2) ^ r1; \
        uint32_t t1 = (m & y1) ^ lead ^ r0; \
        uint32_t t0 = (m & y0) ^ lead; \
        r7=t7; r6=t6; r5=t5; r4=t4; r3=t3; r2=t2; r1=t1; r0=t0; \
    } while(0)

	STEP(7);
	STEP(6);
	STEP(5);
	STEP(4);
	STEP(3);
	STEP(2);
	STEP(1);
	STEP(0);

	res[0] = r0;
	res[1] = r1;
	res[2] = r2;
	res[3] = r3;
	res[4] = r4;
	res[5] = r5;
	res[6] = r6;
	res[7] = r7;
#undef STEP
}

static inline void gf256_bitslice32_mult_hybrid_gf2_default(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	gf256_bitslice32_mult_hybrid_default(res, x, y);
}

static inline void gf256_bitslice32_mult_hybrid_gf16_default(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	x = (x & 1) ^ (-((x >> 1) & 1) & 0xE0) ^ (-((x >> 2) & 1) & 0x5D) ^ (-(x >> 3) & 0xB0);
	gf256_bitslice32_mult_hybrid_default(res, x, y);
}

static inline void gf256_bitslice32_mult_hybrid_gf256_default(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	gf256_bitslice32_mult_hybrid_default(res, x, y);
}

#if defined(FIELDS_BITSLICE_PUBLIC_JUMP)
static inline void gf256_bitslice32_mult_hybrid_public_default(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	uint32_t y0 = y[0], y1 = y[1], y2 = y[2], y3 = y[3];
	uint32_t y4 = y[4], y5 = y[5], y6 = y[6], y7 = y[7];

	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0;

#define STEP(BIT) do { \
        if((x >> (BIT)) & 1u) { \
            uint32_t lead = r7; \
            uint32_t t7 = y7 ^ r6; \
            uint32_t t6 = y6 ^ r5; \
            uint32_t t5 = y5 ^ r4; \
            uint32_t t4 = y4 ^ lead ^ r3; \
            uint32_t t3 = y3 ^ lead ^ r2; \
            uint32_t t2 = y2 ^ r1; \
            uint32_t t1 = y1 ^ lead ^ r0; \
            uint32_t t0 = y0 ^ lead; \
            r7=t7; r6=t6; r5=t5; r4=t4; r3=t3; r2=t2; r1=t1; r0=t0; \
        } else { \
            uint32_t lead = r7; \
            uint32_t t7 = r6; \
            uint32_t t6 = r5; \
            uint32_t t5 = r4; \
            uint32_t t4 = lead ^ r3; \
            uint32_t t3 = lead ^ r2; \
            uint32_t t2 = r1; \
            uint32_t t1 = lead ^ r0; \
            uint32_t t0 = lead; \
            r7=t7; r6=t6; r5=t5; r4=t4; r3=t3; r2=t2; r1=t1; r0=t0; \
        } \
    } while(0)

	STEP(7);
	STEP(6);
	STEP(5);
	STEP(4);
	STEP(3);
	STEP(2);
	STEP(1);
	STEP(0);

	res[0] = r0;
	res[1] = r1;
	res[2] = r2;
	res[3] = r3;
	res[4] = r4;
	res[5] = r5;
	res[6] = r6;
	res[7] = r7;
#undef STEP
}
#else
static inline void gf256_bitslice32_mult_hybrid_public_default(uint32_t res[8], uint8_t x, const uint32_t y[8]) {
	gf256_bitslice32_mult_hybrid_default(res, x, y);
}

#endif

static inline void gf256to2_bitslice32_vect_pack_post_default(uint32_t x_bitsliced[16]) {
	(void) x_bitsliced;
}

static inline void gf256to2_bitslice32_vect_unpack_pre_default(uint32_t x_bitsliced[16]) {
	(void) x_bitsliced;
}

static inline void gf256to2_bitslice32_vect_pack_gf2_default(uint8_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	x_bitsliced[0] |= (x << index32);
}

static inline void gf256to2_bitslice32_vect_pack_gf16_default(uint8_t x, uint32_t x_bitsliced[16], uint32_t index32) {
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

static inline void gf256to2_bitslice32_vect_pack_gf256_default(uint8_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	for (uint32_t k = 0; k < 8; k++) {
		uint32_t b = (x >> k) & 0x01;
		x_bitsliced[k] |= (b << index32);
	}
}

static inline void gf256to2_bitslice32_vect_pack_gf256to2_default(uint16_t x, uint32_t x_bitsliced[16], uint32_t index32) {
	for (uint32_t k = 0; k < 16; k++) {
		uint32_t b = (x >> k) & 0x01;
		x_bitsliced[k] |= (b << index32);
	}
}

static inline uint16_t gf256to2_bitslice32_vect_unpack_gf256to2_default(const uint32_t x_bitsliced[16], uint32_t index32) {
	uint16_t x = 0;
	for (uint32_t k = 0; k < 16; k++) {
		uint16_t b = (x_bitsliced[k] >> index32) & 0x01;
		x |= (b << k);
	}
	return x;
}

static inline void gf256to2_bitslice32_add_default(uint32_t z_bitsliced[16], uint32_t x_bitsliced[16], uint32_t y_bitsliced[16]) {
	for (uint32_t k = 0; k < 16; k++) {
		z_bitsliced[k] = x_bitsliced[k] ^ y_bitsliced[k];
	}
}

static inline void gf256to2_bitslice32_mult_default(uint32_t res[16], const uint32_t x[16], const uint32_t y[16]) {
	uint32_t x1y1[8], x0y0[8], x0_x1[8];
	gf256_bitslice32_mult_default(x1y1, &x[8], &y[8]);
	gf256_bitslice32_mult_default(x0y0, &x[0], &y[0]);

	gf256_bitslice32_mult_hybrid_default(&res[0], 32, x1y1);
	gf256_bitslice32_add_default(&res[0], &res[0], x0y0);

	gf256_bitslice32_add_default(x0_x1, &x[0], &x[8]);
	gf256_bitslice32_add_default(x1y1, &y[0], &y[8]); // We reuse x1y1
	gf256_bitslice32_mult_default(&res[8], x0_x1, x1y1);
	gf256_bitslice32_add_default(&res[8], &res[8], x0y0);
}

static inline void gf256to2_bitslice32_mult_hybrid_default(uint32_t res[16], uint16_t x, const uint32_t y[16]) {
	uint8_t x1, x0;
	x0 = x & 0xff;
	x1 = x >> 8;

	uint32_t x1y1[8], x0y0[8];
	gf256_bitslice32_mult_hybrid_default(x1y1, x1, &y[8]);
	gf256_bitslice32_mult_hybrid_default(x0y0, x0, &y[0]);

	gf256_bitslice32_mult_hybrid_default(&res[0], 32, x1y1);
	gf256_bitslice32_add_default(&res[0], &res[0], x0y0);

	gf256_bitslice32_add_default(x1y1, &y[0], &y[8]); // We reuse x1y1
	gf256_bitslice32_mult_hybrid_default(&res[8], x0 ^ x1, x1y1);
	gf256_bitslice32_add_default(&res[8], &res[8], x0y0);
}

static inline void gf256to2_bitslice32_mult_hybrid_gf2_default(uint32_t res[16], uint8_t x, const uint32_t y[16]) {
	gf256to2_bitslice32_mult_hybrid_default(res, (uint16_t) x, y);
}

static inline void gf256to2_bitslice32_mult_hybrid_gf16_default(uint32_t res[16], uint8_t x, const uint32_t y[16]) {
	x = (x & 1) ^ (-((x >> 1) & 1) & 0xE0) ^ (-((x >> 2) & 1) & 0x5D) ^ (-(x >> 3) & 0xB0);
	gf256to2_bitslice32_mult_hybrid_default(res, (uint16_t) x, y);
}

static inline void gf256to2_bitslice32_mult_hybrid_gf256_default(uint32_t res[16], uint8_t x, const uint32_t y[16]) {
	gf256to2_bitslice32_mult_hybrid_default(res, (uint16_t) x, y);
}

#if defined(FIELDS_BITSLICE_PUBLIC_JUMP)
static inline void gf256to2_bitslice32_mult_hybrid_public_default(uint32_t res[16], uint16_t x, const uint32_t y[16]) {
	uint8_t x1, x0;
	x0 = x & 0xff;
	x1 = x >> 8;

	uint32_t x1y1[8], x0y0[8];
	gf256_bitslice32_mult_hybrid_public_default(x1y1, x1, &y[8]);
	gf256_bitslice32_mult_hybrid_public_default(x0y0, x0, &y[0]);

	gf256_bitslice32_mult_hybrid_public_default(&res[0], 32, x1y1);
	gf256_bitslice32_add_default(&res[0], &res[0], x0y0);

	gf256_bitslice32_add_default(x1y1, &y[0], &y[8]); // We reuse x1y1
	gf256_bitslice32_mult_hybrid_public_default(&res[8], x0 ^ x1, x1y1);
	gf256_bitslice32_add_default(&res[8], &res[8], x0y0);
}
#else
static inline void gf256to2_bitslice32_mult_hybrid_public_default(uint32_t res[16], uint16_t x, const uint32_t y[16]) {
	gf256to2_bitslice32_mult_hybrid_default(res, x, y);
}
#endif

#endif /* __FIELDS_BITSLICED_BRANCHCONST_H__ */

