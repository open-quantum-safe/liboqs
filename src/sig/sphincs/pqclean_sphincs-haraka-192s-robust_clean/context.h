#ifndef SPX_CONTEXT_H
#define SPX_CONTEXT_H

#include <stddef.h>
#include <stdint.h>

#include "params.h"

typedef struct {
    uint8_t pub_seed[SPX_N];
    uint8_t sk_seed[SPX_N];


    uint64_t tweaked512_rc64[10][8];
    uint32_t tweaked256_rc32[10][8];
} spx_ctx;

#define initialize_hash_function SPX_NAMESPACE(initialize_hash_function)
void initialize_hash_function(spx_ctx *ctx);

#define free_hash_function SPX_NAMESPACE(free_hash_function)
void free_hash_function(spx_ctx *ctx);

#endif
