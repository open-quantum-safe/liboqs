/********************************************************************************************
* SHA3-derived functions: SHA3-256, SHA3-512, SHAKE, and cSHAKE
*
* Based on the public domain implementation in crypto_hash/keccakc512/simple/
* from http://bench.cr.yp.to/supercop.html by Ronny Van Keer
* and the public domain "TweetFips202" implementation from https://twitter.com/tweetfips202
* by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe
*
* See NIST Special Publication 800-185 for more information:
* http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-185.pdf
*
* Updated by John Underhill, December 24, 2017
*********************************************************************************************/

#include <oqs/sha3.h>

#define SHA3_CSHAKE_DOMAIN 0x04
#define SHA3_CSHAKE128_RATE 168
#define SHA3_CSHAKE256_RATE 136
#define SHA3_SHA3_DOMAIN 0x06
#define SHA3_SHA3_256_RATE 136
#define SHA3_SHA3_512_RATE 72
#define SHA3_SHAKE_DOMAIN 0x1F
#define SHA3_SHAKE128_RATE 168
#define SHA3_SHAKE256_RATE 136
#define SHA3_STATESIZE 25

/* For Incremental Hshing */
#define NROUNDS 24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

/* Internal */

static uint64_t load64(const uint8_t *a) {
	uint64_t r = 0;
	size_t i;

	for (i = 0; i < 8; ++i) {
		r |= (uint64_t) a[i] << 8 * i;
	}

	return r;
}

static uint64_t rotl64(const uint64_t x, uint32_t shift) {
	return (x << shift) | (x >> (64 - shift));
}

static void store64(uint8_t *a, uint64_t x) {
	size_t i;

	for (i = 0; i < 8; ++i) {
		a[i] = x & 0xFF;
		x >>= 8;
	}
}

/* SHA3 */

