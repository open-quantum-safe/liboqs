HQC
===

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic)
- **Scheme authors**: Carlos Aguilar Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jurjen Bos, Jean-Christophe Deneuville, Philippe Gaborit, Edoardo Persichetti, Jean-Marc Robert, Pascal Véron, Gilles Zémor
- **Authors' website**: http://pqc-hqc.org
- **Version**: 2020/05/29

Implementation
--------------

- **Source of implementation**: http://pqc-hqc.org/doc/hqc-submission_2020-05-29.zip
- **Implementation version**: https://github.com/PQClean/PQClean/commit/383874f4082ab26a234e0e3428ee0f419e008765
- **License**: Public domain
- **Constant-time**: No
- **Optimizations**: Portable C, AVX2

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|----------------|-----------------------------|-------------------------|-------------------------|-------------------------|----------------------------|
| HQC-128       | IND-CCA        | 1                           | 3024                    | 3064                    | 6017                    | 64                         |
| HQC-192       | IND-CCA        | 3                           | 5690                    | 5730                    | 11364                   | 64                         |
| HQC-256       | IND-CCA        | 5                           | 8698                    | 8738                    | 17379                   | 64                         |
| HQC-RMRS-128  | IND-CCA        | 1                           | 2607                    | 2647                    | 5191                    | 64                         |
| HQC-RMRS-192  | IND-CCA        | 3                           | 4906                    | 4946                    | 9794                    | 64                         |
| HQC-RMRS-256  | IND-CCA        | 5                           | 7535                    | 7575                    | 15047                   | 64                         |
