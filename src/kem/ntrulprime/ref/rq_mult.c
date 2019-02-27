#include "params.h"
#include "rq.h"

void rq_mult(modq *h, const modq *f, const small *g) {
	modq fg[p + p - 1];
	modq result;
	int i, j;

	for (i = 0; i < p; ++i) {
		result = 0;
		for (j = 0; j <= i; ++j)
			result = modq_plusproduct(result, f[j], g[i - j]);
		fg[i] = result;
	}
	for (i = p; i < p + p - 1; ++i) {
		result = 0;
		for (j = i - p + 1; j < p; ++j)
			result = modq_plusproduct(result, f[j], g[i - j]);
		fg[i] = result;
	}

	for (i = p + p - 2; i >= p; --i) {
		fg[i - p] = modq_sum(fg[i - p], fg[i]);
		fg[i - p + 1] = modq_sum(fg[i - p + 1], fg[i]);
	}

	for (i = 0; i < p; ++i)
		h[i] = fg[i];
}
