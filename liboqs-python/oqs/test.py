import oqs
import random
import base64

message = bytes(random.getrandbits(8) for _ in range (100))
dil2 = oqs.Signature("DILITHIUM_2")


def gen_dili2_private_key(dil2):
    pubKey = dil2.generate_keypair()
    pKey = dil2.export_secret_key()
    
    return pKey

print (gen_dili2_private_key(dil2))
