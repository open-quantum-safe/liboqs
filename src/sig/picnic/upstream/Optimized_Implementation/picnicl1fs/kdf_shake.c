
#include "kdf_shake.h"

void hash_init(hash_context* ctx, const picnic_instance_t* pp) {
  if (pp->security_level == 64) {
    Keccak_HashInitialize_SHAKE128(ctx);
  } else {
    Keccak_HashInitialize_SHAKE256(ctx);
  }
}
