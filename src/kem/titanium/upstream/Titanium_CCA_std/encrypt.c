/* ****************************** *
 * Titanium_CCA_std               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * CPA Encryption                 *
 * ****************************** */

#include "encrypt.h"
#include "param.h"
#include "fastrandombytes.h"
#include "sampler.h"
#include "ntt.h"
#include "fastmodulo.h"
#include "littleendian.h"
#include "pack.h"
#include <string.h>
#include <stdint.h>

/* mapping between the results of NTT_768 and NTT_1536
 * because omega_1536^2=omega_768, we can directly get the results of NTT_768 from NTT_1536 for the same input */
static const uint32_t mapping_r[D + K + 1]={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,286,288,290,292,294,296,298,300,302,304,306,308,310,312,314,316,318,320,322,324,326,328,330,332,334,336,338,340,342,344,346,348,350,352,354,356,358,360,362,364,366,368,370,372,374,376,378,380,382,384,386,388,390,392,394,396,398,400,402,404,406,408,410,412,414,416,418,420,422,424,426,428,430,432,434,436,438,440,442,444,446,448,450,452,454,456,458,460,462,464,466,468,470,472,474,476,478,480,482,484,486,488,490,492,494,496,498,500,502,504,506,508,510,1024,1026,1028,1030,1032,1034,1036,1038,1040,1042,1044,1046,1048,1050,1052,1054,1056,1058,1060,1062,1064,1066,1068,1070,1072,1074,1076,1078,1080,1082,1084,1086,1088,1090,1092,1094,1096,1098,1100,1102,1104,1106,1108,1110,1112,1114,1116,1118,1120,1122,1124,1126,1128,1130,1132,1134,1136,1138,1140,1142,1144,1146,1148,1150,1152,1154,1156,1158,1160,1162,1164,1166,1168,1170,1172,1174,1176,1178,1180,1182,1184,1186,1188,1190,1192,1194,1196,1198,1200,1202,1204,1206,1208,1210,1212,1214,1216,1218,1220,1222,1224,1226,1228,1230,1232,1234,1236,1238,1240,1242,1244,1246,1248,1250,1252,1254,1256,1258,1260,1262,1264,1266,1268,1270,1272,1274,1276,1278,1280,1282,1284,1286,1288,1290,1292,1294,1296,1298,1300,1302,1304,1306,1308,1310,1312,1314,1316,1318,1320,1322,1324,1326,1328,1330,1332,1334,1336,1338,1340,1342,1344,1346,1348,1350,1352,1354,1356,1358,1360,1362,1364,1366,1368,1370,1372,1374,1376,1378,1380,1382,1384,1386,1388,1390,1392,1394,1396,1398,1400,1402,1404,1406,1408,1410,1412,1414,1416,1418,1420,1422,1424,1426,1428,1430,1432,1434,1436,1438,1440,1442,1444,1446,1448,1450,1452,1454,1456,1458,1460,1462,1464,1466,1468,1470,1472,1474,1476,1478,1480,1482,1484,1486,1488,1490,1492,1494,1496,1498,1500,1502,1504,1506,1508,1510,1512,1514,1516,1518,1520,1522,1524,1526,1528,1530,1532,1534,513,515,517,519,521,523,525,527,529,531,533,535,537,539,541,543,545,547,549,551,553,555,557,559,561,563,565,567,569,571,573,575,577,579,581,583,585,587,589,591,593,595,597,599,601,603,605,607,609,611,613,615,617,619,621,623,625,627,629,631,633,635,637,639,641,643,645,647,649,651,653,655,657,659,661,663,665,667,669,671,673,675,677,679,681,683,685,687,689,691,693,695,697,699,701,703,705,707,709,711,713,715,717,719,721,723,725,727,729,731,733,735,737,739,741,743,745,747,749,751,753,755,757,759,761,763,765,767,769,771,773,775,777,779,781,783,785,787,789,791,793,795,797,799,801,803,805,807,809,811,813,815,817,819,821,823,825,827,829,831,833,835,837,839,841,843,845,847,849,851,853,855,857,859,861,863,865,867,869,871,873,875,877,879,881,883,885,887,889,891,893,895,897,899,901,903,905,907,909,911,913,915,917,919,921,923,925,927,929,931,933,935,937,939,941,943,945,947,949,951,953,955,957,959,961,963,965,967,969,971,973,975,977,979,981,983,985,987,989,991,993,995,997,999,1001,1003,1005,1007,1009,1011,1013,1015,1017,1019,1021,1023};

