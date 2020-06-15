BIKE
====

- **Algorithm type**: Key Encapsulation Mechanism
- **Main cryptographic assumption**: quasi-cyclic syndrome decoding (QCSD)
- **Scheme authors**: Nicolas Aragon, Paulo Barreto, Slim Bettaieb, Loic Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Phillipe Gaborit, Shay Gueron, Tim Guneysu, Carlos Aguilar Melchor, Rafael Misoczki, Edoardo Persichetti, Nicolas Sendrier, Jean-Pierre Tillich, Gilles Zemor
- **Authors' website**: http://bikesuite.org/
- **Version**: 3.2
- **Added to liboqs by**: Shay Gueron and Nir Drucker.

Implementation
--------------

- **Source of implementation**: https://bikesuite.org/additional.html
- **Implementation version**: BIKE-1 L1/3 with the BGF decoder (as defined in "QC-MDPC decoders with several shades of gray" at https://eprint.iacr.org/2019/1423) 
  - BIKE-1-FO L1/3 that matches [BIKE's v3.2](https://bikesuite.org/files/round2/spec/BIKE-Spec-2020.02.07.1.pdf)
  - BIKE-1-CPA L1/3 that matches BIKE Round-1 (and BIKE v3.0) for backward compatibility
- **License**: Apache 2.0 License
- **Constant-time**: Yes
- **Optimizations**: Portable C with optional use (selected at compile-time, enabled by default if available) of AVX2 instructions

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BIKE1-L1-CPA        |     IND-CPA    |              1              |           2542          |          3110          |           2542          |             32             |
| BIKE1-L3-CPA        |     IND-CPA    |              3              |           4964          |          5788          |           4964          |             32             |
| BIKE1-L1-FO         |     IND-CCA    |              1              |           2946          |          6460          |           2946          |             32             |
| BIKE1-L3-FO         |     IND-CCA    |              3              |           6206          |         13236          |           6206          |             32             |
