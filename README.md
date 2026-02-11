liboqs
======================

[![Main Branch Tests](https://github.com/open-quantum-safe/liboqs/actions/workflows/commit-to-main.yml/badge.svg)](https://github.com/open-quantum-safe/liboqs/actions/workflows/commit-to-main.yml)
[![Weekly Tests](https://github.com/open-quantum-safe/liboqs/actions/workflows/weekly.yml/badge.svg)](https://github.com/open-quantum-safe/liboqs/actions/workflows/weekly.yml)
[![Coverage Status](https://coveralls.io/repos/github/open-quantum-safe/liboqs/badge.svg?branch=main)](https://coveralls.io/github/open-quantum-safe/liboqs?branch=main)

liboqs is an open source C library for quantum-safe cryptographic algorithms.

- [liboqs](#liboqs)
	- [Overview](#overview)
	- [Status](#status)
		- [Supported Algorithms](#supported-algorithms)
			- [Key encapsulation mechanisms](#key-encapsulation-mechanisms)
			- [Signature schemes](#signature-schemes)
			- [Stateful signature schemes](#stateful-signature-schemes)
		- [Limitations and Security](#limitations-and-security)
			- [Platform limitations](#platform-limitations)
			- [Support limitations](#support-limitations)
	- [Quickstart](#quickstart)
		- [Linux and Mac](#linux-and-mac)
		- [Windows](#windows)
		- [Cross compilation](#cross-compilation)
	- [Documentation](#documentation)
	- [Contributing](#contributing)
	- [License](#license)
	- [Acknowledgements](#acknowledgements)

## Overview

liboqs provides:

- a collection of open source implementations of quantum-safe key encapsulation mechanisms (KEMs) and digital signature algorithms; the full list can be found [below](#supported-algorithms)
- a common API for these algorithms
- a test harness and benchmarking routines

liboqs is part of the **Open Quantum Safe (OQS)** project, which aims to develop and integrate into applications quantum-safe cryptography to facilitate deployment and testing in real world contexts. In particular, OQS provides prototype integrations of liboqs into protocols like TLS, X.509, and S/MIME, through our [OpenSSL 3 Provider](https://github.com/open-quantum-safe/oqs-provider) and we provide a variety of other [post-quantum-enabled demos](https://github.com/open-quantum-safe/oqs-demos).

The OQS project is supported by the [Post-Quantum Cryptography Alliance](https://pqca.org/) as part of the [Linux Foundation](https://linuxfoundation.org/). More information about the Open Quantum Safe project can be found at [openquantumsafe.org](https://openquantumsafe.org/).

OQS is running a survey to better understand our community. We would like to hear from organizations and individuals about their interest in and use of the Open Quantum Safe project. Please take a few minutes to fill out the survey: https://linuxfoundation.surveymonkey.com/r/oqssurvey

## Status

### Supported Algorithms

Details on each supported algorithm can be found in the [docs/algorithms](https://github.com/open-quantum-safe/liboqs/tree/main/docs/algorithms) folder.

The list below indicates all algorithms currently supported by liboqs, including experimental algorithms and already excluding algorithm variants pruned during the NIST competition, such as Kyber-90s or Dilithium-AES.

The only algorithms in `liboqs` that implement NIST standards are the [`ML-KEM`](https://csrc.nist.gov/pubs/fips/203/final) (final standard) and [`ML-DSA`](https://csrc.nist.gov/pubs/fips/204/final) (final standard) variants with their respective different bit strengths. `liboqs` will retain these algorithm names selected by NIST throughout the finishing stages of the standardization process, so users can rely on their presence going forward. If NIST changes the implementation details of these algorithms, `liboqs` will adjust the implementation so that users are protected from such potential changes.

Falcon has also been [selected for standardization](https://csrc.nist.gov/Projects/post-quantum-cryptography/selected-algorithms-2022), but the `liboqs` implementations of these algorithms are currently tracking Round 3 submissions and not NIST standards drafts.

All names other than `ML-KEM` and `ML-DSA` are subject to change. `liboqs` makes available a [selection mechanism for algorithms on the NIST standards track, continued NIST competition, or purely experimental nature by way of the configuration variable OQS_ALGS_ENABLED](CONFIGURE.md#oQS_ALGS_ENABLED). By default `liboqs` is built supporting all, incl. experimental, PQ algorithms listed below.

<!-- OQS_TEMPLATE_FRAGMENT_ALG_SUPPORT_START -->
#### Key encapsulation mechanisms
| Algorithm family   | Standardization status                                                                                                                                                                                                                    | Primary implementation                                                                                                                    |
|:-------------------|:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:------------------------------------------------------------------------------------------------------------------------------------------|
| BIKE               | Not selected by [NIST](https://bikesuite.org/files/v5.1/BIKE_Spec.2022.10.10.1.pdf)                                                                                                                                                       | [`awslabs/bike-kem`](https://github.com/awslabs/bike-kem)                                                                                 |
| Classic McEliece   | Under [ISO](https://classic.mceliece.org/iso.html) consideration                                                                                                                                                                          | [`PQClean/PQClean@1eacfda`](https://github.com/PQClean/PQClean/commit/1eacfdafc15ddc5d5759d0b85b4cef26627df181)                           |
| FrodoKEM           | Under [ISO](https://frodokem.org/) consideration                                                                                                                                                                                          | [`microsoft/PQCrypto-LWEKE@a2f9dec`](https://github.com/microsoft/PQCrypto-LWEKE/commit/a2f9dec8917ccc3464b3378d46b140fa7353320d)         |
| HQC                | Selected by [NIST](https://pqc-hqc.org/doc/hqc_specifications_2025_08_22.pdf) for upcoming standardization                                                                                                                                | [`PQClean/PQClean@1eacfda`](https://github.com/PQClean/PQClean/commit/1eacfdafc15ddc5d5759d0b85b4cef26627df181)                           |
| Kyber              | Selected by [NIST](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip) as basis for ML-KEM (FIPS 203)                                                                     | [`pq-crystals/kyber@441c051`](https://github.com/pq-crystals/kyber/commit/441c0519a07e8b86c8d079954a6b10bd31d29efc)                       |
| ML-KEM             | Standardized by [NIST](https://csrc.nist.gov/pubs/fips/203/final)                                                                                                                                                                         | [`pq-code-package/mlkem-native@048fc2a`](https://github.com/pq-code-package/mlkem-native/commit/048fc2a7a7b4ba0ad4c989c1ac82491aa94d5bfa) |
| NTRU               | Not selected by [NIST](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/NTRU-Round3.zip), under standardization consideration by [NTT](https://info.isl.ntt.co.jp/crypt/ntru/index.html) | [`PQClean/PQClean@4c9e5a3`](https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6)                           |
| NTRU-Prime         | Not selected by [NIST](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/NTRU-Prime-Round3.zip)                                                                                           | [`PQClean/PQClean@4c9e5a3`](https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6)                           |

#### Signature schemes
| Algorithm family   | Standardization status                                                                                                                                               | Primary implementation                                                                                                                      |
|:-------------------|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------------------|
| CROSS              | Under [NIST](https://www.cross-crypto.com/CROSS_Specification_v2.2.pdf) consideration                                                                                | [`CROSS-signature/CROSS-lib-oqs@a21ebc3`](https://github.com/CROSS-signature/CROSS-lib-oqs/commit/a21ebc314e06b0972a9bbcf2813a185ecb2917f1) |
| Falcon             | Selected by [NIST](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Falcon-Round3.zip) for upcoming standardization | [`PQClean/PQClean@1eacfda`](https://github.com/PQClean/PQClean/commit/1eacfdafc15ddc5d5759d0b85b4cef26627df181)                             |
| MAYO               | Under [NIST](https://csrc.nist.gov/csrc/media/Projects/pqc-dig-sig/documents/round-2/spec-files/mayo-spec-round2-web.pdf) consideration                              | [`PQCMayo/MAYO-C@4b7cd94`](https://github.com/PQCMayo/MAYO-C/commit/4b7cd94c96b9522864efe40c6ad1fa269584a807)                               |
| ML-DSA             | Standardized by [NIST](https://csrc.nist.gov/pubs/fips/204/final)                                                                                                    | [`pq-code-package/mldsa-native@f48f164`](https://github.com/pq-code-package/mldsa-native/commit/f48f164cefb07f4ffa519ddda7cee670b8ee3517)   |
| SLH-DSA            | Standardized by [NIST](https://csrc.nist.gov/pubs/fips/205/final)                                                                                                    | [`pq-code-package/slhdsa-c@a0fc1ff`](https://github.com/pq-code-package/slhdsa-c/commit/a0fc1ff253930060d0246aebca06c2538eb92b88)           |
| SNOVA              | Under [NIST](https://csrc.nist.gov/csrc/media/Projects/pqc-dig-sig/documents/round-2/spec-files/snova-spec-round2-web.pdf) consideration                             | [`vacuas/SNOVA@1c3ca6f`](https://github.com/vacuas/SNOVA/commit/1c3ca6f4f7286c0bde98d7d6f222cf63b9d52bff)                                   |
| UOV                | Under [NIST](https://csrc.nist.gov/csrc/media/Projects/pqc-dig-sig/documents/round-2/spec-files/uov-spec-round2-web.pdf) consideration                               | [`pqov/pqov@33fa527`](https://github.com/pqov/pqov/commit/33fa5278754a32064c55901c3a17d48b06cc2351)                                         |

#### Stateful signature schemes
| Algorithm family   | Standardization status                                                                                                                                         | Primary implementation                                          |
|:-------------------|:---------------------------------------------------------------------------------------------------------------------------------------------------------------|:----------------------------------------------------------------|
| LMS                | Standardized by [IRTF](https://www.rfc-editor.org/info/rfc8554), approved by [NIST](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-208.pdf) | [`cisco/hash-sigs`](https://github.com/cisco/hash-sigs)         |
| XMSS               | Standardized by [IRTF](https://www.rfc-editor.org/info/rfc8391), approved by [NIST](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-208.pdf) | [`XMSS/xmss-reference`](https://github.com/XMSS/xmss-reference) |
<!-- OQS_TEMPLATE_FRAGMENT_ALG_SUPPORT_END -->

### Limitations and Security

While at the time of this writing there are no vulnerabilities known in any of the quantum-safe algorithms used in this library, caution is advised when deploying quantum-safe algorithms as most of the algorithms and software have not been subject to the same degree of scrutiny as for currently deployed algorithms. Particular attention should be paid to guidance provided by the standards community, especially from the NIST [Post-Quantum Cryptography Standardization](https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Post-Quantum-Cryptography-Standardization) project.  As research advances, the supported algorithms may see rapid changes in their security, and may even prove insecure against both classical and quantum computers. Moreover, note that the `sntrup761` is only included for interop testing.

liboqs does not intend to "pick winners": algorithm support is informed by the NIST PQC standardization project. We strongly recommend that applications and protocols rely on the outcomes of this effort when deploying post-quantum cryptography.

We realize some parties may want to deploy quantum-safe cryptography prior to the conclusion of the NIST PQC standardization project.  We strongly recommend such attempts make use of so-called **hybrid cryptography**, in which quantum-safe public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

**WE DO NOT CURRENTLY RECOMMEND RELYING ON THIS LIBRARY IN A PRODUCTION ENVIRONMENT OR TO PROTECT ANY SENSITIVE DATA.** This library is meant to help with research and prototyping.  While we make a best-effort approach to avoid security bugs, this library has not received the level of auditing and analysis that would be necessary to rely on it for high security use.

Please see [SECURITY.md](SECURITY.md#security-policy) for details on how to report a vulnerability and the OQS vulnerability response process.

#### Platform limitations

In order to optimize support effort,
- not all algorithms are equally well supported on all platforms. In case of questions, it is first advised to review the [documentation files for each algorithm](docs/algorithms).
- not all compilers are equally well supported. For example, at least v7.1.0 of the GNU compiler is required.

#### Support limitations

This project is not commercially supported. All guidelines and goals for liboqs are reflections of current practices, executed by a community of academic, part-time, and/or voluntary contributors on a best-effort basis and may change at any time. Any entity seeking more reliable commitments is strongly encouraged to join the OQS community and thus enhance the code and support that the community can provide.


## Quickstart

### Linux and Mac

1. Install dependencies:

	On Ubuntu:

		 sudo apt install astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml valgrind

	On macOS, using a package manager of your choice (we've picked Homebrew):

		brew install cmake ninja openssl@3 wget doxygen graphviz astyle valgrind
		pip3 install pytest pytest-xdist pyyaml

	Using Nix:

	    nix develop

	Note that, if you want liboqs to use OpenSSL for various symmetric crypto algorithms (AES, SHA-2, etc.) then you must have OpenSSL installed (version 3.x recommended; EOL version 1.1.1 also still possible).

2. Get the source:

		git clone -b main https://github.com/open-quantum-safe/liboqs.git
		cd liboqs

	and build:

		mkdir build && cd build
		cmake -GNinja ..
		ninja

Various `cmake` build options to customize the resultant artifacts are available and are [documented in CONFIGURE.md](CONFIGURE.md#options-for-configuring-liboqs-builds). All supported options are also listed in the `.CMake/alg-support.cmake` file, and can be viewed by running `cmake -LAH -N ..` in the `build` directory.

The following instructions assume we are in `build`.

3. By default the main build result is `lib/liboqs.a`, a static library. If you want to build a shared/dynamic library, append [`-DBUILD_SHARED_LIBS=ON`](CONFIGURE.md#bUILD_SHARED_LIBS) to the `cmake -GNinja ..` command above and the result will be `lib/liboqs.so|dylib|dll`. The public headers are located in the `include` directory. There are also a variety of programs built under the `tests` directory:

	- `test_kem`: Simple test harness for key encapsulation mechanisms
	- `test_sig`: Simple test harness for signature schemes
	- `test_sig_stfl`: Simple test harness for stateful signature schemes
	- `test_kem_mem`: Simple test harness for checking memory consumption of key encapsulation mechanisms
	- `test_sig_mem`: Simple test harness for checking memory consumption of signature schemes
	- `kat_kem`: Program that generates known answer test (KAT) values for key encapsulation mechanisms using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
	- `kat_sig`: Program that generates known answer test (KAT) values for signature schemes using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
	- `kat_sig_stfl`: Program for checking results against submitted KAT values using `tests/test_kat.py`
	- `speed_kem`: Benchmarking program for key encapsulation mechanisms; see `./speed_kem --help` for usage instructions
	- `speed_sig`: Benchmarking program for signature mechanisms; see `./speed_sig --help` for usage instructions
	- `speed_sig_stfl`: Benchmarking program for stateful signature mechanisms; see `./speed_sig_stfl --help` for usage instructions
	- `example_kem`: Minimal runnable example showing the usage of the KEM API
	- `example_sig`: Minimal runnable example showing the usage of the signature API
	- `example_sig_stfl`: Minimal runnable example showing the usage of the stateful signature API
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
- `src/kem/kyber/libjade_*` public domain (CC0) or Apache License v2.
- `src/kem/ml_kem/mlkem-native_*`: MIT or Apache License v2.0 or ISC License
- `src/kem/ntru/pqclean_*`: public domain (CC0)
-  src/sig/falcon/pqclean_\*\_aarch64 : Apache License v2.0
- `src/sig/mayo/*`: Apache License v2.0
- `src/sig/ml_dsa/pqcrystals-*`: public domain (CC0) or Apache License v2.0

## Acknowledgements

The OQS project is supported by the [Post-Quantum Cryptography Alliance](https://pqca.org/) as part of the [Linux Foundation](https://linuxfoundation.org/).

The OQS project was founded by Douglas Stebila and Michele Mosca at the University of Waterloo.  [Contributors to liboqs](https://github.com/open-quantum-safe/liboqs/blob/main/CONTRIBUTORS) include individual contributors, academics and researchers, and various companies, including Amazon Web Services, Cisco Systems, evolutionQ, IBM Research, Microsoft Research, SandboxAQ, and softwareQ.

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services, the Canadian Centre for Cyber Security, Cisco, the Unitary Fund, the NGI Assure Fund, and VeriSign Inc.

Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see the source papers for funding acknowledgments.