void OQS_SHA3_keccak_permute(uint64_t *state) {
	uint64_t Aba = state[0];
	uint64_t Abe = state[1];
	uint64_t Abi = state[2];
	uint64_t Abo = state[3];
	uint64_t Abu = state[4];
	uint64_t Aga = state[5];
	uint64_t Age = state[6];
	uint64_t Agi = state[7];
	uint64_t Ago = state[8];
	uint64_t Agu = state[9];
	uint64_t Aka = state[10];
	uint64_t Ake = state[11];
	uint64_t Aki = state[12];
	uint64_t Ako = state[13];
	uint64_t Aku = state[14];
	uint64_t Ama = state[15];
	uint64_t Ame = state[16];
	uint64_t Ami = state[17];
	uint64_t Amo = state[18];
	uint64_t Amu = state[19];
	uint64_t Asa = state[20];
	uint64_t Ase = state[21];
	uint64_t Asi = state[22];
	uint64_t Aso = state[23];
	uint64_t Asu = state[24];

	/* round 1 */
	uint64_t Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	uint64_t Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	uint64_t Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	uint64_t Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	uint64_t Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	uint64_t Da = Cu ^ rotl64(Ce, 1);
	uint64_t De = Ca ^ rotl64(Ci, 1);
	uint64_t Di = Ce ^ rotl64(Co, 1);
	uint64_t Do = Ci ^ rotl64(Cu, 1);
	uint64_t Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	uint64_t Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x0000000000000001ULL;
	uint64_t Ebe = Ce ^ ((~Ci) & Co);
	uint64_t Ebi = Ci ^ ((~Co) & Cu);
	uint64_t Ebo = Co ^ ((~Cu) & Ca);
	uint64_t Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	uint64_t Ega = Ca ^ ((~Ce) & Ci);
	uint64_t Ege = Ce ^ ((~Ci) & Co);
	uint64_t Egi = Ci ^ ((~Co) & Cu);
	uint64_t Ego = Co ^ ((~Cu) & Ca);
	uint64_t Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	uint64_t Eka = Ca ^ ((~Ce) & Ci);
	uint64_t Eke = Ce ^ ((~Ci) & Co);
	uint64_t Eki = Ci ^ ((~Co) & Cu);
	uint64_t Eko = Co ^ ((~Cu) & Ca);
	uint64_t Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	uint64_t Ema = Ca ^ ((~Ce) & Ci);
	uint64_t Eme = Ce ^ ((~Ci) & Co);
	uint64_t Emi = Ci ^ ((~Co) & Cu);
	uint64_t Emo = Co ^ ((~Cu) & Ca);
	uint64_t Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	uint64_t Esa = Ca ^ ((~Ce) & Ci);
	uint64_t Ese = Ce ^ ((~Ci) & Co);
	uint64_t Esi = Ci ^ ((~Co) & Cu);
	uint64_t Eso = Co ^ ((~Cu) & Ca);
	uint64_t Esu = Cu ^ ((~Ca) & Ce);
	/* round 2 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x0000000000008082ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 3 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x800000000000808AULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 4 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x8000000080008000ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 5 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x000000000000808BULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 6 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x0000000080000001ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 7 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x8000000080008081ULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 8 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x8000000000008009ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 9 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x000000000000008AULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 10 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x0000000000000088ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 11 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x0000000080008009ULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 12 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x000000008000000AULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 13 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x000000008000808BULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 14 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x800000000000008BULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 15 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x8000000000008089ULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 16 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x8000000000008003ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 17 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x8000000000008002ULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 18 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x8000000000000080ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 19 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x000000000000800AULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 20 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x800000008000000AULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 21 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x8000000080008081ULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 22 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x8000000000008080ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);
	/* round 23 */
	Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
	Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;
	Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
	Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
	Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Aba ^= Da;
	Ca = Aba;
	Age ^= De;
	Ce = rotl64(Age, 44);
	Aki ^= Di;
	Ci = rotl64(Aki, 43);
	Amo ^= Do;
	Co = rotl64(Amo, 21);
	Asu ^= Du;
	Cu = rotl64(Asu, 14);
	Eba = Ca ^ ((~Ce) & Ci);
	Eba ^= 0x0000000080000001ULL;
	Ebe = Ce ^ ((~Ci) & Co);
	Ebi = Ci ^ ((~Co) & Cu);
	Ebo = Co ^ ((~Cu) & Ca);
	Ebu = Cu ^ ((~Ca) & Ce);
	Abo ^= Do;
	Ca = rotl64(Abo, 28);
	Agu ^= Du;
	Ce = rotl64(Agu, 20);
	Aka ^= Da;
	Ci = rotl64(Aka, 3);
	Ame ^= De;
	Co = rotl64(Ame, 45);
	Asi ^= Di;
	Cu = rotl64(Asi, 61);
	Ega = Ca ^ ((~Ce) & Ci);
	Ege = Ce ^ ((~Ci) & Co);
	Egi = Ci ^ ((~Co) & Cu);
	Ego = Co ^ ((~Cu) & Ca);
	Egu = Cu ^ ((~Ca) & Ce);
	Abe ^= De;
	Ca = rotl64(Abe, 1);
	Agi ^= Di;
	Ce = rotl64(Agi, 6);
	Ako ^= Do;
	Ci = rotl64(Ako, 25);
	Amu ^= Du;
	Co = rotl64(Amu, 8);
	Asa ^= Da;
	Cu = rotl64(Asa, 18);
	Eka = Ca ^ ((~Ce) & Ci);
	Eke = Ce ^ ((~Ci) & Co);
	Eki = Ci ^ ((~Co) & Cu);
	Eko = Co ^ ((~Cu) & Ca);
	Eku = Cu ^ ((~Ca) & Ce);
	Abu ^= Du;
	Ca = rotl64(Abu, 27);
	Aga ^= Da;
	Ce = rotl64(Aga, 36);
	Ake ^= De;
	Ci = rotl64(Ake, 10);
	Ami ^= Di;
	Co = rotl64(Ami, 15);
	Aso ^= Do;
	Cu = rotl64(Aso, 56);
	Ema = Ca ^ ((~Ce) & Ci);
	Eme = Ce ^ ((~Ci) & Co);
	Emi = Ci ^ ((~Co) & Cu);
	Emo = Co ^ ((~Cu) & Ca);
	Emu = Cu ^ ((~Ca) & Ce);
	Abi ^= Di;
	Ca = rotl64(Abi, 62);
	Ago ^= Do;
	Ce = rotl64(Ago, 55);
	Aku ^= Du;
	Ci = rotl64(Aku, 39);
	Ama ^= Da;
	Co = rotl64(Ama, 41);
	Ase ^= De;
	Cu = rotl64(Ase, 2);
	Esa = Ca ^ ((~Ce) & Ci);
	Ese = Ce ^ ((~Ci) & Co);
	Esi = Ci ^ ((~Co) & Cu);
	Eso = Co ^ ((~Cu) & Ca);
	Esu = Cu ^ ((~Ca) & Ce);
	/* round 24 */
	Ca = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
	Ce = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
	Ci = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
	Co = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
	Cu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;
	Da = Cu ^ rotl64(Ce, 1);
	De = Ca ^ rotl64(Ci, 1);
	Di = Ce ^ rotl64(Co, 1);
	Do = Ci ^ rotl64(Cu, 1);
	Du = Co ^ rotl64(Ca, 1);
	Eba ^= Da;
	Ca = Eba;
	Ege ^= De;
	Ce = rotl64(Ege, 44);
	Eki ^= Di;
	Ci = rotl64(Eki, 43);
	Emo ^= Do;
	Co = rotl64(Emo, 21);
	Esu ^= Du;
	Cu = rotl64(Esu, 14);
	Aba = Ca ^ ((~Ce) & Ci);
	Aba ^= 0x8000000080008008ULL;
	Abe = Ce ^ ((~Ci) & Co);
	Abi = Ci ^ ((~Co) & Cu);
	Abo = Co ^ ((~Cu) & Ca);
	Abu = Cu ^ ((~Ca) & Ce);
	Ebo ^= Do;
	Ca = rotl64(Ebo, 28);
	Egu ^= Du;
	Ce = rotl64(Egu, 20);
	Eka ^= Da;
	Ci = rotl64(Eka, 3);
	Eme ^= De;
	Co = rotl64(Eme, 45);
	Esi ^= Di;
	Cu = rotl64(Esi, 61);
	Aga = Ca ^ ((~Ce) & Ci);
	Age = Ce ^ ((~Ci) & Co);
	Agi = Ci ^ ((~Co) & Cu);
	Ago = Co ^ ((~Cu) & Ca);
	Agu = Cu ^ ((~Ca) & Ce);
	Ebe ^= De;
	Ca = rotl64(Ebe, 1);
	Egi ^= Di;
	Ce = rotl64(Egi, 6);
	Eko ^= Do;
	Ci = rotl64(Eko, 25);
	Emu ^= Du;
	Co = rotl64(Emu, 8);
	Esa ^= Da;
	Cu = rotl64(Esa, 18);
	Aka = Ca ^ ((~Ce) & Ci);
	Ake = Ce ^ ((~Ci) & Co);
	Aki = Ci ^ ((~Co) & Cu);
	Ako = Co ^ ((~Cu) & Ca);
	Aku = Cu ^ ((~Ca) & Ce);
	Ebu ^= Du;
	Ca = rotl64(Ebu, 27);
	Ega ^= Da;
	Ce = rotl64(Ega, 36);
	Eke ^= De;
	Ci = rotl64(Eke, 10);
	Emi ^= Di;
	Co = rotl64(Emi, 15);
	Eso ^= Do;
	Cu = rotl64(Eso, 56);
	Ama = Ca ^ ((~Ce) & Ci);
	Ame = Ce ^ ((~Ci) & Co);
	Ami = Ci ^ ((~Co) & Cu);
	Amo = Co ^ ((~Cu) & Ca);
	Amu = Cu ^ ((~Ca) & Ce);
	Ebi ^= Di;
	Ca = rotl64(Ebi, 62);
	Ego ^= Do;
	Ce = rotl64(Ego, 55);
	Eku ^= Du;
	Ci = rotl64(Eku, 39);
	Ema ^= Da;
	Co = rotl64(Ema, 41);
	Ese ^= De;
	Cu = rotl64(Ese, 2);
	Asa = Ca ^ ((~Ce) & Ci);
	Ase = Ce ^ ((~Ci) & Co);
	Asi = Ci ^ ((~Co) & Cu);
	Aso = Co ^ ((~Cu) & Ca);
	Asu = Cu ^ ((~Ca) & Ce);

	state[0] = Aba;
	state[1] = Abe;
	state[2] = Abi;
	state[3] = Abo;
	state[4] = Abu;
	state[5] = Aga;
	state[6] = Age;
	state[7] = Agi;
	state[8] = Ago;
	state[9] = Agu;
	state[10] = Aka;
	state[11] = Ake;
	state[12] = Aki;
	state[13] = Ako;
	state[14] = Aku;
	state[15] = Ama;
	state[16] = Ame;
	state[17] = Ami;
	state[18] = Amo;
	state[19] = Amu;
	state[20] = Asa;
	state[21] = Ase;
	state[22] = Asi;
	state[23] = Aso;
	state[24] = Asu;
}

