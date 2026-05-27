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

## Dependencies
Remember to install the required dependencies before testing:

`sudo apt install -y clang clang-tools`