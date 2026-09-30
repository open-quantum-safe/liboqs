// p500_27: p = 27 * 2^500 - 1 (505 bits, 8 limbs, 7 spare bits)
#include <fp.h>

const digit_t p[NWORDS_FIELD] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x01afffffffffffff };
const digit_t p2[NWORDS_FIELD] = { 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                   0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x035fffffffffffff };
