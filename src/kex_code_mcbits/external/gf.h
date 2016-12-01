#include <stdint.h>

typedef uint16_t gf;

gf gf_mul(gf, gf);
gf gf_sq(gf);
gf gf_inv(gf);
gf gf_diff(gf, gf);

void GF_mul(gf *, gf *, gf *);

