#ifndef MPC_H
#define MPC_H

#include "macros.h"
#include "mzd_additional.h"

// Share count for proofs
#define SC_PROOF 3
// Share count for verification
#define SC_VERIFY 2

typedef union {
  mzd_local_t* s[SC_PROOF];
  uint64_t t[SC_PROOF];
} view_t;

typedef view_t rvec_t;

#define MPC_LOOP_CONST(function, result, first, second, sc)                                        \
  do {                                                                                             \
    for (unsigned int e = 0; e < (sc); ++e) {                                                      \
      function((result)[e], (first)[e], (second));                                                 \
    }                                                                                              \
  } while (0)

#define MPC_LOOP_SHARED(function, result, first, second, sc)                                       \
  do {                                                                                             \
    for (unsigned int o = 0; o < (sc); ++o) {                                                      \
      function((result)[o], (first)[o], (second)[o]);                                              \
    }                                                                                              \
  } while (0)

#define MPC_LOOP_CONST_C(function, result, first, second, sc, c)                                   \
  do {                                                                                             \
    if (!(c)) {                                                                                    \
      function((result)[0], (first)[0], (second));                                                 \
    } else if ((c) == (sc)) {                                                                      \
      function((result)[(sc)-1], first[(sc)-1], (second));                                         \
    }                                                                                              \
  } while (0)

void mpc_shift_right(mzd_local_t* const* res, mzd_local_t* const* val, unsigned count,
                     unsigned sc) ATTR_NONNULL;

void mpc_shift_left(mzd_local_t* const* res, mzd_local_t* const* val, unsigned count,
                    unsigned sc) ATTR_NONNULL;

void mpc_and_const(mzd_local_t* const* res, mzd_local_t* const* first, mzd_local_t const* second,
                   unsigned sc) ATTR_NONNULL;

void mpc_xor(mzd_local_t* const* res, mzd_local_t* const* first, mzd_local_t* const* second,
             unsigned sc) ATTR_NONNULL;

void mpc_clear(mzd_local_t* const* res, unsigned sc) ATTR_NONNULL;

void mpc_and(mzd_local_t* const* res, mzd_local_t* const* first, mzd_local_t* const* second,
             mzd_local_t* const* r, view_t* view, unsigned viewshift,
             mzd_local_t* const* buffer) ATTR_NONNULL;

void mpc_and_verify(mzd_local_t* const* res, mzd_local_t* const* first, mzd_local_t* const* second,
                    mzd_local_t* const* r, view_t* view, mzd_local_t const* mask,
                    unsigned viewshift, mzd_local_t* const* buffer) ATTR_NONNULL;

void mpc_and_uint64(uint64_t* res, uint64_t const* first, uint64_t const* second, uint64_t const* r,
                    view_t* view, unsigned viewshift) __attribute__((nonnull));

void mpc_and_verify_uint64(uint64_t* res, uint64_t const* first, uint64_t const* second,
                           uint64_t const* r, view_t* view, uint64_t const mask, unsigned viewshift)
    __attribute__((nonnull));


void mpc_copy(mzd_local_t** out, mzd_local_t* const* in, unsigned sc) ATTR_NONNULL_ARG(2);

#endif
