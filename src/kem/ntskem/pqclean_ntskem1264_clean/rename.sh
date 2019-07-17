#!/bin/bash

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=additive_fft -new-name=PQCLEAN_NTSKEM1264_CLEAN_additive_fft additive_fft.c nts_kem.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=berlekamp_massey -new-name=PQCLEAN_NTSKEM1264_CLEAN_berlekamp_massey berlekamp_massey.c nts_kem.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=ff_create -new-name=PQCLEAN_NTSKEM1264_CLEAN_ff_create ff.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=ff_release -new-name=PQCLEAN_NTSKEM1264_CLEAN_ff_release ff.c nts_kem.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=m4r_rref -new-name=PQCLEAN_NTSKEM1264_CLEAN_m4r_rref m4r.c matrix_ff2.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=alloc_matrix_ff2 -new-name=PQCLEAN_NTSKEM1264_CLEAN_alloc_matrix_ff2 m4r.c matrix_ff2.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=calloc_matrix_ff2 -new-name=PQCLEAN_NTSKEM1264_CLEAN_calloc_matrix_ff2 matrix_ff2.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=column_swap_matrix_ff2 -new-name=PQCLEAN_NTSKEM1264_CLEAN_column_swap_matrix_ff2 matrix_ff2.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=free_matrix_ff2 -new-name=PQCLEAN_NTSKEM1264_CLEAN_free_matrix_ff2 m4r.c matrix_ff2.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=reduce_row_echelon_matrix_ff2 -new-name=PQCLEAN_NTSKEM1264_CLEAN_reduce_row_echelon_matrix_ff2 matrix_ff2.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=zero_matrix_ff2 -new-name=PQCLEAN_NTSKEM1264_CLEAN_zero_matrix_ff2 matrix_ff2.c nts_kem.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_ciphertext_size -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_ciphertext_size nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_create -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_create kem.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_decapsulate -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_decapsulate kem.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_encapsulate -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_encapsulate kem.c nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_init_from_private_key -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_init_from_private_key nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_key_size -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_key_size nts_kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=nts_kem_release -new-name=PQCLEAN_NTSKEM1264_CLEAN_nts_kem_release kem.c nts_kem.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=clone_poly -new-name=PQCLEAN_NTSKEM1264_CLEAN_clone_poly additive_fft.c polynomial.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=formal_derivative_poly -new-name=PQCLEAN_NTSKEM1264_CLEAN_formal_derivative_poly nts_kem.c polynomial.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=free_poly -new-name=PQCLEAN_NTSKEM1264_CLEAN_free_poly additive_fft.c nts_kem.c polynomial.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=gcd_poly -new-name=PQCLEAN_NTSKEM1264_CLEAN_gcd_poly nts_kem.c polynomial.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=init_poly -new-name=PQCLEAN_NTSKEM1264_CLEAN_init_poly additive_fft.c berlekamp_massey.c nts_kem.c polynomial.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=update_poly_degree -new-name=PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree additive_fft.c polynomial.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=zero_poly -new-name=PQCLEAN_NTSKEM1264_CLEAN_zero_poly additive_fft.c nts_kem.c polynomial.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=random_uint16_bounded -new-name=PQCLEAN_NTSKEM1264_CLEAN_random_uint16_bounded random.c nts_kem.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=stack_create -new-name=PQCLEAN_NTSKEM1264_CLEAN_stack_create additive_fft.c stack.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=stack_free -new-name=PQCLEAN_NTSKEM1264_CLEAN_stack_free additive_fft.c stack.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=stack_pop -new-name=PQCLEAN_NTSKEM1264_CLEAN_stack_pop additive_fft.c stack.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=stack_push -new-name=PQCLEAN_NTSKEM1264_CLEAN_stack_push additive_fft.c stack.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=stack_size -new-name=PQCLEAN_NTSKEM1264_CLEAN_stack_size additive_fft.c stack.c -- -I../../../common

/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=crypto_kem_dec -new-name=PQCLEAN_NTSKEM1264_CLEAN_crypto_kem_dec kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=crypto_kem_enc -new-name=PQCLEAN_NTSKEM1264_CLEAN_crypto_kem_enc kem.c -- -I../../../common
/usr/local/opt/clang\+llvm/bin/clang-rename -i -qualified-name=crypto_kem_keypair -new-name=PQCLEAN_NTSKEM1264_CLEAN_crypto_kem_keypair kem.c -- -I../../../common
sed -E -e 's/CRYPTO_PUBLICKEYBYTES/PQCLEAN_NTSKEM1264_CLEAN_CRYPTO_PUBLICKEYBYTES/g' -i "" kem.c
sed -E -e 's/CRYPTO_SECRETKEYBYTES/PQCLEAN_NTSKEM1264_CLEAN_CRYPTO_SECRETKEYBYTES/g' -i "" kem.c