liboqs algorithm datasheet: `sig_sphincs`
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
| SPHINCS+-Haraka-128f-robust   |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SPHINCS+-Haraka-128f-simple   |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SPHINCS+-Haraka-128s-robust   |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SPHINCS+-Haraka-128s-simple   |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SPHINCS+-Haraka-192f-robust   |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SPHINCS+-Haraka-192f-simple   |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SPHINCS+-Haraka-192s-robust   |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SPHINCS+-Haraka-192s-simple   |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SPHINCS+-Haraka-256f-robust   |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SPHINCS+-Haraka-256f-simple   |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SPHINCS+-Haraka-256s-robust   |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SPHINCS+-Haraka-256s-simple   |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SPHINCS+-SHA256-128f-robust   |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SPHINCS+-SHA256-128f-simple   |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SPHINCS+-SHA256-128s-robust   |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SPHINCS+-SHA256-128s-simple   |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SPHINCS+-SHA256-192f-robust   |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SPHINCS+-SHA256-192f-simple   |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SPHINCS+-SHA256-192s-robust   |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SPHINCS+-SHA256-192s-simple   |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SPHINCS+-SHA256-256f-robust   |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SPHINCS+-SHA256-256f-simple   |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SPHINCS+-SHA256-256s-robust   |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SPHINCS+-SHA256-256s-simple   |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SPHINCS+-SHAKE256-128f-robust |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SPHINCS+-SHAKE256-128f-simple |     EUF-CMA    |              1              |            32           |            64           |          16976         |
| SPHINCS+-SHAKE256-128s-robust |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SPHINCS+-SHAKE256-128s-simple |     EUF-CMA    |              1              |            32           |            64           |          8080          |
| SPHINCS+-SHAKE256-192f-robust |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SPHINCS+-SHAKE256-192f-simple |     EUF-CMA    |              3              |            48           |            96           |          35664         |
| SPHINCS+-SHAKE256-192s-robust |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SPHINCS+-SHAKE256-192s-simple |     EUF-CMA    |              3              |            48           |            96           |          17064         |
| SPHINCS+-SHAKE256-256f-robust |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SPHINCS+-SHAKE256-256f-simple |     EUF-CMA    |              5              |            64           |            128          |          49216         |
| SPHINCS+-SHAKE256-256s-robust |     EUF-CMA    |              5              |            64           |            128          |          29792         |
| SPHINCS+-SHAKE256-256s-simple |     EUF-CMA    |              5              |            64           |            128          |          29792         |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/298dd2cf223cff50cc53bea534474796f9960653
- **License:** CC0 1.0 Universal
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
