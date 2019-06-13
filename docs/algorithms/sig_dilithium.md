liboqs nist-branch algorithm datasheet: `sig_dilithium`
===================================================

Summary
-------

- **Name**: CRYSTALS-DILITHIUM
- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/CRYSTALS-Dilithium-Round2.zip
- **Submitters (to NIST competition)**: Vadim Lyubashevsky, Leo Ducas, Eike Kiltz, Tancrede Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Submitters' website**: https://pq-crystals.org/dilithium/index.shtml
- **Added to liboqs by**: Dimitrios Sikeridis, Panos Kampanakis

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Dilithium-1024x768  |    EUF-CMA     |              1              |          1184           |          2800           |          2044          |
| Dilithium-1280x1024 |    EUF-CMA     |              2              |          1472           |          3504           |          2701          |
| Dilithium-1536x1280 |    EUF-CMA     |              3              |          1760           |          3856           |          3366          |


Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/e56b2e5556934963cae177062fca17798760afd2
- **License:** public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
