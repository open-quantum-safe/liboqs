# MemSan
This directory contains the files required to execute MemSan's tooling for liboqs constant-time testing.

MemSan handles false-positive warnings by storing specific functions into `.txt` files within the `false_positives/` directory. These files are passed as parameters of the `-fsanitize-ignorelist` flag during compilation, successfully disregarding those warnings that are cathegorized as not constant-time issues after review.

## Compiling liboqs with MemSan
MemSan is inherently included with the clang compiler, so no requirement besides installing clang is needed. However, it does require certain workarounds to mark memory as uninitialized when building liboqs. Nonetheless, this process is directly implemented by using the `build()` function within the `ct_test.sh` script.

The `rng_poison_msan.c` file is used to overwrite the original `OQS_randombytes()` and mark secret variables as uninitialized. Note that the actual value is filled with a non-zero buffer (0xA5) to prevent masking of bugs, as well as eliminating any random noise in the heap memory.

For MemSan liboqs testing, it is necessary to compile liboqs with new versions of `tests/CMakeLists.txt`, `tests/test_kem.c`, `tests_sig.c`, which can be found under the repository ct-tools/memsan. These new versions allow for memory "poisoning" during the "randombytes" function in `CMakeLists.txt`, and memory "unpoisioning" of public keys in `test_kem.c` and `test_sig.c`.

Therefore, `build()` replaces the original files with the "poisoned" ones during compilation, so that MemSan testing can successfully take place. Once liboqs compilation is ready, the script replaces the original files with a backup that was temporarily stored so that liboqs is unchanged after constant-time testing with MemSan is finished.

## Algorithms Testing
Because of how many warnings are output, it is not feasible to store all the warnings in terms of memory and runtime. Therefore,  the `test()` function in `ct_test.sh` handles MemSan's output as follows:
- It captures the first SUMMARY line of each warning, which contains key details (file, line, issue type), and stores these in log files.
- Only unique SUMMARY lines are retained, avoiding duplication from repeated warnings during execution.

The testing framework currently skips all SPHINCS and SLH-DSA tests due to the execessive length of time they require to execute.

## False positive handling
MemSan follows a similar suppression mechanism to that of Valgrind-Varlat. Users can specify entities to ignore during testing by listing them in a suppression file, using a prefix that defines the entity's type. For this framework, the `fun:` prefix is used (although there are others too), since the observed false-positives originate from specific functions. The suppression file is then passed to clang at compile-time using the `-fsanitize-ignorelist` flag.

MemSan's output also includes a full stack trace leading to the root cause. To successfully suppress a warning, the suppression file must target the exact function listed in the report's SUMMARY line. For example, given an output of the form:

```text
==9793==WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 0x7eb79d2bf3f2 in sampling.c:62:9
    #1 0x7eb79d2bf3f2 in sampling.c:138:10
    #2 0x7eb79d2bf3f2 in sampling.c:174:12
    #3 0x7eb79d2bd57d in indcpa.c:278:5
    #4 0x7eb79d2bd57d in indcpa.c:508:3
    #5 0x7eb79d2be04a in kem.c:416:9
    #6 0x5d25bce8bce2 in test_kem.c:63:7
    #7 0x5d25bce8bce2 in test_kem.c:293:15
    #8 0x5d25bce8b4a5 in test_kem.c:391:12
    #9 0x7eb79ce9caa3 in pthread_create.c:447:8
    #10 0x7eb79cf29c6b in clone3.S:78

SUMMARY: MemorySanitizer: use-of-uninitialized-value /home/pablogf/liboqs/src/kem/ml_kem/mlkem-native_ml-kem-512_ref/mlkem/src/sampling.c:62:9 in mlk_rej_uniform_c
==9793==WARNING: MemorySanitizer: use-of-uninitialized-value
```

The framework will disregard this warning on future executions by including the following line in the suppression file:
```text
fun:mlk_rej_uniform_c
```
MemSan also enables the use of the wildcard (*) within the suppression files.

Each family of algorithms will have a specific suppression block listing the functions that output false-positives. The framework automatically includes all suppression files within the respective subdirectories so that known false positives are not returned during testing.

For further information see https://clang.llvm.org/docs/MemorySanitizer.html and https://clang.llvm.org/docs/SanitizerSpecialCaseList.html

## Dependencies
Remember to install the required dependencies before testing:

`sudo apt install -y clang clang-tools`