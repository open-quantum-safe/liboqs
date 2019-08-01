liboqs - master branch
======================

**liboqs** is a C library for quantum-resistant cryptographic algorithms.  This branch of liboqs focuses on selected key encapsulations and signature algorithms.

Overview
--------

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  liboqs provides:

- a common API for post-quantum key encapsulation mechanisms and digital signature schemes
- a collection of open source implementations of post-quantum cryptography algorithms
- a test harness and benchmarking routines

The OQS project also provides prototype integrations into application-level protocols to enable testing of quantum-resistant cryptography.

More information on OQS can be found on our website: [https://openquantumsafe.org/](https://openquantumsafe.org/).

master branch
-------------

This branch of liboqs aims to selectively incorporate allegedly quantum-resistant key encapsulation mechanisms and signature schemes, for the purposes of integration into a common API for liboqs-reliant applications.

Implementations on this branch must meet certain acceptance criteria as indicated below.

Algorithms currently included in liboqs master branch include:

- Key encapsulation mechanisms
	- **BIKE**: BIKE1-L1, BIKE1-L3, BIKE1-L5, BIKE2-L1, BIKE2-L3, BIKE2-L5, BIKE3-L1, BIKE3-L3, BIKE3-L5 (NIST Round 1 version)
	- **FrodoKEM**: FrodoKEM-640-AES, FrodoKEM-640-SHAKE, FrodoKEM-976-AES, FrodoKEM-976-SHAKE, FrodoKEM-1344-AES, FrodoKEM-1344-SHAKE
	- **Kyber**: Kyber512, Kyber768, Kyber1024
	- **NewHope**: NewHope-512-CCA, NewHope-1024-CCA
	- **NTRU**: NTRU-HPS-2048-509, NTRU-HPS-2048-677, NTRU-HPS-4096-877, NTRU-HRSS-701
	- **SABER**: LightSaber-KEM, Saber-KEM, FireSaber-KEM
	- **SIKE**: SIDH-p434, SIDH-p503, SIDH-p610, SIDH-p751, SIKE-p434, SIKE-p503, SIKE-p610, SIKE-p751
- Signature schemes
	- **Dilithium**: Dilithium2, Dilithium3, Dilithium4
	- **MQDSS**: MQDSS-31-48, MQDSS-31-64
	- **Picnic**: Picnic-L1-FS, Picnic-L1-UR, Picnic-L3-FS, Picnic-L3-UR, Picnic-L5-FS, Picnic-L5-UR, Picnic2-L1-FS, Picnic2-L3-FS, Picnic2-L5-FS
	- **qTESLA**: qTESLA-I, qTESLA-III-size, qTESLA-III-speed (NIST Round 1 version)
	- **Rainbow**: Rainbow-Ia-Classic, Rainbow-Ia-Cyclic, Rainbow-Ia-Cyclic-Compressed, Rainbow-IIIc-Classic, Rainbow-IIIc-Cyclic, Rainbow-IIIc-Cyclic-Compressed, Rainbow-Vc-Classic, Rainbow-Vc-Cyclic, Rainbow-Vc-Cyclic-Compressed
	- **SPHINCS+**: SPHINCS+-Haraka-128f-robust, SPHINCS+-Haraka-128f-simple, SPHINCS+-Haraka-128s-robust, SPHINCS+-Haraka-128s-simple, SPHINCS+-Haraka-192f-robust, SPHINCS+-Haraka-192f-simple, SPHINCS+-Haraka-192s-robust, SPHINCS+-Haraka-192s-simple, SPHINCS+-Haraka-256f-robust, SPHINCS+-Haraka-256f-simple, SPHINCS+-Haraka-256s-robust, SPHINCS+-Haraka-256s-simple, SPHINCS+-SHAKE256-128f-robust, SPHINCS+-SHAKE256-128f-simple, SPHINCS+-SHAKE256-128s-robust, SPHINCS+-SHAKE256-128s-simple, SPHINCS+-SHAKE256-192f-robust, SPHINCS+-SHAKE256-192f-simple, SPHINCS+-SHAKE256-192s-robust, SPHINCS+-SHAKE256-192s-simple, SPHINCS+-SHAKE256-256f-robust, SPHINCS+-SHAKE256-256f-simple, SPHINCS+-SHAKE256-256s-robust, SPHINCS+-SHAKE256-256s-simple
    - **Falcon**: Falcon-512, Falcon-1024
For details on algorithms included in master branch, see the datasheets in [docs/algorithms](https://github.com/open-quantum-safe/liboqs/tree/master/docs/algorithms).

Limitations and security
------------------------

liboqs is designed for prototyping and evaluating quantum-resistant cryptography.  Security of proposed quantum-resistant algorithms may rapidly change as research advances, and may ultimately be completely insecure against either classical or quantum computers.

We believe that the NIST Post-Quantum Cryptography standardization project is currently the best avenue to identifying potentially quantum-resistant algorithms.  liboqs does not intend to "pick winners", and we strongly recommend that applications and protocols rely on the outcomes of the NIST standardization project when deploying post-quantum cryptography.

We acknowledge that some parties may want to begin deploying post-quantum cryptography prior to the conclusion of the NIST standardization project.  We strongly recommend that any attempts to do make use of so-called **hybrid cryptography**, in which post-quantum public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

liboqs is provided "as is", without warranty of any kind.  See [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/master/LICENSE.txt) for the full disclaimer.

Acceptance criteria for master branch
-------------------------------------

New submissions to master branch must meet the following acceptance criteria:

- **Algorithmic requirements:**
	- The algorithm must be submitted to the NIST Post-Quantum Cryptography project, or posted as update to an existing algorithm, and must be present in the current round
	- Algorithms whose security is considered effectively broken are not eligible for addition; see the Lifecycle section below for conditions on their removal.
	- KEMs can be IND-CPA or IND-CCA-secure, at any NIST security level.
	- Signature schemes can be EUF-CMA-secure, at any NIST security level.
- **Targets**:
	- **Operating systems:** The code must build on Linux, macOS, and Windows.
	- **Architecture:** The code must build at least on x86 and x64.  Build targets are available for various processor-specific features.
- **Source code requirements:**
	- **License:** Source code licensed under the MIT License, the BSD license, or in the public domain can be directly incorporated into the repository.  GPL code will not be included in the repository.
	- **Code quality:** The source code must meet the following quality checks, some of which are enforced by our continuous integration system:
		- Public API functions must be documented with appropriate Doxygen comments.
		- Source code must be formatted in accordance with our formatting requirements (`make prettyprint`).
		- All secrets in memory must be zero'ed out before being deallocated/released using our `OQS_MEM_*` functions.
		- Exported symbols must be namespaced.
		- We will periodically run static analysis other analysis tools to identify additional potential flaws.
	- **Common function usage:** The source code should make use of liboqs' random number generator, AES implementation, and SHA-3 implementation, rather than its own implementation.
- **Maintenance**: We hope the contributor will intend to help update the implementation in liboqs as the algorithm evolves or as we make enhancements to the library.

Algorithms may be removed from liboqs master branch if they no longer meet the acceptance criteria, see the lifecycle section below.

Contributing
------------

Contributions that meet the acceptance criteria above are gratefully welcomed.  See <a href="https://github.com/open-quantum-safe/liboqs/blob/master/CONTRIBUTING.md">CONTRIBUTING.md</a> for details on contributing an implementation.

Lifecycle for master branch
---------------------------

**Release cycle:** We aim to make releases of liboqs master branch every 2 to 3 months.  Plans for each individual release can be found on our [Github projects board](https://github.com/open-quantum-safe/liboqs/projects/).  

**Versioning:** For approximately the period 2018-2019, we plan to label our releases with a variant of semantic versioning, using the notation *0.Y.Z*.  Semantic versioning allows that anything may change between *0.whatever* versions.  We intend to use *X.Y.Z* labelling as follows, for *X=0*.

- *X=0* will be used for approximately the period 2018-2019.  Later in 2019, we will revisit whether we are ready to make a *1.0.0* release, and specify a new verisoning method at that time.
- *Y* will be incremented when backwards incompatible changes are introduced that either change the public API or change the input/output behaviour of a cryptographic algorithm.  See explanation below.
- *Z* will be incremented when backwards compatible bug fixes are introduced.  

**Algorithm deprecation:** If an algorithm in master branch is found to be insecure or does not advance to the next round of the NIST competition, but is included in version $0.Y.Z$, it will be marked as deprecated using a compile time warning in version $0.(Y+1).Z'$ and removed in version $0.(Y+2).Z''$.

**Algorithm compatibility:** Unlike existing standardization cryptographic algorithms (SHA-2, SHA-3, PKCS\#1v1.5, nistp256, ...), post-quantum algorithms are under active development, and the mathematical algorithm of a cryptographic scheme may change: a particular name (e.g., "FrodoKEM-AES-640") may refer to different mathematical algorithms over time.  liboqs may update implementations as algorithms evolve.  During the $0.Y.Z$ phase of liboqs, versions $0.Y.Z$ and version $0.(Y+1).Z'$ may not be interoperable.  liboqs-reliant applications can check the `alg_version` member of the `OQS_KEM` data structure for each algorithm to obtain an identifier of the algorithm version used in a particular implementation; implementations returning the same `alg_version` for an algorithm will be interoperable.

**API stability:** The public API of liboqs master branch is currently considered to be the functions and macros in `oqs/common.h`, `oqs/oqsconfig.h`, `oqs/kem.h`, `oqs/rand.h`, `oqs/sig.h`, and includes all functions marked with `OQS_API`.  During the $0.Y.Z$ phase of liboqs, incompatible changes to the public API will lead to incrementing $Y$.

Building and running liboqs master branch
-----------------------------------------

Builds are tested using the Travis continuous integration system on macOS 10.13.3 (clang 9.1.0) and Ubuntu 14.04.5 (gcc.4.8, gcc-4.9, gcc-5, gcc-6).  It has also been tested manually on macOS 10.14 (clang 10.0.0), Ubuntu 14.04 (gcc-5), Ubuntu 16.04 (gcc-5), and Ubuntu 18.04.1 (gcc-7).

- [Build status using Travis continuous integration system:](https://travis-ci.org/open-quantum-safe/liboqs/branches) ![Build status image](https://travis-ci.org/open-quantum-safe/liboqs.svg?branch=master)

### Install dependencies for Linux Ubuntu

You need to install the following packages:

	sudo apt install autoconf automake libtool gcc libssl-dev python3-pytest unzip xsltproc

### Install dependencies for macOS

You need to install the following packages using brew (or a package manager of your choice):

	brew install autoconf automake libtool openssl wget
	pip3 install pytest

### Install dependencies for OpenBSD

You need to install the following packages using pkg_add:

	pkg_add automake libtool
	pip3 install pytest

On OpenBSD you have to explicitly set the environment variables `AUTOCONF_VERSION` and 
`AUTOMAKE_VERSION` to a version installed on your system.

	export AUTOCONF_VERSION=`ls -1 /usr/local/bin/autoreconf-* | sort | tail -n 1 | cut -d'-' -f2`
	export AUTOMAKE_VERSION=`ls -1 /usr/local/bin/automake-* | sort | tail -n 1 | cut -d'-' -f2`


### Building

To build, first clone or download the source from GitHub:

	git clone -b master https://github.com/open-quantum-safe/liboqs.git
	cd liboqs

Run the build system:

	autoreconf -i
	./configure
	make clean
	make -j

(If on macOS you encounter an error like `Can't exec "libtoolize": No such file or directory at ...`, try running with `LIBTOOLIZE=glibtoolize autoreconf -i`.)

You can enable/disable various algorithms at compile-time, tell liboqs to use OpenSSL for certain crypto algorithms, and more; see `./configure --help`.

### Build results

The main build result is `liboqs.a`, a static library.  (This may be placed in the `.libs` directory.)

There are also a variety of test programs built under the `tests` directory:

- `test_kem`: Simple test harness for all enabled key encapsulation mechanisms
- `test_sig`: Simple test harness for all enabled key signature schemes
- `kat_kem`: Program that generates known answer test (KAT) values for all enabled key encapsulation mechanisms using the same mechanism as the NIST submission requirements, for checking against submitted KAT values using `scripts/check_kats.sh`
- `speed_kem`: Benchmarking program for key encapsulation mechanisms; see `./speed_kem --help` for usage instructions
- `speed_sig`: Benchmarking program for signature mechanisms; see `./speed_sig --help` for usage instructions
- `example_kem`: Minimal runnable example showing the usage of the KEM API
- `example_sig`: Minimal runnable example showing the usage of the signature API
- `test_aes`, `test_sha3`: Simple test harnesses for crypto sub-components

Building and running on Windows
-------------------------------

Windows binaries can be generated using the Visual Studio solution in the `VisualStudio` folder.

Builds are tested using the Appveyor continuous integration system on Windows Server 2016 (Visual Studio 2017).  Our developers also test builds periodically on Windows 10.

- [Build status using Appveyor continuous integration system:](https://ci.appveyor.com/project/dstebila/liboqs) ![Build status image](https://ci.appveyor.com/api/projects/status/9d2ts78x88r8wnii/branch/master?svg=true)

The supported schemes are defined in the projects' `winconfig.h` file.

Building and running on ARM
---------------------------

Binaries for ARM can be cross-compiled from Ubuntu Linux.  Builds are tested using the Travis continuous integration system on Ubuntu 14.04.5 with QEMU target CPU Cortex-A8.

In order to cross compile, you need to have an appropriate toolchain installed, and to test you need QEMU installed.  On Ubuntu:

	sudo apt install gcc-arm-linux-gnueabi libc6-dev-armel-cross qemu
	
Once the toolchain is installed, you can use the following scripts to build and test ARM builds from Ubuntu Linux:

	scripts/arm-cross-compile.sh
	scripts/arm-run-tests-qemu.sh

At present there are still some quirks with our ARM build, including problems with Picnic and qTESLA and the known answer tests causing build errors or segmentation faults.  See issues #461, #462, and #463.

Documentation
-------------

The directory `docs/algorithms` contains information about each algorithm available in this branch of liboqs.

If you have Doxygen installed (Linux: `sudo apt install doxygen graphviz`; macOS: `brew install doxygen graphviz`), you can build HTML documentation of the liboqs master branch API:

	make docs

Then open `docs/doxygen/html/index.html` in your web browser.

### liboqs default algorithms

The key encapsulation algorithm type `OQS_KEM_alg_default` and signature algorithm type `OQS_SIG_alg_default` are intended for testing purposes and can be changed at compile-time. Care should be exercised if these types are used for other purposes.  To change these at compile time, for example:

	./configure KEM_DEFAULT=OQS_KEM_alg_frodokem_976_aes SIG_DEFAULT=OQS_SIG_alg_picnic_L1_FS

For the Visual Studio build, default algorithms are manually specified in the file `VisualStudio/winconfig.h`.

License
-------

liboqs is licensed under the MIT License; see [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/master/LICENSE.txt) for details.

liboqs includes some third party libraries or modules that are licensed differently; the corresponding subfolder contains the license that applies in that case.  In particular:

- `src/crypto/aes/aes_c.c`: public domain
- `src/crypto/sha3/fips202.c`: CC0 (public domain)
- `src/crypto/sha3/keccak4x`: CC0 (public domain), except `brg_endian.h`
- `src/kem/bike/x86_64`: Apache License v2.0
- `src/kem/kyber/pqclean_*`: public domain
- `src/kem/newhope/pqclean_*`: public domain
- `src/kem/ntru/pqclean_*`: public domain
- `src/kem/saber/pqclean_*`: public domain
- `src/sig/dilithium/pqclean_*`: public domain
- `src/sig/mqdss/pqclean_*`: CC0 (public domain)
- `src/sig/picnic/external/sha3`: CC0 (public domain)

Team
----

The Open Quantum Safe project is led by [Douglas Stebila](https://www.douglas.stebila.ca/research/) and [Michele Mosca](http://faculty.iqc.uwaterloo.ca/mmosca/) at the University of Waterloo.

### Contributors

Contributors to this master branch of liboqs include:

- Nicholas Allen (Amazon Web Services)
- Maxime Anvari
- Eric Crockett (Amazon Web Services)
- Nir Drucker (Amazon Web Services)
- Ben Davies (University of Waterloo)
- Javad Doliskani (University of Waterloo)
- Vlad Gheorghiu (evolutionQ)
- Shay Gueron (Amazon Web Services)
- Torben Hansen (Royal Holloway University of London)
- Tancrède Lepoint (SRI International)
- Shravan Mishra (University of Waterloo)
- Christian Paquin (Microsoft Research)
- Alex Parent (University of Waterloo)
- Peter Schwabe (Radboud University Nijmegen)
- Dimitris Sikeridis (University of New Mexico, Cisco Systems)
- Douglas Stebila (University of Waterloo)
- [John Underhill](https://github.com/Steppenwolfe65/CEX)
- Sebastian Verschoor (University of Waterloo)

### Support

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services and the Tutte Institute for Mathematics and Computing.  

We'd like to make a special acknowledgement to the companies who have dedicated programmer time to contribute source code to OQS, including Amazon Web Services, evolutionQ, Microsoft Research, and Cisco Systems.  

Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see the source papers for funding acknowledgments.
