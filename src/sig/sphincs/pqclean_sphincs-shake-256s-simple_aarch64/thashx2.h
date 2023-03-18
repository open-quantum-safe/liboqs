#ifndef SPX_THASHX2_H
#define SPX_THASHX2_H

#include "context.h"
#include "params.h"
#include <stdint.h>

#define thashx2 SPX_NAMESPACE(thashx2)
void thashx2(unsigned char *out0,
             unsigned char *out1,
             const unsigned char *in0,
             const unsigned char *in1,
             unsigned int inblocks,
             const spx_ctx *ctx, uint32_t addrx2[2 * 8]);

#endif
