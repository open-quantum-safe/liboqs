# Security Policy

## Supported Versions

We only support the most recent release.

Using any code prior to 0.10.1 is strongly discouraged due to a [known security vulnerability in Kyber](https://github.com/open-quantum-safe/liboqs/releases/tag/0.10.1).

| Version | Supported          |
| ------- | ------------------ |
| 0.12.0  | :white_check_mark: |
| < 0.12  | :x:                |

## Reporting a Vulnerability

Please follow [this information to report a vulnerability](https://openquantumsafe.org/liboqs/security.html#reporting-security-bugs).

## Threat Model

For certain algorithms and platforms, liboqs aims for "constant-time code" in the sense of no branching or memory accesses based on secret data.

- Algorithms: The [algorithm datasheets](https://github.com/open-quantum-safe/liboqs/blob/main/docs/algorithms/) indicate whether each algorithm and implementation aims for constant-time behaviour. These should be read in conjunction with the [issues/passes files for each algorithm's constant-time tests](https://github.com/open-quantum-safe/liboqs/tree/main/tests/constant_time).
- Platforms: We aim for constant-time behaviour on our [Tier 1 platforms](https://github.com/open-quantum-safe/liboqs/blob/main/PLATFORMS.md).

The following types of attacks are outside the scope of our threat model:

- same physical system side channel
- CPU / hardware flaws
- physical fault injection attacks (including Rowhammer-style attacks)
- physical observation side channels (such as power consumption, electromagnetic emissions)

Mitigations for security issues outside the stated threat model may still be applied depending on the nature of the issue and the mitigation.

(Based in part on https://openssl-library.org/policies/general/security-policy/index.html)
