#include "params.h"
#include "small.h"

/* XXX: these functions rely on p mod 4 = 1 */

/* all coefficients in -1, 0, 1 */
void small_encode(unsigned char *c, const small *f) {
	small c0;
	int i;

	for (i = 0; i < p / 4; ++i) {
		c0 = *f++ + 1;
		c0 += (*f++ + 1) << 2;
		c0 += (*f++ + 1) << 4;
		c0 += (*f++ + 1) << 6;
		*c++ = c0;
	}
	c0 = *f++ + 1;
	*c++ = c0;
}

void small_decode(small *f, const unsigned char *c) {
	unsigned char c0;
	int i;

	for (i = 0; i < p / 4; ++i) {
		c0 = *c++;
		*f++ = ((small)(c0 & 3)) - 1;
		c0 >>= 2;
		*f++ = ((small)(c0 & 3)) - 1;
		c0 >>= 2;
		*f++ = ((small)(c0 & 3)) - 1;
		c0 >>= 2;
		*f++ = ((small)(c0 & 3)) - 1;
	}
	c0 = *c++;
	*f++ = ((small)(c0 & 3)) - 1;
}
