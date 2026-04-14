# HQC

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic).
- **Principal submitters**: Philippe Gaborit.
- **Auxiliary submitters**: Carlos Aguilar-Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Edoardo Persichetti, Gilles Zémor, Jurjen Bos, Arnaud Dion, Jérôme Lacan, Jean-Marc Robert, Pascal Véron, Paulo L. Barreto, Santosh Ghosh, Shay Gueron, Tim Güneysu, Rafael Misoczki, Jan Richter-Brokmann, Nicolas Sendrier, Jean-Pierre Tillich, Valentin Vasseur.
- **Authors' website**: https://pqc-hqc.org/
- **Specification version**: 2025-08-22.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/xuganyu96/pqc-hqc/commit/37555f24745e93554a10875c66fa5dd30594eefc
  - **Implementation license (SPDX-Identifier)**: Public domain
- **Ancestors of primary source**:
  - https://gitlab.com/pqc-hqc, which takes it from:
  - submission 2025-08-22 at https://pqc-hqc.org/implementation.html

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
|      HQC-1      | NA                    | IND-CCA2         |                    1 |                      2241 |                      2321 |                      4433 |                           32 | NA                          | NA                                |
|      HQC-3      | NA                    | IND-CCA2         |                    3 |                      4514 |                      4602 |                      8978 |                           32 | NA                          | NA                                |
|      HQC-5      | NA                    | IND-CCA2         |                    5 |                      7237 |                      7333 |                     14421 |                           32 | NA                          | NA                                |

## HQC-1 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## HQC-3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## HQC-5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.