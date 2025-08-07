# HQC

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic).
- **Principal submitters**: Carlos Aguilar Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jurjen Bos, Jean-Christophe Deneuville, Arnaud Dion, Philippe Gaborit, Jérôme Lacan, Edoardo Persichetti, Jean-Marc Robert, Pascal Véron, Gilles Zémor.
- **Authors' website**: https://pqc-hqc.org/
- **Specification version**: 2023-04-30.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/1eacfdafc15ddc5d5759d0b85b4cef26627df181
  - **Implementation license (SPDX-Identifier)**: Public domain
- **Ancestors of primary source**:
  - https://github.com/SWilson4/package-pqclean/tree/8db1b24b/hqc, which takes it from:
  - submission 2023-04-30 at https://pqc-hqc.org/implementation.html

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
|     HQC-128     | NA                    | IND-CCA2         |                    1 |                      2249 |                      2305 |                      4433 |                           64 | NA                          | NA                                |
|     HQC-192     | NA                    | IND-CCA2         |                    3 |                      4522 |                      4586 |                      8978 |                           64 | NA                          | NA                                |
|     HQC-256     | NA                    | IND-CCA2         |                    5 |                      7245 |                      7317 |                     14421 |                           64 | NA                          | NA                                |

## HQC-128 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## HQC-192 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## HQC-256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.