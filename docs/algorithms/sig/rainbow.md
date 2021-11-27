# Rainbow

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable polynomials, unbalanced oil and vinegar.
- **Principal submitters**: Jintai Ding.
- **Auxiliary submitters**: Ming-Shing Chen, Matthias Kannwischer, Jacques Patarin, Albrecht Petzoldt, Dieter Schmidt, Bo-Yin Yang.
- **Authors' website**: https://www.pqcrainbow.org/
- **Specification version**: NIST Round 3 submission.
- **Implementation source**: https://github.com/PQClean/PQClean/commit/6a32796212b79a5f9126d0a933e1216313f50c16, which takes it from:
  - https://github.com/fast-crypto-lab/rainbow-submission-round2/commit/173ada0e077e1b9dbd8e4a78994f87acc0c92263
- **Implementation license (SPDX-Identifier)**: CC0-1.0.

## Parameter set summary

|       Parameter set        | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:--------------------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|     Rainbow-I-Classic      | EUF-CMA          |                    1 |                    161600 |                    103648 |                       66 |
|  Rainbow-I-Circumzenithal  | EUF-CMA          |                    1 |                     60192 |                    103648 |                       66 |
|    Rainbow-I-Compressed    | EUF-CMA          |                    1 |                     60192 |                        64 |                       66 |
|    Rainbow-III-Classic     | EUF-CMA          |                    3 |                    882080 |                    626048 |                      164 |
| Rainbow-III-Circumzenithal | EUF-CMA          |                    3 |                    264608 |                    626048 |                      164 |
|   Rainbow-III-Compressed   | EUF-CMA          |                    3 |                    264608 |                        64 |                      164 |
|     Rainbow-V-Classic      | EUF-CMA          |                    5 |                   1930600 |                   1408736 |                      212 |
|  Rainbow-V-Circumzenithal  | EUF-CMA          |                    5 |                    536136 |                   1408736 |                      212 |
|    Rainbow-V-Compressed    | EUF-CMA          |                    5 |                    536136 |                        64 |                      212 |

## Rainbow-I-Classic implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                 |

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Rainbow-I-Circumzenithal implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |

## Rainbow-I-Compressed implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |

## Rainbow-III-Classic implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-III-Circumzenithal implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-III-Compressed implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-V-Classic implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-V-Circumzenithal implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-V-Compressed implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | True                 |

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.