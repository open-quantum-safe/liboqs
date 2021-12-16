liboqs version 0.7.1
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OQS-OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **oqs-provider**: A standalone prototype of liboqs-based key exchange for TLS 1.3 using the OpenSSL 3 (alpha) provider integration mechanism.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of BoringSSL; see https://github.com/open-quantum-safe/boringssl.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and key exchange into Secure Shell (SSH) version 2 in our fork of OpenSSH; see https://github.com/open-quantum-safe/openssh.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available for using the above libraries in applications, including Apache, Chromium, curl, haproxy, nginx, and Wireshark.  Performance of liboqs in several settings is measured at https://openquantumsafe.org/benchmarking/.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/open-quantum-safe/liboqs-go
- Java, via https://github.com/open-quantum-safe/liboqs-java
- .NET, via https://github.com/open-quantum-safe/liboqs-dotnet
- Python 3, via https://github.com/open-quantum-safe/liboqs-python
- Rust, via https://github.com/open-quantum-safe/liboqs-rust

Release notes
=============

This is version 0.7.1 of liboqs.  It was released on December 16, 2021.

What's New
----------

This release continues from the 0.7.0 release of liboqs. 

### Key encapsulation mechanisms

- Add NTRU level 5 parameter sets (ntruhps40961229, ntruhrss1373)
- Add NTRU Prime level 5 parameter sets (ntrulpr1277, sntrup1277)
- Add ARMv8 aarch64 optimized implementations of Kyber and SABER
- Minor updates to Kyber, NTRU, NTRU Prime, and SIKE implementations

### Digital signature schemes

- Minor updates to Dilithium implementation

### Other changes

- Optimized AES implementation on ARMv8 with crypto extensions.
- Preliminary support for building on S390x platform
- Improved build configurations on ARM platforms
- Improvements to benchmarking harness, with improved precision on ARM platforms

---

Detailed changelog
------------------

* Use Github Actions for some CI by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1069
* fixing #1076 by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1078
* Add instructions on how to build shared/dynamic library to the README by @Wandering-Consciousness in https://github.com/open-quantum-safe/liboqs/pull/1077
* Updated SIKE to latest upstream commit by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1079
* enabling flexible build dir name in testing by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1080
* run optimized constant time tests only on haswell by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1082
* increase constant time test timeout allowing slow runners to complete [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1085
* skip algorithms for constant_time tests by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1089
* Kyber/Dilithium copy_from_upstream by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1088
* do full upstream doc generation [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1066
* Adding armv8 crypto extensions to AES by @tedeaton in https://github.com/open-quantum-safe/liboqs/pull/1086
* Enabling out-of-source build-and-test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1091
* adding pyyaml prerequisite to README [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1094
* correct out-of-source build-and-test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1093
* Update to PQClean commit 5b8ef3baea3ffdfbf688a3a1bb8f02de44a67ec0 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1097
* skip more SPHINCS algs from weekly testing by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1102
* S390x support by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1103
* Update BIKE documentation to reflect only little endian support [skip ci] by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1105
* Add powerpc64le name as alternative to ppc64le by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1112
* Disable BIKE build on s390x (big endian) by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1115
* adds S/NTRUP1277 by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1111
* Updated Picnic to v3.0.5 by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1116
* Skip non-enabled algs in constant time tests by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1120
* Updated valgrind constant time exception for picnic by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1121
* Adding support for multiple upstream to copy_from_upstream, as well as pqclean's aarch64 kyber implementation by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1117
* Added Keccak constant time exclusion for Picnic AVX2 by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1124
* Adding multiple upstream support to doc generation by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1123
* adding ntruhps40961229 by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1126
* Fix gcc 11 error for Kyber on arm64 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1127
* LF everywhere by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1133
* skip more ClassicMcEliece weekly CI constant time tests [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1134
* more code style checks by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1135
* Added supported-platforms compile guards in the SIKE CMakeLists by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1136
* Adding arm64_v8 optimized versions of saber. by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1131
* add downlevel compiler warning by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1137
* enable ARM64 optimizations when using OQS_MINIMAL_BUILD option by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1139
* Adding a speed_common script to benchmark OQS AES and hash algorithms by @tedeaton in https://github.com/open-quantum-safe/liboqs/pull/1144
* NIST copyright clarification by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1142
* Option to use PMU for benchmarking on ARM by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1147
* aarch64 dist build by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1148
* Tweaks to benchmarking code by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1150
* release performance regression test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1152
* Added spdx headers to noregress.[py|sh] by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1154
* Output documented build options  by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1155
* docs: set license MIT to Falcon Signature by @ax1 in https://github.com/open-quantum-safe/liboqs/pull/1156
* add trigger for oqs-provider CI by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1157

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.7.0...0.7.1
