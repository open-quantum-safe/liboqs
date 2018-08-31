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
#define NBLOCKS_SHAKE256 SHAKE256_RATE / (((PARAM_B_BITS + 1) + 7) / 8)

void sample_y(int64_t *y, const unsigned char *seed, int nonce) { // Sample polynomial y, such that each coefficient is in the range [-B,B]
	unsigned int i = 0, pos = 0, nblocks = PARAM_N;
	unsigned int nbytes = ((PARAM_B_BITS + 1) + 7) / 8;
	unsigned char buf[PARAM_N * nbytes];
	int16_t dmsp = (int16_t)(nonce << 8);

	cshake256_simple((uint8_t *) buf, PARAM_N * nbytes, dmsp++, seed, CRYPTO_RANDOMBYTES);

	while (i < PARAM_N) {
		if (pos >= nblocks * nbytes) {
			nblocks = NBLOCKS_SHAKE256;
			cshake256_simple((uint8_t *) buf, SHAKE256_RATE, dmsp++, seed, CRYPTO_RANDOMBYTES);
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
	                                              // Restriction: 20-bit exponent
	static const double exp[3][32] = {
	    {
	        1.000000000000000000000000000000000000000,
	        0.9951980443443537316500388424172839303752,
	        0.9904191474668262564830185894967173613892,
	        0.9856631986401875746675941557587114196642,
	        0.9809300876689149347041557365309129923940,
	        0.9762197048866395987965541168345276706016,
	        0.9715319411536058687432894158212596709598,
	        0.9668666878541423134736924881553750396380,
	        0.9622238368941451396373408016639000521875,
	        0.9576032806985736469363056351479270970296,
	        0.9530049122089577101698314104664824876542,
	        0.9484286248809172302397073765744987564880,
	        0.9438743126816934966419131566675496907225,
	        0.9393418700876924042461092785035073150884,
	        0.9348311920820394674392081270253399758265,
	        0.9303421741521465749826061515830447550861,
	        0.9258747122872904292046909607697858626672,
	        0.9214287029762026134209634491584644007645,
	        0.9170040432046712317435415947941667461407,
	        0.9126006304531540657099452867877830194818,
	        0.9082183626944031924279067014123113094560,
	        0.9038571383911010091985145255388756529519,
	        0.8995168564935076098442888811876009946339,
	        0.8951974164371194582318032579854959087286,
	        0.8908987181403393047402262055905414183192,
	        0.8866206620021572916876550405654798379036,
	        0.8823631488998431939863624175501337704454,
	        0.8781260801866497415560803096876886684788,
	        0.8739093576895269702812107160640808580937,
	        0.8697128837068475485533842136704059167642,
	        0.8655365610061430266950922187780245940470,
	        0.8613802928218509568132024098758678171240,
	    },
	    {
	        1.000000000000000000000000000000000000000,
	        0.8572439828530728308830350554160731167033,
	        0.7348672461377994256921043490917256989374,
	        0.6299605249474365823836053036391959460526,
	        0.5400298694461530849364654156443919196999,
	        0.4629373561436452146023454803849830673216,
	        0.3968502629920498686879264098181800898090,
	        0.3401975000435942410639200938313063115835,
	        0.2916322598940291452234231586652679613355,
	        0.2500000000000000000000000000000973522517,
	        0.2143109957132682077207587638541017338078,
	        0.1837168115344498564230260872730029657154,
	        0.1574901312368591455959013259098603145887,
	        0.1350074673615382712341163539111505530487,
	        0.1157343390359113036505863700962908348244,
	        0.09921256574801246717198160245458365671892,
	        0.08504937501089856026598002345785969688852,
	        0.07290806497350728630585578966634538139103,
	        0.06250000000000000000000000000004867612583,
	        0.05357774892831705193018969096354629710993,
	        0.04592920288361246410575652181826862667413,
	        0.03937253280921478639897533147748041066606,
	        0.03375186684038456780852908847780078154311,
	        0.02893358475897782591264659252408397570460,
	        0.02480314143700311679299540061365557274640,
	        0.02126234375272464006649500586447320397029,
	        0.01822701624337682157646394741659344311205,
	        0.01562500000000000000000000000001825354719,
	        0.01339443723207926298254742274089179019198,
	        0.01148230072090311602643913045457162797985,
	        0.009843133202303696599743832869373935671238,
	        0.008437966710096141952132272119453481206014,
	    },
	    {
	        1.000000000000000000000000000000000000000,
	        0.007233396189744456478161648131023810675775,
	        5.232202043780962102557587008169005410143E-5,
	        3.784659032745836912993682954976324658164E-7,
	        2.737593822694567686662466634421542264066E-9,
	        1.980210072614684707158711353745069372717E-11,
	        1.432364399414465384287735340977513952565E-13,
	        1.036085918905020069841154248521752033776E-15,
	        7.494419938055456100418425186702743722723E-18,
	        5.421010862427522170037264004417260251684E-20,
	        3.921231931684654880817938739668273317360E-22,
	        2.836382411375207747860568187463889509638E-24,
	        2.051667772709962123314993704273413823620E-26,
	        1.484052584974173558955043468582713624191E-28,
	        1.073474031353259824558654154333806911547E-30,
	        7.764862968180290824468612020607860317513E-33,
	        5.616633020792314645332222710264644852793E-35,
	        4.062733189179202535382045195211707654781E-37,
	        2.938735877055718769921841343128853888538E-39,
	        2.125704089576016965228859756656407540404E-41,
	        1.537605986206336992222535387300608525931E-43,
	        1.112211328195318530448364746285024038827E-45,
	        8.045065183558638234146057828832053516826E-48,
	        5.819314384499884015403474144560288801662E-50,
	        4.209340649576656799996170991423257963815E-52,
	        3.044782861598424467581974062513986546956E-54,
	        2.202412074968526631812431321732133496007E-56,
	        1.593091911132452277028880397827266782094E-58,
	        1.152346495989819456843455045622426762614E-60,
	        8.335378753358135655955994470664225877261E-63,
	        6.029309691461763611680553229574282672923E-65,
	        4.361238574900884540660050746922306538111E-67,
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
	static const int64_t cdt[14][3] = {
	    {0x0000020000000000LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000030000000000LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000032000000000LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000032100000000LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000032102000000LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000032102010000LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000032102010020LL, 0x0000000000000000LL, 0x0000000000000000LL},
	    {0x0000032102010020LL, 0x0100000000000000LL, 0x0000000000000000LL},
	    {0x0000032102010020LL, 0x0100020000000000LL, 0x0000000000000000LL},
	    {0x0000032102010020LL, 0x0100020001000000LL, 0x0000000000000000LL},
	    {0x0000032102010020LL, 0x0100020001000020LL, 0x0000000000000000LL},
	    {0x0000032102010020LL, 0x0100020001000020LL, 0x0001000000000000LL},
	    {0x0000032102010020LL, 0x0100020001000020LL, 0x0001000002000000LL},
	    {0x0000032102010020LL, 0x0100020001000020LL, 0x0001000002000001LL},
	};

	unsigned char seed_ex[PARAM_N * 8];
	int64_t i, j = 0, x_ind;
	int64_t *buf = (int64_t *) seed_ex;
	int64_t sign, k, l, bitsremained, rbits, y, z;
	uint64_t r, s, t, c;
	int16_t dmsp = (int16_t)(nonce << 8);

	cshake256_simple(seed_ex, PARAM_N * 8, dmsp++, seed, CRYPTO_RANDOMBYTES);

	for (x_ind = 0; x_ind < PARAM_N; x_ind++) {
		if ((j + 46) > (PARAM_N)) {
			cshake256_simple((uint8_t *) buf, PARAM_N * 8, dmsp++, seed, CRYPTO_RANDOMBYTES);
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
					t = buf[j++];
					if (bitsremained <= 64 - 6) {
						rbits = (rbits << 6) ^ ((r >> 58) & 63);
						bitsremained += 6;
					}
					r &= 0x000003FFFFFFFFFFLL;
				} while (r > 0x0000032102010020LL); // Checking if r exceeds a maximum value. Variation is random and does not depend on private data
				y = 0;
				for (i = 0; i < 14; i++) {
					c = t - cdt[i][2];
					uint64_t b = (((c & cdt[i][2]) & 1) + (cdt[i][2] >> 1) + (c >> 1)) >> 63;
					// Least significant bits of all cdt[i][1] are zero: overflow cannot occur at this point
					c = s - (cdt[i][1] + b);
					b = (((c & b) & 1) + ((cdt[i][1]) >> 1) + (c >> 1)) >> 63;
					// Least significant bits of all cdt[i][0] are zero: overflow cannot occur at this point
					c = r - (cdt[i][0] + b);
					y += ~(c >> (63)) & 1LL;
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
