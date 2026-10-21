# Security Policy

## Supported Versions

We only support the most recent release.

Using any code prior to 0.12.0 is strongly discouraged due to a [known security vulnerability in HQC](https://github.com/open-quantum-safe/liboqs/security/advisories/GHSA-gpf4-vrrw-r8v7).

| Version | Supported          |
| ------- | ------------------ |
| 0.16.0  | :white_check_mark: |
| < 0.16  | :x:                |

## Deciding how to report

liboqs is intended for prototyping and experimentation, not for production use (see the [support limitations](https://github.com/open-quantum-safe/liboqs/blob/main/README.md#support-limitations)). In most cases there is therefore no deployed software that needs an embargo period before a fix is made public. Handling ordinary bugs in the open gets them fixed faster and reduces the load on a small, time-limited maintainer team. Before opening a private security advisory, please consider whether the problem is serious enough to warrant one and, potentially, a [CVE](https://www.cve.org).

### Report publicly, as an issue or (preferably) a pull request with a fix

- Denial of service: crashes, aborts, hangs or excessive resource consumption, provided no memory corruption or exposure of secret data is involved.
- Memory leaks.
- Failures reachable only by violating documented API preconditions (e.g., `NULL` arguments, undersized buffers, incorrect lengths).
- Non-constant-time behaviour outside the threat model below, or already documented in [tests/constant_time](https://github.com/open-quantum-safe/liboqs/tree/main/tests/constant_time).
- Bugs in tests, examples, benchmarks, build scripts or CI.
- Issues affecting only unsupported versions.
- Hardening suggestions, compiler warnings and static-analysis findings without a demonstrated security impact.

### Report privately, as a GitHub security advisory

- Memory-safety violations (out-of-bounds reads or writes, use-after-free, etc.) that can be triggered by attacker-controlled input such as public keys, ciphertexts or signatures.
- Secret-dependent timing behaviour within the threat model below.
- Functional flaws that break a security property, e.g., signature verification accepting invalid signatures, incorrect decapsulation of malformed ciphertexts, or faulty use of randomness in key generation, encapsulation or signing.
- Disclosure of secret key material through outputs or the public API.

### Not a liboqs vulnerability

- Cryptanalytic weaknesses in the algorithms themselves. These should be raised with the algorithm designers and the wider community; a broken algorithm may be removed from liboqs, temporarily or permanently.
- Many implementations in liboqs are imported from upstream projects, listed in the [algorithm datasheets](https://github.com/open-quantum-safe/liboqs/tree/main/docs/algorithms). Flaws in that code should also be reported upstream. liboqs may patch its copy on a best-effort basis.

## Criteria for a valid security report

- **Categorization**: The issue must fall into one of the categories listed under "Report privately" above.
- **Source**: The report must state whether the affected code is OQS-specific or imported from an upstream project and, in the latter case, whether the upstream has been notified.
- **Severity assessment**: The report must explain why the issue warrants a private advisory rather than a public issue.
- **Reproducibility**: The issue must be reproducible on the latest release or on `main`, with code or a clear set of steps demonstrating it.
- **Mitigation strategy**: The report must include suggested code changes or specific steps to mitigate the issue.

## Reporting a vulnerability

If, considering the above, you believe you have found a vulnerability, please [create a draft security advisory on GitHub](https://github.com/open-quantum-safe/liboqs/security/advisories/new). This is the only channel for private vulnerability reports. We do not run a bug bounty program.

Reports that do not meet the criteria above may be closed and redirected to the public issue tracker. If the volume of such reports becomes too high, or otherwise interferes with the project's normal workflow, maintainers reserve the right to take whatever action is needed, including restricting access to the Security Advisory interface.

## Threat Model

Some timing-based side-channel attacks are within the scope of our threat model. OQS tests for secret-dependent branches and memory accesses on Linux on x86\_64 for some algorithms. Where executed, all test failures are documented as either "passes," which we have assessed to be false positives, or "issues," which may constitute non–constant-time behaviour. The [algorithm datasheets](https://github.com/open-quantum-safe/liboqs/tree/main/docs/algorithms) indicate whether or not an implementation passes our constant-time tests, as well as whether or not it is expected to pass; a no-pass expectation also encompasses the case that no constant-time analysis has been done and not just the case that the algorithm is expected/has been ascertained to not pass. Details about passes and issues are available in the [tests/constant_time directory](https://github.com/open-quantum-safe/liboqs/tree/main/tests/constant_time). These tests do not encompass all classes of non–constant-time behaviour; for example, they do not detect possible variable-time instructions, such as `DIV`. Reports of non–constant-time behaviour that fall outside this scope will be considered on a case-by-case basis, with a priority on [Tier 1 platforms](https://github.com/open-quantum-safe/liboqs/blob/main/PLATFORMS.md#tier-1).


The following types of attacks are outside the scope of our threat model:

- same physical system side channel
- CPU / hardware flaws
- physical fault injection attacks (including Rowhammer-style attacks)
- physical observation side channels (such as power consumption, electromagnetic emissions)

Mitigations for security issues outside the stated threat model may still be applied depending on the nature of the issue and the mitigation.

(Based in part on https://openssl-library.org/policies/general/security-policy/index.html)

## Security Response Process

Security reports for liboqs will be handled in accordance with the [OQS security response process](https://github.com/open-quantum-safe/tsc/blob/main/security/response-process.md). Please also see the general [support disclaimer](README.md#support-limitations) for liboqs.
