/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: configuration file
*********************************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_


// Definition of operating system

#define OS_WIN       1
#define OS_NIX       2

#if defined(WINDOWS)            // Microsoft Windows
    #define OS_TARGET OS_WIN
#elif defined(NIX)              // Unix-like operative systems
    #define OS_TARGET OS_NIX 
#else
    #error -- "Unsupported OS"
#endif


// Definition of compiler

#define COMPILER_VC      1
#define COMPILER_GCC     2
#define COMPILER_CLANG   3

#if defined(_MSC_VER)           // Microsoft Visual C compiler
    #define COMPILER COMPILER_VC
#elif defined(__GNUC__)         // GNU GCC compiler
    #define COMPILER COMPILER_GCC   
#elif defined(__clang__)        // Clang compiler
    #define COMPILER COMPILER_CLANG
#else
    #error -- "Unsupported COMPILER"
#endif


// Definition of the targeted architecture and basic data types
    
#define TARGET_AMD64        1
#define TARGET_x86          2
#define TARGET_ARM          3

#if defined(_AMD64_)
    #define TARGET TARGET_AMD64 
#elif defined(_X86_)
    #define TARGET TARGET_x86
#elif defined(_ARM_)
    #define TARGET TARGET_ARM
#else
    #error -- "Unsupported ARCHITECTURE"
#endif


#if defined(WINDOWS)
    #define ALIGN_HEADER(N) __declspec(align(N))
    #define ALIGN_FOOTER(N) 
#else
    #define ALIGN_HEADER(N)
    #define ALIGN_FOOTER(N) __attribute__((aligned(N)))
#endif


// Selecting implementation: fast, generic or reference
#if defined(_FAST_)    // The "fast" implementation requires support for AVX2 and AES_NI instructions 
    #define USE_AVX2 
    #define AES_ENABLE_NI 
    #define USE_FAST
#elif defined(_FAST_GENERIC_)
    #define USE_FAST_GENERIC
#elif defined(_REFERENCE_)
    #define USE_REFERENCE
#else
    #error -- unsupported implementation
#endif


// Defining method for generating matrix A
#if defined(_AES128_FOR_A_)
    #define USE_AES128_FOR_A
#elif defined(_CSHAKE128_FOR_A_)
    #define USE_CSHAKE128_FOR_A
#else
    ##error -- missing method for generating matrix A
#endif


// Selecting use of OpenSSL's AES functions
#if defined(_USE_OPENSSL_)
    #define USE_OPENSSL
#endif


#endif
