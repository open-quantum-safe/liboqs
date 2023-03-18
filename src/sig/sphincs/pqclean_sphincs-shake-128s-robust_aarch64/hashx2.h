#ifndef SPX_HASHX2_H
#define SPX_HASHX2_H

#include "context.h"
#include "params.h"
#include <stdint.h>

#define prf_addrx2 SPX_NAMESPACE(prf_addrx2)
void prf_addrx2(unsigned char *out0,
                unsigned char *out1,
                const spx_ctx *ctx,
                const uint32_t addrx2[2 * 8]);

#endif
