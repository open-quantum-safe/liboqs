#include "params.h"
#include "ntt.h"

#define QINV 12929 // q^(-1) mod 2^16
#define QINV_PLANT 1951806081u // q^(-1) mod 2^32

const int16_t zetas[192] = {
	 -147, -1033,  -682,  -248,  -708,   682,     1,  -722,
	 -723,  -257, -1124,  -867,  -256,  1484,  1262, -1590,
	 1611,   222,  1164, -1346,  1716, -1521,  -357,   395,
	 -455,   639,   502,   655,  -699,   541,    95, -1577,
	-1241,   550,   -44,    39,  -820,  -216,  -121,  -757,
	 -348,   937,   893,   387,  -603,  1713, -1105,  1058,
	 1449,   837,   901,  1637,  -569, -1617, -1530,  1199,
	   50,  -830,  -625,     4,   176,  -156,  1257, -1507,
	 -380,  -606,  1293,   661,  1428, -1580,  -565,  -992,
	  548,  -800,    64,  -371,   961,   641,    87,   630,
	  675,  -834,   205,    54, -1081,  1351,  1413, -1331,
	-1673, -1267, -1558,   281, -1464,  -588,  1015,   436,
	  223,  1138, -1059,  -397,  -183,  1655,   559, -1674,
	  277,   933,  1723,   437, -1514,   242,  1640,   432,
	-1583,   696,   774,  1671,   927,   514,   512,   489,
	  297,   601,  1473,  1130,  1322,   871,   760,  1212,
	 -312,  -352,   443,   943,     8,  1250,  -100,  1660,
	  -31,  1206, -1341, -1247,   444,   235,  1364, -1209,
	  361,   230,   673,   582,  1409,  1501,  1401,   251,
	 1022, -1063,  1053,  1188,   417, -1391,   -27, -1626,
	 1685,  -315,  1408, -1248,   400,   274, -1543,    32,
	-1550,  1531, -1367,  -124,  1458,  1379,  -940, -1681,
	   22,  1709,  -275,  1108,   354, -1728,  -968,   858,
	 1221,  -218,   294,  -732, -1095,   892,  1588,  -779
};

const uint32_t zetas_plant[192] = {
	    1242397,  -897010815, -1951806081,  -319296093, -1396454510,  1951806081,  -847314925,  1876019849,
	-1571632522, -1283396360, -1100763964,   182632396, -2130711285,  1010068965,   135421300, -1389000126,
	  775255884,   874647665,  1567905331, -1975411629,  2001501971,   275812190,  1843717520,   318053696,
	-1018765746,  -268357806,  -150330067,  -940494719,  -432354243,  1164126224,  1109460745,   480807736,
	 -749165542,  2128226491, -1372848962,  1314456291,  -986463417, -1663569919,  -554109174,  1467271153,
	-1487149509,   634864995,  -737983967, -1493361495,  -170208423,   248479450,   -19878356,  1188974169,
	  601320269,  -532988421,  1073431225,   219904313,  1085855197,    13666370,  -688288077,  1976654026,
	  583926708, -1103248759,  1290850744,   905707596,  1196428553,  -962857869,    77028630,  1298305127,
	 -142875684, -1923230944,  -361537600, -1729416973,  1215064511, -1272214785,  1991562793, -1277184374,
	 -472110955,  -752892734,  1606419645,   821224583,  1774143274, -1962987656,  -701954447, -1232458073,
	 -706924036, -2008956355, -1900867794,  1489634304,  1119399923,  2033804300,  1039886499, -1800233616,
	  218661916,  -193813971,  1561693344, -1872292657,  -776498282,     4969589, -1031189718,   -62119863,
	   27332740,  2123256902,  -341659244,  1376576154,   439808627, -2146862449, -1202640539,  1065976841,
	 1516967043,  -270842601,   365264792,  -909434788, -1360424990,  1108218348,  1972926834,  -967827458,
	 1269729990, -1320668278,  1308244305,  1475967934,   518079654, -1728174576,   -33544726, -2020137930,
	 1750537726,  1864838273,  1740598548,   311841710,   836133350,   723075200,   285751368,  -448505408,
	-1925715739,  1902110191, -1698357042,  -154057259,  1811415192,  1713265809, -1167853416, -2088469779,
	  496958900,   340416847, -1917018958,    39756712,  1749295329, -1550511769,  -391355134, -2093439368,
	 -938009924, -1608904440,   987705814,   783952665,   134178903,  -509382873, -1677236289,  2072318614,
	 1627540399, -1247366840,  1130581498, -1587783687, -1143005471,  1794021630,  1402666496,  -950433897,
	-1796506425,   616229036,   981493828,   887071637,   376446367,  -236055478,  1736871357, -1344273825,
	 -921858760,  -159026848, -1361667387,  1987835601,  1565420536,  -216177122,  1907079780, -1592753276,
	-1461059167,  -657228146,  1083370403,  1772900877,   698227255,  -423657463,  -137906095, -1146732662,
	  515594859,    31059931,    -2484795,  1759234507,    96906986,   109330958, -1211337320, -1366636976
};

