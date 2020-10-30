NTRU-Prime
==========

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: NTRU
- **Scheme authors**: Daniel J. Bernstein, Billy Bob Brumley, Ming-Shing Chen, Chitchanok Chuengsatiansup, Tanja Lange, Adrian Marotzke, Bo-Yuan Peng, Nicola Tuveri, Christine van Vredendaal
Bo-Yin Yang
- **Authors' website**: https://ntruprime.cr.yp.to
- **Version**: supercop-20200826

Implementation
--------------

- **Source of implementation**: SUPERCOP-20200826
- **Implementation version**: https://github.com/PQClean/PQClean/commit/b4078aae55f9efbc7b3ab7b3c702be376f7a7987
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AVX2 instructions (if available at run-time)

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|----------------|-----------------------------|-------------------------|-------------------------|-------------------------|----------------------------|
| ntrulpr653    | IND-CCA        | 2                           | 897                     | 1125                    | 1025                    | 32                         |
| ntrulpr761    | IND-CCA        | 3                           | 1039                    | 1294                    | 1167                    | 32                         |
| ntrulpr857    | IND-CCA        | 4                           | 1184                    | 1463                    | 1312                    | 32                         |
| sntrup653     | IND-CCA        | 2                           | 994                     | 1518                    | 897                     | 32                         |
| sntrup761     | IND-CCA        | 3                           | 1158                    | 1763                    | 1039                    | 32                         |
| sntrup857     | IND-CCA        | 4                           | 1322                    | 1999                    | 1184                    | 32                         |
