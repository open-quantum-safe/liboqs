[AppVeyor](https://ci.appveyor.com/project/dstebila/liboqs): ![Build status image](https://ci.appveyor.com/api/projects/status/9d2ts78x88r8wnii/branch/main?svg=true), [CircleCI](https://circleci.com/gh/open-quantum-safe/liboqs/tree/main): ![Build status image](https://circleci.com/gh/open-quantum-safe/liboqs/tree/main.svg?style=svg), [TravisCI](https://travis-ci.com/github/open-quantum-safe/liboqs): [![Build Status](https://travis-ci.com/open-quantum-safe/liboqs.svg?branch=main)](https://travis-ci.com/open-quantum-safe/liboqs)

liboqs
======================

liboqs is an open source C library for quantum-safe cryptographic algorithms.

- [Overview](#overview)
- [Status](#status)
  * [Supported algorithms](#supported-algorithms)
  * [Limitations and Security](#limitations-and-security)
- [Quickstart](#quickstart)
  * [Linux / macOS](#linuxmacOS)
  * [Windows](#windows)
  * [Cross compilation](#cross-compilation)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Overview

liboqs provides:

- a collection of open source implementations of quantum-safe key encapsulation mechanism (KEM) and digital signature algorithms; the full list can be found [below](#supported-algorithms)
- a common API for these algorithms
- a test harness and benchmarking routines

liboqs is part of the **Open Quantum Safe (OQS)** project led by [Douglas Stebila](https://www.douglas.stebila.ca/research/) and [Michele Mosca](http://faculty.iqc.uwaterloo.ca/mmosca/), which aims to develop and integrate into applications quantum-safe cryptography to facilitate deployment and testing in real world contexts. In particular, OQS provides prototype integrations of liboqs into TLS and SSH, through [OpenSSL](https://github.com/open-quantum-safe/openssl) and [OpenSSH](https://github.com/open-quantum-safe/openssh-portable).

More information on OQS can be found [here](https://openquantumsafe.org/) and in the [associated](https://openquantumsafe.org/papers/SAC-SteMos16.pdf) [whitepapers](https://openquantumsafe.org/papers/NISTPQC-CroPaqSte19.pdf).

## Status

### Supported Algorithms

Details on each supported algorithm can be found in the [docs/algorithms](https://github.com/open-quantum-safe/liboqs/tree/main/docs/algorithms) folder.

The list below indicates all algorithms supported by liboqs, but not all those algorithms have been selected for standardization by NIST, specifically Kyber (excluding the "-90s" variants), Dilithium (excluding the "-AES" variants), Falcon, and SPHINCS+ (excluding the "robust" variants). Activating only those standardized algorithms for use in `liboqs` can be facilitated by setting the [OQS_ALGS_ENABLED](CONFIGURE.md#oqs_algs_enabled) build configuration variable to `STD`. By default `liboqs` is built supporting all, incl. experimental, PQ algorithms listed below.

#### Key encapsulation mechanisms

<!--- OQS_TEMPLATE_FRAGMENT_LIST_KEXS_START -->
- **BIKE**: BIKE-L1, BIKE-L3, BIKE-L5
- **Classic McEliece**: Classic-McEliece-348864†, Classic-McEliece-348864f†, Classic-McEliece-460896†, Classic-McEliece-460896f†, Classic-McEliece-6688128†, Classic-McEliece-6688128f†, Classic-McEliece-6960119†, Classic-McEliece-6960119f†, Classic-McEliece-8192128†, Classic-McEliece-8192128f†
- **FrodoKEM**: FrodoKEM-640-AES, FrodoKEM-640-SHAKE, FrodoKEM-976-AES, FrodoKEM-976-SHAKE, FrodoKEM-1344-AES, FrodoKEM-1344-SHAKE
- **HQC**: HQC-128, HQC-192, HQC-256
- **Kyber**: Kyber512, Kyber768, Kyber1024
- **NTRU-Prime**: sntrup761
<!--- OQS_TEMPLATE_FRAGMENT_LIST_KEXS_END -->

#### Signature schemes

<!--- OQS_TEMPLATE_FRAGMENT_LIST_SIGS_START -->
- **CRYSTALS-Dilithium**: Dilithium2, Dilithium3, Dilithium5
- **Falcon**: Falcon-512, Falcon-1024
- **SPHINCS+-SHA2**: SPHINCS+-SHA2-128f-simple, SPHINCS+-SHA2-128s-simple, SPHINCS+-SHA2-192f-simple, SPHINCS+-SHA2-192s-simple, SPHINCS+-SHA2-256f-simple, SPHINCS+-SHA2-256s-simple
- **SPHINCS+-SHAKE**: SPHINCS+-SHAKE-128f-simple, SPHINCS+-SHAKE-128s-simple, SPHINCS+-SHAKE-192f-simple, SPHINCS+-SHAKE-192s-simple, SPHINCS+-SHAKE-256f-simple, SPHINCS+-SHAKE-256s-simple
<!--- OQS_TEMPLATE_FRAGMENT_LIST_SIGS_END -->

Note that for algorithms marked with a dagger (†), liboqs contains at least one implementation that uses a large amount of stack space; this may cause failures when run in threads or in constrained environments. For more information, consult the algorithm information sheets in the [docs/algorithms](https://github.com/open-quantum-safe/liboqs/tree/main/docs/algorithms) folder.

### Limitations and Security

While at the time of this writing there are no vulnerabilities known in any of the quantum-safe algorithms used in this library, caution is advised when deploying quantum-safe algorithms as most of the algorithms and software have not been subject to the same degree of scrutiny as for currently deployed algorithms. Particular attention should be paid to guidance provided by the standards community, especially from the NIST [Post-Quantum Cryptography Standardization](https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Post-Quantum-Cryptography-Standardization) project.  As research advances, the supported algorithms may see rapid changes in their security, and may even prove insecure against both classical and quantum computers. Moreover, note that the `sntrup761` is only included for interop testing.

liboqs does not intend to "pick winners": algorithm support is informed by the NIST PQC standardization project. We strongly recommend that applications and protocols rely on the outcomes of this effort when deploying post-quantum cryptography.

We realize some parties may want to deploy quantum-safe cryptography prior to the conclusion of the NIST PQC standardization project.  We strongly recommend such attempts make use of so-called **hybrid cryptography**, in which quantum-safe public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

**WE DO NOT CURRENTLY RECOMMEND RELYING ON THIS LIBRARY IN A PRODUCTION ENVIRONMENT OR TO PROTECT ANY SENSITIVE DATA.** This library is meant to help with research and prototyping.  While we make a best-effort approach to avoid security bugs, this library has not received the level of auditing and analysis that would be necessary to rely on it for high security use.

#### Platform limitations

In order to optimize support effort,
- not all algorithms are equally well supported on all platforms. In case of questions, it is first advised to review the [documentation files for each algorithm](docs/algorithms).
- not all compilers are equally well supported. For example, at least v7.1.0 of the GNU compiler is required.

## Quickstart

### Linux/macOS

1. Install dependencies:

	On Ubuntu:

		 sudo apt install astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml valgrind

	On macOS, using a package manager of your choice (we've picked Homebrew):

		brew install cmake ninja openssl@1.1 wget doxygen graphviz astyle valgrind
		pip3 install pytest pytest-xdist pyyaml

	Note that, if you want liboqs to use OpenSSL for various symmetric crypto algorithms (AES, SHA-2, etc.) then you must have OpenSSL installed (version 3.x recommended; EOL version 1.1.1 also still possible).

2. Get the source:

		git clone -b main https://github.com/open-quantum-safe/liboqs.git
		cd liboqs

	and build:

		mkdir build && cd build
		cmake -GNinja ..
		ninja

Various `cmake` build options to customize the resultant artifacts are available and are [documented in CONFIGURE.md](CONFIGURE.md#options-for-configuring-liboqs-builds). All supported options are also listed in the `.CMake/alg-support.cmake` file, and can be viewed by running `cmake -LAH ..` in the `build` directory.

The following instructions assume we are in `build`.

3. By default the main build result is `lib/liboqs.a`, a static library. If you want to build a shared/dynamic library, append [`-DBUILD_SHARED_LIBS=ON`](CONFIGURE.md#build_shared_libs) to the `cmake -GNinja ..` command above and the result will be `lib/liboqs.so|dylib|dll`. The public headers are located in the `include` directory. There are also a variety of programs built under the `tests` directory:

	- `test_kem`: Simple test harness for key encapsulation mechanisms
	- `test_sig`: Simple test harness for key signature schemes
	- `test_kem_mem`: Simple test harness for checking memory consumption of key encapsulation mechanisms
	- `test_sig_mem`: Simple test harness for checking memory consumption of key signature schemes
	- `kat_kem`: Program that generates known answer test (KAT) values for key encapsulation mechanisms using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
	- `kat_sig`: Program that generates known answer test (KAT) values for signature schemes using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
	- `speed_kem`: Benchmarking program for key encapsulation mechanisms; see `./speed_kem --help` for usage instructions
	- `speed_sig`: Benchmarking program for signature mechanisms; see `./speed_sig --help` for usage instructions
	- `example_kem`: Minimal runnable example showing the usage of the KEM API
	- `example_sig`: Minimal runnable example showing the usage of the signature API
	- `test_aes`, `test_sha3`: Simple test harnesses for crypto sub-components
	- `test_portability`: Simple test harnesses for checking cross-CPU code portability; requires presence of `qemu`; proper operation validated only on Ubuntu

	The complete test suite can be run using

		ninja run_tests

4. To generate HTML documentation of the API, run:

		ninja gen_docs

	Then open `docs/html/index.html` in your web browser.

4. `ninja install` can be run to install the built library and `include` files to a location of choice, which can be specified by passing the `-DCMAKE_INSTALL_PREFIX=<dir>` option to `cmake` at configure time. Alternatively, `ninja package` can be run to create an install package.

5. `ninja uninstall` can be run to remove all installation files.


### Windows

Binaries can be generated using Visual Studio 2019 with the [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension installed. The same options as explained above for Linux/macOS can be used and build artifacts are generated in the specified `build` folders.

If you want to create Visual Studio build files, e.g., if not using `ninja`, be sure to _not_ pass the parameter `-GNinja` to the `cmake` command as exemplified above. You can then build all components using `msbuild`, e.g. as follows: `msbuild ALL_BUILD.vcxproj` and install all artifacts e.g. using this command `msbuild INSTALL.vcxproj`.


### Cross compilation

You can cross compile liboqs for various platforms. Detailed information is available [in the Wiki](https://github.com/open-quantum-safe/liboqs/wiki/Platform-specific-notes-for-building-liboqs#cross-compiling).

## Documentation

More detailed information on building, optional build parameters, example applications, coding conventions and more can be found in the [wiki](https://github.com/open-quantum-safe/liboqs/wiki).

## Contributing

Contributions that meet the acceptance criteria are gratefully welcomed. See our [Contributing Guide](https://github.com/open-quantum-safe/liboqs/wiki/Contributing-Guide) for more details.

## License

liboqs is licensed under the MIT License; see [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/main/LICENSE.txt) for details.

liboqs includes some third party libraries or modules that are licensed differently; the corresponding subfolder contains the license that applies in that case.  In particular:

- `.CMake/CMakeDependentOption.cmake`: BSD 3-Clause License
- `src/common/common.c`: includes portions which are Apache License v2.0
- `src/common/crypto/aes/aes_c.c`: public domain or any OSI-approved license
- `src/common/crypto/aes/aes*_ni.c`: public domain
- `src/common/crypto/sha2/sha2_c.c`: public domain
- `src/common/crypto/sha3/xkcp_low` : CC0 (public domain), except `brg_endian.h` and `KeccakP-1600-AVX2.s`
- `src/common/crypto/sha3/xkcp_low/.../brg_endian.h` : BSD 3-Clause License
- `src/common/crypto/sha3/xkcp_low/.../KeccakP-1600-AVX2.s` : BSD-like [CRYPTOGAMS license](http://www.openssl.org/~appro/cryptogams/)
- `src/common/rand/rand_nist.c`: See file
- `src/kem/bike/additional`: Apache License v2.0
- `src/kem/classic_mceliece/pqclean_*`: public domain
- `src/kem/kyber/pqcrystals-*`: public domain (CC0) or Apache License v2.0
- `src/kem/kyber/pqclean_*`: public domain (CC0), and public domain (CC0) or Apache License v2.0, and public domain (CC0) or MIT, and MIT
- `src/sig/dilithium/pqcrystals-*`: public domain (CC0) or Apache License v2.0
- `src/sig/dilithium/pqclean_*`: public domain (CC0), and public domain (CC0) or Apache License v2.0, and public domain (CC0) or MIT, and MIT
- `src/sig/sphincs/pqclean_*`: CC0 (public domain)

## Acknowledgements

Various companies, including Amazon Web Services, Cisco Systems, evolutionQ, IBM Research, Microsoft Research, SandboxAQ, and softwareQ have dedicated programmer time to contribute source code to OQS. [Various people](https://github.com/open-quantum-safe/liboqs/blob/main/CONTRIBUTORS) have contributed source code to liboqs.

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services, the Canadian Centre for Cyber Security, the Unitary Fund, the NGI Assure Fund, and VeriSign Inc.

Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see the source papers for funding acknowledgments.
