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
    
  cshake128_simple((uint8_t*)buf, PARAM_N*nbytes, dmsp, seed, CRYPTO_RANDOMBYTES);

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
  // Restriction: 20-bit exponent
    static const double exp[3][32] = {
    {
       1.000000000000000000000000000000000000000,
       0.9930924954370359015332102168880765048173,
       0.9862327044933591729073804985266878802443,
       0.9794202975869268710835182321094224250961,
       0.9726549474122855185227020947295413763023,
       0.9659363289248455510651443129204733029988,
       0.9592641193252643901322834293949397264660,
       0.9526379980439373889289005948680289570903,
       0.9460576467255959075051119972754354254470,
       0.9395227492140117766851490088262829075331,
       0.9330329915368074159813432661499603336007,
       0.9265880618903708756879317851202732375877,
       0.9201876506248750783904312382017973974891,
       0.9138314502294005401326428921359892449876,
       0.9075191553171608564550809482180658363403,
       0.9012504626108302434560060155923701020040,
       0.8950250709279724289295293992056807493013,
       0.8888426811665701935046683790031660959701,
       0.8827029962906548665450116490541232503509,
       0.8766057213160350863710299119436526437378,
       0.8705505632961241391362700174797799990040,
       0.8645372313078651954249311342751209858410,
       0.8585654364377537683418658040230197384022,
       0.8526348917679567215371033354114150564474,
       0.8467453123625271602457822707284519309456,
       0.8408964152537145430311254762332558266219,
       0.8350879194283693564930171007187976000468,
       0.8293195458144416997480650199452263126561,
       0.8235910172675731299989737240342361894393,
       0.8179020585577811249918276889374069238047,
       0.8122523963562355226097093827753290960475,
       0.8066417592221263022701629871861700330324,
    },
    {
       1.000000000000000000000000000000000000000,
       0.8010698775896220771825769800356152059027,
       0.6417129487814520990379170897814202226189,
       0.5140569133280332546731724793964135739076,
       0.4117955086337865649994868620171982731639,
       0.3298769776932235648435004928075127989168,
       0.2642545101403450936248735535216273644409,
       0.2116863280906317900614455676821954153035,
       0.1695755409309589853967628341412449203495,
       0.1358418578157572626069007404662307881780,
       0.1088188204120155173920337521850360624220,
       0.08717147914690033876721823536549963755605,
       0.06983044612951374791318691470020735054081,
       0.05593906693299827680809558745039861318662,
       0.04481110150049460568456273455878078046507,
       0.03589682359365734396245709208684378568331,
       0.02875586408202734619970097619383416315419,
       0.02303545652017345644205569949585157876541,
       0.01845301033483641249297602669511968917867,
       0.01478215073008743605476737495744594743142,
       0.01184153567586248501833796719772135927005,
       0.009485897534336303604787967133085469399049,
       0.007598866776658480613458610115084898737984,
       0.006087223278597655149117219956228990612855,
       0.004876291206646921576592633968279399063782,
       0.003906250000000000000000000000006084516053,
       0.003129179209334461238994441328268996020587,
       0.002506691206177547261866863631962577257343,
       0.002008034817687629901067079997645368310618,
       0.001608576205600728769529245554757186330929,
       0.001288581944114154550169923800031354012535,
       0.001032244180235723021972162318445464753156,
    },
    {
       1.000000000000000000000000000000000000000,
       0.0008268997191040304299275217487598638498908,
       6.837631454543244275598561791827450446268E-7,
       5.654035529098691704742888887601969318770E-10,
       4.675320390815916240837145591289455678271E-13,
       3.866021117887026910581260785663924052584E-16,
       3.196811776431032265107748321378670183434E-19,
       2.643442759959277106397015416454182808165E-22,
       2.185862075677909177530183421677021601630E-25,
       1.807488736378216004902267757945329990433E-28,
       1.494611928394845722509566662381681852231E-31,
       1.235894183759231170477230799378805483584E-34,
       1.021960553392813221805059629881904702629E-37,
       8.450588945359167454685108853553438401193E-41,
       6.987789625181120323479538530531788834637E-44,
       5.778201278220326478541087516212630539830E-47,
       4.777993013886937548374901071454718579294E-50,
       3.950921081064128423947108109095179681258E-53,
       3.267015532134120033414586853048549151733E-56,
       2.701494225830208356330596231491229575841E-59,
       2.233864816500159437321055999997722887780E-62,
       1.847182189280358319436455385107649366142E-65,
       1.527434433449896263866613728025637317872E-68,
       1.263035103969543081968346060350962609985E-71,
       1.044403372690945043917523022329044283453E-74,
       8.636168555094444625386351863230863826745E-78,
       7.141245352342656606906053992842560076147E-81,
       5.905093775905105564186232605424573035226E-84,
       4.882920384578890205960673105845289217904E-87,
       4.037685494415628551550334502904113261957E-90,
       3.338761001162701476381524668052565130775E-93,
       2.760820534016929266476966660680800456743E-96,
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

  unsigned char seed_ex[PARAM_N*8];
  int64_t i, j=0, x_ind;
  int64_t *buf = (int64_t*)seed_ex;
  int64_t sign, k, l, bitsremained, rbits, y, z;
  uint64_t r, s, t, c;
  int16_t dmsp = (int16_t)(nonce<<8);

  cshake128_simple(seed_ex, PARAM_N*8, dmsp++, seed, CRYPTO_RANDOMBYTES);

  for (x_ind=0; x_ind<PARAM_N; x_ind++){
    if ((j+46) > (PARAM_N)){
      cshake128_simple((uint8_t*)buf, PARAM_N*8, dmsp++, seed, CRYPTO_RANDOMBYTES);
      j=0;
    }
    do {
      rbits=buf[j++]; bitsremained=64;
      do {
        // Sample x from D^+_{\sigma_2} and y from U({0, ..., k-1}):
        do {
          r = buf[j++];
          s = buf[j++];
          if (bitsremained <= 64 - 6) {
            rbits = (rbits << 6) ^ ((r >> 58) & 63); bitsremained += 6;
          }
          r &= 0x03FFFFFFFFFFFFFFLL;
        } while (r > 0x0321020100200100LL);  // Checking if r exceeds a maximum value. Variation is random and does not depend on private data 
        y = 0;
        for (i = 0; i < 12; i++) {
          c = s - cdt[i][1];
          uint64_t b = (((c & cdt[i][1]) & 1) + ((cdt[i][1]) >> 1) + (c >> 1)) >> 63;
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
