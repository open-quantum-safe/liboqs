
/*
 * CC0 1.0 Universal or the following MIT License
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
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

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define ARRAY_N 256

#define NTT_N 256
#define LOGNTT_N 8

// root of unity: 1753


// Q1
#define Q1 8380417
// omegaQ1 = 1753 mod Q1
#define omegaQ1 1753
// invomegaQ1 = omegaQ^{-1} mod Q1
#define invomegaQ1 731434
// R = 2^32 below
// RmodQ1 = 2^32 mod^{+-} Q1
#define RmodQ1 (-4186625)
// Q1prime = Q1^{-1} mod^{+-} 2^32
#define Q1prime 58728449
// invNQ1 = NTT_N^{-1} mod Q1
#define invNQ1 8347681

// invNQ1R2modQ1 = -NTT_N^{-1} 2^32 2^32 mod^{+-} Q1 below
#define invNQ1R2modQ1 (-41978)
// invNQ1R2modQ1_prime = invNQ1R2modQ1 (Q1^{-1} mod^{+-} 2^32) mod^{+-} 2^32
#define invNQ1R2modQ1_prime 8395782
// invNQ1R2modQ1_prime_half = (invNQ1R2modQ1 / 2) (Q1^{-1} mod^{+-} 2^32) mod^{+-} 2^32
#define invNQ1R2modQ1_prime_half 4197891
// invNQ1R2modQ1_doubleprime = (invNQ1R2modQ1_prime Q1 - invNQ1R2modQ1) / 2^32
#define invNQ1R2modQ1_doubleprime 16382

// invNQ1_final_R2modQ1 = -invNQ1R2modQ1 invomegaQ1^{128} mod q
#define invNQ1_final_R2modQ1 4404704
// invNQ1_final_R2modQ1_prime = invNQ1_final_R2modQ1 (Q1^{-1} mod^{+-} 2^32) mod^{+-} 2^32
#define invNQ1_final_R2modQ1_prime (-151046688)
// invNQ1_final_R2modQ1_prime_half = (invNQ1_final_R2modQ1 / 2) (Q1^{-1} mod^{+-} 2^32) mod^{+-} 2^32
#define invNQ1_final_R2modQ1_prime_half (-75523344)
// invNQ1_final_R2modQ1_doubleprime = (invNQ1_final_R2modQ1_prime Q1 - invNQ1_final_R2modQ1) / 2^32
#define invNQ1_final_R2modQ1_doubleprime (-294725)

// RmodQ1_prime = -(RmodQ1 + Q1) Q1prime mod^{+-} 2^32
#define RmodQ1_prime 512
// RmodQ1_prime_half = ( -(RmodQ1 + Q1) / 2) Q1prime mod^{+-} 2^32
#define RmodQ1_prime_half 256
// RmodQ1_doubleprime = (RmodQ1_prime Q1 - RmodQ1_prime ) / 2^32
#define RmodQ1_doubleprime 1

#endif





