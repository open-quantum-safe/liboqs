#include <iostream>
#include <string>
#include <oqs/oqs.h>

class Signature {
public:
	static std::string generate(char* algorithm) {
		OQS_SIG *sig = NULL;
		
		sig = OQS_SIG_new(algorithm);

		//return OQS_SIG_alg_is_enabled(algorithm) ? "true" : "false";
		//sig = OQS_SIG_new("Dilithium2");
		return "Hello World";
	}
};

int main() {
	/*std::string algorithms[] = {
		"Dilithium2",
		"Dilithium3",
		"Dilithium4",
		"Falcon-512",
		"Falcon-1024",
		"MQDSS-31-48",
		"MQDSS-31-64",
		"Picnic-L1-FS",
		"Picnic-L1-UR",
		"Picnic-L3-FS",
		"Picnic-L3-UR",
		"Picnic-L5-FS",
		"Picnic-L5-UR",
		"Picnic2-L1-FS",
		"Picnic2-L3-FS",
		"Picnic2-L5-FS",
		"qTesla-p-I",
		"qTesla-p-III",
		"Rainbow-Ia-Classic",
		"Rainbow-Ia-Cyclic",
		"Rainbow-Ia-Cyclic-Compressed",
		"Rainbow-IIIc-Classic†",
		"Rainbow-IIIc-Cyclic†",
		"Rainbow-IIIc-Cyclic-Compressed†",
		"Rainbow-Vc-Classic†",
		"Rainbow-Vc-Cyclic†",
		"Rainbow-Vc-Cyclic-Compressed†",
		"SPHINCS+-Haraka-128f-robust",
		"SPHINCS+-Haraka-128f-simple",
		"SPHINCS+-Haraka-128s-robust",
		"SPHINCS+-Haraka-128s-simple",
		"SPHINCS+-Haraka-192f-robust",
		"SPHINCS+-Haraka-192f-simple",
		"SPHINCS+-Haraka-192s-robust",
		"SPHINCS+-Haraka-192s-simple",
		"SPHINCS+-Haraka-256f-robust",
		"SPHINCS+-Haraka-256f-simple",
		"SPHINCS+-Haraka-256s-robust",
		"SPHINCS+-Haraka-256s-simple",
		"SPHINCS+-SHA256-128f-robust",
		"SPHINCS+-SHA256-128f-simple",
		"SPHINCS+-SHA256-128s-robust",
		"SPHINCS+-SHA256-128s-simple",
		"SPHINCS+-SHA256-192f-robust",
		"SPHINCS+-SHA256-192f-simple",
		"SPHINCS+-SHA256-192s-robust",
		"SPHINCS+-SHA256-192s-simple",
		"SPHINCS+-SHA256-256f-robust",
		"SPHINCS+-SHA256-256f-simple",
		"SPHINCS+-SHA256-256s-robust",
		"SPHINCS+-SHA256-256s-simple",
		"SPHINCS+-SHAKE256-128f-robust",
		"SPHINCS+-SHAKE256-128f-simple",
		"SPHINCS+-SHAKE256-128s-robust",
		"SPHINCS+-SHAKE256-128s-simple",
		"SPHINCS+-SHAKE256-192f-robust",
		"SPHINCS+-SHAKE256-192f-simple",
		"SPHINCS+-SHAKE256-192s-robust",
		"SPHINCS+-SHAKE256-192s-simple",
		"SPHINCS+-SHAKE256-256f-robust",
		"SPHINCS+-SHAKE256-256f-simple",
		"SPHINCS+-SHAKE256-256s-robust",
		"SPHINCS+-SHAKE256-256s-simple"
	};*/
	std::cout << std::endl;
	std::cout << Signature::generate("DILITHIUM2");
	std::cout << std::endl;
	std::cout << std::endl;
	return 0;
}