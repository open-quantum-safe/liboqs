#ifndef modq_h
#define modq_h

#include "crypto_int16.h"
#include "crypto_int32.h"
#include "crypto_uint16.h"
#include "crypto_uint32.h"

typedef crypto_int16 modq;

/* -1 if x is nonzero, 0 otherwise */
static inline int modq_nonzero_mask(modq x) {
	crypto_int32 r = (crypto_uint16) x;
	r = -r;
	r >>= 30;
	return r;
}

/* input between -9000000 and 9000000 */
/* output between -2295 and 2295 */
static inline modq modq_freeze(crypto_int32 a) {
	a -= 4591 * ((228 * a) >> 20);
	a -= 4591 * ((58470 * a + 134217728) >> 28);
	return a;
}

/* input between 0 and 4294967295 */
/* output = (input % 4591) - 2295 */
static inline modq modq_fromuint32(crypto_uint32 a) {
	crypto_int32 r;
	r = (a & 524287) + (a >> 19) * 914; /* <= 8010861 */
	return modq_freeze(r - 2295);
}

static inline modq modq_minusproduct(modq a, modq b, modq c) {
	crypto_int32 A = a;
	crypto_int32 B = b;
	crypto_int32 C = c;
	return modq_freeze(A - B * C);
}

static inline modq modq_plusproduct(modq a, modq b, modq c) {
	crypto_int32 A = a;
	crypto_int32 B = b;
	crypto_int32 C = c;
	return modq_freeze(A + B * C);
}

static inline modq modq_product(modq a, modq b) {
	crypto_int32 A = a;
	crypto_int32 B = b;
	return modq_freeze(A * B);
}

static inline modq modq_square(modq a) {
	crypto_int32 A = a;
	return modq_freeze(A * A);
}

static inline modq modq_sum(modq a, modq b) {
	crypto_int32 A = a;
	crypto_int32 B = b;
	return modq_freeze(A + B);
}

static inline modq modq_reciprocal(modq a1) {
	modq a2 = modq_square(a1);
	modq a3 = modq_product(a2, a1);
	modq a4 = modq_square(a2);
	modq a8 = modq_square(a4);
	modq a16 = modq_square(a8);
	modq a32 = modq_square(a16);
	modq a35 = modq_product(a32, a3);
	modq a70 = modq_square(a35);
	modq a140 = modq_square(a70);
	modq a143 = modq_product(a140, a3);
	modq a286 = modq_square(a143);
	modq a572 = modq_square(a286);
	modq a1144 = modq_square(a572);
	modq a1147 = modq_product(a1144, a3);
	modq a2294 = modq_square(a1147);
	modq a4588 = modq_square(a2294);
	modq a4589 = modq_product(a4588, a1);
	return a4589;
}

static inline modq modq_quotient(modq num, modq den) {
	return modq_product(num, modq_reciprocal(den));
}

#endif
