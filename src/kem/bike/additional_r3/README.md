Additional implementation of BIKE (Bit Flipping Key Encapsulation) 
------------------------------------------------------------------

This package is an "Additional Optimized" implementation of the 
Key Encapsulation Mechanism (KEM) [BIKE](https://bikesuite.org). 

It was developed and is maintained by Nir Drucker, Shay Gueron, and Dusan Kostic,
(drucker.nir@gmail.com, shay.gueron@gmail.com, dkostic@protonmail.com).

BIKE is a submission to the 
[Post-Quantum Cryptography Standardization project](http://csrc.nist.gov/projects/post-quantum-cryptography).

The official BIKE website is: https://bikesuite.org. 
This package corresponds to the specification document 
["BIKE_Round_3.2020.05.03.1.pdf"](https://bikesuite.org/files/v4.0/BIKE_Spec.2020.05.03.1.pdf).

The package includes implementations for several CPU architectures.
In particular, it can be compiled for a 64-bit ARM and for x86 processors.
ARM architectures are supported by a fully portable implementation written in C.
When the code is compiled for x86 processors, the resulting binary contains
the following implementations:
- Fully portable
- Optimized for AVX2 and AVX512 instruction sets 
- Optimized for CPUs that support PCLMULQDQ, and the latest Intel
  vector-PCLMULQDQ instruction.

When the package is used on an x86 CPU, it automatically (in runtime) detects 
the CPU capabilities and runs the fastest available code path, based on the
detected capabilities. The fully portable version, which is built by default,
requires OpenSSL. The library can also be compiled in a "stand-alone" mode,
without OpenSSL, but only for a processor that supports AES-NI and AVX
instructions. This mode can be enabled by a compilation flag described below.

The package includes testing and it uses the KAT generation utilities provided
by NIST.

All the functionalities in the package are implemented in constant-time,
which means that: 
- No branch depends on a secret piece of information; 
- All the memory access patters are independent of secret information.

The optimizations in this package use, among other techniques, algorithms
presented in the following papers:
- Nir Drucker, Shay Gueron, 
  "A Toolbox for Software Optimization of QC-MDPC Code-Based Cryptosystems."
  Journal of Cryptographic Engineering, January 2019, 1–17
  https://doi.org/10.1007/s13389-018-00200-4.

- Chou, T.: QcBits: Constant-Time Small-Key Code-Based Cryptography. In: 
  Gier-lichs, B., Poschmann, A.Y. (eds.) Cryptographic Hardware and
  Embedded Systems– CHES 2016. pp. 280–300. Springer Berlin Heidelberg, 
  Berlin, Heidelberg (2016)

- Guimarães, Antonio, Diego F Aranha, and Edson Borin. 2019.
  “Optimized Implementation of QC-MDPC Code-Based Cryptography.”
  Concurrency and Computation: Practice and Experience 31 (18):
  e5089. https://doi.org/10.1002/cpe.5089.

The GF2X inversion in this package is based on:
- Nir Drucker, Shay Gueron, and Dusan Kostic. 2020.
  "Fast polynomial inversion for post quantum QC-MDPC cryptography". 
  Cryptology ePrint Archive, 2020. https://eprint.iacr.org/2020/298.pdf

The analysis of the constant-time BGF decoder used in this package is given in:
- Nir Drucker, Shay Gueron, and Dusan Kostic. 2019.
  “On Constant-Time QC-MDPC Decoding with Negligible Failure Rate.”
  Cryptology ePrint Archive, 2019. https://eprint.iacr.org/2019/1289.
- Nir Drucker, Shay Gueron, and Dusan Kostic. 2019.
  “QC-MDPC decoders with several shades of gray.”
  Cryptology ePrint Archive, 2019. https://eprint.iacr.org/2019/1423

## License
This project is licensed under the Apache-2.0 License.

Dependencies
------------
This package requires
- CMake 3 and above
- An installation of OpenSSL for the KAT testing

BUILD
-----

To build the directory first create a working directory
```
mkdir build
cd build
```

Then, run CMake and compile
```
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

By default the package is compiled with parameters defined for NIST security
Level-1 (64-bit quantum security). The default compilation options require
OpenSSL to be available on the system. The "stand-alone" compilation, which can
be enabled by a flag, assumes that AES_NI and POPCNT instructions are available.

Additional compilation flags:
 - USE_NIST_RAND            - Using the DRBG of NIST and generate the KATs.
 - STANDALONE_IMPL          - Stand-alone implementation that doesn't depend on
                              any external library.
 - OPENSSL_DIR              - Set the path of the OpenSSL include/lib
                              directories (required only if OpenSSL is not
                              installed in usual directories).
 - FIXED_SEED               - Using a fixed seed, for debug purposes.
 - RDTSC                    - Benchmark the algorithm (results in CPU cycles).
 - VERBOSE                  - Add verbose (level: 1-4 default: 1).
 - NUM_OF_TESTS             - Set the number of tests (keygen/encaps/decaps)
                              to run (default: 1).
 - LEVEL                    - Security level 1 or 3.
 - ASAN/TSAN/MSAN/UBSAN     - Enable the associated clang sanitizer.
 
To clean - remove the `build` directory. Note that a "clean" is required prior
to compilation with modified flags.

To format (`clang-format-9` or above is required):

`make format`

To use clang-tidy (`clang-tidy-9` is required):

```
CC=clang-9 cmake -DCMAKE_C_CLANG_TIDY="clang-tidy-9;--fix-errors;--format-style=file" ..
make
```

Before committing code, please test it using
`tests/run_tests.sh 0`
This will run all the sanitizers.

The package was compiled and tested with clang (version 10.0.0) in 64-bit mode,
on a Linux (Ubuntu 20.04) on Intel Xeon (x86) and Graviton 2 (ARM) processors.
The x86 tests are done with Intel SDE which can emulate any Intel CPU.  
Compilation on other platforms may require some adjustments.

KATs
----
The KATs are located in the `tests/kats/` directory.
