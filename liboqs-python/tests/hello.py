import oqs
import random

print ("Custom test dilithium3")

message = bytes(random.getrandbits(8) for _ in range (100))
sig = oqs.Signature("DILITHIUM_3")
signature = sig.generate_keypair()
privateKey = sig.export_secret_key()

print (signature)
print ()
print (privateKey)