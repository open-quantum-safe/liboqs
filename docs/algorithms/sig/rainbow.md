# Rainbow

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable polynomials, unbalanced oil and vinegar.
- **Principal submitters**: Jintai Ding.
- **Auxiliary submitters**: Ming-Shing Chen, Matthias Kannwischer, Jacques Patarin, Albrecht Petzoldt, Dieter Schmidt, Bo-Yin Yang.
- **Authors' website**: https://www.pqcrainbow.org/
- **Specification version**: NIST Round 3 submission.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6
  - **Implementation license (SPDX-Identifier)**: CC0-1.0


## Usage advice/warning

[A practical attack against this algorithm has been published and confirmed](https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/KFgw5_qCXiI?pli=1). Thus, caution is advised regarding the use of it. Next steps are tracked under [liboqs issue #1192](https://github.com/open-quantum-safe/liboqs/issues/1192).

## Test limitation

This algorithm is not tested under Windows.

## Parameter set summary

|       Parameter set        | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:--------------------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|    Rainbow-III-Classic     | EUF-CMA          |                    3 |                    882080 |                    626048 |                      164 |
| Rainbow-III-Circumzenithal | EUF-CMA          |                    3 |                    264608 |                    626048 |                      164 |
|   Rainbow-III-Compressed   | EUF-CMA          |                    3 |                    264608 |                        64 |                      164 |
|     Rainbow-V-Classic      | EUF-CMA          |                    5 |                   1930600 |                   1408736 |                      212 |
|  Rainbow-V-Circumzenithal  | EUF-CMA          |                    5 |                    536136 |                   1408736 |                      212 |
|    Rainbow-V-Compressed    | EUF-CMA          |                    5 |                    536136 |                        64 |                      212 |

## Rainbow-III-Classic implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                  |

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Rainbow-III-Circumzenithal implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-III-Compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-V-Classic implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-V-Circumzenithal implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |

## Rainbow-V-Compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.