#ifndef SPX_CONTEXT_H
#define SPX_CONTEXT_H

#include <stdint.h>

#include "immintrin.h"
#include "params.h"

typedef struct {
    uint8_t pub_seed[SPX_N];
    uint8_t sk_seed[SPX_N];

    __m128i rc[40];
} spx_ctx;

#define initialize_hash_function SPX_NAMESPACE(initialize_hash_function)
void initialize_hash_function(spx_ctx *ctx);

#define free_hash_function SPX_NAMESPACE(free_hash_function)
void free_hash_function(spx_ctx *ctx);

#endif
