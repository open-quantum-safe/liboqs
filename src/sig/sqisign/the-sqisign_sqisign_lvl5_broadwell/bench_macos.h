// WARNING: must be run as root on an M1 device
// WARNING: fragile, uses private apple APIs
// currently no command line interface, see variables at top of main

/*
no warranty; use at your own risk - i believe this code needs
some minor changes to work on some later hardware and/or software revisions,
which is unsurprising given the use of undocumented, private APIs.
------------------------------------------------------------------------------
This code is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2020 Dougall Johnson
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

/*
  Based on https://github.com/travisdowns/robsize
  Henry Wong <henry@stuffedcow.net>
  http://blog.stuffedcow.net/2013/05/measuring-rob-capacity/
  2014-10-14
*/

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define KPERF_LIST                                                                                 \
    /*  ret, name, params */                                                                       \
    F(int, kpc_force_all_ctrs_set, int)                                                            \
    F(int, kpc_set_counting, uint32_t)                                                             \
    F(int, kpc_set_thread_counting, uint32_t)                                                      \
    F(int, kpc_set_config, uint32_t, void *)                                                       \
    F(int, kpc_get_thread_counters, int, unsigned int, void *)

#define F(ret, name, ...)                                                                          \
    typedef ret name##proc(__VA_ARGS__);                                                           \
    static name##proc *name;
KPERF_LIST
#undef F

#define CFGWORD_EL0A64EN_MASK (0x20000)

#define CPMU_CORE_CYCLE 0x02

#define KPC_CLASS_FIXED (0)
#define KPC_CLASS_CONFIGURABLE (1)

#define COUNTERS_COUNT 10
#define KPC_MASK ((1u << KPC_CLASS_CONFIGURABLE) | (1u << KPC_CLASS_FIXED))
static uint64_t g_config[COUNTERS_COUNT];
static uint64_t g_counters[COUNTERS_COUNT];

static void
macos_configure_rdtsc()
{
    if (kpc_force_all_ctrs_set(1)) {
        printf("kpc_force_all_ctrs_set failed\n");
        return;
    }

    if (kpc_set_config(KPC_MASK, g_config)) {
        printf("kpc_set_config failed\n");
        return;
    }

    if (kpc_set_counting(KPC_MASK)) {
        printf("kpc_set_counting failed\n");
        return;
    }

    if (kpc_set_thread_counting(KPC_MASK)) {
        printf("kpc_set_thread_counting failed\n");
        return;
    }
}

static void
macos_init_rdtsc()
{
    void *kperf =
        dlopen("/System/Library/PrivateFrameworks/kperf.framework/Versions/A/kperf", RTLD_LAZY);
    if (!kperf) {
        printf("kperf = %p\n", kperf);
        return;
    }
#define F(ret, name, ...)                                                                          \
    name = (name##proc *)(intptr_t)(dlsym(kperf, #name));                                          \
    if (!name) {                                                                                   \
        printf("%s = %p\n", #name, (void *)(intptr_t)name);                                        \
        return;                                                                                    \
    }
    KPERF_LIST
#undef F

    g_config[0] = CPMU_CORE_CYCLE | CFGWORD_EL0A64EN_MASK;

    macos_configure_rdtsc();
}

static uint64_t
macos_rdtsc(void)
{
    if (kpc_get_thread_counters(0, COUNTERS_COUNT, g_counters)) {
        printf("kpc_get_thread_counters failed\n");
        return 1;
    }
    return g_counters[2];
}
