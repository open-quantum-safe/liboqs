#include "params.h"
#include "crypto_uint32.h"
#include "rq.h"

void rq_encoderounded(unsigned char *c, const modq *f) {
	crypto_int32 f0, f1, f2;
	int i;

	for (i = 0; i < p / 3; ++i) {
		f0 = *f++ + qshift;
		f1 = *f++ + qshift;
		f2 = *f++ + qshift;
		f0 = (21846 * f0) >> 16;
		f1 = (21846 * f1) >> 16;
		f2 = (21846 * f2) >> 16;
		/* now want f0 + f1*1536 + f2*1536^2 as a 32-bit integer */
		f2 *= 3;
		f1 += f2 << 9;
		f1 *= 3;
		f0 += f1 << 9;
		*c++ = f0;
		f0 >>= 8;
		*c++ = f0;
		f0 >>= 8;
		*c++ = f0;
		f0 >>= 8;
		*c++ = f0;
	}
	/* XXX: using p mod 3 = 2 */
	f0 = *f++ + qshift;
	f1 = *f++ + qshift;
	f0 = (21846 * f0) >> 16;
	f1 = (21846 * f1) >> 16;
	f1 *= 3;
	f0 += f1 << 9;
	*c++ = f0;
	f0 >>= 8;
	*c++ = f0;
	f0 >>= 8;
	*c++ = f0;
}

void rq_decoderounded(modq *f, const unsigned char *c) {
	crypto_uint32 c0, c1, c2, c3;
	crypto_uint32 f0, f1, f2;
	int i;

	for (i = 0; i < p / 3; ++i) {
		c0 = *c++;
		c1 = *c++;
		c2 = *c++;
		c3 = *c++;

		/* f0 + f1*1536 + f2*1536^2 */
		/* = c0 + c1*256 + c2*256^2 + c3*256^3 */
		/* with each f between 0 and 1530 */

		/* f2 = (64/9)c3 + (1/36)c2 + (1/9216)c1 + (1/2359296)c0 - [0,0.99675] */
		/* claim: 2^21 f2 < x < 2^21(f2+1) */
		/* where x = 14913081*c3 + 58254*c2 + 228*(c1+2) */
		/* proof: x - 2^21 f2 = 456 - (8/9)c0 + (4/9)c1 - (2/9)c2 + (1/9)c3 + 2^21 [0,0.99675] */
		/* at least 456 - (8/9)255 - (2/9)255 > 0 */
		/* at most 456 + (4/9)255 + (1/9)255 + 2^21 0.99675 < 2^21 */
		f2 = (14913081 * c3 + 58254 * c2 + 228 * (c1 + 2)) >> 21;

		c2 += c3 << 8;
		c2 -= (f2 * 9) << 2;
		/* f0 + f1*1536 */
		/* = c0 + c1*256 + c2*256^2 */
		/* c2 <= 35 = floor((1530+1530*1536)/256^2) */
		/* f1 = (128/3)c2 + (1/6)c1 + (1/1536)c0 - (1/1536)f0 */
		/* claim: 2^21 f1 < x < 2^21(f1+1) */
		/* where x = 89478485*c2 + 349525*c1 + 1365*(c0+1) */
		/* proof: x - 2^21 f1 = 1365 - (1/3)c2 - (1/3)c1 - (1/3)c0 + (4096/3)f0 */
		/* at least 1365 - (1/3)35 - (1/3)255 - (1/3)255 > 0 */
		/* at most 1365 + (4096/3)1530 < 2^21 */
		f1 = (89478485 * c2 + 349525 * c1 + 1365 * (c0 + 1)) >> 21;

		c1 += c2 << 8;
		c1 -= (f1 * 3) << 1;

		c0 += c1 << 8;
		f0 = c0;

		*f++ = modq_freeze(f0 * 3 + q - qshift);
		*f++ = modq_freeze(f1 * 3 + q - qshift);
		*f++ = modq_freeze(f2 * 3 + q - qshift);
	}

	c0 = *c++;
	c1 = *c++;
	c2 = *c++;

	f1 = (89478485 * c2 + 349525 * c1 + 1365 * (c0 + 1)) >> 21;

	c1 += c2 << 8;
	c1 -= (f1 * 3) << 1;

	c0 += c1 << 8;
	f0 = c0;

	*f++ = modq_freeze(f0 * 3 + q - qshift);
	*f++ = modq_freeze(f1 * 3 + q - qshift);
}
