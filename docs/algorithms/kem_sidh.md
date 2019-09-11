liboqs algorithm datasheet: `kem_sidh`
====================================================

Summary
-------

- **Name**: SIDH
- **Algorithm type**: key exchange
- **Note**: SIDH is as ephemeral key exchange algorithm related to the SIKE KEM (sike.org) submitted to NIST. It is NOT secure to use it with static keys.
- **Main cryptographic assumption**: (supersingular) isogeny walk problem
- **Submitters' website**: https://github.com/Microsoft/PQCrypto-SIDH
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| SIDH-p434            |     IND-CPA    |              1              |            330          |             28          |            330          |            110             |
| SIDH-p434-compressed |     IND-CPA    |              1              |            196          |             28          |            196          |            110             |
| SIDH-p503            |     IND-CPA    |              2              |            378          |             32          |            378          |            126             |
| SIDH-p503-compressed |     IND-CPA    |              2              |            224          |             32          |            224          |            126             |
| SIDH-p610            |     IND-CPA    |              3              |            462          |             39          |            462          |            154             |
| SIDH-p610-compressed |     IND-CPA    |              3              |            273          |             39          |            273          |            154             |
| SIDH-p751            |     IND-CPA    |              5              |            564          |             48          |            564          |            188             |
| SIDH-p751-compressed |     IND-CPA    |              5              |            334          |             48          |            334          |            188             |

Implementation
--------------

- **Source of implementation:** https://github.com/Microsoft/PQCrypto-SIDH
- **Implementation version:** v3.2 (https://github.com/microsoft/PQCrypto-SIDH/tree/v3.2)
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

No KAT are available for SIDH.