void OQS_SHA3_keccak_absorb(uint64_t *state, size_t rate, const uint8_t *input, size_t inplen, uint8_t domain) {
	uint8_t msg[200];
	size_t i;

	while (inplen >= rate) {
		for (i = 0; i < rate / 8; ++i) {
			state[i] ^= load64(input + (8 * i));
		}

		OQS_SHA3_keccak_permute(state);

		inplen -= rate;
		input += rate;
	}

	for (i = 0; i < inplen; ++i) {
		msg[i] = input[i];
	}

	msg[inplen] = domain;

	for (i = inplen + 1; i < rate; ++i) {
		msg[i] = 0;
	}

	msg[rate - 1] |= 128;

	for (i = 0; i < rate / 8; ++i) {
		state[i] ^= load64(msg + (8 * i));
	}
}

void OQS_SHA3_sha3256(uint8_t *output, const uint8_t *input, size_t inplen) {
	uint64_t state[SHA3_STATESIZE];
	uint8_t hash[SHA3_SHA3_256_RATE];
	size_t i;

	for (i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_keccak_absorb(state, SHA3_SHA3_256_RATE, input, inplen, SHA3_SHA3_DOMAIN);
	OQS_SHA3_keccak_squeezeblocks(hash, 1, state, SHA3_SHA3_256_RATE);

	for (i = 0; i < 32; i++) {
		output[i] = hash[i];
	}
}

void OQS_SHA3_sha3512(uint8_t *output, const uint8_t *input, size_t inplen) {
	uint64_t state[SHA3_STATESIZE];
	uint8_t hash[SHA3_SHA3_512_RATE];
	size_t i;

	for (i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_keccak_absorb(state, SHA3_SHA3_512_RATE, input, inplen, SHA3_SHA3_DOMAIN);
	OQS_SHA3_keccak_squeezeblocks(hash, 1, state, SHA3_SHA3_512_RATE);

	for (i = 0; i < 64; i++) {
		output[i] = hash[i];
	}
}

void OQS_SHA3_keccak_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state, size_t rate) {
	size_t i;

	while (nblocks > 0) {
		OQS_SHA3_keccak_permute(state);

		for (i = 0; i < (rate >> 3); i++) {
			store64(output + 8 * i, state[i]);
		}

		output += rate;
		nblocks--;
	}
}

/* SHAKE */

void OQS_SHA3_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	size_t nblocks = outlen / SHA3_SHAKE128_RATE;
	uint64_t state[SHA3_STATESIZE];
	uint8_t hash[SHA3_SHAKE128_RATE];
	size_t i;

	for (i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_keccak_absorb(state, SHA3_SHAKE128_RATE, input, inplen, SHA3_SHAKE_DOMAIN);
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_SHAKE128_RATE);

	output += (nblocks * SHA3_SHAKE128_RATE);
	outlen -= (nblocks * SHA3_SHAKE128_RATE);

	if (outlen != 0) {
		OQS_SHA3_keccak_squeezeblocks(hash, 1, state, SHA3_SHAKE128_RATE);

		for (i = 0; i < outlen; i++) {
			output[i] = hash[i];
		}
	}
}

