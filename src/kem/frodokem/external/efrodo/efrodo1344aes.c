#include <stddef.h>
#include <stdint.h>

#include <oqs/kem_frodokem.h>

#include "efrodo1344aes_params.h"

#include "../frodo_internal.h"
#include "ekem.c"
#include "../noise.c"
#include "../util.c"

#include "../frodo_macrify_optimized.c"
#include "../frodo_macrify_aes_portable.c"
