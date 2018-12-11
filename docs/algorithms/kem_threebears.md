liboqs nist-branch algorithm datasheet: `kem_threebears`
========================================================

Summary
-------

- **Name**: ThreeBears
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: integer module learning with errors (IMLWE)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip
- **Submitters (to NIST competition)**: Mike Hamburg
- **Submitters' website**: https://www.rambus.com/
- **Added to liboqs by**: Mike Hamburg

Parameter sets
--------------

| Parameter set         | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BabyBear-CCA-KEM      |     IND-CCA    |              2              |           804           |           40            |            917          |             32             |
| MamaBear-CCA-KEM      |     IND-CCA    |              4              |           1194          |           40            |           1307          |             32             |
| PapaBear-CCA-KEM      |     IND-CCA    |              5              |           1584          |           40            |           1697          |             32             |
| BabyBearEphem-CPA-KEM |     IND-CPA    |              2              |           804           |           40            |            917          |             32             |
| MamaBearEphem-CPA-KEM |     IND-CPA    |              5              |           1194          |           40            |           1307          |             32             |
| PapaBearEphem-CPA-KEM |     IND-CPA    |              5              |           1584          |           40            |           1697          |             32             |

Implementation
--------------

- **Source of implementation:** https://sourceforge.net/p/threebears/code/ci/master/tree/ (opt variant)
- **License:** MIT
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64

Additional comments
-------------------
I intended to include this implementation as `Additional_Implementations/With_Asm` in the NIST submission, but due to a typo in the Makefile that implementation was the same as the `Optimized_Implementation`.  The difference is only a few lines of inline assembly for Haswell and later platforms.  Since this code is behind a #ifdef, the package should compile cleanly on other 32- and 64-bit platforms.
