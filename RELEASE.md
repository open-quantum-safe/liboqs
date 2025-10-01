# liboqs version 0.15.0

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **oqs-provider**: A standalone prototype [OpenSSL 3 provider](https://www.openssl.org/docs/manmaster/man7/provider.html) enabling liboqs-based quantum-safe and hybrid key authentication and exchange for TLS 1.3, X.509 certificate generation and CMS operations.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of BoringSSL; see https://github.com/open-quantum-safe/boringssl.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and key exchange into Secure Shell (SSH) version 2 in our fork of OpenSSH; see https://github.com/open-quantum-safe/openssh.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available for using the above libraries in applications, including Apache, Chromium, curl, haproxy, nginx, and Wireshark.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/open-quantum-safe/liboqs-go
- Java, via https://github.com/open-quantum-safe/liboqs-java
- Python 3, via https://github.com/open-quantum-safe/liboqs-python
- Rust, via https://github.com/open-quantum-safe/liboqs-rust

# Release notes
liboqs 0.15.0 was released on Oct ??, 2025.

## Deprecation notice
0.15 is the last version to officially support SPHINCS+. SPHINCS+ will be removed in the 0.16 release and replaced by SLH-DSA.

## Security

## Significant changes
- Added SLH-DSA implementation from [slhdsa-c](https://github.com/pq-code-package/slhdsa-c/)([#2175](#https://github.com/open-quantum-safe/liboqs/pull/2175), [#2237](https://github.com/open-quantum-safe/liboqs/pull/2237))
- Added NTRU back ([#2176](https://github.com/open-quantum-safe/liboqs/pull/2176))
- Removed all Dilithium implementations ([#2275](https://github.com/open-quantum-safe/liboqs/pull/2275))
- Added API for deterministic encapsulation ([#2221](https://github.com/open-quantum-safe/liboqs/pull/2221))
- SPHINCS+ is replaced with SLH-DSA for build option `OQS_ALGS_ENABLED=STD` ([#2290](https://github.com/open-quantum-safe/liboqs/pull/2290))

## Bug fixes
- Bug fixes for Windows builds ([#2181](https://github.com/open-quantum-safe/liboqs/pull/2181), [#2239](https://github.com/open-quantum-safe/liboqs/pull/2239))
- Disable strict aliasing on SPHINCS+-SHAKE ([#2264](https://github.com/open-quantum-safe/liboqs/pull/2264))
- Fixed AES-256-CTR counter size for AES-NI implementation ([#2252](https://github.com/open-quantum-safe/liboqs/pull/2252))
- Fixed missing ICICLE ML-KEM deterministic encapsulation APIs ([#2258](https://github.com/open-quantum-safe/liboqs/pull/2258))
- Fixed incorrect ICICLE ML-KEM-768 implementation ([#2288](https://github.com/open-quantum-safe/liboqs/pull/2288))
- Fixed `test_kem.c` using uninitialized `length_encaps_seed` ([#2266](https://github.com/open-quantum-safe/liboqs/pull/2266))

## Miscellaneous
- Updated ACVP test logic to version 1.1.0.40 ([#2172](https://github.com/open-quantum-safe/liboqs/pull/2172))
- Update nixpkgs from 24.05 to 25.05 ([#2187](https://github.com/open-quantum-safe/liboqs/pull/2187))
- Use `BCryptGenRandom` as the default Win32 system RNG and some other RNG security improvements on Windows ([#2225](https://github.com/open-quantum-safe/liboqs/pull/2225))
- Fixed incorrect GitHub Action workflow permissions ([#2261](https://github.com/open-quantum-safe/liboqs/pull/2261))
- More readable algorithm support status in `README.md` ([#2286](https://github.com/open-quantum-safe/liboqs/pull/2286)

## Contributors
- Saito Masataka (@saitomst)
- Vibhav Tiwari (@vibhav950)
- Douglas Stebila (@dstebila)
- Rodrigo Martín (@RodriM11)
- Bruce Xu (@xuganyu96)
- @nonam3e
- @iyanmv
- @max-p-log-p 
