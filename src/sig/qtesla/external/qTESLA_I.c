/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Merged together in one file for OQS inclusion, made all functions static except
* the api.h ones, and renamed them.
**************************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <oqs/rand.h>
#include <oqs/sha3.h>
#include "qTESLA_I.h"

/* params.h */

#define PARAM_N 512
#define PARAM_N_LOG 9
#define PARAM_SIGMA 23.78
#define PARAM_Xi 27.9988
#define PARAM_Q 4205569
#define PARAM_Q_LOG 23
#define PARAM_QINV 3098553343
#define PARAM_BARR_MULT 1021
#define PARAM_BARR_DIV 32
#define PARAM_B 1048575
#define PARAM_B_BITS 20

#define PARAM_K 1
#define PARAM_SIGMA_E PARAM_SIGMA

#define PARAM_W 30
#define PARAM_D 21

#define PARAM_GEN_A 19

#define PARAM_KEYGEN_BOUND_E 1586
#define PARAM_REJECTION PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 1586
#define PARAM_U PARAM_KEYGEN_BOUND_S

#define PARAM_R2_INVN 113307

/* poly.h */

typedef int64_t __attribute__((aligned(32))) poly[PARAM_N];

static int64_t reduce(int64_t a);
static int32_t barr_reduce(int64_t a);
static void ntt(poly a, const poly w);
static void nttinv(poly a, const poly w);
static void poly_mul(poly result, const poly x, const poly y);
static void poly_add(poly result, const poly x, const poly y);
static void poly_sub(poly result, const poly x, const poly y);
static void poly_uniform(poly a, const unsigned char *seed);

/* api.h */

#define CRYPTO_RANDOMBYTES 32
#define CRYPTO_SEEDBYTES 32
#define CRYPTO_C_BYTES 32

// Contains signature (z,c). z is a polynomial bounded by B, c is the output of a hashed string
#define CRYPTO_BYTES ((PARAM_N * PARAM_D + 7) / 8 + CRYPTO_C_BYTES)
// Contains polynomial s and e, and seeds seed_a and seed_y
#define CRYPTO_SECRETKEYBYTES (2 * sizeof(int16_t) * PARAM_N + 2 * CRYPTO_SEEDBYTES)
// Contains seed_a and polynomial t
#define CRYPTO_PUBLICKEYBYTES ((PARAM_N * PARAM_Q_LOG + 7) / 8 + CRYPTO_SEEDBYTES)

#define CRYPTO_ALGNAME "qTesla-I"

/* sample.h */

static void sample_y(int64_t *y, const unsigned char *seed, int nonce);
static void sample_gauss_poly(int64_t *x, const unsigned char *seed, int nonce);
static void encode_c(uint32_t *pos_list, int16_t *sign_list, unsigned char *c_bin);

/* const.c */

