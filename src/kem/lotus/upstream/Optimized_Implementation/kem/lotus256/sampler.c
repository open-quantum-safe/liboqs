/**
 * @file sampler.c
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief An implementation of uniform and discrete Gaussian samplers.
 */

#include "type.h"
#include "param.h"
#include "crypto.h"
#include "randombytes.h"
#include "kydg_sampler_const.h"
#include "pack.h"

#include <string.h>

/** @brief internal state of csprng */
static LOTUS_SYMENC_ctx *csprng_ctx = NULL;


#ifdef _MSC_VER
/** @brief __builtin_popcount() wrappers for VCs */
unsigned int __builtin_popcount(const unsigned int x){
  return __popcnt(x);
}
#endif

/**
 * @brief Sampling 1 byte from randomness pool
 * @param[in,out] randomness randomness pool, filled when empty
 * @param[in,out] idx index to indicate random-bit in the pool which have not been used yet.
 * @return a random byte
 */
U8 csprng_sample_byte(U8 *randomness, U32 *idx){
  U32 ret;
  /* fill the pool if exhausted */
  if(*idx + 8 >= _LOTUS_RANDOMNESS_BITS){
    crypto_symenc_keystream(csprng_ctx, randomness, _LOTUS_RANDOMNESS_BYTES);
    *idx = 0;
  }
  ret = *((U32*)(randomness + (*idx >> 3)));
  ret >>= (*idx & 0x07);
  *idx += 8;
  return ret;
}

/**
 * @brief Sampling 1 bit from randomness pool
 * @param[in,out] randomness randomness pool, filled when empty
 * @param[in,out] idx index to indicate random-bit in the pool which have not been used yet.
 * @return a random bit (0 or 1)
 */
U8 csprng_sample_bit(U8 *randomness, U32 *idx){
  U8 ret;
  /* fill the pool if exhausted */
  if(*idx >= _LOTUS_RANDOMNESS_BITS){
    crypto_symenc_keystream(csprng_ctx, randomness, _LOTUS_RANDOMNESS_BYTES);
    *idx = 0;
  }
  ret = (randomness[*idx >> 3] >> (*idx & 0x07)) & 1;
  (*idx)++;
  return ret;
}

/**
 * @brief sign extension using a random bit
 * @param[in] r input unsigned discrete gaussian sample
 * @param[in,out] randomness randomness pool
 * @param[in,out] idx index to indicate random-bit in the pool which have not been used yet.
 * @return signed discrete gaussian sample (in Zq = [0, q))
 */
U16 extend_sign_with_random_bit(const U16 r, U8 *randomness, U32 *idx){
  U16 ret[2];
  ret[0] = r;
  ret[1] = (-r) & (_LOTUS_LWE_MOD - 1);
  return ret[csprng_sample_bit(randomness, idx)];
}

/**
 * @brief calculate row index of probability matrix by scanning j-th column
 * @param[in] d distance from current nodes to the most right side intermediate node (d < 0)
 * @param[in] j column index of probability matrix
 * @return row index, which is equivalent to unsigned gaussian sample
 */
U16 scan_bit_and_output(long long d, const int j){
  int t;
  U32 p = _LOTUS_KYDG_SAMPLER_L1_pMat[j];
  for(t = 32 - 1; d < -1; --t) d += (p >> t) & 1;
  while(!((p >> t) & 1)) --t;
  return 32 - t - 1;
}

/**
 * @brief Efficient Knuth-Yao discrete Gaussian sampler from [R. Clercq, S. Roy, F. Vercauteren, and I. Verbauwhede, "Efficient software implementaiton of ring-LWE encryption", DATE '15], with a fixed deviation.
 * @param[in,out] randomness randomness pool
 * @param[in,out] idx index to indicate randoms-bit in the pool which have not been used yet.
 * @return a Zq element sampled from discrete Gaussian distribution
 * @note This is NOT constant-time algorithm, and if it should be, use a sampler from 
 * [D. Micciancio and M. Walter, "Gaussian sampling over the integers: efficient, generic, constant-time", CRYPTO 2017].
*/
U16 sample_unit_discrete_gaussian(U8 *randomness, U32 *idx){
  const U8 msb = 0x80;
  U8 coin;
  U16 r;
  U32 p;
  int j, d;

  while(1){
    coin = csprng_sample_byte(randomness, idx);
    r = _LOTUS_KYDG_SAMPLER_LUT[coin];
    if(!(r & msb)) return extend_sign_with_random_bit(r, randomness, idx);

    d = r ^ msb;
    for(j = 0; j < _LOTUS_KYDG_SAMPLER_L1_WEIGHTDEPTH; ++j){
      coin = csprng_sample_bit(randomness, idx);
      d = (d << 1) + coin;
      d -= _LOTUS_KYDG_SAMPLER_L1_weight[j];
      if(d < 0){
        r = scan_bit_and_output(d, j);
        return extend_sign_with_random_bit(r, randomness, idx);
      }
    }

    for(; j < _LOTUS_KYDG_SAMPLER_L1_NCOL; ++j){
      coin = csprng_sample_bit(randomness, idx);
      d = (d << 1) + coin;
      p = _LOTUS_KYDG_SAMPLER_L1_pMat[j];
      d -= __builtin_popcount(p);
      if(d < 0){
        r = scan_bit_and_output(d, j);
        return extend_sign_with_random_bit(r, randomness, idx);
      }
    }
  }
}

/**
 * @brief Initialize internal state of csprng
 */
void sampler_init(){
  U8 key[_LOTUS_SYMENC_KEY_BYTES];
  randombytes(key, _LOTUS_SYMENC_KEY_BYTES);
  csprng_ctx = crypto_symenc_keysetup(csprng_ctx, key);
}

/**
 * @brief Feed a seed to csprng
 * @param[in] seed seed to feed, assumed to have _LOTUS_SEED_BYTES size
 */
void sampler_set_seed(const U8 *seed){
  csprng_ctx = crypto_symenc_keysetup(csprng_ctx, seed);
}

/**
 * @brief Sample Zq elements at uniformly random
 * @param[out] ret_array Zq-element array to be sampled
 * @param[in] len length of ret_array
 */
void sample_uniform(U16 *ret_array, const U32 len){
  unsigned char buf[208];
  U32 i;
  for(i = 0; i < len; i += 128){
    crypto_symenc_keystream(csprng_ctx, buf, 208);
    unpack_128elems(ret_array + i, buf);
  }
}

/**
 * @brief Sample Zq elements from discrete Gaussian distribution independently
 * @param[out] ret_array Zq-element array to be sampled
 * @param[in] len length of ret_array
 */
void sample_discrete_gaussian(U16 *ret_array, const U32 len){
  U8 randomness[_LOTUS_RANDOMNESS_BYTES + 4]; /* additional 4 bytes are for accepting U32 access */
  U32 idx = _LOTUS_RANDOMNESS_BITS, i;
  for(i = 0; i < len; ++i){
    ret_array[i] = sample_unit_discrete_gaussian(randomness, &idx);
  }
}
