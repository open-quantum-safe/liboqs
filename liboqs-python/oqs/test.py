import oqs
import random
import base64

message = bytes(random.getrandbits(8) for _ in range (100))
sig = oqs.Signature("DILITHIUM_2")
dil2 = oqs.Signature("DILITHIUM_2")
dil3 = oqs.Signature("DILITHIUM_3")

dil2_publicKey = dil2.generate_keypair()
dil2_privateKey = dil2.export_secret_key()
##publicKey = sig.generate_keypair()
##privateKey = sig.export_secret_key()

##print ("Printing algorithms")
##print (oqs._enabled_sigs)

##print ("current algorithm")
##print (oqs.alg_name)

##print (publicKey)
##print ()
##print (privateKey)
print (dil2_privateKey)

##with open("Pkey.txt", 'wb') as out:
##    out.write(privateKey)