void OQS_SHA3_shake128_absorb(uint64_t *state, const uint8_t *input, size_t inplen) {
	for (size_t i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}
	OQS_SHA3_keccak_absorb(state, SHA3_SHAKE128_RATE, input, inplen, SHA3_SHAKE_DOMAIN);
}

void OQS_SHA3_shake128_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state) {
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_SHAKE128_RATE);
}

void OQS_SHA3_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	size_t nblocks = outlen / SHA3_SHAKE256_RATE;
	uint64_t state[SHA3_STATESIZE];
	uint8_t hash[SHA3_SHAKE256_RATE];
	size_t i;

	for (i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_keccak_absorb(state, SHA3_SHAKE256_RATE, input, inplen, SHA3_SHAKE_DOMAIN);
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_SHAKE256_RATE);

	output += (nblocks * SHA3_SHAKE256_RATE);
	outlen -= (nblocks * SHA3_SHAKE256_RATE);

	if (outlen != 0) {
		OQS_SHA3_keccak_squeezeblocks(hash, 1, state, SHA3_SHAKE256_RATE);

		for (i = 0; i < outlen; i++) {
			output[i] = hash[i];
		}
	}
}

void OQS_SHA3_shake256_absorb(uint64_t *state, const uint8_t *input, size_t inplen) {
	for (size_t i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}
	OQS_SHA3_keccak_absorb(state, SHA3_SHAKE256_RATE, input, inplen, SHA3_SHAKE_DOMAIN);
}

void OQS_SHA3_shake256_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state) {
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_SHAKE256_RATE);
}

/* cSHAKE */

void OQS_SHA3_cshake128_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen) {
	size_t nblocks = outlen / SHA3_CSHAKE128_RATE;
	uint64_t state[SHA3_STATESIZE];
	uint8_t hash[SHA3_CSHAKE128_RATE];
	size_t i;

	for (i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_cshake128_simple_absorb(state, cstm, input, inplen);
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_CSHAKE128_RATE);

	output += (nblocks * SHA3_CSHAKE128_RATE);
	outlen -= (nblocks * SHA3_CSHAKE128_RATE);

	if (outlen != 0) {
		OQS_SHA3_keccak_squeezeblocks(hash, 1, state, SHA3_CSHAKE128_RATE);

		for (i = 0; i < outlen; i++) {
			output[i] = hash[i];
		}
	}
}

