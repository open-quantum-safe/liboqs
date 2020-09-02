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
- **Implementation version**: https://github.com/PQClean/PQClean/commit/c50bc64a40ce83639cff54419fce60483a30c430
- **License**: Public domain
- **Constant-time**: No
- **Optimizations**: Portable C

Parameter sets
--------------

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------|----------------|-----------------------------|-------------------------|-------------------------|-------------------------|----------------------------|
| HQC-128-1-CCA2 | IND-CCA        | 1                           | 3125                    | 3165                    | 6234                    | 64                         |
| HQC-192-1-CCA2 | IND-CCA        | 3                           | 5499                    | 5539                    | 10981                   | 64                         |
| HQC-192-2-CCA2 | IND-CCA        | 3                           | 5884                    | 5924                    | 11749                   | 64                         |
| HQC-256-1-CCA2 | IND-CCA        | 5                           | 7989                    | 8029                    | 15961                   | 64                         |
| HQC-256-2-CCA2 | IND-CCA        | 5                           | 8503                    | 8543                    | 16985                   | 64                         |
| HQC-256-3-CCA2 | IND-CCA        | 5                           | 8897                    | 8937                    | 17777                   | 64                         |