liboqs algorithm datasheet: `sig_qtesla`
========================================

Summary
-------

- **Name**: qTesla
- **Algorithm type**: signature
- **Main cryptographic assumption**: decisional Ring Learning With Errors (R-LWE) problem.
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/qtesla.zip
- **Submitters (to NIST competition)**: Sedat Akleylek, Erdem Alkim, Paulo S. L. M. Barreto, Nina Bindel, Johannes Buchmann, Edward Eaton, Gus Gutoski, Juliane Krämer, Patrick Longa, Harun Polat, Jefferson E. Ricardini, and Gustavo Zanon
- **Submitters' website**: https://qtesla.org/
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| qTesla-p-I    |    EUF-CMA     |              1              |         14880           |          5184           |          2592          |
| qTesla-p-III  |    EUF-CMA     |              3              |         38432           |         12352           |          5664          |

Implementation
--------------

- **Source of implementation:** https://github.com/qtesla/qTesla
- **Implementation version:** https://github.com/microsoft/qTESLA-Library/commit/a5e60927e08d9f2fefea74138f208d14514ba5d1
- **License:** public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
