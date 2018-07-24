liboqs nist-branch algorithm datasheet: `sig_dilithium`
===================================================

Summary
-------

- **Name**: Dilithium
- **Algorithm type**: signature
- **Main cryptographic assumption**: Module Learning With Errors (M-LWE) and Module-Short Integer Solution (M-SIS) problems.
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/CRYSTALS_Dilithium.zip
- **Submitters (to NIST competition)**: 
- **Submitters' website**: https://pq-crystals.org/dilithium/
- **Added to liboqs by**: Tancrède Lepoint

Parameter sets
--------------

| Parameter set    | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Dilithium_II_medium |    EUF-CMA     |              1              |          1184           |          2800           |          2044          |
| Dilithium_III_recommended  |    EUF-CMA     |              3              |          1472           |          3504           |          2701          |
| Dilithium_IV_very_high |    EUF-CMA     |              3              |          1760           |          3856           |          3366          |


Implementation
--------------

- **Source of implementation:** https://github.com/pq-crystals/dilithium
- **License:** public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64
