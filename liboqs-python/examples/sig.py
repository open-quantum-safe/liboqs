# signature Python example

from pprint import pprint
import oqs

#######################################################################
# signature example
#######################################################################

sigs = oqs.get_enabled_sig_mechanisms()

print("Enabled signature mechanisms:")
pprint(sigs, compact="True")

message = "This is the message to sign".encode()

# create signer and verifier with default signature mechanisms
sigalg = "DEFAULT"
with oqs.Signature(sigalg) as signer:
    with oqs.Signature(sigalg) as verifier:
        print("\nSignature details:")
        pprint(signer.details)

        # signer generates its keypair
        signer_public_key = signer.generate_keypair()
        # optionally, the secret key can be obtained by calling export_secret_key()
        # and the signer can later be re-instantiated with the key pair:
        # secret_key = signer.export_secret_key()
        # store key pair, wait... (session resumption):
        # signer = oqs.Signature(sigalg, secret_key)

        # signer signs the message
        signature = signer.sign(message)

        # verifier verifies the signature
        is_valid = verifier.verify(message, signature, signer_public_key)

        print("\nValid signature?", is_valid)
