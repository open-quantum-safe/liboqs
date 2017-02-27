[![Build Status](https://travis-ci.org/open-quantum-safe/liboqs.svg?branch=master)](https://travis-ci.org/open-quantum-safe/liboqs)

liboqs
======

liboqs is a C library for quantum-resistant cryptographic algorithms.

Overview
--------

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  liboqs initially focuses on key exchange algorithms.  liboqs provides a common API suitable for post-quantum key exchange algorithms, and will collect together various implementations.  liboqs will also include a test harness and benchmarking routines to compare performance of post-quantum implementations.

OQS will also include integrations into application-level protocols to provide easy prototyping of quantum-resistant cryptography.  Our first integration is in OpenSSL:

- **open-quantum-safe/openssl** is an integration of liboqs into OpenSSL 1.0.2.  The goal of this integration is to provide easy prototyping of quantum-resistant cryptography.  The integration should not be considered "production quality".  See more about this integration in its GitHub repository [open-quantum-safe/openssl/](https://github.com/open-quantum-safe/openssl/).

More information on OQS can be found on our website: [https://openquantumsafe.org/](https://openquantumsafe.org/).

Contents
--------

liboqs currently contains:

- `kex_rlwe_bcns15`: key exchange from the ring learning with errors problem (Bos, Costello, Naehrig, Stebila, *IEEE Symposium on Security & Privacy 2015*, [https://eprint.iacr.org/2014/599](https://eprint.iacr.org/2014/599))
- `kex_rlwe_newhope`: "NewHope": key exchange from the ring learning with errors problem (Alkim, Ducas, Pöppelmann, Schwabe, *USENIX Security 2016*, [https://eprint.iacr.org/2015/1092](https://eprint.iacr.org/2015/1092)) (using the reference C implementation of NewHope from [https://github.com/tpoeppelmann/newhope](https://github.com/tpoeppelmann/newhope))
- `kex_rlwe_msrln16`: Microsoft Research implementation of Peikert's ring-LWE key exchange (Longa, Naehrig, *CANS 2016*, [https://eprint.iacr.org/2016/504](https://eprint.iacr.org/2016/504)) (based on the implementation of Alkim, Ducas, Pöppelmann, and Schwabe, with improvements from Longa and Naehrig, see [https://www.microsoft.com/en-us/research/project/lattice-cryptography-library/](https://www.microsoft.com/en-us/research/project/lattice-cryptography-library/))
- `kex_lwe_frodo`: "Frodo": key exchange from the learning with errors problem (Bos, Costello, Ducas, Mironov, Naehrig, Nikolaenko, Raghunathan, Stebila, *ACM Conference on Computer and Communications Security 2016*, [http://eprint.iacr.org/2016/659](http://eprint.iacr.org/2016/659))
- `kex_sidh_cln16`: key exchange from the supersingular isogeny Diffie-Hellman problem (Costello, Naehrig, Longa, *CRYPTO 2016*, [https://eprint.iacr.org/2016/413](https://eprint.iacr.org/2016/413)), using the implementation of Microsoft Research [https://www.microsoft.com/en-us/research/project/sidh-library/](https://www.microsoft.com/en-us/research/project/sidh-library/)
- `kex_code_mcbits`: "McBits": key exchange from the error correcting codes, specifically Niederreiter's form of McEliece public key encryption using hidden Goppa codes (Bernstein, Chou, Schwabe, *CHES 2013*, [https://eprint.iacr.org/2015/610](https://eprint.iacr.org/2015/610)), using the implementation of McBits from [https://www.win.tue.nl/~tchou/mcbits/](https://www.win.tue.nl/~tchou/mcbits/))
- `kex_ntru`: NTRU: key transport using NTRU public key encryption (Hoffstein, Pipher, Silverman, *ANTS 1998*) with the EES743EP1 parameter set, wrapper around the implementation from the NTRU Open Source project [https://github.com/NTRUOpenSourceProject/NTRUEncrypt](https://github.com/NTRUOpenSourceProject/NTRUEncrypt))


Building and Running
--------------------

Builds have been tested on Mac OS X 10.11.6, macOS 10.12, Ubuntu 16.04.1, and Windows 10.

### Linux and macOS

On macOS, you need to install autoconf, automake and libtool:

	brew install autoconf automake libtool

To build, clone or download the source from GitHub, then simply type:

	autoreconf -i
	./configure
	make clean
	make

This will generate:

- `liboqs.a`: A static library with implementations for the algorithms listed in "Contents" above.
- `test_rand`: A simple test harness for the random number generator.  This will test the distance of PRNG output from uniform using statistical distance.
- `test_aes`: A simple test harness for AES.  This will test the correctness of the C implementation (and of the AES-NI implementation, if not disabled) of AES, and will compare the speed of these implementations against OpenSSL's AES implementation.
- `test_kex`: A simple test harness for the default key exchange algorithm.  This will output key exchange messages; indicate whether the parties agree on the session key or not over a large number of trials; and measure the distance of the sessions keys from uniform using statistical distance.

To run the tests, simply type:

	make test

To run benchmarks, run

	./test_kex --bench

and

	./test_aes --bench

### Windows

Windows binaries can be generated using the Visual Studio solution in the VisualStudio folder.

McBits is disabled by default in the Visual Studio build; follow these steps to enable it:

- Obtain the [libsodium library](https://libsodium.org); compile the static library from the Visual Studio projects.
- Add `ENABLE_CODE_MCBITS` and `SODIUM_STATIC` to the preprocessor definitions of the `oqs` and `test_kex` projects.
- Add the sodium "src/include" location to the "Additional Include Directories" in the oqs project C properties.
- Add the libsodium library to the "Additional Dependencies" in the `test_kex` project Linker properties.

NTRU is disabled by default in the Visual Studio build; follow these steps to enable it:

- Obtain the [NTRU library](https://github.com/NTRUOpenSourceProject/NTRUEncrypt); compile the NtruEncrypt_DLL from the Visual Studio projects.
- Add `ENABLE_NTRU` to the preprocessor definitions of the `oqs` and `test_kex` projects.
- Add the "NTRUEncrypt-master/include" location to the "Additional Include Directories" in the oqs project C properties.
- Add the NtruEncrypt_DLL.lib library to the "Additional Dependencies" in the `test_kex` project Linker properties.


Build options on Linux and macOS
--------------------------------

### Building with `kex_code_mcbits` enabled

The `kex_code_mcbits` key exchange method is not enabled by default.  In order to enable it, you need to carry out the following steps:

1. Install the [libsodium library](https://libsodium.org) and put it in your build path.  Your operating system may make this easy for you:
	- On macOS with brew: `brew install libsodium`
2. Build liboqs with the following option:

~~~
./configure --enable-mcbits
make clean
make
~~~

### Building with `kex_ntru` enabled

The `kex_ntru` key exchange method is not enabled by default.  In order to enable it, you need to carry out the following steps:

1. Download and build the NTRUEncrypt library from the NTRU Open Source project.  You can do this by running the script `download-and-build-ntru.sh`.
2. Build liboqs with the following option:

~~~
./configure --enable-ntru
make clean
make
~~~

Documentation
-------------

Some source files contain inline Doxygen-formatted documentation.  The documentation can be generated by running:

	doxygen

This will generate the `docs/html` directory.

Contributing and using
----------------------

We hope OQS will provide a framework for many post-quantum implementations.

In the immediate term, if you have feedback on our API ([kex.h](https://github.com/open-quantum-safe/liboqs/blob/master/src/kex/kex.h) or [rand.h](https://github.com/open-quantum-safe/liboqs/blob/master/src/rand/rand.h)), please contact us so we can ensure our API covers a wide range of implementation needs.

If you have or are writing an implementation of a post-quantum key exchange algorithm, we hope you will consider making an implementation that meets our API so that others may use it and would be happy to discuss including it directly in liboqs.  Please take a look at our [coding conventions](https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions).

If you would like to use liboqs in an application-level protocol, please get in touch and we can provide some guidance on options for using liboqs.

We are also interested in assistance from code reviewers.

Please contact Douglas Stebila <[stebilad@mcmaster.ca](mailto:stebilad@mcmaster.ca)>.

Current status and plans
------------------------

Our initial launch was on August 11, 2016, containing a single key exchange algorithm (`kex_rlwe_bcns15`) with a basic test harness.

Since our initial launch, we have made the following updates:

- Test harness for key exchange algorithms and random number generator
- Integration of liboqs into OpenSSL ([open-quantum-safe/openssl/](https://github.com/open-quantum-safe/openssl/))
- Licensing liboqs under the MIT license (see below)
- `kex_lwe_frodo` implementation
- Building on Windows
- Use of travis continuous integration system for testing
- `kex_rlwe_newhope` wrapper
- `kex_rlwe_msrln16` implementation contributed by Christian Paquin (Microsoft Research)
- `kex_sidh_cln16` implementation contributed by Christian Paquin (Microsoft Research)
- `kex_code_mcbits` wrapper
- `kex_ntru` wrapper

Our plans for the next few months can be found in [Milestone 1 - Key exchange](https://github.com/open-quantum-safe/liboqs/projects/2).

In the long term, we are also interested in including post-quantum signature schemes.

License
-------

liboqs is licensed under the MIT License; see [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/master/LICENSE.txt) for details.  liboqs includes some third party libraries or modules that are licensed differently; the corresponding subfolder contains the license that applies in that case.  In particular:

- `src/aes/aes.c`: public domain
- `src/kex_rlwe_bcns15`: public domain ([Unlicense](http://unlicense.org))
- `src/kex_rlwe_msrln16`: MIT License
- `src/kex_rlwe_msrln16/external`: public domain ([CC0](http://creativecommons.org/publicdomain/zero/1.0/))
- `src/kex_rlwe_newhope`: public domain
- `src/kex_sidh_cln16`: MIT License
- `src/kex_code_mcbits`: public domain
- `src/rand_urandom_chacha20/external`: public domain

Team
----

The Open Quantum Safe project is lead by [Michele Mosca](http://faculty.iqc.uwaterloo.ca/mmosca/) (University of Waterloo) and [Douglas Stebila](https://www.douglas.stebila.ca/research/) (McMaster University).

### Contributors

- Tancrède Lepoint (SRI)
- Shravan Mishra (University of Waterloo)
- Christian Paquin (Microsoft Research)
- Alex Parent (University of Waterloo)
- Sebastian Verschoor (University of Waterloo)

### Support

Development of Open Quantum Safe has been supported in part by the Tutte Institute for Mathematics and Computing.  Research projects which developed specific components of Open Quantum Safe have been supported by various research grants; see the source papers for funding acknowledgments.
