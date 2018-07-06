import csv
import tempfile
import os
import math
import itertools


################################################################################
def incl_range(l, u, s):
    return range(l, u+1, s)

def write_header_to_csv(filename, header_list):
    with open(filename, "w") as csvfile:
        writer = csv.DictWriter(
            csvfile, fieldnames=header_list)
        writer.writeheader()

def write_params_to_csv(filename, parameter_list, parameter_dict):
    with open(filename, "a") as csvfile:
        writer = csv.DictWriter(
            csvfile, fieldnames=parameter_list,extrasaction='ignore')
        writer.writerow(parameter_dict)

def calculate_sigma(params):
    params['sigma'] = RR(params['xi'] * 1.0) / math.sqrt(2 * math.log(2))
    return params['sigma']


def calculate_Le(params):
    params['Le'] = params['eta_e'] * params['h'] * params['sigma']
    return params['Le']

def calculate_Ls(params):
    params['Ls'] = params['eta_s'] * params['h'] * params['sigma']
    return params['Ls']

def calculate_B(params):
    k = params['k']
    n = params['n']
    Ls = params['Ls']
    M = params['M']
    # k*n root of M
    knm = (M ** (1. / (k * n)))

    initial_B = (knm + 2 * Ls - 1) * 1.0 / (2 * (1 - knm))
    initial_B = 2 ** ceil(RR(log(initial_B,2))) - 1
    params['B'] = initial_B
    return params['B']

def calculate_d(params):
    Le = params['Le']
    k = params['k']
    n = params['n']
    B = params['B']
    # you have to set self.current_param_object.d in the end

    initial_d = ceil(log(B,2))+1

    ls = (0.3 ** (1./ (k*n)))
    t = 1 - ls
    dv = (2*Le + 1) / t
    params['d'] = int(max(ceil(math.log(dv, 2)),initial_d))
    return params['d']

def calculate_h(params):
    n = params['n']
    la = params['la']
    # you have to set self.current_param_object.h in the end

    h = 1
    while 2**h * binomial(n, h) < 2**(2*la):
        h += 1
    params['h'] = h
    return params['h']

def calculate_q_heuristic(params):
    n = params['n']
    d = params['d']
    B = params['B']

    initial_q = 4*B+1
    initial_q_second = 2**d +1
    initial_q = int(max(initial_q,initial_q_second))
    q_found = False
    while not q_found:
        initial_q = next_prime(initial_q)
        q_found = (initial_q % (2 * n) == 1)  
    params['q'] = initial_q
    return params['q']

def calculate_q_provable(params):
    calculate_dH(params)
    calculate_dS(params)
    calculate_dL(params)
    dS = params['dS']
    dH = params['dH']
    dL = params['dL']
    k = params['k']
    la = params['la']
    qs = params['qs']
    qh = params['qh']

    n = params['n']
    d = params['d']
    B = params['B']

    initial_q = 4*B+1
    initial_q_second = 2**d +1 
    initial_q_third = (dS * dH * dL)** (1./(n*k))
    initial_q_forth = (3.*2.**(4. * la + k * n * d ) * qs**3. * (qs + qh)**2.)**(1./(n*k))

    initial_q = int(max(initial_q,initial_q_second,initial_q_third,initial_q_forth))
    q_found = False
    while not q_found:
        initial_q = next_prime(initial_q)
        q_found = (initial_q % (2 * n) == 1)
    params['q'] = initial_q
    return params['q']
    
def calculate_dH(params):
    sm = 0
    for j in incl_range(0, params['h'], 1):
        for i in incl_range(0, params['h'] - j, 1):
            sm += binomial(params['k'] * params['n'], 2 * i) * 2**(2*i) * binomial(params['k'] * params['n'] - 2 * i, j) * 2**j
    params['dH'] = sm
    return params['dH']

def calculate_dS(params):
    params['dS'] = (4 * (params['B'] - params['Ls']) + 1) ** params['n']
    return params['dS']

def calculate_dL(params):
    params['dL'] = (2 ** (params['d'] + 1) + 1) ** (params['n']*params['k'])
    return params['dL']


def calculate_pksize(params):
    k = params['k']
    n = params['n']
    q = params['q']
    kappa = params['kappa']

    params['pksize'] = k * n * (math.ceil(math.log(q, 2))) + k * kappa
    return params['pksize']

def calculate_sksize(params):
    n = params['n']
    sigma = params['sigma']
    k = params['k']
    kappa = params['kappa']
    la = params['la']

    if (la==95):
        t=11.6
    elif (la==128):
        t=13.4
    elif (la==155):
        t=14.8
    elif (la==160):
        t=15 
    elif (la==192):
        t=16.4
    elif (la==225):
        t=17.8
    elif (la==256):
        t=18.9
    else:
        t=14

    params['sksize'] =  n * (k+1) * math.ceil(math.log(t * sigma + 1,2)) + 2*kappa
    return params['sksize']

def calculate_sigsize(params):
    kappa = params['kappa']
    n = params['n']
    B = params['B']
    Ls = params['Ls']
    params['sigsize'] = kappa + n * (math.ceil(math.log(B - Ls, 2)) + 1)
    return params['sigsize']


################################################################################
def call_LWE_Estimator(params):
    n = params['n']
    q = params['q']
    sigma = params['sigma']
    k= params['k']

    load("estimator.py")
    alpha = sigma / q
    costs = estimate_lwe(n, alpha, q, n*k, reduction_cost_model=BKZ.qsieve)
    hardness_usvp = costs['usvp']['rop']
    hardness_lwe = hardness_usvp
    for alg in costs:
        hardness_alg = costs[alg]['rop']
        if hardness_lwe > hardness_alg:
            hardness_lwe = hardness_alg

    params['hardness'] = RR(log(hardness_lwe,2))
    return params['hardness']

