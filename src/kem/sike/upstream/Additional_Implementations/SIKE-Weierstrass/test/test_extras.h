/********************************************************************************************
* Supersingular Isogeny Key Encapsulation Library
*
*
* Abstract: utility header file for tests
*
*********************************************************************************************/

#ifndef __TEST_EXTRAS_H__
#define __TEST_EXTRAS_H__

#include <stdint.h>

#define PASSED    0
#define FAILED    1

#define OS_UNIX 1
#define OS_WIN 2
#define OS_OTHER 3

#define TARGET_ARM64 1
#define TARGET_ARM 2
#define TARGET_AMD64 3
#define TARGET_x86 4
#define TARGET_OTHER 5

#if (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
#define print_unit printf("nsec\n");
#else
#define print_unit printf("cycles\n");
#endif


// Access system counter for benchmarking
int64_t cpucycles(void);

#endif
