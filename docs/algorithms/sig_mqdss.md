liboqs nist-branch algorithm datasheet: `sig_mqdss`
===================================================

Summary
-------

- **Name**: MQDSS
- **Algorithm type**: signature
- **Main cryptographic assumption**: intractability of the search version of the MQ problem in the average case
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/MQDSS-Round2.zip
- **Submitters (to NIST competition)**: Ming-Shing Chen, Andreas Hülsing, Joost Rijneveld, Simona Samardjiska, Peter Schwabe
- **Submitters' website**: http://mqdss.org
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| MQDSS-31-48   |     EUF-CMA    |             1-2             |            46           |            13           |          20854         |
| MQDSS-31-64   |     EUF-CMA    |             3-4             |            64           |            26           |          43728         |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean, originally from https://github.com/joostrijneveld/MQDSS/commit/0c64d4d67a37051c1299a3049a5bb8984ca27ecc
- **Implementation version:** https://github.com/PQClean/PQClean/commit/4270a5416103b5b11697adbb04bab5ac7bdce55c
- **License:** CC0 1.0 Universal
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
