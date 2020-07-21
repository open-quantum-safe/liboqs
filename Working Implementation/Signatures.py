import oqs

available_algorithms = [
	'DILITHIUM_2',
	'DILITHIUM_3',
	'DILITHIUM_4',
	'Falcon-512',
	'Falcon-1024',
	'MQDSS-31-48',
	'MQDSS-31-64',
	'picnic-L1-FS',
	'picnic-L1-UR',
	'picnic-L3-FS',
	'picnic-L3-UR',
	'picnic-L5-FS',
	'picnic-L5-UR',
	'picnic2-L1-FS',
	'picnic2-L3-FS',
	'picnic2-L5-FS',
	'qTesla-p-I',
	'qTesla-p-III',
	'Rainbow-Ia-Classic',
	'Rainbow-Ia-Cyclic',
	'Rainbow-Ia-Cyclic-Compressed',
	'Rainbow-IIIc-Classic†',
	'Rainbow-IIIc-Cyclic†',
	'Rainbow-IIIc-Cyclic-Compressed†',
	'Rainbow-Vc-Classic†',
	'Rainbow-Vc-Cyclic†',
	'Rainbow-Vc-Cyclic-Compressed†',
	'SPHINCS+-Haraka-128f-robust',
	'SPHINCS+-Haraka-128f-simple',
	'SPHINCS+-Haraka-128s-robust',
	'SPHINCS+-Haraka-128s-simple',
	'SPHINCS+-Haraka-192f-robust',
	'SPHINCS+-Haraka-192f-simple',
	'SPHINCS+-Haraka-192s-robust',
	'SPHINCS+-Haraka-192s-simple',
	'SPHINCS+-Haraka-256f-robust',
	'SPHINCS+-Haraka-256f-simple',
	'SPHINCS+-Haraka-256s-robust',
	'SPHINCS+-Haraka-256s-simple',
	'SPHINCS+-SHA256-128f-robust',
	'SPHINCS+-SHA256-128f-simple',
	'SPHINCS+-SHA256-128s-robust',
	'SPHINCS+-SHA256-128s-simple',
	'SPHINCS+-SHA256-192f-robust',
	'SPHINCS+-SHA256-192f-simple',
	'SPHINCS+-SHA256-192s-robust',
	'SPHINCS+-SHA256-192s-simple',
	'SPHINCS+-SHA256-256f-robust',
	'SPHINCS+-SHA256-256f-simple',
	'SPHINCS+-SHA256-256s-robust',
	'SPHINCS+-SHA256-256s-simple',
	'SPHINCS+-SHAKE256-128f-robust',
	'SPHINCS+-SHAKE256-128f-simple',
	'SPHINCS+-SHAKE256-128s-robust',
	'SPHINCS+-SHAKE256-128s-simple',
	'SPHINCS+-SHAKE256-192f-robust',
	'SPHINCS+-SHAKE256-192f-simple',
	'SPHINCS+-SHAKE256-192s-robust',
	'SPHINCS+-SHAKE256-192s-simple',
	'SPHINCS+-SHAKE256-256f-robust',
	'SPHINCS+-SHAKE256-256f-simple',
	'SPHINCS+-SHAKE256-256s-robust',
	'SPHINCS+-SHAKE256-256s-simple'
]

# Given an algorithm (listed above), generate a public and private key pair
def generate(algorithm):
	if algorithm not in available_algorithms:
		raise 'Error: Invalid algorithm used'

	algorithm = oqs.Signature(algorithm)
	public_key = algorithm.generate_keypair()
	private_key = algorithm.export_secret_key()

	return {
		'public': public_key,
		'private': private_key,
	}


# Testing code
userAlg = input("Choose PQ algorithm: ")
result = generate(userAlg)
print('Public key:')
Pbytes = result['public']
print(Pbytes.hex())
print()
print('Private key:')
Prbytes = result['private']
print (Prbytes.hex())