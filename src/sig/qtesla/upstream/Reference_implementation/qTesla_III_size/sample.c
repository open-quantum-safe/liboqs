/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: sampling functions
**************************************************************************************/

#include "api.h"
#include "sample.h"
#include "sha3/fips202.h"
#include "random/random.h"

#define round_double(x) (uint64_t)(x+0.5)


void sample_y(int64_t *y, const unsigned char *seed, int nonce)
{ // Sample polynomial y, such that each coefficient is in the range [-B,B]
  unsigned int i, pos=0;
  unsigned int nbytes = ((PARAM_B_BITS+1)+7)/8;
  unsigned char buf[PARAM_N*nbytes];
  int16_t dmsp = (int16_t)(nonce<<8);
    
  cshake256_simple((uint8_t*)buf, PARAM_N*nbytes, dmsp, seed, CRYPTO_RANDOMBYTES);

  for (i=0; i<PARAM_N; i++) {
    y[i]  = (*(uint32_t*)(buf+pos)) & ((1<<(PARAM_B_BITS+1))-1);
    y[i] -= PARAM_B;
    pos += nbytes;
  }
}


static int64_t mod7(int64_t k)
{ // Compute k modulo 7 
    int64_t i = k;

    for (int j = 0; j < 2; j++) {
        i = (i & 7) + (i >> 3);
    }
    // i <= 7 at this point. If (i == 7) return 0, else return i
    return  ((i-7) >> 3) & i;
}


