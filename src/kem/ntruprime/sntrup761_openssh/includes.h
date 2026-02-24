#include <stdint.h>
#include <oqs/common.h>
#include <oqs/sha2.h>
#include <oqs/rand.h>
#include "sntrup761.h"

#ifdef OQS_ENABLE_KEM_NTRUPRIME
#define USE_SNTRUP761X25519
#endif

#ifdef _MSC_VER
/* for alloca stack allocation of variable length arrays */
#include <malloc.h>
#define alloca _alloca
/* drop attributes on MSVC */
#define __attribute__(x)
#endif
