HQC
===

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic)
- **Scheme authors**: Carlos Aguilar Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Philippe Gaborit, Edoardo Persichetti, Gilles Zémor
- **Authors' website**: http://pqc-hqc.org
- **Version**: 2019/08/24

Implementation
--------------

- **Source of implementation**: http://pqc-hqc.org/doc/hqc-submission_2019-08-24.zip
- **Implementation version**: https://github.com/PQClean/PQClean/commit/90630db2ebada4bacceb5331b0a1a9a356ba65b9
- **License**: Public domain
- **Constant-time**: No
- **Optimizations**: Portable C

Parameter sets
--------------

| Parameter set     | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-------------------|----------------|-----------------------------|-------------------------|-------------------------|-------------------------|----------------------------|
| HQC-128-1-CCA2    | IND-CCA        | 1                           | 3125                    | 3165                    | 6234                    | 64                         |