/*************************************************
* Name:        montgomery_reduce
*
* Description: Montgomery reduction; given a 32-bit integer a, computes
*              16-bit integer congruent to a * R^-1 mod q, where R=2^16
*
* Arguments:   - int32_t a: input integer to be reduced;
*                           has to be in {-q2^15,...,q2^15-1}
*
* Returns:     integer in {-q+1,...,q-1} congruent to a * R^-1 modulo q.
**************************************************/
static inline int16_t montgomery_reduce(int32_t a)
{
	int16_t t;
	
	t = (int16_t)a*QINV;
	t = (a - (int32_t)t*NTRUPLUS_Q) >> 16;
	
	return t;
}

static inline int16_t plantard_reduce(int32_t a)
{
	a = ((int32_t)(a * QINV_PLANT)) >> 16;
	a=((a+8) * NTRUPLUS_Q) >> 16;
	return a;
}

static inline int16_t plantard_reduce_acc(int32_t a)
{
	a = a >> 16;
	a = ((a+8)*NTRUPLUS_Q) >> 16;
	return a;
}

static inline int16_t plantard_mul(int32_t a, int32_t b)
{
	int32_t t = (int32_t)((uint32_t)a*b) >> 16;
	t = ((t+8)*NTRUPLUS_Q) >> 16;
	return t;
}

/*************************************************
* Name:        fqinv
*
* Description: Inversion
*
* Arguments:   - int16_t a: first factor a = x mod q
*
* Returns 16-bit integer congruent to x^{-1} * R^2 mod q
**************************************************/
static inline int16_t fqinv(int16_t a) //-3 => 5
{
	int16_t t1,t2,t3;
	uint32_t A,T1;

	A = a*QINV_PLANT;
	t1 = plantard_reduce_acc(a*A);    //10

	T1 = t1*QINV_PLANT;
	t2 = plantard_reduce_acc(t1*T1);  //100
	t2 = plantard_reduce(t2*t2);      //1000
	t3 = plantard_reduce(t2*t2);      //10000
	t1 = plantard_reduce_acc(t2*T1);  //1010

	T1 = t1*QINV_PLANT;
	t2 = plantard_reduce_acc(t3*T1);  //11010
	t2 = plantard_reduce(t2*t2);      //110100
	t2 = plantard_reduce_acc(t2*A);   //110101

	t1 = plantard_reduce_acc(t2*T1);  //111111

	t2 = plantard_reduce(t2*t2);      //1101010
	t2 = plantard_reduce(t2*t2);      //11010100
	t2 = plantard_reduce(t2*t2);      //110101000
	t2 = plantard_reduce(t2*t2);      //1101010000
	t2 = plantard_reduce(t2*t2);      //11010100000
	t2 = plantard_reduce(t2*t2);      //110101000000
	t2 = plantard_reduce(t2*t1);      //110101111111

	return t2;
}

