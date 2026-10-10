# Fuzzing

Fuzz testing is an automated software testing method that injects invalid,
malformed, or unexpected inputs to reveal defects and vulnerabilities. A
fuzzing tool monitors the system for exceptions like crashes, information
leakage, or errors, helping developers identify and fix bugs and security
loopholes.

## Current state of fuzzing in liboqs
- [ ] kem 
  - [ ] bike
  - [ ] classic_mceliece
  - [ ] frodokem
  - [ ] hqc
  - [ ] kyber
  - [ ] ml_kem
  - [ ] ntruprime
- [ ] sig
  - [x] falcon
  - [x] mayo
  - [x] ml_dsa
- [ ] sig_stfl
  - [ ] lms
  - [ ] sig_stfl
  - [ ] xmss

## Building fuzz targets

Building fuzz tests is very similar to building normally with some optional
steps to target different types of bugs. The most basic ways to build the fuzz
tests is as follows;

```bash
mkdir build
cmake -S . -B build -GNinja \
    -DOQS_BUILD_FUZZ_TESTS=ON \
    -DCMAKE_BUILD_TYPE=Debug
ninja -C build
```

Fuzz target binaries will be located under `build/tests/fuzz`.

MacOS's system default AppleClang might not ship with LLVM LibFuzzer. One way
to work around it is to install LLVM toolchain using Homebrew, then point cmake
to Homebrew's clang:

```bash
cmake -S . -B build -GNinja \
    -DOQS_BUILD_FUZZ_TESTS=ON \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_C_COMPILER="$(brew --prefix llvm@22)/bin/clang"
```

## Fuzz target coverage

- [ ] KEM round trip
- [ ] SIG round trip
- [ ] KEM encap public key
- [ ] KEM decap ciphertext
- [ ] SIG verify signature and public key

## Sanitizers
It is a common pattern to combine fuzzing with various sanitizers to catch
different bugs. One of the simpler sanitizers is the fuzzing sanitizer, which
will instrument the code for coverage driven fuzzing. To enable this simply add
this to your environment variables before configuring cmake;

```
export CFLAGS=-fsanitize=fuzzer-no-link
```

It is common to combine the fuzzer sanitizer with either the
[address](https://clang.llvm.org/docs/AddressSanitizer.html) or the 
[undefined behaviour sanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html).
To add these simply add the relevant flags to BOTH the CFLAGS and LDFLAGS e.g.

```
export CFLAGS=-fsanitize=fuzzer-no-link,address
export LDFLAGS=-fsanitize=address
```

Then rerun cmake as normal i.e.

```bash
mkdir build && cd build
cmake -GNinja ..  -DOQS_BUILD_FUZZ_TESTS=ON
ninja -j$(nproc)
```
