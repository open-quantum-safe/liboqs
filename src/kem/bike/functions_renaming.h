/******************************************************************************
 * BIKE -- Bit Flipping Key Encapsulation
 *
 * Copyright (c) 2017 Nir Drucker, Shay Gueron, Rafael Misoczki, Tobias Oder, Tim Gueneysu
 * (drucker.nir@gmail.com, shay.gueron@gmail.com, rafael.misoczki@intel.com, tobias.oder@rub.de, tim.gueneysu@rub.de)
 *
 * Permission to use this code for BIKE is granted.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the follow
 *
 * SPDX-License-Identifier: BSD-3-Clauseing disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * The names of the contributors may not be used to endorse or promote
 *   products derived from this software without specific prior written
 *   permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ""AS IS"" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS CORPORATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef __FUNCTIONS_RENAMING_H_INCLUDED__
#define __FUNCTIONS_RENAMING_H_INCLUDED__

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define RENAME_FUNC_NAME(fname) EVALUATOR(FUNC_PREFIX, fname)

#define keypair RENAME_FUNC_NAME(keypair)
#define decaps RENAME_FUNC_NAME(decaps)
#define encaps RENAME_FUNC_NAME(encaps)

#define aes_ctr_prf RENAME_FUNC_NAME(aes_ctr_prf)
#define sample_uniform_r_bits_with_fixed_prf_context RENAME_FUNC_NAME(sample_uniform_r_bits_with_fixed_prf_context)
#define init_prf_state RENAME_FUNC_NAME(init_prf_state)
#define clean_prf_state RENAME_FUNC_NAME(clean_prf_state)
#define get_prf_output RENAME_FUNC_NAME(get_prf_output)
#define decode RENAME_FUNC_NAME(decode)
#define print_BE RENAME_FUNC_NAME(print_BE)
#define print_LE RENAME_FUNC_NAME(print_LE)
#define gf2x_mod_mul RENAME_FUNC_NAME(gf2x_mod_mul)
#define secure_set_bits RENAME_FUNC_NAME(secure_set_bits)
#define sha RENAME_FUNC_NAME(sha)
#define count_ones RENAME_FUNC_NAME(count_ones)
#define sha_mb RENAME_FUNC_NAME(sha_mb)
#define split_e RENAME_FUNC_NAME(split_e)
#define compute_syndrome RENAME_FUNC_NAME(compute_syndrome)
#define recompute_syndrome RENAME_FUNC_NAME(recompute_syndrome)
#define bike_errno RENAME_FUNC_NAME(bike_errno)
#define cyclic_product RENAME_FUNC_NAME(cyclic_product)
#define ossl_add RENAME_FUNC_NAME(ossl_add)
#define karatzuba_add1 RENAME_FUNC_NAME(karatzuba_add1)
#define karatzuba_add2 RENAME_FUNC_NAME(karatzuba_add2)
#define gf2x_add RENAME_FUNC_NAME(gf2x_add)
#define gf2_muladd_4x4 RENAME_FUNC_NAME(gf2_muladd_4x4)
#define red RENAME_FUNC_NAME(red)
#define gf2x_mul_1x1 RENAME_FUNC_NAME(gf2x_mul_1x1)
#define rotate_right RENAME_FUNC_NAME(rotate_right)
#define r_bits_vector_weight RENAME_FUNC_NAME(r_bits_vector_weight)
#define rotate256_small RENAME_FUNC_NAME(rotate256_small)

// Additional Round-3 functions
#define rotate_right_port RENAME_FUNC_NAME(rotate_right_port)
#define dup_port RENAME_FUNC_NAME(dup_port)
#define bit_sliced_adder_port RENAME_FUNC_NAME(bit_sliced_adder_port)
#define bit_slice_full_subtract_port RENAME_FUNC_NAME(bit_slice_full_subtract_port)
#define rotate_right_avx2 RENAME_FUNC_NAME(rotate_right_avx2)
#define rotate_right_avx512 RENAME_FUNC_NAME(rotate_right_avx512)
#define dup_avx2 RENAME_FUNC_NAME(dup_avx2)
#define dup_avx512 RENAME_FUNC_NAME(dup_avx512)
#define bit_sliced_adder_avx2 RENAME_FUNC_NAME(bit_sliced_adder_avx2)
#define bit_sliced_adder_avx512 RENAME_FUNC_NAME(bit_sliced_adder_avx512)
#define bit_slice_full_subtract_avx2 RENAME_FUNC_NAME(bit_slice_full_subtract_avx2)
#define bit_slice_full_subtract_avx512 RENAME_FUNC_NAME(bit_slice_full_subtract_avx512)
#define decode_ctx_init RENAME_FUNC_NAME(decode_ctx_init)

#define cpu_features_init RENAME_FUNC_NAME(cpu_features_init)
#define is_avx2_enabled RENAME_FUNC_NAME(is_avx2_enabled)
#define is_avx512_enabled RENAME_FUNC_NAME(is_avx512_enabled)
#define is_pclmul_enabled RENAME_FUNC_NAME(is_pclmul_enabled)
#define is_vpclmul_enabled RENAME_FUNC_NAME(is_vpclmul_enabled)

#define gf2x_mul_base_port RENAME_FUNC_NAME(gf2x_mul_base_port)
#define karatzuba_add1_port RENAME_FUNC_NAME(karatzuba_add1_port)
#define karatzuba_add2_port RENAME_FUNC_NAME(karatzuba_add2_port)
#define karatzuba_add3_port RENAME_FUNC_NAME(karatzuba_add3_port)
#define gf2x_sqr_port RENAME_FUNC_NAME(gf2x_sqr_port)
#define k_sqr_port RENAME_FUNC_NAME(k_sqr_port)
#define gf2x_red_port RENAME_FUNC_NAME(gf2x_red_port)
#define gf2x_mul_base_pclmul RENAME_FUNC_NAME(gf2x_mul_base_pclmul)
#define gf2x_mul_base_vpclmul RENAME_FUNC_NAME(gf2x_mul_base_vpclmul)
#define karatzuba_add1_avx2 RENAME_FUNC_NAME(karatzuba_add1_avx2)
#define karatzuba_add1_avx512 RENAME_FUNC_NAME(karatzuba_add1_avx512)
#define karatzuba_add2_avx2 RENAME_FUNC_NAME(karatzuba_add2_avx2)
#define karatzuba_add2_avx512 RENAME_FUNC_NAME(karatzuba_add2_avx512)
#define karatzuba_add3_avx2 RENAME_FUNC_NAME(karatzuba_add3_avx2)
#define karatzuba_add3_avx512 RENAME_FUNC_NAME(karatzuba_add3_avx512)
#define gf2x_sqr_pclmul RENAME_FUNC_NAME(gf2x_sqr_pclmul)
#define gf2x_sqr_vpclmul RENAME_FUNC_NAME(gf2x_sqr_vpclmul)
#define k_sqr_avx2 RENAME_FUNC_NAME(k_sqr_avx2)
#define k_sqr_avx512 RENAME_FUNC_NAME(k_sqr_avx512)
#define gf2x_red_avx2 RENAME_FUNC_NAME(gf2x_red_avx2)
#define gf2x_red_avx512 RENAME_FUNC_NAME(gf2x_red_avx512)
#define gf2x_mod_add RENAME_FUNC_NAME(gf2x_mod_add)
#define gf2x_mod_inv RENAME_FUNC_NAME(gf2x_mod_inv)
#define gf2x_mod_mul_with_ctx RENAME_FUNC_NAME(gf2x_mod_mul_with_ctx)
#define gf2x_ctx_init RENAME_FUNC_NAME(gf2x_ctx_init)

#define get_seeds RENAME_FUNC_NAME(get_seeds)
#define sample_uniform_r_bits RENAME_FUNC_NAME(sample_uniform_r_bits)
#define sample_indices_fisher_yates RENAME_FUNC_NAME(sample_indices_fisher_yates)
#define generate_sparse_rep_for_sk RENAME_FUNC_NAME(generate_sparse_rep_for_sk)
#define generate_error_vector RENAME_FUNC_NAME(generate_error_vector)
#define generate_secret_key RENAME_FUNC_NAME(generate_secret_key)

#define secure_set_bits_port RENAME_FUNC_NAME(secure_set_bits_port)
#define secure_set_bits_avx2 RENAME_FUNC_NAME(secure_set_bits_avx2)
#define secure_set_bits_avx512 RENAME_FUNC_NAME(secure_set_bits_avx512)
#define sampling_ctx_init RENAME_FUNC_NAME(sampling_ctx_init)

#define function_h RENAME_FUNC_NAME(function_h)
#define get_threshold RENAME_FUNC_NAME(get_threshold)







#endif //__FUNCTIONS_RENAMING_H_INCLUDED__
