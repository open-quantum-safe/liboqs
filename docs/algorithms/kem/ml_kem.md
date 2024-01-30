# ML-KEM

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Module LWE+R with base ring Z[x]/(3329, x^256+1).
- **Principal submitters**: Peter Schwabe.
- **Auxiliary submitters**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/kyber/ and https://csrc.nist.gov/pubs/fips/203/ipd
- **Specification version**: ML-KEM-ipd.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-crystals/kyber/commit/11d00ff1f20cfca1f72d819e5a45165c1e0a2816 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0 or Apache-2.0


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
| ML-KEM-512-ipd  | ML-KEM-512            | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |
| ML-KEM-768-ipd  | ML-KEM-768            | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |
| ML-KEM-1024-ipd | ML-KEM-1024           | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |

## ML-KEM-512-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## ML-KEM-768-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## ML-KEM-1024-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.