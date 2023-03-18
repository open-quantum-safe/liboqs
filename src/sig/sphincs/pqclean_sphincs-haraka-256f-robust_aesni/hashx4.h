#ifndef SPX_HASHX4_H
#define SPX_HASHX4_H

#include <stdint.h>

#include "context.h"
#include "params.h"

#define prf_addrx4 SPX_NAMESPACE(prf_addrx4)
void prf_addrx4(unsigned char *out0,
                unsigned char *out1,
                unsigned char *out2,
                unsigned char *out3,
                const spx_ctx *ctx,
                const uint32_t addrx4[4 * 8]);

#endif
