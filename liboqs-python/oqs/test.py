import oqs
import random
import base64


def gen_dili2_private_key(dil2):
    alg = oqs.Signature(dil2)
    pubKey = alg.generate_keypair()
    pKey = alg.export_secret_key()
    
    return pKey

print (gen_dili2_private_key("DILITHIUM_2"))