#include <stddef.h>
#include <stdint.h>
#include <quaternion_data.h>
const ibz_t QUAT_prime_cofactor = 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x41,0x0,0x0,0x0,0x0,0x0,0x0,0x8000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x41,0x0,0x0,0x800000000000000}}}
#endif
;
const quat_alg_t QUATALG_PINFTY = {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x4ff}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x4ffffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0x4ffffffffffffff}}}
#endif
};
const quat_p_extremal_maximal_order_t EXTREMAL_ORDERS[7] = {{{
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x10000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x0,0x1000000000000000}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x10000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x0,0x1000000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x8000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x0,0x800000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x80000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x8000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x0,0x800000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x0,0x0,0x0,0x10000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x0,0x1000000000000000}}}
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
}}, 5}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x78d4,0x7b85,0x7a64,0xf5f2,0x29b9,0x3696,0x6101,0xb874}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7b8578d4,0xf5f27a64,0x369629b9,0xb8746101}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xf5f27a647b8578d4,0xb8746101369629b9}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x78d4,0x7b85,0x7a64,0xf5f2,0x29b9,0x3696,0x6101,0xb874}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7b8578d4,0xf5f27a64,0x369629b9,0xb8746101}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xf5f27a647b8578d4,0xb8746101369629b9}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbc6a,0x3dc2,0x3d32,0xfaf9,0x14dc,0x9b4b,0x3080,0x5c3a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3dc2bc6a,0xfaf93d32,0x9b4b14dc,0x5c3a3080}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xfaf93d323dc2bc6a,0x5c3a30809b4b14dc}}}
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
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x7d47,0x6fa4,0x2ad5,0x95ad,0x8a4b,0x49be,0x77e7,0xc898,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x6fa47d47,0x95ad2ad5,0x49be8a4b,0xc89877e7,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x95ad2ad56fa47d47,0xc89877e749be8a4b,0x1}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x3f47,0x7060,0x5e29,0x3e35,0xd950,0x2a1b,0x10ae,0x78dd,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x280}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x70603f47,0x3e355e29,0x2a1bd950,0x78dd10ae,0x0,0x0,0x0,0x2800000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3e355e2970603f47,0x78dd10ae2a1bd950,0x0,0x280000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbc6a,0x3dc2,0x3d32,0xfaf9,0x14dc,0x9b4b,0x3080,0x5c3a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3dc2bc6a,0xfaf93d32,0x9b4b14dc,0x5c3a3080}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xfaf93d323dc2bc6a,0x5c3a30809b4b14dc}}}
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
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x3fe7,0x28ee,0x26e8,0xb194,0x6d7a,0xaf58,0xe568,0xd6d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x28ee3fe7,0xb19426e8,0xaf586d7a,0xd6de568}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0xb19426e828ee3fe7,0xd6de568af586d7a}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x78d4,0x7b85,0x7a64,0xf5f2,0x29b9,0x3696,0x6101,0xb874}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7b8578d4,0xf5f27a64,0x369629b9,0xb8746101}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xf5f27a647b8578d4,0xb8746101369629b9}}}
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
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x7d47,0x6fa4,0x2ad5,0x95ad,0x8a4b,0x49be,0x77e7,0xc898,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x6fa47d47,0x95ad2ad5,0x49be8a4b,0xc89877e7,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x95ad2ad56fa47d47,0xc89877e749be8a4b,0x1}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe5e2,0x7715,0xa8e6,0x3c6f,0x9078,0x872b,0x9bec,0x1794}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7715e5e2,0x3c6fa8e6,0x872b9078,0x17949bec}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x3c6fa8e67715e5e2,0x17949bec872b9078}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe5e2,0x7715,0xa8e6,0x3c6f,0x9078,0x872b,0x9bec,0x1794}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7715e5e2,0x3c6fa8e6,0x872b9078,0x17949bec}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x3c6fa8e67715e5e2,0x17949bec872b9078}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf2f1,0x3b8a,0xd473,0x1e37,0xc83c,0x4395,0x4df6,0xbca}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3b8af2f1,0x1e37d473,0x4395c83c,0xbca4df6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1e37d4733b8af2f1,0xbca4df64395c83c}}}
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
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x307a,0x74c8,0x8082,0xb034,0x4e8a,0xc43a,0x399a,0x9ab}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x74c8307a,0xb0348082,0xc43a4e8a,0x9ab399a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0xb034808274c8307a,0x9ab399ac43a4e8a}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x954f,0x6bc9,0xca46,0x3d25,0x431b,0x46ed,0x8229,0x4f5,0xe453,0x6eb3,0x4530,0xeb3e,0x5306,0xb3e4,0x306e,0x45}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x6bc9954f,0x3d25ca46,0x46ed431b,0x4f58229,0x6eb3e453,0xeb3e4530,0xb3e45306,0x45306e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3d25ca466bc9954f,0x4f5822946ed431b,0xeb3e45306eb3e453,0x45306eb3e45306}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf2f1,0x3b8a,0xd473,0x1e37,0xc83c,0x4395,0x4df6,0xbca}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3b8af2f1,0x1e37d473,0x4395c83c,0xbca4df6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1e37d4733b8af2f1,0xbca4df64395c83c}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe7f,0xca3a,0x2454,0xbd31,0xe562,0xcb4c,0x72f0,0x21}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xca3a0e7f,0xbd312454,0xcb4ce562,0x2172f0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xbd312454ca3a0e7f,0x2172f0cb4ce562}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe5e2,0x7715,0xa8e6,0x3c6f,0x9078,0x872b,0x9bec,0x1794}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7715e5e2,0x3c6fa8e6,0x872b9078,0x17949bec}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x3c6fa8e67715e5e2,0x17949bec872b9078}}}
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
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x307a,0x74c8,0x8082,0xb034,0x4e8a,0xc43a,0x399a,0x9ab}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x74c8307a,0xb0348082,0xc43a4e8a,0x9ab399a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0xb034808274c8307a,0x9ab399ac43a4e8a}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
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
}}, 37}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xafa2,0x6dee,0xc511,0xde33,0xc8ce,0xc89e,0x4f97,0x2df9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6deeafa2,0xde33c511,0xc89ec8ce,0x2df94f97}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xde33c5116deeafa2,0x2df94f97c89ec8ce}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xafa2,0x6dee,0xc511,0xde33,0xc8ce,0xc89e,0x4f97,0x2df9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6deeafa2,0xde33c511,0xc89ec8ce,0x2df94f97}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xde33c5116deeafa2,0x2df94f97c89ec8ce}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x57d1,0xb6f7,0xe288,0x6f19,0x6467,0xe44f,0xa7cb,0x16fc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb6f757d1,0x6f19e288,0xe44f6467,0x16fca7cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x6f19e288b6f757d1,0x16fca7cbe44f6467}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdd36,0xda6b,0xa943,0xd17a,0xe307,0x564c,0x4b0c,0x44d4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xda6bdd36,0xd17aa943,0x564ce307,0x44d44b0c}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xd17aa943da6bdd36,0x44d44b0c564ce307}}}
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x3a03,0xc406,0x47c,0xa0a2,0x6dbc,0x1df4,0x796,0x6cee,0xce0c,0xe0c7,0xc7c,0xc7ce,0x7ce0,0xce0c,0xe0c7,0x7c}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xc4063a03,0xa0a2047c,0x1df46dbc,0x6cee0796,0xe0c7ce0c,0xc7ce0c7c,0xce0c7ce0,0x7ce0c7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xa0a2047cc4063a03,0x6cee07961df46dbc,0xc7ce0c7ce0c7ce0c,0x7ce0c7ce0c7ce0}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x57d1,0xb6f7,0xe288,0x6f19,0x6467,0xe44f,0xa7cb,0x16fc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb6f757d1,0x6f19e288,0xe44f6467,0x16fca7cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x6f19e288b6f757d1,0x16fca7cbe44f6467}}}
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
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x188f,0xa1e2,0x2148,0xd9f8,0x2e79,0x1a07,0xe1b2,0xd6}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xa1e2188f,0xd9f82148,0x1a072e79,0xd6e1b2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0xd9f82148a1e2188f,0xd6e1b21a072e79}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xafa2,0x6dee,0xc511,0xde33,0xc8ce,0xc89e,0x4f97,0x2df9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6deeafa2,0xde33c511,0xc89ec8ce,0x2df94f97}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xde33c5116deeafa2,0x2df94f97c89ec8ce}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdd36,0xda6b,0xa943,0xd17a,0xe307,0x564c,0x4b0c,0x44d4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xda6bdd36,0xd17aa943,0x564ce307,0x44d44b0c}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xd17aa943da6bdd36,0x44d44b0c564ce307}}}
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
}}, 41}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x96a4,0x25b,0x14f2,0x3800,0x4e7c,0x7958,0xab7f,0x7bbe,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x25b96a4,0x380014f2,0x79584e7c,0x7bbeab7f,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x380014f2025b96a4,0x7bbeab7f79584e7c,0x1}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x96a4,0x25b,0x14f2,0x3800,0x4e7c,0x7958,0xab7f,0x7bbe,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x25b96a4,0x380014f2,0x79584e7c,0x7bbeab7f,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x380014f2025b96a4,0x7bbeab7f79584e7c,0x1}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xcb52,0x12d,0xa79,0x1c00,0x273e,0xbcac,0x55bf,0xbddf}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x12dcb52,0x1c000a79,0xbcac273e,0xbddf55bf}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1c000a79012dcb52,0xbddf55bfbcac273e}}}
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
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0x73e3,0x3339,0x19e7,0x4ba1,0x6ebc,0x2702,0xee62,0xdbd0,0x7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0x333973e3,0x4ba119e7,0x27026ebc,0xdbd0ee62,0x7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x4ba119e7333973e3,0xdbd0ee6227026ebc,0x7}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xca33,0x3dd0,0x1d92,0x9f0,0x2f81,0xafe9,0xe395,0x83f7,0xfffc,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x27f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x3dd0ca33,0x9f01d92,0xafe92f81,0x83f7e395,0xfffffffc,0xffffffff,0xffffffff,0x27fffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x9f01d923dd0ca33,0x83f7e395afe92f81,0xfffffffffffffffc,0x27fffffffffffff}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xcb52,0x12d,0xa79,0x1c00,0x273e,0xbcac,0x55bf,0xbddf}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x12dcb52,0x1c000a79,0xbcac273e,0xbddf55bf}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1c000a79012dcb52,0xbddf55bfbcac273e}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x35}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x35}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x35}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xeb73,0xf93c,0x71c0,0x87f5,0x667a,0xcb3c,0xb9cb,0x12fa}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf93ceb73,0x87f571c0,0xcb3c667a,0x12fab9cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x87f571c0f93ceb73,0x12fab9cbcb3c667a}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x96a4,0x25b,0x14f2,0x3800,0x4e7c,0x7958,0xab7f,0x7bbe,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x25b96a4,0x380014f2,0x79584e7c,0x7bbeab7f,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0x380014f2025b96a4,0x7bbeab7f79584e7c,0x1}}}
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
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0x73e3,0x3339,0x19e7,0x4ba1,0x6ebc,0x2702,0xee62,0xdbd0,0x7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0x333973e3,0x4ba119e7,0x27026ebc,0xdbd0ee62,0x7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0x4ba119e7333973e3,0xdbd0ee6227026ebc,0x7}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x35}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x35}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x35}}}
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
}}, 53}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x7ffa,0x55af,0x7b9e,0xe2b9,0xa7af,0x578c,0xf76b,0xc227,0xf}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x55af7ffa,0xe2b97b9e,0x578ca7af,0xc227f76b,0xf}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xe2b97b9e55af7ffa,0xc227f76b578ca7af,0xf}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x7ffa,0x55af,0x7b9e,0xe2b9,0xa7af,0x578c,0xf76b,0xc227,0xf}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x55af7ffa,0xe2b97b9e,0x578ca7af,0xc227f76b,0xf}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xe2b97b9e55af7ffa,0xc227f76b578ca7af,0xf}}}
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
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xbffd,0x2ad7,0xbdcf,0xf15c,0x53d7,0xabc6,0xfbb5,0xe113,0x7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2ad7bffd,0xf15cbdcf,0xabc653d7,0xe113fbb5,0x7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xf15cbdcf2ad7bffd,0xe113fbb5abc653d7,0x7}}}
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
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0xd16,0xf02b,0x1ce7,0xa2ef,0x54b,0x2c56,0x5963,0x667,0x6f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0xf02b0d16,0xa2ef1ce7,0x2c56054b,0x6675963,0x6f}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xa2ef1ce7f02b0d16,0x66759632c56054b,0x6f}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xf0ab,0x9d3b,0x6ea,0x84ac,0x62e5,0xdde9,0x882b,0xd021,0xffe2,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x13ff}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x9d3bf0ab,0x84ac06ea,0xdde962e5,0xd021882b,0xffffffe2,0xffffffff,0xffffffff,0x13ffffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x84ac06ea9d3bf0ab,0xd021882bdde962e5,0xffffffffffffffe2,0x13ffffffffffffff}}}
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
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xbffd,0x2ad7,0xbdcf,0xf15c,0x53d7,0xabc6,0xfbb5,0xe113,0x7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2ad7bffd,0xf15cbdcf,0xabc653d7,0xe113fbb5,0x7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xf15cbdcf2ad7bffd,0xe113fbb5abc653d7,0x7}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x308}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x308}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x308}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1f37,0x5c4a,0x13f1,0x770,0x7183,0x5600,0xda31,0x9281}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5c4a1f37,0x77013f1,0x56007183,0x9281da31}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x77013f15c4a1f37,0x9281da3156007183}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x7ffa,0x55af,0x7b9e,0xe2b9,0xa7af,0x578c,0xf76b,0xc227,0xf}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x55af7ffa,0xe2b97b9e,0x578ca7af,0xc227f76b,0xf}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 3, ._mp_d = (mp_limb_t[]) {0xe2b97b9e55af7ffa,0xc227f76b578ca7af,0xf}}}
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
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0xd16,0xf02b,0x1ce7,0xa2ef,0x54b,0x2c56,0x5963,0x667,0x6f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0xf02b0d16,0xa2ef1ce7,0x2c56054b,0x6675963,0x6f}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -3, ._mp_d = (mp_limb_t[]) {0xa2ef1ce7f02b0d16,0x66759632c56054b,0x6f}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x308}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x308}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x308}}}
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
const quat_left_ideal_t CONNECTING_IDEALS[7] = {{{
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x6000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x2,0x0,0x0,0x60000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x2,0x6000000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x10000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1,0x1000000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x6000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x2,0x0,0x0,0x60000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x2,0x6000000000000000}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x5000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x50000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1,0x5000000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x3000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x30000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1,0x3000000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf5fe,0x8673,0x157b,0x7f90,0xd2c5,0xd00b,0xa646,0x78f4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x8673f5fe,0x7f90157b,0xd00bd2c5,0x78f4a646}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x7f90157b8673f5fe,0x78f4a646d00bd2c5}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xfee5,0x2b,0xd6d8,0xe65c,0x68a3,0xe72d,0x373d,0x5b1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x2bfee5,0xe65cd6d8,0xe72d68a3,0x5b1373d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xe65cd6d8002bfee5,0x5b1373de72d68a3}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf5fe,0x8673,0x157b,0x7f90,0xd2c5,0xd00b,0xa646,0x78f4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x8673f5fe,0x7f90157b,0xd00bd2c5,0x78f4a646}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x7f90157b8673f5fe,0x78f4a646d00bd2c5}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf719,0x8647,0x3ea3,0x9933,0x6a21,0xe8de,0x6f08,0x7343}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x8647f719,0x99333ea3,0xe8de6a21,0x73436f08}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x99333ea38647f719,0x73436f08e8de6a21}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xfaff,0xc339,0xabd,0xbfc8,0xe962,0x6805,0x5323,0x3c7a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc339faff,0xbfc80abd,0x6805e962,0x3c7a5323}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xbfc80abdc339faff,0x3c7a53236805e962}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe5e2,0x7715,0xa8e6,0x3c6f,0x9078,0x872b,0x9bec,0x1794}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7715e5e2,0x3c6fa8e6,0x872b9078,0x17949bec}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x3c6fa8e67715e5e2,0x17949bec872b9078}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x8597,0x3af7,0xa5a,0xbb29,0x77c0,0xd2d9,0xf561,0x84f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3af78597,0xbb290a5a,0xd2d977c0,0x84ff561}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xbb290a5a3af78597,0x84ff561d2d977c0}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe5e2,0x7715,0xa8e6,0x3c6f,0x9078,0x872b,0x9bec,0x1794}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7715e5e2,0x3c6fa8e6,0x872b9078,0x17949bec}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x3c6fa8e67715e5e2,0x17949bec872b9078}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x604b,0x3c1e,0x9e8c,0x8146,0x18b7,0xb452,0xa68a,0xf44}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3c1e604b,0x81469e8c,0xb45218b7,0xf44a68a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x81469e8c3c1e604b,0xf44a68ab45218b7}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf2f1,0x3b8a,0xd473,0x1e37,0xc83c,0x4395,0x4df6,0xbca}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3b8af2f1,0x1e37d473,0x4395c83c,0xbca4df6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1e37d4733b8af2f1,0xbca4df64395c83c}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xafa2,0x6dee,0xc511,0xde33,0xc8ce,0xc89e,0x4f97,0x2df9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6deeafa2,0xde33c511,0xc89ec8ce,0x2df94f97}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xde33c5116deeafa2,0x2df94f97c89ec8ce}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x519b,0xa90b,0xcdca,0xd5f5,0x757a,0x83dd,0xb354,0xe59}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa90b519b,0xd5f5cdca,0x83dd757a,0xe59b354}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xd5f5cdcaa90b519b,0xe59b35483dd757a}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xafa2,0x6dee,0xc511,0xde33,0xc8ce,0xc89e,0x4f97,0x2df9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6deeafa2,0xde33c511,0xc89ec8ce,0x2df94f97}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xde33c5116deeafa2,0x2df94f97c89ec8ce}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x5e07,0xc4e3,0xf746,0x83d,0x5354,0x44c1,0x9c43,0x1f9f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc4e35e07,0x83df746,0x44c15354,0x1f9f9c43}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x83df746c4e35e07,0x1f9f9c4344c15354}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x57d1,0xb6f7,0xe288,0x6f19,0x6467,0xe44f,0xa7cb,0x16fc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb6f757d1,0x6f19e288,0xe44f6467,0x16fca7cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x6f19e288b6f757d1,0x16fca7cbe44f6467}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x19f2,0x5594,0xee77,0x52a2,0xf459,0x45c9,0x2187,0xb348}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x559419f2,0x52a2ee77,0x45c9f459,0xb3482187}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x52a2ee77559419f2,0xb348218745c9f459}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdbd3,0x967a,0x8a96,0x1df4,0x7845,0xd70,0x419a,0x222}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x967adbd3,0x1df48a96,0xd707845,0x222419a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1df48a96967adbd3,0x222419a0d707845}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x19f2,0x5594,0xee77,0x52a2,0xf459,0x45c9,0x2187,0xb348}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x559419f2,0x52a2ee77,0x45c9f459,0xb3482187}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x52a2ee77559419f2,0xb348218745c9f459}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x3e1f,0xbf19,0x63e0,0x34ae,0x7c14,0x3859,0xdfed,0xb125}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xbf193e1f,0x34ae63e0,0x38597c14,0xb125dfed}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x34ae63e0bf193e1f,0xb125dfed38597c14}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xcf9,0xaaca,0x773b,0xa951,0xfa2c,0xa2e4,0x10c3,0x59a4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xaaca0cf9,0xa951773b,0xa2e4fa2c,0x59a410c3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xa951773baaca0cf9,0x59a410c3a2e4fa2c}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xaa3a,0x67cf,0x6ad7,0xd031,0x701,0xebca,0xd852,0x2996}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x67cfaa3a,0xd0316ad7,0xebca0701,0x2996d852}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xd0316ad767cfaa3a,0x2996d852ebca0701}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x275,0xd7ab,0xedeb,0xbc67,0xad41,0xaeb5,0xf2e5,0x148e}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xd7ab0275,0xbc67edeb,0xaeb5ad41,0x148ef2e5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xbc67edebd7ab0275,0x148ef2e5aeb5ad41}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xaa3a,0x67cf,0x6ad7,0xd031,0x701,0xebca,0xd852,0x2996}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x67cfaa3a,0xd0316ad7,0xebca0701,0x2996d852}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xd0316ad767cfaa3a,0x2996d852ebca0701}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xa7c5,0x9024,0x7ceb,0x13c9,0x59c0,0x3d14,0xe56d,0x1507}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x9024a7c5,0x13c97ceb,0x3d1459c0,0x1507e56d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x13c97ceb9024a7c5,0x1507e56d3d1459c0}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xd51d,0xb3e7,0xb56b,0xe818,0x380,0x75e5,0x6c29,0x14cb}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb3e7d51d,0xe818b56b,0x75e50380,0x14cb6c29}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xe818b56bb3e7d51d,0x14cb6c2975e50380}}}
#endif
, &MAXORD_O0}};
const quat_alg_elem_t CONJUGATING_ELEMENTS[7] = {{
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x10000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1,0x1000000000000000}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1000}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x1,0x0,0x0,0x10000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0x1,0x1000000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf94f,0x85ef,0x90f3,0xcc79,0x98d9,0x1a83,0x8d,0x67e1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x85eff94f,0xcc7990f3,0x1a8398d9,0x67e1008d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xcc7990f385eff94f,0x67e1008d1a8398d9}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xf94f,0x85ef,0x90f3,0xcc79,0x98d9,0x1a83,0x8d,0x67e1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x85eff94f,0xcc7990f3,0x1a8398d9,0x67e1008d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0xcc7990f385eff94f,0x67e1008d1a8398d9}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x3}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x3}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf2f1,0x3b8a,0xd473,0x1e37,0xc83c,0x4395,0x4df6,0xbca}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3b8af2f1,0x1e37d473,0x4395c83c,0xbca4df6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x1e37d4733b8af2f1,0xbca4df64395c83c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xf2f1,0x3b8a,0xd473,0x1e37,0xc83c,0x4395,0x4df6,0xbca}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x3b8af2f1,0x1e37d473,0x4395c83c,0xbca4df6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0x1e37d4733b8af2f1,0xbca4df64395c83c}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x57d1,0xb6f7,0xe288,0x6f19,0x6467,0xe44f,0xa7cb,0x16fc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb6f757d1,0x6f19e288,0xe44f6467,0x16fca7cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x6f19e288b6f757d1,0x16fca7cbe44f6467}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x57d1,0xb6f7,0xe288,0x6f19,0x6467,0xe44f,0xa7cb,0x16fc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xb6f757d1,0x6f19e288,0xe44f6467,0x16fca7cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0x6f19e288b6f757d1,0x16fca7cbe44f6467}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x6511,0x45fa,0xa368,0xe869,0x4db2,0x88fc,0x6989,0xbdf3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x45fa6511,0xe869a368,0x88fc4db2,0xbdf36989}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0xe869a36845fa6511,0xbdf3698988fc4db2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x6511,0x45fa,0xa368,0xe869,0x4db2,0x88fc,0x6989,0xbdf3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x45fa6511,0xe869a368,0x88fc4db2,0xbdf36989}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0xe869a36845fa6511,0xbdf3698988fc4db2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x5}}}
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
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1e6b,0x5c4a,0x13f1,0x770,0x7183,0x5600,0xda31,0x9281}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5c4a1e6b,0x77013f1,0x56007183,0x9281da31}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 2, ._mp_d = (mp_limb_t[]) {0x77013f15c4a1e6b,0x9281da3156007183}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x1e6b,0x5c4a,0x13f1,0x770,0x7183,0x5600,0xda31,0x9281}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x5c4a1e6b,0x77013f1,0x56007183,0x9281da31}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -2, ._mp_d = (mp_limb_t[]) {0x77013f15c4a1e6b,0x9281da3156007183}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x4}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x4}}}
#endif
}}};
