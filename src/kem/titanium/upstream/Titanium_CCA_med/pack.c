/* ****************************** *
 * Titanium_CCA_med               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Packing/Unpacking              *
 * ****************************** */

#include "pack.h"
#include "param.h"
#include "littleendian.h"
#include <stdint.h>

static inline uint32_t con_sub(uint32_t x) /* conditional subtraction of q */
{
	return x - (1 ^ ((x - Q) >> 31)) * Q;
}

/* convert a polynomial to a binary string */
void poly_encode(unsigned char *b, const uint32_t *p, uint32_t len)
{
	uint32_t i;
	unsigned char *bb;
	uint32_t pp[8];
	
	/* pack 8 19-bit coordinates to 19 bytes */
	for (i = 0; i < len; i += 8)
	{
		/* make sure each coordinate is smaller than Q */
		pp[0] = con_sub(p[i]);
		pp[1] = con_sub(p[i + 1]);
		pp[2] = con_sub(p[i + 2]);
		pp[3] = con_sub(p[i + 3]);
		pp[4] = con_sub(p[i + 4]);
		pp[5] = con_sub(p[i + 5]);
		pp[6] = con_sub(p[i + 6]);
		pp[7] = con_sub(p[i + 7]);
		
		bb = b + (i / 8) * Q_BITS;
		bb[0] = pp[0];
		bb[1] = pp[0] >> 8;
		bb[2] = (pp[0] >> 16) | ((pp[1] & 0x1f) << 3);
		bb[3] = pp[1] >> 5;
		bb[4] = (pp[1] >> 13) | ((pp[2] & 0x03) << 6);
		bb[5] = pp[2] >> 2;
		bb[6] = pp[2] >> 10;
		bb[7] = (pp[2] >> 18) | ((pp[3] & 0x7f) << 1);
		bb[8] = pp[3] >> 7;
		bb[9] = (pp[3] >> 15) | ((pp[4] & 0x0f) << 4);
		bb[10] = pp[4] >> 4;
		bb[11] = (pp[4] >> 12) | ((pp[5] & 0x01) << 7);
		bb[12] = pp[5] >> 1;
		bb[13] = pp[5] >> 9;
		bb[14] = (pp[5] >> 17) | ((pp[6] & 0x3f) << 2);
		bb[15] = pp[6] >> 6;
		bb[16] = (pp[6] >> 14) | ((pp[7] & 0x07) << 5);
		bb[17] = pp[7] >> 3;
		bb[18] = pp[7] >> 11;
	}
}

/* convert a binary string to a polynomial */
void poly_decode(uint32_t *p, const unsigned char *b, uint32_t len)
{
	uint32_t i;
	unsigned char *bb;
	
	/* unpack 19 bytes to 8 19-bit coordinates */
	for (i = 0; i < len; i += 8)
	{
		bb = b + (i / 8) * Q_BITS;

		p[i] = ((uint32_t)bb[0]) | (((uint32_t)bb[1]) << 8) | ((((uint32_t)bb[2]) & 0x07) << 16);
		p[i + 1] = (((uint32_t)bb[2]) >> 3) | (((uint32_t)bb[3]) << 5) | ((((uint32_t)bb[4]) & 0x3f) << 13);
		p[i + 2] = (((uint32_t)bb[4]) >> 6) | (((uint32_t)bb[5]) << 2) | (((uint32_t)bb[6]) << 10) | ((((uint32_t)bb[7]) & 0x01) << 18);
		p[i + 3] = (((uint32_t)bb[7]) >> 1) | (((uint32_t)bb[8]) << 7) | ((((uint32_t)bb[9]) & 0x0f) << 15);
		p[i + 4] = (((uint32_t)bb[9]) >> 4) | (((uint32_t)bb[10]) << 4) | ((((uint32_t)bb[11]) & 0x7f) << 12);
		p[i + 5] = (((uint32_t)bb[11]) >> 7) | (((uint32_t)bb[12]) << 1) | (((uint32_t)bb[13]) << 9) | ((((uint32_t)bb[14]) & 0x03) << 17);
		p[i + 6] = (((uint32_t)bb[14]) >> 2) | (((uint32_t)bb[15]) << 6) | ((((uint32_t)bb[16]) & 0x1f) << 14);
		p[i + 7] = (((uint32_t)bb[16]) >> 5) | (((uint32_t)bb[17]) << 3) | (((uint32_t)bb[18]) << 11);
	}
}

/* convert a polynomial to a binary string (with compression) */
void poly_encode_c2(unsigned char *b, const uint32_t *p, uint32_t len)
{
	uint32_t i;

	for (i = 0; i < len; i++)
	{
		/* each coordinate will become 1 bytes after compression */
		STORE_C2(b + i * C2_COMPRESSION_BYTE, (con_sub(p[i])) >> C2_COMPRESSION_BITS);
	}
}

/* convert a binary string to a polynomial (with compression) */
void poly_decode_c2(uint32_t *p, const unsigned char *b, uint32_t len)
{
	uint32_t i;
	
	for (i = 0; i < len; i++)
	{
		/* shift the compressed coordinates back */
		p[i] = ((uint32_t)(LOAD_C2(b + i * C2_COMPRESSION_BYTE))) << C2_COMPRESSION_BITS;
	}
}

