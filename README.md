liboqs
======================

liboqs is an open source C library that provides implementations of quantum-safe cryptographic algorithms.

[![Appveyor CI](https://ci.appveyor.com/api/projects/status/9d2ts78x88r8wnii/branch/master?svg=true)](https://ci.appveyor.com/project/dstebila/liboqs)
[![CircleCI](https://circleci.com/gh/open-quantum-safe/liboqs/tree/master.svg?style=svg)](https://circleci.com/gh/open-quantum-safe/liboqs/tree/master)
[![Travis CI](https://travis-ci.org/open-quantum-safe/liboqs.svg?branch=master)](https://travis-ci.org/open-quantum-safe/liboqs)

- [Overview](#overview)
- [Status](#status)
- [Quickstart](#quickstart)
- [Docs](#docs)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Overview

liboqs provides:

- a common API for post-quantum key encapsulation mechanisms (KEMs) and digital signature schemes
- a collection of open source implementations of post-quantum cryptography algorithms
- a test harness and benchmarking routines

Supported KEMS include Frodo and SIKE, and supported digital signature algorithms include Dilithium and SPHINCS. For a full list of supported algorithms, check the documentation.

liboqs is part of the **Open Quantum Safe (OQS)** project, which is led by [Douglas Stebila](https://www.douglas.stebila.ca/research/) and [Michele Mosca](http://faculty.iqc.uwaterloo.ca/mmosca/), and which aims to develop and integrate into applications quantum-safe cryptography to facilitate deployment and testing. In particular, the project provides prototype integrations of liboqs into TLS, through [OpenSSL](https://github.com/open-quantum-safe/openssl), and SSH, through [OpenSSH](https://github.com/open-quantum-safe/openssh-portable).

More information on OQS can be found [here](https://openquantumsafe.org/) and in the [associated](https://openquantumsafe.org/papers/SAC-SteMos16.pdf) [whitepapers](https://openquantumsafe.org/papers/NISTPQC-CroPaqSte19.pdf).

## Status

As research advances, the supported algorithms may see rapid changes in their security, and may even prove insecure against both classical and quantum computers.

liboqs does not intend to "pick winners": our algorithm support is informed by the NIST [Post-Quantum Cryptography Standardization](https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Post-Quantum-Cryptography-Standardization) effort. We strongly recommend that applications and protocols rely on the outcomes of ths effort when deploying post-quantum cryptography.

We acknowledge that some parties may want to deploy quantum-safe cryptography prior to the conclusion of the NIST standardization project.  We strongly recommend such attempts make use of so-called **hybrid cryptography**, in which quantum-safe public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

## Quickstart

### On Ubuntu/macOS

0. Install dependencies:

On ubuntu:

	sudo apt install autoconf automake libtool gcc libssl-dev python3-pytest unzip xsltproc

On macOS, using a package manager of your choice (we will use Homebrew):

	brew install autoconf automake libtool openssl wget
	pip3 install pytest

1. Get the source:

	git clone -b master https://github.com/open-quantum-safe/liboqs.git
	cd liboqs

Then build:

	autoreconf -i
	./configure
	make clean
	make -j

(If on macOS you encounter an error like `Can't exec "libtoolize": No such file or directory at ...`, try running with `LIBTOOLIZE=glibtoolize autoreconf -i`.)

3. The main build result is `liboqs.a`, a static library.  (This may be placed in the `.libs` directory.) There are also a variety of test programs built under the `tests` directory:

- `test_kem`: Simple test harness for key encapsulation mechanisms
- `test_sig`: Simple test harness for key signature schemes
- `kat_kem`: Program that generates known answer test (KAT) values for key encapsulation mechanisms using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
- `kat_sig`: Program that generates known answer test (KAT) values for signature schemes using the same procedure as the NIST submission requirements, for checking against submitted KAT values using `tests/test_kat.py`
- `speed_kem`: Benchmarking program for key encapsulation mechanisms; see `./speed_kem --help` for usage instructions
- `speed_sig`: Benchmarking program for signature mechanisms; see `./speed_sig --help` for usage instructions
- `example_kem`: Minimal runnable example showing the usage of the KEM API
- `example_sig`: Minimal runnable example showing the usage of the signature API
- `test_aes`, `test_sha3`: Simple test harnesses for crypto sub-components

A range of tests (including all `test_*` and `kat_*` programs above) can be run using

	python3 -m pytest

### On other platforms

Please consult the [wiki](https://github.com/open-quantum-safe/liboqs/wiki).

## Docs

For details on the supported algorithms, see the datasheets in [docs/algorithms](https://github.com/open-quantum-safe/liboqs/tree/master/docs/algorithms).

If you have Doxygen installed (Linux: `sudo apt install doxygen graphviz`; macOS: `brew install doxygen graphviz`), you can build HTML documentation of the liboqs master branch API:

	make docs

Then open `docs/doxygen/html/index.html` in your web browser.

Additional information can also be found in the [wiki](https://github.com/open-quantum-safe/liboqs/wiki).

## Contributing

Contributions that meet the acceptance criteria are gratefully welcomed. See our [Contributing Guide](CONTRIBUTING.md) for more details.

## License

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

## Acknowledgements

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services and the Tutte Institute for Mathematics and Computing.

Various companies, including Amazon Web Services, evolutionQ, Microsoft Research, and Cisco Systems, have dedicated programmer time to contribute source code to OQS.

Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see the source papers for funding acknowledgments.
