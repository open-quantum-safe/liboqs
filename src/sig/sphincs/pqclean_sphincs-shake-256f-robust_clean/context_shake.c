#include "context.h"

/* For SHAKE256, there is no immediate reason to initialize at the start,
   so this function is an empty operation. */
void initialize_hash_function(spx_ctx *ctx) {
    (void)ctx; /* Suppress an 'unused parameter' warning. */
}

// in case the hash function api is heap-based.
void free_hash_function(spx_ctx *ctx) {
    (void)ctx;
}
