qTesla
======

- **Algorithm type**: signature
- **Main cryptographic assumption**: decisional ring learning with errors (R-LWE) problem.
- **Scheme authors**: Sedat Akleylek, Erdem Alkim, Paulo S. L. M. Barreto, Nina Bindel, Johannes Buchmann, Edward Eaton, Gus Gutoski, Juliane Krämer, Patrick Longa, Harun Polat, Jefferson E. Ricardini, and Gustavo Zanon
- **Authors' website**: https://qtesla.org/
- **Version**: qTesla Library version 1.0
- **Added to liboqs by**: Christian Paquin

Implementation
--------------

- **Source of implementation**: https://github.com/microsoft/qTESLA-Library
- **Implementation version**: https://github.com/microsoft/qTESLA-Library/commit/98b43cda4bbd9f3f0b1343a873a2a6076ddb9a2b
- **License**: MIT
- **Constant-time**: Yes
- **Optimizations**: Portable C with optional use of AVX2 and AESNI instructions (selected at compile-time, enabled by default if available)

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| qTesla-p-I    |    EUF-CMA     |              1              |         14880           |          5224           |          2592          |
| qTesla-p-III  |    EUF-CMA     |              3              |         38432           |         12392           |          5664          |

