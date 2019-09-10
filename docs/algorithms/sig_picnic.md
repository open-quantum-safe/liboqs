liboqs algorithm datasheet: `sig_picnic`
========================================

Summary
-------

- **Name**: Picnic
- **Algorithm type**: signature
- **Main cryptographic assumption**: hash function security (ROM/QROM), key recovery attacks on the lowMC block cipher
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/Picnic-Round2.zip
- **Submitters (to NIST competition)**: Greg Zaverucha, Melissa Chase, David Derler, Steven Goldfeder, Claudio Orlandi, Sebastian Ramacher, Christian Rechberger, Daniel Slamanig, Jonathan Katz, Xiao Wang, Vladmir Kolesnikov
- **Submitters' website**: https://microsoft.github.io/Picnic/
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set   | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|-----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| picnic_L1_FS    |     EUF-CMA    |              1              |            33           |            49           |          34036         |
| picnic_L1_UR    |     EUF-CMA    |              1              |            33           |            49           |          53965         |
| picnic_L3_FS    |     EUF-CMA    |              3              |            49           |            73           |          76776         |
| picnic_L3_UR    |     EUF-CMA    |              3              |            49           |            73           |         121849         |
| picnic_L5_FS    |     EUF-CMA    |              5              |            65           |            97           |         132860         |
| picnic_L5_UR    |     EUF-CMA    |              5              |            65           |            97           |         209510         |
| picnic2_L1_FS   |     EUF-CMA    |              1              |            33           |            49           |          13806         |
| picnic2_L3_FS   |     EUF-CMA    |              3              |            49           |            73           |          29754         |
| picnic2_L5_FS   |     EUF-CMA    |              5              |            65           |            97           |          54736         |

Implementation
--------------

- **Source of implementation:** https://github.com/IAIK/Picnic
- **Implementation version:** https://github.com/IAIK/Picnic/tree/v2.1.1
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

The original Picnic implementation includes optimizations that are not currently being built in liboqs. See src/sig/picnic/upstream/README for details.
