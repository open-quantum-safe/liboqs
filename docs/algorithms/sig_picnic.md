liboqs nist-branch algorithm datasheet: `sig_picnic`
===================================================

Summary
-------

- **Name**: Picnic
- **Algorithm type**: signature
- **Main cryptographic assumption**: hash function security (ROM/QROM), key recovery attacks on the lowMC block cipher
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/Picnic.zip
- **Submitters (to NIST competition)**: Greg Zaverucha, Melissa Chase, David Derler, Steven Goldfeder, Claudio Orlandi, Sebastian Ramacher, Christian Rechberger, Daniel Slamanig
- **Submitters' website**: https://microsoft.github.io/Picnic/
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set   | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|-----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| picnic_L1_FS    |     EUF-CMA    |              1              |            33           |            49           |          34020         |
| picnic_L1_UR    |     EUF-CMA    |              1              |            33           |            49           |          53949         |
| picnic_L3_FS    |     EUF-CMA    |              3              |            49           |            73           |          76768         |
| picnic_L3_UR    |     EUF-CMA    |              3              |            49           |            73           |         121841         |
| picnic_L5_FS    |     EUF-CMA    |              5              |            65           |            97           |         132860         |
| picnic_L5_UR    |     EUF-CMA    |              5              |            65           |            97           |         209510         |

Implementation
--------------

- **Source of implementation:** https://github.com/IAIK/Picnic
- **Implementation version:** https://github.com/IAIK/Picnic/tree/v1.3.1
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

The original Picnic implementation includes optimizations that are not currently being built in liboqs. See src/sig/picnic/upstream/README for details.