/*************************************************
* Name:        ntt
*
* Description: number-theoretic transform (NTT) in Rq.
*
* Arguments:   - int16_t r[NTRUPLUS_N]: pointer to output vector of elements of Zq
*              - int16_t a[NTRUPLUS_N]: pointer to input vector of elements of Zq
**************************************************/
void ntt(int16_t r[NTRUPLUS_N], const int16_t a[NTRUPLUS_N])
{
	int16_t t1,t2,t3;
	uint32_t T1,T2;
	uint32_t zeta[5];
	int16_t v[8];

	int index = 1;

	zeta[0] = zetas_plant[index++];

	for(int i = 0; i < NTRUPLUS_N/2; i++)
	{
		t1 = plantard_mul(zeta[0], a[i + NTRUPLUS_N/2]);

		r[i + NTRUPLUS_N/2] = a[i] + a[i + NTRUPLUS_N/2] - t1;
		r[i               ] = a[i]                       + t1;
	}

	for(int i = 0; i < 2; i++)
	{
		zeta[0] = zetas_plant[2+2*i];
		zeta[1] = zetas_plant[3+2*i];
		zeta[2] = zetas_plant[6+3*i];
		zeta[3] = zetas_plant[7+3*i];
		zeta[4] = zetas_plant[8+3*i];

		for(int j = 0; j < 64; j++)
		{
			for(int k = 0; k < 6; k++)
			{
				v[k] = r[64*k+j+384*i];
			}

			t1 = plantard_mul(zeta[0], v[2]);
			t2 = plantard_mul(zeta[1], v[4]);
			t3 = plantard_mul(-898253212, t1 - t2);

			v[4] = v[0] - t1 - t3;
			v[2] = v[0] - t2 + t3;
			v[0] = v[0] + t1 + t2;

			t1 = plantard_mul(zeta[0], v[3]);
			t2 = plantard_mul(zeta[1], v[5]);
			t3 = plantard_mul(-898253212, t1 - t2);

			v[5] = v[1] - t1 - t3;
			v[3] = v[1] - t2 + t3;
			v[1] = v[1] + t1 + t2;

			t1 = plantard_mul(zeta[2], v[1]);
			v[1] = v[0] - t1;
			v[0] = v[0] + t1;

			t1 = plantard_mul(zeta[3], v[3]);
			v[3] = v[2] - t1;
			v[2] = v[2] + t1;

			t1 = plantard_mul(zeta[4], v[5]);
			v[5] = v[4] - t1;
			v[4] = v[4] + t1;

			for (int k = 0; k < 6; k++)
			{
				r[64*k+j+384*i] = v[k];
			}
		}
	}

	for (int i = 0; i < 12; i++)
	{
		zeta[0] = zetas_plant[12+i];
		zeta[1] = zetas_plant[24+2*i];
		zeta[2] = zetas_plant[25+2*i];

		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				v[k] = r[8*k+j+64*i];
			}

			t1 = plantard_mul(zeta[0], v[4]);
			v[4] = v[0] - t1;
			v[0] = v[0] + t1;

			t1 = plantard_mul(zeta[0], v[5]);
			v[5] = v[1] - t1;
			v[1] = v[1] + t1;

			t1 = plantard_mul(zeta[0], v[6]);
			v[6] = v[2] - t1;
			v[2] = v[2] + t1;

			t1 = plantard_mul(zeta[0], v[7]);
			v[7] = v[3] - t1;
			v[3] = v[3] + t1;

			t1 = plantard_mul(zeta[1], v[2]);
			v[2] = v[0] - t1;
			v[0] = v[0] + t1;

			t1 = plantard_mul(zeta[1], v[3]);
			v[3] = v[1] - t1;
			v[1] = v[1] + t1;

			t1 = plantard_mul(zeta[2], v[6]);
			v[6] = v[4] - t1;
			v[4] = v[4] + t1;

			t1 = plantard_mul(zeta[2], v[7]);
			v[7] = v[5] - t1;
			v[5] = v[5] + t1;

			for (int k = 0; k < 8; k++)
			{
				r[8*k+j+64*i] = v[k];
			}
		}
	}

	for (int i = 0; i < 48; i++)
	{
		zeta[0] = zetas_plant[48+i];
		zeta[1] = zetas_plant[96+2*i];
		zeta[2] = zetas_plant[97+2*i];

		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				v[k] = r[2*k+j+16*i];
			}

			t1 = plantard_mul(zeta[0], v[4]);
			v[4] = v[0] - t1;
			v[0] = v[0] + t1;

			t1 = plantard_mul(zeta[0], v[5]);
			v[5] = v[1] - t1;
			v[1] = v[1] + t1;

			t1 = plantard_mul(zeta[0], v[6]);
			v[6] = v[2] - t1;
			v[2] = v[2] + t1;

			t1 = plantard_mul(zeta[0], v[7]);
			v[7] = v[3] - t1;
			v[3] = v[3] + t1;

			T1 = v[0] * zetas_plant[0];
			T2 = v[2] * zeta[1];
			v[2] = plantard_reduce_acc(T1 - T2);
			v[0] = plantard_reduce_acc(T1 + T2);

			T1 = v[1] * zetas_plant[0];
			T2 = v[3] * zeta[1];
			v[3] = plantard_reduce_acc(T1 - T2);
			v[1] = plantard_reduce_acc(T1 + T2);

			T1 = v[4] * zetas_plant[0];
			T2 = v[6] * zeta[2];
			v[6] = plantard_reduce_acc(T1 - T2);
			v[4] = plantard_reduce_acc(T1 + T2);

			T1 = v[5] * zetas_plant[0];
			T2 = v[7] * zeta[2];
			v[7] = plantard_reduce_acc(T1 - T2);
			v[5] = plantard_reduce_acc(T1 + T2);

			for (int k = 0; k < 8; k++)
			{
				r[2*k+j+16*i] = v[k];
			}
		}
	}
}