static poly zeta = {

    3359531,
    2189080,
    370173,
    677362,
    3132616,
    2989204,
    2362181,
    1720831,
    1203721,
    3239574,
    641414,
    3932234,
    3634017,
    2251707,
    355329,
    4152265,
    1356023,
    4021436,
    1465601,
    4145892,
    3348341,
    675693,
    1598775,
    2799365,
    3336234,
    3856839,
    603157,
    1381183,
    1069471,
    2142038,
    2877387,
    2653969,
    2055310,
    3837123,
    3141231,
    1951522,
    2375048,
    445122,
    1689285,
    3664328,
    676319,
    3844199,
    3669724,
    1009639,
    3666694,
    1585701,
    2102892,
    966523,
    4069555,
    3246046,
    846643,
    2088895,
    4068915,
    3715722,
    4119007,
    230501,
    1626667,
    2119752,
    1171284,
    3153846,
    17941,
    1316589,
    1814059,
    3185686,
    1183551,
    2533671,
    4152595,
    2616162,
    3015757,
    194860,
    1601807,
    1271569,
    139534,
    2581874,
    2183200,
    2060697,
    1036874,
    646550,
    2823563,
    3312274,
    391700,
    99391,
    638903,
    2397164,
    3924868,
    3315551,
    1170767,
    422539,
    1801679,
    166402,
    742283,
    222557,
    522210,
    3415900,
    177835,
    3243355,
    4196855,
    1821376,
    1290490,
    3624896,
    1546898,
    1282351,
    3960516,
    835944,
    2251927,
    90910,
    3034838,
    4082965,
    2311377,
    3512216,
    2652413,
    2191140,
    302935,
    3866228,
    2007511,
    744185,
    2801160,
    3993630,
    592962,
    795067,
    2822609,
    3471782,
    3710854,
    1824985,
    1495256,
    3906591,
    3111335,
    3902620,
    11234,
    1586236,
    3698245,
    492808,
    2729660,
    3369937,
    1869963,
    7244,
    1453951,
    1757304,
    1005437,
    3668653,
    1821321,
    4203686,
    1192473,
    113408,
    2904803,
    1346735,
    4161890,
    711442,
    4020959,
    1164150,
    2139014,
    4134238,
    731747,
    3856202,
    2351090,
    3382729,
    2644693,
    617098,
    2796766,
    1911274,
    552932,
    2476095,
    1801797,
    1381577,
    2338697,
    1336590,
    2798544,
    459121,
    3555631,
    741068,
    2302686,
    1883916,
    2148181,
    2471691,
    2174195,
    1684042,
    3266036,
    227434,
    4107207,
    2910899,
    3427718,
    2011049,
    2706372,
    4182237,
    1243355,
    2908998,
    15068,
    1966206,
    2157082,
    4114100,
    1846352,
    230880,
    1161075,
    1259576,
    1212857,
    1697580,
    39500,
    3079648,
    2529577,
    2082167,
    50282,
    476606,
    1494601,
    1334236,
    3349015,
    1600445,
    413060,
    3104844,
    139283,
    1688398,
    3230017,
    1009712,
    614253,
    2973529,
    2077610,
    2218429,
    4185344,
    254428,
    506799,
    196179,
    3310395,
    4183346,
    3897905,
    2234639,
    1859699,
    3322900,
    2151737,
    1904476,
    2457045,
    383438,
    2543045,
    2985636,
    731083,
    1609871,
    2171434,
    535413,
    2666041,
    405934,
    3303186,
    802974,
    3573046,
    1760267,
    2758359,
    2102800,
    1512274,
    3981750,
    1838169,
    2101846,
    1363757,
    1342163,
    3608830,
    321523,
    1072908,
    855117,
    1679204,
    3624675,
    3183259,
    2438624,
    407591,
    1549799,
    490068,
    2769318,
    3185950,
    990968,
    3700398,
    2715638,
    3672301,
    3203080,
    1775408,
    2071611,
    778637,
    2335351,
    3317014,
    3768001,
    571163,
    2618746,
    1028702,
    3174131,
    764504,
    1386439,
    4188876,
    1131998,
    1057083,
    39651,
    2588805,
    2519763,
    3838931,
    4130059,
    1893001,
    2066802,
    572208,
    2529031,
    220967,
    3880345,
    1820301,
    2205978,
    3036090,
    1648541,
    4012391,
    1432533,
    3068186,
    1645476,
    1397186,
    2112498,
    4168213,
    1234734,
    1648052,
    1803157,
    2011730,
    1648875,
    2547914,
    437873,
    2460774,
    3403214,
    2690605,
    2567052,
    739775,
    1854855,
    520305,
    3661464,
    1120944,
    1245195,
    1147367,
    2571134,
    696367,
    3009976,
    834907,
    1691662,
    1384090,
    2795844,
    1813845,
    3425954,
    4194068,
    1317042,
    2056507,
    470026,
    3097617,
    2678203,
    3077203,
    2116013,
    4155561,
    2844478,
    1467696,
    4150754,
    992951,
    471101,
    4062883,
    1584992,
    2252609,
    3322854,
    1597940,
    3581574,
    1115369,
    4153697,
    3236495,
    4075586,
    2066340,
    1262360,
    2730720,
    3664692,
    2681478,
    2929295,
    3831713,
    3683420,
    2511172,
    3689552,
    2645837,
    2414330,
    857564,
    3703853,
    468246,
    1574274,
    3590547,
    2348366,
    1565207,
    1815326,
    2508730,
    1749217,
    465029,
    260794,
    1630097,
    3019607,
    3872759,
    1053481,
    3958758,
    3415305,
    54348,
    2516,
    3045515,
    3011542,
    1951553,
    1882613,
    1729323,
    801736,
    3662451,
    909634,
    2949838,
    2598628,
    1652685,
    1945350,
    3221627,
    2879417,
    2732226,
    3883548,
    1891328,
    3215710,
    3159721,
    1318941,
    2153764,
    1870381,
    4039453,
    3375151,
    2655219,
    4089723,
    1388508,
    3436490,
    3956335,
    2748982,
    4111030,
    328986,
    1780674,
    2570336,
    2608795,
    2600572,
    2748827,
    790335,
    1988956,
    3946950,
    1789942,
    710384,
    3900335,
    457139,
    2550557,
    3042298,
    1952120,
    1998308,
    259999,
    2361900,
    119023,
    3680445,
    1893737,
    4050016,
    2696786,
    567472,
    3085466,
    1580931,
    1360307,
    3075154,
    904205,
    1306381,
    3257843,
    2926984,
    2065676,
    3221598,
    2551064,
    1580354,
    1636374,
    699891,
    1821560,
    670885,
    947258,
    2908840,
    3049868,
    1038075,
    1701447,
    2439140,
    2048478,
    3183312,
    2224644,
    320592,
    3304074,
    2611056,
    422256,
    1752180,
    2217951,
    2900510,
    1321050,
    2797671,
    312886,
    2624042,
    3166863,
    908176,
    24947,
    152205,
    2891981,
    189908,
    1959427,
    1365987,
    2071767,
    1932065,
    3185693,
    3889374,
    3644713,
    79765,
    969178,
    11268,
    1992233,
    1579325,
    1224905,
    3741957,
    1894871,
    3060100,
    1787540,
    4194180,
    1396587,
    2745514,
    26822,
    695515,
    2348201,
    249698,
    2988539,
    1081347,
};

