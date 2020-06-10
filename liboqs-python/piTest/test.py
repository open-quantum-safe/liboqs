import oqs
import random

message = bytes(random.getrandbits(8) for _ in range (100))
sig = oqs.Signature("DILITHIUM_3")
publicKey = sig.generate_keypair()
privateKey = sig.export_secret_key()


print (publicKey)
print ()
print (privateKey)