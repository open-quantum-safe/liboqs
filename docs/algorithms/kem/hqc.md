# HQC

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic)
- **Scheme authors**: Carlos Aguilar Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jurjen Bos, Jean-Christophe Deneuville, Arnaud Dion, Philippe Gaborit, Jérôme Lacan, Edoardo Persichetti, Jean-Marc Robert, Rascal Véron, Gilles Zémor
- **Authors' website**: http://pqc-hqc.org
- **Version**: 2020/10/01

## Implementation

- **Source of implementation**: hqc-submission_2020-10-01 via https://github.com/jschanck/package-pqclean/tree/c9181076/hqc
- **Implementation version**: https://github.com/PQClean/PQClean.git, master, ebcc71c51a30b6e5db4f1fade22999b346fdafce
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AVX2, BMI1, PCLMULQDQ instructions (if available at run-time)

## Parameter sets

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
| ------------- | -------------- | --------------------------- | ----------------------- | ----------------------- | ----------------------- | -------------------------- |
| HQC-128       | IND-CCA2       | 1                           | 2249                    | 2289                    | 4481                    | 64                         |
| HQC-192       | IND-CCA2       | 3                           | 4522                    | 4562                    | 9026                    | 64                         |
| HQC-256       | IND-CCA2       | 5                           | 7245                    | 7285                    | 14469                   | 64                         |
