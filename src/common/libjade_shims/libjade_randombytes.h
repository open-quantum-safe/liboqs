#ifndef LIBJADE_RANDOMBYTES_H
#define LIBJADE_RANDOMBYTES_H

#include <stdint.h>

void __jasmin_syscall_randombytes__(uint8_t* _x, uint64_t xlen) __asm__("__jasmin_syscall_randombytes__");

#endif // LIBJADE_RANDOMBYTES_H
