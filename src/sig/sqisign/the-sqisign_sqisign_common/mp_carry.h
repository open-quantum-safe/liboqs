#ifndef MP_CARRY_H
#define MP_CARRY_H

#include <limits.h>
#include <tutil.h>

// Carry-propagating addition and borrow-propagating subtraction on a single digit:
//
//   ADDC(sumOut, carryOut, addend1, addend2, carryIn)              sumOut        = addend1 + addend2 + carryIn
//   ADDC64(sumOut, carryOut, addend1, addend2, carryIn)            same, on uint64_t limbs regardless of RADIX
//   SUBC(differenceOut, borrowOut, minuend, subtrahend, borrowIn)  differenceOut = minuend - subtrahend - borrowIn
//
// carryIn/borrowIn must be 0 or 1, and carryOut/borrowOut are 0 or 1, so they chain straight into the next limb. All
// forms are branchless: these run on secret data in mp_add/mp_sub/mp_mul. Three implementations follow, fastest first;
// the choice depends both on which builtins the compiler has and on the width of digit_t, which is why it lives here
// rather than in mp.h.

#if defined(__has_builtin)
#define MP_HAS_BUILTIN(x) __has_builtin(x)
#else
#define MP_HAS_BUILTIN(x) 0
#endif

// Define MP_CARRY_FORCE_TIER to 1, 2 or 3 to pin the implementation, so tiers no host selects stay testable.
// Not for production use.
#if !defined(MP_CARRY_FORCE_TIER)
#if (UINT_MAX == UINT32_MAX && ULLONG_MAX == UINT64_MAX && MP_HAS_BUILTIN(__builtin_addc) &&                           \
     MP_HAS_BUILTIN(__builtin_subc) && MP_HAS_BUILTIN(__builtin_addcll) && MP_HAS_BUILTIN(__builtin_subcll))
#define MP_CARRY_FORCE_TIER 1
#elif MP_HAS_BUILTIN(__builtin_add_overflow) || (defined(__GNUC__) && __GNUC__ >= 5)
#define MP_CARRY_FORCE_TIER 2
#else
#define MP_CARRY_FORCE_TIER 3
#endif
#endif

#if MP_CARRY_FORCE_TIER == 1

// Tier 1: the dedicated multiprecision carry builtins, supported on clang 3.8+, gcc 14+.

#if RADIX == 64
#define MP_ADDC_DIGIT(a, b, cin, coutp) __builtin_addcll((a), (b), (cin), (coutp))
#define MP_SUBC_DIGIT(a, b, bin, boutp) __builtin_subcll((a), (b), (bin), (boutp))
typedef unsigned long long mp_carry_t;
#elif RADIX == 32
#define MP_ADDC_DIGIT(a, b, cin, coutp) __builtin_addc((a), (b), (cin), (coutp))
#define MP_SUBC_DIGIT(a, b, bin, boutp) __builtin_subc((a), (b), (bin), (boutp))
typedef unsigned int mp_carry_t;
#else
#error "Radix must be 32bit or 64 bit"
#endif

#define ADDC(sumOut, carryOut, addend1, addend2, carryIn)                                                              \
    do {                                                                                                               \
        mp_carry_t addcCarry;                                                                                          \
        (sumOut) = MP_ADDC_DIGIT((addend1), (addend2), (mp_carry_t)(carryIn), &addcCarry);                             \
        (carryOut) = (unsigned int)addcCarry;                                                                          \
    } while (0)

#define ADDC64(sumOut, carryOut, addend1, addend2, carryIn)                                                            \
    do {                                                                                                               \
        unsigned long long addc64Carry;                                                                                \
        (sumOut) = __builtin_addcll((addend1), (addend2), (unsigned long long)(carryIn), &addc64Carry);                \
        (carryOut) = (unsigned int)addc64Carry;                                                                        \
    } while (0)

#define SUBC(differenceOut, borrowOut, minuend, subtrahend, borrowIn)                                                  \
    do {                                                                                                               \
        mp_carry_t subcBorrow;                                                                                         \
        (differenceOut) = MP_SUBC_DIGIT((minuend), (subtrahend), (mp_carry_t)(borrowIn), &subcBorrow);                 \
        (borrowOut) = (unsigned int)subcBorrow;                                                                        \
    } while (0)

#elif MP_CARRY_FORCE_TIER == 2

// Tier 2: the generic overflow builtins, chained; covers gcc 5-13. carryIn is 0 or 1, so at most one of the two
// additions can overflow and the two carries can be OR-ed.

#define ADDC(sumOut, carryOut, addend1, addend2, carryIn)                                                              \
    do {                                                                                                               \
        digit_t addcTmp;                                                                                               \
        unsigned int addcC1 = (unsigned int)__builtin_add_overflow((addend1), (digit_t)(carryIn), &addcTmp);           \
        unsigned int addcC2 = (unsigned int)__builtin_add_overflow(addcTmp, (addend2), &(sumOut));                     \
        (carryOut) = addcC1 | addcC2;                                                                                  \
    } while (0)

#define ADDC64(sumOut, carryOut, addend1, addend2, carryIn)                                                            \
    do {                                                                                                               \
        uint64_t addc64Tmp;                                                                                            \
        unsigned int addc64C1 = (unsigned int)__builtin_add_overflow((addend1), (uint64_t)(carryIn), &addc64Tmp);      \
        unsigned int addc64C2 = (unsigned int)__builtin_add_overflow(addc64Tmp, (addend2), &(sumOut));                 \
        (carryOut) = addc64C1 | addc64C2;                                                                              \
    } while (0)

#define SUBC(differenceOut, borrowOut, minuend, subtrahend, borrowIn)                                                  \
    do {                                                                                                               \
        digit_t subcTmp;                                                                                               \
        unsigned int subcB1 = (unsigned int)__builtin_sub_overflow((minuend), (subtrahend), &subcTmp);                 \
        unsigned int subcB2 = (unsigned int)__builtin_sub_overflow(subcTmp, (digit_t)(borrowIn), &(differenceOut));    \
        (borrowOut) = subcB1 | subcB2;                                                                                 \
    } while (0)

#else

// Tier 3: the generic overflow primitives, which keep the carry out of a C relational operator the compiler is
// free to branch on.

#define ADDC(sumOut, carryOut, addend1, addend2, carryIn)                                                              \
    do {                                                                                                               \
        digit_t addcTmp = (addend1) + (digit_t)(carryIn);                                                              \
        (sumOut) = (addend2) + addcTmp;                                                                                \
        (carryOut) = (unsigned int)((addcTmp < (digit_t)(carryIn)) | ((sumOut) < addcTmp));                            \
    } while (0)

#define ADDC64(sumOut, carryOut, addend1, addend2, carryIn)                                                            \
    do {                                                                                                               \
        uint64_t addc64Tmp = (addend1) + (uint64_t)(carryIn);                                                          \
        (sumOut) = (addend2) + addc64Tmp;                                                                              \
        (carryOut) = (unsigned int)((addc64Tmp < (uint64_t)(carryIn)) | ((sumOut) < addc64Tmp));                       \
    } while (0)

#define SUBC(differenceOut, borrowOut, minuend, subtrahend, borrowIn)                                                  \
    do {                                                                                                               \
        digit_t subcTmp = (minuend) - (subtrahend);                                                                    \
        unsigned int subcBorrow = ((minuend) < (subtrahend)) | ((borrowIn) & !(subcTmp));                              \
        (differenceOut) = subcTmp - (digit_t)(borrowIn);                                                               \
        (borrowOut) = subcBorrow;                                                                                      \
    } while (0)

#endif

#endif
