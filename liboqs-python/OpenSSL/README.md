[![CircleCI](https://circleci.com/gh/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable.svg?style=svg)](https://circleci.com/gh/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable)

OQS-OpenSSL\_1\_1\_1
==================================

[OpenSSL](https://openssl.org/) is an open-source implementation of the TLS protocol and various cryptographic algorithms ([View the original README](https://github.com/open-quantum-safe/openssl/blob/OQS-OpenSSL_1_1_1-stable/README).)

OQS-OpenSSL\_1\_1\_1 is a fork of OpenSSL 1.1.1 that adds quantum-safe key exchange and authentication algorithms using [liboqs](https://github.com/open-quantum-safe/liboqs) for prototyping and evaluation purposes. This fork is not endorsed by the OpenSSL project.

- [Overview](#overview)
- [Status](#status)
  * [Limitations and Security](#limitations-and-security)
  * [Supported Algorithms](#supported-algorithms)
- [Quickstart](#quickstart)
  * [Building](#building)
    * [Linux and macOS](#linux-and-macOS)
    * [Windows](#windows)
    * [Cross-compiling for Windows on Linux](#cross-compiling-for-windows-on-linux)
  * [Running](#running)
- [Third Party Integrations](#third-party-integrations)
- [Contributing](#contributing)
- [License](#license)
- [Team](#team)
- [Acknowledgements](#acknowledgements)

## Overview

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms. See [here](https://github.com/open-quantum-safe/liboqs/) for more information.

**OQS-OpenSSL\_1\_1\_1-stable** is a fork that integrates liboqs into OpenSSL 1.1.1.  The goal of this integration is to provide easy prototyping of quantum-safe cryptography in the TLS 1.3 protocol. (For TLS 1.2, see the [OQS-OpenSSL\_1\_0\_2-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_0_2-stable) branch.)

Both liboqs and this fork are part of the **Open Quantum Safe (OQS) project**, which aims to develop and prototype quantum-safe cryptography. More information about the project can be found [here](https://openquantumsafe.org/).

## Status

This fork is currently in sync with the [OpenSSL\_1\_1\_1d tag](https://github.com/openssl/openssl/tree/OpenSSL_1_1_1d), and adds the following:

- quantum-safe key exchange in TLS 1.3
- hybrid (quantum-safe + elliptic curve) key exchange in TLS 1.3
- quantum-safe authentication in TLS 1.3
- hybrid (quantum-safe + elliptic curve) authentication in TLS 1.3
- CMS support (sign & verify using any of the [supported quantum-safe signature algorithms](#authentication))

For more information, see the [release notes](RELEASE.md).

**THIS FORK IS AT AN EXPERIMENTAL STAGE**, and has not received the same level of auditing and analysis that OpenSSL has received. See the [Limitations and Security](#limitations-and-security) section below for more information.

**WE DO NOT RECOMMEND RELYING ON THIS FORK TO PROTECT SENSITIVE DATA.**

liboqs and our integration into OpenSSL is provided "as is", without warranty of any kind.  See the [LICENSE](https://github.com/open-quantum-safe/liboqs/blob/master/LICENSE.txt) for the full disclaimer.

### Limitations and security
As research advances, the supported algorithms may see rapid changes in their security, and may even prove insecure against both classical and quantum computers.

We believe that the NIST Post-Quantum Cryptography standardization project is currently the best avenue to identifying potentially quantum-resistant algorithms, and strongly recommend that applications and protocols rely on the outcomes of the NIST standardization project when deploying quantum-safe cryptography.

While at the time of this writing there are no vulnerabilities known in any of the quantum-safe algorithms used in this fork, it is advisable to wait on deploying quantum-safe algorithms until further guidance is provided by the standards community, especially from the NIST standardization project.

We realize some parties may want to deploy quantum-safe cryptography prior to the conclusion of the standardization project.  We strongly recommend such attempts make use of so-called **hybrid cryptography**, in which quantum-safe public-key algorithms are combined with traditional public key algorithms (like RSA or elliptic curves) such that the solution is at least no less secure than existing traditional cryptography. This fork provides the ability to use hybrid cryptography.

Proofs of TLS such as [[JKSS12]](https://eprint.iacr.org/2011/219) and [[KPW13]](https://eprint.iacr.org/2013/339) require a key exchange mechanism that has a form of active security, either in the form of the PRF-ODH assumption, or an IND-CCA KEM.
Some of the KEMs provided in liboqs do provide IND-CCA security; others do not ([these datasheets](https://github.com/open-quantum-safe/liboqs/tree/master/docs/algorithms) specify which provide what security), in which case existing proofs of security of TLS against active attackers do not apply.

### Supported Algorithms

If an algorithm is provided by liboqs but is not listed below, it can still be used in the fork through [either one of two ways](https://github.com/open-quantum-safe/openssl/wiki/Using-liboqs-algorithms-that-are-not-in-the-forks).

#### Key Exchange

The following quantum-safe algorithms from liboqs are supported (assuming they have been enabled in liboqs):

- `oqs_kem_default` (see [here](https://github.com/open-quantum-safe/openssl/wiki/Using-liboqs-algorithms-that-are-not-in-the-forks#oqsdefault) for what this denotes)
- **BIKE**: `bike1l1cpa`, `bike1l3cpa`, `bike1l1fo`, `bike1l3fo`
- **FrodoKEM**: `frodo640aes`, `frodo640shake`, `frodo976aes`, `frodo976shake`, `frodo1344aes`, `frodo1344shake`
- **Kyber**: `kyber512`, `kyber768`, `kyber1024`
- **NewHope**: `newhope512cca`, `newhope1024cca`
- **NTRU**: `ntru_hps2048509`, `ntru_hps2048677`, `ntru_hps4096821`, `ntru_hrss701`
- **Saber**: `lightsaber`, `saber`, `firesaber`
- **SIDH** and **SIKE**: `sidhp434`, `sidhp503`, `sidhp610`, `sidhp751`, `sikep434`, `sikep503`, `sikep610`, `sikep751`

The following hybrid algorithms are supported only for L1 schemes; they combine an L1 quantum-safe algorithm listed above with ECDH that uses NIST's P256 curve:
- `p256_<KEX>`, where ``<KEX>`` is any one of the L1 algorithms listed above.

#### Authentication

The following digital signature algorithms from liboqs are supported (assuming they have been enabled in liboqs):

- `oqs_sig_default` (see [here](https://github.com/open-quantum-safe/openssl/wiki/Using-liboqs-algorithms-that-are-not-in-the-forks#oqsdefault) for what this denotes)
- **Dilithium**: `dilithium2`, `dilithium3`, `dilithium4` (not currently on Windows)
- **Picnic**: `picnicl1fs`
- **qTesla**: `qteslapi`, `qteslapiii`

The following hybrid algorithms are supported; they combine a quantum-safe algorithm listed above with a traditional digital signature algorithm (`<SIG>` is any one of the algorithms listed above):

- if `<SIG>` has L1 security, then the fork provides the methods `rsa3072_<SIG>` and `p256_<SIG>`, which combine `<SIG>` with RSA3072 and with ECDSA using NIST's P256 curve respectively.
- if `<SIG>` has L3 security, the fork provides the method `p384_<SIG>`, which combines `<SIG>` with ECDSA using NIST's P384 curve.
- if `<SIG>` has L5 security, the fork provides the method `p521_<SIG>`, which combines `<SIG>` with ECDSA using NIST's P521 curve.

## Quickstart

The steps below have been confirmed to work on macOS 10.14 (clang 10.0.0), Ubuntu 18.04.1 (gcc-7), and Windows 10 (VS2017 build tools).

### Building

#### Linux and macOS

#### Step 0: Get pre-requisites

On **Ubuntu**, you need to install the following packages:

	sudo apt install cmake gcc libtool libssl-dev make ninja-build unzip xsltproc git

On **macOS**, you need to install the following packages using `brew` (or a package manager of your choice):

	brew install cmake ninja libtool openssl@1.1 wget

Then, get source code of this fork (`<OPENSSL_DIR>` is a directory of your choosing):

	git clone --branch OQS-OpenSSL_1_1_1-stable https://github.com/open-quantum-safe/openssl.git <OPENSSL_DIR>

#### Step 1: Build and install liboqs

The following instructions will download and build liboqs, then install it into a subdirectory inside the OpenSSL folder.

	git clone --branch master https://github.com/open-quantum-safe/liboqs.git
	cd liboqs
	mkdir build && cd build
	cmake -GNinja -DCMAKE_INSTALL_PREFIX=<OPENSSL_DIR>/oqs ..
	ninja
	ninja install

Building liboqs requires your system to have (a standard) OpenSSL already installed. `configure` will detect it if it is located in a standard location, such as `/usr` or `/usr/local/opt/openssl` (for brew on macOS).  Otherwise, you may need to specify it with `-DOPENSSL_ROOT_DIR=<path-to-system-openssl-dir>` added to the `cmake` command.

#### Step 2: Build the fork

Now we follow the standard instructions for building OpenSSL. Navigate to `<OPENSSL_DIR>`, and:

on **Ubuntu**, run:

	./Configure no-shared linux-x86_64 -lm
	make -j

on **macOS**, run:

	./Configure no-shared darwin64-x86_64-cc
	make -j

On Ubuntu and macOS, the fork can also be built as a set of shared libraries by specifying `shared` instead of `no-shared` in the above commands; We have used `no-shared` to avoid having to get the libraries in the right place for the runtime linker.

#### Windows

#### Step 0

Make sure you can build the unmodified version of OpenSSL by following the instructions in [INSTALL](https://github.com/open-quantum-safe/openssl/blob/OQS-OpenSSL_1_1_1-stable/INSTALL) and [NOTES.WIN](https://github.com/open-quantum-safe/openssl/blob/OQS-OpenSSL_1_1_1-stable/NOTES.WIN).

Then, get the fork source code (`<OPENSSL_DIR>` is a directory of your choosing):

	git clone --branch OQS-OpenSSL_1_1_1-stable https://github.com/open-quantum-safe/openssl.git <OPENSSL_DIR>

The above command uses `git`, but alternatively, an archive of the source code can be downloaded and expanded into `<OPENSSL_DIR>`

#### Step 1: Build and install liboqs

The following instructions will download (using git, alternatively, [download an archive of the source](https://github.com/open-quantum-safe/liboqs/archive/master.zip) and unzip the project) and build the x64 release configuration of liboqs, then copy the required files it into a subdirectory inside the OpenSSL folder.  You may need to install dependencies before building liboqs; see the [liboqs README](https://github.com/open-quantum-safe/liboqs/blob/master/README.md).

	git clone --branch master https://github.com/open-quantum-safe/liboqs.git
	msbuild liboqs\VisualStudio\liboqs.sln /p:Configuration=Release;Platform=x64
	mkdir openssl\oqs
	mkdir openssl\oqs\lib
	mkdir openssl\oqs\include
	xcopy liboqs\VisualStudio\x64\Release\oqs.lib openssl\oqs\lib\
	xcopy /S liboqs\VisualStudio\include openssl\oqs\include\

#### Step 2: Build the fork

Now we follow the standard instructions for building OpenSSL:

	perl Configure VC-WIN64A
	nmake

#### Cross-compiling for Windows on Linux

#### Step 0: Get pre-requisites

On **Ubuntu**, you need to install the following packages:

	sudo apt install cmake gcc libtool libssl-dev make ninja-build unzip xsltproc gcc-mingw-w64 git

Then, get source code of this fork (`<OPENSSL_DIR>` is a directory of your choosing):

	git clone --branch OQS-OpenSSL_1_1_1-stable https://github.com/open-quantum-safe/openssl.git <OPENSSL_DIR>

#### Step 1: 	Build and install liboqs

	git clone --branch master https://github.com/open-quantum-safe/liboqs.git
	cd liboqs
	mkdir build && cd build
	cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../.CMake/toolchain_windows-amd64.cmake -DCMAKE_INSTALL_PREFIX=<OPENSSL_DIR>/oqs ..
	ninja
	ninja install

#### Step 2: Build the fork

Now we follow the standard instructions for building OpenSSL. Navigate to `<OPENSSL_DIR>`, and:

on **Ubuntu**, run:

	./Configure no-shared mingw64 -lm --cross-compile-prefix=x86_64-w64-mingw32- -Wl,--dynamicbase,--nxcompat -static-libgcc no-capieng
	make -j

The fork can also be built as a set of shared libraries by specifying `shared` instead of `no-shared` in the above commands; We have used `no-shared` to avoid having to get the libraries in the right place for the runtime linker.

### Running

#### TLS demo

OpenSSL contains a basic TLS server (`s_server`) and TLS client (`s_client`) which can be used to demonstrate and test TLS connections.

To run a server, you first need to generate a X.509 certificate, using either a classical (`rsa`), quantum-safe (any quantum-safe authentication algorithm in the [Supported Algorithms](#supported-algorithms) section above), or hybrid (any hybrid authentication algorithm in the [Supported Algorithms](#supported-algorithms) section above) algorithm. The server certificate can either be self-signed or part of a chain. In either case, you need to generate a self-signed root CA certificate using the following command, replacing `<SIG>` with an algorithm mentioned above:

	apps/openssl req -x509 -new -newkey <SIG> -keyout <SIG>_CA.key -out <SIG>_CA.crt -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf

If you want an ECDSA certificate (`<SIG>` = `ecdsa`), you instead need to run:

	apps/openssl req -x509 -new -newkey ec:<(apps/openssl ecparam -name secp384r1) -keyout <SIG>_CA.key -out <SIG>_CA.crt -nodes -subj "/CN=oqstest" -days 365 -config apps/openssl.cnf

The root CA certificate can be used directly to start the server (see below), or can be used to issue a server certificate, using the usual OpenSSL process (note that for simplicity, we use the same algorithm for the server and CA certificates; in practice the CA is likely to use a stronger one):

1. The server generates its key pair:

		apps/openssl genpkey -algorithm <SIG> -out <SIG>_srv.key

2. The server generates a certificate request and sends it the to CA:

		apps/openssl req -new -newkey <SIG> -keyout <SIG>_srv.key -out <SIG>_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf

3. The CA generates the signed server certificate:

		apps/openssl x509 -req -in <SIG>_srv.csr -out <SIG>_srv.crt -CA <SIG>_CA.crt -CAkey <SIG>_CA.key -CAcreateserial -days 365

To run a basic TLS server with all libOQS ciphersuites enabled, run the following command, replacing `<SERVER>` with either `<SIG>_CA` or `<SIG>_srv`:

	apps/openssl s_server -cert <SERVER>.crt -key <SERVER>.key -www -tls1_3

In another terminal window, you can run a TLS client requesting one of the supported ciphersuites (`<KEX>` = one of the quantum-safe or hybrid key exchange algorithms listed in the [Supported Algorithms section above](#key-exchange):

	apps/openssl s_client -groups <KEX> -CAfile <SIG>_CA.crt 

#### Performance testing

In order to do TLS-level end-to-end performance testing, one can run "empty" TLS handshakes via the standard `openssl s_time` command. In order to suitably trigger this with an OQS KEM/SIG pair of choice, one can follow all steps outlined above to obtain an OQS-signed server certificate. As the `openssl s_time` command does not permit selection of suitable groups --which are used to encode the OQS-KEM--, you have to exchange the two last commands in the description above as follows to facilitate timed OQS-TLS handshake performance testing (exchanging SERVER and KEX variables as per the instructions above):

	apps/openssl s_server -cert <SERVER>.crt -key <SERVER>.key -www -tls1_3 -groups <KEX>

and

	apps/openssl s_time 

##### Performance testing simplified

If you don't want to build all components but just want to do comparative QSC as well as "classic crypto" TLS handshake performance testing a [docker-based performance test environment is available in the `oqs-demos` subproject](https://github.com/open-quantum-safe/oqs-demos/tree/master/curl#performance-testing).


#### CMS demo

OpenSSL has facilities to perform signing operations pursuant [RFC 5652](https://datatracker.ietf.org/doc/rfc5652). This can now be used to demonstrate data or code signing utilizing quantum-safe algorithms. 

Building on the artifacts created in the TLS setup above (CA and server certificate creation using a specific (QS) SIG algorithm), the following command can be used to generate a (QS-)signed file from some inputfile:

	apps/openssl cms -in inputfile -sign -signer <SIG>_srv.crt -inkey <SIG>_srv.key -nodetach -outform pem -binary -out signedfile.cms 

This command can be used to verify (and extract the contents) of the CMS file resultant from the command above:

	apps/openssl cms -verify -CAfile <SIG>_CA.crt -inform pem -in signedfile.cms -crlfeol -out signeddatafile

It would be expected that the contents of `inputfile` and the resultant `signeddatafile` are the same.

#### Speed

OpenSSL has facilities to perform pure speed tests of the cryptographic algorithms supported. This can now also be used to compare relative performance of OQS algorithms.

This command can be used to determine the speed of all KEM algorithms supported by the underlying `liboqs`:

	apps/openssl speed oqskem

This command can be used to determine the speed of all OQS signature algorithms supported by OQS-openssl:

	apps/openssl speed oqssig

As with standard OpenSSL one can also pass the specific algorithm name to be tested, e.g., `apps/openssl speed dilithium2`.


## Third Party Integrations

Various third-party software applications, such as [nginx](https://www.nginx.com/) and [curl](https://curl.haxx.se/) use OpenSSL to establish TLS connections; they can be built against our fork to make use of quantum-safe cryptography. The [oqs-software](https://github.com/open-quantum-safe/oqs-software) repository provides instructions for building various software like so.

## Contributing

Contributions are gratefully welcomed. See our [Contributing Guide](https://github.com/open-quantum-safe/openssl/wiki/Contributing-Guide) for more details.

## License

All modifications to this repository are released under the same terms as OpenSSL, namely as described in the file [LICENSE](https://github.com/open-quantum-safe/openssl/blob/OQS-OpenSSL_1_1_1-stable/LICENSE).

## Team

The Open Quantum Safe project is led by [Douglas Stebila](https://www.douglas.stebila.ca/research/) and [Michele Mosca](http://faculty.iqc.uwaterloo.ca/mmosca/) at the University of Waterloo.

Contributors to OQS-OpenSSL\_1\_1\_1 include:

- Christian Paquin (Microsoft Research)
- Dimitris Sikeridis (University of New Mexico / Cisco Systems)
- Douglas Stebila (University of Waterloo)
- Goutam Tamvada (University of Waterloo)
- Michael Baentsch (IBM Research Zurich)

## Acknowledgments

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services and the Tutte Institute for Mathematics and Computing.

We'd like to make a special acknowledgement to the companies who have dedicated programmer time to contribute source code to OQS, including Amazon Web Services, Cisco Systems, evolutionQ, and Microsoft Research.

Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see [here](https://openquantumsafe.org/papers/SAC-SteMos16.pdf) and [here](https://openquantumsafe.org/papers/NISTPQC-CroPaqSte19.pdf) for funding acknowledgments.
