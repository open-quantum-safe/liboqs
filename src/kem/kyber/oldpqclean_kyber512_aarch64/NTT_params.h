
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

#define NTT_N 128
#define LOGNTT_N 7

// Q1
#define Q1 3329
// omegaQ1 = 17 mod Q1
#define omegaQ1 17
// invomegaQ1 = omegaQ^{-1} mod Q1
#define invomegaQ1 1175
// R = 2^15 below
// RmodQ1 = 2^15 mod^{+-} Q1
#define RmodQ1 (-522)
// Q1prime = Q1^{-1} mod^{+-} 2^15
#define Q1prime (-3327)
// invNQ1 = NTT_N^{-1} mod Q1
#define invNQ1 3303
// R2modQ1 = 2^16 mod^{+-} Q1
#define R2modQ1 (-1044)
// Q1prime2 = -Q1^{-1} mod^{+-} 2^16
#define Q1prime2 3327

// R3modQ1 = -2^32 mod^{+-} Q1
#define R3modQ1 (-1353)
// R3modQ1_prime = (R3modQ1 + Q1) (Q1^{-1} mod^{+-} 2^16) mod^{+-} 2^16
#define R3modQ1_prime (-20552)
// R3modQ1_prime_half = ( (R3modQ1 + Q1) / 2) (Q1^{-1} mod^{+-} 2^16) mod^{+-} 2^16
#define R3modQ1_prime_half (-10276)
// R3modQ1_doubleprime (R3modQ1_prime Q1 - (R3modQ1 + Q1)) / 2^16
#define R3modQ1_doubleprime (-1044)

// invNQ1_R3modQ1 = -NTT_N^{-1} 2^32 mod^{+-} Q1
#define invNQ1_R3modQ1 (-1441)
// invNQ1_R3modQ1_prime = (invNQ1_R3modQ1 + Q1) (Q1^{-1} mod^{+-} 2^16) mod^{+-} 2^16
#define invNQ1_R3modQ1_prime 10080
// invNQ1_R3modQ1_prime_half = ( (invNQ1_R3modQ1 + Q1) / 2) (Q1^{-1} mod^{+-} 2^16) mod^{+-} 2^16
#define invNQ1_R3modQ1_prime_half 5040
// invNQ1_R3modQ1_doubleprime = (invNQ1_R3modQ1_prime Q1 - (invNQ1_R3modQ1 + Q1)) / 2^16
#define invNQ1_R3modQ1_doubleprime 512

// invNQ1_final_R3modQ1 = -invomegaQ1^{64} NTT_N^{-1} 2^32 mod^{+-} Q1
#define invNQ1_final_R3modQ1 1397
// invNQ1_final_R3modQ1_prime ( invNQ1_final_R3modQ1 - Q1) (Q1^{-1} mod^{+-} 2^16) mod^{+-} 2^16
#define invNQ1_final_R3modQ1_prime 5236
// invNQ1_final_R3modQ1_prime_half ( (invNQ1_final_R3modQ1 - Q1) / 2) (Q1^{-1} mod^{+-} 2^16) mod^{+-} 2^16
#define invNQ1_final_R3modQ1_prime_half 2618
// invNQ1_final_R3modQ1_doubleprime (invNQ1_final_R3modQ1_prime Q1 - (invNQ1_final_R3modQ1 - Q1)) / 2^16
#define invNQ1_final_R3modQ1_doubleprime 266

// RmodQ1Q1prime = RmodQ1 * Q1prime mod^{+-} 2^16
#define RmodQ1Q1prime 32758

// roundRdivQ1 = round (2^26 / Q1)
#define roundRdivQ1 20159

#endif





