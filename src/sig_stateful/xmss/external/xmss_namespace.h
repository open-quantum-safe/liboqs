
// hash.h - ALL THE HASHING FUNCTIONS
#define prf oqs_sig_stfl_xmss_prf
#define h_msg oqs_sig_stfl_xmss_h_msg
#define thash_h oqs_sig_stfl_xmss_thash_h
#define thash_f oqs_sig_stfl_xmss_thash_f
#define hash_message oqs_sig_stfl_xmss_hash_message
#define hash_prg oqs_sig_stfl_xmss_hash_prg

// hash_address.h - ALL THE HASH ADDRESSING FUNCTIONS


// params.h - ALL THE PARAMETER FUNCTIONS
#define xmss_str_to_oid oqs_sig_stfl_xmss_xmss_str_to_oid
#define xmssmt_str_to_oid oqs_sig_stfl_xmssmt_str_to_oid
#define xmss_parse_oid oqs_sig_stfl_xmss_parse_oid
#define xmssmt_parse_oid oqs_sig_stfl_xmssmt_parse_oid
#define xmss_xmssmt_initialize_params oqs_sig_stfl_xmss_xmssmt_initialize_params

// utils.c - ULL <-> BYTES FUNCTIONS
#define ull_to_bytes oqs_sig_stfl_xmss_ull_to_bytes
#define bytes_to_ull oqs_sig_stfl_xmss_bytes_to_ull

// xmss_commons.c - XMSS VERFICIATION FUNCTIONS
#define gen_leaf_wots oqs_sig_stfl_xmss_gen_leaf_wots
#define get_seed oqs_sig_stfl_xmss_get_seed
#define xmss_core_sign_open oqs_sig_stfl_xmss_core_sign_open
#define xmssmt_core_sign_open oqs_sig_stfl_xmssmt_core_sign_open

// xmss_core.h - XMSS CORE FUNCTIONS
#define xmss_xmssmt_core_sk_bytes oqs_sig_stfl_xmss_xmssmt_core_sk_bytes
#define xmss_core_increment_authpath oqs_sig_stfl_xmss_core_increment_authpath
#define xmss_core_keypair oqs_sig_stfl_xmss_core_keypair
#define xmss_core_sign oqs_sig_stfl_xmss_core_sign
#define xmssmt_core_increment_authpath oqs_sig_stfl_xmss_core_increment_authpath
#define xmssmt_core_keypair oqs_sig_stfl_xmss_core_keypair
#define xmssmt_core_sign oqs_sig_stfl_xmss_core_sign

// xmss.c - XMSS VARIANT PARSING FUNCTIONS
#define xmss_derive_subkey oqs_sig_stfl_xmss_derive_subkey
#define xmss_keypair oqs_sig_stfl_xmss_keypair
#define xmss_sign oqs_sig_stfl_xmss_sign
#define xmss_sign_open oqs_sig_stfl_xmss_sign_open
#define xmssmt_keypair oqs_sig_stfl_xmss_keypair
#define xmsmt_sign oqs_sig_stfl_xmss_sign
#define xmssmt_sign_open oqs_sig_stfl_xmss_sign_open

