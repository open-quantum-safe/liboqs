/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: sampling functions
**************************************************************************************/

#include "api.h"
#include "sample.h"
#include "sha3/fips202.h"
#include "random/random.h"

#define round_double(x) (uint64_t)(x + 0.5)
#define NBLOCKS_SHAKE128 SHAKE128_RATE / (((PARAM_B_BITS + 1) + 7) / 8)

void sample_y(int64_t *y, const unsigned char *seed, int nonce) { // Sample polynomial y, such that each coefficient is in the range [-B,B]
	unsigned int i = 0, pos = 0, nblocks = PARAM_N;
	unsigned int nbytes = ((PARAM_B_BITS + 1) + 7) / 8;
	unsigned char buf[PARAM_N * nbytes];
	int16_t dmsp = (int16_t)(nonce << 8);

	cshake128_simple((uint8_t *) buf, PARAM_N * nbytes, dmsp++, seed, CRYPTO_RANDOMBYTES);

	while (i < PARAM_N) {
		if (pos >= nblocks * nbytes) {
			nblocks = NBLOCKS_SHAKE128;
			cshake128_simple((uint8_t *) buf, SHAKE128_RATE, dmsp++, seed, CRYPTO_RANDOMBYTES);
			pos = 0;
		}
		y[i] = (*(uint32_t *) (buf + pos)) & ((1 << (PARAM_B_BITS + 1)) - 1);
		y[i] -= PARAM_B;
		if (y[i] != (1 << PARAM_B_BITS))
			i++;
		pos += nbytes;
	}
}

static int64_t mod7(int64_t k) { // Compute k modulo 7
	int64_t i = k;

	for (int j = 0; j < 2; j++) {
		i = (i & 7) + (i >> 3);
	}
	// i <= 7 at this point. If (i == 7) return 0, else return i
	return ((i - 7) >> 3) & i;
}

