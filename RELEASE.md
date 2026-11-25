# liboqs version 0.17.0-rc1

The **Open Quantum Safe (OQS) project** has the goal of developing and
prototyping quantum-resistant cryptography. More information on OQS can be
found on our website: <https://openquantumsafe.org/> and on Github at
<https://github.com/open-quantum-safe/>.

**liboqs** is an open source C library for quantum-resistant cryptographic
algorithms. Details about liboqs can be found in
[README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).
See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **oqs-provider**: A standalone prototype [OpenSSL 3
provider](https://www.openssl.org/docs/manmaster/man7/provider.html) enabling
liboqs-based quantum-safe and hybrid key authentication and exchange for TLS
1.3, X.509 certificate generation and CMS operations.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and
key exchange into TLS 1.3 in our fork of BoringSSL; see
<https://github.com/open-quantum-safe/boringssl>.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and
key exchange into Secure Shell (SSH) version 2 in our fork of OpenSSH; see
<https://github.com/open-quantum-safe/openssh>.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available
for using the above libraries in applications, including Apache, Chromium,
curl, haproxy, nginx, and Wireshark.

liboqs can also be used in the following programming languages via
language-specific wrappers:

- C++, via <https://github.com/open-quantum-safe/liboqs-cpp>
- Go, via <https://github.com/open-quantum-safe/liboqs-go>
- Java, via <https://github.com/open-quantum-safe/liboqs-java>
- Python 3, via <https://github.com/open-quantum-safe/liboqs-python>
- Rust, via <https://github.com/open-quantum-safe/liboqs-rust>

## Release notes

liboqs 0.17.0-rc1 was released on Tuesday, November 24, 2026.

### Breaking changes

- **OpenSSL 1.1.1 is no longer supported.** When building with the default
  `OQS_USE_OPENSSL=ON`, OpenSSL 3.0 or newer is now required, and CMake fails at
  configure if only an older version is found. Platforms that only ship OpenSSL
  1.1.1 (e.g. Ubuntu 20.04, RHEL 8, Debian 11, Amazon Linux 2) must install
  OpenSSL 3.x or configure with `-DOQS_USE_OPENSSL=OFF`.
  [#2661](https://github.com/open-quantum-safe/liboqs/pull/2661)
- **The libjade Kyber implementations were removed.** The formally verified
  Kyber512 and Kyber768 implementations (ref and avx2) imported from libjade are
  no longer included, because their upstream source can no longer be retrieved
  or verified. The `OQS_LIBJADE_BUILD` option, the `OQS_ENABLE_LIBJADE_KEM_*`
  options and their `oqsconfig.h` macros are gone, and configuring with
  `-DOQS_LIBJADE_BUILD=ON` now fails. Kyber512 and Kyber768 remain available
  through the pq-crystals implementations, with unchanged outputs.
  [#2714](https://github.com/open-quantum-safe/liboqs/pull/2714)
- **Classic McEliece is disabled by default.** Following the [OQS TSC meeting of
  2026-10-20](https://github.com/open-quantum-safe/tsc/blob/main/meetings/2026-10-20/minutes.md),
  and based on recent cryptanalysis reports, a BSI recommendation and the lack
  of upstream maintenance, `OQS_ENABLE_KEM_CLASSIC_MCELIECE` now defaults to
  `OFF`; configure with `-DOQS_ENABLE_KEM_CLASSIC_MCELIECE=ON` to build it.
  `OQS_ALGS_ENABLED=NIST_R4` still enables every Classic McEliece parameter set.
  [#2640](https://github.com/open-quantum-safe/liboqs/pull/2640)
- **MQOM v2 was replaced by MQOM v3.** The `mqom2_*` algorithm identifiers are
  removed and the NIST round 3 `mqom3_*` parameter sets are added. MQOM v3 is a
  major update of the scheme and is not compatible with v2.
  [#2563](https://github.com/open-quantum-safe/liboqs/pull/2563)
- **UOV, MAYO and SNOVA were updated to NIST round 3.** Algorithm identifiers are
  unchanged, but parameters, key and signature sizes, and outputs differ from
  0.16.0 for most parameter sets (for UOV, OV-Ip, OV-III and OV-V change; OV-Is
  is unchanged). Keys and signatures from 0.16.0 are not interoperable.
  [#2554](https://github.com/open-quantum-safe/liboqs/pull/2554),
  [#2564](https://github.com/open-quantum-safe/liboqs/pull/2564),
  [#2562](https://github.com/open-quantum-safe/liboqs/pull/2562)
- **Windows static builds no longer export the public API.** `OQS_API` now
  expands to `__declspec(dllexport)` only when liboqs is built as a shared
  library, so applications linking the static library no longer re-export the
  entire liboqs API.
  [#2584](https://github.com/open-quantum-safe/liboqs/pull/2584)

### Security issues

- Fixed out-of-bounds read in SLH-DSA verification on a short signature
[#2587](https://github.com/open-quantum-safe/liboqs/pull/2587)
- Added missing signature length check in CROSS `crypto_sign_verify`
[#2555](https://github.com/open-quantum-safe/liboqs/pull/2555)
- Fixed out-of-bounds access in `xmss_core_sign` on secret key OID mismatch
[#2511](https://github.com/open-quantum-safe/liboqs/pull/2511)
- Locked the secret key in XMSS/XMSS^MT `sigs_remaining`
[#2621](https://github.com/open-quantum-safe/liboqs/pull/2621)
- Made LMS stateful signing fail closed and validated LMS parameter sets
[#2608](https://github.com/open-quantum-safe/liboqs/pull/2608)
- Fixed out-of-bounds read in `hss_validate_signature_init`
[#2507](https://github.com/open-quantum-safe/liboqs/pull/2507)
- Fixed out-of-bounds write in SHA-256 `inc_blocks` on a zero-block call
[#2627](https://github.com/open-quantum-safe/liboqs/pull/2627)
- Fixed undersized allocation in `oqs_aes128_load_schedule_no_bitslice`
[#2515](https://github.com/open-quantum-safe/liboqs/pull/2515)
- Fixed integer overflow in `OQS_MEM_aligned_alloc` size calculation
[#2492](https://github.com/open-quantum-safe/liboqs/pull/2492)

### Significant changes

#### Add remaining NIST on-ramp Round 3 Candidates

Four signature schemes from NIST's Additional Digital Signatures process were
added, all enabled by default:

- **SQIsign** (`SQIsign-p324-3`, `SQIsign-p500-27`, `SQIsign-p664-17`), with a
  portable and an arm64 backend.
  [#2593](https://github.com/open-quantum-safe/liboqs/pull/2593),
  [#2711](https://github.com/open-quantum-safe/liboqs/pull/2711)
- **SDitH** (`SDitH3-L<1|3|5>-gf2-<fast|short>[-cipherpow]`). This is
  not built on Windows, s390x or ppc64.
  [#2586](https://github.com/open-quantum-safe/liboqs/pull/2586)
- **QR-UOV**, with ref, opt and avx2 implementations.
  [#2582](https://github.com/open-quantum-safe/liboqs/pull/2582)
- **FAEST** (`FAEST-<128|192|256><F|S>` and `FAEST-EM-<128|192|256><F|S>`).
  [#2592](https://github.com/open-quantum-safe/liboqs/pull/2592)

#### ML-DSA External-μ

New `ML-DSA-<44|65|87>-extmu` algorithm identifiers expose the FIPS 204
External-μ signing interface, where the caller supplies the precomputed message
representative μ. [#2366](https://github.com/open-quantum-safe/liboqs/pull/2366)

#### Updated implementations

- mlkem-native updated to v2.0.0
[#2521](https://github.com/open-quantum-safe/liboqs/pull/2521)
- mldsa-native updated to v2.0.0, and built with `MLD_CONFIG_REDUCE_RAM` when
`OQS_MEMOPT_BUILD` is set.
[#2522](https://github.com/open-quantum-safe/liboqs/pull/2522),
[#2660](https://github.com/open-quantum-safe/liboqs/pull/2660)
- HQC updated to v5.1.0, with new AVX2 backends, thread-safe x86_64 scratch
buffers and cleansing of secrets.
[#2692](https://github.com/open-quantum-safe/liboqs/pull/2692)

#### AES improvements

Added AES-192 (`OQS_AES192_ECB_*`).
The AES-NI and ARMv8 ECB and CTR paths now process four blocks at a time.
[#2571](https://github.com/open-quantum-safe/liboqs/pull/2571),
[#2594](https://github.com/open-quantum-safe/liboqs/pull/2594)

#### Constant-time analysis framework

New constant-time testing framework that leverages Valgrind's memcheck with
the Kyberslash patch (Valgrind-Varlat) and Clang's MemorySanitizer (MemSan).
These can be run using the unified `tests/ct_tooling/ct_test.sh` tool.
[#2449](https://github.com/open-quantum-safe/liboqs/pull/2449)

### Bug fixes

- Fixed RDTSC bug on x86_64
[#2708](https://github.com/open-quantum-safe/liboqs/pull/2708)
- Fixed XGETBV inline assembly usage in CPU extension detection
[#2503](https://github.com/open-quantum-safe/liboqs/pull/2503)
- Fixed ARMv8 SHA3 handling in signature and KEM families
[#2597](https://github.com/open-quantum-safe/liboqs/pull/2597)
- Aligned XKCP extraction parameters with the dispatch header
[#2652](https://github.com/open-quantum-safe/liboqs/pull/2652)
- Linked libm via a bare `m` flag for portable static and cross linking
[#2664](https://github.com/open-quantum-safe/liboqs/pull/2664)
<!-- e77d81d superseded by #2664 -->
- Allowed `CMAKE_POSITION_INDEPENDENT_CODE` to be overridden
[#2524](https://github.com/open-quantum-safe/liboqs/pull/2524)
- Made `OPENSSL_NO_STDIO` visible by including `opensslconf.h`
[#2576](https://github.com/open-quantum-safe/liboqs/pull/2576)
- Fixed ARM SHA2 build with AppleClang on macOS
[#2508](https://github.com/open-quantum-safe/liboqs/pull/2508)
- Fixed AND separator in ARM64 `required_flags` CMake guard
[#2567](https://github.com/open-quantum-safe/liboqs/pull/2567)
- Fixed `strdup` deprecation in the MSVC CRT
[#2703](https://github.com/open-quantum-safe/liboqs/pull/2703)
- Fixed two CMP warnings in CMake >= 4.1 and 4.4
[#2704](https://github.com/open-quantum-safe/liboqs/pull/2704)
- Fixed XMSS code scanning issues
[#2577](https://github.com/open-quantum-safe/liboqs/pull/2577)

### Miscellaneous

- Switched C code formatting from astyle to clang-format
[#2649](https://github.com/open-quantum-safe/liboqs/pull/2649)
- Added ML-KEM ACVP tests
[#2386](https://github.com/open-quantum-safe/liboqs/pull/2386)
- Adapted and fixed the latest Wycheproof tests
[#2387](https://github.com/open-quantum-safe/liboqs/pull/2387)
- Improved SHA3 tests
[#2450](https://github.com/open-quantum-safe/liboqs/pull/2450)
- Added a test for NULL guards after `OQS_MEM_malloc` calls
[#2633](https://github.com/open-quantum-safe/liboqs/pull/2633)
- Added a CodeQL query enforcing OpenSSL return code handling
[#2634](https://github.com/open-quantum-safe/liboqs/pull/2634)
- Fixed minor constant-time testing bugs in CI
[#2677](https://github.com/open-quantum-safe/liboqs/pull/2677),
[#2716](https://github.com/open-quantum-safe/liboqs/pull/2716)
- Marked `rc` as public via declassification in tests
[#2473](https://github.com/open-quantum-safe/liboqs/pull/2473)
- Renamed shadowed `test_sig` to `test_sig_stfl` in speed tests
[#2541](https://github.com/open-quantum-safe/liboqs/pull/2541)
- Ignored `.venv` in `tests/test_spdx.sh`
[#2557](https://github.com/open-quantum-safe/liboqs/pull/2557)
- Added another two batches of declared algorithm support
[#2505](https://github.com/open-quantum-safe/liboqs/pull/2505),
[#2722](https://github.com/open-quantum-safe/liboqs/pull/2722)
- Added security research guidance and a threat model
[#2477](https://github.com/open-quantum-safe/liboqs/pull/2477)
- Expanded the SECURITY.md classification of vulnerabilities
[#2639](https://github.com/open-quantum-safe/liboqs/pull/2639)
- Documented caller length check responsibilities for FIPS 203
[#2688](https://github.com/open-quantum-safe/liboqs/pull/2688)
- Documented the imported implementation status contract
[#2469](https://github.com/open-quantum-safe/liboqs/pull/2469)
- Added algorithm-integrator guidance to the `copy_from_upstream` README
[#2501](https://github.com/open-quantum-safe/liboqs/pull/2501)
- Fixed stale loop variable in `copy_from_upstream` arch-specific ignore check
and skipped empty SIG and KEM implementation targets
[#2547](https://github.com/open-quantum-safe/liboqs/pull/2547),
[#2654](https://github.com/open-quantum-safe/liboqs/pull/2654)
- Linked TSC review guidelines from GOVERNANCE.md and removed the README's
obsolete survey reference
[#2612](https://github.com/open-quantum-safe/liboqs/pull/2612),
[#2611](https://github.com/open-quantum-safe/liboqs/pull/2611)
- Replaced a relative link with an absolute URL
[#2622](https://github.com/open-quantum-safe/liboqs/pull/2622)
- Added parentheses around uses of the `sizeof` operator
[#2471](https://github.com/open-quantum-safe/liboqs/pull/2471)
- Added Markdown documentation linting
[#2446](https://github.com/open-quantum-safe/liboqs/pull/2446)
- Added `requirements.txt` for Python dependencies, pinned `charset-normalizer`
and fixed the pre-commit hook's requirements path
[#2510](https://github.com/open-quantum-safe/liboqs/pull/2510),
[#2578](https://github.com/open-quantum-safe/liboqs/pull/2578),
[#2544](https://github.com/open-quantum-safe/liboqs/pull/2544)
- Updated Nixpkgs to 26.05
[#2457](https://github.com/open-quantum-safe/liboqs/pull/2457)
- Reduced CI runtime: per-platform and per-stage test suite selection, a lighter
scope on PRs, a workflow optimizer and runtime report, faster platform-test
scheduling, and removal of retired macOS 14 runners
[#2693](https://github.com/open-quantum-safe/liboqs/pull/2693),
[#2667](https://github.com/open-quantum-safe/liboqs/pull/2667),
[#2509](https://github.com/open-quantum-safe/liboqs/pull/2509),
[#2500](https://github.com/open-quantum-safe/liboqs/pull/2500),
[#2493](https://github.com/open-quantum-safe/liboqs/pull/2493),
[#2595](https://github.com/open-quantum-safe/liboqs/pull/2595),
[#2663](https://github.com/open-quantum-safe/liboqs/pull/2663)

### New contributors

- Joe (@jc2a)
- @0rlych1kk4
- @charishma1407
- Matvey Teslov (@msteslov)
- Mohamad Momeni (@mohamadm0meni)
- Scott Arciszewski (@tob-scott-a)
- Samy (@Ap4sh)
- Owen Waldron (@owaldron)
- @Ramya-9353
- Maxim Gagiev (@maximilliangrand)
- Eva Crystal (@0xiviel)
- Mr Water (@MrWater00)
- Johnson Ajibi (@Johnsonajibi)
- Aryan K. Gupta (@AKG451)
- Siyâvash (@mkhellat)
- Laasya priya Bhadri (@laasyapriya2007)
- Ishan Kumthekar (@ishanrk)
- Nikolai Puch (@Dowafu)
- Jacob Alzén (@Jacalz)