void OQS_SHA3_cshake128_simple_absorb(uint64_t *state, uint16_t cstm, const uint8_t *input, size_t inplen) {
	/* Note: This function doesn't align exactly to cSHAKE (SP800-185 3.2), which should produce
	SHAKE output if S and N = zero (sort of a customized custom-SHAKE function).
	Padding is hard-coded as the first 32 bits, plus 16 bits of fixed S,
	and 16 bits of counter, equivalant to S=cstm, N=0.
	The short integer optimizes this function for a digest counter configuration */

	uint8_t sep[8];
	sep[0] = 0x01; /* bytepad */
	sep[1] = 0xA8;
	sep[2] = 0x01;
	sep[3] = 0x00;
	sep[4] = 0x01;
	sep[5] = 0x10; /* bitlen of cstm */
	sep[6] = cstm & 0xFF;
	sep[7] = cstm >> 8;

	state[0] = load64(sep);

	/* transform the domain string */
	OQS_SHA3_keccak_permute(state);

	/* absorb the state */
	OQS_SHA3_keccak_absorb(state, SHA3_CSHAKE128_RATE, input, inplen, SHA3_CSHAKE_DOMAIN);
}

void OQS_SHA3_cshake128_simple_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state) {
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_CSHAKE128_RATE);
}

void OQS_SHA3_cshake256_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen) {
	size_t nblocks = outlen / SHA3_CSHAKE256_RATE;
	uint64_t state[SHA3_STATESIZE];
	uint8_t hash[SHA3_CSHAKE256_RATE];
	size_t i;

	for (i = 0; i < SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_cshake256_simple_absorb(state, cstm, input, inplen);
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_CSHAKE256_RATE);

	output += (nblocks * SHA3_CSHAKE256_RATE);
	outlen -= (nblocks * SHA3_CSHAKE256_RATE);

	if (outlen != 0) {
		OQS_SHA3_keccak_squeezeblocks(hash, 1, state, SHA3_CSHAKE256_RATE);

		for (i = 0; i < outlen; i++) {
			output[i] = hash[i];
		}
	}
}

void OQS_SHA3_cshake256_simple_absorb(uint64_t *state, uint16_t cstm, const uint8_t *input, size_t inplen) {
	uint8_t sep[8];
	sep[0] = 0x01; /* bytepad */
	sep[1] = 0x88;
	sep[2] = 0x01;
	sep[3] = 0x00;
	sep[4] = 0x01;
	sep[5] = 0x10; /* bitlen of cstm */
	sep[6] = cstm & 0xFF;
	sep[7] = cstm >> 8;

	state[0] = load64(sep);

	/* transform the domain string */
	OQS_SHA3_keccak_permute(state);

	/* absorb the state */
	OQS_SHA3_keccak_absorb(state, SHA3_CSHAKE256_RATE, input, inplen, SHA3_CSHAKE_DOMAIN);
}

void OQS_SHA3_cshake256_simple_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state) {
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, SHA3_CSHAKE256_RATE);
}

/****************************************************************************/

/* Incremental Hashing */

/* Keccak round constants */
static const uint64_t KeccakF_RoundConstants[NROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL};

/*************************************************
 * Name:        KeccakF1600_StatePermute
 *
 * Description: The Keccak F1600 Permutation
 *
 * Arguments:   - uint64_t *state: pointer to input/output Keccak state
 **************************************************/
