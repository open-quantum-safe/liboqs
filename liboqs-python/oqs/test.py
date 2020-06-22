import oqs

##public = bytearray()

##def gen_dili2_private_key(dil2):
##    alg = oqs.Signature(dil2)
##    public = alg.generate_keypair()
##    pKey = alg.export_secret_key()
    
##    return pKey

##gen_dili2_private_key("DILITHIUM_2")
##print (public)

##def gen_dili2_public_key(public)

##    return public


# A Python program to return multiple  
# values from a method using class 
class Test: 
    def __init__(self): 
        self.private =" "
        self.public = " "
  
# This function returns an object of Test 
def gen_dili2_private_key(dil2):
    alg = oqs.Signature(dil2)
    self.public = alg.generate_keypair()
    self.private = alg.export_secret_key()
    print (self.public)
    print (self.private)
      
# Driver code to test above method 
t = Test(gen_dili2_private_key("DILITHIUM_2"))
##print(t.private) 
##print(t.public) 