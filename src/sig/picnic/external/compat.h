/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC_COMPAT_H
#define PICNIC_COMPAT_H

#include <endian_compat.h>

// void* picnic_aligned_alloc(size_t alignment, size_t size);
#define picnic_aligned_alloc   OQS_MEM_aligned_alloc

// void picnic_aligned_free(void* ptr);
#define picnic_aligned_free    OQS_MEM_aligned_free

// int picnic_timingsafe_bcmp(const void* a, const void* b, size_t len);
#define picnic_timingsafe_bcmp OQS_MEM_secure_bcmp

// void picnic_explicit_bzero(void* a, size_t len);
#define picnic_explicit_bzero  OQS_MEM_cleanse

#endif