################################################################################
def calculate_params(xi, k, n, eta_e, eta_s, M, kappa, la, prof):
    assert prof in ["Heuristic","Provably-Secure"], 'Proof for parameter selection should be either "Heuristic" or "Provably-Secure".' 
    parameters = {}
    parameters['xi'] = xi
    parameters['k'] = k
    parameters['n'] = n
    parameters['eta_e'] = eta_e
    parameters['eta_s'] = eta_s
    parameters['M'] = M
    parameters['kappa'] = kappa
    parameters['la'] = la
    parameters['qs'] = 2 ** 64
    parameters['qh'] = 2 ** 128

    sigma = calculate_sigma(parameters)
    h = calculate_h(parameters)
    Le = calculate_Le(parameters)
    Ls = calculate_Ls(parameters)
    B = calculate_B(parameters)
    d = calculate_d(parameters)

    if prof == "Heuristic":
        q = calculate_q_heuristic(parameters)
    elif prof == "Provably-Secure":
        q = calculate_q_provable(parameters)

    parameters['logq'] = round(RR(log(q,2)),3)
    pksize = calculate_pksize(parameters)
    sksize = calculate_sksize(parameters)
    sigsize = calculate_sigsize(parameters)

    if parameters['q']>2**(32):
        parameters['hardness'] = 0
    else: 
        parameters['hardness'] = call_LWE_Estimator(parameters)

    return parameters

##############
def range_calculate_params(xi_start, xi_end, xi_step,k_start, k_end, k_step,n_start, n_end, n_step,eta_e, eta_s, M, kappa, la,prof,filename):
    assert prof in ["Heuristic","Provably-Secure"], """Proof for parameter selection should be either "Heuristic" or "Provably-Secure"."""
    
    xi_range = incl_range(xi_start, xi_end, xi_step)
    k_range = incl_range(k_start, k_end, k_step)
    n_range = incl_range(n_start, n_end, n_step)

    parameter_list = [
        'xi', 'la', 'n', 'k', 'eta_e', 'eta_s', 'M', 'kappa', 'sigma',
        'h', 'Le', 'Ls', 'q', 'logq', 'B', 'd', 'qs' , 'qh', 'pksize',
        'sksize', 'sigsize'] + ['hardness']
    write_header_to_csv(filename, parameter_list)

    # itertools produces all possible combinations of the free parameters
    # xi, k and n
    # range n: 512, 1024, 2048, step = no steps just those three 
    # range k: 1,...,10, step = 1
    # range xi: 10,...,180, step = 10
    # eta_e=eta_s=2.8
    # M= 0.6
    # kappa=256
    # la = 128, 192, 256 + probability from key generation + loss in the number of LWE-samples (reduction from uniform to gaussian error)

    parameter_combinations = itertools.product(xi_range, k_range, n_range)

    for combination in parameter_combinations:
        parameters = calculate_params(
            combination[0], combination[1], combination[2], eta_e, eta_s, M,
            kappa, la, prof)

        if parameters['hardness']>= parameters['la']: 
            if parameters['logq']<= 32: 
		print parameter_list
		print parameters
		write_params_to_csv(filename, parameter_list, parameters)

def print_qtesla_parameters():
    parameter_list = [
        'xi', 'la', 'n', 'k', 'eta_e', 'eta_s', 'M', 'kappa', 'sigma',
        'h', 'Le', 'Ls', 'q', 'logq', 'B', 'd', 'pksize',
        'sksize', 'sigsize'] + ['hardness']

    print "qTesla Heuristic category 1 parameters"
    print """calculate_params(27.9988,1,512,2.223,2.223,0.3,256,95,"Heuristic")"""
    params=calculate_params(27.9988,1,512,2.223,2.223,0.3,256,95,"Heuristic")
    print "\n",['{}'.format(round(params[k],3)).rstrip('0').rstrip('.') for k in parameter_list],"\n"

    print "qTesla Heuristic category 3 (speed) parameters"
    print """calculate_params(12,1,1024,2.346,2.52,0.3,256,160,"Heuristic")"""
    params=calculate_params(12,1,1024,2.346,2.52,0.3,256,160,"Heuristic")
    print "\n",['{}'.format(round(params[k],3)).rstrip('0').rstrip('.') for k in parameter_list],"\n"

    print "qTesla Heuristic category 3 (size) parameters"
    print """calculate_params(9.9962,1,1024,2.233,2.233,0.3,256,160,"Heuristic")"""
    params=calculate_params(9.9962,1,1024,2.233,2.233,0.3,256,160,"Heuristic")
    print "\n",['{}'.format(round(params[k],3)).rstrip('0').rstrip('.') for k in parameter_list],"\n"

    print "qTesla Provable category 1 parameters"   
    print """"calculate_params(10,4,1024,2.61,2.61,0.3,256,95,"Provably-Secure")"""
    params=calculate_params(10,4,1024,2.61,2.61,0.3,256,95,"Provably-Secure")
    print "\n",['{}'.format(round(params[k],3)).rstrip('0').rstrip('.') for k in parameter_list],"\n"

    print "qTesla Provable category 3 parameters"   
    print """calculate_params(10,5,2048,2.65,2.65,0.3,256,160,"Provably-Secure")"""
    params=calculate_params(10,5,2048,2.65,2.65,0.3,256,160,"Provably-Secure")
    print "\n",['{}'.format(round(params[k],3)).rstrip('0').rstrip('.') for k in parameter_list],"\n"
