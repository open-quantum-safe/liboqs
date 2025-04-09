// Put symbols in namespace for use in a library.
// Only used by the liboqs integration

#ifndef SNOVA_NAMESPACE_H
#define SNOVA_NAMESPACE_H

#include <stdint.h>

#if OPTIMISATION == 2
#if __ARM_NEON
#define OPT_NAME neon
#else
#define OPT_NAME avx2
#endif
#elif OPTIMISATION == 1
#define OPT_NAME opt
#else
#define OPT_NAME ref
#endif

#if PK_EXPAND_SHAKE
#define PKX_NAME shake
#else
#define PKX_NAME aes
#endif

#if sk_is_seed
#define ESK_NAME ssk
#else
#define ESK_NAME esk
#endif

#define PARAM_JOIN_(o, p, s, a, b, c, f) _snova_##a##_##b##_##c##_##p##_##s##_##o##_##f
#define PARAM_JOIN(o, p, s, a, b, c, f) PARAM_JOIN_(o, p, s, a, b, c, f)
#define SNOVA_NAMESPACE(f) PARAM_JOIN(OPT_NAME, PKX_NAME, ESK_NAME, v_SNOVA, o_SNOVA, l_SNOVA, f)

// snova.c

#define generate_keys_core SNOVA_NAMESPACE(generate_keys_core)
#define generate_keys_ssk SNOVA_NAMESPACE(generate_keys_ssk)
#define generate_keys_esk SNOVA_NAMESPACE(generate_keys_esk)
#define generate_pk_with_ssk SNOVA_NAMESPACE(generate_pk_with_ssk)
#define generate_pk_with_esk SNOVA_NAMESPACE(generate_pk_with_esk)
#define sign_digest_ssk SNOVA_NAMESPACE(sign_digest_ssk)
#define sign_digest_esk SNOVA_NAMESPACE(sign_digest_esk)
#define verify_signture SNOVA_NAMESPACE(verify_signture)
#define verify_signture_pkx SNOVA_NAMESPACE(verify_signture_pkx)
#define expand_public_key SNOVA_NAMESPACE(expand_public_key)
#define expand_public_pack SNOVA_NAMESPACE(expand_public_pack)
#define fixed_abq SNOVA_NAMESPACE(fixed_abq)
#define sign_expand_ssk SNOVA_NAMESPACE(sign_expand_ssk)
#define expand_secret SNOVA_NAMESPACE(expand_secret)
#define snova_init SNOVA_NAMESPACE(snova_init)
#define snova_plasma_init SNOVA_NAMESPACE(snova_plasma_init)
#define init_avx_table SNOVA_NAMESPACE(init_avx_table)
#define inv4b SNOVA_NAMESPACE(inv4b)
#define mt4b SNOVA_NAMESPACE(mt4b)

// Change library function names

#define ct_is_negative SNOVA_NAMESPACE(ct_is_negative)
#define ct_xgf16_is_not_zero SNOVA_NAMESPACE(ct_ct_xgf16_is_not_zerois_negative)
#define ct_gf16_is_not_zero SNOVA_NAMESPACE(ct_gf16_is_not_zero)
#define snova_set_zero SNOVA_NAMESPACE(snova_set_zero)
#define snova_shake SNOVA_NAMESPACE(snova_shake)

#define shake256 SNOVA_NAMESPACE(shake256)
#define convert_bytes_to_GF16s SNOVA_NAMESPACE(convert_bytes_to_GF16s)
#define convert_GF16s_to_bytes SNOVA_NAMESPACE(convert_GF16s_to_bytes)
#define convert_bytes_to_GF16s_cut_in_half SNOVA_NAMESPACE(convert_bytes_to_GF16s_cut_in_half)
#define convert_GF16s_to_bytes_merger_in_half SNOVA_NAMESPACE(convert_GF16s_to_bytes_merger_in_half)

#define AES_128_CTR SNOVA_NAMESPACE(AES_128_CTR)
#define AES_256_ECB SNOVA_NAMESPACE(AES_256_ECB)

// snova_opt

#define gen_P22_opt SNOVA_NAMESPACE(gen_P22_opt)
#define gen_F_opt SNOVA_NAMESPACE(gen_F_opt)
#define sign_digest_core_opt SNOVA_NAMESPACE(sign_digest_core_opt)
#define verify_signture_opt_core SNOVA_NAMESPACE(verify_signture_opt_core)
#define verify_signture_opt SNOVA_NAMESPACE(verify_signture_opt)
#define verify_signture_pkx_opt SNOVA_NAMESPACE(verify_signture_pkx_opt)

// snova_plasma

#define init_vector_table SNOVA_NAMESPACE(init_vector_table)
#define gen_P22_vtl SNOVA_NAMESPACE(gen_P22_vtl)
#define gen_F_vtl SNOVA_NAMESPACE(gen_F_vtl)
#define calc_LR_J_vtl SNOVA_NAMESPACE(calc_LR_J_vtl)
#define sign_digest_core_gnl_vtl SNOVA_NAMESPACE(sign_digest_core_gnl_vtl)
#define verify_signture_vtl_core SNOVA_NAMESPACE(verify_signture_vtl_core)
#define verify_signture_vtl SNOVA_NAMESPACE(verify_signture_vtl)
#define verify_signture_pkx_vtl SNOVA_NAMESPACE(verify_signture_pkx_vtl)
#define mtk2_16 SNOVA_NAMESPACE(mtk2_16)

#endif