int crypto_encrypt_keypair(unsigned char *pk, unsigned char *sk, const unsigned char *randomness)
{
	uint32_t s[N + D + K + 1];
	uint32_t a[T][N + D + K + 1];
	uint32_t b[T][D + K + 1];
	uint32_t e[T][D + K];
	unsigned char seed_sk[CRYPTO_RANDOMBYTES], seed_pk[CRYPTO_RANDOMBYTES];
	uint32_t i, j;
	
	fastrandombytes_setseed(randomness);
	
	/* generate seed_pk and seed_sk */
	fastrandombytes(seed_sk, CRYPTO_RANDOMBYTES);
	fastrandombytes(seed_pk, CRYPTO_RANDOMBYTES);
		
	fastrandombytes_setseed(seed_pk); /* use seed_pk to sample a_i */
	for (i = 0; i < T; i++)
	{
		sampler_zq(a[i], N, ZQ_BYTPCA); /* a_i <-- U(Z_q^{<n}[x]) */
		NTT_N_NDK(a[i]); /* transform a_i here */
	}
	
	fastrandombytes_setseed(seed_sk); /* change to seed_sk */
	
	/* sample s <-- NTT(lambda_s) over Z_q^{<n+d+k+1)[x] 
	 * generate two more samples, which wouldn't affect the result of middle-product a_i mp_{d+k} s
	 * (can save the adjustment of s to make the most significant bits of lambda_s be 0) */
	sampler_zq(s, N + D + K + 1, ZQ_BYTPCS);
	
	sampler_binomial(e); /* sample e_i <-- lambda_e over Z_q^{<d+k}[x] */
	
	for (i = 0; i < T; i++)
	{
		/* a_i <-- a_i mp_{d+k} s */
		for (j = 0; j < N + D + K + 1; j++)
		{
			a[i][j] = barrett_4q2((uint64_t)a[i][j] * (uint64_t)s[j]);
		}
		
		INTT_NDK_DK(a[i]);
		
		/* b_i <-- as_i + e_i */
		for (j = 0; j < D + K; j++)
		{
			b[i][j] = a[i][j] + e[i][j];
		}

		/* pre-transform b_i here to make Enc and Dec faster */ 
		b[i][D + K] = 0;
		NTT_DK_DK_INV(b[i]);
	}
	
	/* encode pk and sk to binary string */
	/* sk <-- seed_sk */
	memcpy(sk, seed_sk, CRYPTO_RANDOMBYTES);
	
	/* pk <-- (seed_pk, b_i) */
	memcpy(pk, seed_pk, CRYPTO_RANDOMBYTES); 
	for (i = 0; i < T; i++)
	{
		poly_encode(pk + CRYPTO_RANDOMBYTES + i * (D + K + 1) / 8 * Q_BITS, b[i], D + K + 1);
	}
	
	return 0;
}

