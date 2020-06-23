import oqs


global alg

alg = oqs.Signature("DILITHIUM_2")

def pqPublic():
    public=alg.generate_keypair()
    return public

def pqPrivate():
    private = alg.export_secret_key()
    return private

##print (pqPublic())
##print (pqPrivate())