Algorithm data sheet: `kex_sidh_cln16`
=======================================

Algorithm
---------

**Name:** SIDH-CLN16

**Description:** Key exchange protocol proposed by Costello et al. [CLN16] based on the hidden supersingular isogeny problem. Instantiation of the SIDH scheme proposed by De Feo, Jao and Plut in [DJP11].

**Supporting research:**

- [CLN16] Craig Costello, Patrick Longa, and Michael Naehrig. Efficient algorithms for supersingular isogeny
Diffie-Hellman. In *CRYPTO 2016*, volume 9814 of LNCS, pp 572-601. Springer, 2016. [https://eprint.iacr.org/2016/413](https://eprint.iacr.org/2016/413)
- [DJP11] Luca De Feo and David Jao and Jérôme Plût. Towards quantum-resistant cryptosystems from supersingular elliptic curve isogenies. In *PQCRYPTO 2011*, volume 7071 of LNCS, pages 19-34. Springer, 2011. [https://eprint.iacr.org/2011/506](https://eprint.iacr.org/2011/506)


Security
--------

**Security model:** Authenticated-links adversarial model

**Underlying hard problem(s):** Supersingular Decision Diffie-Hellman problem

Parameter set 1
---------------

"Recommended" parameter set from [CLN16]

**Claimed classical security:**

- 2^192 (CLN16)

**Claimed quantum security:**

- 2^128 (CLN16)

**Communication size:**

- Alice → Bob: 564 bytes (uncompressed), 330 bytes (compressed)
- Bob → Alice: 564 bytes (uncompressed), 330 bytes (compressed)
- total: 1128 bytes (uncompressed), 660 bytes (compressed)

Implementation
--------------

**Source of implementation:** Original research paper ([https://github.com/Microsoft/PQCrypto-SIDH](https://github.com/Microsoft/PQCrypto-SIDH))

**License:** MIT License

**Language:** C

**Constant-time:** Yes

**Testing:**

- Correctness: covered by test harness `test_kex`
- Statistics of shared secrets: covered by test harness `test_kex`
- Static analysis:
	- `scan_build`
