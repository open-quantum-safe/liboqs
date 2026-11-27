// p664_17: p = 17 * 2^664 - 1 (669 bits, 11 limbs, 35 spare bits)
#include <fp.h>

const digit_t p[NWORDS_FIELD] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                  0xffffffffffffffff, 0xffffffffffffffff, 0x0000000010ffffff };
const digit_t p2[NWORDS_FIELD] = { 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                   0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                   0xffffffffffffffff, 0xffffffffffffffff, 0x0000000021ffffff };
