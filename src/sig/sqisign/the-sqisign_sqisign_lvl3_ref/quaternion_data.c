#include <stddef.h>
#include <stdint.h>
#include <quaternion_data.h>
const ibz_t QUAT_prime_cofactor = 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 24, ._mp_d = (mp_limb_t[]) {0x171,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x171,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x171,0x0,0x0,0x0,0x0,0x8000000000000000}}}
#endif
;
const quat_alg_t QUATALG_PINFTY = {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 24, ._mp_d = (mp_limb_t[]) {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x40ff}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x40ffffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0x40ffffffffffffff}}}
#endif
};
const quat_p_extremal_maximal_order_t EXTREMAL_ORDERS[8] = {{{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 1}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xafcc,0xe7ed,0x58f3,0x3e59,0x9763,0x88d6,0xf2c5,0x4a6d,0x3afe,0xf44b,0x7d27,0x3c31}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xe7edafcc,0x3e5958f3,0x88d69763,0x4a6df2c5,0xf44b3afe,0x3c317d27}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x3e5958f3e7edafcc,0x4a6df2c588d69763,0x3c317d27f44b3afe}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xafcc,0xe7ed,0x58f3,0x3e59,0x9763,0x88d6,0xf2c5,0x4a6d,0x3afe,0xf44b,0x7d27,0x3c31}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xe7edafcc,0x3e5958f3,0x88d69763,0x4a6df2c5,0xf44b3afe,0x3c317d27}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x3e5958f3e7edafcc,0x4a6df2c588d69763,0x3c317d27f44b3afe}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xd7e6,0xf3f6,0xac79,0x9f2c,0x4bb1,0xc46b,0xf962,0x2536,0x9d7f,0xfa25,0xbe93,0x1e18}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xf3f6d7e6,0x9f2cac79,0xc46b4bb1,0x2536f962,0xfa259d7f,0x1e18be93}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x9f2cac79f3f6d7e6,0x2536f962c46b4bb1,0x1e18be93fa259d7f}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x2fb7,0xea91,0xa3ea,0x2a21,0x9cd1,0x26b3,0xde73,0xa2d3,0xcecc,0x20a1,0xc963,0x266b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xea912fb7,0x2a21a3ea,0x26b39cd1,0xa2d3de73,0x20a1cecc,0x266bc963}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x2a21a3eaea912fb7,0xa2d3de7326b39cd1,0x266bc96320a1cecc}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 24, ._mp_d = (mp_limb_t[]) {0x1e5f,0x4aa8,0x9064,0x8436,0x8fae,0x50ab,0x2fd8,0xdd15,0x617a,0x8343,0x9423,0x3d7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x680}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x4aa81e5f,0x84369064,0x50ab8fae,0xdd152fd8,0x8343617a,0x3d79423,0x0,0x0,0x0,0x0,0x0,0x6800000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x843690644aa81e5f,0xdd152fd850ab8fae,0x3d794238343617a,0x0,0x0,0x680000000000000}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xd7e6,0xf3f6,0xac79,0x9f2c,0x4bb1,0xc46b,0xf962,0x2536,0x9d7f,0xfa25,0xbe93,0x1e18}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xf3f6d7e6,0x9f2cac79,0xc46b4bb1,0x2536f962,0xfa259d7f,0x1e18be93}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x9f2cac79f3f6d7e6,0x2536f962c46b4bb1,0x1e18be93fa259d7f}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x1e5f,0x4aa8,0x9064,0x8436,0x8fae,0x50ab,0x2fd8,0xdd15,0x617a,0x8343,0x9423,0x3d7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x4aa81e5f,0x84369064,0x50ab8fae,0xdd152fd8,0x8343617a,0x3d79423}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x843690644aa81e5f,0xdd152fd850ab8fae,0x3d794238343617a}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xafcc,0xe7ed,0x58f3,0x3e59,0x9763,0x88d6,0xf2c5,0x4a6d,0x3afe,0xf44b,0x7d27,0x3c31}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xe7edafcc,0x3e5958f3,0x88d69763,0x4a6df2c5,0xf44b3afe,0x3c317d27}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x3e5958f3e7edafcc,0x4a6df2c588d69763,0x3c317d27f44b3afe}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x2fb7,0xea91,0xa3ea,0x2a21,0x9cd1,0x26b3,0xde73,0xa2d3,0xcecc,0x20a1,0xc963,0x266b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xea912fb7,0x2a21a3ea,0x26b39cd1,0xa2d3de73,0x20a1cecc,0x266bc963}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x2a21a3eaea912fb7,0xa2d3de7326b39cd1,0x266bc96320a1cecc}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 5}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xc40,0xb21f,0x5bc7,0x8dca,0xc2a2,0xca0a,0xc8b1,0xbddd,0xcb7d,0xc9d5,0xa9d,0x3cc0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xb21f0c40,0x8dca5bc7,0xca0ac2a2,0xbdddc8b1,0xc9d5cb7d,0x3cc00a9d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x8dca5bc7b21f0c40,0xbdddc8b1ca0ac2a2,0x3cc00a9dc9d5cb7d}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xc40,0xb21f,0x5bc7,0x8dca,0xc2a2,0xca0a,0xc8b1,0xbddd,0xcb7d,0xc9d5,0xa9d,0x3cc0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xb21f0c40,0x8dca5bc7,0xca0ac2a2,0xbdddc8b1,0xc9d5cb7d,0x3cc00a9d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x8dca5bc7b21f0c40,0xbdddc8b1ca0ac2a2,0x3cc00a9dc9d5cb7d}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x8620,0xd90f,0x2de3,0x46e5,0x6151,0xe505,0xe458,0xdeee,0xe5be,0xe4ea,0x54e,0x1e60}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xd90f8620,0x46e52de3,0xe5056151,0xdeeee458,0xe4eae5be,0x1e60054e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x46e52de3d90f8620,0xdeeee458e5056151,0x1e60054ee4eae5be}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x6801,0x9f6f,0x4e1e,0xbd0b,0x5c5e,0xaa12,0xb4c6,0x3849,0xd6c7,0x2d76,0x3227,0xb106}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x9f6f6801,0xbd0b4e1e,0xaa125c5e,0x3849b4c6,0x2d76d6c7,0xb1063227}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbd0b4e1e9f6f6801,0x3849b4c6aa125c5e,0xb10632272d76d6c7}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 24, ._mp_d = (mp_limb_t[]) {0x8400,0x4135,0x343c,0xa4cf,0x6603,0xa414,0xc207,0x5ac7,0x921b,0xd084,0x1ed,0x6cf,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x280}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x41358400,0xa4cf343c,0xa4146603,0x5ac7c207,0xd084921b,0x6cf01ed,0x0,0x0,0x0,0x0,0x0,0x2800000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xa4cf343c41358400,0x5ac7c207a4146603,0x6cf01edd084921b,0x0,0x0,0x280000000000000}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x8620,0xd90f,0x2de3,0x46e5,0x6151,0xe505,0xe458,0xdeee,0xe5be,0xe4ea,0x54e,0x1e60}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xd90f8620,0x46e52de3,0xe5056151,0xdeeee458,0xe4eae5be,0x1e60054e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x46e52de3d90f8620,0xdeeee458e5056151,0x1e60054ee4eae5be}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x8400,0x4135,0x343c,0xa4cf,0x6603,0xa414,0xc207,0x5ac7,0x921b,0xd084,0x1ed,0x6cf}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x41358400,0xa4cf343c,0xa4146603,0x5ac7c207,0xd084921b,0x6cf01ed}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xa4cf343c41358400,0x5ac7c207a4146603,0x6cf01edd084921b}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xc40,0xb21f,0x5bc7,0x8dca,0xc2a2,0xca0a,0xc8b1,0xbddd,0xcb7d,0xc9d5,0xa9d,0x3cc0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xb21f0c40,0x8dca5bc7,0xca0ac2a2,0xbdddc8b1,0xc9d5cb7d,0x3cc00a9d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x8dca5bc7b21f0c40,0xbdddc8b1ca0ac2a2,0x3cc00a9dc9d5cb7d}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x6801,0x9f6f,0x4e1e,0xbd0b,0x5c5e,0xaa12,0xb4c6,0x3849,0xd6c7,0x2d76,0x3227,0xb106}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x9f6f6801,0xbd0b4e1e,0xaa125c5e,0x3849b4c6,0x2d76d6c7,0xb1063227}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbd0b4e1e9f6f6801,0x3849b4c6aa125c5e,0xb10632272d76d6c7}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 13}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x5b40,0x7328,0xdb38,0x5357,0x465b,0x31d8,0x1f3,0x85cf,0x32b9,0xd8dc,0x6f6d,0x3dab,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x73285b40,0x5357db38,0x31d8465b,0x85cf01f3,0xd8dc32b9,0x3dab6f6d,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5357db3873285b40,0x85cf01f331d8465b,0x3dab6f6dd8dc32b9,0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x5b40,0x7328,0xdb38,0x5357,0x465b,0x31d8,0x1f3,0x85cf,0x32b9,0xd8dc,0x6f6d,0x3dab,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x73285b40,0x5357db38,0x31d8465b,0x85cf01f3,0xd8dc32b9,0x3dab6f6d,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5357db3873285b40,0x85cf01f331d8465b,0x3dab6f6dd8dc32b9,0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x2da0,0x3994,0xed9c,0xa9ab,0x232d,0x98ec,0x80f9,0xc2e7,0x195c,0xec6e,0xb7b6,0x1ed5,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x39942da0,0xa9abed9c,0x98ec232d,0xc2e780f9,0xec6e195c,0x1ed5b7b6,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa9abed9c39942da0,0xc2e780f998ec232d,0x1ed5b7b6ec6e195c,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -13, ._mp_d = (mp_limb_t[]) {0xb7ef,0x4ddc,0x58cc,0xe284,0xc4a7,0xb9ed,0xdca9,0xc383,0xc3dd,0x5a13,0xd2bc,0x7663,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -7, ._mp_d = (mp_limb_t[]) {0x4ddcb7ef,0xe28458cc,0xb9edc4a7,0xc383dca9,0x5a13c3dd,0x7663d2bc,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xe28458cc4ddcb7ef,0xc383dca9b9edc4a7,0x7663d2bc5a13c3dd,0x3}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 24, ._mp_d = (mp_limb_t[]) {0xdc07,0x925a,0x605a,0x9489,0x475b,0x7944,0x880f,0x65fa,0xed5a,0x329c,0x13f8,0x78f2,0xfffe,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x207f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x925adc07,0x9489605a,0x7944475b,0x65fa880f,0x329ced5a,0x78f213f8,0xfffffffe,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x207fffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x9489605a925adc07,0x65fa880f7944475b,0x78f213f8329ced5a,0xfffffffffffffffe,0xffffffffffffffff,0x207fffffffffffff}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x2da0,0x3994,0xed9c,0xa9ab,0x232d,0x98ec,0x80f9,0xc2e7,0x195c,0xec6e,0xb7b6,0x1ed5,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x39942da0,0xa9abed9c,0x98ec232d,0xc2e780f9,0xec6e195c,0x1ed5b7b6,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa9abed9c39942da0,0xc2e780f998ec232d,0x1ed5b7b6ec6e195c,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x11}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x11}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x11}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x9c07,0x5ca4,0xc660,0xc2e5,0x94d7,0x2b1d,0x3b32,0xa3de,0x67a4,0x2fd3,0xfeab,0x1a11}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x5ca49c07,0xc2e5c660,0x2b1d94d7,0xa3de3b32,0x2fd367a4,0x1a11feab}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xc2e5c6605ca49c07,0xa3de3b322b1d94d7,0x1a11feab2fd367a4}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x5b40,0x7328,0xdb38,0x5357,0x465b,0x31d8,0x1f3,0x85cf,0x32b9,0xd8dc,0x6f6d,0x3dab,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x73285b40,0x5357db38,0x31d8465b,0x85cf01f3,0xd8dc32b9,0x3dab6f6d,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5357db3873285b40,0x85cf01f331d8465b,0x3dab6f6dd8dc32b9,0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -13, ._mp_d = (mp_limb_t[]) {0xb7ef,0x4ddc,0x58cc,0xe284,0xc4a7,0xb9ed,0xdca9,0xc383,0xc3dd,0x5a13,0xd2bc,0x7663,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -7, ._mp_d = (mp_limb_t[]) {0x4ddcb7ef,0xe28458cc,0xb9edc4a7,0xc383dca9,0x5a13c3dd,0x7663d2bc,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xe28458cc4ddcb7ef,0xc383dca9b9edc4a7,0x7663d2bc5a13c3dd,0x3}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x11}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x11}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x11}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 17}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xdfda,0xabc3,0xd4b8,0x7c1c,0x4727,0x66b2,0x21da,0x79cc,0xe3a3,0x553d,0x9b8d,0xa12d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xabc3dfda,0x7c1cd4b8,0x66b24727,0x79cc21da,0x553de3a3,0xa12d9b8d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x7c1cd4b8abc3dfda,0x79cc21da66b24727,0xa12d9b8d553de3a3}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xdfda,0xabc3,0xd4b8,0x7c1c,0x4727,0x66b2,0x21da,0x79cc,0xe3a3,0x553d,0x9b8d,0xa12d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xabc3dfda,0x7c1cd4b8,0x66b24727,0x79cc21da,0x553de3a3,0xa12d9b8d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x7c1cd4b8abc3dfda,0x79cc21da66b24727,0xa12d9b8d553de3a3}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xefed,0x55e1,0x6a5c,0xbe0e,0x2393,0x3359,0x10ed,0xbce6,0xf1d1,0xaa9e,0xcdc6,0x5096}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x55e1efed,0xbe0e6a5c,0x33592393,0xbce610ed,0xaa9ef1d1,0x5096cdc6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbe0e6a5c55e1efed,0xbce610ed33592393,0x5096cdc6aa9ef1d1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xe8e2,0xea6f,0x72f1,0x2e52,0x152a,0xc137,0x5fe4,0xfd0e,0x9736,0x7a1,0xfa3d,0xc6b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0xea6fe8e2,0x2e5272f1,0xc137152a,0xfd0e5fe4,0x7a19736,0xc6bfa3d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x2e5272f1ea6fe8e2,0xfd0e5fe4c137152a,0xc6bfa3d07a19736}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 24, ._mp_d = (mp_limb_t[]) {0x9a15,0x48a0,0x16ae,0xa42,0x3772,0x534a,0x26a7,0x2f5e,0xce7c,0x39eb,0xa365,0x745c,0x6a25,0xa257,0x2576,0x576a,0x76a2,0x6a25,0xa257,0x2576,0x576a,0x76a2,0x6a25,0x657}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x48a09a15,0xa4216ae,0x534a3772,0x2f5e26a7,0x39ebce7c,0x745ca365,0xa2576a25,0x576a2576,0x6a2576a2,0x2576a257,0x76a2576a,0x6576a25}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xa4216ae48a09a15,0x2f5e26a7534a3772,0x745ca36539ebce7c,0x576a2576a2576a25,0x2576a2576a2576a2,0x6576a2576a2576a}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xefed,0x55e1,0x6a5c,0xbe0e,0x2393,0x3359,0x10ed,0xbce6,0xf1d1,0xaa9e,0xcdc6,0x5096}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x55e1efed,0xbe0e6a5c,0x33592393,0xbce610ed,0xaa9ef1d1,0x5096cdc6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbe0e6a5c55e1efed,0xbce610ed33592393,0x5096cdc6aa9ef1d1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x8}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x50e5,0x2533,0xb03b,0x2c45,0xfde,0xaaf1,0xafff,0x8c73,0xebfd,0xfb3,0xc7bc,0x26}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x253350e5,0x2c45b03b,0xaaf10fde,0x8c73afff,0xfb3ebfd,0x26c7bc}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x2c45b03b253350e5,0x8c73afffaaf10fde,0x26c7bc0fb3ebfd}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xdfda,0xabc3,0xd4b8,0x7c1c,0x4727,0x66b2,0x21da,0x79cc,0xe3a3,0x553d,0x9b8d,0xa12d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xabc3dfda,0x7c1cd4b8,0x66b24727,0x79cc21da,0x553de3a3,0xa12d9b8d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x7c1cd4b8abc3dfda,0x79cc21da66b24727,0xa12d9b8d553de3a3}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xe8e2,0xea6f,0x72f1,0x2e52,0x152a,0xc137,0x5fe4,0xfd0e,0x9736,0x7a1,0xfa3d,0xc6b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0xea6fe8e2,0x2e5272f1,0xc137152a,0xfd0e5fe4,0x7a19736,0xc6bfa3d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x2e5272f1ea6fe8e2,0xfd0e5fe4c137152a,0xc6bfa3d07a19736}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x8}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 41}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xb000,0x6067,0x56e7,0x950c,0xb3d,0x28e6,0x1bfb,0xb990,0xeb8,0x7184,0x2273,0x29aa}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x6067b000,0x950c56e7,0x28e60b3d,0xb9901bfb,0x71840eb8,0x29aa2273}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x950c56e76067b000,0xb9901bfb28e60b3d,0x29aa227371840eb8}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xb000,0x6067,0x56e7,0x950c,0xb3d,0x28e6,0x1bfb,0xb990,0xeb8,0x7184,0x2273,0x29aa}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x6067b000,0x950c56e7,0x28e60b3d,0xb9901bfb,0x71840eb8,0x29aa2273}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x950c56e76067b000,0xb9901bfb28e60b3d,0x29aa227371840eb8}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xd800,0xb033,0x2b73,0xca86,0x59e,0x9473,0xdfd,0x5cc8,0x75c,0xb8c2,0x1139,0x14d5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xb033d800,0xca862b73,0x9473059e,0x5cc80dfd,0xb8c2075c,0x14d51139}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xca862b73b033d800,0x5cc80dfd9473059e,0x14d51139b8c2075c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xffff,0xef7f,0x6120,0xdec9,0x3d80,0xfcb4,0xe8d7,0x2d72,0x4077,0xeecc,0xcd2a,0x4bc9,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xef7fffff,0xdec96120,0xfcb43d80,0x2d72e8d7,0xeecc4077,0x4bc9cd2a,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdec96120ef7fffff,0x2d72e8d7fcb43d80,0x4bc9cd2aeecc4077,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -24, ._mp_d = (mp_limb_t[]) {0x73ba,0x1227,0x9519,0xedfb,0x605b,0xe80,0x1a20,0xf0b2,0xb418,0xa90c,0xb325,0xefd6,0x7e3e,0xf8fc,0xe3f1,0x8fc7,0x3f1f,0xfc7e,0xf1f8,0xc7e3,0x1f8f,0x7e3f,0xf8fc,0x71}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x122773ba,0xedfb9519,0xe80605b,0xf0b21a20,0xa90cb418,0xefd6b325,0xf8fc7e3e,0x8fc7e3f1,0xfc7e3f1f,0xc7e3f1f8,0x7e3f1f8f,0x71f8fc}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0xedfb9519122773ba,0xf0b21a200e80605b,0xefd6b325a90cb418,0x8fc7e3f1f8fc7e3e,0xc7e3f1f8fc7e3f1f,0x71f8fc7e3f1f8f}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xd800,0xb033,0x2b73,0xca86,0x59e,0x9473,0xdfd,0x5cc8,0x75c,0xb8c2,0x1139,0x14d5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xb033d800,0xca862b73,0x9473059e,0x5cc80dfd,0xb8c2075c,0x14d51139}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xca862b73b033d800,0x5cc80dfd9473059e,0x14d51139b8c2075c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x73ba,0x8a7,0x681e,0x130f,0xeee3,0xd966,0x4ebe,0xf78b,0xba4d,0xfa9,0xc409,0x245}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x8a773ba,0x130f681e,0xd966eee3,0xf78b4ebe,0xfa9ba4d,0x245c409}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x130f681e08a773ba,0xf78b4ebed966eee3,0x245c4090fa9ba4d}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xb000,0x6067,0x56e7,0x950c,0xb3d,0x28e6,0x1bfb,0xb990,0xeb8,0x7184,0x2273,0x29aa}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x6067b000,0x950c56e7,0x28e60b3d,0xb9901bfb,0x71840eb8,0x29aa2273}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x950c56e76067b000,0xb9901bfb28e60b3d,0x29aa227371840eb8}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xffff,0xef7f,0x6120,0xdec9,0x3d80,0xfcb4,0xe8d7,0x2d72,0x4077,0xeecc,0xcd2a,0x4bc9,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xef7fffff,0xdec96120,0xfcb43d80,0x2d72e8d7,0xeecc4077,0x4bc9cd2a,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdec96120ef7fffff,0x2d72e8d7fcb43d80,0x4bc9cd2aeecc4077,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 73}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x7b26,0x37a0,0xc8dc,0x97d3,0x7f2f,0xd6bd,0x931,0x1df2,0x2918,0x4a3e,0x2591,0x6ee7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x37a07b26,0x97d3c8dc,0xd6bd7f2f,0x1df20931,0x4a3e2918,0x6ee72591}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x97d3c8dc37a07b26,0x1df20931d6bd7f2f,0x6ee725914a3e2918}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x7b26,0x37a0,0xc8dc,0x97d3,0x7f2f,0xd6bd,0x931,0x1df2,0x2918,0x4a3e,0x2591,0x6ee7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x37a07b26,0x97d3c8dc,0xd6bd7f2f,0x1df20931,0x4a3e2918,0x6ee72591}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x97d3c8dc37a07b26,0x1df20931d6bd7f2f,0x6ee725914a3e2918}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x3d93,0x1bd0,0xe46e,0xcbe9,0xbf97,0xeb5e,0x498,0xef9,0x148c,0xa51f,0x92c8,0x3773}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x1bd03d93,0xcbe9e46e,0xeb5ebf97,0xef90498,0xa51f148c,0x377392c8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xcbe9e46e1bd03d93,0xef90498eb5ebf97,0x377392c8a51f148c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x4fae,0x9faa,0x2b8a,0x6a69,0x436c,0x633a,0x7892,0x301c,0xec62,0xcb54,0xe41,0xac50}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x9faa4fae,0x6a692b8a,0x633a436c,0x301c7892,0xcb54ec62,0xac500e41}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x6a692b8a9faa4fae,0x301c7892633a436c,0xac500e41cb54ec62}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -24, ._mp_d = (mp_limb_t[]) {0x30b3,0xeb66,0x87b7,0x617e,0x27c,0xfa7,0xdcf4,0x90c8,0x7e8b,0x9e3c,0xaf36,0xb7ba,0x5eeb,0xbaf7,0xbdd7,0x75ee,0x7baf,0xebdd,0xf75e,0xd7ba,0xeebd,0xaf75,0xdd7b,0x2eb}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xeb6630b3,0x617e87b7,0xfa7027c,0x90c8dcf4,0x9e3c7e8b,0xb7baaf36,0xbaf75eeb,0x75eebdd7,0xebdd7baf,0xd7baf75e,0xaf75eebd,0x2ebdd7b}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x617e87b7eb6630b3,0x90c8dcf40fa7027c,0xb7baaf369e3c7e8b,0x75eebdd7baf75eeb,0xd7baf75eebdd7baf,0x2ebdd7baf75eebd}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x3d93,0x1bd0,0xe46e,0xcbe9,0xbf97,0xeb5e,0x498,0xef9,0x148c,0xa51f,0x92c8,0x3773}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x1bd03d93,0xcbe9e46e,0xeb5ebf97,0xef90498,0xa51f148c,0x377392c8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xcbe9e46e1bd03d93,0xef90498eb5ebf97,0x377392c8a51f148c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x8}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xb5ab,0x986,0x1b92,0x5123,0x4b2a,0x653b,0x4896,0xc0fd,0x579e,0xc06c,0xd20e,0xf7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x986b5ab,0x51231b92,0x653b4b2a,0xc0fd4896,0xc06c579e,0xf7d20e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x51231b920986b5ab,0xc0fd4896653b4b2a,0xf7d20ec06c579e}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x7b26,0x37a0,0xc8dc,0x97d3,0x7f2f,0xd6bd,0x931,0x1df2,0x2918,0x4a3e,0x2591,0x6ee7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x37a07b26,0x97d3c8dc,0xd6bd7f2f,0x1df20931,0x4a3e2918,0x6ee72591}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x97d3c8dc37a07b26,0x1df20931d6bd7f2f,0x6ee725914a3e2918}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x4fae,0x9faa,0x2b8a,0x6a69,0x436c,0x633a,0x7892,0x301c,0xec62,0xcb54,0xe41,0xac50}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x9faa4fae,0x6a692b8a,0x633a436c,0x301c7892,0xcb54ec62,0xac500e41}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x6a692b8a9faa4fae,0x301c7892633a436c,0xac500e41cb54ec62}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x8}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 89}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xa194,0x6df0,0x4b4c,0xf874,0xb43e,0x362a,0x11bb,0x84f2,0xc623,0x61a2,0x7d42,0xe501,0x30}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x6df0a194,0xf8744b4c,0x362ab43e,0x84f211bb,0x61a2c623,0xe5017d42,0x30}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf8744b4c6df0a194,0x84f211bb362ab43e,0xe5017d4261a2c623,0x30}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xa194,0x6df0,0x4b4c,0xf874,0xb43e,0x362a,0x11bb,0x84f2,0xc623,0x61a2,0x7d42,0xe501,0x30}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x6df0a194,0xf8744b4c,0x362ab43e,0x84f211bb,0x61a2c623,0xe5017d42,0x30}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf8744b4c6df0a194,0x84f211bb362ab43e,0xe5017d4261a2c623,0x30}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x50ca,0x36f8,0x25a6,0x7c3a,0x5a1f,0x9b15,0x8dd,0xc279,0x6311,0x30d1,0xbea1,0x7280,0x18}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x36f850ca,0x7c3a25a6,0x9b155a1f,0xc27908dd,0x30d16311,0x7280bea1,0x18}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7c3a25a636f850ca,0xc27908dd9b155a1f,0x7280bea130d16311,0x18}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x83f7,0x63ae,0x245e,0x2154,0x883c,0x544b,0x8f96,0x1b2d,0xcc0c,0x8d73,0x7bdd,0x118e,0x1df}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x63ae83f7,0x2154245e,0x544b883c,0x1b2d8f96,0x8d73cc0c,0x118e7bdd,0x1df}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x2154245e63ae83f7,0x1b2d8f96544b883c,0x118e7bdd8d73cc0c,0x1df}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -24, ._mp_d = (mp_limb_t[]) {0xbd79,0x489c,0xbd84,0xce46,0x9344,0xb194,0x642a,0x3c5a,0xdb04,0x96f5,0x6e1f,0x4dcb,0xff6e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x207f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x489cbd79,0xce46bd84,0xb1949344,0x3c5a642a,0x96f5db04,0x4dcb6e1f,0xffffff6e,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x207fffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0xce46bd84489cbd79,0x3c5a642ab1949344,0x4dcb6e1f96f5db04,0xffffffffffffff6e,0xffffffffffffffff,0x207fffffffffffff}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x50ca,0x36f8,0x25a6,0x7c3a,0x5a1f,0x9b15,0x8dd,0xc279,0x6311,0x30d1,0xbea1,0x7280,0x18}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x36f850ca,0x7c3a25a6,0x9b155a1f,0xc27908dd,0x30d16311,0x7280bea1,0x18}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7c3a25a636f850ca,0xc27908dd9b155a1f,0x7280bea130d16311,0x18}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x61}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x61}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x61}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -13, ._mp_d = (mp_limb_t[]) {0xa1c9,0x3fda,0x577,0x71a8,0xf4d3,0x4269,0xecf2,0x2a5d,0x41b6,0x6e41,0x47e5,0x782c,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -7, ._mp_d = (mp_limb_t[]) {0x3fdaa1c9,0x71a80577,0x4269f4d3,0x2a5decf2,0x6e4141b6,0x782c47e5,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x71a805773fdaa1c9,0x2a5decf24269f4d3,0x782c47e56e4141b6,0x2}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xa194,0x6df0,0x4b4c,0xf874,0xb43e,0x362a,0x11bb,0x84f2,0xc623,0x61a2,0x7d42,0xe501,0x30}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x6df0a194,0xf8744b4c,0x362ab43e,0x84f211bb,0x61a2c623,0xe5017d42,0x30}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf8744b4c6df0a194,0x84f211bb362ab43e,0xe5017d4261a2c623,0x30}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x83f7,0x63ae,0x245e,0x2154,0x883c,0x544b,0x8f96,0x1b2d,0xcc0c,0x8d73,0x7bdd,0x118e,0x1df}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x63ae83f7,0x2154245e,0x544b883c,0x1b2d8f96,0x8d73cc0c,0x118e7bdd,0x1df}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x2154245e63ae83f7,0x1b2d8f96544b883c,0x118e7bdd8d73cc0c,0x1df}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x61}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x61}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x61}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 97}};
const quat_left_ideal_t CONNECTING_IDEALS[8] = {{{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xbf5a,0x9a6f,0xcde1,0x21d4,0x52b1,0xe7a0,0xf3ba,0x78eb,0xc45c,0x787f,0x5c29,0x1c51,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x9a6fbf5a,0x21d4cde1,0xe7a052b1,0x78ebf3ba,0x787fc45c,0x1c515c29,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x21d4cde19a6fbf5a,0x78ebf3bae7a052b1,0x1c515c29787fc45c,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x8015,0xfd5c,0xb508,0x1437,0xfa92,0x6222,0x1452,0xa79a,0x6c31,0xd3a9,0xb3c4,0x15c5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xfd5c8015,0x1437b508,0x6222fa92,0xa79a1452,0xd3a96c31,0x15c5b3c4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x1437b508fd5c8015,0xa79a14526222fa92,0x15c5b3c4d3a96c31}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xbf5a,0x9a6f,0xcde1,0x21d4,0x52b1,0xe7a0,0xf3ba,0x78eb,0xc45c,0x787f,0x5c29,0x1c51,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x9a6fbf5a,0x21d4cde1,0xe7a052b1,0x78ebf3ba,0x787fc45c,0x1c515c29,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x21d4cde19a6fbf5a,0x78ebf3bae7a052b1,0x1c515c29787fc45c,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x3f45,0x9d13,0x18d8,0xd9d,0x581f,0x857d,0xdf68,0xd151,0x582a,0xa4d6,0xa864,0x68b,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x9d133f45,0xd9d18d8,0x857d581f,0xd151df68,0xa4d6582a,0x68ba864,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xd9d18d89d133f45,0xd151df68857d581f,0x68ba864a4d6582a,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xdfad,0xcd37,0x66f0,0x90ea,0x2958,0x73d0,0xf9dd,0x3c75,0xe22e,0xbc3f,0xae14,0x8e28}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xcd37dfad,0x90ea66f0,0x73d02958,0x3c75f9dd,0xbc3fe22e,0x8e28ae14}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x90ea66f0cd37dfad,0x3c75f9dd73d02958,0x8e28ae14bc3fe22e}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x9d7a,0x920e,0xe71,0xc120,0x8fbf,0x607e,0x29f,0xff55,0x7422,0x4796,0xbca4,0x125b,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x920e9d7a,0xc1200e71,0x607e8fbf,0xff55029f,0x47967422,0x125bbca4,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc1200e71920e9d7a,0xff55029f607e8fbf,0x125bbca447967422,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xbcbf,0x76ed,0xc538,0xec53,0xeb88,0xb40d,0xa54e,0x14f,0x8bb2,0x300a,0xedb2,0x539}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x76edbcbf,0xec53c538,0xb40deb88,0x14fa54e,0x300a8bb2,0x539edb2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xec53c53876edbcbf,0x14fa54eb40deb88,0x539edb2300a8bb2}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x9d7a,0x920e,0xe71,0xc120,0x8fbf,0x607e,0x29f,0xff55,0x7422,0x4796,0xbca4,0x125b,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x920e9d7a,0xc1200e71,0x607e8fbf,0xff55029f,0x47967422,0x125bbca4,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc1200e71920e9d7a,0xff55029f607e8fbf,0x125bbca447967422,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xe0bb,0x1b20,0x4939,0xd4cc,0xa436,0xac70,0x5d50,0xfe05,0xe870,0x178b,0xcef2,0xd21,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x1b20e0bb,0xd4cc4939,0xac70a436,0xfe055d50,0x178be870,0xd21cef2,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xd4cc49391b20e0bb,0xfe055d50ac70a436,0xd21cef2178be870,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x4ebd,0xc907,0x738,0xe090,0x47df,0xb03f,0x814f,0x7faa,0x3a11,0x23cb,0xde52,0x892d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xc9074ebd,0xe0900738,0xb03f47df,0x7faa814f,0x23cb3a11,0x892dde52}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xe0900738c9074ebd,0x7faa814fb03f47df,0x892dde5223cb3a11}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xd282,0xcb1f,0x6532,0xe33e,0x153d,0xfd8,0x4275,0x2b62,0xf17d,0xdb04,0x3f12,0xf722,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xcb1fd282,0xe33e6532,0xfd8153d,0x2b624275,0xdb04f17d,0xf7223f12,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xe33e6532cb1fd282,0x2b6242750fd8153d,0xf7223f12db04f17d,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x50bf,0xeebf,0xe944,0xea4d,0x76d,0xcbc5,0x4919,0x12b0,0x71f3,0x9e30,0x3304,0x1265}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xeebf50bf,0xea4de944,0xcbc5076d,0x12b04919,0x9e3071f3,0x12653304}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xea4de944eebf50bf,0x12b04919cbc5076d,0x126533049e3071f3}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xd282,0xcb1f,0x6532,0xe33e,0x153d,0xfd8,0x4275,0x2b62,0xf17d,0xdb04,0x3f12,0xf722,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xcb1fd282,0xe33e6532,0xfd8153d,0x2b624275,0xdb04f17d,0xf7223f12,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xe33e6532cb1fd282,0x2b6242750fd8153d,0xf7223f12db04f17d,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x81c3,0xdc60,0x7bed,0xf8f0,0xdcf,0x4413,0xf95b,0x18b1,0x7f8a,0x3cd4,0xc0e,0xe4bd,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xdc6081c3,0xf8f07bed,0x44130dcf,0x18b1f95b,0x3cd47f8a,0xe4bd0c0e,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf8f07beddc6081c3,0x18b1f95b44130dcf,0xe4bd0c0e3cd47f8a,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xe941,0x658f,0x3299,0xf19f,0xa9e,0x87ec,0x213a,0x95b1,0x78be,0x6d82,0x1f89,0xfb91}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x658fe941,0xf19f3299,0x87ec0a9e,0x95b1213a,0x6d8278be,0xfb911f89}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xf19f3299658fe941,0x95b1213a87ec0a9e,0xfb911f896d8278be}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xdfda,0xabc3,0xd4b8,0x7c1c,0x4727,0x66b2,0x21da,0x79cc,0xe3a3,0x553d,0x9b8d,0xa12d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xabc3dfda,0x7c1cd4b8,0x66b24727,0x79cc21da,0x553de3a3,0xa12d9b8d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x7c1cd4b8abc3dfda,0x79cc21da66b24727,0xa12d9b8d553de3a3}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x60fb,0xd399,0x887f,0xd263,0xe0e7,0xb202,0x699b,0xea34,0x5a15,0x4b8a,0x6763,0x8e95}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xd39960fb,0xd263887f,0xb202e0e7,0xea34699b,0x4b8a5a15,0x8e956763}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xd263887fd39960fb,0xea34699bb202e0e7,0x8e9567634b8a5a15}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xdfda,0xabc3,0xd4b8,0x7c1c,0x4727,0x66b2,0x21da,0x79cc,0xe3a3,0x553d,0x9b8d,0xa12d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xabc3dfda,0x7c1cd4b8,0x66b24727,0x79cc21da,0x553de3a3,0xa12d9b8d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x7c1cd4b8abc3dfda,0x79cc21da66b24727,0xa12d9b8d553de3a3}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x7edf,0xd82a,0x4c38,0xa9b9,0x663f,0xb4af,0xb83e,0x8f97,0x898d,0x9b3,0x342a,0x1298}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xd82a7edf,0xa9b94c38,0xb4af663f,0x8f97b83e,0x9b3898d,0x1298342a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xa9b94c38d82a7edf,0x8f97b83eb4af663f,0x1298342a09b3898d}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xefed,0x55e1,0x6a5c,0xbe0e,0x2393,0x3359,0x10ed,0xbce6,0xf1d1,0xaa9e,0xcdc6,0x5096}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x55e1efed,0xbe0e6a5c,0x33592393,0xbce610ed,0xaa9ef1d1,0x5096cdc6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbe0e6a5c55e1efed,0xbce610ed33592393,0x5096cdc6aa9ef1d1}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xe00e,0xd869,0x1a76,0xd8de,0xfe4c,0xabc5,0x99e1,0xf264,0x7d83,0x9c3,0x32ab,0xb60b,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xd869e00e,0xd8de1a76,0xabc5fe4c,0xf26499e1,0x9c37d83,0xb60b32ab,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xd8de1a76d869e00e,0xf26499e1abc5fe4c,0xb60b32ab09c37d83,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xb00f,0x8bbf,0x19a9,0xd6b,0xf7b,0xcd5c,0x74e7,0xd7e2,0xa419,0x3593,0x56a8,0x8de8,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0x8bbfb00f,0xd6b19a9,0xcd5c0f7b,0xd7e274e7,0x3593a419,0x8de856a8,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xd6b19a98bbfb00f,0xd7e274e7cd5c0f7b,0x8de856a83593a419,0x1}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0xe00e,0xd869,0x1a76,0xd8de,0xfe4c,0xabc5,0x99e1,0xf264,0x7d83,0x9c3,0x32ab,0xb60b,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xd869e00e,0xd8de1a76,0xabc5fe4c,0xf26499e1,0x9c37d83,0xb60b32ab,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xd8de1a76d869e00e,0xf26499e1abc5fe4c,0xb60b32ab09c37d83,0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x2fff,0x4caa,0xcd,0xcb73,0xeed1,0xde69,0x24f9,0x1a82,0xd96a,0xd42f,0xdc02,0x2822}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x4caa2fff,0xcb7300cd,0xde69eed1,0x1a8224f9,0xd42fd96a,0x2822dc02}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xcb7300cd4caa2fff,0x1a8224f9de69eed1,0x2822dc02d42fd96a}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xf007,0x6c34,0xd3b,0x6c6f,0xff26,0xd5e2,0x4cf0,0xf932,0xbec1,0x84e1,0x9955,0xdb05}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x6c34f007,0x6c6f0d3b,0xd5e2ff26,0xf9324cf0,0x84e1bec1,0xdb059955}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x6c6f0d3b6c34f007,0xf9324cf0d5e2ff26,0xdb05995584e1bec1}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x7b26,0x37a0,0xc8dc,0x97d3,0x7f2f,0xd6bd,0x931,0x1df2,0x2918,0x4a3e,0x2591,0x6ee7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x37a07b26,0x97d3c8dc,0xd6bd7f2f,0x1df20931,0x4a3e2918,0x6ee72591}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x97d3c8dc37a07b26,0x1df20931d6bd7f2f,0x6ee725914a3e2918}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x3a91,0xcd01,0xac55,0x9a52,0x9887,0x118f,0x4dec,0x4245,0xd869,0x1022,0x1d16,0x7ad}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xcd013a91,0x9a52ac55,0x118f9887,0x42454dec,0x1022d869,0x7ad1d16}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x9a52ac55cd013a91,0x42454dec118f9887,0x7ad1d161022d869}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x7b26,0x37a0,0xc8dc,0x97d3,0x7f2f,0xd6bd,0x931,0x1df2,0x2918,0x4a3e,0x2591,0x6ee7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x37a07b26,0x97d3c8dc,0xd6bd7f2f,0x1df20931,0x4a3e2918,0x6ee72591}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x97d3c8dc37a07b26,0x1df20931d6bd7f2f,0x6ee725914a3e2918}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x4095,0x6a9f,0x1c86,0xfd81,0xe6a7,0xc52d,0xbb45,0xdbac,0x50ae,0x3a1b,0x87b,0x673a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x6a9f4095,0xfd811c86,0xc52de6a7,0xdbacbb45,0x3a1b50ae,0x673a087b}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xfd811c866a9f4095,0xdbacbb45c52de6a7,0x673a087b3a1b50ae}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x3d93,0x1bd0,0xe46e,0xcbe9,0xbf97,0xeb5e,0x498,0xef9,0x148c,0xa51f,0x92c8,0x3773}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x1bd03d93,0xcbe9e46e,0xeb5ebf97,0xef90498,0xa51f148c,0x377392c8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xcbe9e46e1bd03d93,0xef90498eb5ebf97,0x377392c8a51f148c}}}
#endif
, &MAXORD_O0}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xe5ca,0x3b34,0xb04b,0x430f,0xe795,0xa04a,0x8c7d,0xec47,0x77df,0x8e5c,0xb71e,0xd31f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x3b34e5ca,0x430fb04b,0xa04ae795,0xec478c7d,0x8e5c77df,0xd31fb71e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x430fb04b3b34e5ca,0xec478c7da04ae795,0xd31fb71e8e5c77df}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x4d27,0x98d5,0x3839,0x83ff,0x48b7,0x4d5b,0xc95b,0xbe45,0x9d44,0x36f3,0x4d57,0x6c26}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x98d54d27,0x83ff3839,0x4d5b48b7,0xbe45c95b,0x36f39d44,0x6c264d57}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x83ff383998d54d27,0xbe45c95b4d5b48b7,0x6c264d5736f39d44}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xe5ca,0x3b34,0xb04b,0x430f,0xe795,0xa04a,0x8c7d,0xec47,0x77df,0x8e5c,0xb71e,0xd31f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x3b34e5ca,0x430fb04b,0xa04ae795,0xec478c7d,0x8e5c77df,0xd31fb71e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x430fb04b3b34e5ca,0xec478c7da04ae795,0xd31fb71e8e5c77df}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x98a3,0xa25f,0x7811,0xbf10,0x9edd,0x52ef,0xc322,0x2e01,0xda9b,0x5768,0x69c7,0x66f9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0xa25f98a3,0xbf107811,0x52ef9edd,0x2e01c322,0x5768da9b,0x66f969c7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbf107811a25f98a3,0x2e01c32252ef9edd,0x66f969c75768da9b}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x72e5,0x9d9a,0xd825,0xa187,0x73ca,0xd025,0xc63e,0xf623,0x3bef,0x472e,0xdb8f,0x698f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x9d9a72e5,0xa187d825,0xd02573ca,0xf623c63e,0x472e3bef,0x698fdb8f}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xa187d8259d9a72e5,0xf623c63ed02573ca,0x698fdb8f472e3bef}}}
#endif
, &MAXORD_O0}};
const quat_alg_elem_t CONJUGATING_ELEMENTS[8] = {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x8015,0xfd5c,0xb508,0x1437,0xfa92,0x6222,0x1452,0xa79a,0x6c31,0xd3a9,0xb3c4,0x15c5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0xfd5c8015,0x1437b508,0x6222fa92,0xa79a1452,0xd3a96c31,0x15c5b3c4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x1437b508fd5c8015,0xa79a14526222fa92,0x15c5b3c4d3a96c31}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x8015,0xfd5c,0xb508,0x1437,0xfa92,0x6222,0x1452,0xa79a,0x6c31,0xd3a9,0xb3c4,0x15c5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0xfd5c8015,0x1437b508,0x6222fa92,0xa79a1452,0xd3a96c31,0x15c5b3c4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x1437b508fd5c8015,0xa79a14526222fa92,0x15c5b3c4d3a96c31}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xbcbf,0x76ed,0xc538,0xec53,0xeb88,0xb40d,0xa54e,0x14f,0x8bb2,0x300a,0xedb2,0x539}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x76edbcbf,0xec53c538,0xb40deb88,0x14fa54e,0x300a8bb2,0x539edb2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xec53c53876edbcbf,0x14fa54eb40deb88,0x539edb2300a8bb2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xbcbf,0x76ed,0xc538,0xec53,0xeb88,0xb40d,0xa54e,0x14f,0x8bb2,0x300a,0xedb2,0x539}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x76edbcbf,0xec53c538,0xb40deb88,0x14fa54e,0x300a8bb2,0x539edb2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xec53c53876edbcbf,0x14fa54eb40deb88,0x539edb2300a8bb2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x2341,0xb9df,0x4e77,0xcd8c,0x1cab,0xdb9d,0x8b8e,0x3e12,0x6370,0x7935,0x7217,0x987,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xb9df2341,0xcd8c4e77,0xdb9d1cab,0x3e128b8e,0x79356370,0x9877217,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xcd8c4e77b9df2341,0x3e128b8edb9d1cab,0x987721779356370,0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -13, ._mp_d = (mp_limb_t[]) {0x2341,0xb9df,0x4e77,0xcd8c,0x1cab,0xdb9d,0x8b8e,0x3e12,0x6370,0x7935,0x7217,0x987,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -7, ._mp_d = (mp_limb_t[]) {0xb9df2341,0xcd8c4e77,0xdb9d1cab,0x3e128b8e,0x79356370,0x9877217,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xcd8c4e77b9df2341,0x3e128b8edb9d1cab,0x987721779356370,0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0xefed,0x55e1,0x6a5c,0xbe0e,0x2393,0x3359,0x10ed,0xbce6,0xf1d1,0xaa9e,0xcdc6,0x5096}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x55e1efed,0xbe0e6a5c,0x33592393,0xbce610ed,0xaa9ef1d1,0x5096cdc6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xbe0e6a5c55e1efed,0xbce610ed33592393,0x5096cdc6aa9ef1d1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0xefed,0x55e1,0x6a5c,0xbe0e,0x2393,0x3359,0x10ed,0xbce6,0xf1d1,0xaa9e,0xcdc6,0x5096}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x55e1efed,0xbe0e6a5c,0x33592393,0xbce610ed,0xaa9ef1d1,0x5096cdc6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xbe0e6a5c55e1efed,0xbce610ed33592393,0x5096cdc6aa9ef1d1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x2fff,0x4caa,0xcd,0xcb73,0xeed1,0xde69,0x24f9,0x1a82,0xd96a,0xd42f,0xdc02,0x2822}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x4caa2fff,0xcb7300cd,0xde69eed1,0x1a8224f9,0xd42fd96a,0x2822dc02}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xcb7300cd4caa2fff,0x1a8224f9de69eed1,0x2822dc02d42fd96a}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x2fff,0x4caa,0xcd,0xcb73,0xeed1,0xde69,0x24f9,0x1a82,0xd96a,0xd42f,0xdc02,0x2822}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x4caa2fff,0xcb7300cd,0xde69eed1,0x1a8224f9,0xd42fd96a,0x2822dc02}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xcb7300cd4caa2fff,0x1a8224f9de69eed1,0x2822dc02d42fd96a}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 12, ._mp_d = (mp_limb_t[]) {0x3d93,0x1bd0,0xe46e,0xcbe9,0xbf97,0xeb5e,0x498,0xef9,0x148c,0xa51f,0x92c8,0x3773}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 6, ._mp_d = (mp_limb_t[]) {0x1bd03d93,0xcbe9e46e,0xeb5ebf97,0xef90498,0xa51f148c,0x377392c8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xcbe9e46e1bd03d93,0xef90498eb5ebf97,0x377392c8a51f148c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -12, ._mp_d = (mp_limb_t[]) {0x3d93,0x1bd0,0xe46e,0xcbe9,0xbf97,0xeb5e,0x498,0xef9,0x148c,0xa51f,0x92c8,0x3773}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -6, ._mp_d = (mp_limb_t[]) {0x1bd03d93,0xcbe9e46e,0xeb5ebf97,0xef90498,0xa51f148c,0x377392c8}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xcbe9e46e1bd03d93,0xef90498eb5ebf97,0x377392c8a51f148c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x4385,0xf091,0xe8e9,0xfaa3,0x7d60,0x8ab7,0x68b2,0x8a57,0x2754,0xa10c,0x6f20,0x71e4,0x4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xf0914385,0xfaa3e8e9,0x8ab77d60,0x8a5768b2,0xa10c2754,0x71e46f20,0x4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xfaa3e8e9f0914385,0x8a5768b28ab77d60,0x71e46f20a10c2754,0x4}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 13, ._mp_d = (mp_limb_t[]) {0x4385,0xf091,0xe8e9,0xfaa3,0x7d60,0x8ab7,0x68b2,0x8a57,0x2754,0xa10c,0x6f20,0x71e4,0x4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 7, ._mp_d = (mp_limb_t[]) {0xf0914385,0xfaa3e8e9,0x8ab77d60,0x8a5768b2,0xa10c2754,0x71e46f20,0x4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xfaa3e8e9f0914385,0x8a5768b28ab77d60,0x71e46f20a10c2754,0x4}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x9}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x9}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x9}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x9}}}
#endif
}}};