static uint32_t Bernoulli(int64_t r, int64_t t) { // Sample a bit from Bernoulli
	                                              // Restriction: 15-bit exponent
	static const double exp[3][32] = {
	    {
	        1.000000000000000000000000000000000000000,
	        0.9991161986946362635440596569713669685880,
	        0.9982331784940198897326533185472850242681,
	        0.9973509387078094832208641647995024627420,
	        0.9964694786462737732901278365428682898391,
	        0.9955887976202910746192912304685682364684,
	        0.9947088949413487485322425364041275159470,
	        0.9938297699215426647216913224142898699467,
	        0.9929514218735766634486778457089421310313,
	        0.9920738501107620182173911392473163552182,
	        0.9911970539470168989248757955220583150316,
	        0.9903210326968658354852077402726923030103,
	        0.9894457856754391819277196598158280316812,
	        0.9885713121984725809688571162894359262234,
	        0.9876976115823064290572467553909520317038,
	        0.9868246831438853418915583811441525830737,
	        0.9859525262007576204107430418579502424629,
	        0.9850811400710747172562296407417980684195,
	        0.9842105240735907037056629536175321639825,
	        0.9833406775276617370777663048165281180240,
	        0.9824715997532455286079125206742780153835,
	        0.9816032900709008117939871480322018994711,
	        0.9807357478017868112121282928289778252539,
	        0.9798689722676627118019278012111954856228,
	        0.9790029627908871286206788726159970207714,
	        0.9781377186944175770662555609768519653065,
	        0.9772732393018099435682099865780080346346,
	        0.9764095239372179567466734471337520306711,
	        0.9755465719253926590386479823956917338692,
	        0.9746843825916818787912753119951161678163,
	        0.9738229552620297028216704313083937453701,
	        0.9729622892629759494429075148916109501561,
	    },
	    {
	        1.000000000000000000000000000000000000000,
	        0.9721023839216556419557461414665235406759,
	        0.9449830448261659815506003316741838244400,
	        0.9186202706410607262354282576433721865857,
	        0.8929929550089316248724185119772265808860,
	        0.8680805803894262140332557150199116840610,
	        0.8438632016326556552525292339254629390907,
	        0.8203214300108653339129866284361494061943,
	        0.7974364176955837811756247839373545193403,
	        0.7751898426678221357270891859654278174299,
	        0.7535638940492430676420382969418743828861,
	        0.7325412578425551199256826085929390870521,
	        0.7121051030697160541445567434958450232311,
	        0.6922390682968472772769625589791250165011,
	        0.6729272485350710325059754287002821864844,
	        0.6541541825067830050114340006988959714053,
	        0.6359048402671655658518413475133857442506,
	        0.6181646111710312869993190871292294307424,
	        0.6009192921753628362027560473439879450541,
	        0.5841550764681841229558144388281781547134,
	        0.5678585424146588316121044339920926624110,
	        0.5520166428115664537913591359859590337117,
	        0.5366166944415528229941896994764026030425,
	        0.5216463679187921574527288412861688503326,
	        0.5070936778179309248395866416383933681722,
	        0.4929469730784106413269415008349571859159,
	        0.4791949276764871892092025854239549577609,
	        0.4658265315574785583888478629564160135480,
	        0.4528310818209812590638301874088128310074,
	        0.4401981741519981827475211630454194703175,
	        0.4279176944911175684463420329167665844223,
	        0.4159798109370741180402885145593642305967,
	    },
	    {
	        1.000000000000000000000000000000000000000,
	        0.4043749658752093528844267111687024946352,
	        0.1635191130265767286466168121241953230561,
	        0.06612303575006646581251581957553699053968,
	        0.02673850032499837519017926564442803281999,
	        0.01081238015647549215912747014396255582252,
	        0.004372255856804567905094277088171797889884,
	        0.001768030812893031377541692153909093867261,
	        0.0007149473996299382156691604651236075335476,
	        0.0002891068303279259298899010575129069771848,
	        0.0001169075646481449883075929977115181069539,
	        4.727449246514746096480558381507760086106E-5,
	        1.911662127736184620584353997559933697984E-5,
	        7.730283076682497589319888445443844075884E-6,
	        3.125932955339193327950538126733860718160E-6,
	        1.264049032143478624120751312511989786231E-6,
	        5.111497842376105790435567936679525625580E-7,
	        2.066961765582044014739815034159084709228E-7,
	        8.358275934226015223807850299656474431551E-8,
	        3.379877545678228479840194532767858969538E-8,
	        1.366737867196019982379028600007217696759E-8,
	        5.526745784077469928250904895328215282900E-9,
	        2.234877637837284060764188149247765384222E-9,
	        9.037285685357202289540219994637413662239E-10,
	        3.654452090620836644843916087377680577752E-10,
	        1.477768939437388295908878759559360151466E-10,
	        5.975727644564382090472646695947245344300E-11,
	        2.416434662350267172894275660335977107355E-11,
	        9.771456841275623229028471594408844775750E-12,
	        3.951332526741911117382517682123044657845E-12,
	        1.597819955662865055885641210966604195704E-12,
	        6.461183900458995781949987131389237968733E-13,
	    },
	};

	// Compute the actual Bernoulli parameter c = exp(-t/f):
	double c = 4611686018427387904.0; // This yields a fraction of 2^62, to keep only 62 bits of precision in this implementation

	for (int64_t i = 0, s = t; i < 3; i++, s >>= 5) {
		c *= exp[i][s & 31];
	}
	// Sample from Bernoulli_c
	return (uint32_t)((uint64_t)((r & 0x3FFFFFFFFFFFFFFFLL) - round_double(c)) >> 63);
}

