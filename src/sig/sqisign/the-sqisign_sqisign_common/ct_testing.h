// SPDX-License-Identifier: Apache-2.0

#ifndef CT_TESTING_H
#define CT_TESTING_H

// liboqs build shim for SQIsign's constant-time annotations.
//
// Replaces include/ct_testing.h when SQIsign is built inside liboqs. Upstream the
// annotations are enabled by ENABLE_CT_TESTING, which liboqs never defines; it has
// its own switch, OQS_ENABLE_TEST_CONSTANT_TIME (cmake -DOQS_ENABLE_TEST_CONSTANT_TIME=ON),
// and drives the same valgrind primitives from tests/test_helpers.h. Keying off the
// liboqs switch keeps SQIsign's poison/declassify annotations live in liboqs'
// constant-time test instead of silently compiling them out.
//
// CT_TESTING_MAKE_PUBLIC(addr, len)  declassify: region is defined/public.
// CT_TESTING_MAKE_SECRET(addr, len)  poison:     region is undefined/secret.

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
#include <valgrind/memcheck.h>

#define CT_TESTING_MAKE_PUBLIC(addr, len) VALGRIND_MAKE_MEM_DEFINED((addr), (len))
#define CT_TESTING_MAKE_SECRET(addr, len) VALGRIND_MAKE_MEM_UNDEFINED((addr), (len))
#else
// Still evaluate (as void) both arguments so a variable used only in an annotation
// does not trip -Wunused-variable under -Werror.
#define CT_TESTING_MAKE_PUBLIC(addr, len) ((void)(addr), (void)(len))
#define CT_TESTING_MAKE_SECRET(addr, len) ((void)(addr), (void)(len))
#endif

#endif /* CT_TESTING_H */
