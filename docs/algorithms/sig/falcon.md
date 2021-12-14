# Falcon

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of NTRU lattice problems.
- **Principal submitters**: Thomas Prest.
- **Auxiliary submitters**: Pierre-Alain Fouque, Jeffrey Hoffstein, Paul Kirchner, Vadim Lyubashevsky, Thomas Pornin, Thomas Ricosset, Gregor Seiler, William Whyte, Zhenfei Zhang.
- **Authors' website**: https://falcon-sign.info
- **Specification version**: v1.2.
- **Implementation source**: https://github.com/PQClean/PQClean/commit/6a32796212b79a5f9126d0a933e1216313f50c16, which takes it from:
  - https://github.com/jschanck/package-pqclean/tree/cea1fa5a/falcon, which takes it from:
  - supercop-20201018
- **Implementation license (SPDX-Identifier)**: MIT.

## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|   Falcon-512    | EUF-CMA          |                    1 |                       897 |                      1281 |                      690 |
|   Falcon-1024   | EUF-CMA          |                    5 |                      1793 |                      2305 |                     1330 |

## Falcon-512 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                 |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Falcon-1024 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.
