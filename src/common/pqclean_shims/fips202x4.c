//  SPDX-License-Identifier: MIT

#include "fips202x4.h"

void shake128x4_absorb_once(shake128x4incctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	shake128x4_inc_ctx_reset(state);
	shake128x4_inc_absorb(state, in0, in1, in2, in3, inlen);
	shake128x4_inc_finalize(state);
}

void shake256x4_absorb_once(shake256x4incctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	shake256x4_inc_ctx_reset(state);
	shake256x4_inc_absorb(state, in0, in1, in2, in3, inlen);
	shake256x4_inc_finalize(state);
}
