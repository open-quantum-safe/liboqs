BIKE
====

- **Algorithm type**: Key Encapsulation Mechanism
- **Main cryptographic assumption**: quasi-cyclic syndrome decoding (QCSD)
- **Scheme authors**: Nicolas Aragon, Paulo Barreto, Slim Bettaieb, Loic Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Phillipe Gaborit, Shay Gueron, Tim Guneysu, Carlos Aguilar Melchor, Rafael Misoczki, Edoardo Persichetti, Nicolas Sendrier, Jean-Pierre Tillich, Gilles Zemor
- **Authors' website**: http://bikesuite.org/
- **Version**: 3.2
- **Added to liboqs by**: Shay Gueron, Nir Drucker, Dusan Kostic.

Implementation
--------------

- **Source of implementation**: https://bikesuite.org/additional.html
- **Implementation version**: BIKE Round-3 code from the "Additional implementation" available at https://github.com/awslabs/bike-kem.
- **License**: Apache 2.0 License
- **Constant-time**: Yes
- **Optimizations**: Portable C with optional use (selected at compile-time, enabled by default if available) of AVX2 instructions

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BIKE-L1             |     IND-CPA    |              1              |           1541          |          5223          |           1573          |             32             |
| BIKE-L3             |     IND-CPA    |              3              |           3083          |         10105          |           3115          |             32             |
