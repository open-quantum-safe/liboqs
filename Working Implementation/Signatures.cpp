#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <oqs/oqs.h>

/*
	algorithm: What algorithm the instance of the class is using
	public_key_length: Bytes of the public key
	private_key_length: Bytes of the private key
	signature_length: Bytes of the signature length

	generate_keypair(): Generate the public and private keys
	get_public_key(): Get the public key in a hex string
	get_private_key(): Get the private key in a hex string
	public_key: The public key as a char array
	private_key: The private key as a char array

	sign(message): Return the signature (unsigned char*) for a message (std::string)

	verify(message, signature): Return true or false for if a message and signature are valid
*/
class SignatureManager {
private:
	char* algorithm_char;
	OQS_SIG *sig;

public:
	std::string algorithm;
	unsigned int public_key_length, private_key_length, signature_length;
	unsigned char *public_key, *private_key;

	SignatureManager(std::string _algorithm) {
		algorithm = _algorithm;
		algorithm_char = const_cast<char*>(algorithm.c_str());
		
		if(OQS_SIG_alg_is_enabled(algorithm_char) == 0) {
			throw std::runtime_error("ERROR: Algorithm \"" + algorithm + "\" does not exist");
		}

		OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);
		sig = OQS_SIG_new(algorithm_char);

		public_key_length = sig->length_public_key;
		private_key_length = sig->length_secret_key;
		signature_length = sig->length_signature;

		public_key = (unsigned char*) malloc(public_key_length);
		private_key = (unsigned char*) malloc(private_key_length);
	}

	// Generate a public and private key pair
	void generate_keypair() {
		OQS_STATUS status = OQS_SIG_keypair(sig, public_key, private_key);
		if(status != OQS_SUCCESS) throw std::runtime_error("ERROR: OQS_SIG_keypair failed\n");
	}

	// Get the generated public key
	std::string get_public_key() {
		return bytes_to_hex(public_key, public_key_length);
	}

	// Get the generated private key
	std::string get_private_key() {
		return bytes_to_hex(private_key, private_key_length);
	}

	// Sign a message, returns its signature
	unsigned char* sign(std::string message) {
		unsigned char *signature = (unsigned char*) malloc(signature_length);
		unsigned int message_length = message.length();
		size_t *signature_len = (size_t*) &signature_length;
		uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_SIG_sign(sig, signature, signature_len, message_bytes, message_length, private_key);

		if (status != OQS_SUCCESS) throw std::runtime_error("ERROR: OQS_SIG_sign failed\n");

		return signature;
	}

	// Verify a signature
	bool verify(std::string message, unsigned char* signature) {
		unsigned int message_length = message.length();
		//size_t *signature_len = (size_t*) &signature_length;
		uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_SIG_verify(sig, message_bytes, message_length, signature, signature_length, public_key);

		return status == OQS_SUCCESS;
	}

	// Given an array of bytes, convert it to a hexadecimal string
	static std::string bytes_to_hex(unsigned char* bytes, int len) {
		constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		std::string output(len * 2, ' ');
		for (int i = 0; i < len; ++i) {
			output[2 * i] = hexmap[(bytes[i] & 0xF0) >> 4];
			output[2 * i + 1] = hexmap[bytes[i] & 0x0F];
		}
		return output;
	}
};



// Return the header row for the CSV file
std::string benchmarkLogHeader() {
	std::string row = "";
	row += "Name,";
	row += "Public Key Length (bytes),";
	row += "Private Key Length (bytes),";
	row += "Signature Length (bytes),";
	row += "Public+Signature (bytes),";
	row += "Number of samples,";
	row += "Sample input message,";
	row += "Initialization milliseconds,";
	row += "Key generation milliseconds,";
	row += "Signing milliseconds,";
	row += "Verifying milliseconds,";
	return row;
}

