#include <stdio.h>
#if defined(_WIN32)
#include <windows.h>
#include <Wincrypt.h>
#include <assert.h>
#define strcasecmp _stricmp
#else
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#endif
#if defined(HAVE_GETENTROPY)
#include <sys/random.h>
#endif
#include <fcntl.h>

#include <oqs/oqs.h>

void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read);
void OQS_randombytes_nist_kat(uint8_t *random_array, size_t bytes_to_read);

#ifdef USE_OPENSSL
#include <openssl/rand.h>
// Use OpenSSL's RAND_bytes as the default PRNG
static void (*oqs_randombytes_algorithm)(uint8_t *, size_t) = (void (*)(uint8_t *, size_t)) & RAND_bytes;
#else
static void (*oqs_randombytes_algorithm)(uint8_t *, size_t) = &OQS_randombytes_system;
#endif

OQS_STATUS OQS_randombytes_switch_algorithm(const char *algorithm) {
	if (0 == strcasecmp(OQS_RAND_alg_system, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_system;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_nist_kat, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_nist_kat;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_openssl, algorithm)) {
#ifdef USE_OPENSSL
		oqs_randombytes_algorithm = (void (*)(uint8_t *, size_t)) & RAND_bytes;
		return OQS_SUCCESS;
#else
		return OQS_ERROR;
#endif
	} else {
		return OQS_ERROR;
	}
}

void OQS_randombytes_custom_algorithm(void (*algorithm_ptr)(uint8_t *, size_t)) {
	oqs_randombytes_algorithm = algorithm_ptr;
}

void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	oqs_randombytes_algorithm(random_array, bytes_to_read);
}

static __inline void delay(unsigned int count) {
	while (count--) {
	}
}

#if !defined(_WIN32)
#if defined(HAVE_GETENTROPY)
void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read) {

	int rc;
	do {
		rc = getentropy(random_array, bytes_to_read);
	} while (rc != 0);
}
#else
void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read) {

	FILE *handle;
	do {
		handle = fopen("/dev/urandom", "rb");
		if (handle == NULL) {
			delay(0xFFFFF);
		}
	} while (handle == NULL);

	int bytes_last_read, bytes_total_read, bytes_left_to_read;
	bytes_total_read = 0;
	bytes_left_to_read = bytes_to_read;
	while (bytes_left_to_read > 0) {
		do {
			bytes_last_read = fread(random_array + bytes_total_read, 1, bytes_left_to_read, handle);
			if (bytes_last_read <= 0) {
				delay(0xFFFF);
			}
		} while (bytes_last_read <= 0);
		bytes_total_read += bytes_last_read;
		bytes_left_to_read -= bytes_last_read;
	}
	fclose(handle);
}
#endif
#else
void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read) {
	HCRYPTPROV hCryptProv;
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ||
	    !CryptGenRandom(hCryptProv, (DWORD) bytes_to_read, random_array)) {
		assert(0); // no other way to return an error; better fail than return bad random data
	}
	CryptReleaseContext(hCryptProv, 0);
}
#endif

/************************************************************
 *** START DEPRECATED CODE *** expected removal Aug. 2018 ***
 ************************************************************/

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <oqs/rand_urandom_aesctr.h>
#include <oqs/rand_urandom_chacha20.h>

OQS_RAND *OQS_RAND_new(enum OQS_RAND_alg_name alg_name) {
	switch (alg_name) {
	case OQS_RAND_alg_default:
	case OQS_RAND_alg_urandom_chacha20:
		return OQS_RAND_urandom_chacha20_new();
	case OQS_RAND_alg_urandom_aesctr:
		return OQS_RAND_urandom_aesctr_new();
	default:
		assert(0);
		return NULL; // avoid the warning of potentialy uninitialized variable in VS
	}
}

uint8_t OQS_RAND_8(OQS_RAND *r) {
	return r->rand_8(r);
}

uint32_t OQS_RAND_32(OQS_RAND *r) {
	return r->rand_32(r);
}

uint64_t OQS_RAND_64(OQS_RAND *r) {
	return r->rand_64(r);
}

void OQS_RAND_n(OQS_RAND *r, uint8_t *out, size_t n) {
	r->rand_n(r, out, n);
}

void OQS_RAND_free(OQS_RAND *r) {
	if (r) {
		r->free(r); // IGNORE free-check
	}
}

#if !defined(_WIN32)
/* For some reason specifying inline results in a build error */
inline
#endif
    void
    OQS_RAND_test_record_occurrence(const unsigned char b, unsigned long occurrences[256]) {
	occurrences[b] += 1;
}

double OQS_RAND_test_statistical_distance_from_uniform(const unsigned long occurrences[256]) {

	// compute total number of samples
	unsigned long total = 0;
	for (int i = 0; i < 256; i++) {
		total += occurrences[i];
	}

	// compute statistical distance from uniform
	// SD(X,Y) = 1/2 \sum_z | Pr[X=z] - Pr[Y=z] |
	//         = 1/2 \sum_z | 1/256   - Pr[Y=z] |
	double distance = 0.0;
	for (int i = 0; i < 256; i++) {
		distance += fabs(1.0 / 256.0 - (double) occurrences[i] / (double) total);
	}
	distance /= 2.0;

	return distance;
}

