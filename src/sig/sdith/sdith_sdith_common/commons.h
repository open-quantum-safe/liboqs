#ifndef COMMONS_H
#define COMMONS_H

/* liboqs links one shared copy of this code into every scheme variant, so the
 * internal symbols need a namespace of their own. See gen_namespace.sh. */
#ifdef SDITH3_FOR_LIBOQS
#include "sdith_namespace.h"
#endif

#ifdef __cplusplus
#define EXPORT extern "C"
#define EXPORT_DECL extern "C"
#include <cstdint>
#else
#define EXPORT
#define EXPORT_DECL extern
#include "stdint.h"
#endif

#ifdef __x86_64__
#define CPU_SUPPORTS(feature) __builtin_cpu_supports(feature)
#endif

/* Namespacing with the appropriate prefix */
#ifndef SDITH_PUBLIC_API_NAMESPACE
#ifdef APPLY_PUBLIC_API_NAMESPACE
#ifndef concat2
#define _concat2(a, b) a ## b
#define concat2(a, b) _concat2(a, b)
#endif
#define SDITH_PUBLIC_API_NAMESPACE(s) concat2(APPLY_PUBLIC_API_NAMESPACE, s)
#else
#define SDITH_PUBLIC_API_NAMESPACE(s) s
#endif
#endif

typedef void bitvec_t;   // vector of bits row major (dimensions provided at runtime)
typedef void bitmat_t;   // matrix of bits row major (dimensions provided at runtime)
typedef void flambda_t;  // at runtime, the size of a big fielf elem  is lam (i.e. lambda/8)
typedef void fpoly_t;    // at runtime, the size of a degree d is (d+1).lam
typedef void seed_t;     // at runtime, the size of a commit is 1.lam
typedef void salt_t;     // at runtime, the size of a commit is 1.lam
typedef void hash_t;     // at runtime, the size of a hash is 2.lam
typedef void commit_t;   // at runtime, the size of a commit is 2.lam
typedef void hash_ctx_t;

// type large enough to hold one field element
typedef uint64_t flambda_max_t[4] __attribute((aligned(32)));  // usable as data type for temporary variables

typedef enum proofow_variant_t {
  PROOFOW_VARIANT_SHAKE = 0,
  PROOFOW_VARIANT_CIPHER = 1
} proofow_variant_t;

#endif  // COMMONS_H
