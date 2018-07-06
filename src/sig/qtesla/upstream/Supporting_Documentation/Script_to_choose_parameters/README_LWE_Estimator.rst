Security Estimates for the Learning with Errors Problem
=======================================================

This `Sage <http://sagemath.org>`__ module provides functions for estimating the concrete security
of `Learning with Errors <https://en.wikipedia.org/wiki/Learning_with_errors>`__ instances.

The main intend of this estimator is to give designers an easy way to choose parameters resisting
known attacks and to enable cryptanalysts to compare their results and ideas with other techniques
known in the literature.

Usage Examples
--------------

::

    sage: load("estimator.py")
    sage: n, alpha, q = Param.Regev(128)
    sage: costs = estimate_lwe(n, alpha, q)
    usvp: rop:  ≈2^51.1,  red:  ≈2^51.1,  δ_0: 1.009214,  β:  102,  d:  357,  m:      610
     dec: rop:  ≈2^56.8,  m:      235,  red:  ≈2^56.8,  δ_0: 1.009311,  β:   99,  d:  363,  babai:  ≈2^42.2,  babai_op:  ≈2^57.3,  repeat:      146,  ε: 0.031250
    dual: rop:  ≈2^74.7,  m:      376,  red:  ≈2^74.7,  δ_0: 1.008810,  β:  111,  d:  376,  |v|:  736.521,  repeat:  ≈2^19.0,  ε: 0.003906

Online
------

You can `run the estimator
online <http://aleph.sagemath.org/?z=eJxNjcEKwjAQBe-F_kPoqYXYjZWkKHgQFPyLkOhii6mJyWrx782hiO84MPOcN9e6GohC2gHYkezrckdqfbzBZJwFN-MKE42TIR8hmhnOp8MRfqgNn6opiwdnxoXBcPZke9ZJxZlohRDbXknVSbGMMyXlpi-LhKTfGK1PWK-zr7O1NFHnz_ov2HwBPwsyhw==&lang=sage>`__
using the `Sage Math Cell <http://aleph.sagemath.org/>`__ server.

Coverage
--------

At present the following algorithms are covered by this estimator.

-  meet-in-the-middle exhaustive search
-  Coded-BKW [C:GuoJohSta15]
-  dual-lattice attack and small/sparse secret variant [EC:Albrecht17]
-  lattice-reduction + enumeration [RSA:LinPei11]
-  primal attack via uSVP [ICISC:AlbFitGop13,ACISP:BaiGal14]
-  Arora-Ge algorithm [ICALP:AroGe11] using Gröbner bases
   [EPRINT:ACFP14]

Above, we use `cryptobib <http://cryptobib.di.ens.fr>`__-style bibtex keys as references.

Evolution
---------

This code is evolving, new results are added and bugs are fixed. Hence, estimations from earlier
versions might not match current estimations. This is annoying but unavoidable at present. We
recommend to also state the commit that was used when referencing this project.

We also encourage authors to let us know if their paper uses this code. In particular, we thrive to
tag commits with those cryptobib ePrint references that use it. For example, `this commit
<https://bitbucket.org/malb/lwe-estimator/src/6295aa59048daa5d9598378386cb61887a1fe949/?at=EPRINT_Albrecht17>`__
corresponds to this `ePrint entry <https://ia.cr/2017/047>`__.

Contributions
-------------

Our intent is for this estimator to be maintained by the research community. For example, we
encourage algorithm designers to add their own algorithms to this estimator and we are happy to help
with that process.

More generally, all contributions such as bugfixes, documentation and tests are welcome. Please go
ahead and submit your pull requests. Also, don’t forget to add yourself to the list of contributors
below in your pull requests.

At present, this estimator is maintained by Martin Albrecht. Contributors are:

-  Martin Albrecht
-  Florian Göpfert
-  Cedric Lefebvre
-  Rachel Player
-  Markus Schmidt
-  Sam Scott

Please follow `PEP8 <https://www.python.org/dev/peps/pep-0008/>`__ in your submissions. You can use
`flake8 <http://flake8.pycqa.org/en/latest/>`__ to check for compliance. We use the following flake8
configuration (to allow longer line numbers and more complex functions):

::

    [flake8]
    max-line-length = 120
    max-complexity = 16
    ignore = E22,E241

Bugs
----

If you run into a bug, please open an `issue on bitbucket
<https://bitbucket.org/malb/lwe-estimator/issues?status=new&status=open>`__. Also, please check
first if the issue has already been reported.

Citing
------

If you use this estimator in your work, please cite

    | Martin R. Albrecht, Rachel Player and Sam Scott. *On the concrete hardness of Learning with Errors*.
    | Journal of Mathematical Cryptology. Volume 9, Issue 3, Pages 169–203, ISSN (Online) 1862-2984,
    | ISSN (Print) 1862-2976 DOI: 10.1515/jmc-2015-0016, October 2015

