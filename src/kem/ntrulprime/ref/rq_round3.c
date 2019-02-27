#include "params.h"
#include "rq.h"

void rq_round3(modq *h, const modq *f) {
	int i;

	for (i = 0; i < p; ++i)
		h[i] = ((21846 * (f[i] + 2295) + 32768) >> 16) * 3 - 2295;
}
