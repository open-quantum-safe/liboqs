#include <oqs/rand.h>

#if defined(OQS_RAND_DEFAULT_URANDOM_CHACHA20)
#include <oqs/rand_urandom_chacha20.h>
#else
#error "No default RAND method defined."
#endif

OQS_RAND *OQS_RAND_new() {
#if defined(OQS_RAND_DEFAULT_URANDOM_CHACHA20)
	return OQS_RAND_urandom_chacha20_new();
#else
#error "No default RAND method defined."
#endif
}

uint8_t OQS_RAND_8(OQS_RAND *r) {
	return r->rand_8(r);
}

uint32_t OQS_RAND_32(OQS_RAND *r) {
	return r->rand_32(r);
}

uint64_t OQS_RAND_64(OQS_RAND *r) {
	return r->rand_64(r);
}

void OQS_RAND_n(OQS_RAND *r, uint8_t *out, size_t n) {
	r->rand_n(r, out, n);
}

void OQS_RAND_free(OQS_RAND *r) {
	if (r) {
		r->free(r);
	}
}