// Even for a perfectly uniform generator, if the number of samples is
// low then the std dev of the counts will be high.  So, instead, whilst
// still assuming the number of samples isn't super-low, we calculate an
// approximate Chi-squared statistic and back-convert to the Normal
// distribution.  The number of sigmas is reported: -3 to +3 is pretty
// ordinary, big negative is suspiciously-flat counts, big positive is
// wildly-fluctuating counts.
double OQS_RAND_zscore_deviation_from_uniform(const unsigned long occurrences[256]) {
	double quantiles[102] = {
	    156.7872, 158.4155, 160.0555, 161.7072, 163.3707, 165.0460, 166.7331, 168.4321,
	    170.1430, 171.8658, 173.6006, 175.3475, 177.1064, 178.8773, 180.6604, 182.4557,
	    184.2631, 186.0828, 187.9147, 189.7589, 191.6155, 193.4844, 195.3657, 197.2594,
	    199.1656, 201.0843, 203.0155, 204.9593, 206.9157, 208.8847, 210.8663, 212.8607,
	    214.8678, 216.8877, 218.9203, 220.9658, 223.0241, 225.0953, 227.1794, 229.2765,
	    231.3866, 233.5096, 235.6457, 237.7949, 239.9572, 242.1326, 244.3212, 246.5230,
	    248.7380, 250.9663, 253.2079, 255.4627, 257.7310, 260.0126, 262.3076, 264.6160,
	    266.9379, 269.2733, 271.6222, 273.9846, 276.3607, 278.7503, 281.1536, 283.5705,
	    286.0011, 288.4454, 290.9035, 293.3754, 295.8610, 298.3605, 300.8739, 303.4011,
	    305.9422, 308.4973, 311.0663, 313.6493, 316.2463, 318.8574, 321.4825, 324.1217,
	    326.7751, 329.4426, 332.1242, 334.8201, 337.5301, 340.2544, 342.9930, 345.7459,
	    348.5131, 351.2947, 354.0906, 356.9009, 359.7256, 362.5648, 365.4184, 368.2866,
	    371.1692, 374.0664, 376.9782, 379.9045, 382.8454, 385.8010}; // -5.05 to +5.05 sigma: qchisq(pnorm(seq(-5.05,5.05,length.out=102)),255)
	unsigned long total;
	double chsq;
	int i;

	for (total = i = 0; i < 256; i++) {
		total += occurrences[i];
	}
	if (total / 256. < 5) {
		return ZSCORE_SPARSE;
	}

	for (chsq = i = 0; i < 256; i++) {
		chsq += pow(occurrences[i] - total / 256., 2) * 256. / total;
	}

	if (chsq <= quantiles[0]) {
		return ZSCORE_BIGNEG;
	}
	for (i = 1; i < 102; i++) {
		if (chsq <= quantiles[i]) {
			return (i - 51) / 10.0;
		}
	}
	return ZSCORE_BIGPOS;
}
//
// convenience function for statistics reporting
void OQS_RAND_report_statistics(const unsigned long occurrences[256], const char *indent) {
	double zscore = OQS_RAND_zscore_deviation_from_uniform(occurrences);
	printf("%sStatistical distance from uniform: %12.10f\n", indent, OQS_RAND_test_statistical_distance_from_uniform(occurrences));
	printf("%s   Z-score deviation from uniform: ", indent);
	if (zscore == ZSCORE_BIGNEG) {
		printf("less than -5.0 sigma ***\n");
	} else if (zscore == ZSCORE_BIGPOS) {
		printf("more than +5.0 sigma ***\n");
	} else if (zscore == ZSCORE_SPARSE) {
		printf("(too few data)\n");
	} else {
		printf("about %.1f sigma\n", zscore);
	}
	return;
}

OQS_STATUS OQS_RAND_get_system_entropy(uint8_t *buf, size_t n) {
	OQS_STATUS result = OQS_ERROR;

#if !defined(_WIN32)
	int fd = 0;
#endif

	if (!buf) {
		goto err;
	}

#if defined(_WIN32)
	HCRYPTPROV hCryptProv;
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ||
	    !CryptGenRandom(hCryptProv, (DWORD) n, buf)) {
		goto err;
	}
#else
	fd = open("/dev/urandom", O_RDONLY);
	if (fd <= 0) {
		goto err;
	}
	size_t r = read(fd, buf, n);
	if (r != n) {
		goto err;
	}
#endif
	result = OQS_SUCCESS;

err:
#if !defined(_WIN32)
	if (fd > 0) {
		close(fd);
	}
#endif

	return result;
}

/***********************************************************
 *** STOP DEPRECATED CODE *** expected removal Aug. 2018 ***
 ***********************************************************/