static void KeccakF1600_StatePermute(uint64_t *state) {
	int round;

	uint64_t Aba, Abe, Abi, Abo, Abu;
	uint64_t Aga, Age, Agi, Ago, Agu;
	uint64_t Aka, Ake, Aki, Ako, Aku;
	uint64_t Ama, Ame, Ami, Amo, Amu;
	uint64_t Asa, Ase, Asi, Aso, Asu;
	uint64_t BCa, BCe, BCi, BCo, BCu;
	uint64_t Da, De, Di, Do, Du;
	uint64_t Eba, Ebe, Ebi, Ebo, Ebu;
	uint64_t Ega, Ege, Egi, Ego, Egu;
	uint64_t Eka, Eke, Eki, Eko, Eku;
	uint64_t Ema, Eme, Emi, Emo, Emu;
	uint64_t Esa, Ese, Esi, Eso, Esu;

	// copyFromState(A, state)
	Aba = state[0];
	Abe = state[1];
	Abi = state[2];
	Abo = state[3];
	Abu = state[4];
	Aga = state[5];
	Age = state[6];
	Agi = state[7];
	Ago = state[8];
	Agu = state[9];
	Aka = state[10];
	Ake = state[11];
	Aki = state[12];
	Ako = state[13];
	Aku = state[14];
	Ama = state[15];
	Ame = state[16];
	Ami = state[17];
	Amo = state[18];
	Amu = state[19];
	Asa = state[20];
	Ase = state[21];
	Asi = state[22];
	Aso = state[23];
	Asu = state[24];

	for (round = 0; round < NROUNDS; round += 2) {
		//    prepareTheta
		BCa = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
		BCe = Abe ^ Age ^ Ake ^ Ame ^ Ase;
		BCi = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
		BCo = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
		BCu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;

		// thetaRhoPiChiIotaPrepareTheta(round  , A, E)
		Da = BCu ^ ROL(BCe, 1);
		De = BCa ^ ROL(BCi, 1);
		Di = BCe ^ ROL(BCo, 1);
		Do = BCi ^ ROL(BCu, 1);
		Du = BCo ^ ROL(BCa, 1);

		Aba ^= Da;
		BCa = Aba;
		Age ^= De;
		BCe = ROL(Age, 44);
		Aki ^= Di;
		BCi = ROL(Aki, 43);
		Amo ^= Do;
		BCo = ROL(Amo, 21);
		Asu ^= Du;
		BCu = ROL(Asu, 14);
		Eba = BCa ^ ((~BCe) & BCi);
		Eba ^= KeccakF_RoundConstants[round];
		Ebe = BCe ^ ((~BCi) & BCo);
		Ebi = BCi ^ ((~BCo) & BCu);
		Ebo = BCo ^ ((~BCu) & BCa);
		Ebu = BCu ^ ((~BCa) & BCe);

		Abo ^= Do;
		BCa = ROL(Abo, 28);
		Agu ^= Du;
		BCe = ROL(Agu, 20);
		Aka ^= Da;
		BCi = ROL(Aka, 3);
		Ame ^= De;
		BCo = ROL(Ame, 45);
		Asi ^= Di;
		BCu = ROL(Asi, 61);
		Ega = BCa ^ ((~BCe) & BCi);
		Ege = BCe ^ ((~BCi) & BCo);
		Egi = BCi ^ ((~BCo) & BCu);
		Ego = BCo ^ ((~BCu) & BCa);
		Egu = BCu ^ ((~BCa) & BCe);

		Abe ^= De;
		BCa = ROL(Abe, 1);
		Agi ^= Di;
		BCe = ROL(Agi, 6);
		Ako ^= Do;
		BCi = ROL(Ako, 25);
		Amu ^= Du;
		BCo = ROL(Amu, 8);
		Asa ^= Da;
		BCu = ROL(Asa, 18);
		Eka = BCa ^ ((~BCe) & BCi);
		Eke = BCe ^ ((~BCi) & BCo);
		Eki = BCi ^ ((~BCo) & BCu);
		Eko = BCo ^ ((~BCu) & BCa);
		Eku = BCu ^ ((~BCa) & BCe);

		Abu ^= Du;
		BCa = ROL(Abu, 27);
		Aga ^= Da;
		BCe = ROL(Aga, 36);
		Ake ^= De;
		BCi = ROL(Ake, 10);
		Ami ^= Di;
		BCo = ROL(Ami, 15);
		Aso ^= Do;
		BCu = ROL(Aso, 56);
		Ema = BCa ^ ((~BCe) & BCi);
		Eme = BCe ^ ((~BCi) & BCo);
		Emi = BCi ^ ((~BCo) & BCu);
		Emo = BCo ^ ((~BCu) & BCa);
		Emu = BCu ^ ((~BCa) & BCe);

		Abi ^= Di;
		BCa = ROL(Abi, 62);
		Ago ^= Do;
		BCe = ROL(Ago, 55);
		Aku ^= Du;
		BCi = ROL(Aku, 39);
		Ama ^= Da;
		BCo = ROL(Ama, 41);
		Ase ^= De;
		BCu = ROL(Ase, 2);
		Esa = BCa ^ ((~BCe) & BCi);
		Ese = BCe ^ ((~BCi) & BCo);
		Esi = BCi ^ ((~BCo) & BCu);
		Eso = BCo ^ ((~BCu) & BCa);
		Esu = BCu ^ ((~BCa) & BCe);

		//    prepareTheta
		BCa = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
		BCe = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
		BCi = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
		BCo = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
		BCu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;

		// thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
		Da = BCu ^ ROL(BCe, 1);
		De = BCa ^ ROL(BCi, 1);
		Di = BCe ^ ROL(BCo, 1);
		Do = BCi ^ ROL(BCu, 1);
		Du = BCo ^ ROL(BCa, 1);

		Eba ^= Da;
		BCa = Eba;
		Ege ^= De;
		BCe = ROL(Ege, 44);
		Eki ^= Di;
		BCi = ROL(Eki, 43);
		Emo ^= Do;
		BCo = ROL(Emo, 21);
		Esu ^= Du;
		BCu = ROL(Esu, 14);
		Aba = BCa ^ ((~BCe) & BCi);
		Aba ^= KeccakF_RoundConstants[round + 1];
		Abe = BCe ^ ((~BCi) & BCo);
		Abi = BCi ^ ((~BCo) & BCu);
		Abo = BCo ^ ((~BCu) & BCa);
		Abu = BCu ^ ((~BCa) & BCe);

		Ebo ^= Do;
		BCa = ROL(Ebo, 28);
		Egu ^= Du;
		BCe = ROL(Egu, 20);
		Eka ^= Da;
		BCi = ROL(Eka, 3);
		Eme ^= De;
		BCo = ROL(Eme, 45);
		Esi ^= Di;
		BCu = ROL(Esi, 61);
		Aga = BCa ^ ((~BCe) & BCi);
		Age = BCe ^ ((~BCi) & BCo);
		Agi = BCi ^ ((~BCo) & BCu);
		Ago = BCo ^ ((~BCu) & BCa);
		Agu = BCu ^ ((~BCa) & BCe);

		Ebe ^= De;
		BCa = ROL(Ebe, 1);
		Egi ^= Di;
		BCe = ROL(Egi, 6);
		Eko ^= Do;
		BCi = ROL(Eko, 25);
		Emu ^= Du;
		BCo = ROL(Emu, 8);
		Esa ^= Da;
		BCu = ROL(Esa, 18);
		Aka = BCa ^ ((~BCe) & BCi);
		Ake = BCe ^ ((~BCi) & BCo);
		Aki = BCi ^ ((~BCo) & BCu);
		Ako = BCo ^ ((~BCu) & BCa);
		Aku = BCu ^ ((~BCa) & BCe);

		Ebu ^= Du;
		BCa = ROL(Ebu, 27);
		Ega ^= Da;
		BCe = ROL(Ega, 36);
		Eke ^= De;
		BCi = ROL(Eke, 10);
		Emi ^= Di;
		BCo = ROL(Emi, 15);
		Eso ^= Do;
		BCu = ROL(Eso, 56);
		Ama = BCa ^ ((~BCe) & BCi);
		Ame = BCe ^ ((~BCi) & BCo);
		Ami = BCi ^ ((~BCo) & BCu);
		Amo = BCo ^ ((~BCu) & BCa);
		Amu = BCu ^ ((~BCa) & BCe);

		Ebi ^= Di;
		BCa = ROL(Ebi, 62);
		Ego ^= Do;
		BCe = ROL(Ego, 55);
		Eku ^= Du;
		BCi = ROL(Eku, 39);
		Ema ^= Da;
		BCo = ROL(Ema, 41);
		Ese ^= De;
		BCu = ROL(Ese, 2);
		Asa = BCa ^ ((~BCe) & BCi);
		Ase = BCe ^ ((~BCi) & BCo);
		Asi = BCi ^ ((~BCo) & BCu);
		Aso = BCo ^ ((~BCu) & BCa);
		Asu = BCu ^ ((~BCa) & BCe);
	}

	// copyToState(state, A)
	state[0] = Aba;
	state[1] = Abe;
	state[2] = Abi;
	state[3] = Abo;
	state[4] = Abu;
	state[5] = Aga;
	state[6] = Age;
	state[7] = Agi;
	state[8] = Ago;
	state[9] = Agu;
	state[10] = Aka;
	state[11] = Ake;
	state[12] = Aki;
	state[13] = Ako;
	state[14] = Aku;
	state[15] = Ama;
	state[16] = Ame;
	state[17] = Ami;
	state[18] = Amo;
	state[19] = Amu;
	state[20] = Asa;
	state[21] = Ase;
	state[22] = Asi;
	state[23] = Aso;
	state[24] = Asu;
}

