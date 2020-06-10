import oqs
import random

message = bytes(random.getrandbits(8) for _ in range (100))
sig = oqs.Signature("DILITHIUM_3")
publicKey = sig.generate_keypair()
privateKey = sig.export_secret_key()

##print ("Printing algorithms")
##print (oqs._enabled_sigs)

print ("current algorithm")
print (oqs.Signature.details)

print (publicKey)
print ()
print (privateKey)