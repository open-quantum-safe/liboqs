#ifndef GF16M_H
#define GF16M_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "deriv_params.h"
#include "gf16.h"

#define get_gf16m(gf16m, x, y) (gf16m[(((x)*rank) + (y))])
#define set_gf16m(gf16m, x, y, value) (gf16m[(((x)*rank) + (y))] = value)

typedef gf16_t gf16m_t[sq_rank];

#endif
