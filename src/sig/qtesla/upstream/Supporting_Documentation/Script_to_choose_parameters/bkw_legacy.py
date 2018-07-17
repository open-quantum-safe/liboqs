# -*- coding: utf-8 -*-
"""
BKW legacy code.

.. moduleauthor:: Martin R. Albrecht <martinralbrecht@googlemail.com>

"""
from collections import OrderedDict
from estimator import cost_reorder, stddevf, sigmaf
from estimator import preprocess_params, amplify_sigma, secret_distribution_variance
from sage.functions.log import log
from sage.functions.other import ceil, sqrt
from sage.matrix.all import Matrix
from sage.modules.all import vector
from sage.rings.all import RealField
from sage.rings.all import ZZ
from sage.rings.infinity import PlusInfinity
from sage.structure.element import parent
from sage.symbolic.all import pi


oo = PlusInfinity()


def bkw_decision(n, alpha, q, success_probability=0.99, prec=None):
    """
    Estimate the cost of running BKW to solve Decision-LWE following [DCC:ACFFP15]_.

    :param n:                    dimension > 0
    :param alpha:                fraction of the noise α < 1.0
    :param q:                    modulus > 0
    :param success_probability:  probability of success < 1.0
    :param prec:                 precision used for floating point computations
    :param m:                    the number of available samples

    .. [DCC:ACFFP15] Albrecht, M. R., Cid, C., Jean-Charles Faugère, Fitzpatrick, R., &
                     Perret, L. (2015). On the complexity of the BKW algorithm on LWE.
                     Designs, Codes & Cryptography, Volume 74, Issue 2, pp 325-354
    """
    n, alpha, q, success_probability = preprocess_params(n, alpha, q, success_probability)
    sigma = alpha*q

    RR = parent(alpha)

    def _run(t):
        a = RR(t*log(n, 2))  # target number of adds: a = t*log_2(n)
        b = RR(n/a)  # window width
        sigma_final = RR(n**t).sqrt() * sigma  # after n^t adds we get this σ

        m = amplify_sigma(success_probability, sigma_final, q)

        tmp = a*(a-1)/2 * (n+1) - b*a*(a-1)/4 - b/6 * RR((a-1)**3 + 3/2*(a-1)**2 + (a-1)/2)
        stage1a = RR(q**b-1)/2 * tmp
        stage1b = m * (a/2 * (n + 2))
        stage1  = stage1a + stage1b

        nrops = RR(stage1)
        nbops = RR(log(q, 2) * nrops)
        ncalls = RR(a * ceil(RR(q**b)/RR(2)) + m)
        nmem = ceil(RR(q**b)/2) * a * (n + 1 - b * (a-1)/2)

        current = OrderedDict([(u"t", t),
                               (u"bop", nbops),
                               (u"oracle", ncalls),
                               (u"m", m),
                               (u"mem", nmem),
                               (u"rop", nrops),
                               (u"a", a),
                               (u"b", b),
                               ])

        current = cost_reorder(current, ("rop", u"oracle", u"t"))
        return current

    best_runtime = None
    t = RR(2*(log(q, 2) - log(sigma, 2))/log(n, 2))
    while True:
        current = _run(t)
        if not best_runtime:
            best_runtime = current
        else:
            if best_runtime["rop"] > current["rop"]:
                best_runtime = current
            else:
                break
        t += 0.05

    return best_runtime


def bkw_search(n, alpha, q, success_probability=0.99, prec=None):
    """
    Estimate the cost of running BKW to solve Search-LWE following [C:DucTraVau15]_.

    :param n:                    dimension > 0
    :param alpha:                fraction of the noise α < 1.0
    :param q:                    modulus > 0
    :param success_probability:  probability of success < 1.0
    :param prec:                 precision used for floating point computations

    .. [EC:DucTraVau15] Duc, A., Florian Tramèr, & Vaudenay, S. (2015). Better algorithms for
                        LWE and LWR.
    """
    n, alpha, q, success_probability = preprocess_params(n, alpha, q, success_probability)
    sigma = stddevf(alpha*q)
    eps = success_probability

    RR = parent(alpha)

    # "To simplify our result, we considered operations over C to have the same
    # complexity as operations over Z_q . We also took C_FFT = 1 which is the
    # best one can hope to obtain for a FFT."
    c_cost = 1
    c_mem = 1
    c_fft = 1

    def _run(t):
        a = RR(t*log(n, 2))  # target number of adds: a = t*log_2(n)
        b = RR(n/a)  # window width
        epp = (1- eps)/a

        m = lambda j, eps: 8 * b * log(q/eps) * (1 -  (2 * pi**2 * sigma**2)/(q**2))**(-2**(a-j))  # noqa

        c1 = (q**b-1)/2 * ((a-1)*(a-2)/2 * (n+1) - b/6 * (a*(a-1) * (a-2)))
        c2 = sum([m(j, epp) * (a-1-j)/2 * (n+2) for j in range(a)])
        c3 = (2*sum([m(j, epp) for j in range(a)]) + c_fft * n * q**b * log(q, 2)) * c_cost
        c4 = (a-1)*(a-2) * b * (q**b - 1)/2

        nrops = RR(c1 + c2 + c3 + c4)
        nbops = RR(log(q, 2) * nrops)
        ncalls = (a-1) * (q**b - 1)/2 + m(0, eps)
        nmem = ((q**b - 1)/2 * (a-1) * (n + 1 - b*(a-2)/2)) + m(0, eps) + c_mem * q**b

        current = OrderedDict([(u"t", t),
                               (u"bop", nbops),
                               (u"oracle", ncalls),
                               (u"m", m(0, eps)),
                               (u"mem", nmem),
                               (u"rop", nrops),
                               (u"a", a),
                               (u"b", b),
                               ])

        current = cost_reorder(current, ("rop", u"oracle", u"t"))
        return current

    best_runtime = None
    best = None
    t = RR(2*(log(q, 2) - log(sigma, 2))/log(n, 2))
    while True:
        current = _run(t)

        if not best_runtime:
            best_runtime = current
        else:
            if best_runtime["rop"] > current["rop"]:
                best_runtime = current
            else:
                break
        t += 0.05

    return best


