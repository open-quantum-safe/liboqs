liboqs algorithm datasheet: `kem_bike`
======================================

Summary
-------

- **Name**: BIKE
- **Algorithm type**: Key Encapsulation Mechanism
- **Main cryptographic assumption**: Quasi-cyclic syndrome decoding (QCSD)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/BIKE-Round2.zip
- **Submitters (to NIST competition)**: Nicolas Aragon, Paulo Barreto, Slim Bettaieb, Loic Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Phillipe Gaborit, Shay Gueron, Tim Guneysu, Carlos Aguilar Melchor, Rafael Misoczki, Edoardo Persichetti, Nicolas Sendrier, Jean-Pierre Tillich, Gilles Zemor
- **Submitters' website**: http://bikesuite.org/
- **Added to liboqs by**: Shay Gueron and Nir Drucker.

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BIKE1-L1-CPA        |     IND-CPA    |              1              |           2542          |          3110          |           2542          |             32             |
| BIKE1-L3-CPA        |     IND-CPA    |              3              |           4964          |          5788          |           4964          |             32             |
| BIKE1-L1-FO         |     IND-CPA    |              1              |           2946          |          6460          |           2946          |             32             |
| BIKE1-L3-FO         |     IND-CPA    |              3              |           6206          |         13236          |           6206          |             32             |


Implementation
--------------

- **Source of implementation:** https://bikesuite.org/additional.html
- **Implementation version:** 07/06/2019
- **License:** Apache 2.0 License
- **Language:** C
- **Constant-time:** Yes
