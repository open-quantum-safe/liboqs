liboqs algorithm datasheet: `kem_threebears`
============================================

Summary
-------

- **Name**: ThreeBears
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: integer module learning with errors (I-MLWE)
- **Scheme authors**: Mike Hamburg
- **Authors' website**: https://sourceforge.net/projects/threebears/
- **Added to liboqs by**: Douglas Stebila
- **NIST Round 2 submission**

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BabyBearEphem |     IND-CPA    |              1              |           804           |            40           |           917           |             32             |
| BabyBear      |     IND-CCA    |              1              |           804           |            40           |           917           |             32             |
| MamaBearEphem |     IND-CPA    |              3              |           1194          |            40           |           1307          |             32             |
| MamaBear      |     IND-CCA    |              3              |           1194          |            40           |           1307          |             32             |
| PapaBearEphem |     IND-CPA    |              5              |           1584          |            40           |           1697          |             32             |
| PapaBear      |     IND-CCA    |              5              |           1584          |            40           |           1697          |             32             |

Implementation
--------------

- **Source of implementation:** https://sourceforge.net/p/threebears/code/ci/f4ce0ebfc84a5e01a75bfc8297b6d175e993cfa4/
- **Implementation version:** https://github.com/PQClean/PQClean/commit/6de728361e33ad3a5a6997e0896ff9fe8e44a999
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
