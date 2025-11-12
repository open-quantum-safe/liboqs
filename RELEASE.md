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
liboqs 0.15.0 was released on Friday, Nov 14, 2025.

## Deprecation notice
liboqs 0.15.0 is the last version to officially support SPHINCS+. SPHINCS+ will be removed in the 0.16.0 release and replaced by SLH-DSA. liboqs 0.15.0 also removes support for Dilithium.

## Security issues
None reported

<!--- git log HEAD...0.14.0 --->
## Significant changes
- **Integrated SLH-DSA implementation from [pq-code-package/slhdsa-c](https://github.com/pq-code-package/slhdsa-c/)**
    - SLH-DSA ACVP tests ([#2237](https://github.com/open-quantum-safe/liboqs/pull/2237))
    - Integrate SLH-DSA-C Library ([#2175](https://github.com/open-quantum-safe/liboqs/pull/2175))
- Added NTRU back ([#2176](https://github.com/open-quantum-safe/liboqs/pull/2176))
- Removed all Dilithium implementations ([#2275](https://github.com/open-quantum-safe/liboqs/pull/2275))
- Replaced SPHINCS+ with SLH-DSA for CMake build option `OQS_ALGS_ENABLED=STD` ([#2290](https://github.com/open-quantum-safe/liboqs/pull/2290))
- Updated CROSS to version 2.2 ([#2247](https://github.com/open-quantum-safe/liboqs/pull/2247))
- Included DeriveEncapsulation functionality ([#2221](https://github.com/open-quantum-safe/liboqs/pull/2221))
- Integrated ML-KEM implementation from ICICLE-PQC ([#2216](https://github.com/open-quantum-safe/liboqs/pull/2216))

## Bug fixes
- Fixed erroneously disabled LMS variants with build flag `OQS_ENABLE_SIG_STFL_LMS` ([#2310](https://github.com/open-quantum-safe/liboqs/pull/2310))
- Fixed incorrect import in OV-III-pkc_skc ([#2299](https://github.com/open-quantum-safe/liboqs/pull/2299))
- Fixed incorrect actual signature length in signature full-cycle speed test ([#2293](https://github.com/open-quantum-safe/liboqs/pull/2293))
- Fixed ICICLE ML-KEM integration ([#2288](https://github.com/open-quantum-safe/liboqs/pull/2293))
- Disabled strict aliasing on SPHINCS+-SHAKE ([#2264](https://github.com/open-quantum-safe/liboqs/pull/2264))
- Fixed typo in `test_kem.c` ([#2281](https://github.com/open-quantum-safe/liboqs/pull/2281))
- Fixed uninitialized `length_encaps_seed` for NTRU implementations ([#2266](https://github.com/open-quantum-safe/liboqs/pull/2266))
<!-- NOTES: #2258 was overwritten by #2288 and thus omitted from release notes for the sake of clarity -->
<!-- - Patched ICICLE ML-KEM for deterministic encapsulation ([#2258](https://github.com/open-quantum-safe/liboqs/pull/2258)) -->
- Changed 64 bit add to 32 bit add to wrap on 32 bit counter for AES-CTR AES-NI implementation ([#2252](https://github.com/open-quantum-safe/liboqs/pull/2252))
- Improved random number generator security ([#2225](https://github.com/open-quantum-safe/liboqs/pull/2225))
- Avoided invalid compiler options for MSVC ([#2239](https://github.com/open-quantum-safe/liboqs/pull/2239))
- Added Classic McEliece sanitization patch ([#2218](https://github.com/open-quantum-safe/liboqs/pull/2218))

## Miscellaneous
- Deprecated `noregress` scripts ([#2295](https://github.com/open-quantum-safe/liboqs/pull/2295))
- Updated no-pass explanation for constant-time testing ([#2294](https://github.com/open-quantum-safe/liboqs/pull/2294))
- Fixed weekly test failures in GitHub Action ([#2285](https://github.com/open-quantum-safe/liboqs/pull/2294))
- Improved algorithm support readability in README.md ([#2286](https://github.com/open-quantum-safe/liboqs/pull/2286))
- Re-enabled all ACVP tests ([#2283](https://github.com/open-quantum-safe/liboqs/pull/2283))
- Moved continuous benchmarking to weekly tests ([#2276](https://github.com/open-quantum-safe/liboqs/pull/2276))
- Linked to contribution wishlist from CONTRIBUTING.md ([#2273](https://github.com/open-quantum-safe/liboqs/pull/2273))
- Added content:read permission to scorecard workflow ([#2261](https://github.com/open-quantum-safe/liboqs/pull/2261))
- Added guidance and questions on generative AI use ([#2269](https://github.com/open-quantum-safe/liboqs/pull/2269))
- Moved `linux_arm_emulated` to extended tests ([#2236](https://github.com/open-quantum-safe/liboqs/pull/2236))
- Fixed pluralization in docs: "key encapsulation mechanisms (KEMs)" ([#2255](https://github.com/open-quantum-safe/liboqs/pull/2255))
- Fixed permissions for poutine_analysis job ([#2246](https://github.com/open-quantum-safe/liboqs/pull/2246))
- Disabled testing on Travis CI ([#2248](https://github.com/open-quantum-safe/liboqs/pull/2248))
- Upgraded Jinja to 3.1.6 ([#2214](https://github.com/open-quantum-safe/liboqs/pull/2214))
- Updated license info for ML-KEM ([#2250](https://github.com/open-quantum-safe/liboqs/pull/2250))
- Fixed typos across codebase ([#2244](https://github.com/open-quantum-safe/liboqs/pull/2244))
- Restructured basic checks ([#2233](https://github.com/open-quantum-safe/liboqs/pull/2233))
- Fixed code scanning workflow ([#2217](https://github.com/open-quantum-safe/liboqs/pull/2217))
- Changed Nix install action to verified ([#2212](https://github.com/open-quantum-safe/liboqs/pull/2212))
- Removed `armel` comment ([#2231](https://github.com/open-quantum-safe/liboqs/pull/2231))
- Removed `armel` support from `PLATFORMS.md` ([#2232](https://github.com/open-quantum-safe/liboqs/pull/2232))
- Added Poutine SASL ([#2213](https://github.com/open-quantum-safe/liboqs/pull/2213))
- Updated nixpkgs from 24.05 to 25.05 ([#2187](https://github.com/open-quantum-safe/liboqs/pull/2187))
- Added Custom Timeout to CI ([#2189](https://github.com/open-quantum-safe/liboqs/pull/2189))
- Updated ACVP to 1.1.0.40 ([#2172](https://github.com/open-quantum-safe/liboqs/pull/2172))
- Switched to dev mode for 0.14.1 ([#2199](https://github.com/open-quantum-safe/liboqs/pull/2199))

## New contributors
- Ganyu (Bruce) Xu (@xuganyu96)
- Ethan Li (@lir2002)
- @nonam3e (associated with Ingonyama)
- Saito Masataka (@saitomst)
- @max-p-log-p
- Vibhav Tiwari (@vibhav950)
- Dmitry (@mdqst)
- Rodrigo Martín (@RodriM11)
- @sukrucildirr
- Kitsios Konstantinos (@kitsiosk)

<!-- TODO: include full changelog statement at full release -->
