#include "crypto_stream_aes256ctr.h"
#include "rq.h"
#include "params.h"

static const unsigned char n[16] = {0};

void rq_fromseed(modq *h, const unsigned char *K) {
	crypto_uint32 buf[p];
	int i;

	crypto_stream_aes256ctr((unsigned char *) buf, sizeof buf, n, K);
	for (i = 0; i < p; ++i)
		h[i] = modq_fromuint32(buf[i]);
}
