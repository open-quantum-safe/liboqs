# HQC

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic).
- **Principal submitters**: Philippe Gaborit.
- **Auxiliary submitters**: Carlos Aguilar-Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Edoardo Persichetti, Gilles Zémor, Jurjen Bos, Arnaud Dion, Jérôme Lacan, Jean-Marc Robert, Pascal Véron, Paulo L. Barreto, Santosh Ghosh, Shay Gueron, Tim Güneysu, Rafael Misoczki, Jan Richter-Brokmann, Nicolas Sendrier, Jean-Pierre Tillich, Valentin Vasseur.
- **Authors' website**: https://pqc-hqc.org/
- **Specification version**: 2026-10-12.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://gitlab.com/pqc-hqc/hqc/commit/06567ccbad10580d78dd7863b15d23f41076d57f with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: Public domain
- **Optimized Implementation sources**:
  - **x86_64**:<a name="x86_64"></a>
      - **Source**: https://gitlab.com/pqc-hqc/hqc/commit/06567ccbad10580d78dd7863b15d23f41076d57f with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Public domain
- **Ancestors of primary source**:
  - https://gitlab.com/pqc-hqc

## Support

- **Upstream maintenance**: TBD
- **OQS support tier**: Tier 2

## Advisories

- HQC's AVX2 backend is not thread-safe. `src/kem/hqc/pqc-hqc_hqc_avx256_common/hqc.c` and `src/kem/hqc/pqc-hqc_hqc-<1,3,5>_x86_64/gf2x.c` use static buffers that might get corrupted when multiple instances of HQC AVX2 backends run concurrently. These `static` declarations were inherited from upstream and were likely put there to reduce stack usage. Set `OQS_ENABLE_KEM_hqc_{1,3,5}_x86_64=OFF` at CMake configure time to turn off AVX256 backends.

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
|      HQC-1      | NA                    | IND-CCA2         |                    1 |                      2241 |                        32 |                      4433 |                           32 | NA                          | NA                                |
|      HQC-3      | NA                    | IND-CCA2         |                    3 |                      4514 |                        32 |                      8978 |                           32 | NA                          | NA                                |
|      HQC-5      | NA                    | IND-CCA2         |                    5 |                      7237 |                        32 |                     14421 |                           32 | NA                          | NA                                |

## HQC-1 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                 |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX,AVX2,BMI1,PCLMULQDQ | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## HQC-3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX,AVX2,BMI1,PCLMULQDQ | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## HQC-5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX,AVX2,BMI1,PCLMULQDQ | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.