# key encapsulation Python example

from pprint import pprint
import oqs

#######################################################################
# KEM example
#######################################################################

kems = oqs.get_enabled_KEM_mechanisms()

print("Enabled KEM mechanisms:")
pprint(kems, compact="True")

# create client and server with default KEM mechanisms
kemalg = "DEFAULT"
with oqs.KeyEncapsulation(kemalg) as client:
    with oqs.KeyEncapsulation(kemalg) as server:
        print("\nKey encapsulation details:")
        pprint(client.details)

        # client generates its keypair
        public_key = client.generate_keypair()
        # optionally, the secret key can be obtained by calling export_secret_key()
        # and the client can later be re-instantiated with the key pair:
        # secret_key = client.export_secret_key()
        # store key pair, wait... (session resumption):
        # client = oqs.KeyEncapsulation(kemalg, secret_key)

        # the server encapsulates its secret using the client's public key
        ciphertext, shared_secret_server = server.encap_secret(public_key)

        # the client decapsulates the the server's ciphertext to obtain the shared secret
        shared_secret_client = client.decap_secret(ciphertext)

        print("\nShared secretes coincide:", shared_secret_client == shared_secret_server)
