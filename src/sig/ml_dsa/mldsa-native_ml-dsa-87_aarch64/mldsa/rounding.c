/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#include <stdint.h>

#include "ct.h"
#include "rounding.h"


void mld_power2round(int32_t *a0, int32_t *a1, const int32_t a)
{
  *a1 = (a + (1 << (MLDSA_D - 1)) - 1) >> MLDSA_D;
  *a0 = a - (*a1 << MLDSA_D);
}

void mld_decompose(int32_t *a0, int32_t *a1, int32_t a)
{
  *a1 = (a + 127) >> 7;
  /* We know a >= 0 and a < MLDSA_Q, so... */
  cassert(*a1 >= 0 && *a1 <= 65472);

#if MLDSA_MODE == 2
  *a1 = (*a1 * 11275 + (1 << 23)) >> 24;
  cassert(*a1 >= 0 && *a1 <= 44);

  *a1 = mld_ct_sel_int32(0, *a1, mld_ct_cmask_neg_i32(43 - *a1));
  cassert(*a1 >= 0 && *a1 <= 43);
#else /* MLDSA_MODE == 2 */
  *a1 = (*a1 * 1025 + (1 << 21)) >> 22;
  cassert(*a1 >= 0 && *a1 <= 16);

  *a1 &= 15;
  cassert(*a1 >= 0 && *a1 <= 15);

#endif /* MLDSA_MODE != 2 */

  *a0 = a - *a1 * 2 * MLDSA_GAMMA2;
  *a0 = mld_ct_sel_int32(*a0 - MLDSA_Q, *a0,
                         mld_ct_cmask_neg_i32((MLDSA_Q - 1) / 2 - *a0));
}

unsigned int mld_make_hint(int32_t a0, int32_t a1)
{
  if (a0 > MLDSA_GAMMA2 || a0 < -MLDSA_GAMMA2 ||
      (a0 == -MLDSA_GAMMA2 && a1 != 0))
  {
    return 1;
  }

  return 0;
}

int32_t mld_use_hint(int32_t a, unsigned int hint)
{
  int32_t a0, a1;

  mld_decompose(&a0, &a1, a);
  if (hint == 0)
  {
    return a1;
  }

#if MLDSA_MODE == 2
  if (a0 > 0)
  {
    return (a1 == 43) ? 0 : a1 + 1;
  }
  else
  {
    return (a1 == 0) ? 43 : a1 - 1;
  }
#else  /* MLDSA_MODE == 2 */
  if (a0 > 0)
  {
    return (a1 + 1) & 15;
  }
  else
  {
    return (a1 - 1) & 15;
  }
#endif /* MLDSA_MODE != 2 */
}
