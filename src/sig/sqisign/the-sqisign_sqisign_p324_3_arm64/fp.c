// p324_3: p = 3 * 2^324 - 1 (326 bits, 6 limbs, 58 spare bits)
#include <fp.h>

const digit_t p[NWORDS_FIELD] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                  0xffffffffffffffff, 0xffffffffffffffff, 0x000000000000002f };
const digit_t p2[NWORDS_FIELD] = { 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff,
                                   0xffffffffffffffff, 0xffffffffffffffff, 0x000000000000005f };
