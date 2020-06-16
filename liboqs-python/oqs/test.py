import oqs


def gen_dili2_private_key(dil2):
    alg = oqs.Signature(dil2)
    public = alg.generate_keypair()
    pKey = alg.export_secret_key()
    
    return pKey

