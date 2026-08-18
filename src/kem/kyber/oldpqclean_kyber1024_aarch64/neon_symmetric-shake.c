
/*
 * This file was originally licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * at https://github.com/GMUCERG/PQC_NEON/blob/main/neon/kyber or
 * public domain at https://github.com/cothan/kyber/blob/master/neon
 *
 * We choose
 * CC0 1.0 Universal or the following MIT License for this file.
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "fips202x2.h"
#include "symmetric.h"

/*************************************************
* Name:        kyber_shake128_absorb
*
* Description: Absorb step of the SHAKE128 specialized for the Kyber context.
*
* Arguments:   - keccak_state *state: pointer to (uninitialized) output
*                                     Keccak state
*              - const uint8_t *seed: pointer to KYBER_SYMBYTES input
*                                     to be absorbed into state
*              - uint8_t i            additional byte of input
*              - uint8_t j            additional byte of input
**************************************************/
void neon_kyber_shake128_absorb(keccakx2_state *state,
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t x1, uint8_t x2,
                                uint8_t y1, uint8_t y2)
{
  unsigned int i;
  uint8_t extseed1[KYBER_SYMBYTES+2+6];
  uint8_t extseed2[KYBER_SYMBYTES+2+6];

  for(i=0;i<KYBER_SYMBYTES;i++){
    extseed1[i] = seed[i];
    extseed2[i] = seed[i];
  }
  extseed1[KYBER_SYMBYTES  ] = x1;
  extseed1[KYBER_SYMBYTES+1] = y1;

  extseed2[KYBER_SYMBYTES  ] = x2;
  extseed2[KYBER_SYMBYTES+1] = y2;

  shake128x2_absorb(state, extseed1, extseed2, KYBER_SYMBYTES+2);
}

/*************************************************
* Name:        kyber_shake256_prf
*
* Description: Usage of SHAKE256 as a PRF, concatenates secret and public input
*              and then generates outlen bytes of SHAKE256 output
*
* Arguments:   - uint8_t *out:       pointer to output
*              - size_t outlen:      number of requested output bytes
*              - const uint8_t *key: pointer to the key
*                                    (of length KYBER_SYMBYTES)
*              - uint8_t nonce:      single-byte nonce (public PRF input)
**************************************************/
void neon_kyber_shake256_prf(uint8_t *out1, uint8_t *out2,
                             size_t outlen,
                             const uint8_t key[KYBER_SYMBYTES],
                             uint8_t nonce1, uint8_t nonce2) {
    unsigned int i;
    uint8_t extkey1[KYBER_SYMBYTES + 1 + 15];
    uint8_t extkey2[KYBER_SYMBYTES + 1 + 15];

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        extkey1[i] = key[i];
        extkey2[i] = key[i];
    }

    extkey1[i] = nonce1;
    extkey2[i] = nonce2;

    shake256x2(out1, out2, outlen, extkey1, extkey2, KYBER_SYMBYTES + 1);
}
