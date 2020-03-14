liboqs algorithm datasheet: `kem_classic_mceliece`
==================================================

Summary
-------

- **Name**: Classic McEliece
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Niederreiter's dual version of McEliece's public key encryption using binary Goppa codes
- **Scheme authors**: Daniel J. Bernstein, Tung Chou, Tanja Lange, Ingo von Maurich, Rafael Misoczki, Ruben Niederhagen, Edoardo Persichetti, Christiane Peters, Peter Schwabe, Nicolas Sendrier, Jakub Szefer, Wen Wang
- **Authors' website**: https://classic.mceliece.org
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set             | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Classic-McEliece-348864   |     IND-CCA    |              1              |          261120         |           6452          |           128           |             32             |
| Classic-McEliece-348864f  |     IND-CCA    |              1              |          261120         |           6452          |           128           |             32             |
| Classic-McEliece-460896   |     IND-CCA    |              3              |          524160         |          13568          |           188           |             32             |
| Classic-McEliece-460896f  |     IND-CCA    |              3              |          524160         |          13568          |           188           |             32             |
| Classic-McEliece-6688128  |     IND-CCA    |              5              |         1044992         |          13892          |           240           |             32             |
| Classic-McEliece-6688128f |     IND-CCA    |              5              |         1044992         |          13892          |           240           |             32             |
| Classic-McEliece-6960119  |     IND-CCA    |              5              |         1047319         |          13908          |           226           |             32             |
| Classic-McEliece-6960119f |     IND-CCA    |              5              |         1047319         |          13908          |           226           |             32             |
| Classic-McEliece-8192128  |     IND-CCA    |              5              |         1357824         |          14080          |           240           |             32             |
| Classic-McEliece-8192128f |     IND-CCA    |              5              |         1357824         |          14080          |           240           |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/b50dbbf544726006b9a08947e387f8972cb37492
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
