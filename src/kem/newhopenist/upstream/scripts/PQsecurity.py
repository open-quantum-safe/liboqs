from math import *
from decimal import *
getcontext().prec = 30

log_infinity = 9999

global_log2_eps = 0

## The root hermite factor delta of BKZ-b
def delta_BKZ(b):
	return ( (pi*b)**(1./b) * b / (2*pi*exp(1)))**(1./(2.*b-2.))



## log_2 of best plausible Quantum Cost of SVP in dimension b
def svp_plausible(b):
	return b *log(sqrt(4./3))/log(2)   # .2075 * b 

## log_2 of best plausible Quantum Cost of SVP in dimension b
def svp_quantum(b):
	return b *log(sqrt(13./9))/log(2) # .265 * b  [Laarhoven Thesis]

## log_2 of best known Quantum Cost of SVP in dimension b
def svp_classical(b):
	return b *log(sqrt(3./2))/log(2) # .292 * b [Becker Ducas Laarhoven Gama]

def nvec_sieve(b):
	return b *log(sqrt(4./3))/log(2) #.2075 * b


## Return the cost of the primal attack using m samples and blocksize b (infinity = fail)
def primal_cost(q,n,m,s,b, cost_svp = svp_classical,verbose=False):
	d = n + m
	delta = delta_BKZ(b)
	if verbose:
		print "Primal attacks uses block-size", b, "and ", m, "samples"

	if s * sqrt(b) < delta**(2.*b-d-1) * q**(1.*m /d):
		return cost_svp(b)
	else:
		return log_infinity


## Return the cost of the dual attack using m samples and blocksize b (infinity = fail)
def dual_cost(q,n,m,s,b, cost_svp = svp_classical, verbose = False):
	global global_log2_eps
	d = n+m
	delta = delta_BKZ(b)
	l = delta**d * q**(1.*n/d)

	tau = l * s / q
	log2_eps = - 2 * pi * pi * tau**2 / log(2)
	global_log2_eps = log2_eps
	log2_R = max( 0 , - 2 * log2_eps - nvec_sieve(b) ) 
	if verbose:
		print "Dual attacks uses block-size", b, "and ", m, "samples"
		print "log2(epsilon) = ", log2_eps, "log2 nvector per run", nvec_sieve(b)
	return + cost_svp(b) + log2_R


## Find optimal parameters for a given attack
def optimize_attack(q,n,k,s, cost_attack = primal_cost, cost_svp = svp_classical, verbose = True):
	best_cost = log_infinity
	best_b = 0
	best_m = 0
	for b in range(50,2*n+k+1):
		if cost_svp(b) > best_cost:
			break
		for m in range(max(1,b-n),n+k ):
			cost = cost_attack(q,n,m,s,b, cost_svp)
			if cost<best_cost:
				best_cost = cost
				best_m = m
				best_b = b

	cost_attack(q,n,best_m,s,best_b, cost_svp = svp_classical, verbose = verbose)
	return (best_m,best_b,best_cost)




## Create a report on the best primal and dual BKZ attacks
def summarize_params(q,n,s, error_tol, security_only = True):
	print "Parameters : q = ", q, "n = ", n , " sigma^2 = ", s**2


	if not security_only:
		print
		std_dev = sqrt (4*(2* n * s**4 + s**2))
		print "final error std dev", std_dev
		tailcut = Decimal(error_tol / std_dev)
		print "tailcut = ", tailcut, "for error tolerance", error_tol
                print "Heuristic error proba per bit 2^", Decimal.ln(2* Decimal.exp(-tailcut**2 / 2))/Decimal.ln(Decimal(2))
                print "Heuristic error proba per exchange 2^", (Decimal.ln(Decimal(n/4)/Decimal.ln(Decimal(2)))  + (Decimal.ln(2* Decimal.exp(-tailcut**2 / 2))/Decimal.ln(Decimal(2))))
     
	k = n

	
	(m_pc,b_pc,c_pc) = optimize_attack(q,n,k,s, cost_attack = primal_cost, cost_svp = svp_classical, verbose = False)
	(m_pq,b_pq,c_pq) = optimize_attack(q,n,k,s, cost_attack = primal_cost, cost_svp = svp_quantum, verbose = False)
	(m_pp,b_pp,c_pp) = optimize_attack(q,n,k,s, cost_attack = primal_cost, cost_svp = svp_plausible, verbose = False)

	if (m_pc - m_pq)>1:
		print "m and b not equals among the three models"
	if (m_pq - m_pp)>1:
		print "m and b not equals among the three models"
	if (b_pc - b_pq)>1:
		print "m and b not equals among the three models"
	if (b_pq - b_pp)>1:
		print "m and b not equals among the three models"

	print "Primal ", "&", m_pq, "&", b_pq, "&", int(floor(c_pc)), "&", int(floor(c_pq))	, "&", int(floor(c_pp)) 


	(m_pc,b_pc,c_pc) = optimize_attack(q,n,k,s, cost_attack = dual_cost, cost_svp = svp_classical, verbose = False)
	(m_pq,b_pq,c_pq) = optimize_attack(q,n,k,s, cost_attack = dual_cost, cost_svp = svp_quantum, verbose = False)
	(m_pp,b_pp,c_pp) = optimize_attack(q,n,k,s, cost_attack = dual_cost, cost_svp = svp_plausible, verbose = False)

	if (m_pc - m_pq)>1:
		print "m and b not equals among the three models"
	if (m_pq - m_pp)>1:
		print "m and b not equals among the three models"
	if (b_pc - b_pq)>1:
		print "m and b not equals among the three models"
	if (b_pq - b_pp)>1:
		print "m and b not equals among the three models"

	print "Dual     ", "&", m_pq, "&", b_pq, "&", int(floor(c_pc)), "&", int(floor(c_pq))	, "&", int(floor(c_pp)) 




#print "### BCNS ###"
#summarize_params(2**32,1024,3.192, 2**32 /8)
#print

q = 12289
k = 24.0
print "### NewHope-512k24 ###"
summarize_params(q,512,sqrt(k/2), 3.*q/(4 * sqrt(2)), False)
print

q = 12289
k = 16.0
print "### NewHope-512k16 ###"
summarize_params(q,512,sqrt(k/2), 3.*q/(4 * sqrt(2)), False)
print

q = 12289
k = 8.0
error_tolerance = 3.*q/(4 * sqrt(2))
print "### NewHope-512k8 ###"
summarize_params(q,512,sqrt(k/2), error_tolerance, False)
print

print "### NewHope-1024k16 ###"
k = 16.0
#error_tolerance = 3.*q/4
error_tolerance = 4*(q/4)-3076.25
summarize_params(q,1024,sqrt(k/2), error_tolerance, False)
print

print "### NewHope-1024k8 ###"
k = 8.0
summarize_params(q,1024,sqrt(k/2), 3.*q/4, False)
print



