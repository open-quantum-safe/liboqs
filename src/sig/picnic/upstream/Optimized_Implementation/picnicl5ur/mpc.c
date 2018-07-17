/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */


#include "mpc.h"

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif

#include <string.h>

void mpc_clear(mzd_local_t* const* res, unsigned sc) {
  for (unsigned int i = 0; i < sc; i++) {
    mzd_local_clear(res[i]);
  }
}

void mpc_shift_right(mzd_local_t* const* res, mzd_local_t* const* val, unsigned count,
                     unsigned sc) {
  MPC_LOOP_CONST(mzd_shift_right, res, val, count, sc);
}

void mpc_shift_left(mzd_local_t* const* res, mzd_local_t* const* val, unsigned count, unsigned sc) {
  MPC_LOOP_CONST(mzd_shift_left, res, val, count, sc);
}

void mpc_and_const(mzd_local_t* const* result, mzd_local_t* const* first, mzd_local_t const* second,
                   unsigned sc) {
  MPC_LOOP_CONST(mzd_xor, result, first, second, sc);
}

void mpc_xor(mzd_local_t* const* result, mzd_local_t* const* first, mzd_local_t* const* second,
             unsigned sc) {
  MPC_LOOP_SHARED(mzd_xor, result, first, second, sc);
}

void mpc_and_uint64(uint64_t* res, uint64_t const* first, uint64_t const* second, uint64_t const* r,
                    view_t* view, unsigned viewshift) {
  for (unsigned m = 0; m < SC_PROOF; ++m) {
    const unsigned j = (m + 1) % SC_PROOF;
    uint64_t tmp1    = second[m] ^ second[j];
    uint64_t tmp2    = first[j] & second[m];
    tmp1             = tmp1 & first[m];
    tmp1             = tmp1 ^ tmp2;
    tmp2             = r[m] ^ r[j];
    res[m] = tmp1 = tmp1 ^ tmp2;
    if (viewshift) {
      tmp1          = tmp1 >> viewshift;
      view->t[m]    = view->t[m] ^ tmp1;
    } else {
      // on first call (viewshift == 0), view->t[0..2] == 0
      view->t[m] = tmp1;
    }
  }
}



void mpc_and_verify_uint64(uint64_t* res, uint64_t const* first, uint64_t const* second,
                           uint64_t const* r, view_t* view, uint64_t const mask,
                           unsigned viewshift) {
  for (unsigned m = 0; m < (SC_VERIFY - 1); ++m) {
    const unsigned j = (m + 1);
    uint64_t tmp1    = second[m] ^ second[j];
    uint64_t tmp2    = first[j] & second[m];
    tmp1             = tmp1 & first[m];
    tmp1             = tmp1 ^ tmp2;
    tmp2             = r[m] ^ r[j];
    res[m] = tmp1 = tmp1 ^ tmp2;
    if (viewshift || m) {
      tmp1          = tmp1 >> viewshift;
      view->t[m]    = view->t[m] ^ tmp1;
    } else {
      // on first call (viewshift == 0), view->t[0] == 0
      view->t[m] = tmp1;
    }
  }

  const uint64_t rsc       = view->t[SC_VERIFY - 1] << viewshift;
  res[SC_VERIFY - 1] = rsc & mask;
}



void mpc_copy(mzd_local_t** out, mzd_local_t* const* in, unsigned sc) {
  for (unsigned i = 0; i < sc; ++i) {
    mzd_local_copy(out[i], in[i]);
  }
}
