#include "context.h"
#include "haraka.h"

void initialize_hash_function(spx_ctx *ctx) {
    tweak_constants(ctx);
}

// we don't support heap-based haraka right now
void free_hash_function(spx_ctx *ctx) {
    (void)ctx; // suppress unused variable warnings
}
