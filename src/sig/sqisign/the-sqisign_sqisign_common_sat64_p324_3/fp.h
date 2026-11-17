#ifndef FP_P324_3_H
#define FP_P324_3_H

#include <sqisign_namespace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <tutil.h>
#include <fp_constants.h>
#include <fp_generic.h>

_Static_assert(NWORDS_FIELD == FPG_N, "fp_constants.h disagrees with fp_generic_params.h");

// Type for elements of GF(p)
#define fp_t fpg_t

// Constants in Montgomery form, provided by fp_generic.c
#undef ZERO
#undef ONE
#define ZERO fpg_ZERO
#define ONE fpg_ONE

#endif
