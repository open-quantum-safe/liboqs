#ifndef WOTSX1_H_
#define WOTSX1_H_

#include <string.h>

#include "context.h"
#include "params.h"

/*
 * This is here to provide an interface to the internal wots_gen_leafx1
 * routine.  While this routine is not referenced in the package outside of
 * wots.c, it is called from the stand-alone benchmark code to characterize
 * the performance
 */
struct leaf_info_x1 {
    unsigned char *wots_sig;
    uint32_t wots_sign_leaf; /* The index of the WOTS we're using to sign */
    uint32_t *wots_steps;
    uint32_t leaf_addr[8];
    uint32_t pk_addr[8];
};

/* Macro to set the leaf_info to something 'benign', that is, it would */
/* run with the same time as it does during the real signing process */
/* Used only by the benchmark code */
#define INITIALIZE_LEAF_INFO_X1(info, addr, step_buffer) { \
        (info).wots_sig = 0;             \
        (info).wots_sign_leaf = ~0;      \
        (info).wots_steps = step_buffer; \
        memcpy( &(info).leaf_addr[0], (addr), 32 ); \
        memcpy( &(info).pk_addr[0], (addr), 32 ); \
    }

#define wots_gen_leafx1 SPX_NAMESPACE(wots_gen_leafx1)
void wots_gen_leafx1(unsigned char *dest,
                     const spx_ctx *ctx,
                     uint32_t leaf_idx, void *v_info);

#endif /* WOTSX1_H_ */
