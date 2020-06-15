ThreeBears
==========

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: integer module learning with errors (I-MLWE)
- **Scheme authors**: Mike Hamburg
- **Authors' website**: https://sourceforge.net/projects/threebears/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://sourceforge.net/projects/threebears/
- **Implementation version**: https://github.com/PQClean/PQClean/commit/9023fef55861faccd82146cf599b9e46fb9606aa
- **License**: MIT License
- **Constant-time**: Yes
- **Optimizations**: Portable C

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
