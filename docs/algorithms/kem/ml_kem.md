# ML-KEM

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Module LWE+R with base ring Z[x]/(3329, x^256+1).
- **Principal submitters**: Peter Schwabe.
- **Auxiliary submitters**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/kyber/ and https://csrc.nist.gov/pubs/fips/203
- **Specification version**: ML-KEM.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-code-package/mlkem-native/commit/048fc2a7a7b4ba0ad4c989c1ac82491aa94d5bfa
  - **Implementation license (SPDX-Identifier)**: MIT or Apache-2.0 or ISC
- **Optimized Implementation sources**:
  - **x86_64**:<a name="x86_64"></a>
      - **Source**: https://github.com/pq-code-package/mlkem-native/commit/048fc2a7a7b4ba0ad4c989c1ac82491aa94d5bfa
      - **Implementation license (SPDX-Identifier)**: MIT or Apache-2.0 or ISC
  - **aarch64**:<a name="aarch64"></a>
      - **Source**: https://github.com/pq-code-package/mlkem-native/commit/048fc2a7a7b4ba0ad4c989c1ac82491aa94d5bfa
      - **Implementation license (SPDX-Identifier)**: MIT or Apache-2.0 or ISC
  - **cupqc-cuda**:<a name="cupqc-cuda"></a>
      - **Source**: https://github.com/open-quantum-safe/liboqs-cupqc-meta/commit/b026f4e5475cd9c20c2082c7d9bad80e5b0ba89e
      - **Implementation license (SPDX-Identifier)**: Apache-2.0
  - **icicle-icicle_cuda**:<a name="icicle-icicle_cuda"></a>
      - **Source**: https://github.com/ingonyama-zk/icicle-liboqs/commit/4ea3e612ff26e3e72b5e5bcfff4cf3dda45dc0a8
      - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |   Keypair seed size (bytes) |   Encapsulation seed size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|----------------------------:|----------------------------------:|
|   ML-KEM-512    | NA                    | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |                          64 |                                32 |
|   ML-KEM-768    | NA                    | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |                          64 |                                32 |
|   ML-KEM-1024   | NA                    | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |                          64 |                                32 |

## ML-KEM-512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                 |
|     [cupqc-cuda](#cupqc-cuda)     | cuda                     | CUDA                        | Linux,Darwin                    | None                    | False                              | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## ML-KEM-768 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
|     [cupqc-cuda](#cupqc-cuda)     | cuda                     | CUDA                        | Linux,Darwin                    | None                    | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## ML-KEM-1024 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
|     [cupqc-cuda](#cupqc-cuda)     | cuda                     | CUDA                        | Linux,Darwin                    | None                    | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.