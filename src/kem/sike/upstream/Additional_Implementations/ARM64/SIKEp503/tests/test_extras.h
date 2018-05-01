/********************************************************************************************
* Supersingular Isogeny Key Encapsulation Library
*
* Abstract: utility header file for tests
*********************************************************************************************/  

#ifndef __TEST_EXTRAS_H__
#define __TEST_EXTRAS_H__
    
#include "../config.h"

#define PASSED    0
#define FAILED    1


#if (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
    #define print_unit printf("nsec");
#else
    #define print_unit printf("cycles");
#endif

    
// Access system counter for benchmarking
int64_t cpucycles(void);

#endif