/*************************************************
 * Name:        keccak_inc_init
 *
 * Description: Initializes the incremental Keccak state to zero.
 *
 * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 **************************************************/
static void keccak_inc_init(uint64_t *s_inc) {
	size_t i;

	for (i = 0; i < 25; ++i) {
		s_inc[i] = 0;
	}
	s_inc[25] = 0;
}

/*************************************************
 * Name:        keccak_inc_absorb
 *
 * Description: Incremental keccak absorb
 *              Preceded by keccak_inc_init, succeeded by keccak_inc_finalize
 *
 * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 *              - const uint8_t *m: pointer to input to be absorbed into s
 *              - size_t mlen: length of input in bytes
 **************************************************/
static void keccak_inc_absorb(uint64_t *s_inc, uint32_t r, const uint8_t *m,
                              size_t mlen) {
	size_t i;

	/* Recall that s_inc[25] is the non-absorbed bytes xored into the state */
	while (mlen + s_inc[25] >= r) {
		for (i = 0; i < r - s_inc[25]; i++) {
			/* Take the i'th byte from message
               xor with the s_inc[25] + i'th byte of the state; little-endian */
			s_inc[(s_inc[25] + i) >> 3] ^= (uint64_t) m[i] << (8 * ((s_inc[25] + i) & 0x07));
		}
		mlen -= (size_t)(r - s_inc[25]);
		m += r - s_inc[25];
		s_inc[25] = 0;

		KeccakF1600_StatePermute(s_inc);
	}

	for (i = 0; i < mlen; i++) {
		s_inc[(s_inc[25] + i) >> 3] ^= (uint64_t) m[i] << (8 * ((s_inc[25] + i) & 0x07));
	}
	s_inc[25] += mlen;
}

