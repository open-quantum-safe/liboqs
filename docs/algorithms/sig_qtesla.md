Algorithm data sheet: `sig_qtesla`
==================================

Algorithm
---------

**Name:** qTesla

**Description:**

qTESLA is a family of post-quantum signature schemes based on the hardness of the decisional Ring Learning With Errors (R-LWE) problem. The scheme is an efficient variant of the Bai-Galbraith signature scheme, which in turn is based on the "Fiat-Shamir with Aborts" framework by Lyubashevsky, adapted to the setting of ideal lattices.

qTESLA utilizes two different approaches for parameter generation in order to target a wide range of application scenarios. The first approach, referred to as "heuristic qTESLA", follows a heuristic parameter generation. The second approach, referred to as "provably- secure qTESLA", follows a provably-secure parameter generation according to existing security reductions.

**Supporting research:**
- [qTesla] Sedat Akleylek, Erdem Alkim, Paulo S. L. M. Barreto, Nina Bindel, Johannes Buchmann, Edward Eaton, Gus Gutoski, Juliane Krämer, Patrick Longa, Harun Polat, Jefferson E. Ricardini, and Gustavo Zanon. qTesta NIST submission available at [https://qtesla.org/wp-content/uploads/2018/07/qTESLA_NIST_update_06.30.2018.zip].

Security
--------

**Security model:** existential unforgeability in the random oracle model (ROM), or quantum random oracle model (QROM).

**Underlying hard problem(s):** decisional Ring Learning With Errors (R-LWE).


Parameter set 1
---------------

`qTESLA_I` parameter set from [qTesla]

**Claimed classical security:** 

- 104 bits

**Claimed quantum security:** 

- 97 bits

**Data sizes:** 

- Private key: 1216 bytes
- Public key: 1504 bytes
- Signature: 1376 bytes

Parameter set 2
---------------

`qTESLA_III_speed` parameter set from [qTesla]

**Claimed classical security:** 

- 178 bits

**Claimed quantum security:** 

- 164 bits

**Data sizes:** 

- Private key: 2112 bytes
- Public key: 3104 bytes
- Signature: 2848 bytes

Parameter set 3
---------------

`qTESLA_III_size` parameter set from [qTesla]

**Claimed classical security:** 

- 188 bits

**Claimed quantum security:** 

- 169 bits

**Data sizes:** 

- Private key: 2112 bytes
- Public key: 2976 bytes
- Signature: 2720 bytes

Implementation
--------------

**Source of implementation:** https://github.com/qtesla/qTesla

**License:** public domain

**Language:** C

**Constant-time:** Yes

**Testing:**

- Correctness: covered by test harness `test_sig`
- Statistics of signatures: covered by test harness `test_sig`
- Static analysis:
	- `scan_build`

