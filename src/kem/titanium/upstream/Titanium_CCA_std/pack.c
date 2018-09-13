/* ****************************** *
 * Titanium_CCA_std               *
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
	
	/* pack 8 17-bit coordinates to 17 bytes */
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
		bb[2] = (pp[0] >> 16) | ((pp[1] & 0x7f) << 1);
		bb[3] = pp[1] >> 7;
		bb[4] = (pp[1] >> 15) | ((pp[2] & 0x3f) << 2);
		bb[5] = pp[2] >> 6;
		bb[6] = (pp[2] >> 14) | ((pp[3] & 0x1f) << 3);
		bb[7] = pp[3] >> 5;
		bb[8] = (pp[3] >> 13) | ((pp[4] & 0x0f) << 4);
		bb[9] = pp[4] >> 4;
		bb[10] = (pp[4] >> 12) | ((pp[5] & 0x07) << 5);
		bb[11] = pp[5] >> 3;
		bb[12] = (pp[5] >> 11) | ((pp[6] & 0x03) << 6);
		bb[13] = pp[6] >> 2;
		bb[14] = (pp[6] >> 10) | ((pp[7] & 0x01) << 7);
		bb[15] = pp[7] >> 1;
		bb[16] = pp[7] >> 9;
	}
}

/* convert a binary string to a polynomial */
void poly_decode(uint32_t *p, const unsigned char *b, uint32_t len)
{
	uint32_t i;
	unsigned char *bb;
	
	/* unpack 17 bytes to 8 17-bit coordinates */
	for (i = 0; i < len; i += 8)
	{
		bb = b + (i / 8) * Q_BITS;
		
		p[i] = ((uint32_t)bb[0]) | (((uint32_t)bb[1]) << 8) | ((((uint32_t)bb[2]) & 0x01) << 16);
		p[i + 1] = (((uint32_t)bb[2]) >> 1) | (((uint32_t)bb[3]) << 7) | ((((uint32_t)bb[4]) & 0x03) << 15);
		p[i + 2] = (((uint32_t)bb[4]) >> 2) | (((uint32_t)bb[5]) << 6) | ((((uint32_t)bb[6]) & 0x07) << 14);
		p[i + 3] = (((uint32_t)bb[6]) >> 3) | (((uint32_t)bb[7]) << 5) | ((((uint32_t)bb[8]) & 0x0f) << 13);
		p[i + 4] = (((uint32_t)bb[8]) >> 4) | (((uint32_t)bb[9]) << 4) | ((((uint32_t)bb[10]) & 0x1f) << 12);
		p[i + 5] = (((uint32_t)bb[10]) >> 5) | (((uint32_t)bb[11]) << 3) | ((((uint32_t)bb[12]) & 0x3f) << 11);
		p[i + 6] = (((uint32_t)bb[12]) >> 6) | (((uint32_t)bb[13]) << 2) | ((((uint32_t)bb[14]) & 0x7f) << 10);
		p[i + 7] = (((uint32_t)bb[14]) >> 7) | (((uint32_t)bb[15]) << 1) | (((uint32_t)bb[16]) << 9);
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

