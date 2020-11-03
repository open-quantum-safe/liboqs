[AppVeyor](https://ci.appveyor.com/project/dstebila/liboqs): ![Build status image](https://ci.appveyor.com/api/projects/status/9d2ts78x88r8wnii/branch/master?svg=true), [CircleCI](https://circleci.com/gh/open-quantum-safe/liboqs/tree/master): ![Build status image](https://circleci.com/gh/open-quantum-safe/liboqs/tree/master.svg?style=svg)

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

Details on each supported algorithm can be found in the [docs/algorithms folder](https://github.com/open-quantum-safe/liboqs/tree/master/docs/algorithms).

#### Key encapsulation mechanisms

- **BIKE**: BIKE1-L1-CPA, BIKE1-L3-CPA, BIKE1-L1-FO, BIKE1-L3-FO
- **Classic McEliece**: Classic-McEliece-348864†, Classic-McEliece-348864f†, Classic-McEliece-460896†, Classic-McEliece-460896f†, Classic-McEliece-6688128†, Classic-McEliece-6688128f†, Classic-McEliece-6960119†, Classic-McEliece-6960119f†, Classic-McEliece-8192128†, Classic-McEliece-8192128f†
- **FrodoKEM**: FrodoKEM-640-AES, FrodoKEM-640-SHAKE, FrodoKEM-976-AES, FrodoKEM-976-SHAKE, FrodoKEM-1344-AES, FrodoKEM-1344-SHAKE
- **HQC**: HQC-128-1-CCA2, HQC-192-1-CCA2, HQC-192-2-CCA2, HQC-256-1-CCA2†, HQC-256-2-CCA2†, HQC-256-3-CCA2†
- **Kyber**: Kyber512, Kyber768, Kyber1024, Kyber512-90s, Kyber768-90s, Kyber1024-90s
- **NTRU**: NTRU-HPS-2048-509, NTRU-HPS-2048-677, NTRU-HPS-4096-821, NTRU-HRSS-701
- **NTRU-Prime**: ntrulpr653, ntrulpr761, ntrulpr857, sntrup653, sntrup761, sntrup857
- **SABER**: LightSaber-KEM, Saber-KEM, FireSaber-KEM
- **SIKE**: SIDH-p434, SIDH-p503, SIDH-p610, SIDH-p751, SIKE-p434, SIKE-p503, SIKE-p610, SIKE-p751, SIDH-p434-compressed, SIDH-p503-compressed, SIDH-p610-compressed, SIDH-p751-compressed, SIKE-p434-compressed, SIKE-p503-compressed, SIKE-p610-compressed, SIKE-p751-compressed

#### Signature schemes

- **Dilithium**: Dilithium2, Dilithium3, Dilithium4
- **Falcon**: Falcon-512, Falcon-1024
- **Picnic**: Picnic-L1-FS, Picnic-L1-UR, Picnic-L1-full, Picnic-L3-FS, Picnic-L3-UR, Picnic-L3-full, Picnic-L5-FS, Picnic-L5-UR, Picnic-L5-full, Picnic3-L1, Picnic3-L3, Picnic3-L5
- **Rainbow**: Rainbow-Ia-Classic, Rainbow-Ia-Cyclic, Rainbow-Ia-Cyclic-Compressed, Rainbow-IIIc-Classic†, Rainbow-IIIc-Cyclic†, Rainbow-IIIc-Cyclic-Compressed†, Rainbow-Vc-Classic†, Rainbow-Vc-Cyclic†, Rainbow-Vc-Cyclic-Compressed†
- **SPHINCS+-Haraka**: SPHINCS+-Haraka-128f-robust, SPHINCS+-Haraka-128f-simple, SPHINCS+-Haraka-128s-robust, SPHINCS+-Haraka-128s-simple, SPHINCS+-Haraka-192f-robust, SPHINCS+-Haraka-192f-simple, SPHINCS+-Haraka-192s-robust, SPHINCS+-Haraka-192s-simple, SPHINCS+-Haraka-256f-robust, SPHINCS+-Haraka-256f-simple, SPHINCS+-Haraka-256s-robust, SPHINCS+-Haraka-256s-simple
- **SPHINCS+-SHA256**: SPHINCS+-SHA256-128f-robust, SPHINCS+-SHA256-128f-simple, SPHINCS+-SHA256-128s-robust, SPHINCS+-SHA256-128s-simple, SPHINCS+-SHA256-192f-robust, SPHINCS+-SHA256-192f-simple, SPHINCS+-SHA256-192s-robust, SPHINCS+-SHA256-192s-simple, SPHINCS+-SHA256-256f-robust, SPHINCS+-SHA256-256f-simple, SPHINCS+-SHA256-256s-robust, SPHINCS+-SHA256-256s-simple
- **SPHINCS+-SHAKE256**: SPHINCS+-SHAKE256-128f-robust, SPHINCS+-SHAKE256-128f-simple, SPHINCS+-SHAKE256-128s-robust, SPHINCS+-SHAKE256-128s-simple, SPHINCS+-SHAKE256-192f-robust, SPHINCS+-SHAKE256-192f-simple, SPHINCS+-SHAKE256-192s-robust, SPHINCS+-SHAKE256-192s-simple, SPHINCS+-SHAKE256-256f-robust, SPHINCS+-SHAKE256-256f-simple, SPHINCS+-SHAKE256-256s-robust, SPHINCS+-SHAKE256-256s-simple

Note that algorithms marked with a dagger (†) have large stack usage and may cause failures when run on threads or in constrained environments.

### Limitations and Security

While at the time of this writing there are no vulnerabilities known in any of the quantum-safe algorithms used in this library, caution is advised when deploying quantum-safe algorithms as most of the algorithms and software have not been subject to the same degree of scrutiny as for currently deployed algorithms. Particular attention should be paid to guidance provided by the standards community, especially from the NIST [Post-Quantum Cryptography Standardization](https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Post-Quantum-Cryptography-Standardization) project.  As research advances, the supported algorithms may see rapid changes in their security, and may even prove insecure against both classical and quantum computers.

liboqs does not intend to "pick winners": algorithm support is informed by the NIST PQC standardization project. We strongly recommend that applications and protocols rely on the outcomes of ths effort when deploying post-quantum cryptography.

We realize some parties may want to deploy quantum-safe cryptography prior to the conclusion of the NIST PQC standardization project.  We strongly recommend such attempts make use of so-called **hybrid cryptography**, in which quantum-safe public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

**WE DO NOT CURRENTLY RECOMMEND RELYING ON THIS LIBRARY IN A PRODUCTION ENVIRONMENT OR TO PROTECT ANY SENSITIVE DATA.** This library is meant to help with research and prototyping.  While we make a best-effort approach to avoid security bugs, this library has not received the level of auditing and analysis that would be necessary to rely on it for high security use.

## Quickstart

### Linux/macOS

1. Install dependencies:

	On Ubuntu:

		 sudo apt install cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz

	On macOS, using a package manager of your choice (we've picked Homebrew):

		brew install cmake ninja openssl@1.1 wget doxygen graphviz
		pip3 install pytest pytest-xdist
	
	Note that, if you want liboqs to use OpenSSL for various symmetric crypto algorithms (AES, SHA-2, etc.) then you must have OpenSSL version 1.1.1 or higher.

2. Get the source:

		git clone -b master https://github.com/open-quantum-safe/liboqs.git
		cd liboqs

	and build:

		mkdir build && cd build
		cmake -GNinja ..
		ninja

Various options can be passed to `cmake` to customize the build. Some of them include:

- `-DOQS_USE_OPENSSL=<val>`: `<val>` can be `ON` or `OFF`; when `ON`, liboqs uses OpenSSL's AES, SHA-2, and SHA-3 implementations.
- `-DBUILD_SHARED_LIBS=<val>`: `<val>` can be `ON` or `OFF`; when `ON`, CMake generates instructions for building a shared library, otherwise it generates instructions for building a static library.
- `-DOPENSSL_ROOT_DIR=<dir>`: `<dir>` specifies the directory in which CMake will look for OpenSSL.

All supported options are listed in the `.CMake/alg-support.cmake` file, and can be viewed by running `cmake -LAH ..` in the `build` directory. They are also listed and explained in [the wiki](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs).

The following instructions assume we are in `build`.

3. The main build result is `lib/liboqs.a`, a static library. The public headers are located in the `include` directory. There are also a variety of programs built under the `tests` directory:

	- `test_kem`: Simple test harness for key encapsulation mechanisms
	- `test_sig`: Simple test harness for key signature schemes
	- `kat_kem`: Program that generates known answer test (KAT) values for key encapsulation mechanisms using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
	- `kat_sig`: Program that generates known answer test (KAT) values for signature schemes using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
	- `speed_kem`: Benchmarking program for key encapsulation mechanisms; see `./speed_kem --help` for usage instructions
	- `speed_sig`: Benchmarking program for signature mechanisms; see `./speed_sig --help` for usage instructions
	- `example_kem`: Minimal runnable example showing the usage of the KEM API
	- `example_sig`: Minimal runnable example showing the usage of the signature API
	- `test_aes`, `test_sha3`: Simple test harnesses for crypto sub-components

	The test suite can be run using

		ninja run_tests

4. To generate HTML documentation of the API, run:

		ninja gen_docs

	Then open `docs/doxygen/html/index.html` in your web browser.

4. Finally, `ninja install` can be run to install the built library and `include` files to a location of choice, which can be specified by passing the `-DCMAKE_INSTALL_PREFIX=<dir>` option to `cmake` at configure time.

### Windows

Binaries can be generated using Visual Studio 2019 with the [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension installed.

### Cross compilation

You can cross compile liboqs for various platform by supplying CMake with an appropriate [toolchain](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html) file.

For example, to cross compile for a Raspberry Pi from Ubuntu Bionic:

	apt install gcc-8-arm-linux-gnueabihf
	mkdir build && cd build
	cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../.CMake/toolchain_rasppi.cmake -DOQS_USE_OPENSSL=OFF ..
	ninja
	
Or to compile for Windows AMD64 from Ubuntu Bionic:

	apt install gcc-mingw-w64
	mkdir build && cd build
	cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../.CMake/toolchain_windows-amd64.cmake -DOQS_USE_CPU_EXTENSIONS=OFF ..
	ninja

## Documentation

Further information can be found in the [wiki](https://github.com/open-quantum-safe/liboqs/wiki).

## Contributing

Contributions that meet the acceptance criteria are gratefully welcomed. See our [Contributing Guide](https://github.com/open-quantum-safe/liboqs/wiki/Contributing-Guide) for more details.

## License

liboqs is licensed under the MIT License; see [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/master/LICENSE.txt) for details.

liboqs includes some third party libraries or modules that are licensed differently; the corresponding subfolder contains the license that applies in that case.  In particular:

- `.CMake/CMakeDependentOption.cmake`: BSD 3-Clause License
- `src/common/common.c`: includes portions which are Apache License v2.0
- `src/common/crypto/aes/aes_c.c`: public domain or any OSI-approved license
- `src/common/crypto/aes/aes*_ni.c`: public domain
- `src/common/crypto/sha2/sha2_c.c`: public domain
- `src/common/crypto/sha3/fips202.c`: public domain
- `src/common/crypto/sha3/keccak4x`: CC0 (public domain), except `brg_endian.h`
- `src/kem/bike/additional`: Apache License v2.0
- `src/kem/classic_mceliece/pqclean_*`: public domain
- `src/kem/kyber/pqclean_*`: public domain
- `src/kem/ntru/pqclean_*`: public domain
- `src/kem/saber/pqclean_*`: public domain
- `src/sig/dilithium/pqclean_*`: public domain
- `src/sig/picnic/external/sha3`: CC0 (public domain)
- `src/sig/rainbow/pqclean_*`: CC0 (public domain)
- `src/sig/sphincs/pqclean_*`: CC0 (public domain)

## Acknowledgements

Various companies, including Amazon Web Services, Cisco Systems, evolutionQ, IBM Research, and Microsoft Research have dedicated programmer time to contribute source code to OQS. [Various people](CONTRIBUTORS) have contributed source code to liboqs.

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services and the Tutte Institute for Mathematics and Computing.
Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see the source papers for funding acknowledgments.
