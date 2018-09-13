liboqs nist-branch algorithm datasheet: `kem_titanium`
======================================================

Summary
-------

- **Name:** Titanium CCA
- **Algorithm type:** Key Encapsulation Mechanism
- **Main cryptographic assumption:** Middle-Product Learning with Errors (MP-LWE)
- **NIST submission URL:** https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/Titanium.zip
- **Submitters (to NIST competition):** Ron Steinfeld, Amin Sakzad, Raymond K. Zhao
- **Submitters' website:** http://users.monash.edu.au/~rste/Titanium.html
- **Added to liboqs by:** Ben Davies

Parameter sets
--------------

| Parameter set      | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|--------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Titanium-CCA-super |    IND-CCA2    |             5               |       26912             |      26944              |      8352               |          32                |
| Titanium-CCA-hi    |    IND-CCA2    |             3               |       20512             |      20544              |      6048               |          32                |
| Titanium-CCA-med   |    IND-CCA2    |             1               |       18272             |      18304              |      4544               |          32                |
| Titanium-CCA-std   |    IND-CCA2    |             1               |       16352             |      16384              |      3552               |          32                |

Implementation
--------------

- **Source of implementation:** https://github.com/raykzhao/Titanium (commit a7547ad486e7220e1d9ffac7a76fe260dfa36cc3)
- **License:** Released into the public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x64

Additional comments
-------------------