static uint32_t Bernoulli(int64_t r, int64_t t)
{ // Sample a bit from Bernoulli
  // Restriction: 15-bit exponent
    static const double exp[3][32] = {
    {
       1.000000000000000000000000000000000000000,
       0.9930872609369466438446685776045870783769,
       0.9862223078352471514816645800946617163753,
       0.9794048103630198064987096277024276745233,
       0.9726344404718809949964892248205218503718,
       0.9659108723811599790550031774166135553877,
       0.9592337825622227893519943162404944070569,
       0.9526028497229044826212304268593937992247,
       0.9460177547920490148527778717375614297184,
       0.9394781809041559863157234389191387682224,
       0.9329838133841335196263226397557142740444,
       0.9265343397321565371915254554479065418152,
       0.9201294496086297094295001392864839167637,
       0.9137688348192543502053861599866811078566,
       0.9074521893001985409223010760173581553575,
       0.9011792091033697696748437406081452352264,
       0.8949495923817893768052167676933918240273,
       0.8887630393750681031008720036903279549798,
       0.8826192523949820417374995711487459848121,
       0.8765179358111482999024679709014128311544,
       0.8704587960367996808317122241155297845427,
       0.8644415415146577017577899722859465798926,
       0.8584658827029032679986111034824856707621,
       0.8525315320612443281154225239327537518954,
       0.8466382040370798397352222582144291697216,
       0.8407856150517593802671106888137768620013,
       0.8349734834869377413443844341919836129518,
       0.8292015296710238503946615698754495016414,
       0.8234694758657233672792155446698925939247,
       0.8177770462526743084502076388248436931084,
       0.8121239669201750555508610480023292486879,
       0.8065099658500041098290290520491383950857,
    },
    {
       1.000000000000000000000000000000000000000,
       0.8009347729043309581492891476788353792319,
       0.6414965104473122043734993727836560472824,
       0.5137968619140387723649494445724253935375,
       0.4115177729160785361721013764372879501486,
       0.3295988939966353993797546624907733779656,
       0.2639872153127138259822760194764351771638,
       0.2114365403461351682772711099029673784632,
       0.1693468774258091812043290184144321199053,
       0.1356358028130980473854681895334247066416,
       0.1086354309238052987136421660339381364741,
       0.08700989419632212963388864322350046226584,
       0.06968924984856112912390130844621296957238,
       0.05581654350133048857116894376531772850611,
       0.04470541059354284482553748295802110496277,
       0.03580611788133410988908922580970256346882,
       0.02867836489387203925107389509582024748183,
       0.02296949967354093915736415916088875712606,
       0.01839707100475361618465239672776920511300,
       0.01473485388729718934389942060072706417326,
       0.01180165685200087257748615707785816032450,
       0.009452357350652160270369000159777795264839,
       0.007570721688055171417140197893346564436337,
       0.006063654255944361840448638204382454042975,
       0.004856591544455177358529410131498186861861,
       0.003889813045747301426610201899960753418453,
       0.003115486528435918796419578590889832746504,
       0.002495301495139325055182486610241845985297,
       0.001998573736337252813277171935604760180550,
       0.001600727201645837799022382894438631230426,
       0.001282078077731994286308984939556462536997,
       0.001026860914033896017267762777785748817307,
    },
    {
       1.000000000000000000000000000000000000000,
       0.0008224486129860722211363916637848860326918,
       6.764217210027140041831031984333572710182E-7,
       5.563221062323340498274022484847072751687E-10,
       4.575463446442734637606790950284979597994E-13,
       3.763083565295300843546185184301854315545E-16,
       3.094942858827803718449578214814971509997E-19,
       2.545431461514076294341840669798321223682E-22,
       2.093486574973362721749187178812187137854E-25,
       1.721785129891805064378362119288044621215E-28,
       1.416079791939559272724577415918469648772E-31,
       1.164652860758296257488258742700152576162E-34,
       9.578671299409218577167988854386920292388E-38,
       7.877964924448609922540282389759450919100E-41,
       6.479221325265686467208309495512157540942E-44,
       5.328826592194544529621668305843636827587E-47,
       4.382686039593701056175477296603487664723E-50,
       3.604534054417261435200071909889027197334E-53,
       2.964544033516540037422419325585053196748E-56,
       2.438185128501814352606789000040829068102E-59,
       2.005281977139585478986634802357029989886E-62,
       1.649241380744420660650718985253047314575E-65,
       1.356416286072483410597007136332173243839E-68,
       1.115582693112033332431603568345878366580E-71,
       9.175094386212588788144072292603516054032E-75,
       7.546043651956841286284539906283246167227E-78,
       6.206233135084259224486265414976130044209E-81,
       5.104307833818251594629732382370662953839E-84,
       4.198030898177763847327057349990235798892E-87,
       3.452664689478976857663267096113988698397E-90,
       2.839639284967972258959000310917203801857E-93,
       2.335457391302670665789857816505217149076E-96,
    },
    };
 
    // Compute the actual Bernoulli parameter c = exp(-t/f):
    double c = 4611686018427387904.0;  // This yields a fraction of 2^62, to keep only 62 bits of precision in this implementation

    for (int64_t i = 0, s = t; i < 3; i++, s >>= 5) {
        c *= exp[i][s & 31]; 
    }
    // Sample from Bernoulli_c
    return (uint32_t)((uint64_t)((r & 0x3FFFFFFFFFFFFFFFLL) - round_double(c)) >> 63);
}