void sample_gauss_poly(int64_t *x, const unsigned char *seed, int nonce) { // Gaussian sampler
	static const int64_t cdt[12][2] = {
	    {0x0200000000000000LL, 0x0000000000000000LL},
	    {0x0300000000000000LL, 0x0000000000000000LL},
	    {0x0320000000000000LL, 0x0000000000000000LL},
	    {0x0321000000000000LL, 0x0000000000000000LL},
	    {0x0321020000000000LL, 0x0000000000000000LL},
	    {0x0321020100000000LL, 0x0000000000000000LL},
	    {0x0321020100200000LL, 0x0000000000000000LL},
	    {0x0321020100200100LL, 0x0000000000000000LL},
	    {0x0321020100200100LL, 0x0200000000000000LL},
	    {0x0321020100200100LL, 0x0200010000000000LL},
	    {0x0321020100200100LL, 0x0200010000200000LL},
	    {0x0321020100200100LL, 0x0200010000200001LL},
	};

	unsigned char seed_ex[PARAM_N * 8];
	int64_t i, j = 0, x_ind;
	int64_t *buf = (int64_t *) seed_ex;
	int64_t sign, k, l, bitsremained, rbits, y, z;
	uint64_t r, s, c_lo, c_hi;
	int16_t dmsp = (int16_t)(nonce << 8);

	cshake128_simple(seed_ex, PARAM_N * 8, dmsp++, seed, CRYPTO_RANDOMBYTES);

	for (x_ind = 0; x_ind < PARAM_N; x_ind++) {
		if ((j + 46) > (PARAM_N)) {
			cshake128_simple((uint8_t *) buf, PARAM_N * 8, dmsp++, seed, CRYPTO_RANDOMBYTES);
			j = 0;
		}
		do {
			rbits = buf[j++];
			bitsremained = 64;
			do {
				// Sample x from D^+_{\sigma_2} and y from U({0, ..., k-1}):
				do {
					r = buf[j++];
					s = buf[j++];
					if (bitsremained <= 64 - 6) {
						rbits = (rbits << 6) ^ ((r >> 58) & 63);
						bitsremained += 6;
					}
					r &= 0x03FFFFFFFFFFFFFFLL;
				} while (r > 0x0321020100200100LL); // Checking if r exceeds a maximum value. Variation is random and does not depend on private data
				y = 0;
				for (i = 0; i < 12; i++) {
					c_lo = s - cdt[i][1];
					uint64_t b = (((c_lo & cdt[i][1]) & 1) + (cdt[i][1] >> 1) + (c_lo >> 1)) >> 63;
					c_hi = r - (cdt[i][0] + b);
					y += ~(c_hi >> (63)) & 1LL;
				}
				// The next sampler works exclusively for PARAM_Xi <= 28
				do {
					do {
						if (bitsremained < 6) {
							rbits = buf[j++];
							bitsremained = 64;
						}
						z = rbits & 63;
						rbits >>= 6;
						bitsremained -= 6;
					} while (z == 63);
					if (bitsremained < 2) {
						rbits = buf[j++];
						bitsremained = 64;
					}
					z = (mod7(z) << 2) + (rbits & 3);
					rbits >>= 2;
					bitsremained -= 2;
				} while (z >= PARAM_Xi); // Making sure random z does not exceed a certain limit. No private data leaked, it varies uniformly
				k = PARAM_Xi * y + z;
				// Sample a bit from Bernoulli_{exp(-y*(y + 2*k*x)/(2*k^2*sigma_2^2))}
			} while (Bernoulli(buf[j++], z * ((k << 1) - z)) == 0);
			// Put last randombits into sign bit
			rbits <<= (64 - bitsremained);
			if (bitsremained == 0LL) {
				rbits = buf[j++];
				bitsremained = 64;
			}
			sign = rbits >> 63;
			rbits <<= 1;
			bitsremained--;
		} while ((k | (sign & 1)) == 0);
		if (bitsremained == 0LL) {
			rbits = buf[j++];
			bitsremained = 64;
		}
		sign = rbits >> 63;
		rbits <<= 1;
		bitsremained--;
		k = ((k << 1) & sign) - k;
		x[x_ind] = (k << 48) >> 48;
	}
}

void encode_c(uint32_t *pos_list, int16_t *sign_list, unsigned char *c_bin) { // Encoding of c' by mapping the output of the hash function H to an N-element vector with entries {-1,0,1}
	int i, pos, cnt = 0;
	int16_t c[PARAM_N];
	const int RLENGTH = SHAKE128_RATE;
	unsigned char r[RLENGTH];
	uint16_t dmsp = 0;

	// Use the hash value as key to generate some randomness
	cshake128_simple(r, RLENGTH, dmsp++, c_bin, CRYPTO_RANDOMBYTES);

	// Use rejection sampling to determine positions to be set in the new vector
	for (i = 0; i < PARAM_N; i++)
		c[i] = 0;

	for (i = 0; i < PARAM_W;) { // Sample a unique position k times. Use two bytes
		if (cnt > (RLENGTH - 3)) {
			cshake128_simple(r, RLENGTH, dmsp++, c_bin, CRYPTO_RANDOMBYTES);
			cnt = 0;
		}
		pos = (r[cnt] << 8) | (r[cnt + 1]);
		pos = pos & (PARAM_N - 1); // Position is in the range [0,N-1]

		if (c[pos] == 0) { // Position has not been set yet. Determine sign
			if ((r[cnt + 2] & 1) == 1)
				c[pos] = -1;
			else
				c[pos] = 1;
			pos_list[i] = pos;
			sign_list[i] = c[pos];
			i++;
		}
		cnt += 3;
	}
}
