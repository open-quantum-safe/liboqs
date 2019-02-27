#include "int32_sort.h"
#include "crypto_uint32.h"

static void minmax(crypto_int32 *x, crypto_int32 *y) {
	crypto_uint32 xi = *x;
	crypto_uint32 yi = *y;
	crypto_uint32 xy = xi ^ yi;
	crypto_uint32 c = yi - xi;
	c ^= xy & (c ^ yi);
	c >>= 31;
	c = -c;
	c &= xy;
	*x = xi ^ c;
	*y = yi ^ c;
}

void int32_sort(crypto_int32 *x, int n) {
	int top, p, q, i;

	if (n < 2)
		return;
	top = 1;
	while (top < n - top)
		top += top;

	for (p = top; p > 0; p >>= 1) {
		for (i = 0; i < n - p; ++i)
			if (!(i & p))
				minmax(x + i, x + i + p);
		for (q = top; q > p; q >>= 1)
			for (i = 0; i < n - q; ++i)
				if (!(i & p))
					minmax(x + i + p, x + i + q);
	}
}