static poly zetainv = {

    1217030,
    3955871,
    1857368,
    3510054,
    4178747,
    1460055,
    2808982,
    11389,
    2418029,
    1145469,
    2310698,
    463612,
    2980664,
    2626244,
    2213336,
    4194301,
    3236391,
    4125804,
    560856,
    316195,
    1019876,
    2273504,
    2133802,
    2839582,
    2246142,
    4015661,
    1313588,
    4053364,
    4180622,
    3297393,
    1038706,
    1581527,
    3892683,
    1407898,
    2884519,
    1305059,
    1987618,
    2453389,
    3783313,
    1594513,
    901495,
    3884977,
    1980925,
    1022257,
    2157091,
    1766429,
    2504122,
    3167494,
    1155701,
    1296729,
    3258311,
    3534684,
    2384009,
    3505678,
    2569195,
    2625215,
    1654505,
    983971,
    2139893,
    1278585,
    947726,
    2899188,
    3301364,
    1130415,
    2845262,
    2624638,
    1120103,
    3638097,
    1508783,
    155553,
    2311832,
    525124,
    4086546,
    1843669,
    3945570,
    2207261,
    2253449,
    1163271,
    1655012,
    3748430,
    305234,
    3495185,
    2415627,
    258619,
    2216613,
    3415234,
    1456742,
    1604997,
    1596774,
    1635233,
    2424895,
    3876583,
    94539,
    1456587,
    249234,
    769079,
    2817061,
    115846,
    1550350,
    830418,
    166116,
    2335188,
    2051805,
    2886628,
    1045848,
    989859,
    2314241,
    322021,
    1473343,
    1326152,
    983942,
    2260219,
    2552884,
    1606941,
    1255731,
    3295935,
    543118,
    3403833,
    2476246,
    2322956,
    2254016,
    1194027,
    1160054,
    4203053,
    4151221,
    790264,
    246811,
    3152088,
    332810,
    1185962,
    2575472,
    3944775,
    3740540,
    2456352,
    1696839,
    2390243,
    2640362,
    1857203,
    615022,
    2631295,
    3737323,
    501716,
    3348005,
    1791239,
    1559732,
    516017,
    1694397,
    522149,
    373856,
    1276274,
    1524091,
    540877,
    1474849,
    2943209,
    2139229,
    129983,
    969074,
    51872,
    3090200,
    623995,
    2607629,
    882715,
    1952960,
    2620577,
    142686,
    3734468,
    3212618,
    54815,
    2737873,
    1361091,
    50008,
    2089556,
    1128366,
    1527366,
    1107952,
    3735543,
    2149062,
    2888527,
    11501,
    779615,
    2391724,
    1409725,
    2821479,
    2513907,
    3370662,
    1195593,
    3509202,
    1634435,
    3058202,
    2960374,
    3084625,
    544105,
    3685264,
    2350714,
    3465794,
    1638517,
    1514964,
    802355,
    1744795,
    3767696,
    1657655,
    2556694,
    2193839,
    2402412,
    2557517,
    2970835,
    37356,
    2093071,
    2808383,
    2560093,
    1137383,
    2773036,
    193178,
    2557028,
    1169479,
    1999591,
    2385268,
    325224,
    3984602,
    1676538,
    3633361,
    2138767,
    2312568,
    75510,
    366638,
    1685806,
    1616764,
    4165918,
    3148486,
    3073571,
    16693,
    2819130,
    3441065,
    1031438,
    3176867,
    1586823,
    3634406,
    437568,
    888555,
    1870218,
    3426932,
    2133958,
    2430161,
    1002489,
    533268,
    1489931,
    505171,
    3214601,
    1019619,
    1436251,
    3715501,
    2655770,
    3797978,
    1766945,
    1022310,
    580894,
    2526365,
    3350452,
    3132661,
    3884046,
    596739,
    2863406,
    2841812,
    2103723,
    2367400,
    223819,
    2693295,
    2102769,
    1447210,
    2445302,
    632523,
    3402595,
    902383,
    3799635,
    1539528,
    3670156,
    2034135,
    2595698,
    3474486,
    1219933,
    1662524,
    3822131,
    1748524,
    2301093,
    2053832,
    882669,
    2345870,
    1970930,
    307664,
    22223,
    895174,
    4009390,
    3698770,
    3951141,
    20225,
    1987140,
    2127959,
    1232040,
    3591316,
    3195857,
    975552,
    2517171,
    4066286,
    1100725,
    3792509,
    2605124,
    856554,
    2871333,
    2710968,
    3728963,
    4155287,
    2123402,
    1675992,
    1125921,
    4166069,
    2507989,
    2992712,
    2945993,
    3044494,
    3974689,
    2359217,
    91469,
    2048487,
    2239363,
    4190501,
    1296571,
    2962214,
    23332,
    1499197,
    2194520,
    777851,
    1294670,
    98362,
    3978135,
    939533,
    2521527,
    2031374,
    1733878,
    2057388,
    2321653,
    1902883,
    3464501,
    649938,
    3746448,
    1407025,
    2868979,
    1866872,
    2823992,
    2403772,
    1729474,
    3652637,
    2294295,
    1408803,
    3588471,
    1560876,
    822840,
    1854479,
    349367,
    3473822,
    71331,
    2066555,
    3041419,
    184610,
    3494127,
    43679,
    2858834,
    1300766,
    4092161,
    3013096,
    1883,
    2384248,
    536916,
    3200132,
    2448265,
    2751618,
    4198325,
    2335606,
    835632,
    1475909,
    3712761,
    507324,
    2619333,
    4194335,
    302949,
    1094234,
    298978,
    2710313,
    2380584,
    494715,
    733787,
    1382960,
    3410502,
    3612607,
    211939,
    1404409,
    3461384,
    2198058,
    339341,
    3902634,
    2014429,
    1553156,
    693353,
    1894192,
    122604,
    1170731,
    4114659,
    1953642,
    3369625,
    245053,
    2923218,
    2658671,
    580673,
    2915079,
    2384193,
    8714,
    962214,
    4027734,
    789669,
    3683359,
    3983012,
    3463286,
    4039167,
    2403890,
    3783030,
    3034802,
    890018,
    280701,
    1808405,
    3566666,
    4106178,
    3813869,
    893295,
    1382006,
    3559019,
    3168695,
    2144872,
    2022369,
    1623695,
    4066035,
    2934000,
    2603762,
    4010709,
    1189812,
    1589407,
    52974,
    1671898,
    3022018,
    1019883,
    2391510,
    2888980,
    4187628,
    1051723,
    3034285,
    2085817,
    2578902,
    3975068,
    86562,
    489847,
    136654,
    2116674,
    3358926,
    959523,
    136014,
    3239046,
    2102677,
    2619868,
    538875,
    3195930,
    535845,
    361370,
    3529250,
    541241,
    2516284,
    3760447,
    1830521,
    2254047,
    1064338,
    368446,
    2150259,
    1551600,
    1328182,
    2063531,
    3136098,
    2824386,
    3602412,
    348730,
    869335,
    1406204,
    2606794,
    3529876,
    857228,
    59677,
    2739968,
    184133,
    2849546,
    53304,
    3850240,
    1953862,
    571552,
    273335,
    3564155,
    965995,
    3001848,
    2484738,
    1843388,
    1216365,
    1072953,
    3528207,
    3835396,
    2016489,
    846038,
    3124222,
};

