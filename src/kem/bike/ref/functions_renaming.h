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
 *   notice, this list of conditions and the following disclaimer in the
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
 ******************************************************************************/

#ifndef __FUNCTIONS_RENAMING_H_INCLUDED__
#define __FUNCTIONS_RENAMING_H_INCLUDED__

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define RENAME_FUNC_NAME(fname) EVALUATOR(FUNC_PREFIX, fname)

#define aes_ctr_prf RENAME_FUNC_NAME(aes_ctr_prf)
#define sample_uniform_r_bits RENAME_FUNC_NAME(sample_uniform_r_bits)
#define init_aes_ctr_prf_state RENAME_FUNC_NAME(init_aes_ctr_prf_state)
#define generate_sparse_rep RENAME_FUNC_NAME(generate_sparse_rep)
#define parallel_hash RENAME_FUNC_NAME(parallel_hash)
#define getHammingWeight RENAME_FUNC_NAME(getHammingWeight)
#define decode RENAME_FUNC_NAME(decode)
#define generate_sparse_rep RENAME_FUNC_NAME(generate_sparse_rep)
#define convert2compact RENAME_FUNC_NAME(convert2compact)
#define convertByteToBinary RENAME_FUNC_NAME(convertByteToBinary)
#define convertBinaryToByte RENAME_FUNC_NAME(convertBinaryToByte)
#define keypair RENAME_FUNC_NAME(keypair)
#define decaps RENAME_FUNC_NAME(decaps)
#define encaps RENAME_FUNC_NAME(encaps)

#endif //__FUNCTIONS_RENAMING_H_INCLUDED__
