from math import *
import operator as op
import numpy as np

dim  = 512
k = 8
q = 12289
bound_CC = 34000    # Bound n*mu+beta to use in Cramer-Chernoff Ineq.
n = 2 * dim / 2      # Number of samples in Cramer-Chernoff Ineq.


check_each_y = True 
# Check each distribution phi_y (slower)
#experimentally, they are always equal probably due to symmetries


subg_param = sqrt(k/2)
tau = 17.6


subg_value = sqrt(bound_CC) * subg_param * tau
p_subg = exp(-tau**2/2 )

print "#### "
print "#### Correctness check :"
print "#### "

print "target bound (before +e'')=", subg_value
print "target bound =", subg_value + 2*k
print "(sqrt(2)/2.) * 3q/4 =", (sqrt(2)/2.) * 3*q/4.
print  "Correct ?", subg_value + 2*k < (sqrt(2)/2.) * 3*q/4.
assert subg_value + 2*k < (sqrt(2)/2.) * 3*q/4.


print
print "#### "
print "#### Lemma 6 Verification :"
print "#### "


def rot(v):
	return [-v[1],v[0]]

if check_each_y:
	y_list = [
		[ 1, 1],
		[ 1,-1],
		[-1, 1],
		[-1,-1]
	]
else:
	y_list = [[ 1, 1]]

### Construct the binomial law
supp = xrange(-k,k+1)

def binomial(n, r):
    r = min(r, n-r)
    if r == 0: return 1
    numer = reduce(op.mul, xrange(n, n-r, -1))
    denom = reduce(op.mul, xrange(1, r+1))
    return numer//denom

def pdf_binom(x):
	return binomial(2*k,x+k) / 2.**(2*k)





# Union bound on each application of Chernoff-Cramer
Union_CC = 0. 

for y in y_list:
	print "y=",y
	### Construct a 4*4 matrix associated to y
	v1 = y
	v2 = rot(v1)
	mat = np.matrix([v1,v2])


	### Initialize a table to store the pdf of \varphi_y
	### the range of the above distribution
	ima = xrange(4 * (k*4)**2 )
	varphi_y_Table = [0 for i in ima]

	### Brute force computation of the pdf of || x*y ||^2
	s = 0.
	for x1 in supp:
		for x2 in supp:
			x = np.array([x1,x2])
			mx = x * mat
			s = mx[0,0]**2 + mx[0,1]**2
			p = pdf_binom(x1) * pdf_binom(x2)
			varphi_y_Table[s] += p                   ### The result is always a multiple of 4

	avg = sum([i*varphi_y_Table[i] for i in ima])
	print "varphi_y has average ",avg

	def Moment(t):
		return sum([varphi_y_Table[i] * exp(t*(i - avg)) for i in range(len(ima))])



	def ChernoffCramer(n,t,bound):
		beta = bound - n*avg
		p =  exp(- t*beta + n*log(Moment(t)))
		return p

	t_CC = 0.0060
	p_CC = ChernoffCramer(n,t_CC,bound_CC)

	print "Chernoff-Cramer Bound", p_CC, " = 2^", log(p_CC)/log(2)
	
	if check_each_y:
		Union_CC += p_CC


else:
	Union_CC = 4*p_CC



print
print "#### "
print "#### Conclusion of Lemma 6 :"
print "#### "

print "with parameter k=",k, "and parameter n = ",n,"(v has dimension 4n)"
print "The bound ||v||_2^2 <=", bound_CC, " fails with probability less than 2^", log(Union_CC)/log(2)

print "#### "
print "#### Corollary 1 :"
print "#### "

print "subgaussian parameter sigma = ", subg_param
print "tailcut parameter tau = ", tau
print "tailcut value ||v||* sigma * tau = ", subg_value

print "sub-gaussian tail-bound (Lemma 5): 2^", log(p_subg) /log(2)
print "(failure proba per bit-agreement and per y)"
print
print "#### "
print "#### Conclusion of Corollary 1 :"
print "#### "
print "Partial bound ||(es'-e's )_i||_1 <=", subg_value

print "Final bound ||(es'-e's +e'')_i||_1 <=", subg_value + 2*k
print "fails with probability at most 2^",
print log(Union_CC + 4 * 256 * p_subg)/log(2)

