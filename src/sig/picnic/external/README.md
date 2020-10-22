Picnic: Post-Quantum Signatures
===============================

The Picnic signature scheme is a family of digital signature schemes secure against attacks by quantum computers. This repository contains an optimized implementation of these schemes. The scheme and parameter sets are specified in the [Picnic Specification Document](https://github.com/Microsoft/Picnic/blob/master/spec.pdf). The public API of the library and the serialization format is compatible with the [reference implementation](https://github.com/Microsoft/Picnic).

Research paper describing the signature scheme and the optimizations are also available:
* **Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives** Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg Zaverucha. *In Proceedings of ACM CCS 2017*. *[Cryptology ePrint Archive: Report 2017/279](https://eprint.iacr.org/2017/279)*
* **Improved Non-Interactive Zero Knowledge with Applications to Post-Quantum Signatures** Jonathan Katz and Vladimir Kolesnikov and Xiao Wang. *In Proceedings of ACM CCS 2018*. *[Cryptology ePrint Archive: Report 2018/475](https://eprint.iacr.org/2018/475)*
* **Linear Equivalence of Block Ciphers with Partial Non-Linear Layers: Application to LowMC** Itai Dinur and Daniel Kales and Angela Promitzer and Sebastian Ramacher and Christian Rechberger. *In Proceedings of Eurocrypt 2019*. *[Cryptology ePrint Archive: Report 2018/772](https://eprint.iacr.org/2018/772)*
* **Improving the Performance of the Picnic Signature Scheme** Daniel Kales and Greg Zaverucha. *[Cryptology ePrint Archive: Report 2020/427](https://eprint.iacr.org/2020/427)*


Packages
--------

Packages for Ubuntu bionic, disco, and eoan are available via a [PPA](https://launchpad.net/~s-ramacher/+archive/ubuntu/picnic/).

Building
--------

First configure the build with `cmake` and then run `make`:
```sh
mkdir build
cd build
cmake ..
make
```

The cmake based build system supports the following flags:
* ``WITH_ZKBPP``: Enable ZKB++-based Picnic instances.
* ``WITH_KKW``: Enable KKW-based Picnic instances.
* ``WITH_SIMD_OPT``: Enable SIMD optimizations.
* ``WITH_AVX2``: Use AVX2 if available.
* ``WITH_SSE2``: Use SSE2 if available.
* ``WITH_NEON``: Use NEON if available.
* ``WITH_MARCH_NATIVE``: Build with -march=native -mtune=native (if supported).
* ``WITH_LTO``: Enable link-time optimization (if supported).
* ``WITH_SHA3_IMPL={opt64,avx2,armv8a-neon,s390-cpacf}``: Select SHA3 implementation opt64 (the default, from Keccak code package), avx2 (for AVX2 capable x86-64 systems, from Keccak code package), armv8a-neon (for NEON capable ARM systems, from Keccak code package), s390-cpacf (for IBM z14 and newer systems supporting SHAKE)

Building on Windows
-------------------

On Windows the code can be built using Visual Studio and cmake's Visual Studio solution generator as follows: Open the "Developer Command Prompt for VS 2017" and from the source folder, run:
```sh
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ..
msbuild  /t:Rebuild /p:Configuration=Release picnic.sln
```

After running `cmake`, one can also open and build the solution directly with the Visual Studio IDE. The code was tested using `cmake' for Windows version 3.10 and Visual Studio 15 2017.

Cross-compiling to Windows using MinGW
--------------------------------------

Using MinGW, Picnic can be cross-compiled from Linux to Windows. First create a `cmake` toolchain file with the following content:
```
SET(CMAKE_SYSTEM_NAME Windows)

# compilers, etc
SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
SET(CMAKE_DLLTOOL x86_64-w64-mingw32-dlltool)
# use wine to execute tests
set(CMAKE_CROSSCOMPILING_EMULATOR wine)

# target environment
SET(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```
Now invoke `cmake` as usual and also pass `-DCMAKE_TOOLCHAIN_FILE=<path>` where `<path>` refers to the toolchain file. After that build with `make`.

If the the cross-built DLLs are intended to be used in a Visual Studio project, some post-processing of the build artifacts is required. Create a file named `libpicnic.def` with the following content:
```
EXPORTS
picnic_clear_private_key
picnic_get_param_name
picnic_get_private_key_size
picnic_get_public_key_size
picnic_keygen
picnic_read_private_key
picnic_read_public_key
picnic_sign
picnic_signature_size
picnic_sk_to_pk
picnic_validate_keypair
picnic_verify
picnic_write_private_key
picnic_write_public_key
```
Then run
```sh
lib /def:"libpicnic.def" /out:"libpicnic.lib" /machine:x64
```
to produce the `lib` file required by Visual Studio for linking.

The instructions were tested for a 64 bit build of Picnic on a recent Debian and Ubuntu installation with `mingw-w64` installed. For a 32 bits the paths need to be changed in the toolchain file and `lib` needs to be called with `/machine:x86`.

CI status
---------
[![Travis CI build status](https://travis-ci.org/IAIK/Picnic.svg?branch=master)](https://travis-ci.org/IAIK/Picnic)
[![AppVeyor CI build status](https://ci.appveyor.com/api/projects/status/github/IAIK/Picnic?branch=master&svg=true)](https://ci.appveyor.com/project/sebastinas/picnic)

License
-------

The code is licensed under the MIT license.
