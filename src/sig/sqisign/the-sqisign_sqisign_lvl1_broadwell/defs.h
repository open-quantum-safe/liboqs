/***************************************************************************
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*  
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*  
*     http://www.apache.org/licenses/LICENSE-2.0
*  
* or in the "license" file accompanying this file. This file is distributed 
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
* express or implied. See the License for the specific language governing 
* permissions and limitations under the License.
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#pragma once

#include <string.h>

#ifdef __cplusplus
  #define EXTERNC extern "C"
#else
  #define EXTERNC
#endif

// For code clarity.
#define IN
#define OUT

#define ALIGN(n) __attribute__((aligned(n)))
#define _INLINE_ static inline

typedef enum
{
  SUCCESS=0,
  ERROR=1
} status_t;

#define SUCCESS 0
#define ERROR 1
#define GUARD(func) {if(SUCCESS != func) {return ERROR;}}

#if defined(__GNUC__) && __GNUC__ >= 2
static inline uint32_t CRYPTO_bswap4(uint32_t x) {
  return __builtin_bswap32(x);
}
#endif

_INLINE_ void secure_clean(OUT uint8_t *p, IN const uint32_t len)
{
#ifdef _WIN32
    SecureZeroMemory(p, len);
#else
    typedef void *(*memset_t)(void *, int, size_t);
    static volatile memset_t memset_func = memset;
    memset_func(p, 0, len);
#endif
}
