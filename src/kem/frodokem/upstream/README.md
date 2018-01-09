# FrodoKEM: Learning with Errors Key Encapsulation

This C library implements **FrodoKEM**, an IND-CCA secure key encapsulation (KEM) protocol based on the well-studied Learning with Errors (LWE) problem [1], which in turn has close connections to conjectured-hard problems on generic, "algebraically unstructured" lattices.
**FrodoKEM** is conjectured to be secure against quantum computer attacks.

Concretely, this library includes the following KEM schemes using AES128 for the generation of the public matrix "A":

* FrodoKEM-640-AES: matching the post-quantum security of AES128.
* FrodoKEM-976-AES: matching the post-quantum security of AES192.

And the following KEM schemes using cSHAKE128 for the generation of the public matrix "A":

* FrodoKEM-640-cSHAKE: matching the post-quantum security of AES128.
* FrodoKEM-976-cSHAKE: matching the post-quantum security of AES192.

The library was developed by [Microsoft Research](http://research.microsoft.com/) for experimentation purposes.

## Contents

* [`KAT folder`](KAT/): Known Answer Test (KAT) files for the KEM.
* [`src folder`](src/): C and header files. Public APIs can be found in [`api_frodo640.h`](src/api_frodo640.h) and [`api_frodo976.h`](src/api_frodo976.h).
* [`Optimized matrix operations`](src/frodo_macrify.c): optimized implementation of the matrix operations. 
* [`Reference matrix operations`](frodo_macrify_reference.c): reference implementation of the matrix operations.
* [`AES folder`](src/aes/): AES implementation.
* [`random folder`](src/random/): randombytes function using the system random number generator.
* [`sha3 folder`](src/sha3/): cSHAKE256 implementation.  
* [`Test folder`](tests/): test files.  
* [`Visual Studio folder`](VisualStudio/): Visual Studio 2015 files for compilation in Windows.
* [`Makefile`](Makefile): Makefile for compilation using the GNU GCC or clang compilers on Unix-like operative systems. 
* [`License`](LICENSE): MIT license file.
* [`Readme`](README.md): this readme file.


### Complementary crypto functions

Random values are generated with /dev/urandom on Unix-like operative systems, and CNG's BCryptGenRandom function in Windows. 
Check the folder [`random`](src/random/) for details.

The library includes standalone implementations of AES and cSHAKE. The generation of the matrix
"A" (see the specification document [1]) can be carried out with either AES128 or cSHAKE128. By
default AES128 is used.

There are two options for AES: the standalone implementation that is included in the software or
OpenSSL's AES implementation. OpenSSL's AES implementation is used by default.

## Supported Platforms

The FrodoKEM library is supported on a wide range of platforms including x64, x86 and ARM devices running Windows, Linux or macOS. 
We have tested the library with Microsoft Visual Studio 2015, GNU GCC v5.4, and clang v3.8.
See instructions below to choose an implementation option and compile on one of the supported platforms.

## Implementation Options

 The following implementation options are available:
- Reference portable implementation enabled by setting `OPT_LEVEL=REFERENCE`. 
- Optimized portable implementation enabled by setting `OPT_LEVEL=FAST_GENERIC`. 
- Optimized x64 implementation using AVX2 intrinsics and AES-NI instructions enabled by setting `ARCH=x64` and `OPT_LEVEL=FAST`.

Follow the instructions in the sections "_Instructions for Linux_" or "_Instructions for Windows_" below to configure these different implementation options.

## Instructions for Linux 

### Using AES128

By simply executing:

```sh
$ make
```

the library is compiled for x64 using gcc, and optimization level `FAST`, which uses AVX2 instrinsics. 
AES128 is used by default to generate the matrix "A". For AES, OpenSSL's AES implementation is used by default.

Testing and benchmarking results are obtained by running:

```sh
$ ./frodo640/test_KEM
$ ./frodo976/test_KEM
```

To run the implementations against the KATs, execute:

```sh
$ ./frodo640/PQCtestKAT_kem
$ ./frodo976/PQCtestKAT_kem
```

### Using cSHAKE128

By executing:

```sh
$ make GENERATION_A=CSHAKE128
```

the library is compiled for x64 using gcc, and optimization level `FAST`, which uses AVX2 instrinsics. 
cSHAKE128 is used to generate the matrix "A".

Testing and benchmarking results are obtained by running:

```sh
$ ./frodo640/test_KEM
$ ./frodo976/test_KEM
```

To run the implementations against the KATs, execute:

```sh
$ ./frodo640/PQCtestKAT_kem_cshake
$ ./frodo976/PQCtestKAT_kem_cshake
```

### Additional options

These are all the available options for compilation:

```sh
$ make CC=[gcc/clang] ARCH=[x64/x86/ARM] OPT_LEVEL=[REFERENCE/FAST_GENERIC/FAST] GENERATION_A=[AES128/CSHAKE128] USE_OPENSSL=[TRUE/FALSE]
```

Note that the `FAST` option is only available for x64 with support for AVX2 and AES-NI instructions.
The USE_OPENSSL flag specifies whether OpenSSL's AES implementation is used (`=TRUE`) or if the
standalone AES implementation is used (`=FALSE`). Therefore, this flag only applies when `GENERATION_A=
AES128` (or if `GENERATION_A` is left blank).

If OpenSSL is being used and is installed in an alternate location, use the following make options:
    
```sh
OPENSSL_INCLUDE_DIR=/path/to/openssl/include
OPENSSL_LIB_DIR=/path/to/openssl/lib
```

The program tries its best at auto-correcting unsupported configurations. 
For example, since the `FAST` implementation is currently only available for x64 doing `make ARCH=x86 OPT_LEVEL=FAST` 
is actually processed using `ARCH=x86 OPT_LEVEL=FAST_GENERIC`.

## Instructions for Windows

### Building the library with Visual Studio:

Open the solution file [`frodoKEM.sln`](VisualStudio/frodoKEM.sln) in Visual Studio, and choose either x64 or x86 from the platform menu. 
Make sure `Fast_generic` is selected in the configuration menu. Finally, select "Build Solution" from the "Build" menu. 

### Running the tests:

After building the solution file, there should be two executable files: `test_KEM640.exe` and `test_KEM976.exe`, to run tests for the KEM. 

### Using the library:

After building the solution file, add the generated `FrodoKEM-640.lib` and `FrodoKEM-976.lib` library files to the set of References for a project, 
and add [`api_frodo640.h`](src/api_frodo640.h) and [`api_frodo976.h`](src/api_frodo976.h) to the list of header files of a project.


## License

This software is licensed under the MIT License; see the LICENSE file for details.
It includes some third party modules that are licensed differently. In particular:

- `src/aes/aes_c.c`: public domain
- `src/aes/aes_ni.c`: public domain
- `src/sha3/fips202.c`: public domain
- `src/sha3/fips202x4.c`: public domain
- `src/sha3/keccak4x`: all files in this folder are public domain ([CC0](http://creativecommons.org/publicdomain/zero/1.0/)), excepting
- `src/sha3/keccak4x/brg_endian.h` which is copyrighted by Brian Gladman and comes with a BSD 3-clause license.
- `tests/ds_benchmark.h`: public domain
- `tests/PQCtestKAT_kem<#>.c`: copyrighted by Lawrence E. Bassham 
- `tests/PQCtestKAT_kem<#>_cshake.c`: copyrighted by Lawrence E. Bassham
- `tests/rng.c`: copyrighted by Lawrence E. Bassham 

## Contributors

The library was developed by Microsoft Research using as basis the Frodo implementation from [`liboqs`](https://github.com/open-quantum-safe/liboqs),
which in turn is based on [2].

Other contributors include:

- Erdem Alkim, while he was an intern with Microsoft Research.

# References

[1]  Erdem Alkim, Joppe W. Bos, Léo Ducas, Karen Easterbrook, Brian LaMacchia, Patrick Longa, Ilya Mironov, Valeria Nikolaenko, Chris Peikert, Ananth Raghunathan, and Douglas Stebila, 
"FrodoKEM: Learning With Errors Key Encapsulation". Submission to the NIST Post-Quantum Standardization project, 2017. The specification of FrodoKEM is available [`here`](https://frodokem.org/files/FrodoKEM-specification-20171130.pdf). 

[2]  Joppe W. Bos, Craig Costello, Léo Ducas, Ilya Mironov, Michael Naehrig, Valeria Nikolaenko, Ananth Raghunathan, and Douglas Stebila, 
"Frodo: Take off the ring! Practical, quantum-secure key exchange from LWE". 
ACM CCS 2016, 2016. The preprint version is available [`here`](http://eprint.iacr.org/2016/659). 

# Contributing

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