A pre-print is available as

    Cryptology ePrint Archive, Report 2015/046, 2015. https://eprint.iacr.org/2015/046

A high-level overview of that work is given, for instance, in this
`talk <https://martinralbrecht.files.wordpress.com/2015/05/20150507-lwe-survey-london.pdf>`__.


Parameters from the Literature
------------------------------

The following estimates for various schemes from the literature illustrate the behaviour of the
``estimator``. These estimates do not necessarily correspond to the claimed security levels of the
respective schemes: for several parameter sets below the claimed security level by the designers’ is
lower than the complexity estimated by the ``estimator``. This is usually because the designers
anticipate potential future improvements to lattice-reduction algorithms and strategies. We
recommend to follow the designers’ decision. We intend to extend the estimator to cover these more
optimistic (from an attacker’s point of view) estimates in the future … pull requests welcome, as
always.

`New Hope <http://ia.cr/2015/1092>`__ ::

    sage: load("estimator.py")
    sage: n = 1024; q = 12289; stddev = sqrt(16/2); alpha = alphaf(sigmaf(stddev), q)
    sage: _ = estimate_lwe(n, alpha, q, reduction_cost_model=BKZ.sieve)
    usvp: rop: ≈2^313.1,  red: ≈2^313.1,  δ_0: 1.002094,  β:  968,  d: 2101,  m:  ≈2^11.7
     dec: rop: ≈2^410.0,  m:     1308,  red: ≈2^410.0,  δ_0: 1.001763,  β: 1213,  d: 2332,  babai: ≈2^395.5,  babai_op: ≈2^410.6,  repeat:  ≈2^25.2,  ε: ≈2^-23.0
    dual: rop: ≈2^355.5,  m:  ≈2^11.1,  red: ≈2^355.5,  δ_0: 1.001884,  β: 1113,  repeat: ≈2^307.0,  d: 2263,  c:        1
    
`Frodo <http://ia.cr/2016/659>`__ ::

    sage: load("estimator.py")
    sage: n = 752; q = 2^15; stddev = sqrt(1.75); alpha = alphaf(sigmaf(stddev), q)
    sage: _ = estimate_lwe(n, alpha, q, reduction_cost_model=BKZ.sieve)
    usvp: rop: ≈2^173.0,  red: ≈2^173.0,  δ_0: 1.003453,  β:  490,  d: 1448,  m:  ≈2^11.1
     dec: rop: ≈2^208.3,  m:      829,  red: ≈2^208.3,  δ_0: 1.003064,  β:  579,  d: 1581,  babai: ≈2^194.5,  babai_op: ≈2^209.6,  repeat:      588,  ε: 0.007812
    dual: rop: ≈2^196.2,  m:     1588,  red: ≈2^196.2,  δ_0: 1.003104,  β:  569,  repeat: ≈2^135.0,  d: 1588,  c:        1

`TESLA <http://ia.cr/2015/755>`__ ::

    sage: load("estimator.py")
    sage: n = 804;  q = 2^31 - 19; alpha = sqrt(2*pi)*57/q; m = 4972
    sage: _ = estimate_lwe(n, alpha, q, m=m, reduction_cost_model=BKZ.sieve)
    usvp: rop: ≈2^129.3,  red: ≈2^129.3,  δ_0: 1.004461,  β:  339,  d: 1954,  m:  ≈2^11.6
     dec: rop: ≈2^144.9,  m:     1237,  red: ≈2^144.9,  δ_0: 1.004148,  β:  378,  d: 2041,  babai: ≈2^130.9,  babai_op: ≈2^146.0,  repeat:       17,  ε: 0.250000
    dual: rop: ≈2^139.4,  m:     2035,  red: ≈2^139.4,  δ_0: 1.004180,  β:  373,  repeat:  ≈2^93.0,  d: 2035,  c:        1

`SEAL v2.2 <https://www.microsoft.com/en-us/research/wp-content/uploads/2017/06/sealmanual.pdf>`__ ::

    sage: load("estimator.py")
    sage: n = 2048; q = 2^60 - 2^14 + 1; alpha = 8/q; m = 2*n
    sage: _ = estimate_lwe(n, alpha, q, secret_distribution=(-1,1), reduction_cost_model=BKZ.sieve, m=m)
    usvp: rop: ≈2^115.5,  red: ≈2^115.5,  δ_0: 1.004975,  β:  288,  d: 4013,  m:  ≈2^12.6
     dec: rop: ≈2^127.1,  m:  ≈2^11.1,  red: ≈2^127.1,  δ_0: 1.004663,  β:  318,  d: 4237,  babai: ≈2^114.8,  babai_op: ≈2^129.9,  repeat:        7,  ε: 0.500000
    dual: rop: ≈2^118.4,  m:  ≈2^12.0,  red: ≈2^118.4,  δ_0: 1.004864,  β:  298,  repeat:  ≈2^58.8,  d: 4090,  c:    3.908,  k:       30,  postprocess:       13
