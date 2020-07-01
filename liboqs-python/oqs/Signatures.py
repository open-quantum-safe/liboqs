import oqs

def generate(algorithm):
	algorithm = oqs.Signature(algorithm)
	public_key = algorithm.generate_keypair()
	private_key = algorithm.export_secret_key()
	return {
		'public': public_key,
		'private': private_key,
	}

'''
class Signature:
	def __init__(self):
		self.public_key = ''
		self.private_key = ''
		self.algorithm = None

	def generate(self, algorithm):
		self.algorithm = oqs.Signature(algorithm)
		self.public_key = self.algorithm.generate_keypair()
		self.private_key = self.algorithm.export_secret_key()
		return {
			'public': self.public_key,
			'private': self.private_key,
		}
'''


result = generate('SPHINCS+-Haraka-128s-simple')
print('Public key:')
print(result['public'])
print()
print('Private key:')
print(result['private'])