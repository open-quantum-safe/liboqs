# NTRU

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: NTRU in Z[x]/(q, x^n-1) with prime n and power-of-two q.
- **Principal submitters**: John M. Schanck.
- **Auxiliary submitters**: Cong Chen, Oussama Danba, Jeffrey Hoffstein, Andreas Hülsing, Joost Rijneveld, Tsunekazu Saito, Peter Schwabe, William Whyte, Keita Xagawa, Takashi Yamakawa, Zhenfei Zhang.
- **Authors' website**: https://ntru.org/
- **Specification version**: NIST Round 3 submission.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6
  - **Implementation license (SPDX-Identifier)**: CC0-1.0
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6
      - **Implementation license (SPDX-Identifier)**: CC0-1.0
- **Ancestors of primary source**:
  - https://github.com/jschanck/ntru/tree/a43a4457

## Parameter set summary

|   Parameter set    | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
| NTRU-HPS-2048-509  | NA                    | IND-CCA2         |                    1 |                       699 |                       935 |                       699 |                           32 | NA                          | NA                                |
| NTRU-HPS-2048-677  | NA                    | IND-CCA2         |                    3 |                       930 |                      1234 |                       930 |                           32 | NA                          | NA                                |
| NTRU-HPS-4096-821  | NA                    | IND-CCA2         |                    5 |                      1230 |                      1590 |                      1230 |                           32 | NA                          | NA                                |
| NTRU-HPS-4096-1229 | NA                    | IND-CCA2         |                    5 |                      1842 |                      2366 |                      1842 |                           32 | NA                          | NA                                |
|   NTRU-HRSS-701    | NA                    | IND-CCA2         |                    3 |                      1138 |                      1450 |                      1138 |                           32 | NA                          | NA                                |
|   NTRU-HRSS-1373   | NA                    | IND-CCA2         |                    5 |                      2401 |                      2983 |                      2401 |                           32 | NA                          | NA                                |

## NTRU-HPS-2048-509 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2               | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## NTRU-HPS-2048-677 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2               | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## NTRU-HPS-4096-821 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2               | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## NTRU-HPS-4096-1229 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## NTRU-HRSS-701 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2               | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## NTRU-HRSS-1373 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.