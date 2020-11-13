Falcon
======

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of NTRU lattice problems
- **Scheme authors**: Thomas Prest, Pierre-Alain Fouque, Jeffrey Hoffstein, Paul Kirchner, Vadim Lyubashevsky, Thomas Pornin, Thomas Ricosset, Gregor Seiler, William Whyte, Zhenfei Zhang
- **Authors' website**: https://falcon-sign.info
- **Version**: 20201018

Implementation
--------------

- **Source of implementation**: supercop-20201018 via https://github.com/jschanck/package-pqclean/tree/78831f03/falcon
- **Implementation version**: https://github.com/PQClean/PQClean/commit/3c8be3cb1f8ca0953f2df44ea665f118efb468d6
- **License**: CC0 1.0 Universal
- **Constant-time**: Yes
- **Optimizations**: Portable C, with AVX2 instructions (if available at runtime)

Parameter sets
--------------

  Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Falcon-512    |     EUF-CMA    |             1               |             897         |            1281         |            690         |
| Falcon-1024   |     EUF-CMA    |             5               |            1793         |            2305         |           1330         |

