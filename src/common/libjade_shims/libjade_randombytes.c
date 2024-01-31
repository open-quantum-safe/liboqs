#include <oqs/rand.h>
#include "libjade_randombytes.h"

// make return type uint8_t*
void __jasmin_syscall_randombytes__(uint8_t* x, uint64_t xlen) {
    OQS_randombytes(x, xlen);
}
