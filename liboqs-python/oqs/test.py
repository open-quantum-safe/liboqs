import oqs

public = bytearray()

def gen_dili2_private_key(dil2):
    alg = oqs.Signature(dil2)
    public = alg.generate_keypair()
    pKey = alg.export_secret_key()
    
    return pKey

def gen_dili2_public_key(public)

    return public