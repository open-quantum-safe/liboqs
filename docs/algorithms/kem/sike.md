SIKE
====

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: (supersingular) isogeny walk problem
- **Scheme authors**: David Jao, Reza Azarderakhsh, Matthew Campagna, Craig Costello, Luca De Feo, Basil Hess, Amir Jalali, Brian Koziel, Brian LaMacchia, Patrick Longa, Michael Naehrig, Joost Renes, Vladimir Soukharev, David Urbanik
- **Authors' website**: http://sike.org/
- **Version**: 3.2
- **Added to liboqs by**: Christian Paquin

Implementation
--------------

- **Source of implementation**: https://github.com/Microsoft/PQCrypto-SIDH
- **Implementation version**: v3.2 (https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311)
- **License**: MIT License
- **Constant-time**: Yes
- **Optimizations**: Portable C, with assembly optimizations on AMD64 and selected parameter sets on ARM64

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| SIDH-p434            |     IND-CPA    |              1              |           330           |            28           |           330           |             110            |
| SIDH-p434-compressed |     IND-CPA    |              1              |           196           |            28           |           196           |             110            |
| SIDH-p503            |     IND-CPA    |              2              |           378           |            32           |           378           |             126            |
| SIDH-p503-compressed |     IND-CPA    |              2              |           224           |            32           |           224           |             126            |
| SIDH-p610            |     IND-CPA    |              3              |           462           |            39           |           462           |             154            |
| SIDH-p610-compressed |     IND-CPA    |              3              |           273           |            39           |           273           |             154            |
| SIDH-p751            |     IND-CPA    |              5              |           564           |            48           |           564           |             188            |
| SIDH-p751-compressed |     IND-CPA    |              5              |           334           |            48           |           334           |             188            |
| SIKE-p434            |     IND-CCA    |              1              |           330           |           374           |           346           |             16             |
| SIKE-p434-compressed |     IND-CCA    |              1              |           196           |           239           |           209           |             16             |
| SIKE-p503            |     IND-CCA    |              2              |           378           |           434           |           402           |             24             |
| SIKE-p503-compressed |     IND-CCA    |              2              |           224           |           280           |           248           |             24             |
| SIKE-p610            |     IND-CCA    |              3              |           462           |           524           |           486           |             24             |
| SIKE-p610-compressed |     IND-CCA    |              3              |           273           |           336           |           297           |             24             |
| SIKE-p751            |     IND-CCA    |              3              |           564           |           644           |           596           |             32             |
| SIKE-p751-compressed |     IND-CCA    |              3              |           334           |           413           |           363           |             32             |

Additional comments
-------------------

No KAT are available for SIDH.
