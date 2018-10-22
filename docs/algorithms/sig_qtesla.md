liboqs nist-branch algorithm datasheet: `sig_qtesla`
===================================================

Summary
-------

- **Name**: qTESLA
- **Algorithm type**: signature
- **Main cryptographic assumption**: decisional Ring Learning With Errors (R-LWE) problem.
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/qtesla.zip
- **Submitters (to NIST competition)**: Sedat Akleylek, Erdem Alkim, Paulo S. L. M. Barreto, Nina Bindel, Johannes Buchmann, Edward Eaton, Gus Gutoski, Juliane Krämer, Patrick Longa, Harun Polat, Jefferson E. Ricardini, and Gustavo Zanon
- **Submitters' website**: https://qtesla.org/
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set    | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| qTESLA_I         |    EUF-CMA     |              1              |          1504           |          2112           |          1376          |
| qTESLA_III_size  |    EUF-CMA     |              3              |          2976           |          4160           |          2720          |
| qTESLA_III_speed |    EUF-CMA     |              3              |          3104           |          4160           |          2848          |


Implementation
--------------

- **Source of implementation:** https://github.com/qtesla/qTesla
- **Implementation version:** https://github.com/qtesla/qTesla/commit/5e921da989b9b44aba95f63d9c28927d518f630c
- **License:** public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
