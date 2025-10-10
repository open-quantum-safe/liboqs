# Security Policy

## Supported Versions

We only support the most recent release.

Using any code prior to 0.12.0 is strongly discouraged due to a [known security vulnerability in HQC](https://github.com/open-quantum-safe/liboqs/security/advisories/GHSA-gpf4-vrrw-r8v7).

| Version | Supported          |
| ------- | ------------------ |
| 0.15.0  | :white_check_mark: |
| < 0.15  | :x:                |

## Reporting a Vulnerability

Please follow [this information to report a vulnerability](https://openquantumsafe.org/liboqs/security.html#reporting-security-bugs).

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
