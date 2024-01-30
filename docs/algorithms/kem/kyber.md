# Kyber

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Module LWE+R with base ring Z[x]/(3329, x^256+1).
- **Principal submitters**: Peter Schwabe.
- **Auxiliary submitters**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/
- **Specification version**: NIST Round 3 submission.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-crystals/kyber/commit/b628ba78711bc28327dc7d2d5c074a00f061884e with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0 or Apache-2.0
- **Optimized Implementation sources**: https://github.com/pq-crystals/kyber/commit/b628ba78711bc28327dc7d2d5c074a00f061884e with copy_from_upstream patches
  - **oldpqclean-aarch64**:<a name="oldpqclean-aarch64"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/8e220a87308154d48fdfac40abbb191ac7fce06a with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: CC0-1.0 and (CC0-1.0 or Apache-2.0) and (CC0-1.0 or MIT) and MIT


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
|    Kyber512     | NA                    | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |
|    Kyber768     | NA                    | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |
|    Kyber1024    | NA                    | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |

## Kyber512 implementation characteristics

|           Implementation source           | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:-----------------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|     [Primary Source](#primary-source)     | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
|     [Primary Source](#primary-source)     | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                 |
| [oldpqclean-aarch64](#oldpqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Kyber768 implementation characteristics

|           Implementation source           | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:-----------------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|     [Primary Source](#primary-source)     | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
|     [Primary Source](#primary-source)     | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [oldpqclean-aarch64](#oldpqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber1024 implementation characteristics

|           Implementation source           | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:-----------------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|     [Primary Source](#primary-source)     | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
|     [Primary Source](#primary-source)     | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [oldpqclean-aarch64](#oldpqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.