void sample_gauss_poly(int64_t *x, const unsigned char *seed, int nonce)
{ // Gaussian sampler
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

  unsigned char seed_ex[PARAM_N*8]; 
  int64_t i, j=0, x_ind;
  int64_t *buf = (int64_t*)seed_ex;
  int64_t sign, k, l, bitsremained, rbits, y, z;
  uint64_t r, s, t, c;
  int16_t dmsp = (int16_t)(nonce<<8);

  cshake256_simple(seed_ex, PARAM_N*8, dmsp++, seed, CRYPTO_RANDOMBYTES);

  for (x_ind=0; x_ind<PARAM_N; x_ind++){
    if ((j+46) > (PARAM_N)){
	  cshake256_simple((uint8_t*)buf, PARAM_N*8, dmsp++, seed, CRYPTO_RANDOMBYTES);
	  j=0;
    }
    do {
      rbits=buf[j++]; bitsremained=64;
      do {
        // Sample x from D^+_{\sigma_2} and y from U({0, ..., k-1}):
        do {
          r = buf[j++];
          s = buf[j++];
	  t = buf[j++];
          if (bitsremained <= 64 - 6) {
            rbits = (rbits << 6) ^ ((r >> 58) & 63); bitsremained += 6;
          }
          r &= 0x000003FFFFFFFFFFLL;
        } while (r > 0x0000032102010020LL);  // Checking if r exceeds a maximum value. Variation is random and does not depend on private data
        y = 0;
        for (i = 0; i < 14; i++) {
          c = t - cdt[i][2];
          uint64_t b = (((c & cdt[i][2]) & 1) + (cdt[i][2] >> 1) + (c >> 1)) >> 63;
          // Least significant bits of all cdt[i][1] are zero: overflow cannot occur at this point
          c = s - (cdt[i][1]+ b);
          b = (((c & b) & 1) + ((cdt[i][1]) >> 1) + (c >> 1)) >> 63;
          // Least significant bits of all cdt[i][0] are zero: overflow cannot occur at this point
          c = r - (cdt[i][0] + b);
          y += ~(c >> (63)) & 1LL;
        }
        // The next sampler works exclusively for PARAM_Xi <= 28
        do {
          do {
            if (bitsremained < 6) {
              rbits = buf[j++]; bitsremained = 64;
            }
            z = rbits & 63; rbits >>= 6; bitsremained -= 6;
          } while (z == 63);
          if (bitsremained < 2) {
            rbits = buf[j++]; bitsremained = 64;
          }
          z = (mod7(z) << 2) + (rbits & 3); rbits >>= 2; bitsremained -= 2;
        } while (z >= PARAM_Xi);  // Making sure random z does not exceed a certain limit. No private data leaked, it varies uniformly
        k = PARAM_Xi*y + z;
        // Sample a bit from Bernoulli_{exp(-y*(y + 2*k*x)/(2*k^2*sigma_2^2))}
      } while (Bernoulli(buf[j++], z*((k << 1) - z)) == 0);
      // Put last randombits into sign bit
      rbits <<=(64-bitsremained);
      if (bitsremained==0LL) {
        rbits = buf[j++]; bitsremained=64;
      }
      sign = rbits >> 63; rbits <<= 1; bitsremained--;
    } while ((k | (sign & 1)) == 0);
    if (bitsremained==0LL) {
      rbits = buf[j++]; bitsremained=64;
    }
    sign = rbits >> 63; rbits <<= 1; bitsremained--;
    k = ((k << 1) & sign) - k;
    x[x_ind] = (k<<48)>>48;
  }
}


void encode_c(uint32_t *pos_list, int16_t *sign_list, unsigned char *c_bin)
{ // Encoding of c' by mapping the output of the hash function H to an N-element vector with entries {-1,0,1} 
  int i, pos, cnt=0;
  int16_t c[PARAM_N];
  const int RLENGTH = SHAKE128_RATE;
  unsigned char r[RLENGTH];
  uint16_t dmsp=0;
  
  // Use the hash value as key to generate some randomness
  cshake128_simple(r, RLENGTH, dmsp++, c_bin, CRYPTO_RANDOMBYTES);

  // Use rejection sampling to determine positions to be set in the new vector
  for (i=0; i<PARAM_N; i++)
    c[i] = 0;

  for (i=0; i<PARAM_W;) {     // Sample a unique position k times. Use two bytes
    pos = (r[cnt]<<8) | (r[cnt+1]);
    pos = pos & (PARAM_N-1);  // Position is in the range [0,N-1]
    cnt += 2;

    if (c[pos] == 0) {  // Position has not been set yet. Determine sign
      if ((r[cnt] & 1) == 1)
        c[pos] = -1;
      else
        c[pos] = 1;
      pos_list[i] = pos;
      sign_list[i] = c[pos];
      i++;
      cnt++;
    }
    if (cnt > (RLENGTH - 3)) {
      cshake128_simple(r, RLENGTH, dmsp++, c_bin, CRYPTO_RANDOMBYTES);  
      cnt = 0; 
    }
  }
}
