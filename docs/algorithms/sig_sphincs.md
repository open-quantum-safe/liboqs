liboqs nist-branch algorithm datasheet: `sig_sphincs`
=====================================================

Summary
-------

- **Name**: SPHINCS+
- **Algorithm type**: signature
- **Main cryptographic assumption**: hash-based signatures
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/SPHINCS-Round2.zip
- **Submitters (to NIST competition)**: Andreas Hulsing, Daniel J. Bernstein, Christoph Dobraunig, Maria Eichlseder, Scott Fluhrer, Stefan-Lukas Gazdag, Panos Kampanakis, Stefan Kolbl, Tanja Lange, Martin M Lauridsen, Florian Mendel, Ruben Niederhagen, Christian Rechberger, Joost Rijneveld, Peter Schwabe, Jean-Philippe Aumasson
- **Submitters' website**: https://sphincs.org/
- **Added to liboqs by**: Thom Wiggers and Douglas Stebila

Parameter sets
--------------

| Parameter set                 | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|-------------------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| SHPINCS+-Haraka-128f-robust   |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SHPINCS+-Haraka-128f-simple   |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SHPINCS+-Haraka-128s-robust   |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SHPINCS+-Haraka-128s-simple   |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SHPINCS+-Haraka-192f-robust   |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SHPINCS+-Haraka-192f-simple   |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SHPINCS+-Haraka-192s-robust   |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SHPINCS+-Haraka-192s-simple   |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SHPINCS+-Haraka-256f-robust   |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SHPINCS+-Haraka-256f-simple   |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SHPINCS+-Haraka-256s-robust   |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SHPINCS+-Haraka-256s-simple   |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SHPINCS+-SHAKE256-128f-robust |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SHPINCS+-SHAKE256-128f-simple |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SHPINCS+-SHAKE256-128s-robust |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SHPINCS+-SHAKE256-128s-simple |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SHPINCS+-SHAKE256-192f-robust |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SHPINCS+-SHAKE256-192f-simple |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SHPINCS+-SHAKE256-192s-robust |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SHPINCS+-SHAKE256-192s-simple |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SHPINCS+-SHAKE256-256f-robust |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SHPINCS+-SHAKE256-256f-simple |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SHPINCS+-SHAKE256-256s-robust |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SHPINCS+-SHAKE256-256s-simple |     EUF-CMA    |              5              |            64           |            128          |          29792         |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf
- **License:** CC0 1.0 Universal
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

**Note** The SHA256 variants have not yet been implemented.