/*************************************************
* Name:        invntt
*
* Description: inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor R = 2^16.
*
* Arguments:   - int16_t r[NTRUPLUS_N]: pointer to output vector of elements of Zq
*              - int16_t a[NTRUPLUS_N]: pointer to input vector of elements of Zq
**************************************************/
void invntt(int16_t r[NTRUPLUS_N], const int16_t a[NTRUPLUS_N])
{
	int16_t t1,t2,t3;
	uint32_t zeta[7];
	int16_t v[8];

	for (int i = 0; i < 48; i++)
	{
		zeta[0] = zetas_plant[191-2*i];
		zeta[1] = zetas_plant[190-2*i];
		zeta[2] = zetas_plant[95-i];

		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				v[k] = a[2*k+j+16*i];
			}

			t1 = v[2];

			v[2] = plantard_mul(zeta[0],  t1 - v[0]);
			v[0] = v[0] + t1;

			t1 = v[3];

			v[3] = plantard_mul(zeta[0],  t1 - v[1]);
			v[1] = v[1] + t1;

			t1 = v[6];

			v[6] = plantard_mul(zeta[1],  t1 - v[4]);
			v[4] = v[4] + t1;

			t1 = v[7];

			v[7] = plantard_mul(zeta[1],  t1 - v[5]);
			v[5] = v[5] + t1;


			t1 = v[4];

			v[4] = plantard_mul(zeta[2],  t1 - v[0]);
			v[0] = v[0] + t1;

			t1 = v[5];

			v[5] = plantard_mul(zeta[2],  t1 - v[1]);
			v[1] = v[1] + t1;

			t1 = v[6];

			v[6] = plantard_mul(zeta[2],  t1 - v[2]);
			v[2] = v[2] + t1;

			t1 = v[7];

			v[7] = plantard_mul(zeta[2],  t1 - v[3]);
			v[3] = v[3] + t1;			
								

			for (int k = 0; k < 8; k++)
			{
				r[2*k+j+16*i] = v[k];
			}
		}
	}
	
	for (int i = 0; i < 6; i++)
	{
		zeta[0] = zetas_plant[47-4*i];
		zeta[1] = zetas_plant[46-4*i];
		zeta[2] = zetas_plant[45-4*i];
		zeta[3] = zetas_plant[44-4*i];
		zeta[4] = zetas_plant[23-2*i];
		zeta[5] = zetas_plant[22-2*i];
		zeta[6] = zetas_plant[11-i];

		for (int j = 0; j < 16; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				v[k] = a[16*k+j+128*i];
			}

			t1 = v[1];

			v[1] = plantard_mul(zeta[0],  t1 - v[0]);
			v[0] = (v[0] + t1);

			t1 = v[3];

			v[3] = plantard_mul(zeta[1],  t1 - v[2]);
			v[2] = (v[2] + t1);

			t1 = v[5];

			v[5] = plantard_mul(zeta[2],  t1 - v[4]);
			v[4] = (v[4] + t1);

			t1 = v[7];

			v[7] = plantard_mul(zeta[3],  t1 - v[6]);
			v[6] = (v[6] + t1);


			t1 = v[2];

			v[2] = plantard_mul(zeta[4],  t1 - v[0]);
			v[0] = v[0] + t1;

			t1 = v[3];

			v[3] = plantard_mul(zeta[4],  t1 - v[1]);
			v[1] = v[1] + t1;

			t1 = v[6];

			v[6] = plantard_mul(zeta[5],  t1 - v[4]);
			v[4] = v[4] + t1;

			t1 = v[7];

			v[7] = plantard_mul(zeta[5],  t1 - v[5]);
			v[5] = v[5] + t1;


			t1 = v[4];

			v[4] = plantard_mul(zeta[6],  t1 - v[0]);
			v[0] = plantard_mul(v[0] + t1, zetas_plant[0]);

			t1 = v[5];

			v[5] = plantard_mul(zeta[6],  t1 - v[1]);
			v[1] = plantard_mul(v[1] + t1, zetas_plant[0]);

			t1 = v[6];

			v[6] = plantard_mul(zeta[6],  t1 - v[2]);
			v[2] = plantard_mul(v[2] + t1, zetas_plant[0]);

			t1 = v[7];

			v[7] = plantard_mul(zeta[6],  t1 - v[3]);
			v[3] = plantard_mul(v[3] + t1, zetas_plant[0]);		
				
			for (int k = 0; k < 8; k++)
			{
				r[16*k+j+128*i] = v[k];
			}
		}
	}

	zeta[0] = zetas_plant[4];
	zeta[1] = zetas_plant[5];
	zeta[2] = zetas_plant[2];
	zeta[3] = zetas_plant[3];

	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			v[j] = a[128*j+i];
		}

		t1 = plantard_mul(-898253212,    v[1] - v[0]);
		t2 = plantard_mul(zeta[0], v[2] - v[0] + t1);
		t3 = plantard_mul(zeta[1], v[2] - v[1] - t1);

		v[0] = v[0] + v[1] + v[2];
		v[1] = t2;
		v[2] = t3;

		t1 = plantard_mul(-898253212,    v[4] - v[3]);
		t2 = plantard_mul(zeta[2], v[5] - v[3] + t1);
		t3 = plantard_mul(zeta[3], v[5] - v[4] - t1);

		v[3] = v[3] + v[4] + v[5];
		v[4] = t2;
		v[5] = t3;

		t1 = v[0] + v[3];
		t2 = plantard_mul(2030077108, v[0] - v[3]);

		v[0] = plantard_mul(-1007584170, t1 - t2);
		v[3] = plantard_mul(-2015168341, t2);

		t1 = v[1] + v[4];
		t2 = plantard_mul(2030077108, v[1] - v[4]);

		v[1] = plantard_mul(-1007584170, t1 - t2);
		v[4] = plantard_mul(-2015168341, t2);

		t1 = v[2] + v[5];
		t2 = plantard_mul(2030077108, v[2] - v[5]);

		v[2] = plantard_mul(-1007584170, t1 - t2);
		v[5] = plantard_mul(-2015168341, t2);				

		for (int j = 0; j < 6; j++)
		{
			r[128*j+i] = v[j];
		}
	}
}