int crypto_encrypt(unsigned char *c, unsigned long long *clen, const unsigned char *m, unsigned long long mlen, const unsigned char *pk, const unsigned char *randomness)
{
	uint32_t mu[D];
	uint32_t r[T][N + K + 1];
	uint32_t a[T][N + K + 1];
	uint32_t b[T][D + K + 1];
	uint32_t c1[N + K + 1];
	uint32_t c2[D + K + 1];
	uint32_t i, j;
	
	/* reconstruct a_i by the seed saved in pk */
	fastrandombytes_setseed(pk);
	
	for(i = 0; i < T; i++)
	{
		sampler_zq(a[i], N, ZQ_BYTPCA); /* reconstruct a_i */
		NTT_N_NK(a[i]); /* transform a_i here */
	}
	
	/* take hash(m) as the seed */
	fastrandombytes_setseed(randomness);
	
	/* decode pk to polynomials */
	for (i = 0; i < T; i++) 
	{
		poly_decode(b[i], pk + CRYPTO_RANDOMBYTES + i * (D + K + 1) / 8 * Q_BITS, D + K + 1);
	}
	
	/* convert the message to a bit string and map {0,1}^D-->{0,Q/2}^D */
	memset(mu, 0, sizeof(mu));
	for (i = 0; i < mlen; i++)
	{
		for (j = 0; j < 8; j++)
		{
			mu[i * 8 + j] = ((m[i] >> j) & 0x1) * ((Q + 1) / 2);
		}
	}
	
	/* sample r_i <-- lambda_r over Z_q^{<k+1}[x] */
	sampler_zb(r);
	for (i = 0; i < T; i++)
	{
		NTT_K_NK(r[i]); /* transform r_i here */
	}
	
	/* c1 <-- NTT(sum(r_i * a_i)) */
	for (j = 0; j < N + K + 1; j++)
	{
		c1[j] = barrett_64q2((uint64_t)r[0][j] * (uint64_t)a[0][j] + (uint64_t)r[1][j] * (uint64_t)a[1][j] + (uint64_t)r[2][j] * (uint64_t)a[2][j] + (uint64_t)r[3][j] * (uint64_t)a[3][j] + (uint64_t)r[4][j] * (uint64_t)a[4][j] + (uint64_t)r[5][j] * (uint64_t)a[5][j] + (uint64_t)r[6][j] * (uint64_t)a[6][j] + (uint64_t)r[7][j] * (uint64_t)a[7][j] + (uint64_t)r[8][j] * (uint64_t)a[8][j] + (uint64_t)r[9][j] * (uint64_t)a[9][j]);
	}
	
	/* c2 <-- sum(r_i mp_d b_i) + mu */
	for (j = 0; j < D + K + 1; j++)
	{
		c2[j] = barrett_64q2((uint64_t)r[0][mapping_r[j]] * (uint64_t)b[0][j] + (uint64_t)r[1][mapping_r[j]] * (uint64_t)b[1][j] + (uint64_t)r[2][mapping_r[j]] * (uint64_t)b[2][j] + (uint64_t)r[3][mapping_r[j]] * (uint64_t)b[3][j] + (uint64_t)r[4][mapping_r[j]] * (uint64_t)b[4][j] + (uint64_t)r[5][mapping_r[j]] * (uint64_t)b[5][j] + (uint64_t)r[6][mapping_r[j]] * (uint64_t)b[6][j] + (uint64_t)r[7][mapping_r[j]] * (uint64_t)b[7][j] + (uint64_t)r[8][mapping_r[j]] * (uint64_t)b[8][j] + (uint64_t)r[9][mapping_r[j]] * (uint64_t)b[9][j]);
	}
	
	INTT_DK_D(c2);
	
	for (i = 0; i < D; i++)
	{
		c2[i] = barrett_short(c2[i] + mu[i]);
	}
	
	/* encode c <-- (c1,c2) */
	poly_encode(c, c1, N + K + 1);
	poly_encode_c2(c + (N + K + 1) / 8 * Q_BITS, c2, D);
	
	*clen = (N + K + 1) / 8 * Q_BITS + D * C2_COMPRESSION_BYTE;
	
	return 0;
}

int crypto_encrypt_open(unsigned char *m, unsigned long long *mlen, const unsigned char *c, unsigned long long clen, const unsigned char *sk)
{
	uint32_t s[N + D + K + 1];
	uint32_t c1[N + D + K + 1];
	uint32_t c2[D];
	uint32_t c_prime[D];
	uint32_t i, j;
	
	/* recover sk */
	fastrandombytes_setseed(sk);
	sampler_zq(s, N + D + K + 1, ZQ_BYTPCS);
	
	/* decode c to polynomials c1,c2*/
	poly_decode(c1, c, N + K + 1);
	poly_decode_c2(c2, c + (N + K + 1) / 8 * Q_BITS, D);
	
	/* transform c1 back here (will save 1 NTT in the Encap) */
	INTT_NK_NK_INV(c1);
	
	/* c1 <-- c1 mp_d s*/
	NTT_NK_NDK(c1);
	
	for (i = 0; i < N + D + K + 1; i++)
	{
		c1[i] = barrett_4q2((uint64_t)c1[i] * (uint64_t)s[i]);
	}
	
	INTT_NDK_D(c1);

	/* c^prime <-- c2 - c1 mp_d s */	
	for (i = 0; i < D; i++)
	{
		c_prime[i] = Q2 + c2[i] - c1[i];
	}

	/* decode the message (still work for x+kQ) */
	memset(m, 0, D_BYTES);
	for (i = 0; i < D_BYTES; i++)
	{
		for (j = 0; j < 8; j++)
		{
			m[i] |= ((((c_prime[i * 8 + j] << 1) + Q / 2) / Q) & 0x1) << j;
		}
	}
	
	*mlen = D_BYTES;
	return 0;
}
