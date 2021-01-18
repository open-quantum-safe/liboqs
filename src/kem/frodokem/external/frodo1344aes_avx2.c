#include <stddef.h>
#include <stdint.h>

#include <oqs/kem_frodokem.h>

#include "frodo_internal.h"
#include "frodo1344aes_params.h"

#include "frodo_macrify_aes_avx2.c"

#define frodo_mul_add_as_plus_e_actual frodo_mul_add_as_plus_e_avx2
#include "frodo_macrify_as_plus_e.c"
