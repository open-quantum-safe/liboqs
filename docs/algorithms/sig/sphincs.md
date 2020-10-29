SPHINCS+
========

- **Algorithm type**: signature
- **Main cryptographic assumption**: hash-based signatures
- **Scheme authors**: Andreas Hulsing, Daniel J. Bernstein, Christoph Dobraunig, Maria Eichlseder, Scott Fluhrer, Stefan-Lukas Gazdag, Panos Kampanakis, Stefan Kolbl, Tanja Lange, Martin M Lauridsen, Florian Mendel, Ruben Niederhagen, Christian Rechberger, Joost Rijneveld, Peter Schwabe, Jean-Philippe Aumasson
- **Authors' website**: https://sphincs.org/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/sphincs/sphincsplus
- **Implementation version**: https://github.com/PQClean/PQClean/commit/b4078aae55f9efbc7b3ab7b3c702be376f7a7987
- **License**: CC0 1.0 Universal
- **Constant-time**: Yes
- **Optimizations**: Portable C with AESNI and AVX2 instructions (if available at run-time)

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
