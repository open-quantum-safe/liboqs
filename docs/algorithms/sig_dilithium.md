liboqs algorithm datasheet: `sig_dilithium`
===========================================

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
| DILITHIUM_2         |    EUF-CMA     |              1              |          1184           |          2800           |          2044          |
| DILITHIUM_3         |    EUF-CMA     |              2              |          1472           |          3504           |          2701          |
| DILITHIUM_4         |    EUF-CMA     |              3              |          1760           |          3856           |          3366          |


Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/bf3ef4ad405e00301cadf1b40bb96609b2968158
- **License:** public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