/*************************************************
 * Name:        keccak_inc_finalize
 *
 * Description: Finalizes Keccak absorb phase, prepares for squeezing
 *
 * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 *              - uint8_t p: domain-separation byte for different
 *                                 Keccak-derived functions
 **************************************************/
static void keccak_inc_finalize(uint64_t *s_inc, uint32_t r, uint8_t p) {
	/* After keccak_inc_absorb, we are guaranteed that s_inc[25] < r,
       so we can always use one more byte for p in the current state. */
	s_inc[s_inc[25] >> 3] ^= (uint64_t) p << (8 * (s_inc[25] & 0x07));
	s_inc[(r - 1) >> 3] ^= (uint64_t) 128 << (8 * ((r - 1) & 0x07));
	s_inc[25] = 0;
}

/*************************************************
 * Name:        keccak_inc_squeeze
 *
 * Description: Incremental Keccak squeeze; can be called on byte-level
 *
 * Arguments:   - uint8_t *h: pointer to output bytes
 *              - size_t outlen: number of bytes to be squeezed
 *              - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 **************************************************/
static void keccak_inc_squeeze(uint8_t *h, size_t outlen,
                               uint64_t *s_inc, uint32_t r) {
	size_t i;

	/* First consume any bytes we still have sitting around */
	for (i = 0; i < outlen && i < s_inc[25]; i++) {
		/* There are s_inc[25] bytes left, so r - s_inc[25] is the first
           available byte. We consume from there, i.e., up to r. */
		h[i] = (uint8_t)(s_inc[(r - s_inc[25] + i) >> 3] >> (8 * ((r - s_inc[25] + i) & 0x07)));
	}
	h += i;
	outlen -= i;
	s_inc[25] -= i;

	/* Then squeeze the remaining necessary blocks */
	while (outlen > 0) {
		KeccakF1600_StatePermute(s_inc);

		for (i = 0; i < outlen && i < r; i++) {
			h[i] = (uint8_t)(s_inc[i >> 3] >> (8 * (i & 0x07)));
		}
		h += i;
		outlen -= i;
		s_inc[25] = r - i;
	}
}

void OQS_SHA3_shake128_inc_init(uint64_t *s_inc) {
	keccak_inc_init(s_inc);
}

void OQS_SHA3_shake128_inc_absorb(uint64_t *s_inc, const uint8_t *input, size_t inlen) {
	keccak_inc_absorb(s_inc, SHA3_SHAKE128_RATE, input, inlen);
}

void OQS_SHA3_shake128_inc_finalize(uint64_t *s_inc) {
	keccak_inc_finalize(s_inc, SHA3_SHAKE128_RATE, 0x1F);
}

void OQS_SHA3_shake128_inc_squeeze(uint8_t *output, size_t outlen, uint64_t *s_inc) {
	keccak_inc_squeeze(output, outlen, s_inc, SHA3_SHAKE128_RATE);
}

void OQS_SHA3_shake256_inc_init(uint64_t *s_inc) {
	keccak_inc_init(s_inc);
}

void OQS_SHA3_shake256_inc_absorb(uint64_t *s_inc, const uint8_t *input, size_t inlen) {
	keccak_inc_absorb(s_inc, SHA3_SHAKE256_RATE, input, inlen);
}

void OQS_SHA3_shake256_inc_finalize(uint64_t *s_inc) {
	keccak_inc_finalize(s_inc, SHA3_SHAKE256_RATE, 0x1F);
}

void OQS_SHA3_shake256_inc_squeeze(uint8_t *output, size_t outlen, uint64_t *s_inc) {
	keccak_inc_squeeze(output, outlen, s_inc, SHA3_SHAKE256_RATE);
}