/*************************************************
* Name:        baseinv
*
* Description: Inversion of polynomial in Zq[X]/(X^4-zeta)
*              used for inversion of element in Rq in NTT domain
*
* Arguments:   - int16_t r[4]: pointer to the output polynomial
*              - const int16_t a[4]: pointer to the input polynomial
*              - int32_t zeta: integer defining the reduction polynomial
**************************************************/
int baseinv(int16_t r[8], const int16_t a[8], uint32_t zeta)
{
	int16_t t0, t1, t2;
	int16_t s0, s1, s2;
	uint32_t A0, A1, A2, A3;
	uint32_t B0, B1, B2, B3;
	int32_t det0, det1;
	uint32_t zeta1, zeta2;
	uint32_t T, S;

	zeta1 = zeta;
	zeta2 = -zeta;

	A0 = a[0]*QINV_PLANT;
	A1 = a[1]*QINV_PLANT;
	A2 = a[2]*QINV_PLANT;
	A3 = a[3]*QINV_PLANT;
	B0 = a[4]*QINV_PLANT;
	B1 = a[5]*QINV_PLANT;
	B2 = a[6]*QINV_PLANT;
	B3 = a[7]*QINV_PLANT;

	t0 = plantard_reduce_acc(a[2]*A2 - 2*a[1]*A3);
	s0 = plantard_reduce_acc(a[6]*B2 - 2*a[5]*B3);
	t1 = plantard_reduce_acc(a[3]*A3);
	s1 = plantard_reduce_acc(a[7]*B3);
	t0 = plantard_reduce_acc(a[0]*A0 + t0*zeta1);
	s0 = plantard_reduce_acc(a[4]*B0 + s0*zeta2);
	t1 = plantard_reduce_acc(a[1]*A1 + t1*zeta1 - 2*a[0]*A2);
	s1 = plantard_reduce_acc(a[5]*B1 + s1*zeta2 - 2*a[4]*B2);
	t2 = plantard_reduce_acc(t1*zeta1);
	s2 = plantard_reduce_acc(s1*zeta2);
	
	det0 = plantard_reduce(t0*t0 - t1*t2);
	det1 = plantard_reduce(s0*s0 - s1*s2);
	
	if(!(det0 && det1)) return 1;

	r[0] = plantard_reduce_acc(A0*t0 + A2*t2);
	r[1] = plantard_reduce_acc(A3*t2 + A1*t0);
	r[2] = plantard_reduce_acc(A2*t0 + A0*t1);
	r[3] = plantard_reduce_acc(A1*t1 + A3*t0);
	r[4] = plantard_reduce_acc(B0*s0 + B2*s2);
	r[5] = plantard_reduce_acc(B3*s2 + B1*s0);
	r[6] = plantard_reduce_acc(B2*s0 + B0*s1);
	r[7] = plantard_reduce_acc(B1*s1 + B3*s0);

	det0 = fqinv(det0);
	det1 = fqinv(det1);
	det0 = plantard_reduce(det0);
	det1 = plantard_reduce(det1);

	T = det0 * QINV_PLANT;
	S = det1 * QINV_PLANT;

	r[0] = -plantard_reduce_acc(r[0]*T);
	r[1] =  plantard_reduce_acc(r[1]*T);
	r[2] = -plantard_reduce_acc(r[2]*T);
	r[3] =  plantard_reduce_acc(r[3]*T);
	r[4] = -plantard_reduce_acc(r[4]*S);
	r[5] =  plantard_reduce_acc(r[5]*S);
	r[6] = -plantard_reduce_acc(r[6]*S);
	r[7] =  plantard_reduce_acc(r[7]*S);

	return 0;
}