// Given an algorithm name, run a benchmark n times and return its CSV row
std::string benchmarkLog(std::string algorithm, int n) {
	std::string message = "Hello, World!";

	double clocks_initialization = 0;
	double clocks_keypair_generation = 0;
	double clocks_signing = 0;
	double clocks_verifying = 0;
	for(int i = 0; i < n; i++) {

		std::clock_t t1 = std::clock();
    		SignatureManager sigmanager(algorithm);
		std::clock_t t2 = std::clock();
			sigmanager.generate_keypair();
		std::clock_t t3 = std::clock();
			unsigned char* signature = sigmanager.sign(message);
		std::clock_t t4 = std::clock();
			bool result = sigmanager.verify(message, signature);
		std::clock_t t5 = std::clock();

		clocks_initialization += (t2 - t1);
		clocks_keypair_generation += (t3 - t2);
		clocks_signing += (t4 - t3);
		clocks_verifying += (t5 - t4);
	}

	clocks_initialization /= (double)n;
	clocks_keypair_generation /= (double)n;
	clocks_signing /= (double)n;
	clocks_verifying /= (double)n;

	double ms_initialization = clocks_initialization / (double)(CLOCKS_PER_SEC / 1000);
	double ms_keypair_generation = clocks_keypair_generation / (double)(CLOCKS_PER_SEC / 1000);
	double ms_signing = clocks_signing / (double)(CLOCKS_PER_SEC / 1000);
	double ms_verifying =  clocks_verifying / (double)(CLOCKS_PER_SEC / 1000);

	// Used to quickly grab the key and signature lengths
    SignatureManager sigmanager(algorithm);

 	std::cout << "Seconds: i-" << ms_initialization << "/k-" << clocks_keypair_generation << "/s-" << clocks_signing << "/v-" << clocks_verifying << " ms\t";
	std::cout << "Bytes: " << (sigmanager.public_key_length + sigmanager.signature_length) << "\t";
	std::cout << algorithm;
	std::cout << std::endl;


	std::string row = "";
	row += "\"" + algorithm + "\",";
	row += std::to_string(sigmanager.public_key_length) + ",";
	row += std::to_string(sigmanager.private_key_length) + ",";
	row += std::to_string(sigmanager.signature_length) + ",";
	row += std::to_string(sigmanager.public_key_length + sigmanager.signature_length) + ",";
	row += std::to_string(n) + ",";
	row += "\"" + message + "\",";
	row += std::to_string(ms_initialization) + ",";
	row += std::to_string(ms_keypair_generation) + ",";
	row += std::to_string(ms_signing) + ",";
	row += std::to_string(ms_verifying) + ",";
	return row;
}

int main(int argc, char** argv) {
	std::string fileName = "Algorithm_benchmark.csv";
	int numSamples = 0;

	std::ofstream outputFile;
	outputFile.open(fileName);

	outputFile << benchmarkLogHeader() << std::endl;
	
	std::cout << "Listing available algorithms:"<<"\n";

	// A list of all available algorithms
	const char *availAlgs[63]={"DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"};
    
    for (int i = 0; i <63; i++) {
    	std::string algorithm = availAlgs[i];
    	std::string row = benchmarkLog(algorithm, numSamples);
		outputFile << row << std::endl;
    }

    outputFile.close();
    std::cout << std::endl << "All data has been successfully saved to " << fileName << "!" << std::endl;

	//getting user choice for the algorithm
	std::string userChoice;
	std::cout << "Enter algorithm of choice: ";
	std::cin >> userChoice;

	std::string algorithm = userChoice;
	std::string message = "Hello, world!";

	std::cout << std::endl;

	std::cout << "Algorithm: " << algorithm << std::endl << std::endl;

	SignatureManager sigmanager(algorithm);
	sigmanager.generate_keypair();

	std::cout << "Public key (" << sigmanager.public_key_length << " bytes):" << std::endl << sigmanager.get_public_key() << std::endl;
	std::cout << std::endl;

	std::cout << "Private key (" << sigmanager.private_key_length << " bytes):" << std::endl << sigmanager.get_private_key() << std::endl;
	std::cout << std::endl;

	std::cout << "Signing message \"" << message << "\" to get signature (" << sigmanager.signature_length << " bytes)" << std::endl;

	unsigned char* signature = sigmanager.sign(message);

	// Print the signature
	//std::cout << sigmanager.bytes_to_hex(signature, sigmanager.signature_length) << std::endl;

	std::cout << std::endl;

	std::cout << "Verifying message and signature: ";

	//message[0]++; // Modify the message to fail verification
	//signature[0]++; // Modify the signature to fail verification

	bool result = sigmanager.verify(message, signature);
	if(result) std::cout << "SUCCESS" << std::endl;
	else std::cout << "FAILED" << std::endl;

	std::cout << std::endl;
	return 0;
}


/* The available algorithms are as follows:

"DEFAULT","DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"
*/