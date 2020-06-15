import oqs
import random
import base64

message = bytes(random.getrandbits(8) for _ in range (100))
sig = oqs.Signature("DILITHIUM_2")
dil2 = oqs.Signature("DILITHIUM_2")
dil3 = oqs.Signature("DILITHIUM_3")

def gen_dili2_private_key(dil2):
    pkey = dil2.export_secret_key()

    return pkey

