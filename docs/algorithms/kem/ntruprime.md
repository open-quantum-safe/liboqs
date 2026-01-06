# NTRU-Prime

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: NTRU.
- **Principal submitters**: Daniel J. Bernstein, Billy Bob Brumley, Ming-Shing Chen, Chitchanok Chuengsatiansup, Tanja Lange, Adrian Marotzke, Bo-Yuan Peng, Nicola Tuveri, Christine van Vredendaal, Bo-Yin Yang.
- **Authors' website**: https://ntruprime.cr.yp.to
- **Specification version**: supercop-20200826.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6
  - **Implementation license (SPDX-Identifier)**: Public domain
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6
      - **Implementation license (SPDX-Identifier)**: Public domain
- **Ancestors of primary source**:
  - https://github.com/jschanck/package-pqclean/tree/4d9f08c3/ntruprime, which takes it from:
  - supercop-20210604

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
|    sntrup761    | NA                    | IND-CCA2         |                    2 |                      1158 |                      1763 |                      1039 |                           32 | NA                          | NA                                |

## sntrup761 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.