/*************************************************
* Name:        basemul
*
* Description: Multiplication of polynomials in Zq[X]/(X^4-zeta)
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t r[4]: pointer to the output polynomial
*              - const int16_t a[4]: pointer to the first factor
*              - const int16_t b[4]: pointer to the second factor
*              - int16_t zeta: integer defining the reduction polynomial
**************************************************/
void basemul(int16_t r[4], const int16_t a[4], const int16_t b[4], int16_t zeta)
{
	r[0] = montgomery_reduce(a[1]*b[3]+a[2]*b[2]+a[3]*b[1]);
	r[1] = montgomery_reduce(a[2]*b[3]+a[3]*b[2]);
	r[2] = montgomery_reduce(a[3]*b[3]);

	r[0] = montgomery_reduce(r[0]*zeta+a[0]*b[0]);
	r[1] = montgomery_reduce(r[1]*zeta+a[0]*b[1]+a[1]*b[0]);
	r[2] = montgomery_reduce(r[2]*zeta+a[0]*b[2]+a[1]*b[1]+a[2]*b[0]);
	r[3] = montgomery_reduce(a[0]*b[3]+a[1]*b[2]+a[2]*b[1]+a[3]*b[0]);
}

/*************************************************
* Name:        basemul_add
*
* Description: Multiplication then addition of polynomials in Zq[X]/(X^4-zeta)
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t c[4]: pointer to the output polynomial
*              - const int16_t a[4]: pointer to the first factor
*              - const int16_t b[4]: pointer to the second factor
*              - const int16_t c[4]: pointer to the third factor
*              - int16_t zeta: integer defining the reduction polynomial
**************************************************/
void basemul_add(int16_t r[4], const int16_t a[4], const int16_t b[4], const int16_t c[4], int16_t zeta)
{
	r[0] = montgomery_reduce(a[1]*b[3]+a[2]*b[2]+a[3]*b[1]);
	r[1] = montgomery_reduce(a[2]*b[3]+a[3]*b[2]);
	r[2] = montgomery_reduce(a[3]*b[3]);

	r[0] = montgomery_reduce(c[0]*(-147)+r[0]*zeta+a[0]*b[0]);
	r[1] = montgomery_reduce(c[1]*(-147)+r[1]*zeta+a[0]*b[1]+a[1]*b[0]);
	r[2] = montgomery_reduce(c[2]*(-147)+r[2]*zeta+a[0]*b[2]+a[1]*b[1]+a[2]*b[0]);
	r[3] = montgomery_reduce(c[3]*(-147)+a[0]*b[3]+a[1]*b[2]+a[2]*b[1]+a[3]*b[0]);
}
