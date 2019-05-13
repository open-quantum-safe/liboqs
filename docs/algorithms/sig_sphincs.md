liboqs nist-branch algorithm datasheet: `sig_sphincs`
===================================================

Summary
-------

- **Name**: SPHINCS+
- **Algorithm type**: signature
- **Main cryptographic assumption**: Hash-based signatures
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/SPHINCS-Round2.zip
- **Submitters (to NIST competition)**: Andreas Hulsing, Daniel J. Bernstein, Christoph Dobraunig, Maria Eichlseder, Scott Fluhrer, Stefan-Lukas Gazdag, Panos Kampanakis, Stefan Kolbl, Tanja Lange, Martin M Lauridsen, Florian Mendel, Ruben Niederhagen, Christian Rechberger, Joost Rijneveld, Peter Schwabe, Jean-Philippe Aumasson
- **Submitters' website**: https://sphincs.org/
- **Added to liboqs by**: Thom Wiggers

Parameter sets
--------------

| Parameter set             | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| SPHINCS+-128s             |    EUF-CMA     |              1              |          32             |          64             |          8080          |
| SPHINCS+-128f             |    EUF-CMA     |              1              |          32             |          64             |         16976          |
| SPHINCS+-192s             |    EUF-CMA     |              3              |          48             |          96             |         17064          |
| SPHINCS+-192f             |    EUF-CMA     |              3              |          48             |          96             |         35664          |
| SPHINCS+-256s             |    EUF-CMA     |              5              |          64             |         128             |         29792          |
| SPHINCS+-256f             |    EUF-CMA     |              5              |          64             |         128             |         49216          |

These schemes are then all instantiated with the hash functions `SHA256`, `SHAKE256`, and `Haraka` in a `Simple` and `Robust` version.
In total there are 36 instantiations of SPHINCS+.


Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf
- **License:** CC0 1.0 Universal
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
