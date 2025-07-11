#include <stddef.h>
#include <stdint.h>
#include <quaternion_data.h>
const ibz_t QUAT_prime_cofactor = 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 32, ._mp_d = (mp_limb_t[]) {0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2000000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x200000000000000}}}
#endif
;
const quat_alg_t QUATALG_PINFTY = {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 32, ._mp_d = (mp_limb_t[]) {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x1af}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x1afffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0x1afffffffffffff}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc584,0x10dc,0x9924,0xb9c2,0x5a67,0xfbc8,0xbdfb,0xe941,0xc61,0xdee2,0xcd5c,0xc570,0xa2d4,0xb3ff,0x2e51,0x4927}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x10dcc584,0xb9c29924,0xfbc85a67,0xe941bdfb,0xdee20c61,0xc570cd5c,0xb3ffa2d4,0x49272e51}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb9c2992410dcc584,0xe941bdfbfbc85a67,0xc570cd5cdee20c61,0x49272e51b3ffa2d4}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc584,0x10dc,0x9924,0xb9c2,0x5a67,0xfbc8,0xbdfb,0xe941,0xc61,0xdee2,0xcd5c,0xc570,0xa2d4,0xb3ff,0x2e51,0x4927}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x10dcc584,0xb9c29924,0xfbc85a67,0xe941bdfb,0xdee20c61,0xc570cd5c,0xb3ffa2d4,0x49272e51}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb9c2992410dcc584,0xe941bdfbfbc85a67,0xc570cd5cdee20c61,0x49272e51b3ffa2d4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x62c2,0x86e,0x4c92,0xdce1,0x2d33,0xfde4,0xdefd,0xf4a0,0x630,0x6f71,0x66ae,0x62b8,0xd16a,0xd9ff,0x9728,0x2493}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x86e62c2,0xdce14c92,0xfde42d33,0xf4a0defd,0x6f710630,0x62b866ae,0xd9ffd16a,0x24939728}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdce14c92086e62c2,0xf4a0defdfde42d33,0x62b866ae6f710630,0x24939728d9ffd16a}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xc93,0xd634,0x4632,0x353f,0x76ba,0x220d,0x5084,0x3187,0xb121,0xfc8,0x6860,0xa3e4,0x1368,0x7388,0x5e0,0x7e52}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xd6340c93,0x353f4632,0x220d76ba,0x31875084,0xfc8b121,0xa3e46860,0x73881368,0x7e5205e0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x353f4632d6340c93,0x31875084220d76ba,0xa3e468600fc8b121,0x7e5205e073881368}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 32, ._mp_d = (mp_limb_t[]) {0x2f6d,0xbfbd,0x6af0,0xbcd3,0x5c61,0x8f62,0x9b0b,0xd78a,0x3142,0x61aa,0x4716,0x208,0x93c7,0x43bd,0x97d6,0xda1a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xbfbd2f6d,0xbcd36af0,0x8f625c61,0xd78a9b0b,0x61aa3142,0x2084716,0x43bd93c7,0xda1a97d6,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xd7ffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbcd36af0bfbd2f6d,0xd78a9b0b8f625c61,0x208471661aa3142,0xda1a97d643bd93c7,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xd7ffffffffffff}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x62c2,0x86e,0x4c92,0xdce1,0x2d33,0xfde4,0xdefd,0xf4a0,0x630,0x6f71,0x66ae,0x62b8,0xd16a,0xd9ff,0x9728,0x2493}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x86e62c2,0xdce14c92,0xfde42d33,0xf4a0defd,0x6f710630,0x62b866ae,0xd9ffd16a,0x24939728}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdce14c92086e62c2,0xf4a0defdfde42d33,0x62b866ae6f710630,0x24939728d9ffd16a}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x9add,0x156b,0x8705,0x6bb9,0x8bdf,0xd034,0x21a6,0xb827,0x44e9,0x34c7,0x3da3,0xa9fd,0xcebd,0x3ec0,0xcd63,0xca1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x156b9add,0x6bb98705,0xd0348bdf,0xb82721a6,0x34c744e9,0xa9fd3da3,0x3ec0cebd,0xca1cd63}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6bb98705156b9add,0xb82721a6d0348bdf,0xa9fd3da334c744e9,0xca1cd633ec0cebd}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc584,0x10dc,0x9924,0xb9c2,0x5a67,0xfbc8,0xbdfb,0xe941,0xc61,0xdee2,0xcd5c,0xc570,0xa2d4,0xb3ff,0x2e51,0x4927}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x10dcc584,0xb9c29924,0xfbc85a67,0xe941bdfb,0xdee20c61,0xc570cd5c,0xb3ffa2d4,0x49272e51}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb9c2992410dcc584,0xe941bdfbfbc85a67,0xc570cd5cdee20c61,0x49272e51b3ffa2d4}}}
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xc93,0xd634,0x4632,0x353f,0x76ba,0x220d,0x5084,0x3187,0xb121,0xfc8,0x6860,0xa3e4,0x1368,0x7388,0x5e0,0x7e52}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xd6340c93,0x353f4632,0x220d76ba,0x31875084,0xfc8b121,0xa3e46860,0x73881368,0x7e5205e0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x353f4632d6340c93,0x31875084220d76ba,0xa3e468600fc8b121,0x7e5205e073881368}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x5}}}
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x169a,0xd34,0xbfd7,0x4089,0x1be8,0xc17d,0xfefb,0x6efd,0x76e0,0x58ea,0xfedb,0xd204,0x1e3,0x3b9a,0x47d6,0xdf28,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xd34169a,0x4089bfd7,0xc17d1be8,0x6efdfefb,0x58ea76e0,0xd204fedb,0x3b9a01e3,0xdf2847d6,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x4089bfd70d34169a,0x6efdfefbc17d1be8,0xd204fedb58ea76e0,0xdf2847d63b9a01e3,0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x169a,0xd34,0xbfd7,0x4089,0x1be8,0xc17d,0xfefb,0x6efd,0x76e0,0x58ea,0xfedb,0xd204,0x1e3,0x3b9a,0x47d6,0xdf28,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xd34169a,0x4089bfd7,0xc17d1be8,0x6efdfefb,0x58ea76e0,0xd204fedb,0x3b9a01e3,0xdf2847d6,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x4089bfd70d34169a,0x6efdfefbc17d1be8,0xd204fedb58ea76e0,0xdf2847d63b9a01e3,0x2}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xb4d,0x869a,0xdfeb,0x2044,0x8df4,0xe0be,0xff7d,0x377e,0x3b70,0xac75,0x7f6d,0xe902,0xf1,0x1dcd,0x23eb,0x6f94,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x869a0b4d,0x2044dfeb,0xe0be8df4,0x377eff7d,0xac753b70,0xe9027f6d,0x1dcd00f1,0x6f9423eb,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2044dfeb869a0b4d,0x377eff7de0be8df4,0xe9027f6dac753b70,0x6f9423eb1dcd00f1,0x1}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -17, ._mp_d = (mp_limb_t[]) {0x2412,0x6ec4,0x4dda,0x2cde,0x281d,0xaaa7,0x3a33,0xc1d6,0x5c26,0x22e3,0x816d,0x13fb,0xac81,0x58e8,0xd1a7,0xadaa,0xc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0x6ec42412,0x2cde4dda,0xaaa7281d,0xc1d63a33,0x22e35c26,0x13fb816d,0x58e8ac81,0xadaad1a7,0xc}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0x2cde4dda6ec42412,0xc1d63a33aaa7281d,0x13fb816d22e35c26,0xadaad1a758e8ac81,0xc}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 32, ._mp_d = (mp_limb_t[]) {0x1f45,0x5630,0xd526,0x9cc7,0x1aab,0x114d,0x87b3,0xbb27,0xc6b6,0xe50,0x8bb4,0x813f,0xff7a,0xf810,0xa8d3,0x66ee,0xfffc,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x35f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x56301f45,0x9cc7d526,0x114d1aab,0xbb2787b3,0xe50c6b6,0x813f8bb4,0xf810ff7a,0x66eea8d3,0xfffffffc,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x35fffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x9cc7d52656301f45,0xbb2787b3114d1aab,0x813f8bb40e50c6b6,0x66eea8d3f810ff7a,0xfffffffffffffffc,0xffffffffffffffff,0xffffffffffffffff,0x35fffffffffffff}}}
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xb4d,0x869a,0xdfeb,0x2044,0x8df4,0xe0be,0xff7d,0x377e,0x3b70,0xac75,0x7f6d,0xe902,0xf1,0x1dcd,0x23eb,0x6f94,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x869a0b4d,0x2044dfeb,0xe0be8df4,0x377eff7d,0xac753b70,0xe9027f6d,0x1dcd00f1,0x6f9423eb,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2044dfeb869a0b4d,0x377eff7de0be8df4,0xe9027f6dac753b70,0x6f9423eb1dcd00f1,0x1}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x94}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x94}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x94}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x233f,0x38d9,0x6fc1,0xa333,0xaeb,0xce6a,0x2a4c,0xa1c1,0x274c,0xa9fc,0xd4c6,0xb0b3,0x555b,0x7a48,0x411a,0x2bdc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x38d9233f,0xa3336fc1,0xce6a0aeb,0xa1c12a4c,0xa9fc274c,0xb0b3d4c6,0x7a48555b,0x2bdc411a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa3336fc138d9233f,0xa1c12a4cce6a0aeb,0xb0b3d4c6a9fc274c,0x2bdc411a7a48555b}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x169a,0xd34,0xbfd7,0x4089,0x1be8,0xc17d,0xfefb,0x6efd,0x76e0,0x58ea,0xfedb,0xd204,0x1e3,0x3b9a,0x47d6,0xdf28,0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xd34169a,0x4089bfd7,0xc17d1be8,0x6efdfefb,0x58ea76e0,0xd204fedb,0x3b9a01e3,0xdf2847d6,0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x4089bfd70d34169a,0x6efdfefbc17d1be8,0xd204fedb58ea76e0,0xdf2847d63b9a01e3,0x2}}}
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
{{._mp_alloc = 0, ._mp_size = -17, ._mp_d = (mp_limb_t[]) {0x2412,0x6ec4,0x4dda,0x2cde,0x281d,0xaaa7,0x3a33,0xc1d6,0x5c26,0x22e3,0x816d,0x13fb,0xac81,0x58e8,0xd1a7,0xadaa,0xc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0x6ec42412,0x2cde4dda,0xaaa7281d,0xc1d63a33,0x22e35c26,0x13fb816d,0x58e8ac81,0xadaad1a7,0xc}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0x2cde4dda6ec42412,0xc1d63a33aaa7281d,0x13fb816d22e35c26,0xadaad1a758e8ac81,0xc}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x94}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x94}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x94}}}
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xc31e,0x416e,0xe2a5,0x25c2,0xa926,0x45fa,0x578b,0xf395,0x6ac9,0x7901,0xd14d,0xd089,0x9e5c,0xe062,0xbc4,0x583b,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x416ec31e,0x25c2e2a5,0x45faa926,0xf395578b,0x79016ac9,0xd089d14d,0xe0629e5c,0x583b0bc4,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x25c2e2a5416ec31e,0xf395578b45faa926,0xd089d14d79016ac9,0x583b0bc4e0629e5c,0x3}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xc31e,0x416e,0xe2a5,0x25c2,0xa926,0x45fa,0x578b,0xf395,0x6ac9,0x7901,0xd14d,0xd089,0x9e5c,0xe062,0xbc4,0x583b,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x416ec31e,0x25c2e2a5,0x45faa926,0xf395578b,0x79016ac9,0xd089d14d,0xe0629e5c,0x583b0bc4,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x25c2e2a5416ec31e,0xf395578b45faa926,0xd089d14d79016ac9,0x583b0bc4e0629e5c,0x3}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x618f,0xa0b7,0x7152,0x12e1,0x5493,0xa2fd,0xabc5,0xf9ca,0xb564,0xbc80,0xe8a6,0x6844,0x4f2e,0x7031,0x85e2,0xac1d,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xa0b7618f,0x12e17152,0xa2fd5493,0xf9caabc5,0xbc80b564,0x6844e8a6,0x70314f2e,0xac1d85e2,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x12e17152a0b7618f,0xf9caabc5a2fd5493,0x6844e8a6bc80b564,0xac1d85e270314f2e,0x1}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x4c2,0x1467,0x5829,0xf8ca,0x2d31,0xb661,0xa4a1,0x434a,0x25cb,0xbffa,0xe232,0x8565,0x2305,0xe42,0x3f64,0x710,0x11}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x146704c2,0xf8ca5829,0xb6612d31,0x434aa4a1,0xbffa25cb,0x8565e232,0xe422305,0x7103f64,0x11}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0xf8ca5829146704c2,0x434aa4a1b6612d31,0x8565e232bffa25cb,0x7103f640e422305,0x11}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -32, ._mp_d = (mp_limb_t[]) {0x3b03,0xe541,0x6454,0x6f9,0x3808,0xb93,0x7509,0x2b52,0xed1,0xf4fe,0x8961,0x4869,0x4671,0xdd21,0x4c4c,0x70b0,0xfff9,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x35f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xe5413b03,0x6f96454,0xb933808,0x2b527509,0xf4fe0ed1,0x48698961,0xdd214671,0x70b04c4c,0xfffffff9,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0x35fffff}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x6f96454e5413b03,0x2b5275090b933808,0x48698961f4fe0ed1,0x70b04c4cdd214671,0xfffffffffffffff9,0xffffffffffffffff,0xffffffffffffffff,0x35fffffffffffff}}}
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x618f,0xa0b7,0x7152,0x12e1,0x5493,0xa2fd,0xabc5,0xf9ca,0xb564,0xbc80,0xe8a6,0x6844,0x4f2e,0x7031,0x85e2,0xac1d,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xa0b7618f,0x12e17152,0xa2fd5493,0xf9caabc5,0xbc80b564,0x6844e8a6,0x70314f2e,0xac1d85e2,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x12e17152a0b7618f,0xf9caabc5a2fd5493,0x6844e8a6bc80b564,0xac1d85e270314f2e,0x1}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0xf4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0xf4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0xf4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xe953,0xf5ac,0x2ee2,0xc962,0x459d,0xd9a0,0xd761,0x7c5a,0x8268,0xcf36,0x9b08,0xb7a6,0xbd23,0x7e04,0xe324,0x23ba}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xf5ace953,0xc9622ee2,0xd9a0459d,0x7c5ad761,0xcf368268,0xb7a69b08,0x7e04bd23,0x23bae324}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xc9622ee2f5ace953,0x7c5ad761d9a0459d,0xb7a69b08cf368268,0x23bae3247e04bd23}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xc31e,0x416e,0xe2a5,0x25c2,0xa926,0x45fa,0x578b,0xf395,0x6ac9,0x7901,0xd14d,0xd089,0x9e5c,0xe062,0xbc4,0x583b,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x416ec31e,0x25c2e2a5,0x45faa926,0xf395578b,0x79016ac9,0xd089d14d,0xe0629e5c,0x583b0bc4,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x25c2e2a5416ec31e,0xf395578b45faa926,0xd089d14d79016ac9,0x583b0bc4e0629e5c,0x3}}}
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x4c2,0x1467,0x5829,0xf8ca,0x2d31,0xb661,0xa4a1,0x434a,0x25cb,0xbffa,0xe232,0x8565,0x2305,0xe42,0x3f64,0x710,0x11}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x146704c2,0xf8ca5829,0xb6612d31,0x434aa4a1,0xbffa25cb,0x8565e232,0xe422305,0x7103f64,0x11}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0xf8ca5829146704c2,0x434aa4a1b6612d31,0x8565e232bffa25cb,0x7103f640e422305,0x11}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0xf4}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0xf4}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0xf4}}}
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
}}, 61}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xf94,0x28f0,0x8955,0xb7aa,0x538f,0x65b6,0x5a15,0xd3b4,0x9667,0xcba4,0x3554,0x11f8,0x25a9,0x6265,0x196a,0x83d,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x28f00f94,0xb7aa8955,0x65b6538f,0xd3b45a15,0xcba49667,0x11f83554,0x626525a9,0x83d196a,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0xb7aa895528f00f94,0xd3b45a1565b6538f,0x11f83554cba49667,0x83d196a626525a9,0x1}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xf94,0x28f0,0x8955,0xb7aa,0x538f,0x65b6,0x5a15,0xd3b4,0x9667,0xcba4,0x3554,0x11f8,0x25a9,0x6265,0x196a,0x83d,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x28f00f94,0xb7aa8955,0x65b6538f,0xd3b45a15,0xcba49667,0x11f83554,0x626525a9,0x83d196a,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0xb7aa895528f00f94,0xd3b45a1565b6538f,0x11f83554cba49667,0x83d196a626525a9,0x1}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x7ca,0x9478,0x44aa,0xdbd5,0x29c7,0xb2db,0x2d0a,0xe9da,0x4b33,0x65d2,0x1aaa,0x88fc,0x92d4,0x3132,0x8cb5,0x841e}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x947807ca,0xdbd544aa,0xb2db29c7,0xe9da2d0a,0x65d24b33,0x88fc1aaa,0x313292d4,0x841e8cb5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdbd544aa947807ca,0xe9da2d0ab2db29c7,0x88fc1aaa65d24b33,0x841e8cb5313292d4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -17, ._mp_d = (mp_limb_t[]) {0x5409,0xed37,0x7339,0xcbe7,0x95ab,0x2582,0x18fa,0xcc9d,0x13ae,0x2543,0xaefd,0xb168,0xfdbc,0x68b,0xc9af,0x6d9d,0x6}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0xed375409,0xcbe77339,0x258295ab,0xcc9d18fa,0x254313ae,0xb168aefd,0x68bfdbc,0x6d9dc9af,0x6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0xcbe77339ed375409,0xcc9d18fa258295ab,0xb168aefd254313ae,0x6d9dc9af068bfdbc,0x6}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -32, ._mp_d = (mp_limb_t[]) {0x7013,0x423f,0x42b7,0x3f3d,0x82a,0x9883,0x52bf,0xfede,0x8018,0xa449,0xf571,0xb8a,0x3139,0xbe7,0x439d,0x9e1f,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xd8}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x423f7013,0x3f3d42b7,0x9883082a,0xfede52bf,0xa4498018,0xb8af571,0xbe73139,0x9e1f439d,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0xd80000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x3f3d42b7423f7013,0xfede52bf9883082a,0xb8af571a4498018,0x9e1f439d0be73139,0x2,0x0,0x0,0xd8000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x7ca,0x9478,0x44aa,0xdbd5,0x29c7,0xb2db,0x2d0a,0xe9da,0x4b33,0x65d2,0x1aaa,0x88fc,0x92d4,0x3132,0x8cb5,0x841e}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x947807ca,0xdbd544aa,0xb2db29c7,0xe9da2d0a,0x65d24b33,0x88fc1aaa,0x313292d4,0x841e8cb5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdbd544aa947807ca,0xe9da2d0ab2db29c7,0x88fc1aaa65d24b33,0x841e8cb5313292d4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xca2d,0x34af,0xea29,0x177b,0x91ed,0x86ca,0x588a,0xe94d,0x55df,0x4621,0xa1e4,0x67d7,0xb617,0x6a1,0x88f5,0x87b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x34afca2d,0x177bea29,0x86ca91ed,0xe94d588a,0x462155df,0x67d7a1e4,0x6a1b617,0x87b88f5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x177bea2934afca2d,0xe94d588a86ca91ed,0x67d7a1e4462155df,0x87b88f506a1b617}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xf94,0x28f0,0x8955,0xb7aa,0x538f,0x65b6,0x5a15,0xd3b4,0x9667,0xcba4,0x3554,0x11f8,0x25a9,0x6265,0x196a,0x83d,0x1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0x28f00f94,0xb7aa8955,0x65b6538f,0xd3b45a15,0xcba49667,0x11f83554,0x626525a9,0x83d196a,0x1}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0xb7aa895528f00f94,0xd3b45a1565b6538f,0x11f83554cba49667,0x83d196a626525a9,0x1}}}
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
{{._mp_alloc = 0, ._mp_size = -17, ._mp_d = (mp_limb_t[]) {0x5409,0xed37,0x7339,0xcbe7,0x95ab,0x2582,0x18fa,0xcc9d,0x13ae,0x2543,0xaefd,0xb168,0xfdbc,0x68b,0xc9af,0x6d9d,0x6}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0xed375409,0xcbe77339,0x258295ab,0xcc9d18fa,0x254313ae,0xb168aefd,0x68bfdbc,0x6d9dc9af,0x6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0xcbe77339ed375409,0xcc9d18fa258295ab,0xb168aefd254313ae,0x6d9dc9af068bfdbc,0x6}}}
#endif
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
}}, 97}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x1e34,0xdf45,0xcfa6,0xa203,0xe18a,0xd8c5,0x1d42,0x7a30,0x86ce,0x7a12,0xf933,0x741d,0x6b48,0xe48e,0xbf65,0x1854}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdf451e34,0xa203cfa6,0xd8c5e18a,0x7a301d42,0x7a1286ce,0x741df933,0xe48e6b48,0x1854bf65}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa203cfa6df451e34,0x7a301d42d8c5e18a,0x741df9337a1286ce,0x1854bf65e48e6b48}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x1e34,0xdf45,0xcfa6,0xa203,0xe18a,0xd8c5,0x1d42,0x7a30,0x86ce,0x7a12,0xf933,0x741d,0x6b48,0xe48e,0xbf65,0x1854}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdf451e34,0xa203cfa6,0xd8c5e18a,0x7a301d42,0x7a1286ce,0x741df933,0xe48e6b48,0x1854bf65}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa203cfa6df451e34,0x7a301d42d8c5e18a,0x741df9337a1286ce,0x1854bf65e48e6b48}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x8f1a,0x6fa2,0xe7d3,0x5101,0xf0c5,0x6c62,0xea1,0x3d18,0x4367,0xbd09,0xfc99,0x3a0e,0x35a4,0xf247,0x5fb2,0xc2a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x6fa28f1a,0x5101e7d3,0x6c62f0c5,0x3d180ea1,0xbd094367,0x3a0efc99,0xf24735a4,0xc2a5fb2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5101e7d36fa28f1a,0x3d180ea16c62f0c5,0x3a0efc99bd094367,0xc2a5fb2f24735a4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xcf03,0xa322,0x8523,0x93d,0x3bc,0x4b50,0x33ca,0x56b5,0x863f,0x87c8,0x38bf,0x98c6,0x8ce8,0xfab7,0xfc02,0x78ed}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xa322cf03,0x93d8523,0x4b5003bc,0x56b533ca,0x87c8863f,0x98c638bf,0xfab78ce8,0x78edfc02}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x93d8523a322cf03,0x56b533ca4b5003bc,0x98c638bf87c8863f,0x78edfc02fab78ce8}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 32, ._mp_d = (mp_limb_t[]) {0x24ed,0x1400,0x74a1,0x1310,0xce8a,0x1c0d,0x512a,0x3500,0x2451,0x6992,0x892c,0x3cdb,0x45d8,0x520,0x420,0xf11f,0x6cb,0xbe4d,0xd06c,0xcbe4,0x4d06,0x6cbe,0xe4d0,0x6cb,0xbe4d,0xd06c,0xcbe4,0x4d06,0x6cbe,0xe4d0,0x6cb,0x15}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x140024ed,0x131074a1,0x1c0dce8a,0x3500512a,0x69922451,0x3cdb892c,0x52045d8,0xf11f0420,0xbe4d06cb,0xcbe4d06c,0x6cbe4d06,0x6cbe4d0,0xd06cbe4d,0x4d06cbe4,0xe4d06cbe,0x1506cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x131074a1140024ed,0x3500512a1c0dce8a,0x3cdb892c69922451,0xf11f0420052045d8,0xcbe4d06cbe4d06cb,0x6cbe4d06cbe4d06,0x4d06cbe4d06cbe4d,0x1506cbe4d06cbe}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x8f1a,0x6fa2,0xe7d3,0x5101,0xf0c5,0x6c62,0xea1,0x3d18,0x4367,0xbd09,0xfc99,0x3a0e,0x35a4,0xf247,0x5fb2,0xc2a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x6fa28f1a,0x5101e7d3,0x6c62f0c5,0x3d180ea1,0xbd094367,0x3a0efc99,0xf24735a4,0xc2a5fb2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5101e7d36fa28f1a,0x3d180ea16c62f0c5,0x3a0efc99bd094367,0xc2a5fb2f24735a4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xb}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xb}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x98b3,0xd2e,0x314c,0x5199,0x7a5a,0xb592,0xbd65,0x1ef7,0x7d32,0x94fd,0x6cfe,0x68e3,0xcda6,0x8d91,0xfb73,0x88}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xd2e98b3,0x5199314c,0xb5927a5a,0x1ef7bd65,0x94fd7d32,0x68e36cfe,0x8d91cda6,0x88fb73}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x5199314c0d2e98b3,0x1ef7bd65b5927a5a,0x68e36cfe94fd7d32,0x88fb738d91cda6}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x1e34,0xdf45,0xcfa6,0xa203,0xe18a,0xd8c5,0x1d42,0x7a30,0x86ce,0x7a12,0xf933,0x741d,0x6b48,0xe48e,0xbf65,0x1854}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdf451e34,0xa203cfa6,0xd8c5e18a,0x7a301d42,0x7a1286ce,0x741df933,0xe48e6b48,0x1854bf65}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa203cfa6df451e34,0x7a301d42d8c5e18a,0x741df9337a1286ce,0x1854bf65e48e6b48}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xcf03,0xa322,0x8523,0x93d,0x3bc,0x4b50,0x33ca,0x56b5,0x863f,0x87c8,0x38bf,0x98c6,0x8ce8,0xfab7,0xfc02,0x78ed}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xa322cf03,0x93d8523,0x4b5003bc,0x56b533ca,0x87c8863f,0x98c638bf,0xfab78ce8,0x78edfc02}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x93d8523a322cf03,0x56b533ca4b5003bc,0x98c638bf87c8863f,0x78edfc02fab78ce8}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xb}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xb}}}
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
}}, 113}, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x779c,0xdc0d,0x3c7f,0x2e51,0x85b9,0xfb7d,0xb29e,0xd38c,0x64bb,0x9ea5,0xa4fa,0x79a3,0xdf70,0xf359,0xca21,0xa977,0x6}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xdc0d779c,0x2e513c7f,0xfb7d85b9,0xd38cb29e,0x9ea564bb,0x79a3a4fa,0xf359df70,0xa977ca21,0x6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2e513c7fdc0d779c,0xd38cb29efb7d85b9,0x79a3a4fa9ea564bb,0xa977ca21f359df70,0x6}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x779c,0xdc0d,0x3c7f,0x2e51,0x85b9,0xfb7d,0xb29e,0xd38c,0x64bb,0x9ea5,0xa4fa,0x79a3,0xdf70,0xf359,0xca21,0xa977,0x6}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xdc0d779c,0x2e513c7f,0xfb7d85b9,0xd38cb29e,0x9ea564bb,0x79a3a4fa,0xf359df70,0xa977ca21,0x6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2e513c7fdc0d779c,0xd38cb29efb7d85b9,0x79a3a4fa9ea564bb,0xa977ca21f359df70,0x6}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xbbce,0xee06,0x9e3f,0x9728,0xc2dc,0x7dbe,0x594f,0xe9c6,0xb25d,0x4f52,0xd27d,0x3cd1,0xefb8,0xf9ac,0xe510,0x54bb,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xee06bbce,0x97289e3f,0x7dbec2dc,0xe9c6594f,0x4f52b25d,0x3cd1d27d,0xf9acefb8,0x54bbe510,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x97289e3fee06bbce,0xe9c6594f7dbec2dc,0x3cd1d27d4f52b25d,0x54bbe510f9acefb8,0x3}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -17, ._mp_d = (mp_limb_t[]) {0x8db1,0xaa9d,0x1944,0x727a,0xc6c3,0xffc0,0x39b4,0x5643,0x2de0,0xb534,0xc0a9,0x5371,0x8e58,0x80df,0xa6c4,0x5a83,0x36}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0xaa9d8db1,0x727a1944,0xffc0c6c3,0x564339b4,0xb5342de0,0x5371c0a9,0x80df8e58,0x5a83a6c4,0x36}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0x727a1944aa9d8db1,0x564339b4ffc0c6c3,0x5371c0a9b5342de0,0x5a83a6c480df8e58,0x36}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -32, ._mp_d = (mp_limb_t[]) {0x9c90,0x5de8,0xf815,0x67c5,0x989,0xc9,0x7c9e,0x180b,0x526d,0xdf5a,0x3386,0xea88,0x580a,0x24c5,0x5507,0x3bad,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x438}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x5de89c90,0x67c5f815,0xc90989,0x180b7c9e,0xdf5a526d,0xea883386,0x24c5580a,0x3bad5507,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x4380000}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x67c5f8155de89c90,0x180b7c9e00c90989,0xea883386df5a526d,0x3bad550724c5580a,0x10,0x0,0x0,0x438000000000000}}}
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
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0xbbce,0xee06,0x9e3f,0x9728,0xc2dc,0x7dbe,0x594f,0xe9c6,0xb25d,0x4f52,0xd27d,0x3cd1,0xefb8,0xf9ac,0xe510,0x54bb,0x3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xee06bbce,0x97289e3f,0x7dbec2dc,0xe9c6594f,0x4f52b25d,0x3cd1d27d,0xf9acefb8,0x54bbe510,0x3}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x97289e3fee06bbce,0xe9c6594f7dbec2dc,0x3cd1d27d4f52b25d,0x54bbe510f9acefb8,0x3}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2e9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2e9}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2e9}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xa1f8,0x1530,0xa6be,0x126c,0xfd3b,0xbdd9,0xb3bc,0x8495,0x5457,0x1985,0xcfae,0xf440,0x4ea6,0x84ba,0x6881,0x2eb1}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1530a1f8,0x126ca6be,0xbdd9fd3b,0x8495b3bc,0x19855457,0xf440cfae,0x84ba4ea6,0x2eb16881}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x126ca6be1530a1f8,0x8495b3bcbdd9fd3b,0xf440cfae19855457,0x2eb1688184ba4ea6}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 17, ._mp_d = (mp_limb_t[]) {0x779c,0xdc0d,0x3c7f,0x2e51,0x85b9,0xfb7d,0xb29e,0xd38c,0x64bb,0x9ea5,0xa4fa,0x79a3,0xdf70,0xf359,0xca21,0xa977,0x6}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 9, ._mp_d = (mp_limb_t[]) {0xdc0d779c,0x2e513c7f,0xfb7d85b9,0xd38cb29e,0x9ea564bb,0x79a3a4fa,0xf359df70,0xa977ca21,0x6}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 5, ._mp_d = (mp_limb_t[]) {0x2e513c7fdc0d779c,0xd38cb29efb7d85b9,0x79a3a4fa9ea564bb,0xa977ca21f359df70,0x6}}}
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
{{._mp_alloc = 0, ._mp_size = -17, ._mp_d = (mp_limb_t[]) {0x8db1,0xaa9d,0x1944,0x727a,0xc6c3,0xffc0,0x39b4,0x5643,0x2de0,0xb534,0xc0a9,0x5371,0x8e58,0x80df,0xa6c4,0x5a83,0x36}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -9, ._mp_d = (mp_limb_t[]) {0xaa9d8db1,0x727a1944,0xffc0c6c3,0x564339b4,0xb5342de0,0x5371c0a9,0x80df8e58,0x5a83a6c4,0x36}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -5, ._mp_d = (mp_limb_t[]) {0x727a1944aa9d8db1,0x564339b4ffc0c6c3,0x5371c0a9b5342de0,0x5a83a6c480df8e58,0x36}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2e9}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2e9}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2e9}}}
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
}}, 149}};
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xb4ca,0xbe8d,0xcee3,0x9669,0x9cb,0x86eb,0xf6f9,0x374b,0x2e68,0xd1f2,0x3315,0xab5f,0x2208,0xa9c9,0x686e,0x2541}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbe8db4ca,0x9669cee3,0x86eb09cb,0x374bf6f9,0xd1f22e68,0xab5f3315,0xa9c92208,0x2541686e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x9669cee3be8db4ca,0x374bf6f986eb09cb,0xab5f3315d1f22e68,0x2541686ea9c92208}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xdb03,0x2777,0xbc36,0x4be5,0x38dd,0xd474,0x83b4,0x41a7,0x5426,0xa361,0x1f00,0xc617,0xe350,0x8cb4,0x2b1c,0xaa2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x2777db03,0x4be5bc36,0xd47438dd,0x41a783b4,0xa3615426,0xc6171f00,0x8cb4e350,0xaa22b1c}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x4be5bc362777db03,0x41a783b4d47438dd,0xc6171f00a3615426,0xaa22b1c8cb4e350}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xb4ca,0xbe8d,0xcee3,0x9669,0x9cb,0x86eb,0xf6f9,0x374b,0x2e68,0xd1f2,0x3315,0xab5f,0x2208,0xa9c9,0x686e,0x2541}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbe8db4ca,0x9669cee3,0x86eb09cb,0x374bf6f9,0xd1f22e68,0xab5f3315,0xa9c92208,0x2541686e}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x9669cee3be8db4ca,0x374bf6f986eb09cb,0xab5f3315d1f22e68,0x2541686ea9c92208}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xd9c7,0x9715,0x12ad,0x4a84,0xd0ee,0xb276,0x7344,0xf5a4,0xda41,0x2e90,0x1415,0xe548,0x3eb7,0x1d14,0x3d52,0x1a9f}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x9715d9c7,0x4a8412ad,0xb276d0ee,0xf5a47344,0x2e90da41,0xe5481415,0x1d143eb7,0x1a9f3d52}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x4a8412ad9715d9c7,0xf5a47344b276d0ee,0xe54814152e90da41,0x1a9f3d521d143eb7}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xda65,0xdf46,0xe771,0xcb34,0x84e5,0xc375,0xfb7c,0x1ba5,0x1734,0xe8f9,0x998a,0x55af,0x9104,0x54e4,0xb437,0x12a0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xdf46da65,0xcb34e771,0xc37584e5,0x1ba5fb7c,0xe8f91734,0x55af998a,0x54e49104,0x12a0b437}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xcb34e771df46da65,0x1ba5fb7cc37584e5,0x55af998ae8f91734,0x12a0b43754e49104}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x8412,0x5a4d,0xe982,0x7e48,0x619e,0x6d03,0x297c,0x2598,0x6aff,0x24ff,0xc89e,0x51c8,0x6f8,0x6965,0x7e7b,0x13de}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x5a4d8412,0x7e48e982,0x6d03619e,0x2598297c,0x24ff6aff,0x51c8c89e,0x696506f8,0x13de7e7b}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7e48e9825a4d8412,0x2598297c6d03619e,0x51c8c89e24ff6aff,0x13de7e7b696506f8}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x6e7d,0xd8b2,0x8be,0xf2e3,0x7c3e,0x1572,0x7609,0xf4ae,0x8366,0xb93e,0x53ec,0x9b03,0x6573,0xae18,0x41b0,0x707}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xd8b26e7d,0xf2e308be,0x15727c3e,0xf4ae7609,0xb93e8366,0x9b0353ec,0xae186573,0x70741b0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf2e308bed8b26e7d,0xf4ae760915727c3e,0x9b0353ecb93e8366,0x70741b0ae186573}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x8412,0x5a4d,0xe982,0x7e48,0x619e,0x6d03,0x297c,0x2598,0x6aff,0x24ff,0xc89e,0x51c8,0x6f8,0x6965,0x7e7b,0x13de}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x5a4d8412,0x7e48e982,0x6d03619e,0x2598297c,0x24ff6aff,0x51c8c89e,0x696506f8,0x13de7e7b}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7e48e9825a4d8412,0x2598297c6d03619e,0x51c8c89e24ff6aff,0x13de7e7b696506f8}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x1595,0x819b,0xe0c3,0x8b65,0xe55f,0x5790,0xb373,0x30e9,0xe798,0x6bc0,0x74b1,0xb6c5,0xa184,0xbb4c,0x3cca,0xcd7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x819b1595,0x8b65e0c3,0x5790e55f,0x30e9b373,0x6bc0e798,0xb6c574b1,0xbb4ca184,0xcd73cca}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x8b65e0c3819b1595,0x30e9b3735790e55f,0xb6c574b16bc0e798,0xcd73ccabb4ca184}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc209,0x2d26,0x74c1,0x3f24,0xb0cf,0x3681,0x14be,0x92cc,0xb57f,0x127f,0x644f,0x28e4,0x837c,0xb4b2,0x3f3d,0x9ef}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x2d26c209,0x3f2474c1,0x3681b0cf,0x92cc14be,0x127fb57f,0x28e4644f,0xb4b2837c,0x9ef3f3d}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x3f2474c12d26c209,0x92cc14be3681b0cf,0x28e4644f127fb57f,0x9ef3f3db4b2837c}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xb376,0x7694,0x643d,0xf407,0xf5c,0x6e43,0xd345,0x5c1f,0xecc4,0x777d,0x1005,0x24fe,0x88e4,0x536a,0x5c85,0xe09}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x7694b376,0xf407643d,0x6e430f5c,0x5c1fd345,0x777decc4,0x24fe1005,0x536a88e4,0xe095c85}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf407643d7694b376,0x5c1fd3456e430f5c,0x24fe1005777decc4,0xe095c85536a88e4}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x9427,0xa69c,0xda24,0xb3a7,0x4f9a,0x22fc,0xa39a,0xcb05,0xd93e,0x923d,0xb97d,0xad95,0x3374,0x96bd,0xbdeb,0x51}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xa69c9427,0xb3a7da24,0x22fc4f9a,0xcb05a39a,0x923dd93e,0xad95b97d,0x96bd3374,0x51bdeb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb3a7da24a69c9427,0xcb05a39a22fc4f9a,0xad95b97d923dd93e,0x51bdeb96bd3374}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xb376,0x7694,0x643d,0xf407,0xf5c,0x6e43,0xd345,0x5c1f,0xecc4,0x777d,0x1005,0x24fe,0x88e4,0x536a,0x5c85,0xe09}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x7694b376,0xf407643d,0x6e430f5c,0x5c1fd345,0x777decc4,0x24fe1005,0x536a88e4,0xe095c85}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xf407643d7694b376,0x5c1fd3456e430f5c,0x24fe1005777decc4,0xe095c85536a88e4}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x1f4f,0xcff8,0x8a18,0x405f,0xbfc2,0x4b46,0x2fab,0x911a,0x1385,0xe540,0x5687,0x7768,0x556f,0xbcad,0x9e99,0xdb7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xcff81f4f,0x405f8a18,0x4b46bfc2,0x911a2fab,0xe5401385,0x77685687,0xbcad556f,0xdb79e99}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x405f8a18cff81f4f,0x911a2fab4b46bfc2,0x77685687e5401385,0xdb79e99bcad556f}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x59bb,0xbb4a,0xb21e,0x7a03,0x87ae,0xb721,0xe9a2,0x2e0f,0xf662,0xbbbe,0x802,0x127f,0x4472,0xa9b5,0xae42,0x704}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbb4a59bb,0x7a03b21e,0xb72187ae,0x2e0fe9a2,0xbbbef662,0x127f0802,0xa9b54472,0x704ae42}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x7a03b21ebb4a59bb,0x2e0fe9a2b72187ae,0x127f0802bbbef662,0x704ae42a9b54472}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x7a2a,0xcc34,0x1fb9,0x5b4e,0x6acf,0x4f0f,0xbb68,0x211d,0xa57b,0xae74,0x782,0xa512,0xd75c,0xb576,0x5af5,0xa035}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xcc347a2a,0x5b4e1fb9,0x4f0f6acf,0x211dbb68,0xae74a57b,0xa5120782,0xb576d75c,0xa0355af5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5b4e1fb9cc347a2a,0x211dbb684f0f6acf,0xa5120782ae74a57b,0xa0355af5b576d75c}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xa3e3,0x12fb,0x32f3,0xb40f,0x4bbe,0x537d,0xbefc,0xdda9,0x8954,0xaca9,0xaaf3,0xc020,0x17da,0xf48f,0x88fd,0x21a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x12fba3e3,0xb40f32f3,0x537d4bbe,0xdda9befc,0xaca98954,0xc020aaf3,0xf48f17da,0x21a88fd}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xb40f32f312fba3e3,0xdda9befc537d4bbe,0xc020aaf3aca98954,0x21a88fdf48f17da}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x7a2a,0xcc34,0x1fb9,0x5b4e,0x6acf,0x4f0f,0xbb68,0x211d,0xa57b,0xae74,0x782,0xa512,0xd75c,0xb576,0x5af5,0xa035}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xcc347a2a,0x5b4e1fb9,0x4f0f6acf,0x211dbb68,0xae74a57b,0xa5120782,0xb576d75c,0xa0355af5}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x5b4e1fb9cc347a2a,0x211dbb684f0f6acf,0xa5120782ae74a57b,0xa0355af5b576d75c}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xd647,0xb938,0xecc6,0xa73e,0x1f10,0xfb92,0xfc6b,0x4373,0x1c26,0x1cb,0x5c8f,0xe4f1,0xbf81,0xc0e7,0xd1f7,0x9e1a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xb938d647,0xa73eecc6,0xfb921f10,0x4373fc6b,0x1cb1c26,0xe4f15c8f,0xc0e7bf81,0x9e1ad1f7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa73eecc6b938d647,0x4373fc6bfb921f10,0xe4f15c8f01cb1c26,0x9e1ad1f7c0e7bf81}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x3d15,0xe61a,0xfdc,0xada7,0xb567,0x2787,0xddb4,0x908e,0x52bd,0x573a,0x3c1,0x5289,0x6bae,0xdabb,0xad7a,0x501a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe61a3d15,0xada70fdc,0x2787b567,0x908eddb4,0x573a52bd,0x528903c1,0xdabb6bae,0x501aad7a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xada70fdce61a3d15,0x908eddb42787b567,0x528903c1573a52bd,0x501aad7adabb6bae}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x81d6,0x1f29,0xf1a,0x365e,0x8f4a,0x95c8,0x38b1,0x87f1,0xb9ff,0x9cca,0x8239,0x1cb1,0x70f,0x8fde,0x5f3f,0x25be}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1f2981d6,0x365e0f1a,0x95c88f4a,0x87f138b1,0x9ccab9ff,0x1cb18239,0x8fde070f,0x25be5f3f}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x365e0f1a1f2981d6,0x87f138b195c88f4a,0x1cb182399ccab9ff,0x25be5f3f8fde070f}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xbe61,0x7c4c,0xa49c,0x48f9,0x3fb4,0xb80a,0xf823,0xd427,0x593e,0x9ec7,0x3a56,0xda06,0x551e,0x2875,0xc2cb,0xfb0}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x7c4cbe61,0x48f9a49c,0xb80a3fb4,0xd427f823,0x9ec7593e,0xda063a56,0x2875551e,0xfb0c2cb}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x48f9a49c7c4cbe61,0xd427f823b80a3fb4,0xda063a569ec7593e,0xfb0c2cb2875551e}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x81d6,0x1f29,0xf1a,0x365e,0x8f4a,0x95c8,0x38b1,0x87f1,0xb9ff,0x9cca,0x8239,0x1cb1,0x70f,0x8fde,0x5f3f,0x25be}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x1f2981d6,0x365e0f1a,0x95c88f4a,0x87f138b1,0x9ccab9ff,0x1cb18239,0x8fde070f,0x25be5f3f}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x365e0f1a1f2981d6,0x87f138b195c88f4a,0x1cb182399ccab9ff,0x25be5f3f8fde070f}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc375,0xa2dc,0x6a7d,0xed64,0x4f95,0xddbe,0x408d,0xb3c9,0x60c0,0xfe03,0x47e2,0x42ab,0xb1f0,0x6768,0x9c74,0x160d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xa2dcc375,0xed646a7d,0xddbe4f95,0xb3c9408d,0xfe0360c0,0x42ab47e2,0x6768b1f0,0x160d9c74}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xed646a7da2dcc375,0xb3c9408dddbe4f95,0x42ab47e2fe0360c0,0x160d9c746768b1f0}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc0eb,0xf94,0x78d,0x1b2f,0x47a5,0xcae4,0x9c58,0xc3f8,0x5cff,0xce65,0xc11c,0x8e58,0x387,0xc7ef,0x2f9f,0x12df}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf94c0eb,0x1b2f078d,0xcae447a5,0xc3f89c58,0xce655cff,0x8e58c11c,0xc7ef0387,0x12df2f9f}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x1b2f078d0f94c0eb,0xc3f89c58cae447a5,0x8e58c11cce655cff,0x12df2f9fc7ef0387}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x5a86,0x1729,0x8ced,0x8280,0xd48f,0x1e0f,0x5e39,0x24b3,0x74ba,0xa294,0xd9f3,0x4e2e,0x8cc1,0xee6b,0xdd2,0x3079}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x17295a86,0x82808ced,0x1e0fd48f,0x24b35e39,0xa29474ba,0x4e2ed9f3,0xee6b8cc1,0x30790dd2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x82808ced17295a86,0x24b35e391e0fd48f,0x4e2ed9f3a29474ba,0x30790dd2ee6b8cc1}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x9203,0x57ee,0x3867,0xdf50,0xd8ad,0xbe9c,0x9e30,0x7a77,0xcd0f,0x77d9,0xbb7f,0x65f1,0x1b16,0xbbf5,0xe5c0,0x2563}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x57ee9203,0xdf503867,0xbe9cd8ad,0x7a779e30,0x77d9cd0f,0x65f1bb7f,0xbbf51b16,0x2563e5c0}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xdf50386757ee9203,0x7a779e30be9cd8ad,0x65f1bb7f77d9cd0f,0x2563e5c0bbf51b16}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x5a86,0x1729,0x8ced,0x8280,0xd48f,0x1e0f,0x5e39,0x24b3,0x74ba,0xa294,0xd9f3,0x4e2e,0x8cc1,0xee6b,0xdd2,0x3079}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x17295a86,0x82808ced,0x1e0fd48f,0x24b35e39,0xa29474ba,0x4e2ed9f3,0xee6b8cc1,0x30790dd2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x82808ced17295a86,0x24b35e391e0fd48f,0x4e2ed9f3a29474ba,0x30790dd2ee6b8cc1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xc883,0xbf3a,0x5485,0xa330,0xfbe1,0x5f72,0xc008,0xaa3b,0xa7aa,0x2aba,0x1e74,0xe83d,0x71aa,0x3276,0x2812,0xb15}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xbf3ac883,0xa3305485,0x5f72fbe1,0xaa3bc008,0x2abaa7aa,0xe83d1e74,0x327671aa,0xb152812}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xa3305485bf3ac883,0xaa3bc0085f72fbe1,0xe83d1e742abaa7aa,0xb152812327671aa}}}
#endif
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xad43,0x8b94,0x4676,0xc140,0xea47,0x8f07,0xaf1c,0x1259,0x3a5d,0xd14a,0x6cf9,0xa717,0xc660,0x7735,0x86e9,0x183c}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x8b94ad43,0xc1404676,0x8f07ea47,0x1259af1c,0xd14a3a5d,0xa7176cf9,0x7735c660,0x183c86e9}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc14046768b94ad43,0x1259af1c8f07ea47,0xa7176cf9d14a3a5d,0x183c86e97735c660}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0x8fcd,0xe605,0x8b19,0xe24f,0x42a8,0x5b5f,0x7aae,0x78f3,0x828e,0x7553,0x5216,0x7176,0x559,0x367e,0x938b,0x2fe3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xe6058fcd,0xe24f8b19,0x5b5f42a8,0x78f37aae,0x7553828e,0x71765216,0x367e0559,0x2fe3938b}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xe24f8b19e6058fcd,0x78f37aae5b5f42a8,0x717652167553828e,0x2fe3938b367e0559}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x8fcd,0xe605,0x8b19,0xe24f,0x42a8,0x5b5f,0x7aae,0x78f3,0x828e,0x7553,0x5216,0x7176,0x559,0x367e,0x938b,0x2fe3}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xe6058fcd,0xe24f8b19,0x5b5f42a8,0x78f37aae,0x7553828e,0x71765216,0x367e0559,0x2fe3938b}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xe24f8b19e6058fcd,0x78f37aae5b5f42a8,0x717652167553828e,0x2fe3938b367e0559}}}
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x2315,0x38d9,0x6fc1,0xa333,0xaeb,0xce6a,0x2a4c,0xa1c1,0x274c,0xa9fc,0xd4c6,0xb0b3,0x555b,0x7a48,0x411a,0x2bdc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x38d92315,0xa3336fc1,0xce6a0aeb,0xa1c12a4c,0xa9fc274c,0xb0b3d4c6,0x7a48555b,0x2bdc411a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xa3336fc138d92315,0xa1c12a4cce6a0aeb,0xb0b3d4c6a9fc274c,0x2bdc411a7a48555b}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x2315,0x38d9,0x6fc1,0xa333,0xaeb,0xce6a,0x2a4c,0xa1c1,0x274c,0xa9fc,0xd4c6,0xb0b3,0x555b,0x7a48,0x411a,0x2bdc}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x38d92315,0xa3336fc1,0xce6a0aeb,0xa1c12a4c,0xa9fc274c,0xb0b3d4c6,0x7a48555b,0x2bdc411a}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xa3336fc138d92315,0xa1c12a4cce6a0aeb,0xb0b3d4c6a9fc274c,0x2bdc411a7a48555b}}}
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
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xe8f5,0xf5ac,0x2ee2,0xc962,0x459d,0xd9a0,0xd761,0x7c5a,0x8268,0xcf36,0x9b08,0xb7a6,0xbd23,0x7e04,0xe324,0x23ba}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf5ace8f5,0xc9622ee2,0xd9a0459d,0x7c5ad761,0xcf368268,0xb7a69b08,0x7e04bd23,0x23bae324}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc9622ee2f5ace8f5,0x7c5ad761d9a0459d,0xb7a69b08cf368268,0x23bae3247e04bd23}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xe8f5,0xf5ac,0x2ee2,0xc962,0x459d,0xd9a0,0xd761,0x7c5a,0x8268,0xcf36,0x9b08,0xb7a6,0xbd23,0x7e04,0xe324,0x23ba}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xf5ace8f5,0xc9622ee2,0xd9a0459d,0x7c5ad761,0xcf368268,0xb7a69b08,0x7e04bd23,0x23bae324}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0xc9622ee2f5ace8f5,0x7c5ad761d9a0459d,0xb7a69b08cf368268,0x23bae3247e04bd23}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x2}}}
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
{{._mp_alloc = 0, ._mp_size = 16, ._mp_d = (mp_limb_t[]) {0xfef5,0x4752,0xbb14,0x6ee3,0x5898,0x6a2,0x8282,0x1179,0xe429,0xf5d1,0x5ad8,0x642d,0x3061,0x58d,0x9c04,0x917b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x4752fef5,0x6ee3bb14,0x6a25898,0x11798282,0xf5d1e429,0x642d5ad8,0x58d3061,0x917b9c04}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 4, ._mp_d = (mp_limb_t[]) {0x6ee3bb144752fef5,0x1179828206a25898,0x642d5ad8f5d1e429,0x917b9c04058d3061}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xfef5,0x4752,0xbb14,0x6ee3,0x5898,0x6a2,0x8282,0x1179,0xe429,0xf5d1,0x5ad8,0x642d,0x3061,0x58d,0x9c04,0x917b}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0x4752fef5,0x6ee3bb14,0x6a25898,0x11798282,0xf5d1e429,0x642d5ad8,0x58d3061,0x917b9c04}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x6ee3bb144752fef5,0x1179828206a25898,0x642d5ad8f5d1e429,0x917b9c04058d3061}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -1, ._mp_d = (mp_limb_t[]) {0x7}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x7}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x7}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x7}}}
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xc375,0xa2dc,0x6a7d,0xed64,0x4f95,0xddbe,0x408d,0xb3c9,0x60c0,0xfe03,0x47e2,0x42ab,0xb1f0,0x6768,0x9c74,0x160d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xa2dcc375,0xed646a7d,0xddbe4f95,0xb3c9408d,0xfe0360c0,0x42ab47e2,0x6768b1f0,0x160d9c74}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xed646a7da2dcc375,0xb3c9408dddbe4f95,0x42ab47e2fe0360c0,0x160d9c746768b1f0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0xc375,0xa2dc,0x6a7d,0xed64,0x4f95,0xddbe,0x408d,0xb3c9,0x60c0,0xfe03,0x47e2,0x42ab,0xb1f0,0x6768,0x9c74,0x160d}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xa2dcc375,0xed646a7d,0xddbe4f95,0xb3c9408d,0xfe0360c0,0x42ab47e2,0x6768b1f0,0x160d9c74}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0xed646a7da2dcc375,0xb3c9408dddbe4f95,0x42ab47e2fe0360c0,0x160d9c746768b1f0}}}
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
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x95f7,0xbdd2,0x75d6,0x430e,0x5c58,0xbd78,0x16b0,0x1278,0xc3f8,0x16b,0xb5dc,0xbbcf,0xfa18,0x1815,0x3c32,0x624a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xbdd295f7,0x430e75d6,0xbd785c58,0x127816b0,0x16bc3f8,0xbbcfb5dc,0x1815fa18,0x624a3c32}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x430e75d6bdd295f7,0x127816b0bd785c58,0xbbcfb5dc016bc3f8,0x624a3c321815fa18}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = -16, ._mp_d = (mp_limb_t[]) {0x95f7,0xbdd2,0x75d6,0x430e,0x5c58,0xbd78,0x16b0,0x1278,0xc3f8,0x16b,0xb5dc,0xbbcf,0xfa18,0x1815,0x3c32,0x624a}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = -8, ._mp_d = (mp_limb_t[]) {0xbdd295f7,0x430e75d6,0xbd785c58,0x127816b0,0x16bc3f8,0xbbcfb5dc,0x1815fa18,0x624a3c32}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = -4, ._mp_d = (mp_limb_t[]) {0x430e75d6bdd295f7,0x127816b0bd785c58,0xbbcfb5dc016bc3f8,0x624a3c321815fa18}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xd}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xd}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xd}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 16
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xd}}}
#elif GMP_LIMB_BITS == 32
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xd}}}
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0xd}}}
#endif
}}};
