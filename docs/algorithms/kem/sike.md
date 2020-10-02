SIKE
====

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: (supersingular) isogeny walk problem
- **Scheme authors**: David Jao, Reza Azarderakhsh, Matthew Campagna, Craig Costello, Luca De Feo, Basil Hess, Amir Jalali, Brian Koziel, Brian LaMacchia, Patrick Longa, Michael Naehrig, Joost Renes, Vladimir Soukharev, David Urbanik
- **Authors' website**: http://sike.org/
- **Version**: 3.3
- **Added to liboqs by**: Christian Paquin

Implementation
--------------

- **Source of implementation**: https://github.com/Microsoft/PQCrypto-SIDH
- **Implementation version**: v3.3 + fixes (https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44
- **License**: MIT License
- **Constant-time**: Yes
- **Optimizations**: Portable C, with assembly optimizations on AMD64 and selected parameter sets on ARM64 (selected at compile-time, enabled by default if available)

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| SIDH-p434            |     IND-CPA    |              1              |           330           |            28           |           330           |             110            |
| SIDH-p434-compressed |     IND-CPA    |              1              |           197           |            28           |           197           |             110            |
| SIDH-p503            |     IND-CPA    |              2              |           378           |            32           |           378           |             126            |
| SIDH-p503-compressed |     IND-CPA    |              2              |           225           |            32           |           225           |             126            |
| SIDH-p610            |     IND-CPA    |              3              |           462           |            39           |           462           |             154            |
| SIDH-p610-compressed |     IND-CPA    |              3              |           274           |            39           |           274           |             154            |
| SIDH-p751            |     IND-CPA    |              5              |           564           |            48           |           564           |             188            |
| SIDH-p751-compressed |     IND-CPA    |              5              |           335           |            48           |           335           |             188            |
| SIKE-p434            |     IND-CCA    |              1              |           330           |           374           |           346           |             16             |
| SIKE-p434-compressed |     IND-CCA    |              1              |           197           |           350           |           236           |             16             |
| SIKE-p503            |     IND-CCA    |              2              |           378           |           434           |           402           |             24             |
| SIKE-p503-compressed |     IND-CCA    |              2              |           225           |           407           |           280           |             24             |
| SIKE-p610            |     IND-CCA    |              3              |           462           |           524           |           486           |             24             |
| SIKE-p610-compressed |     IND-CCA    |              3              |           274           |           491           |           336           |             24             |
| SIKE-p751            |     IND-CCA    |              5              |           564           |           644           |           596           |             32             |
| SIKE-p751-compressed |     IND-CCA    |              5              |           335           |           602           |           410           |             32             |

Additional comments
-------------------

No KAT are available for SIDH.