/* poly.c */

static int64_t reduce(int64_t a) { // Montgomery reduction
	int64_t u;

	u = (a * PARAM_QINV) & 0xFFFFFFFF;
	u *= PARAM_Q;
	a += u;
	return a >> 32;
}

static int32_t barr_reduce(int64_t a) { // Barrett reduction
	int64_t u = ((a * PARAM_BARR_MULT) >> PARAM_BARR_DIV) * PARAM_Q;
	return a - u;
}

static void ntt(poly a, const poly w) { // Forward NTT transform
	int NumoProblems = PARAM_N >> 1, jTwiddle = 0;

	for (; NumoProblems > 0; NumoProblems >>= 1) {
		int jFirst, j = 0;
		for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
			int W = w[jTwiddle++];
			for (j = jFirst; j < jFirst + NumoProblems; j++) {
				int temp = reduce(W * a[j + NumoProblems]);
				a[j + NumoProblems] = a[j] + (PARAM_Q - temp);
				a[j] = temp + a[j];
			}
		}
	}
}

static void nttinv(poly a, const poly w) { // Inverse NTT transform
	int NumoProblems = 1, jTwiddle = 0;
	for (NumoProblems = 1; NumoProblems < PARAM_N; NumoProblems *= 2) {
		int jFirst, j = 0;
		for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
			int W = w[jTwiddle++];
			for (j = jFirst; j < jFirst + NumoProblems; j++) {
				int temp = a[j];
				a[j] = barr_reduce(temp + a[j + NumoProblems]);
				a[j + NumoProblems] = reduce(W * (temp + (2 * PARAM_Q - a[j + NumoProblems])));
			}
		}
	}
}

static void poly_pointwise(poly result, const poly x, const poly y) { // Pointwise polynomial multiplication result = x.y
	unsigned int i;

	for (i = 0; i < PARAM_N; i++)
		result[i] = reduce(x[i] * y[i]);
}

static void poly_mul(poly result, const poly x, const poly y) { // Polynomial multiplication result = x*y, with in place reduction for (X^N+1)
	// The input x is assumed to be in NTT form
	poly y_ntt;

	for (int i = 0; i < PARAM_N; i++)
		y_ntt[i] = y[i];

	ntt(y_ntt, zeta);
	poly_pointwise(result, x, y_ntt);
	nttinv(result, zetainv);
}

static void poly_add(poly result, const poly x, const poly y) { // Polynomial addition result = x+y
	unsigned int i;

	for (i = 0; i < PARAM_N; i++)
		result[i] = x[i] + y[i];
}

static void poly_sub(poly result, const poly x, const poly y) { // Polynomial subtraction result = x-y
	unsigned int i;

	for (i = 0; i < PARAM_N; i++)
		result[i] = barr_reduce(x[i] + (2 * PARAM_Q - y[i]));
}

static void poly_uniform(poly a, const unsigned char *seed) { // Generation of polynomial "a"
	unsigned int pos = 0, i = 0, nbytes = (PARAM_Q_LOG + 7) / 8;
	unsigned int nblocks = PARAM_GEN_A;
	uint32_t val1, val2, val3, val4, mask = (1 << PARAM_Q_LOG) - 1;
	unsigned char buf[OQS_SHA3_SHAKE128_RATE * nblocks];
	uint16_t dmsp = 0;

	OQS_SHA3_cshake128_simple(buf, OQS_SHA3_SHAKE128_RATE * nblocks, dmsp++, seed, CRYPTO_RANDOMBYTES);

	while (i < PARAM_N) {
		if (pos > OQS_SHA3_SHAKE128_RATE * nblocks - 4 * nbytes) {
			nblocks = 1;
			OQS_SHA3_cshake128_simple(buf, OQS_SHA3_SHAKE128_RATE * nblocks, dmsp++, seed, CRYPTO_RANDOMBYTES);
			pos = 0;
		}
		val1 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		val2 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		val3 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		val4 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		if (val1 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val1 * PARAM_R2_INVN);
		if (val2 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val2 * PARAM_R2_INVN);
		if (val3 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val3 * PARAM_R2_INVN);
		if (val4 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val4 * PARAM_R2_INVN);
	}
}

/* sample.c */

#define round_double(x) (uint64_t)(x + 0.5)
#define NBLOCKS_SHAKE128 OQS_SHA3_SHAKE128_RATE / (((PARAM_B_BITS + 1) + 7) / 8)

