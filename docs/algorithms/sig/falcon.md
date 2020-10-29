Falcon
======

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of NTRU lattice problems
- **Scheme authors**: Thomas Prest, Pierre-Alain Fouque, Jeffrey Hoffstein, Paul Kirchner, Vadim Lyubashevsky, Thomas Pornin, Thomas Ricosset, Gregor Seiler, William Whyte, Zhenfei Zhang
- **Authors' website**: https://falcon-sign.info
- **Version**: 20190920

Implementation
--------------

- **Source of implementation**: https://falcon-sign.info/impl/falcon.h.html
- **Implementation version**: https://github.com/PQClean/PQClean/commit/b4078aae55f9efbc7b3ab7b3c702be376f7a7987
- **License**: CC0 1.0 Universal
- **Constant-time**: Yes
- **Optimizations**: Portable C

Parameter sets
--------------

  Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Falcon-512    |     EUF-CMA    |             1               |             897         |            1281         |            690         |
| Falcon-1024   |     EUF-CMA    |             5               |            1793         |            2305         |           1330         |

