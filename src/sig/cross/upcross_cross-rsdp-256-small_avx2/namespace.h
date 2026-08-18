
#pragma once

#define CROSS_NAMESPACE(s) PQCLEAN_CROSSRSDP256SMALL_AVX2_ ## s

#define crypto_sign                             CROSS_NAMESPACE(crypto_sign)
#define crypto_sign_keypair                     CROSS_NAMESPACE(crypto_sign_keypair)
#define crypto_sign_open                        CROSS_NAMESPACE(crypto_sign_open)
#define crypto_sign_signature                   CROSS_NAMESPACE(crypto_sign_signature)
#define crypto_sign_verify                      CROSS_NAMESPACE(crypto_sign_verify)

#define CROSS_keygen                            CROSS_NAMESPACE(CROSS_keygen)
#define CROSS_sign                              CROSS_NAMESPACE(CROSS_sign)
#define CROSS_verify                            CROSS_NAMESPACE(CROSS_verify)
#define expand_digest_to_fixed_weight           CROSS_NAMESPACE(expand_digest_to_fixed_weight)
#define gen_seed_tree                           CROSS_NAMESPACE(gen_seed_tree)
#define pack_fp_syn                             CROSS_NAMESPACE(pack_fp_syn)
#define pack_fp_vec                             CROSS_NAMESPACE(pack_fp_vec)
#define pack_fz_rsdp_g_vec                      CROSS_NAMESPACE(pack_fz_rsdp_g_vec)
#define pack_fz_vec                             CROSS_NAMESPACE(pack_fz_vec)
#define rebuild_leaves                          CROSS_NAMESPACE(rebuild_leaves)
#define rebuild_tree                            CROSS_NAMESPACE(rebuild_tree)
#define recompute_root                          CROSS_NAMESPACE(recompute_root)
#define seed_leaves                             CROSS_NAMESPACE(seed_leaves)
#define seed_path                               CROSS_NAMESPACE(seed_path)
#define tree_proof                              CROSS_NAMESPACE(tree_proof)
#define tree_root                               CROSS_NAMESPACE(tree_root)
#define unpack_fp_syn                           CROSS_NAMESPACE(unpack_fp_syn)
#define unpack_fp_vec                           CROSS_NAMESPACE(unpack_fp_vec)
#define unpack_fz_rsdp_g_vec                    CROSS_NAMESPACE(unpack_fz_rsdp_g_vec)
#define unpack_fz_vec                           CROSS_NAMESPACE(unpack_fz_vec)