static void sample_y(int64_t *y, const unsigned char *seed, int nonce) { // Sample polynomial y, such that each coefficient is in the range [-B,B]
	unsigned int i = 0, pos = 0, nblocks = PARAM_N;
	unsigned int nbytes = ((PARAM_B_BITS + 1) + 7) / 8;
	unsigned char buf[PARAM_N * nbytes];
	int16_t dmsp = (int16_t)(nonce << 8);

	OQS_SHA3_cshake128_simple((uint8_t *) buf, PARAM_N * nbytes, dmsp++, seed, CRYPTO_RANDOMBYTES);

	while (i < PARAM_N) {
		if (pos >= nblocks * nbytes) {
			nblocks = NBLOCKS_SHAKE128;
			OQS_SHA3_cshake128_simple((uint8_t *) buf, OQS_SHA3_SHAKE128_RATE, dmsp++, seed, CRYPTO_RANDOMBYTES);
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

static void sample_gauss_poly(int64_t *x, const unsigned char *seed, int nonce) { // Gaussian sampler
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
	int64_t sign, k, /* (OQS note: unused) l, */ bitsremained, rbits, y, z;
	uint64_t r, s, c_lo, c_hi;
	int16_t dmsp = (int16_t)(nonce << 8);

	OQS_SHA3_cshake128_simple(seed_ex, PARAM_N * 8, dmsp++, seed, CRYPTO_RANDOMBYTES);

	for (x_ind = 0; x_ind < PARAM_N; x_ind++) {
		if ((j + 46) > (PARAM_N)) {
			OQS_SHA3_cshake128_simple((uint8_t *) buf, PARAM_N * 8, dmsp++, seed, CRYPTO_RANDOMBYTES);
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
	const int RLENGTH = OQS_SHA3_SHAKE128_RATE;
	unsigned char r[OQS_SHA3_SHAKE128_RATE]; /* OQS note: was RLENGTH but it was failing on macOS */
	uint16_t dmsp = 0;

	// Use the hash value as key to generate some randomness
	OQS_SHA3_cshake128_simple(r, RLENGTH, dmsp++, c_bin, CRYPTO_RANDOMBYTES);

	// Use rejection sampling to determine positions to be set in the new vector
	for (i = 0; i < PARAM_N; i++)
		c[i] = 0;

	for (i = 0; i < PARAM_W;) { // Sample a unique position k times. Use two bytes
		if (cnt > (RLENGTH - 3)) {
			OQS_SHA3_cshake128_simple(r, RLENGTH, dmsp++, c_bin, CRYPTO_RANDOMBYTES);
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

/* sign.c */

#ifdef DEBUG
unsigned long long rejwctr;
unsigned long long rejyzctr;
unsigned long long ctr_keygen;
unsigned long long ctr_sign;
#endif

static void pack_sk(unsigned char *sk, poly s, poly e, unsigned char *seeds) { // Pack secret key sk
	int i;
	int16_t *isk = (int16_t *) sk;

	for (i = 0; i < PARAM_N; i++)
		isk[i] = s[i];

	isk += PARAM_N;
	for (i = 0; i < (PARAM_N); i++)
		isk[i] = e[i];

	memcpy(&isk[PARAM_N], seeds, 2 * CRYPTO_SEEDBYTES);
}

static void encode_pk(unsigned char *pk, const poly t, const unsigned char *seedA) { // Encode public key pk
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) pk;

	for (i = 0; i < (PARAM_N * PARAM_Q_LOG / 32); i += PARAM_Q_LOG) {
		pt[i] = t[j] | (t[j + 1] << 23);
		pt[i + 1] = (t[j + 1] >> 9) | (t[j + 2] << 14);
		pt[i + 2] = (t[j + 2] >> 18) | (t[j + 3] << 5) | (t[j + 4] << 28);
		pt[i + 3] = (t[j + 4] >> 4) | (t[j + 5] << 19);
		pt[i + 4] = (t[j + 5] >> 13) | (t[j + 6] << 10);
		pt[i + 5] = (t[j + 6] >> 22) | (t[j + 7] << 1) | (t[j + 8] << 24);
		pt[i + 6] = (t[j + 8] >> 8) | (t[j + 9] << 15);
		pt[i + 7] = (t[j + 9] >> 17) | (t[j + 10] << 6) | (t[j + 11] << 29);
		pt[i + 8] = (t[j + 11] >> 3) | (t[j + 12] << 20);
		pt[i + 9] = (t[j + 12] >> 12) | (t[j + 13] << 11);
		pt[i + 10] = (t[j + 13] >> 21) | (t[j + 14] << 2) | (t[j + 15] << 25);
		pt[i + 11] = (t[j + 15] >> 7) | (t[j + 16] << 16);
		pt[i + 12] = (t[j + 16] >> 16) | (t[j + 17] << 7) | (t[j + 18] << 30);
		pt[i + 13] = (t[j + 18] >> 2) | (t[j + 19] << 21);
		pt[i + 14] = (t[j + 19] >> 11) | (t[j + 20] << 12);
		pt[i + 15] = (t[j + 20] >> 20) | (t[j + 21] << 3) | (t[j + 22] << 26);
		pt[i + 16] = (t[j + 22] >> 6) | (t[j + 23] << 17);
		pt[i + 17] = (t[j + 23] >> 15) | (t[j + 24] << 8) | (t[j + 25] << 31);
		pt[i + 18] = (t[j + 25] >> 1) | (t[j + 26] << 22);
		pt[i + 19] = (t[j + 26] >> 10) | (t[j + 27] << 13);
		pt[i + 20] = (t[j + 27] >> 19) | (t[j + 28] << 4) | (t[j + 29] << 27);
		pt[i + 21] = (t[j + 29] >> 5) | (t[j + 30] << 18);
		pt[i + 22] = (t[j + 30] >> 14) | (t[j + 31] << 9);
		j += 32;
	}
	memcpy(&pk[PARAM_N * PARAM_Q_LOG / 8], seedA, CRYPTO_SEEDBYTES);
}

static void decode_pk(int32_t *pk, unsigned char *seedA, const unsigned char *pk_in) { // Decode public key pk
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) pk_in, *pp = (uint32_t *) pk, mask23 = (1 << PARAM_Q_LOG) - 1;

	for (i = 0; i < PARAM_N; i += 32) {
		pp[i] = pt[j] & mask23;
		pp[i + 1] = ((pt[j + 0] >> 23) | (pt[j + 1] << 9)) & mask23;
		pp[i + 2] = ((pt[j + 1] >> 14) | (pt[j + 2] << 18)) & mask23;
		pp[i + 3] = (pt[j + 2] >> 5) & mask23;
		pp[i + 4] = ((pt[j + 2] >> 28) | (pt[j + 3] << 4)) & mask23;
		pp[i + 5] = ((pt[j + 3] >> 19) | (pt[j + 4] << 13)) & mask23;
		pp[i + 6] = ((pt[j + 4] >> 10) | (pt[j + 5] << 22)) & mask23;
		pp[i + 7] = (pt[j + 5] >> 1) & mask23;
		pp[i + 8] = ((pt[j + 5] >> 24) | (pt[j + 6] << 8)) & mask23;
		pp[i + 9] = ((pt[j + 6] >> 15) | (pt[j + 7] << 17)) & mask23;
		pp[i + 10] = (pt[j + 7] >> 6) & mask23;
		pp[i + 11] = ((pt[j + 7] >> 29) | (pt[j + 8] << 3)) & mask23;
		pp[i + 12] = ((pt[j + 8] >> 20) | (pt[j + 9] << 12)) & mask23;
		pp[i + 13] = ((pt[j + 9] >> 11) | (pt[j + 10] << 21)) & mask23;
		pp[i + 14] = (pt[j + 10] >> 2) & mask23;
		pp[i + 15] = ((pt[j + 10] >> 25) | (pt[j + 11] << 7)) & mask23;
		pp[i + 16] = ((pt[j + 11] >> 16) | (pt[j + 12] << 16)) & mask23;
		pp[i + 17] = (pt[j + 12] >> 7) & mask23;
		pp[i + 18] = ((pt[j + 12] >> 30) | (pt[j + 13] << 2)) & mask23;
		pp[i + 19] = ((pt[j + 13] >> 21) | (pt[j + 14] << 11)) & mask23;
		pp[i + 20] = ((pt[j + 14] >> 12) | (pt[j + 15] << 20)) & mask23;
		pp[i + 21] = (pt[j + 15] >> 3) & mask23;
		pp[i + 22] = ((pt[j + 15] >> 26) | (pt[j + 16] << 6)) & mask23;
		pp[i + 23] = ((pt[j + 16] >> 17) | (pt[j + 17] << 15)) & mask23;
		pp[i + 24] = (pt[j + 17] >> 8) & mask23;
		pp[i + 25] = ((pt[j + 17] >> 31) | (pt[j + 18] << 1)) & mask23;
		pp[i + 26] = ((pt[j + 18] >> 22) | (pt[j + 19] << 10)) & mask23;
		pp[i + 27] = ((pt[j + 19] >> 13) | (pt[j + 20] << 19)) & mask23;
		pp[i + 28] = (pt[j + 20] >> 4) & mask23;
		pp[i + 29] = ((pt[j + 20] >> 27) | (pt[j + 21] << 5)) & mask23;
		pp[i + 30] = ((pt[j + 21] >> 18) | (pt[j + 22] << 14)) & mask23;
		pp[i + 31] = pt[j + 22] >> 9;
		j += 23;
	}
	memcpy(seedA, &pk_in[PARAM_N * PARAM_Q_LOG / 8], CRYPTO_SEEDBYTES);
}

static void encode_sig(unsigned char *sm, unsigned char *c, poly z) { // Encode signature sm
	unsigned int i, j = 0;
	uint64_t *t = (uint64_t *) z;
	uint32_t *pt = (uint32_t *) sm;

	for (i = 0; i < (PARAM_N * PARAM_D / 32); i += PARAM_D) {
		pt[i] = (t[j] & ((1 << 21) - 1)) | (t[j + 1] << 21);
		pt[i + 1] = ((t[j + 1] >> 11) & ((1 << 10) - 1)) | ((t[j + 2] & ((1 << 21) - 1)) << 10) | (t[j + 3] << 31);
		pt[i + 2] = ((t[j + 3] >> 1) & ((1 << 20) - 1)) | (t[j + 4] << 20);
		pt[i + 3] = ((t[j + 4] >> 12) & ((1 << 9) - 1)) | ((t[j + 5] & ((1 << 21) - 1)) << 9) | (t[j + 6] << 30);
		pt[i + 4] = ((t[j + 6] >> 2) & ((1 << 19) - 1)) | (t[j + 7] << 19);
		pt[i + 5] = ((t[j + 7] >> 13) & ((1 << 8) - 1)) | ((t[j + 8] & ((1 << 21) - 1)) << 8) | (t[j + 9] << 29);
		pt[i + 6] = ((t[j + 9] >> 3) & ((1 << 18) - 1)) | (t[j + 10] << 18);
		pt[i + 7] = ((t[j + 10] >> 14) & ((1 << 7) - 1)) | ((t[j + 11] & ((1 << 21) - 1)) << 7) | (t[j + 12] << 28);
		pt[i + 8] = ((t[j + 12] >> 4) & ((1 << 17) - 1)) | (t[j + 13] << 17);
		pt[i + 9] = ((t[j + 13] >> 15) & ((1 << 6) - 1)) | ((t[j + 14] & ((1 << 21) - 1)) << 6) | (t[j + 15] << 27);
		pt[i + 10] = ((t[j + 15] >> 5) & ((1 << 16) - 1)) | (t[j + 16] << 16);
		pt[i + 11] = ((t[j + 16] >> 16) & ((1 << 5) - 1)) | ((t[j + 17] & ((1 << 21) - 1)) << 5) | (t[j + 18] << 26);
		pt[i + 12] = ((t[j + 18] >> 6) & ((1 << 15) - 1)) | (t[j + 19] << 15);
		pt[i + 13] = ((t[j + 19] >> 17) & ((1 << 4) - 1)) | ((t[j + 20] & ((1 << 21) - 1)) << 4) | (t[j + 21] << 25);
		pt[i + 14] = ((t[j + 21] >> 7) & ((1 << 14) - 1)) | (t[j + 22] << 14);
		pt[i + 15] = ((t[j + 22] >> 18) & ((1 << 3) - 1)) | ((t[j + 23] & ((1 << 21) - 1)) << 3) | (t[j + 24] << 24);
		pt[i + 16] = ((t[j + 24] >> 8) & ((1 << 13) - 1)) | (t[j + 25] << 13);
		pt[i + 17] = ((t[j + 25] >> 19) & ((1 << 2) - 1)) | ((t[j + 26] & ((1 << 21) - 1)) << 2) | (t[j + 27] << 23);
		pt[i + 18] = ((t[j + 27] >> 9) & ((1 << 12) - 1)) | (t[j + 28] << 12);
		pt[i + 19] = ((t[j + 28] >> 20) & ((1 << 1) - 1)) | ((t[j + 29] & ((1 << 21) - 1)) << 1) | (t[j + 30] << 22);
		pt[i + 20] = ((t[j + 30] >> 10) & ((1 << 11) - 1)) | (t[j + 31] << 11);
		j += 32;
	}
	memcpy(&sm[PARAM_N * PARAM_D / 8], c, CRYPTO_C_BYTES);
}

static void decode_sig(unsigned char *c, poly z, const unsigned char *sm) { // Decode signature sm
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) sm;

	for (i = 0; i < PARAM_N; i += 32) {
		z[i] = ((int32_t) pt[j + 0] << 11) >> 11;
		z[i + 1] = (int32_t)(pt[j + 0] >> 21) | ((int32_t)(pt[j + 1] << 22) >> 11);
		z[i + 2] = ((int32_t) pt[j + 1] << 1) >> 11;
		z[i + 3] = (int32_t)(pt[j + 1] >> 31) | ((int32_t)(pt[j + 2] << 12) >> 11);
		z[i + 4] = (int32_t)(pt[j + 2] >> 20) | ((int32_t)(pt[j + 3] << 23) >> 11);
		z[i + 5] = (int32_t)(pt[j + 3] << 2) >> 11;
		z[i + 6] = (int32_t)(pt[j + 3] >> 30) | ((int32_t)(pt[j + 4] << 13) >> 11);
		z[i + 7] = (int32_t)(pt[j + 4] >> 19) | ((int32_t)(pt[j + 5] << 24) >> 11);
		z[i + 8] = (int32_t)(pt[j + 5] << 3) >> 11;
		z[i + 9] = (int32_t)(pt[j + 5] >> 29) | ((int32_t)(pt[j + 6] << 14) >> 11);
		z[i + 10] = (int32_t)(pt[j + 6] >> 18) | ((int32_t)(pt[j + 7] << 25) >> 11);
		z[i + 11] = (int32_t)(pt[j + 7] << 4) >> 11;
		z[i + 12] = (int32_t)(pt[j + 7] >> 28) | ((int32_t)(pt[j + 8] << 15) >> 11);
		z[i + 13] = (int32_t)(pt[j + 8] >> 17) | ((int32_t)(pt[j + 9] << 26) >> 11);
		z[i + 14] = (int32_t)(pt[j + 9] << 5) >> 11;
		z[i + 15] = (int32_t)(pt[j + 9] >> 27) | ((int32_t)(pt[j + 10] << 16) >> 11);
		z[i + 16] = (int32_t)(pt[j + 10] >> 16) | ((int32_t)(pt[j + 11] << 27) >> 11);
		z[i + 17] = (int32_t)(pt[j + 11] << 6) >> 11;
		z[i + 18] = (int32_t)(pt[j + 11] >> 26) | ((int32_t)(pt[j + 12] << 17) >> 11);
		z[i + 19] = (int32_t)(pt[j + 12] >> 15) | ((int32_t)(pt[j + 13] << 28) >> 11);
		z[i + 20] = (int32_t)(pt[j + 13] << 7) >> 11;
		z[i + 21] = (int32_t)(pt[j + 13] >> 25) | ((int32_t)(pt[j + 14] << 18) >> 11);
		z[i + 22] = (int32_t)(pt[j + 14] >> 14) | ((int32_t)(pt[j + 15] << 29) >> 11);
		z[i + 23] = (int32_t)(pt[j + 15] << 8) >> 11;
		z[i + 24] = (int32_t)(pt[j + 15] >> 24) | ((int32_t)(pt[j + 16] << 19) >> 11);
		z[i + 25] = (int32_t)(pt[j + 16] >> 13) | ((int32_t)(pt[j + 17] << 30) >> 11);
		z[i + 26] = (int32_t)(pt[j + 17] << 9) >> 11;
		z[i + 27] = (int32_t)(pt[j + 17] >> 23) | ((int32_t)(pt[j + 18] << 20) >> 11);
		z[i + 28] = (int32_t)(pt[j + 18] >> 12) | ((int32_t)(pt[j + 19] << 31) >> 11);
		z[i + 29] = (int32_t)(pt[j + 19] << 10) >> 11;
		z[i + 30] = (int32_t)(pt[j + 19] >> 22) | ((int32_t)(pt[j + 20] << 21) >> 11);
		z[i + 31] = (int32_t) pt[j + 20] >> 11;
		j += 21;
	}
	memcpy(c, &sm[PARAM_N * PARAM_D / 8], CRYPTO_C_BYTES);
}

static void hash_vm(unsigned char *c_bin, poly v, const unsigned char *m, unsigned long long mlen) { // Hash to generate c'
	unsigned char t[PARAM_N + mlen];
	int64_t mask, cL;
	unsigned int i;

	for (i = 0; i < PARAM_N; i++) {
		// If v[i] > PARAM_Q/2 then v[i] -= PARAM_Q
		mask = (int64_t)(PARAM_Q / 2 - v[i]) >> 63;
		v[i] = ((v[i] - PARAM_Q) & mask) | (v[i] & ~mask);

		cL = v[i] & ((1 << PARAM_D) - 1);
		// If cL > 2^(d-1) then cL -= 2^d
		mask = (int64_t)((1 << (PARAM_D - 1)) - cL) >> 63;
		cL = ((cL - (1 << PARAM_D)) & mask) | (cL & ~mask);
		t[i] = (unsigned char) ((v[i] - cL) >> PARAM_D);
	}
	memcpy(&t[PARAM_N], m, mlen);
	OQS_SHA3_shake128(c_bin, CRYPTO_C_BYTES, t, mlen + PARAM_N);
}

static __inline uint64_t Abs(int64_t value) { // Compute absolute value

	uint64_t mask = (uint64_t)(value >> 63);
	return ((mask ^ value) - mask);
}

static int test_rejection(poly z) { // Check bounds for signature vector z during signing. Returns 0 if valid, otherwise outputs 1 if invalid (rejected).
	// This function leaks the position of the coefficient that fails the test (but this is independent of the secret data).
	// It does not leak the sign of the coefficients.
	unsigned int i;

	for (i = 0; i < PARAM_N; i++) {
		if (Abs(z[i]) > (PARAM_B - PARAM_U))
			return 1;
	}
	return 0;
}

static int test_v(poly v) { // Check bounds for w = v - ec during signature verification. Returns 0 if valid, otherwise outputs 1 if invalid (rejected).
	// This function leaks the position of the coefficient that fails the test (but this is independent of the secret data).
	// It does not leak the sign of the coefficients.
	unsigned int i;
	int64_t mask, left, val;
	uint64_t t0, t1;

	for (i = 0; i < PARAM_N; i++) {
		// If v[i] > PARAM_Q/2 then v[i] -= PARAM_Q
		mask = (int64_t)(PARAM_Q / 2 - v[i]) >> 63;
		val = ((v[i] - PARAM_Q) & mask) | (v[i] & ~mask);
		// If (Abs(val) < PARAM_Q/2 - PARAM_REJECTION) then t0 = 0, else t0 = 1
		t0 = (uint64_t)(~((int64_t) Abs(val) - (int64_t)(PARAM_Q / 2 - PARAM_REJECTION))) >> 63;

		left = val;
		val = (int32_t)((val + (1 << (PARAM_D - 1)) - 1) >> PARAM_D);
		val = left - (val << PARAM_D);
		// If (Abs(val) < (1<<(PARAM_D-1))-PARAM_REJECTION) then t1 = 0, else t1 = 1
		t1 = (uint64_t)(~((int64_t) Abs(val) - (int64_t)((1 << (PARAM_D - 1)) - PARAM_REJECTION))) >> 63;

		if ((t0 | t1) == 1) // Returns 1 if any of the two tests failed
			return 1;
	}
	return 0;
}

static int test_z(poly z) { // Check bounds for signature vector z during signature verification
	// Returns 0 if valid, otherwise outputs 1 if invalid (rejected)
	unsigned int i;

	for (i = 0; i < PARAM_N; i++) {
		if (z[i] < -(PARAM_B - PARAM_U) || z[i] > (PARAM_B - PARAM_U))
			return 1;
	}
	return 0;
}

static int check_ES(poly p, int bound) { // Checks the generated polynomial e or s
	// Returns 0 if ok, otherwise returns 1
	unsigned int i, j, sum = 0, limit = PARAM_N;
	int16_t temp, mask, list[PARAM_N];

	for (j = 0; j < PARAM_N; j++)
		list[j] = (int16_t)(Abs(p[j]));

	for (j = 0; j < PARAM_W; j++) {
		for (i = 0; i < limit - 1; i++) {
			// If list[i+1] > list[i] then exchange contents
			mask = (list[i + 1] - list[i]) >> 15;
			temp = (list[i + 1] & mask) | (list[i] & ~mask);
			list[i + 1] = (list[i] & mask) | (list[i + 1] & ~mask);
			list[i] = temp;
		}
		sum += list[limit - 1];
		limit -= 1;
	}

	if ((int) sum > bound)
		return 1;
	return 0;
}

/********************************************************************************************
* Name:        sparse_mul16
* Description: performs sparse polynomial multiplication
* Parameters:  inputs:
*              - const unsigned char* sk: part of the secret key
*              - const uint32_t pos_list[PARAM_W]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_W]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*
* Note: pos_list[] and sign_list[] contain public information since c is public
*********************************************************************************************/
static void sparse_mul16(poly prod, const unsigned char *sk, const uint32_t pos_list[PARAM_W], const int16_t sign_list[PARAM_W]) {
	int i, j, pos;
	int16_t *t = (int16_t *) sk;

	for (i = 0; i < PARAM_N; i++)
		prod[i] = 0;

	for (i = 0; i < PARAM_W; i++) {
		pos = pos_list[i];
		for (j = 0; j < pos; j++) {
			prod[j] = prod[j] - sign_list[i] * t[j + PARAM_N - pos];
		}
		for (j = pos; j < PARAM_N; j++) {
			prod[j] = prod[j] + sign_list[i] * t[j - pos];
		}
	}
}

/********************************************************************************************
* Name:        sparse_mul32
* Description: performs sparse polynomial multiplication
* Parameters:  inputs:
*              - const int32_t* pk: part of the public key
*              - const uint32_t pos_list[PARAM_W]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_W]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*********************************************************************************************/
static void sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_W], const int16_t sign_list[PARAM_W]) {
	int i, j, pos;

	for (i = 0; i < PARAM_N; i++)
		prod[i] = 0;

	for (i = 0; i < PARAM_W; i++) {
		pos = pos_list[i];
		for (j = 0; j < pos; j++) {
			prod[j] = prod[j] - sign_list[i] * pk[j + PARAM_N - pos];
		}
		for (j = pos; j < PARAM_N; j++) {
			prod[j] = prod[j] + sign_list[i] * pk[j - pos];
		}
	}
	for (i = 0; i < PARAM_N; i++)
		prod[i] = barr_reduce(prod[i]);
}

#include "qTESLA_api.c"

OQS_API OQS_STATUS OQS_SIG_qTESLA_I_keypair(unsigned char *pk, unsigned char *sk) {
	return crypto_sign_keypair(pk, sk);
}

OQS_API OQS_STATUS OQS_SIG_qTESLA_I_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk) {
	return crypto_sign(sm, smlen, m, mlen, sk);
}

OQS_API OQS_STATUS OQS_SIG_qTESLA_I_verify(unsigned char *m, unsigned long long mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk) {
	return crypto_verify(m, mlen, sm, smlen, pk);
}
