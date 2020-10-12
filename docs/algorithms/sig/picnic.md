Picnic
======

- **Algorithm type**: signature
- **Main cryptographic assumption**: hash function security (ROM/QROM), key recovery attacks on the lowMC block cipher
- **Scheme authors**: Greg Zaverucha, Melissa Chase, David Derler, Steven Goldfeder, Claudio Orlandi, Sebastian Ramacher, Christian Rechberger, Daniel Slamanig, Jonathan Katz, Xiao Wang, Vladmir Kolesnikov
- **Authors' website**: https://microsoft.github.io/Picnic/
- **Version**: 3.0.3
- **Added to liboqs by**: Christian Paquin

Implementation
--------------

- **Source of implementation**: https://github.com/IAIK/Picnic
- **Implementation version**: https://github.com/IAIK/Picnic/tree/v3.0.3
- **License**: MIT License
- **Constant-time**: Yes
- **Optimizations**: Portable C with optional use of AVX2 and SSE2 instructions (selected at compile-time, enabled by default if available)

Parameter sets
--------------

| Parameter set   | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|-----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| picnic_L1_FS    |     EUF-CMA    |              1              |            33           |            49           |          34036         |
| picnic_L1_UR    |     EUF-CMA    |              1              |            33           |            49           |          53965         |
| picnic_L1_full  |     EUF-CMA    |              1              |            35           |            52           |          32065         |
| picnic_L3_FS    |     EUF-CMA    |              3              |            49           |            73           |          76776         |
| picnic_L3_UR    |     EUF-CMA    |              3              |            49           |            73           |         121849         |
| picnic_L3_full  |     EUF-CMA    |              3              |            49           |            73           |          71183         |
| picnic_L5_FS    |     EUF-CMA    |              5              |            65           |            97           |         132860         |
| picnic_L5_UR    |     EUF-CMA    |              5              |            65           |            97           |         209510         |
| picnic_L5_full  |     EUF-CMA    |              5              |            65           |            97           |         126290         |
| picnic3_L1      |     EUF-CMA    |              1              |            35           |            52           |          14612         |
| picnic3_L3      |     EUF-CMA    |              3              |            49           |            73           |          35028         |
| picnic3_L5      |     EUF-CMA    |              5              |            65           |            97           |          61028         |

Additional comments
-------------------

The original Picnic implementation includes optimizations that are not currently being built in liboqs. See src/sig/picnic/external/README.md for details.