def bkw_small_secret_variances(q, a, b, kappa, o, RR=None):
    """
    Helper function for small secret BKW variant.

    :param q:
    :param a:
    :param b:
    :param kappa:
    :param o:
    :param RR:
    :returns:
    :rtype:

    """
    if RR is None:
        RR = RealField()
    q = RR(q)
    a = RR(a).round()
    b = RR(b)
    n = a*b
    kappa = RR(kappa)
    T = RR(2)**(b*kappa)
    n = RR(o)/RR(T*(a+1)) + RR(1)

    U_Var = lambda x: (x**2 - 1)/12  # noqa
    red_var   = 2*U_Var(q/(2**kappa))

    if o:
        c_ = map(RR, [0.0000000000000000,
                      0.4057993538687922, 0.6924478992819291, 0.7898852691349439,
                      0.8441959360364506, 0.8549679124679972, 0.8954469872316165,
                      0.9157093365103325, 0.9567635780119543, 0.9434245442818547,
                      0.9987153221343770])

        M = Matrix(RR, a, a)  # rows are tables, columns are entries those tables
        for l in range(M.ncols()):
            for c in range(l, M.ncols()):
                M[l, c] = U_Var(q)

        for l in range(1, a):
            for i in range(l):
                M[l, i] = red_var + sum(M[i+1:l].column(i))

                bl = b*l
                if round(bl) < len(c_):
                    c_tau = c_[round(bl)]
                else:
                    c_tau = RR(1)/RR(5)*RR(sqrt(bl)) + RR(1)/RR(3)

                f = (c_tau*n**(~bl) + 1 - c_tau)**2
                for i in range(l):
                    M[l, i] = M[l, i]/f

        v = vector(RR, a)
        for i in range(a):
            v[i] = red_var + sum(M[i+1:].column(i))
    else:
        v = vector(RR, a)
        for i in range(a)[::-1]:
            v[i] = 2**(a-i-1) * red_var

    return v


def bkw_small_secret(n, alpha, q, secret_distribution=True, success_probability=0.99, t=None, o=0, samples=None):  # noqa
    """
    :param n:               number of variables in the LWE instance
    :param alpha:           standard deviation of the LWE instance
    :param q:               size of the finite field (default: n^2)
    """

    def sigma2f(kappa):
        v = bkw_small_secret_variances(q, a, b, kappa, o, RR=RR)
        return sigmaf(sum([b * e * secret_variance for e in v], RR(0)).sqrt())

    def Tf(kappa):
        return min(q**b, ZZ(2)**(b*kappa))/2

    def ops_tf(kappa):
        T = Tf(kappa)
        return T * (a*(a-1)/2 * (n+1) - b*a*(a-1)/4 - b/6 * ((a-1)**3 + 3/2*(a-1)**2 + 1/RR(2)*(a-1)))

    def bkwssf(kappa):
        ret = OrderedDict()
        ret[u"κ"] = kappa
        m = amplify_sigma(success_probability, [sigma_final, sigma2f(kappa)], q)
        ret["m"] = m
        ropsm = (m + o)  * (a/2 * (n + 2))
        ropst = ops_tf(kappa)
        ret["rop"] = ropst + ropsm
        T = Tf(kappa)
        ret["mem"] = T * a * (n + 1 - b * (a-1)/2)
        ret["oracle"] = T * a + ret["m"] + o
        return ret

    n, alpha, q, success_probability = preprocess_params(n, alpha, q, success_probability, prec=4*n)
    RR = alpha.parent()
    sigma = alpha*q

    if o is None:
        best = bkw_small_secret(n, alpha, q, secret_distribution, success_probability, t=t, o=0)
        o = best["oracle"]/2
        while True:
            current = bkw_small_secret(n, alpha, q, secret_distribution, success_probability, t=t, o=o)
            if best is None or current["rop"] < best["rop"]:
                best = current
            if current["rop"] > best["rop"]:
                break

            o = o/2
        return best

    if t is None:
        t = RR(2*(log(q, 2) - log(sigma, 2))/log(n, 2))
        best = None
        while True:
            current = bkw_small_secret(n, alpha, q, secret_distribution, success_probability, t=t, o=o)
            if best is None or current["rop"] < best["rop"]:
                best = current
            if current["rop"] > best["rop"]:
                break
            t += 0.01
        return best

    secret_variance = secret_distribution_variance(secret_distribution)
    secret_variance = RR(secret_variance)

    a = RR(t*log(n, 2))  # the target number of additions: a = t*log_2(n)
    b = n/a  # window width b = n/a
    sigma_final = RR(n**t).sqrt() * sigma  # after n^t additions we get this stddev
    transformation_noise = sqrt(n * 1/RR(12) * secret_variance)
    kappa = ceil(log(round(q*transformation_noise/stddevf(sigma)), 2.0)) + 1

    if kappa > ceil(log(q, 2)):
        kappa = ceil(log(q, 2))

    best = None
    while kappa > 0:
        current = bkwssf(kappa)
        if best is None or current["rop"] < best["rop"]:
            best = current
        if current["rop"] > best["rop"]:
            break
        kappa -= 1

    best["o"] = o
    best["t"] = t
    best["a"] = a
    best["b"] = b
    best = cost_reorder(best, ["rop", "oracle", "t", "m", "mem"])
    return best
