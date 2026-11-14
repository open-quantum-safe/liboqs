/*
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "compat.h"

#include <string.h>

#if !defined(HAVE_MEMPCPY)

/**
 * Compatibility implementation of mempcpy
 */
void* faest_mempcpy(void* dst, const void* src, size_t len) {
  uint8_t* ret = memcpy(dst, src, len);
  return ret + len;
}
#endif /* HAVE_MEMPCPY */
