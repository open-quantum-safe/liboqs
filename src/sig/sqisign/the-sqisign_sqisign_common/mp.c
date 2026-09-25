#include <mp.h>
#include <modqx.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include <prng.h>
#include <primality_test.h>
#include <mp_ct.h>
#include <mp_carry.h>

// Count number of words for given bitlength
#define NUM_LIMBS(bits) ((int)(((bits) + (NUM_BITS_LIMB - 1)) / (NUM_BITS_LIMB)))

static inline int
int_min(int a, int b)
{
    return a < b ? a : b;
}
static inline int
int_max(int a, int b)
{
    return a > b ? a : b;
}

const ibz_t ibz_const_zero = {
    .limbs = { 0 },
    .bitlen = 1,
};

const ibz_t ibz_const_one = {
    .limbs = { 1 },
    .bitlen = 2,
};

const ibz_t ibz_const_two = {
    .limbs = { 2 },
    .bitlen = 3,
};

const ibz_t ibz_const_three = {
    .limbs = { 3 },
    .bitlen = 3,
};

// clang-format off
void
ibz_init(ibz_t *x)
{
    ibz_set(x, 0, 0);
}

// clang-format on

#include <mp_carry.h>

// Shift right with flexible datatype
#define SHIFTR(highIn, lowIn, shift, shiftOut, DigitSize)                                                              \
    (shiftOut) = ((lowIn) >> (shift)) ^ ((highIn) << ((DigitSize) - (shift)));

// Digit shift left
#define SHIFTL(highIn, lowIn, shift, shiftOut, DigitSize)                                                              \
    (shiftOut) = ((highIn) << (shift)) ^ ((lowIn) >> (RADIX - (shift)));

static digit_t
mp_shiftr(digit_t *x, const unsigned int shift, const unsigned int nwords)
{ // Multiprecision right shift by 1...RADIX-1
    digit_t bit_out = x[0] & 1;

    for (unsigned int i = 0; i < nwords - 1; i++) {
        SHIFTR(x[i + 1], x[i], shift, x[i], RADIX);
    }
    x[nwords - 1] >>= shift;
    return bit_out;
}

static void
multiple_mp_shiftr(digit_t *x, unsigned int shift, unsigned int nwords)
{
    unsigned int t = shift;
    while (t > NUM_BITS_LIMB - 1) {
        mp_shiftr(x, NUM_BITS_LIMB - 1, nwords);
        t -= (NUM_BITS_LIMB - 1);
    }
    mp_shiftr(x, t, nwords);
}

/**
 * Sets the bits i+1, i+2, ... NUM_BITS_LIMB-1 in the integer pointed to by a to the value of the i-th bit in the
 * same integer.
 */
static void
propagate_bit(digit_t *a, int i)
{
    digit_t v = (digit_t)1 << i;
    *a = (*a & (v - 1)) - (*a & v);
}

/**
 * Makes sure that a
 *
 * - has the correct higher bits in the highest limb, and
 * - is within the interval -2^(bitlen-1)+1 to 2^(bitlen-1)-1, inclusive; if a is equal to -2^(bitlen-1), a is set to
 *   zero.
 *
 * Non-constant-time version.
 */
static void
normalise(ibz_t *a)
{
    assert(a->bitlen > 0);

    int n = NUM_LIMBS(a->bitlen);
    int b = a->bitlen - NUM_BITS_LIMB * (n - 1); // number of bits used in highest limb
    assert(b > 0);
    assert(b <= NUM_BITS_LIMB);
    a->limbs[n - 1] &= ((digit_t)-1) >> (NUM_BITS_LIMB - b); // clear additional bits in highest limb

    // Check if a is -2^(bitlen - 1)
    int i = 0;
    while (i < n - 1 && !a->limbs[i])
        ++i;
    if (i == n) {
        // Set to zero if a is -2^(bitlen - 1)
        a->limbs[0] = 0;
        a->bitlen = 1;
        return;
    }
    propagate_bit(&a->limbs[n - 1], b - 1);
}

void
ibz_set(ibz_t *a, sdigit_t x, int bitlen)
{
    assert(bitlen >= 0 && bitlen <= 32);
    bitlen += !bitlen;
    assert(((digit_t)x & (((digit_t)-1) >> (NUM_BITS_LIMB - bitlen))) != ((digit_t)1 << (bitlen - 1)));
    // Set bitlen
    int flag = bitlen <= 32;
    a->bitlen = flag * bitlen + (1 - flag) * 32;
    // Set value
    a->limbs[0] = x;
    normalise(a);
}

void
ibz_copy(ibz_t *target, const ibz_t *source)
{
    assert(source->bitlen > 0);
    memmove(target->limbs, source->limbs, (size_t)NUM_LIMBS(source->bitlen) * sizeof(digit_t));
    target->bitlen = source->bitlen;
}

void
ibz_swap(ibz_t *a, ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);
    ibz_cswap(a, b, (digit_t)-1);
}

void
ibz_cswap(ibz_t *a, ibz_t *b, digit_t cond)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    digit_t t;
    for (int i = 0; i < IBZ_NLIMBS; i++) {
        t = cond & (a->limbs[i] ^ b->limbs[i]);
        a->limbs[i] ^= t;
        b->limbs[i] ^= t;
    }
    int t1 = ((int)cond) & (a->bitlen ^ b->bitlen);
    a->bitlen ^= t1;
    b->bitlen ^= t1;
}

int
ibz_get_bound(const ibz_t *x)
{
    assert(x->bitlen > 0);
    return x->bitlen;
}

void
ibz_set_bound(ibz_t *a, int bitlen)
{
    assert(a->bitlen > 0);
    assert(bitlen >= 0 && bitlen <= IBZ_NLIMBS * NUM_BITS_LIMB);
    bitlen = int_max(bitlen, 0);
    bitlen = int_min(bitlen, IBZ_NLIMBS * NUM_BITS_LIMB);
    if (bitlen == a->bitlen)
        return;
    if (bitlen > a->bitlen) {
        // Increase bound
        int n0 = NUM_LIMBS(a->bitlen);
        int n1 = NUM_LIMBS(bitlen);
        // Old highest limb has the correct higher bits, just propagate the highest bit to the new limbs. A
        // zero-initialized bitlen-0 input has no highest limb to read: its value is zero, so the propagated bit is 0.
        int bit = a->limbs[n0 - 1] >> (NUM_BITS_LIMB - 1);
        for (int i = n0; i < n1; i++) {
            a->limbs[i] = (digit_t)-bit;
        }
        a->bitlen = bitlen;
    } else {
        // Decrease bound
        a->bitlen = bitlen + !bitlen;
        normalise(a);
    }
}

void
ibz_copy_bits(ibz_t *target, const digit_t *source, int bitlen)
{
    assert(bitlen >= 0 && bitlen + 1 <= IBZ_NLIMBS * NUM_BITS_LIMB);
    // Copy bits
    int n = NUM_LIMBS(bitlen);
    memcpy(target->limbs, source, (size_t)n * sizeof(digit_t));
    target->bitlen = bitlen + 1;
    int n1 = NUM_LIMBS(target->bitlen);
    if (n1 > n) {
        // bitlen is limb-aligned: the sign limb is entirely new
        target->limbs[n1 - 1] = 0;
    } else {
        // Keep only the low j value bits of the top limb, clearing the sign bit and everything above it
        int j = bitlen - NUM_BITS_LIMB * (n1 - 1); // number of value bits in the highest limb, in [1, NUM_BITS_LIMB-1]
        target->limbs[n1 - 1] &= (((digit_t)1) << j) - 1;
    }
}

void
ibz_copy_digits(ibz_t *target, const digit_t *source, int numwords)
{
    ibz_copy_bits(target, source, numwords * NUM_BITS_LIMB);
}

void
ibz_to_digits(digit_t *target, const ibz_t *source)
{
    assert(source->bitlen > 0);
    assert(ibz_is_positive(source));
    size_t bitlen = ibz_bitsize(source);
    assert(NUM_LIMBS(bitlen) <= IBZ_NLIMBS);
    memcpy(target, source->limbs, NUM_LIMBS(bitlen) * sizeof(digit_t));
}

int32_t
ibz_get(const ibz_t *i)
{
    assert(i->bitlen > 0);
    return (int32_t)(i->limbs[0] & ((digit_t)0xFFFFFFFF));
}

int
ibz_convert_to_str(const ibz_t *i, char *str, int base)
{
    assert(i->bitlen > 0);

    if (!str || (base != 10 && base != 16)) {
        return 0;
    }

    int negative = (ibz_is_positive(i) == 0);
    ibz_t absval = { 0 };
    ibz_abs(&absval, i);

    int pos = 0;
    if (negative) {
        str[pos++] = '-';
    }

    if (ibz_is_zero(&absval)) {
        str[pos++] = '0';
        str[pos] = '\0';
        return -1;
    }

    if (base == 16) {
        static const char hex_digits[] = "0123456789abcdef";
        int bits = ibz_bitsize(&absval);
        int nnibbles = (bits + 3) / 4;
        for (int k = nnibbles - 1; k >= 0; k--) {
            int bitpos = k * 4;
            int limb = bitpos / NUM_BITS_LIMB;
            int off = bitpos % NUM_BITS_LIMB;
            uint8_t nibble = (absval.limbs[limb] >> off) & 0xF;
            str[pos++] = hex_digits[nibble];
        }
        str[pos] = '\0';
        return -1;
    }

    char tmp[IBZ_NLIMBS * NUM_BITS_LIMB + 2];
    int len = 0;
    ibz_t ten = { 0 };
    ibz_set(&ten, 10, 5);
    while (!ibz_is_zero(&absval)) {
        ibz_t q = { 0 }, r = { 0 };
        ibz_div(&q, &r, &absval, &ten);
        tmp[len++] = (char)('0' + ibz_get(&r));
        ibz_copy(&absval, &q);
    }
    for (int k = len - 1; k >= 0; k--) {
        str[pos++] = tmp[k];
    }
    str[pos] = '\0';
    return -1;
}

int
ibz_set_from_str(ibz_t *i, const char *str, int base)
{
    if (!str || (base != 10 && base != 16)) {
        return 0;
    }

    memset(i->limbs, 0, sizeof(i->limbs));
    i->bitlen = 1;

    const char *p = str;
    int negative = 0;
    if (*p == '-') {
        negative = 1;
        p++;
    } else if (*p == '+') {
        p++;
    }
    if (*p == '\0') {
        return 0;
    }

    const int capacity = IBZ_NLIMBS * NUM_BITS_LIMB;
    ibz_t base_ibz = { 0 };
    ibz_set(&base_ibz, (sdigit_t)base, (base == 16) ? 6 : 5);

    for (; *p != '\0'; p++) {
        char c = *p;
        int d;
        if (c >= '0' && c <= '9') {
            d = c - '0';
        } else if (base == 16 && c >= 'a' && c <= 'f') {
            d = c - 'a' + 10;
        } else if (base == 16 && c >= 'A' && c <= 'F') {
            d = c - 'A' + 10;
        } else {
            return 0;
        }
        if (d >= base) {
            return 0;
        }

        ibz_t digit_ibz = { 0 };
        ibz_set(&digit_ibz, (sdigit_t)d, (base == 16) ? 6 : 5);
        ibz_mul(i, i, &base_ibz);
        ibz_add(i, i, &digit_ibz);
    }
    i->bitlen = (ibz_bitsize(i) + 1);
    if (negative) {
        ibz_neg(i, i);
    }
    if (i->bitlen > capacity) {
        i->bitlen = capacity;
        return 0;
    }
    return -1;
}

int
ibz_print(const ibz_t *num, int base)
{
    assert(num->bitlen > 0);
    char str[IBZ_NLIMBS * NUM_BITS_LIMB + 2];
    int ret = ibz_convert_to_str(num, str, base);
    if (ret == 0) {
        printf("?ibz_print:Error");
    } else {
        printf("%s", str);
    }
    return ret;
}

void
ibz_add(ibz_t *sum, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    int la = NUM_LIMBS(a->bitlen);
    int lb = NUM_LIMBS(b->bitlen);
    digit_t a_hi = ~(digit_t)ibz_is_positive(a);
    digit_t b_hi = ~(digit_t)ibz_is_positive(b);

    // Addition increases bit precision by 1
    int n = int_max(a->bitlen, b->bitlen) + 1;
    n = int_min(n, IBZ_NLIMBS * NUM_BITS_LIMB);
    sum->bitlen = n;

    unsigned carry = 0;
    for (int i = 0; i < NUM_LIMBS(n); i++) {
        digit_t a_limb = i < la ? a->limbs[i] : a_hi;
        digit_t b_limb = i < lb ? b->limbs[i] : b_hi;
        ADDC(sum->limbs[i], carry, a_limb, b_limb, carry);
    }
}

void
ibz_sub(ibz_t *diff, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    int la = NUM_LIMBS(a->bitlen);
    int lb = NUM_LIMBS(b->bitlen);
    digit_t a_hi = ~(digit_t)ibz_is_positive(a);
    digit_t b_hi = ~(digit_t)ibz_is_positive(b);

    // Subtraction increases bit precision by 1
    int n = int_max(a->bitlen, b->bitlen) + 1;
    n = int_min(n, IBZ_NLIMBS * NUM_BITS_LIMB);
    diff->bitlen = n;

    unsigned int carry = 0;
    for (int i = 0; i < NUM_LIMBS(n); i++) {
        digit_t a_limb = i < la ? a->limbs[i] : a_hi;
        digit_t b_limb = i < lb ? b->limbs[i] : b_hi;
        SUBC(diff->limbs[i], carry, a_limb, b_limb, carry);
    }
}

void
ibz_mul(ibz_t *prod, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    digit_t m1_is_neg = ~(digit_t)ibz_is_positive(a);
    digit_t m2_is_neg = ~(digit_t)ibz_is_positive(b);

    ibz_t m1, m2;
    ibz_cneg(&m1, a, m1_is_neg);
    ibz_cneg(&m2, b, m2_is_neg);

    // Zero-initialized in full: the accumulation below touches res.limbs[i + j] up to IBZ_NLIMBS - 1, which can lie
    // past NUM_LIMBS(res.bitlen)
    ibz_t res = { 0 };
    int n = a->bitlen + b->bitlen - 1;
    n = int_min(n, IBZ_NLIMBS * NUM_BITS_LIMB);
    res.bitlen = n;

    int n1 = NUM_LIMBS(m1.bitlen);
    int n2 = NUM_LIMBS(m2.bitlen);
    for (int i = 0; i < n1; i++) {
        digit_t carry = 0;
        int n3 = int_min(n2, IBZ_NLIMBS - i);
        for (int j = 0; j < n3; j++) {
            ddigit_t prod_ij = (ddigit_t)m1.limbs[i] * m2.limbs[j];
            ddigit_t sum = prod_ij + res.limbs[i + j] + carry;
            res.limbs[i + j] = sum;
            carry = sum >> NUM_BITS_LIMB;
        }
        if (i + n2 < IBZ_NLIMBS)
            res.limbs[i + n2] = carry;
    }

    ibz_cneg(prod, &res, m1_is_neg ^ m2_is_neg);
}

void
ibz_neg(ibz_t *neg, const ibz_t *a)
{
    assert(a->bitlen > 0);
    int n = NUM_LIMBS(a->bitlen);
    digit_t carry = 1;
    for (int i = 0; i < n; i++) {
        ddigit_t t = (ddigit_t)~a->limbs[i] + carry;
        neg->limbs[i] = t;
        carry = t >> NUM_BITS_LIMB;
    }
    neg->bitlen = a->bitlen;
}

void
ibz_cneg(ibz_t *out, const ibz_t *a, digit_t mask)
{
    assert(a->bitlen > 0);
    int n = NUM_LIMBS(a->bitlen);
    ddigit_t carry = (ddigit_t)(mask & 1); // using digit_t was slower
    for (int i = 0; i < n; i++) {
        ddigit_t t = (ddigit_t)(a->limbs[i] ^ mask) + carry;
        out->limbs[i] = (digit_t)t;
        carry = t >> NUM_BITS_LIMB;
    }
    out->bitlen = a->bitlen;
}

void
ibz_abs(ibz_t *abs, const ibz_t *a)
{
    assert(a->bitlen > 0);
    ibz_cneg(abs, a, ~(digit_t)ibz_is_positive(a));
}

static inline void
mp_shl1_bit(digit_t *x, int nwords, digit_t bit_in)
{
    digit_t carry = bit_in;
    for (int i = 0; i < nwords; i++) {
        digit_t v = x[i];
        x[i] = (v << 1) | carry;
        carry = v >> (NUM_BITS_LIMB - 1);
    }
}

/* Counts the number of leading zero bits in a digit_t. NON-CONSTANT-TIME. CT alternative: ct_limb_bit_length */
static unsigned
mp_clz_digit(digit_t x)
{
    unsigned c = 0;
    for (int bit = NUM_BITS_LIMB - 1; bit >= 0; bit--) {
        if ((x >> bit) & 1) {
            break;
        }
        c++;
    }
    return c;
}

// r = a << shift (non-CT), a is an n-word array and shift must be in [0,NUM_BITS_LIMB).
// Returns the bits that fall off the top of word n-1.
static digit_t
mp_shl_words(digit_t *r, const digit_t *a, int n, unsigned shift)
{
    assert(shift < (unsigned)NUM_BITS_LIMB);
    if (shift == 0) {
        if (r != a) {
            memcpy(r, a, (size_t)n * sizeof(digit_t));
        }
        return 0;
    }
    digit_t carry = 0;
    for (int i = 0; i < n; i++) {
        digit_t w = a[i];
        r[i] = (digit_t)((w << shift) | carry);
        carry = (digit_t)(w >> (NUM_BITS_LIMB - shift));
    }
    return carry;
}

// r = a >> shift (non-CT), a is an n-word array and shift must be in [0,NUM_BITS_LIMB).
static void
mp_shr_words(digit_t *r, const digit_t *a, int n, unsigned shift)
{
    if (shift == 0) {
        if (r != a) {
            memcpy(r, a, (size_t)n * sizeof(digit_t));
        }
        return;
    }
    digit_t carry = 0;
    for (int i = n - 1; i >= 0; i--) {
        digit_t w = a[i];
        r[i] = (digit_t)((w >> shift) | carry);
        carry = (digit_t)(w << (NUM_BITS_LIMB - shift));
    }
}

// Non-CT Knuth's Algorithm D (TAOCP vol.2 4.3.1): unsigned u divided by v (v[vlen-1] != 0
// Requires ulen >= vlen >= 1, q gets ulen-vlen+1 words, r gets vlen words.
static void
mp_divmod_schoolbook(digit_t *q, digit_t *r, const digit_t *u, int ulen, const digit_t *v, int vlen)
{
    assert(vlen >= 1);
    assert(v[vlen - 1] != 0);
    assert(ulen >= vlen);

    if (vlen == 1) {
        digit_t d = v[0];
        ddigit_t rem = 0;
        for (int i = ulen - 1; i >= 0; i--) {
            ddigit_t num = (rem << NUM_BITS_LIMB) | (ddigit_t)u[i];
            q[i] = (digit_t)(num / d);
            rem = num % d;
        }
        r[0] = (digit_t)rem;
        return;
    }

    assert(vlen <= IBZ_NLIMBS && ulen <= 2 * IBZ_NLIMBS);
    unsigned shift = mp_clz_digit(v[vlen - 1]);
    digit_t vn[IBZ_NLIMBS];
    mp_shl_words(vn, v, vlen, shift);
    digit_t un[2 * IBZ_NLIMBS + 1];
    digit_t topcarry = mp_shl_words(un, u, ulen, shift);
    un[ulen] = topcarry;

    int qlen = ulen - vlen + 1;
    ddigit_t base = ((ddigit_t)1) << NUM_BITS_LIMB;

    for (int j = qlen - 1; j >= 0; j--) {
        ddigit_t num = ((ddigit_t)un[j + vlen] << NUM_BITS_LIMB) | (ddigit_t)un[j + vlen - 1];
        ddigit_t qhat = num / (ddigit_t)vn[vlen - 1];
        ddigit_t rhat = num % (ddigit_t)vn[vlen - 1];
        if (qhat >= base) {
            qhat = base - 1;
            rhat = num - qhat * (ddigit_t)vn[vlen - 1];
        }

        while (rhat < base && qhat * (ddigit_t)vn[vlen - 2] > (rhat << NUM_BITS_LIMB) + (ddigit_t)un[j + vlen - 2]) {
            qhat -= 1;
            rhat += (ddigit_t)vn[vlen - 1];
        }

        digit_t borrow = 0;
        ddigit_t carry = 0;
        for (int i = 0; i < vlen; i++) {
            ddigit_t prod = qhat * (ddigit_t)vn[i] + carry;
            carry = prod >> NUM_BITS_LIMB;
            digit_t prod_lo = (digit_t)prod;
            digit_t t = un[j + i];
            digit_t sub1 = (digit_t)(t - prod_lo);
            digit_t borrow_a = (t < prod_lo) ? 1 : 0;
            digit_t sub2 = (digit_t)(sub1 - borrow);
            digit_t borrow_b = (sub1 < borrow) ? 1 : 0;
            un[j + i] = sub2;
            borrow = (digit_t)(borrow_a + borrow_b);
        }
        {
            digit_t t = un[j + vlen];
            digit_t c_lo = (digit_t)carry;
            digit_t sub1 = (digit_t)(t - c_lo);
            digit_t borrow_a = (t < c_lo) ? 1 : 0;
            digit_t sub2 = (digit_t)(sub1 - borrow);
            digit_t borrow_b = (sub1 < borrow) ? 1 : 0;
            un[j + vlen] = sub2;
            borrow = (digit_t)(borrow_a + borrow_b);
        }

        if (borrow != 0) {
            qhat -= 1;
            ddigit_t addc = 0;
            for (int i = 0; i < vlen; i++) {
                ddigit_t s = (ddigit_t)un[j + i] + (ddigit_t)vn[i] + addc;
                un[j + i] = (digit_t)s;
                addc = s >> NUM_BITS_LIMB;
            }
            un[j + vlen] = (digit_t)((ddigit_t)un[j + vlen] + addc);
        }

        q[j] = (digit_t)qhat;
    }
    mp_shr_words(r, un, vlen, shift);
}

// r = a + b, unsigned integers, convenient for adding a short value into a longer one.
// Returns the final carry past word rlen-1 (0 or 1): callers must size rlen large enough.
static digit_t
mp_add_raw(digit_t *r, int rlen, const digit_t *a, int alen, const digit_t *b, int blen)
{
    unsigned carry = 0;
    for (int i = 0; i < rlen; i++) {
        digit_t av = (i < alen) ? a[i] : 0;
        digit_t bv = (i < blen) ? b[i] : 0;
        ADDC(r[i], carry, av, bv, carry);
    }
    return (digit_t)carry;
}

// r = a - b, unsigned integers, returns 1 iff the true a-b is negative,
// in which case r holds the two's-complement a-b+2^(rlen*NUM_BITS_LIMB).
static digit_t
mp_sub_raw(digit_t *r, int rlen, const digit_t *a, int alen, const digit_t *b, int blen)
{
    unsigned borrow = 0;
    for (int i = 0; i < rlen; i++) {
        digit_t av = (i < alen) ? a[i] : 0;
        digit_t bv = (i < blen) ? b[i] : 0;
        SUBC(r[i], borrow, av, bv, borrow);
    }
    return (digit_t)borrow;
}

// dst[0..n-1] += src[0..n-1]*limb (in place), returning the carry past word n-1 (non-CT).
static digit_t
mp_addmul_1_raw(digit_t *dst, const digit_t *src, int n, digit_t limb)
{
    ddigit_t carry = 0;
    int j = 0;
    for (; j + 4 <= n; j += 4) {
        ddigit_t t0 = (ddigit_t)limb * (ddigit_t)src[j] + (ddigit_t)dst[j] + carry;
        dst[j + 0] = (digit_t)t0;
        carry = t0 >> NUM_BITS_LIMB;
        ddigit_t t1 = (ddigit_t)limb * (ddigit_t)src[j + 1] + (ddigit_t)dst[j + 1] + carry;
        dst[j + 1] = (digit_t)t1;
        carry = t1 >> NUM_BITS_LIMB;
        ddigit_t t2 = (ddigit_t)limb * (ddigit_t)src[j + 2] + (ddigit_t)dst[j + 2] + carry;
        dst[j + 2] = (digit_t)t2;
        carry = t2 >> NUM_BITS_LIMB;
        ddigit_t t3 = (ddigit_t)limb * (ddigit_t)src[j + 3] + (ddigit_t)dst[j + 3] + carry;
        dst[j + 3] = (digit_t)t3;
        carry = t3 >> NUM_BITS_LIMB;
    }
    for (; j < n; j++) {
        ddigit_t t = (ddigit_t)limb * (ddigit_t)src[j] + (ddigit_t)dst[j] + carry;
        dst[j] = (digit_t)t;
        carry = t >> NUM_BITS_LIMB;
    }
    return (digit_t)carry;
}

#define MP_MUL_SWAP_RATIO_THRESHOLD 4

// prod = a*b, plain schoolbook, prod must have exactly alen+blen words (non-CT).
static void
mp_mul_raw(digit_t *prod, const digit_t *a, int alen, const digit_t *b, int blen)
{
    memset(prod, 0, (size_t)(alen + blen) * sizeof(digit_t));

    const digit_t *outer, *inner;
    int outer_len, inner_len;
    int longer_len = int_max(alen, blen);
    int shorter_len = int_min(alen, blen);
    int worth_swapping = (longer_len >= MP_MUL_SWAP_RATIO_THRESHOLD * shorter_len);
    if (worth_swapping && alen > blen) {
        outer = b;
        outer_len = blen;
        inner = a;
        inner_len = alen;
    } else {
        outer = a;
        outer_len = alen;
        inner = b;
        inner_len = blen;
    }

    for (int i = 0; i < outer_len; i++) {
        digit_t oi = outer[i];
        if (oi == 0) {
            continue;
        }
        digit_t carry = mp_addmul_1_raw(prod + i, inner, inner_len, oi);
        int k = i + inner_len;
        while (carry != 0) {
            ddigit_t t = (ddigit_t)prod[k] + carry;
            prod[k] = (digit_t)t;
            carry = (digit_t)(t >> NUM_BITS_LIMB);
            k++;
        }
    }
}

// Adds X into R at word-offset off, as if X is shifted left by `off` words first (non-CT).
static void
mp_add_inplace_at(digit_t *R, int rlen, const digit_t *X, int xlen, int off)
{
    (void)rlen;
    ddigit_t carry = 0;
    int i = 0;
    for (; i < xlen; i++) {
        int idx = off + i;
        assert(idx < rlen);
        ddigit_t s = (ddigit_t)R[idx] + (ddigit_t)X[i] + carry;
        R[idx] = (digit_t)s;
        carry = s >> NUM_BITS_LIMB;
    }
    while (carry != 0) {
        int idx = off + i;
        assert(idx < rlen);
        ddigit_t s = (ddigit_t)R[idx] + carry;
        R[idx] = (digit_t)s;
        carry = s >> NUM_BITS_LIMB;
        i++;
    }
}

// a -= 1, a being a len-word unsigned array (non-CT).
static void
mp_dec1_raw(digit_t *a, int len)
{
    for (int i = 0; i < len; i++) {
        if (a[i] != 0) {
            a[i]--;
            return;
        }
        a[i] = (digit_t)-1;
    }
}

// Burnikel-Ziegler Alg.1 (D2n1n): divides a 2n-word A by an n-word B, B[n-1] != 0,
// producing a quotient Q of n+1 words and a remainder R of n words (0 <= R < B).
static void mp_div_d2n1n(digit_t *Q, digit_t *R, const digit_t *A, const digit_t *B, int n);

// Burnikel-Ziegler Alg.2 (D3n2n): divides a 3n-word A by a 2n-word B, B[2n-1] != 0,
// producing a quotient Qhat of n+1 words and a remainder R of 2n words (0 <= R < B).
static void
mp_div_d3n2n(digit_t *Qhat, digit_t *R, const digit_t *A, const digit_t *B, int n)
{
    assert(n <= IBZ_NLIMBS / 2);
    const digit_t *B0 = B;
    const digit_t *B1 = B + n;
    const digit_t *A0 = A;
    const digit_t *AH = A + n;

    digit_t R1[IBZ_NLIMBS / 2];
    mp_div_d2n1n(Qhat, R1, AH, B1, n);

    int L = 2 * n + 1;
    digit_t Rbuf[IBZ_NLIMBS + 1];
    memset(Rbuf, 0, (size_t)L * sizeof(digit_t));
    memcpy(Rbuf, A0, (size_t)n * sizeof(digit_t));
    memcpy(Rbuf + n, R1, (size_t)n * sizeof(digit_t));

    digit_t prod[IBZ_NLIMBS + 1];
    mp_mul_raw(prod, Qhat, n + 1, B0, n);

    digit_t borrow = mp_sub_raw(Rbuf, L, Rbuf, L, prod, L);
    while (borrow) {
        mp_dec1_raw(Qhat, n + 1);
        digit_t addcarry = mp_add_raw(Rbuf, L, Rbuf, L, B, 2 * n);
        borrow = addcarry ? 0 : 1;
    }
    memcpy(R, Rbuf, (size_t)(2 * n) * sizeof(digit_t));
}

// mp_div_d2n1n only recurses when n >= 2 and is even, if not, it falls back to mp_divmod_schoolbook.
// NB: A larger threshold could be used, schoolbook may be well faster than the recursion for small n.
#define MP_BZ_SHOULD_RECURSE(n) (((n) % 2 == 0) && ((n) >= 2))

static void
mp_div_d2n1n(digit_t *Q, digit_t *R, const digit_t *A, const digit_t *B, int n)
{
    assert(n <= IBZ_NLIMBS);
    if (!MP_BZ_SHOULD_RECURSE(n)) {
        mp_divmod_schoolbook(Q, R, A, 2 * n, B, n);
        return;
    }
    int n2 = n / 2;
    const digit_t *A0 = A;
    const digit_t *A1 = A + n2;

    digit_t Q1[IBZ_NLIMBS / 2 + 1], R1[IBZ_NLIMBS];
    mp_div_d3n2n(Q1, R1, A1, B, n2);

    digit_t AA[IBZ_NLIMBS + IBZ_NLIMBS / 2];
    memcpy(AA, A0, (size_t)n2 * sizeof(digit_t));
    memcpy(AA + n2, R1, (size_t)n * sizeof(digit_t));

    digit_t Q2[IBZ_NLIMBS / 2 + 1];
    mp_div_d3n2n(Q2, R, AA, B, n2);

    memset(Q, 0, (size_t)(n + 1) * sizeof(digit_t));
    memcpy(Q, Q2, (size_t)(n2 + 1) * sizeof(digit_t));
    mp_add_inplace_at(Q, n + 1, Q1, n2 + 1, n2);
}

// Burnikel-Ziegler's unsigned division of u by v (non-CT), via blockwise long division
// using mp_div_d2n1n as the "single big digit" quotient step (block size n = vlen).
// u may have any length >= 1 word (including ulen < vlen, or leading zero words).
// v must be trimmed to its true significant length (v[vlen-1] != 0).
static void
mp_div_unsigned(digit_t *q, digit_t *r, const digit_t *u, int ulen, const digit_t *v, int vlen)
{
    assert(vlen >= 1);
    assert(v[vlen - 1] != 0);
    assert(ulen <= IBZ_NLIMBS && vlen <= IBZ_NLIMBS);
    int n = vlen;

    unsigned shift = mp_clz_digit(v[n - 1]);
    digit_t vn[IBZ_NLIMBS];
    mp_shl_words(vn, v, n, shift);

    int un_len = ulen + 1;
    digit_t un[IBZ_NLIMBS + 1];
    digit_t topcarry = mp_shl_words(un, u, ulen, shift);
    un[ulen] = topcarry;

    int t = un_len / n;
    if (un_len % n) {
        t++;
    }
    int qbuf_len = t * n + 1;

    memset(q, 0, (size_t)qbuf_len * sizeof(digit_t));

    digit_t rem[IBZ_NLIMBS];
    memset(rem, 0, (size_t)n * sizeof(digit_t));

    for (int i = t - 1; i >= 0; i--) {
        digit_t window[2 * IBZ_NLIMBS];
        for (int k = 0; k < n; k++) {
            int j = i * n + k;
            window[k] = (j < un_len) ? un[j] : 0;
        }
        if (i == t - 1) {
            digit_t borrow = mp_sub_raw(rem, n, window, n, vn, n);
            if (borrow) {
                memcpy(rem, window, (size_t)n * sizeof(digit_t));
            } else {
                q[i * n] = 1;
            }
            continue;
        }
        memcpy(window + n, rem, (size_t)n * sizeof(digit_t));
        digit_t qi[IBZ_NLIMBS + 1];
        mp_div_d2n1n(qi, rem, window, vn, n);
        mp_add_inplace_at(q, qbuf_len, qi, n + 1, i * n);
    }
    mp_shr_words(r, rem, n, shift);
}

// Returns the exact word count mp_div_unsigned for q (non-CT).
static int
mp_div_qlen(int ulen, int vlen)
{
    int n = vlen;
    int un_len = ulen + 1;
    int t = un_len / n;
    if (un_len % n) {
        t++;
    }
    return t * n + 1;
}

// Variable-time (non-constant-time) Euclidean division of a by b.
// Uses the Burnikel-Ziegler recursive division algorithm, except for special cases:
//  - |a| < |b|: the quotient is trivially 0 and the remainder is just |a|.
//  - divisor size that Burnikel-Ziegler can't recurse on (MP_BZ_SHOULD_RECURSE false, i.e.
//    vlen odd or < 2): mp_div_unsigned's block-splitting over such a vlen buys nothing --
//    every block just falls back to schoolbook anyway -- so call mp_divmod_schoolbook
//    directly on the whole operands and skip the block/window copying overhead.
void
ibz_div(ibz_t *quotient, ibz_t *remainder, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    assert(quotient != remainder);
    assert(!ibz_is_zero(b));

    int n = int_max(a->bitlen, b->bitlen);
    int L = NUM_LIMBS(n);

    digit_t a_neg = ~(digit_t)ibz_is_positive(a);
    digit_t b_neg = ~(digit_t)ibz_is_positive(b);

    ibz_t abs_a = { 0 }, abs_b = { 0 };
    ibz_cneg(&abs_a, a, a_neg);
    ibz_cneg(&abs_b, b, b_neg);

    int ulen = L;
    while (ulen > 1 && abs_a.limbs[ulen - 1] == 0) {
        ulen--;
    }
    int vlen = L;
    while (vlen > 1 && abs_b.limbs[vlen - 1] == 0) {
        vlen--;
    }

    memset(quotient->limbs, 0, L * sizeof(digit_t));
    memset(remainder->limbs, 0, L * sizeof(digit_t));

    int a_lt_b = ulen < vlen;
    if (!a_lt_b && ulen == vlen) {
        for (int i = ulen - 1; i >= 0; i--) {
            if (abs_a.limbs[i] != abs_b.limbs[i]) {
                a_lt_b = abs_a.limbs[i] < abs_b.limbs[i];
                break;
            }
        }
    }

    if (a_lt_b) {
        memcpy(remainder->limbs, abs_a.limbs, (size_t)ulen * sizeof(digit_t));
    } else if (!MP_BZ_SHOULD_RECURSE(vlen)) {
        mp_divmod_schoolbook(quotient->limbs, remainder->limbs, abs_a.limbs, ulen, abs_b.limbs, vlen);
    } else {
        int qbuf_len = mp_div_qlen(ulen, vlen);

        digit_t qbuf[2 * IBZ_NLIMBS + 1];
        digit_t rbuf[IBZ_NLIMBS];
        mp_div_unsigned(qbuf, rbuf, abs_a.limbs, ulen, abs_b.limbs, vlen);

        int qcopy = int_min(qbuf_len, L);
        memcpy(quotient->limbs, qbuf, (size_t)qcopy * sizeof(digit_t));
        memcpy(remainder->limbs, rbuf, (size_t)vlen * sizeof(digit_t));
    }

    quotient->bitlen = a->bitlen;
    remainder->bitlen = b->bitlen;

    if (a_neg != b_neg) {
        ibz_neg(quotient, quotient);
    }
    if (a_neg) {
        ibz_neg(remainder, remainder);
    }
}

void
ibz_div_2exp(ibz_t *quotient, const ibz_t *a, uint32_t exp)
{
    assert(a->bitlen > 0);

    ibz_copy(quotient, a);

    if (!exp)
        return;

    assert(exp <= IBZ_NLIMBS * NUM_BITS_LIMB);
    int iexp = int_min(exp, IBZ_NLIMBS * NUM_BITS_LIMB);

    multiple_mp_shiftr(quotient->limbs, iexp, NUM_LIMBS(a->bitlen));
    quotient->bitlen = iexp < a->bitlen ? a->bitlen - iexp : 1;
}

static inline void
mp_shiftl(digit_t *x, const unsigned int shift, const unsigned int nwords)
{ // Multiprecision left shift by 1...RADIX-1

    for (int i = nwords - 1; i > 0; i--) {
        SHIFTL(x[i], x[i - 1], shift, x[i], RADIX);
    }
    x[0] <<= shift;
}

/**
 * In-place logical left shift of x by `shift` bits.
 * NON-CT.
 */
static inline void
multiple_mp_shiftl(digit_t *x, const unsigned int shift, const unsigned int nwords)
{
    const int word_shift = (int)(shift / RADIX);
    const unsigned int bit_shift = shift % RADIX;

    for (int i = (int)nwords - 1; i >= 0; i--) {
        const int src = i - word_shift;
        const digit_t lo = (src >= 0) ? x[src] : 0;
        const digit_t hi = (src - 1 >= 0) ? x[src - 1] : 0;
        x[i] = (bit_shift == 0) ? lo : ((lo << bit_shift) | (hi >> (RADIX - bit_shift)));
    }
}

/**
 * Constant-time ibz_set_bound. Bitlen is public, value not.
 */
void
ibz_set_bound_ct(ibz_t *a, int bitlen)
{
    assert(a->bitlen > 0);
    assert(bitlen >= 0 && bitlen <= IBZ_NLIMBS * NUM_BITS_LIMB);
    bitlen = int_max(bitlen, 0);
    bitlen = int_min(bitlen, IBZ_NLIMBS * NUM_BITS_LIMB);

    if (bitlen >= a->bitlen) {
        int n0 = NUM_LIMBS(a->bitlen);
        int n1 = NUM_LIMBS(bitlen);
        int bit = (n0 > 0) ? (int)(a->limbs[n0 - 1] >> (NUM_BITS_LIMB - 1)) : 0;
        for (int i = n0; i < n1; i++)
            a->limbs[i] = (digit_t)-bit;
        a->bitlen = bitlen;
    } else {
        a->bitlen = bitlen + (bitlen == 0);
        int n = NUM_LIMBS(a->bitlen);
        int b = a->bitlen - NUM_BITS_LIMB * (n - 1);
        digit_t mask_p = (b >= NUM_BITS_LIMB) ? (digit_t)-1 : (((digit_t)1 << b) - 1);
        digit_t signmask = (digit_t)0 - ((a->limbs[n - 1] >> (b - 1)) & (digit_t)1);
        a->limbs[n - 1] = (a->limbs[n - 1] & mask_p) | (signmask & ~mask_p);
    }
}

/**
 * Constant-time ibz_bitsize.
 */
int
ibz_bitsize_ct(const ibz_t *a)
{
    int nwords = NUM_LIMBS(a->bitlen);
    if (nwords == 0)
        return 0;
    ibz_t abs_a = { 0 };
    ibz_cneg(&abs_a, a, ~(digit_t)ibz_is_positive(a));
    digit_t top_word = 0;
    int top_idx = 0;
    unsigned int found = 0;
    for (int i = nwords - 1; i >= 0; i--) {
        digit_t w = abs_a.limbs[i];
        unsigned int nz = ct_is_digit_nonzero(w);
        // all-ones on the first nonzero limb.
        digit_t take = ct_barrier_digit((digit_t)0 - (digit_t)(nz & (unsigned int)(1 - found)));
        top_word = (top_word & ~take) | (w & take);
        top_idx = (int)(((digit_t)top_idx & ~take) | ((digit_t)i & take));
        found |= nz;
    }
    return top_idx * NUM_BITS_LIMB + (int)ct_limb_bit_length(top_word);
}

void
ibz_mul_2exp(ibz_t *product, const ibz_t *a, uint32_t exp)
{
    assert(a->bitlen > 0);

    ibz_copy(product, a);

    if (!exp)
        return;

    assert(exp <= IBZ_NLIMBS * NUM_BITS_LIMB);
    int iexp = int_min(exp, IBZ_NLIMBS * NUM_BITS_LIMB);

    int bitlen_out = a->bitlen + iexp;
    bitlen_out = int_min(bitlen_out, IBZ_NLIMBS * NUM_BITS_LIMB);
    // only increase-branch of normalise is CT.
    // not happening but assert to catch this in the future.
    assert(bitlen_out >= product->bitlen);
    ibz_set_bound(product, bitlen_out);

    multiple_mp_shiftl(product->limbs, iexp, NUM_LIMBS(bitlen_out));
}

static int
digit_tailzerolen(digit_t x)
{
#ifdef __has_builtin
#if __has_builtin(__builtin_ctzg)
    return __builtin_ctzg(x, 0);
#endif
#endif
    int n = 0;
    for (int shift = 8 * sizeof(digit_t) / 2; shift >= 1; shift >>= 1) {
        digit_t low_mask = ((digit_t)1 << shift) - 1;
        digit_t hi = x >> shift;
        unsigned int low_is_zero = 1 - ct_is_digit_nonzero(x & low_mask);
        digit_t mask = (digit_t)0 - (digit_t)low_is_zero;
        x = ((x ^ hi) & mask) ^ x;
        n += (int)((digit_t)shift & mask);
    }
    return n;
}

int
ibz_two_adic(const ibz_t *pow)
{
    assert(pow->bitlen > 0);

    int nwords = NUM_LIMBS(pow->bitlen);

    int result = 0;
    unsigned int found = 0;

    for (int i = 0; i < nwords; i++) {
        digit_t w = pow->limbs[i];
        unsigned int nz = ct_is_digit_nonzero(w);
        int candidate = i * 8 * sizeof(digit_t) + digit_tailzerolen(w);

        int mask = -(int)(nz & (unsigned int)(1 - found));
        result = ((result ^ candidate) & mask) ^ result;
        found |= nz;
        if (found)
            break; // remove this line for a constant-time version
    }

    return result;
}

// Variable-time (non-constant-time) r = a mod b.
void
ibz_mod(ibz_t *r, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);
    assert(!ibz_is_zero(b));
    assert(b != r);

    int n = int_max(a->bitlen, b->bitlen);
    int L = NUM_LIMBS(n);

    digit_t a_neg = ~(digit_t)ibz_is_positive(a);
    digit_t b_neg = ~(digit_t)ibz_is_positive(b);

    ibz_t abs_a = { 0 }, abs_b = { 0 };
    ibz_cneg(&abs_a, a, a_neg);
    ibz_cneg(&abs_b, b, b_neg);

    int ulen = L;
    while (ulen > 1 && abs_a.limbs[ulen - 1] == 0) {
        ulen--;
    }
    int vlen = L;
    while (vlen > 1 && abs_b.limbs[vlen - 1] == 0) {
        vlen--;
    }

    memset(r->limbs, 0, L * sizeof(digit_t));

    int a_lt_b = ulen < vlen;
    if (!a_lt_b && ulen == vlen) {
        for (int i = ulen - 1; i >= 0; i--) {
            if (abs_a.limbs[i] != abs_b.limbs[i]) {
                a_lt_b = abs_a.limbs[i] < abs_b.limbs[i];
                break;
            }
        }
    }

    if (a_lt_b) {
        memcpy(r->limbs, abs_a.limbs, (size_t)ulen * sizeof(digit_t));
    } else if (!MP_BZ_SHOULD_RECURSE(vlen)) {
        digit_t qscratch[IBZ_NLIMBS];
        mp_divmod_schoolbook(qscratch, r->limbs, abs_a.limbs, ulen, abs_b.limbs, vlen);
    } else {
        digit_t qbuf[2 * IBZ_NLIMBS + 1];
        digit_t rbuf[IBZ_NLIMBS];
        mp_div_unsigned(qbuf, rbuf, abs_a.limbs, ulen, abs_b.limbs, vlen);
        memcpy(r->limbs, rbuf, (size_t)vlen * sizeof(digit_t));
    }

    r->bitlen = b->bitlen;
    if (a_neg) {
        ibz_neg(r, r);
    }

    if (!ibz_is_positive(r)) {
        if (b_neg) {
            ibz_sub(r, r, b);
        } else {
            ibz_add(r, r, b);
        }
    }
}

static void
mp_mod_2exp(digit_t *a, unsigned int e, unsigned int nwords)
{ // Multiprecision modulo 2^e, with 0 <= a < 2^(e)
    unsigned int i, q = e >> LOG2RADIX, r = e & (RADIX - 1);

    if (q < nwords) {
        a[q] &= ((digit_t)1 << r) - 1;

        for (i = q + 1; i < nwords; i++) {
            a[i] = 0;
        }
    }
}

void
ibz_mod2exp(ibz_t *r, const ibz_t *a, const uint32_t exp)
{
    assert(a->bitlen > 0);
    assert((int)exp + 1 <= IBZ_NLIMBS * NUM_BITS_LIMB);

    ibz_copy(r, a);
    ibz_set_bound(r, (int)exp + 1);
    mp_mod_2exp(r->limbs, exp, (unsigned int)NUM_LIMBS((int)exp + 1));
}

unsigned long int
ibz_mod_ui(const ibz_t *n, unsigned long int d)
{
    assert(n->bitlen > 0);
    assert(d != 0);

    const int bits_uli = (int)(8 * sizeof(unsigned long int));
    const int limbs_uli = (bits_uli + NUM_BITS_LIMB - 1) / NUM_BITS_LIMB;

    // Do not use limbs_uli here, as it causes a gnu-folding-constant warning
    digit_t d_digits[((int)(8 * sizeof(unsigned long int)) + NUM_BITS_LIMB - 1) / NUM_BITS_LIMB];
    for (int i = 0; i < limbs_uli; i++) {
        int w = i * NUM_BITS_LIMB;
        d_digits[i] = (w < bits_uli) ? (digit_t)(d >> w) : 0;
    }

    ibz_t d_ibz = { 0 };
    ibz_copy_bits(&d_ibz, d_digits, bits_uli);

    ibz_t r = { 0 };
    ibz_mod(&r, n, &d_ibz);

    unsigned long int result = 0;
    for (int i = limbs_uli - 1; i >= 0; i--) {
        int w = i * NUM_BITS_LIMB;
        if (w < bits_uli) {
            result |= ((unsigned long int)r.limbs[i]) << w;
        }
    }
    return result;
}

int
ibz_divides(const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);
    assert(!ibz_is_zero(b));

    ibz_t r = { 0 };
    ibz_mod(&r, a, b);
    return ibz_is_zero(&r);
}

void
ibz_pow(ibz_t *pow, const ibz_t *x, uint32_t e, int ebits)
{
    assert(x->bitlen > 0);
    (void)ebits; // not needed unless we want constant-time

    ibz_t r0 = { 0 }, r1 = { 0 };
    ibz_copy(&r0, &ibz_const_one);
    ibz_copy(&r1, x);
    while (e) {
        if (e & 1)
            ibz_mul(&r0, &r0, &r1);
        if (e >>= 1)
            ibz_mul(&r1, &r1, &r1);
    }
    ibz_copy(pow, &r0);
}

static void
ibz_to_modq(spint *a, const ibz_t *x)
{
    assert(x->bitlen > 0);
    assert(ibz_is_positive(x));
    int nlimbs = NUM_LIMBS(x->bitlen);
    for (int i = 0; i < MODQLIMBS; i++) {
        int bitpos = i * MODQRADIX;
        if (bitpos >= x->bitlen) {
            a[i] = 0;
            continue;
        }
        int limb = bitpos / NUM_BITS_LIMB;
        int offset = bitpos % NUM_BITS_LIMB;
        ddigit_t v = (ddigit_t)x->limbs[limb] >> offset;
        if (offset != 0 && offset + MODQRADIX > NUM_BITS_LIMB && limb + 1 < nlimbs) {
            v |= (ddigit_t)x->limbs[limb + 1] << (NUM_BITS_LIMB - offset);
        }
        a[i] = (spint)v & MODQLIMBMASK;
    }
}

static void
ibz_from_modq(ibz_t *x, const spint *a, int numwords)
{
    int i;
    spint aa[MODQLIMBS];
    assert(numwords < MODQLIMBS);
    assert(numwords * MODQRADIX < (int)(IBZ_NLIMBS * NUM_BITS_LIMB - 1));
    modqx_modcpy(a, aa, numwords);
    ibz_set(x, 0, 1);
    for (i = numwords - 1; i >= 0; i--) {
        ibz_mul_2exp(x, x, MODQRADIX);
        x->limbs[0] |= (aa[i] & MODQLIMBMASK);
    }
    x->bitlen = numwords * MODQRADIX + 1;
}

void
ibz_pow_mod(ibz_t *res, const ibz_t *x, const ibz_t *e, const ibz_t *p)
{
    assert(x->bitlen > 0);
    assert(e->bitlen > 0);
    assert(p->bitlen > 0);
    spint pp[MODQLIMBS], ee[MODQLIMBS], xx[MODQLIMBS], two_p[MODQLIMBS], ndash, table[TABLE_SIZE][MODQLIMBS];
    int pbitlen = p->bitlen;
    // 2 extra bits of headroom (p < 2^(numwords*MODQRADIX)/4) are required for modqx_modmul's <2p output bound to hold
    int numwords = (p->bitlen + MODQRADIX + 1) / MODQRADIX;
    assert(numwords < MODQLIMBS);
    assert(ibz_is_positive(p));
    assert(ibz_is_positive(x));
    assert(ibz_is_positive(e));
    assert(ibz_cmp(x, p) < 0);
    assert(ibz_cmp(e, p) < 0);
    ibz_to_modq(pp, p);
    modqx_modcpy(pp, two_p, numwords);
    modqx_modshl(1, two_p, numwords);
    ibz_to_modq(xx, x);
    modqx_nresx(xx, xx, two_p, numwords);
    ibz_to_modq(ee, e);
    ndash = modqx_getndash(pp[0]);
    modqx_modone(table[0], two_p, numwords);
#if WINDOW_SIZE == 1
    modqx_modxpowe(pp, table[0], ndash, xx, ee, xx, numwords);
#else
    modqx_modxpowe_windowed(pp, two_p, ndash, xx, ee, table, xx, numwords);
#endif
    modqx_redc(xx, xx, pp, ndash, numwords);
    ibz_from_modq(res, xx, numwords);
    res->bitlen = pbitlen;
}

static int
ibz_bit_at(const ibz_t *x, int i)
{
    assert(x->bitlen > 0);
    return (int)((x->limbs[i / NUM_BITS_LIMB] >> (i % NUM_BITS_LIMB)) & 1);
}

// Variable-time left-to-right sliding-window modular exponentiation:
// computes s = x^e mod n, entirely in Montgomery form, x must already
// be converted in on entry; s is left in Montgomery form on exit.
static void
modxpowe_non_ct(spint s[MODQLIMBS],
                const spint x[MODQLIMBS],
                const ibz_t *e,
                const spint n[MODQLIMBS],
                const spint two_n[MODQLIMBS],
                const spint ndash,
                int numwords,
                int window)
{
    assert(window >= 1 && window <= 6);
    // precompute x^1, x^3, x^5, ...
    int table_size = 1 << (window - 1);
    spint table[32][MODQLIMBS];
    modqx_modcpy(x, table[0], numwords);
    if (window > 1) {
        spint x2[MODQLIMBS];
        modqx_modsqr(x, x2, n, ndash, numwords);
        for (int i = 1; i < table_size; i++)
            modqx_modmul(table[i - 1], x2, table[i], n, ndash, numwords);
    }
    int nb = ibz_bitsize(e);
    if (nb == 0) {
        modqx_modone(s, two_n, numwords);
        return;
    }
    // Find first non-zero window
    int i = nb - 1;
    int wlen = (i + 1 < window) ? (i + 1) : window;
    while (wlen > 1 && !ibz_bit_at(e, i - wlen + 1))
        wlen--;
    int wval = 0;
    for (int k = 0; k < wlen; k++)
        wval = (wval << 1) | ibz_bit_at(e, i - k);
    modqx_modcpy(table[(wval - 1) / 2], s, numwords);
    i -= wlen;
    // main loop
    while (i >= 0) {
        if (!ibz_bit_at(e, i)) {
            modqx_modsqr(s, s, n, ndash, numwords);
            i--;
            continue;
        }
        wlen = (i + 1 < window) ? (i + 1) : window;
        while (wlen > 1 && !ibz_bit_at(e, i - wlen + 1))
            wlen--;
        wval = 0;
        for (int k = 0; k < wlen; k++)
            wval = (wval << 1) | ibz_bit_at(e, i - k);
        for (int k = 0; k < wlen; k++)
            modqx_modsqr(s, s, n, ndash, numwords);
        modqx_modmul(s, table[(wval - 1) / 2], s, n, ndash, numwords);
        i -= wlen;
    }
}

// Computes a square root of -1 modulo m, assuming that m is a prime congruent to 1 mod 4.
void
ibz_sqrt_m1_mod(ibz_t *res, const ibz_t *p)
{
    assert(p->bitlen > 0);

    ibz_t x = { 0 }, e = { 0 };
    int pbitlen = p->bitlen;
    int window = (ibz_bitsize(p) <= 160) ? 4 : 6;
    assert(window >= 1 && window <= 6);
    // 2 extra bits of headroom (p < 2^(numwords*MODQRADIX)/4) are required for modqx_modmul's <2p output bound to hold
    int numwords = (p->bitlen + MODQRADIX + 1) / MODQRADIX;
    assert(numwords < MODQLIMBS);
    spint pp[MODQLIMBS], xx[MODQLIMBS], yy[MODQLIMBS], two_p[MODQLIMBS], one[MODQLIMBS], ndash;

    assert(ibz_is_positive(p));
    assert((ibz_get(p) & 3) == 1); // check m=1 mod 4
    assert(p->bitlen + 64 < IBZ_NLIMBS * NUM_BITS_LIMB);
    ibz_to_modq(pp, p);
    modqx_modcpy(pp, two_p, numwords);
    modqx_modshl(1, two_p, numwords);
    ibz_div_2exp(&e, p, 2);
    ndash = modqx_getndash(pp[0]);
    modqx_modone(one, two_p, numwords);
    while (1) {
        ibz_rand_interval(&x, &ibz_const_zero, p);
        ibz_to_modq(xx, &x);
        modxpowe_non_ct(xx, xx, &e, pp, two_p, ndash, numwords, window);
        modqx_modsqr(xx, yy, pp, ndash, numwords);
        modqx_modadd(yy, one, yy, two_p, numwords);
        if (modqx_modis0(yy, pp, ndash, numwords))
            break;
    }
    modqx_redc(xx, xx, pp, ndash, numwords);
    ibz_from_modq(res, xx, numwords);
    res->bitlen = pbitlen;
}

// Variable-time Tonelli-Shanks square root modulo p = 1 mod 4
static int
ibz_sqrt_mod_p_1mod4(ibz_t *sqrt, const ibz_t *a, const ibz_t *p)
{
    assert(a->bitlen > 0);
    assert(p->bitlen > 0);

    int pbitlen = p->bitlen;
    int numwords = (p->bitlen + MODQRADIX + 1) / MODQRADIX;
    int window = (ibz_bitsize(p) <= 160) ? 4 : 6;
    assert(window >= 1 && window <= 6);
    assert(p->bitlen + 64 < IBZ_NLIMBS * NUM_BITS_LIMB);
    assert((ibz_get(p) & 3) == 1); // check p=1 mod 4
    assert(ibz_is_positive(p));
    assert(ibz_is_positive(a));
    assert(ibz_cmp(a, p) < 0);

    // Factor p-1 = q * 2^s with q odd
    ibz_t pm1 = { 0 }, q = { 0 }, qp1o2 = { 0 }, z = { 0 };
    ibz_sub(&pm1, p, &ibz_const_one);
    int s = ibz_two_adic(&pm1);
    ibz_div_2exp(&q, &pm1, s);
    ibz_add(&qp1o2, &q, &ibz_const_one);
    ibz_div_2exp(&qp1o2, &qp1o2, 1);

    // Find a fixed quadratic non-residue z; variable time, but independent of the secret a
    do {
        ibz_rand_interval(&z, &ibz_const_one, &pm1);
    } while (ibz_legendre(&z, p) != -1);

    spint pp[MODQLIMBS], two_p[MODQLIMBS], one[MODQLIMBS], ndash;
    spint aa[MODQLIMBS], zz[MODQLIMBS], cc[MODQLIMBS], tt[MODQLIMBS], RR[MODQLIMBS];
    spint bb[MODQLIMBS], bsq[MODQLIMBS], tmp[MODQLIMBS];
    ibz_to_modq(pp, p);
    modqx_modcpy(pp, two_p, numwords);
    modqx_modshl(1, two_p, numwords);
    ndash = modqx_getndash(pp[0]);
    modqx_modone(one, two_p, numwords);

    ibz_to_modq(aa, a);
    modqx_nresx(aa, aa, two_p, numwords);
    ibz_to_modq(zz, &z);
    modqx_nresx(zz, zz, two_p, numwords);

    modxpowe_non_ct(cc, zz, &q, pp, two_p, ndash, numwords, window);     // c = z^q
    modxpowe_non_ct(tt, aa, &q, pp, two_p, ndash, numwords, window);     // t = a^q
    modxpowe_non_ct(RR, aa, &qp1o2, pp, two_p, ndash, numwords, window); // R = a^((q+1)/2)

    int M = s;
    while (!modqx_modcmp(tt, one, pp, ndash, numwords)) {
        // Find the least i in (0, M) such that t^(2^i) = 1
        modqx_modcpy(tt, tmp, numwords);
        int i = 0;
        while (!modqx_modcmp(tmp, one, pp, ndash, numwords)) {
            modqx_modsqr(tmp, tmp, pp, ndash, numwords);
            i++;
            if (i >= M)
                return 0; // Number is not a square
        }

        // b = c^(2^(M-i-1))
        modqx_modcpy(cc, bb, numwords);
        for (int k = 0; k < M - i - 1; k++) {
            modqx_modsqr(bb, bb, pp, ndash, numwords);
        }
        modqx_modmul(RR, bb, RR, pp, ndash, numwords);  // R = R*b
        modqx_modsqr(bb, bsq, pp, ndash, numwords);     // b^2
        modqx_modmul(tt, bsq, tt, pp, ndash, numwords); // t = t*b^2
        modqx_modcpy(bsq, cc, numwords);                // c = b^2
        M = i;
    }

    modqx_redc(RR, RR, pp, ndash, numwords);
    ibz_from_modq(sqrt, RR, numwords);
    sqrt->bitlen = pbitlen;
    return 1;
}

// Variable-time exponentiation-based square root modulo p = 3 mod 4
static int
ibz_sqrt_mod_p_3mod4(ibz_t *sqrt, const ibz_t *a, const ibz_t *p)
{
    assert(a->bitlen > 0);
    assert(p->bitlen > 0);

    int pbitlen = p->bitlen;
    int numwords = (p->bitlen + MODQRADIX + 1) / MODQRADIX;
    int window = (ibz_bitsize(p) <= 160) ? 4 : 6;
    assert(window >= 1 && window <= 6);
    assert(numwords < MODQLIMBS);
    assert((ibz_get(p) & 3) == 3); // check p=3 mod 4
    assert(ibz_is_positive(p));
    assert(ibz_is_positive(a));
    assert(ibz_cmp(a, p) < 0);
    assert(p->bitlen + 64 < IBZ_NLIMBS * NUM_BITS_LIMB);

    // e = (p+1)/4
    ibz_t e = { 0 };
    ibz_add(&e, p, &ibz_const_one);
    ibz_div_2exp(&e, &e, 2);

    spint pp[MODQLIMBS], two_p[MODQLIMBS], aa[MODQLIMBS], ss[MODQLIMBS], ndash;
    ibz_to_modq(pp, p);
    modqx_modcpy(pp, two_p, numwords);
    modqx_modshl(1, two_p, numwords);
    ndash = modqx_getndash(pp[0]);
    ibz_to_modq(aa, a);
    modqx_nresx(aa, aa, two_p, numwords);

    modxpowe_non_ct(ss, aa, &e, pp, two_p, ndash, numwords, window); // a^((p+1)/4)
    modqx_modsqr(ss, two_p, pp, ndash, numwords);
    int result = 1 & modqx_modcmp(two_p, aa, pp, ndash, numwords);

    modqx_redc(ss, ss, pp, ndash, numwords);
    ibz_from_modq(sqrt, ss, numwords);
    sqrt->bitlen = pbitlen;
    return -result;
}

int
ibz_sqrt_mod_p(ibz_t *sqrt, const ibz_t *a, const ibz_t *p)
{
    assert(a->bitlen > 0);
    assert(p->bitlen > 0);

    if ((ibz_get(p) & 3) == 3)
        return ibz_sqrt_mod_p_3mod4(sqrt, a, p);
    else if ((ibz_get(p) & 3) == 1)
        return ibz_sqrt_mod_p_1mod4(sqrt, a, p);
    else
        return 0;
}

int
ibz_cmp(const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    int la = NUM_LIMBS(a->bitlen);
    int lb = NUM_LIMBS(b->bitlen);

    digit_t a_hi = -(digit_t)(a->limbs[la - 1] >> (NUM_BITS_LIMB - 1));
    digit_t b_hi = -(digit_t)(b->limbs[lb - 1] >> (NUM_BITS_LIMB - 1));

    if (!a_hi && b_hi)
        return +1;
    if (a_hi && !b_hi)
        return -1;
    assert(a_hi == b_hi);

    for (int i = int_max(la, lb) - 1; i >= 0; --i) {
        digit_t a_limb = i < la ? a->limbs[i] : a_hi;
        digit_t b_limb = i < lb ? b->limbs[i] : b_hi;
        if (a_limb > b_limb)
            return +1;
        if (a_limb < b_limb)
            return -1;
    }
    return 0;
}

int
ibz_cmp_int32(const ibz_t *x, int32_t y)
{
    assert(x->bitlen > 0);
    assert(y != INT32_MIN); // -2^31 is not representable at bitlen 32 (normalise folds it to 0)
    ibz_t yy;
    ibz_set(&yy, y, 32);
    return ibz_cmp(x, &yy);
}

int
ibz_is_zero(const ibz_t *x)
{
    assert(x->bitlen > 0);
    for (int i = 0; i < NUM_LIMBS(x->bitlen); ++i)
        if (x->limbs[i])
            return 0;
    return -1;
}

int
ibz_is_one(const ibz_t *x)
{
    assert(x->bitlen > 0);
    if (x->limbs[0] != 1)
        return 0;
    for (int i = 1; i < NUM_LIMBS(x->bitlen); ++i)
        if (x->limbs[i])
            return 0;
    return -1;
}

// zero is considered positive
int
ibz_is_positive(const ibz_t *x)
{
    assert(x->bitlen > 0);
    int n = NUM_LIMBS(x->bitlen);
    assert(n > 0 && n <= IBZ_NLIMBS);
    return (x->limbs[n - 1] >> (NUM_BITS_LIMB - 1)) - 1;
}

int
ibz_is_even(const ibz_t *x)
{
    assert(x->bitlen > 0);
    return -(int)((x->limbs[0] & 1) ^ 1);
}

int
ibz_is_odd(const ibz_t *x)
{
    assert(x->bitlen > 0);
    return -(int)(x->limbs[0] & 1);
}

int
ibz_rand_interval_with_domain(ibz_t *rand, const ibz_t *a, const ibz_t *b, prng_domain_ctx_t *domain)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    ibz_t t = { 0 }, bma = { 0 };
    assert(ibz_cmp(b, a) >= 0);
    ibz_sub(&bma, b, a);
    assert(ibz_is_positive(&bma));
    ibz_add(&bma, &bma, &ibz_const_one);
    int numbits = b->bitlen + 64; // Add 64 bits to reduce bias
    assert(numbits < IBZ_NLIMBS * NUM_BITS_LIMB);
    int numbytes = (numbits + 7) / 8;
    int numwords = numbits / NUM_BITS_LIMB + 1;
    assert(numwords >= 1 && numwords <= IBZ_NLIMBS);
    int padlen = numbits - (numwords - 1) * NUM_BITS_LIMB;
    assert(padlen >= 0 && padlen < NUM_BITS_LIMB);
    while (1) {
        if (prng_random_bytes(domain, (unsigned char *)(t.limbs), (size_t)numbytes) != 0)
            return 0;
#ifdef TARGET_BIG_ENDIAN
        // Bring each limb back to host order; the mask below then clears the top limb's stale, unsampled high bits.
        for (int i = 0; i < numwords; i++)
            t.limbs[i] = BSWAP_DIGIT(t.limbs[i]);
#endif
        t.limbs[numwords - 1] &= (((digit_t)1) << padlen) - 1;
        t.bitlen = numbits + 1;
        assert(ibz_is_positive(&t));
        ibz_add(&t, &t, &bma);
        if (ibz_bitsize(&t) <= numbits)
            break;
        assert(0);
    }
    ibz_mod(&t, &t, &bma);
    ibz_add(rand, a, &t);
    ibz_set_bound(rand, b->bitlen);
    assert(ibz_cmp(rand, a) >= 0);
    assert(ibz_cmp(rand, b) <= 0);
    return -1;
}

int
ibz_rand_interval(ibz_t *rand, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);
    return ibz_rand_interval_with_domain(rand, a, b, &PRNG_default_domain);
}

int
ibz_rand_interval_minm_m_with_domain(ibz_t *rand, int32_t m, prng_domain_ctx_t *domain)
{
    ibz_t mm;
    assert(m > 0);
    ibz_set(&mm, m, 32);
    ibz_add(&mm, &mm, &mm);
    int ret = ibz_rand_interval_with_domain(rand, &ibz_const_zero, &mm, domain);
    if (ret == 0)
        return 0;
    ibz_set(&mm, m, 32);
    ibz_sub(rand, rand, &mm);
    ibz_set_bound(rand, 32);
#ifndef NDEBUG
    ibz_abs(&mm, rand);
    assert(mm.limbs[0] <= (digit_t)m);
#endif
    return ret;
}

int
ibz_rand_interval_minm_m(ibz_t *rand, int32_t m)
{
    return ibz_rand_interval_minm_m_with_domain(rand, m, &PRNG_default_domain);
}

// Bit length of a single unsigned digit_t word, without branching on its value.
static int
digit_bitlen(digit_t x)
{
#ifdef __has_builtin
#if __has_builtin(__builtin_clzg)
    return NUM_BITS_LIMB - __builtin_clzg(x, 0);
#endif
#endif
    int n = 0;
    for (int shift = 8 * sizeof(digit_t) / 2; shift >= 1; shift >>= 1) {
        digit_t hi = x >> shift;
        digit_t mask = (digit_t)0 - (digit_t)ct_is_digit_nonzero(hi);
        x = ((x ^ hi) & mask) ^ x;
        n += (int)((digit_t)shift & mask);
    }
    return n + (int)(x & 1);
}

int
ibz_bitsize(const ibz_t *a)
{
    assert(a->bitlen > 0);

    int n = NUM_LIMBS(a->bitlen), i;
    digit_t sign = -(digit_t)(a->limbs[n - 1] >> (NUM_BITS_LIMB - 1));

    for (i = n - 1; i >= 0; --i)
        if (a->limbs[i] != sign)
            break;

    if (i < 0)
        return -sign;

    int l = i * NUM_BITS_LIMB;
    digit_t d = a->limbs[i] ^ sign;

    if (sign) {
        int carry = 1;
        for (int j = i - 1; j >= 0; --j)
            if (a->limbs[j]) {
                carry = 0;
                break;
            }
        d += carry;
    }

    if (!d)
        return l + NUM_BITS_LIMB + 1;
#ifdef __has_builtin
#if __has_builtin(__builtin_clzg)
    return l + NUM_BITS_LIMB - __builtin_clzg(d, -1);
#endif
#endif
    while (d) {
        d >>= 1;
        ++l;
    }
    return l;
}

int
ibz_size_in_base(const ibz_t *a, int base)
{
    assert(a->bitlen > 0);

    if (base < 2) {
        return 0;
    }

    int n = ibz_bitsize(a);
    if (n == 0) {
        return 0;
    }

    // base is a power of two: exact digit count.
    if ((base & (base - 1)) == 0) {
        int k = digit_bitlen((digit_t)base) - 1;
        return (n + k - 1) / k;
    }

    // Generic non-power-of-two base: log2(base) is strictly greater than digit_bitlen(base) - 1, so dividing by that
    // gives a safe upper bound.
    int b = digit_bitlen((digit_t)base) - 1;
    return (n + b - 1) / b + 1;
}

void
ibz_mul_by_int_and_set_bound(ibz_t *prod, const ibz_t *a, int32_t b, int result_bound)
{
    assert(a->bitlen > 0);

    digit_t xs = (digit_t)0 - (digit_t)(b < 0);
    digit_t xmag = ((digit_t)(sdigit_t)b ^ xs) - xs; // |b|

    int La = NUM_LIMBS(a->bitlen);
    digit_t as = (La > 0) ? ((digit_t)0 - (a->limbs[La - 1] >> (NUM_BITS_LIMB - 1))) : 0;
    ibz_cneg(prod, a, as); // prod = |a|

    int n = NUM_LIMBS(result_bound);
    assert(n <= IBZ_NLIMBS);
    ddigit_t carry = 0;
    for (int i = 0; i < n; i++) {
        digit_t ai = (i < La) ? prod->limbs[i] : 0;
        ddigit_t t = (ddigit_t)ai * (ddigit_t)xmag + carry;
        prod->limbs[i] = (digit_t)t;
        carry = t >> NUM_BITS_LIMB;
    }
    prod->bitlen = n * NUM_BITS_LIMB;
    ibz_cneg(prod, prod, as ^ xs);
    ibz_set_bound_ct(prod, result_bound);
}

void
ibz_add_and_set_bound(ibz_t *sum, const ibz_t *a, const ibz_t *b, int result_bound)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);
    ibz_add(sum, a, b);
    ibz_set_bound_ct(sum, result_bound);
}

void
ibz_add_int_and_set_bound(ibz_t *sum, const ibz_t *a, int32_t b, int result_bound)
{
    assert(a->bitlen > 0);
    assert(b != INT32_MIN);
    ibz_t bb = { 0 };
    ibz_set(&bb, (sdigit_t)b, 32);
    ibz_add(sum, a, &bb);
    ibz_set_bound(sum, result_bound);
}

int64_t
ibz_extract_i64(const ibz_t *a, int offset)
{
    assert(a->bitlen > 0);
    int64_t s;
    uint64_t u = ibz_extract_u64(a, offset);
    u &= (((uint64_t)1) << 63) - 1;
    s = 1 - 2 * (int64_t)(ibz_is_positive(a) + 1);
    return s * ((int64_t)u);
}

/**
 * Constant-time extract, independent of offset.
 */
uint64_t
ibz_extract_u64(const ibz_t *a, int offset)
{
    assert(a->bitlen > 0);
    ibz_t abs_a = { 0 };
    ibz_cneg(&abs_a, a, ~(digit_t)ibz_is_positive(a)); // |a|

    int n = NUM_LIMBS(abs_a.bitlen);
    const int nlimbs64 = (64 + NUM_BITS_LIMB - 1) / NUM_BITS_LIMB;
    int word_off = offset >> LOG2RADIX;
    int bit_off = offset & (NUM_BITS_LIMB - 1);
    digit_t w[((64 + NUM_BITS_LIMB - 1) / NUM_BITS_LIMB) + 1];
    for (int j = 0; j <= nlimbs64; j++) {
        digit_t acc = 0;
        for (int idx = 0; idx < n; idx++) {
            digit_t hit = ct_barrier_digit((digit_t)0 - (digit_t)ct_is_digit_zero((digit_t)(idx ^ (word_off + j))));
            acc |= abs_a.limbs[idx] & hit;
        }
        w[j] = acc;
    }

    uint64_t ret = 0;
    for (int j = 0; j < nlimbs64; j++) {
        digit_t merged = (w[j] >> bit_off) | ((w[j + 1] << 1) << (NUM_BITS_LIMB - 1 - bit_off));
        ret |= (uint64_t)merged << (j * NUM_BITS_LIMB);
    }
    return ret;
}

// Lehmer's algorithm inner loop, double-digit version (Tudor Jebelean, "A Double-Digit Lehmer-Euclid
// Algorithm for Finding the GCD of Long Integers", J. Symbolic Computation 19 (1995), section 3).
static int
mp_lehmer_inner(ddigit_t x, ddigit_t y, sdigit_t *A_out, sdigit_t *B_out, sdigit_t *C_out, sdigit_t *D_out)
{
    sddigit_t A = 1, B = 0, C = 0, D = 1;
    ddigit_t xx = x, yy = y;
    const sddigit_t BOUND = (sddigit_t)1 << (NUM_BITS_LIMB - 2);
    const ddigit_t Q_BOUND = ((ddigit_t)1) << NUM_BITS_LIMB;
    const int Q_SMALL = 32;
    int steps = 0;

    while (1) {
        if (yy == 0) {
            break;
        }

        ddigit_t q, rem;
        {
            ddigit_t r = xx;
            int cnt = 0;
            while (cnt < Q_SMALL && r >= yy) {
                r -= yy;
                cnt++;
            }
            if (r < yy) {
                q = (ddigit_t)cnt;
                rem = r;
            } else {
                ddigit_t extra_q = r / yy;
                q = (ddigit_t)Q_SMALL + extra_q;
                rem = r - extra_q * yy;
            }
        }

        if (q >= Q_BOUND) {
            break;
        }
        sddigit_t w = (sddigit_t)q;

        sddigit_t nC = A - w * C;
        sddigit_t nD = B - w * D;
        if (nC > BOUND || nC < -BOUND || nD > BOUND || nD < -BOUND) {
            break;
        }

        sddigit_t nA = C, nB = D;
        ddigit_t nx = yy, ny = rem;

        sddigit_t v_diff = nD - D;
        if (v_diff < 0) {
            v_diff = -v_diff;
        }
        sddigit_t abs_nD = (nD < 0) ? -nD : nD;
        if (!(ny >= (ddigit_t)abs_nD && nx - ny >= (ddigit_t)v_diff)) {
            break;
        }

        A = nA;
        B = nB;
        C = nC;
        D = nD;
        xx = nx;
        yy = ny;
        steps++;

        if (B == 0) {
            break;
        }
    }

    *A_out = (sdigit_t)A;
    *B_out = (sdigit_t)B;
    *C_out = (sdigit_t)C;
    *D_out = (sdigit_t)D;
    return steps;
}

// result = A*u + B*v: A, B are signed single-digit values and u, v are nonnegative len1/len2-word.
static void
mp_lehmer_combine(digit_t *result,
                  int reslen,
                  sdigit_t A,
                  const digit_t *u,
                  int len1,
                  sdigit_t B,
                  const digit_t *v,
                  int len2)
{
    assert(len1 <= IBZ_NLIMBS && len2 <= IBZ_NLIMBS && reslen <= IBZ_NLIMBS + 2);
    int neg1 = (A < 0), neg2 = (B < 0);
    assert(!(neg1 && neg2));
    digit_t abs1 = (digit_t)(neg1 ? -A : A);
    digit_t abs2 = (digit_t)(neg2 ? -B : B);

    digit_t term1[IBZ_NLIMBS + 2], term2[IBZ_NLIMBS + 2];
    memset(term1, 0, (size_t)reslen * sizeof(digit_t));
    memset(term2, 0, (size_t)reslen * sizeof(digit_t));
    digit_t tmp1[IBZ_NLIMBS + 1], tmp2[IBZ_NLIMBS + 1];
    mp_mul_raw(tmp1, u, len1, &abs1, 1);
    mp_mul_raw(tmp2, v, len2, &abs2, 1);
    int c1 = int_min(len1 + 1, reslen);
    int c2 = int_min(len2 + 1, reslen);
    memcpy(term1, tmp1, (size_t)c1 * sizeof(digit_t));
    memcpy(term2, tmp2, (size_t)c2 * sizeof(digit_t));

    if (neg1 == neg2) {
        digit_t carry = mp_add_raw(result, reslen, term1, reslen, term2, reslen);
        assert(carry == 0);
        (void)carry;
    } else {
        const digit_t *pos_term = neg1 ? term2 : term1;
        const digit_t *neg_term = neg1 ? term1 : term2;
        digit_t borrow = mp_sub_raw(result, reslen, pos_term, reslen, neg_term, reslen);
        assert(borrow == 0);
        (void)borrow;
    }
}

static inline int
mp_compare(const digit_t *a, const digit_t *b, unsigned int nwords)
{ // Multiprecision comparison, a=b? : (1) a>b, (0) a=b, (-1) a<b

    for (int i = nwords - 1; i >= 0; i--) {
        if (a[i] > b[i])
            return 1;
        else if (a[i] < b[i])
            return -1;
    }
    return 0;
}

// Variable-time (non-constant-time) GCD, uses double-digit version of Lehmer's algorithm.
void
ibz_gcd(ibz_t *gcd, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    if (ibz_is_zero(a) && ibz_is_zero(b)) {
        // gcd(0,0) = 0 by convention
        ibz_set(gcd, 0, 0);
        return;
    }
    int n = int_max(a->bitlen, b->bitlen);
    int L = NUM_LIMBS(n);

    digit_t a_neg = ~(digit_t)ibz_is_positive(a);
    digit_t b_neg = ~(digit_t)ibz_is_positive(b);
    ibz_t abs_a = { 0 }, abs_b = { 0 };
    ibz_cneg(&abs_a, a, a_neg);
    ibz_cneg(&abs_b, b, b_neg);

    digit_t u[IBZ_NLIMBS], v[IBZ_NLIMBS];
    if (mp_compare(abs_a.limbs, abs_b.limbs, (unsigned int)L) >= 0) {
        memcpy(u, abs_a.limbs, (size_t)L * sizeof(digit_t));
        memcpy(v, abs_b.limbs, (size_t)L * sizeof(digit_t));
    } else {
        memcpy(u, abs_b.limbs, (size_t)L * sizeof(digit_t));
        memcpy(v, abs_a.limbs, (size_t)L * sizeof(digit_t));
    }

    int ulen = L;
    while (ulen > 1 && u[ulen - 1] == 0) {
        ulen--;
    }
    int vlen = L;
    while (vlen > 1 && v[vlen - 1] == 0) {
        vlen--;
    }

    while (1) {
        if (v[vlen - 1] == 0) {
            break;
        }

        sdigit_t A, B, C, D;
        int steps;
        if (ulen - vlen >= 2) {
            steps = 0;
        } else {
            int has_lo = (ulen >= 2);
            ddigit_t x = ((ddigit_t)u[ulen - 1] << NUM_BITS_LIMB) | (has_lo ? (ddigit_t)u[ulen - 2] : 0);
            ddigit_t y = ((ddigit_t)v[ulen - 1] << NUM_BITS_LIMB) | (has_lo ? (ddigit_t)v[ulen - 2] : 0);
            steps = mp_lehmer_inner(x, y, &A, &B, &C, &D);
        }

        if (steps == 0) {
            digit_t qbuf[2 * IBZ_NLIMBS + 1];
            digit_t rbuf[IBZ_NLIMBS];
            mp_div_unsigned(qbuf, rbuf, u, ulen, v, vlen);

            memcpy(u, v, (size_t)vlen * sizeof(digit_t));
            memset(u + vlen, 0, (size_t)(L - vlen) * sizeof(digit_t));
            memcpy(v, rbuf, (size_t)vlen * sizeof(digit_t));
            memset(v + vlen, 0, (size_t)(L - vlen) * sizeof(digit_t));
        } else {
            int reslen = ((ulen > vlen) ? ulen : vlen) + 2;
            digit_t new_u[IBZ_NLIMBS + 2], new_v[IBZ_NLIMBS + 2];
            mp_lehmer_combine(new_u, reslen, A, u, ulen, B, v, vlen);
            mp_lehmer_combine(new_v, reslen, C, u, ulen, D, v, vlen);

            memset(u, 0, (size_t)L * sizeof(digit_t));
            memset(v, 0, (size_t)L * sizeof(digit_t));
            int ccount = (reslen < L) ? reslen : L;
            memcpy(u, new_u, (size_t)ccount * sizeof(digit_t));
            memcpy(v, new_v, (size_t)ccount * sizeof(digit_t));
        }

        ulen = L;
        while (ulen > 1 && u[ulen - 1] == 0) {
            ulen--;
        }
        vlen = L;
        while (vlen > 1 && v[vlen - 1] == 0) {
            vlen--;
        }

        assert(mp_compare(u, v, (unsigned int)L) >= 0);
    }

    gcd->bitlen = n;
    memcpy(gcd->limbs, u, (size_t)L * sizeof(digit_t));
}

// Multiplies a single-word signed coefficient by an ibz_t (non-CT).
static void
mp_mul_small_signed(ibz_t *prod, sdigit_t coef, const ibz_t *x)
{
    if (coef == 0 || ibz_is_zero(x)) {
        ibz_set(prod, 0, 1);
        return;
    }

    digit_t x_neg = ~(digit_t)ibz_is_positive(x);
    int result_neg = ((coef < 0) ^ x_neg) & 1;
    digit_t abs_coef = (coef < 0) ? -coef : coef;

    ibz_t abs_x = { 0 };
    ibz_cneg(&abs_x, x, x_neg);

    int xlen = NUM_LIMBS(abs_x.bitlen);
    digit_t result[IBZ_NLIMBS + 1];
    mp_mul_raw(result, abs_x.limbs, xlen, &abs_coef, 1);

    int result_bitlen = abs_x.bitlen + digit_bitlen(abs_coef) - 1;
    int max_bitlen = IBZ_NLIMBS * NUM_BITS_LIMB - 1;
    result_bitlen = int_min(result_bitlen, max_bitlen);
    ibz_copy_bits(prod, result, result_bitlen);
    if (result_neg) {
        ibz_neg(prod, prod);
    }
}

// Variable-time (non-constant-time) Extended GCD.
// Runs Lehmer's-algorithm loop, extended with two cofactor pairs.
void
ibz_xgcd(ibz_t *gcd, ibz_t *u, ibz_t *v, const ibz_t *a, const ibz_t *b)
{
    assert(a->bitlen > 0);
    assert(b->bitlen > 0);

    int n = int_max(a->bitlen, b->bitlen);
    int L = NUM_LIMBS(n);

    digit_t a_neg = ~(digit_t)ibz_is_positive(a);
    digit_t b_neg = ~(digit_t)ibz_is_positive(b);
    ibz_t abs_a = { 0 }, abs_b = { 0 };
    ibz_cneg(&abs_a, a, a_neg);
    ibz_cneg(&abs_b, b, b_neg);

    digit_t ru[IBZ_NLIMBS], rv[IBZ_NLIMBS];
    ibz_t cu_a = { 0 }, cu_b = { 0 }, cv_a = { 0 }, cv_b = { 0 };
    if (mp_compare(abs_a.limbs, abs_b.limbs, (unsigned int)L) >= 0) {
        memcpy(ru, abs_a.limbs, (size_t)L * sizeof(digit_t));
        memcpy(rv, abs_b.limbs, (size_t)L * sizeof(digit_t));
        ibz_set(&cu_a, 1, 2);
        ibz_set(&cu_b, 0, 2);
        ibz_set(&cv_a, 0, 2);
        ibz_set(&cv_b, 1, 2);
    } else {
        memcpy(ru, abs_b.limbs, (size_t)L * sizeof(digit_t));
        memcpy(rv, abs_a.limbs, (size_t)L * sizeof(digit_t));
        ibz_set(&cu_a, 0, 2);
        ibz_set(&cu_b, 1, 2);
        ibz_set(&cv_a, 1, 2);
        ibz_set(&cv_b, 0, 2);
    }

    int ulen = L;
    while (ulen > 1 && ru[ulen - 1] == 0) {
        ulen--;
    }
    int vlen = L;
    while (vlen > 1 && rv[vlen - 1] == 0) {
        vlen--;
    }

    while (1) {
        if (rv[vlen - 1] == 0) {
            break;
        }

        sdigit_t A, B, C, D;
        int steps;
        if (ulen - vlen >= 2) {
            steps = 0;
        } else {
            int has_lo = (ulen >= 2);
            ddigit_t x = ((ddigit_t)ru[ulen - 1] << NUM_BITS_LIMB) | (has_lo ? (ddigit_t)ru[ulen - 2] : 0);
            ddigit_t y = ((ddigit_t)rv[ulen - 1] << NUM_BITS_LIMB) | (has_lo ? (ddigit_t)rv[ulen - 2] : 0);
            steps = mp_lehmer_inner(x, y, &A, &B, &C, &D);
        }

        if (steps == 0) {
            int qbuf_len = mp_div_qlen(ulen, vlen);
            digit_t qbuf[2 * IBZ_NLIMBS + 1];
            digit_t rbuf[IBZ_NLIMBS];
            mp_div_unsigned(qbuf, rbuf, ru, ulen, rv, vlen);

            int qlen = qbuf_len;
            while (qlen > 1 && qbuf[qlen - 1] == 0) {
                qlen--;
            }

            ibz_t qz = { 0 };
            ibz_copy_digits(&qz, qbuf, qlen);

            ibz_t q_cv_a = { 0 }, new_cv_a = { 0 };
            ibz_mul(&q_cv_a, &qz, &cv_a);
            ibz_sub(&new_cv_a, &cu_a, &q_cv_a);

            ibz_t q_cv_b = { 0 }, new_cv_b = { 0 };
            ibz_mul(&q_cv_b, &qz, &cv_b);
            ibz_sub(&new_cv_b, &cu_b, &q_cv_b);

            ibz_copy(&cu_a, &cv_a);
            ibz_copy(&cu_b, &cv_b);
            ibz_copy(&cv_a, &new_cv_a);
            ibz_copy(&cv_b, &new_cv_b);

            memcpy(ru, rv, (size_t)vlen * sizeof(digit_t));
            memset(ru + vlen, 0, (size_t)(L - vlen) * sizeof(digit_t));
            memcpy(rv, rbuf, (size_t)vlen * sizeof(digit_t));
            memset(rv + vlen, 0, (size_t)(L - vlen) * sizeof(digit_t));
        } else {
            int reslen = ((ulen > vlen) ? ulen : vlen) + 2;
            digit_t new_ru[IBZ_NLIMBS + 2], new_rv[IBZ_NLIMBS + 2];
            mp_lehmer_combine(new_ru, reslen, A, ru, ulen, B, rv, vlen);
            mp_lehmer_combine(new_rv, reslen, C, ru, ulen, D, rv, vlen);

            ibz_t t1 = { 0 }, t2 = { 0 }, new_cu_a = { 0 }, new_cv_a = { 0 };
            mp_mul_small_signed(&t1, A, &cu_a);
            mp_mul_small_signed(&t2, B, &cv_a);
            ibz_add(&new_cu_a, &t1, &t2);
            mp_mul_small_signed(&t1, C, &cu_a);
            mp_mul_small_signed(&t2, D, &cv_a);
            ibz_add(&new_cv_a, &t1, &t2);

            ibz_t new_cu_b = { 0 }, new_cv_b = { 0 };
            mp_mul_small_signed(&t1, A, &cu_b);
            mp_mul_small_signed(&t2, B, &cv_b);
            ibz_add(&new_cu_b, &t1, &t2);
            mp_mul_small_signed(&t1, C, &cu_b);
            mp_mul_small_signed(&t2, D, &cv_b);
            ibz_add(&new_cv_b, &t1, &t2);

            ibz_copy(&cu_a, &new_cu_a);
            ibz_copy(&cv_a, &new_cv_a);
            ibz_copy(&cu_b, &new_cu_b);
            ibz_copy(&cv_b, &new_cv_b);

            memset(ru, 0, (size_t)L * sizeof(digit_t));
            memset(rv, 0, (size_t)L * sizeof(digit_t));
            int ccount = (reslen < L) ? reslen : L;
            memcpy(ru, new_ru, (size_t)ccount * sizeof(digit_t));
            memcpy(rv, new_rv, (size_t)ccount * sizeof(digit_t));
        }

        ulen = L;
        while (ulen > 1 && ru[ulen - 1] == 0) {
            ulen--;
        }
        vlen = L;
        while (vlen > 1 && rv[vlen - 1] == 0) {
            vlen--;
        }

        assert(mp_compare(ru, rv, (unsigned int)L) >= 0);
    }

    gcd->bitlen = n;
    memcpy(gcd->limbs, ru, (size_t)L * sizeof(digit_t));

    cu_a.bitlen = ibz_bitsize(&cu_a) + 1;
    cu_b.bitlen = ibz_bitsize(&cu_b) + 1;

    ibz_cneg(u, &cu_a, a_neg);
    ibz_cneg(v, &cu_b, b_neg);
}

// Variable-time (non-constant-time) CRT: returns 0 <= x < m1*m2 for x = a1 (mod m1) and x = a2 (mod m2).
// m1, m2 are positive and coprime and a1, a2 may be any signed representative of their residue class.
void
ibz_crt(ibz_t *x, const ibz_t *a1, const ibz_t *a2, const ibz_t *m1, const ibz_t *m2)
{
    assert(a1->bitlen > 0);
    assert(a2->bitlen > 0);
    assert(m1->bitlen > 0);
    assert(m2->bitlen > 0);

    ibz_t u = { 0 };
    int ok = ibz_invmod(&u, m1, m2);
    assert(ok);
    (void)ok;

    ibz_t diff_a = { 0 }, prod = { 0 }, r = { 0 };
    ibz_sub(&diff_a, a2, a1);
    assert(diff_a.bitlen + u.bitlen <= IBZ_NLIMBS * NUM_BITS_LIMB);
    ibz_mul(&prod, &diff_a, &u);
    ibz_mod(&r, &prod, m2);

    ibz_t x0 = { 0 };
    ibz_mul(&prod, m1, &r);
    ibz_add(&x0, a1, &prod);
    assert(m1->bitlen + m2->bitlen <= IBZ_NLIMBS * NUM_BITS_LIMB);
    ibz_mul(&prod, m1, m2);
    ibz_mod(x, &x0, &prod);
}

int
ibz_legendre(const ibz_t *a, const ibz_t *p)
{
    assert(a->bitlen > 0);
    assert(p->bitlen > 0);

    // Jacobi, actually

    if (ibz_is_zero(a))
        return 0;

    ibz_t uu, vv;
    ibz_t *u = &uu, *v = &vv;
    ibz_copy(u, a);
    ibz_copy(v, p);

    unsigned sym = 1;
    while (!ibz_is_one(u)) {
        assert(!ibz_is_zero(u) && !ibz_is_zero(v));

        unsigned u4 = u->limbs[0] % 4;

        if (u4 == 0) {
            ibz_div_2exp(u, u, 2);
            continue;
        }

        if (u4 == 2) {
            ibz_div_2exp(u, u, 1);
            if (v->limbs[0] % 8 == 3 || v->limbs[0] % 8 == 5)
                sym = !sym;
            continue;
        }

        if (ibz_cmp(u, v) >= 0) {
            ibz_sub(u, u, v);
            continue;
        }

        assert((u4 & 1) && (v->limbs[0] & 1));

        if (u4 == 3 && v->limbs[0] % 4 == 3)
            sym = !sym;

        // swap
        ibz_t *w = u;
        u = v;
        v = w;
    }

    return sym ? +1 : -1;
}

// Variable-time (non-constant-time) modular inverse. Runs Lehmer's-algorithm loop,
// extended with one extra pair of cofactors (cu, cv) tracking the coefficient of |a| alone.
int
ibz_invmod(ibz_t *inv, const ibz_t *a, const ibz_t *mod)
{
    assert(a->bitlen > 0);
    assert(mod->bitlen > 0);
    assert(ibz_is_positive(mod));
    assert(!ibz_is_zero(mod));
    assert(inv != mod);

    int n = int_max(a->bitlen, mod->bitlen);
    int L = NUM_LIMBS(n);

    digit_t a_neg = ~(digit_t)ibz_is_positive(a);
    ibz_t abs_a = { 0 }, abs_mod = { 0 };
    ibz_cneg(&abs_a, a, a_neg);
    ibz_copy(&abs_mod, mod);

    digit_t u[IBZ_NLIMBS], v[IBZ_NLIMBS];
    ibz_t cu = { 0 }, cv = { 0 };
    if (mp_compare(abs_a.limbs, abs_mod.limbs, (unsigned int)L) >= 0) {
        memcpy(u, abs_a.limbs, (size_t)L * sizeof(digit_t));
        memcpy(v, abs_mod.limbs, (size_t)L * sizeof(digit_t));
        ibz_set(&cu, 1, 2);
        ibz_set(&cv, 0, 2);
    } else {
        memcpy(u, abs_mod.limbs, (size_t)L * sizeof(digit_t));
        memcpy(v, abs_a.limbs, (size_t)L * sizeof(digit_t));
        ibz_set(&cu, 0, 2);
        ibz_set(&cv, 1, 2);
    }

    int ulen = L;
    while (ulen > 1 && u[ulen - 1] == 0) {
        ulen--;
    }
    int vlen = L;
    while (vlen > 1 && v[vlen - 1] == 0) {
        vlen--;
    }

    while (1) {
        if (v[vlen - 1] == 0) {
            break;
        }

        sdigit_t A, B, C, D;
        int steps;
        if (ulen - vlen >= 2) {
            steps = 0;
        } else {
            int has_lo = (ulen >= 2);
            ddigit_t x = ((ddigit_t)u[ulen - 1] << NUM_BITS_LIMB) | (has_lo ? (ddigit_t)u[ulen - 2] : 0);
            ddigit_t y = ((ddigit_t)v[ulen - 1] << NUM_BITS_LIMB) | (has_lo ? (ddigit_t)v[ulen - 2] : 0);
            steps = mp_lehmer_inner(x, y, &A, &B, &C, &D);
        }

        if (steps == 0) {
            int qbuf_len = mp_div_qlen(ulen, vlen);
            digit_t qbuf[2 * IBZ_NLIMBS + 1];
            digit_t rbuf[IBZ_NLIMBS];
            mp_div_unsigned(qbuf, rbuf, u, ulen, v, vlen);

            int qlen = qbuf_len;
            while (qlen > 1 && qbuf[qlen - 1] == 0) {
                qlen--;
            }

            ibz_t qz = { 0 }, q_cv = { 0 }, new_cv = { 0 };
            ibz_copy_digits(&qz, qbuf, qlen);
            ibz_mul(&q_cv, &qz, &cv);
            ibz_sub(&new_cv, &cu, &q_cv);
            ibz_copy(&cu, &cv);
            ibz_copy(&cv, &new_cv);

            memcpy(u, v, (size_t)vlen * sizeof(digit_t));
            memset(u + vlen, 0, (size_t)(L - vlen) * sizeof(digit_t));
            memcpy(v, rbuf, (size_t)vlen * sizeof(digit_t));
            memset(v + vlen, 0, (size_t)(L - vlen) * sizeof(digit_t));
        } else {
            int reslen = ((ulen > vlen) ? ulen : vlen) + 2;
            digit_t new_u[IBZ_NLIMBS + 2], new_v[IBZ_NLIMBS + 2];
            mp_lehmer_combine(new_u, reslen, A, u, ulen, B, v, vlen);
            mp_lehmer_combine(new_v, reslen, C, u, ulen, D, v, vlen);

            ibz_t t1 = { 0 }, t2 = { 0 }, new_cu = { 0 }, new_cv = { 0 };
            mp_mul_small_signed(&t1, A, &cu);
            mp_mul_small_signed(&t2, B, &cv);
            ibz_add(&new_cu, &t1, &t2);
            mp_mul_small_signed(&t1, C, &cu);
            mp_mul_small_signed(&t2, D, &cv);
            ibz_add(&new_cv, &t1, &t2);
            ibz_copy(&cu, &new_cu);
            ibz_copy(&cv, &new_cv);

            memset(u, 0, (size_t)L * sizeof(digit_t));
            memset(v, 0, (size_t)L * sizeof(digit_t));
            int ccount = (reslen < L) ? reslen : L;
            memcpy(u, new_u, (size_t)ccount * sizeof(digit_t));
            memcpy(v, new_v, (size_t)ccount * sizeof(digit_t));
        }

        ulen = L;
        while (ulen > 1 && u[ulen - 1] == 0) {
            ulen--;
        }
        vlen = L;
        while (vlen > 1 && v[vlen - 1] == 0) {
            vlen--;
        }

        assert(mp_compare(u, v, (unsigned int)L) >= 0);
    }

    ibz_t coeff = { 0 };
    ibz_cneg(&coeff, &cu, a_neg);
    ibz_mod(inv, &coeff, mod);

    return (ulen == 1 && u[0] == 1) ? -1 : 0;
}

// Computes an inverse mod 2^e via Newton-Hensel lifting
static int
ibz_invmod_2e(ibz_t *inv, const ibz_t *a, int e)
{
    assert(a->bitlen > 0);
    assert(e >= 0);
    assert(e + 1 <= IBZ_NLIMBS * NUM_BITS_LIMB);

    if (e == 0) {
        ibz_set(inv, 0, 1);
        return -1;
    }

    int exists = ibz_is_odd(a);

    ibz_t x = { 0 }, t = { 0 };
    ibz_copy(&x, &ibz_const_one);

    int prec = 1;
    while (prec < e) {
        int next = 2 * prec;
        if (next > e)
            next = e;
        ibz_mul(&t, a, &x);
        ibz_sub(&t, &ibz_const_two, &t);
        ibz_mul(&x, &x, &t);
        ibz_mod2exp(&x, &x, (uint32_t)next);
        prec = next;
    }
    ibz_mod2exp(inv, &x, (uint32_t)e);
    return exists;
}

// In-place inverse of the 2x2 matrix M=[[r1,r2],[s1,s2]] modulo 2^e, i.e. M_original * M_new = I (mod 2^e).
// M is invertible mod 2^e if and only if det=ad-bc is odd, and in this case M^-1 = (1/det)*[[s2,-s2],[-s1,r1]]
int
ibz_invmat(ibz_t *r1, ibz_t *r2, ibz_t *s1, ibz_t *s2, int e)
{
    assert(r1->bitlen > 0);
    assert(r2->bitlen > 0);
    assert(s1->bitlen > 0);
    assert(s2->bitlen > 0);
    assert(e >= 0);

    ibz_t a = { 0 }, b = { 0 }, c = { 0 }, d = { 0 };
    ibz_copy(&a, r1);
    ibz_copy(&b, r2);
    ibz_copy(&c, s1);
    ibz_copy(&d, s2);
    if (a.bitlen > e + 1)
        a.bitlen = e + 1;
    if (b.bitlen > e + 1)
        b.bitlen = e + 1;
    if (c.bitlen > e + 1)
        c.bitlen = e + 1;
    if (d.bitlen > e + 1)
        d.bitlen = e + 1;

    ibz_t det = { 0 }, tmp = { 0 };
    ibz_mul(&det, &a, &d);
    ibz_mul(&tmp, &b, &c);
    ibz_sub(&det, &det, &tmp);

    int exists = ibz_is_odd(&det);

    ibz_invmod_2e(&det, &det, e);

    ibz_t neg = { 0 };
    ibz_mul(&tmp, &d, &det);
    ibz_mod2exp(r1, &tmp, (uint32_t)e);
    ibz_neg(&neg, &b);
    ibz_mul(&tmp, &neg, &det);
    ibz_mod2exp(r2, &tmp, (uint32_t)e);
    ibz_neg(&neg, &c);
    ibz_mul(&tmp, &neg, &det);
    ibz_mod2exp(s1, &tmp, (uint32_t)e);
    ibz_mul(&tmp, &a, &det);
    ibz_mod2exp(s2, &tmp, (uint32_t)e);

    return exists;
}

// Top keep limbs of a[la] * b[lb], i.e. r[keep] = floor(a * b / 2^(R*(la + lb - keep))). r must not alias a or b.
// One-sided: r never exceeds the true value and falls short by at most (la + lb) ulps of r[0]. Only the columns
// from c0 upwards are summed; the two guard limbs below the requested window keep the skipped columns' total under
// one ulp of the least kept limb.
static void
mp_mul_high(digit_t *r, const digit_t *a, int la, const digit_t *b, int lb, unsigned int keep)
{
    int c0 = la + lb - keep - 2;
    if (c0 < 0)
        c0 = 0; // keep + 2 >= la + lb: degenerates to the exact full product, then truncated
    int next = la + lb - c0;
    digit_t ext[2 * ((IBZ_NLIMBS + 1) / 2 + 1) + 4];
    assert(next <= (int)(sizeof(ext) / sizeof(*ext)));
    memset(ext, 0, (size_t)next * sizeof(digit_t));
    for (int i = 0; i < la; i++) {
        int j0 = (c0 > i) ? c0 - i : 0;
        if (j0 >= lb)
            continue; // row i lies entirely below the window
        ddigit_t carry = 0;
        for (int j = j0; j < lb; j++) {
            ddigit_t sum = (ddigit_t)a[i] * b[j] + ext[i + j - c0] + carry;
            ext[i + j - c0] = (digit_t)sum;
            carry = sum >> NUM_BITS_LIMB;
        }
        ext[i + lb - c0] = (digit_t)carry; // highest index touched is next - 1
    }
    memcpy(r, ext + (next - keep), (size_t)keep * sizeof(digit_t));
}

// Reciprocal square root of a single normalized limb, in Q2.(R-2) for R = NUM_BITS_LIMB: given a1 >= 2^(R-2),
// representing alpha_1 = a1/2^R in [1/4, 1), returns y <= alpha^(-1/2) * 2^(R-2) for every alpha consistent with a1,
// short of it by at most ~24 ulps. Integer-only Newton iteration y <- y*(3 - alpha_1*y^2)/2, seeded with y = 1;
// eight steps suffice from the worst case alpha = 1/4, and the final -16 covers the two upward-rounding error terms
// (the floor inside q flipped in sign through h = 3 - q, and the truncation of alpha to alpha_1), keeping y one-sided.
static digit_t
digit_rsqrt_seed(digit_t a1)
{
    assert(a1 >= ((digit_t)1) << (NUM_BITS_LIMB - 2));

    digit_t y = ((digit_t)1) << (NUM_BITS_LIMB - 2); // 1.0
    for (int i = 0; i < 8; i++) {
        digit_t p = (digit_t)(((ddigit_t)y * y) >> NUM_BITS_LIMB);  // y^2,             frac R-4
        digit_t q = (digit_t)(((ddigit_t)p * a1) >> NUM_BITS_LIMB); // alpha_1 * y^2,   frac R-4, in (0, 1]
        digit_t h = (((digit_t)3) << (NUM_BITS_LIMB - 4)) - q;      // 3 - alpha_1*y^2, frac R-4, in [2, 3)
        y = (digit_t)(((ddigit_t)y * h) >> (NUM_BITS_LIMB - 3));    // y*h/2,           frac R-2
    }
    return y - 16;
}

// Bit length of an unsigned limb array, zero if the array is zero. Variable time: stops at the first nonzero limb.
static int
mp_bitlen_non_ct(const digit_t *x, int nwords)
{
    for (int i = nwords - 1; i >= 0; i--) {
        if (x[i] != 0)
            return i * NUM_BITS_LIMB + digit_bitlen(x[i]);
    }
    return 0;
}

// Variable-time (non-constant-time) floor square root.
// Division-free: Newton's reciprocal square root iteration y <- y*(3 - alpha*y^2)/2 at doubling widths on the
// normalized input, then multiply through (Shat = A*y) and correct exactly. The seed's -16 and each stage's -64
// keep every estimate one-sided, so the correction loop only increments (at most twice).
void
ibz_sqrt_floor(ibz_t *sqrt, const ibz_t *a)
{
    assert(a->bitlen > 0);
    assert(ibz_is_positive(a));

#ifndef NDEBUG
    ibz_t a_orig = { 0 };
    ibz_copy(&a_orig, a);
#endif

    int b_eff = mp_bitlen_non_ct(a->limbs, NUM_LIMBS(a->bitlen));

    if (b_eff == 0) {
        ibz_set(sqrt, 0, 1);
    } else {
        enum
        {
            MAX_M = (IBZ_NLIMBS + 1) / 2
        };
        int n_eff = NUM_LIMBS(b_eff);
        unsigned int m = (n_eff + 1) / 2; // the value part of A is 2m limbs wide, and so is the squaring at the end

        // Step 1: normalize. z == R*(2m - n_eff) + clz(top limb of a), in [0, 2R-1], and the limb part of the
        // even shift t is off == t/R. A carries two extra zero limbs below the value, so that each stage's
        // top-2L slice of it (zero-padded on the last stage, where 2L == 2m + 2) is a pointer instead of a copy.
        int z = 2 * NUM_BITS_LIMB * m - b_eff;
        int t = z & ~1;
        int th = z >> 1;
        int off = 2 * m - n_eff;
        int tb = t - NUM_BITS_LIMB * off;

        digit_t A[2 * MAX_M + 2];
        memset(A, 0, (size_t)(off + 2) * sizeof(digit_t));
        memcpy(A + 2 + off, a->limbs, (size_t)n_eff * sizeof(digit_t)); // last read of a: sqrt may alias it
        if (tb != 0) // mp_shiftl is only defined for shifts in [1, R-1]
            mp_shiftl(A + 2, (unsigned int)tb, (unsigned int)(2 * m));
        assert(A[2 * m + 1] >= ((digit_t)1) << (NUM_BITS_LIMB - 2)); // alpha >= 1/4

        // Step 2: seed at L = 1 (frac R-2), embedded into L = 2 by scaling with 2^R
        digit_t Y[MAX_M + 1];
        Y[0] = 0;
        Y[1] = digit_rsqrt_seed(A[2 * m + 1]);
        unsigned int L = 2;

        // Step 3: Newton stages. The first pass is a refresh at L = 2, which squares the seed's error; after that
        // the width grows as fast as the guard limb allows, Lout <= 2L - 1, and stops at m + 1.
        digit_t S[2 * (MAX_M + 1)], T[2 * (MAX_M + 1) + 2], h[2 * (MAX_M + 1) + 2], U[3 * (MAX_M + 1)];
        const digit_t margin = 64;
        unsigned int Lout = 2;
        do {
            assert(Lout <= 2 * L - 1 && Lout <= m + 1);
            mp_mul_raw(S, Y, L, Y, L);
            const digit_t *A_t = A + (2 * m + 2 - 2 * L); // top 2L limbs; alpha_t <= alpha, exact on the last stage
            mp_mul_high(T, A_t, 2 * L, S, 2 * L, 2 * L + 2);
            memset(h, 0, (size_t)(2 * L + 2) * sizeof(digit_t));
            h[2 * L + 1] = ((digit_t)3) << (NUM_BITS_LIMB - 4);
            assert(mp_compare(h, T, (unsigned int)(2 * L + 2)) >= 0); // alpha_t*y^2 <= 1 < 3, and T underestimates
            mp_sub_raw(h, 2 * L + 2, h, 2 * L + 2, T, 2 * L + 2);
            mp_mul_raw(U, Y, L, h + 2, 2 * L);
            digit_t *win = U + (3 * L - Lout - 1); // spans up to U[3L-1], the top limb of U
            mp_shiftr(win, (unsigned int)(NUM_BITS_LIMB - 3), (unsigned int)(Lout + 1));
            assert(win[Lout] == 0); // y*h/2 < 3 drains the window's top limb
            memcpy(Y, win, (size_t)Lout * sizeof(digit_t));
            mp_sub_raw(Y, Lout, Y, Lout, &margin, 1);
            L = Lout;
            Lout = (2 * L - 1 < m + 1) ? (2 * L - 1) : (m + 1);
        } while (L < m + 1);

        // Step 4: multiply through, then correct. A_f is the top m+1 limbs of A.
        const digit_t *A_f = A + (m + 1);
        digit_t P[2 * (MAX_M + 1)];
        mp_mul_raw(P, A_f, m + 1, Y, m + 1);
        digit_t *sh = P + (m + 1); // drop m+1 limbs, then R-2 bits, leaving Shat in sh[0 .. m-1]
        mp_shiftr(sh, (unsigned int)(NUM_BITS_LIMB - 2), (unsigned int)(m + 1));
        assert(sh[m] == 0);

        digit_t *Av = A + 2; // the 2m-limb value of A, holding the remainder A - Shat^2 from here on
        digit_t ssq[2 * MAX_M], d[2 * MAX_M];
        const digit_t one = 1, two = 2;
        mp_mul_raw(ssq, sh, m, sh, m);
        assert(mp_compare(Av, ssq, (unsigned int)(2 * m)) >= 0); // Shat is one-sided, so this cannot borrow
        mp_sub_raw(Av, 2 * m, Av, 2 * m, ssq, 2 * m);
        memcpy(d, sh, (size_t)m * sizeof(digit_t));
        memset(d + m, 0, (size_t)m * sizeof(digit_t));
        mp_shl1_bit(d, 2 * m, 1); // d = 2*Shat + 1
#ifndef NDEBUG
        int corrections = 0;
#endif
        while (mp_compare(Av, d, (unsigned int)(2 * m)) >= 0) {
            mp_sub_raw(Av, 2 * m, Av, 2 * m, d, 2 * m);
            mp_add_raw(sh, m, sh, m, &one, 1);
            mp_add_raw(d, 2 * m, d, 2 * m, &two, 1);
#ifndef NDEBUG
            corrections++;
#endif
        }
        assert(corrections <= 2);

        // Step 5: denormalize and write out; floor(sqrt) of a b_eff-bit value has exactly ceil(b_eff/2) bits
        if (th != 0) // mp_shiftr is only defined for shifts in [1, R-1]
            mp_shiftr(sh, (unsigned int)th, (unsigned int)m);
        ibz_copy_bits(sqrt, sh, (b_eff + 1) / 2);
    }

#ifndef NDEBUG
    // Postcondition in remainder form: 0 <= a - sqrt^2 <= 2*sqrt, which holds exactly when sqrt is the floor of the
    // root. (Comparing against (sqrt+1)^2 would need one bit more than an ibz_t can hold when a fills the container.)
    {
        ibz_t chk = { 0 }, r = { 0 };
        ibz_mul(&chk, sqrt, sqrt);
        ibz_sub(&r, &a_orig, &chk);
        assert(ibz_cmp(&r, &ibz_const_zero) >= 0);
        ibz_sub(&r, &r, sqrt);
        ibz_sub(&r, &r, sqrt);
        assert(ibz_cmp(&r, &ibz_const_zero) <= 0);
    }
#endif
}

int
ibz_probab_prime(const ibz_t *n, int reps)
{
    assert(n->bitlen > 0);
    assert(reps > 0);
    assert(n->bitlen <= MODQBITS - 2);
    assert(n->bitlen + 65 < IBZ_NLIMBS * NUM_BITS_LIMB);

    const ibz_t *arg = n;
    ibz_t tight = { 0 };

    /* Adjusting the ibz_t variable */
    if (ibz_is_positive(n)) {
        int actual_bits = ibz_bitsize(n);
        int tight_bound = actual_bits + 1;

        if (tight_bound < n->bitlen) {
            ibz_copy_bits(&tight, n->limbs, actual_bits);
#ifndef NDEBUG
            assert(ibz_cmp(&tight, n) == 0);
#endif
            arg = &tight;
        }
    }

    return probprime_bpsw_internal(arg, reps) ? -1 : 0;
}

/* Returns 1 only if a verified root of -1 is found.
   This is a Public function. It performs:

     Step 0. Searches for a small prime $a$ such that the Jacobi symbol $(a/n)$ is $-1.$
     Step 1. computes a sqrt with one modular exponentiation:
             res = a^((n-1)/4) mod n

     Step 2. Verifies: res^2 == -1 mod n
     Step 3. Returns 1 when res contains a verified root.
             In case of success, one can pass res and n to Cornacchia.
*/

int
ibz_sqrt_m1_mod_verified(ibz_t *res, const ibz_t *n)
{
    assert(n->bitlen > 0);
    assert(ibz_is_positive(n));
    assert((n->limbs[0] & 3) == 1);
    assert(n->bitlen <= MODQBITS - 2);

    int window = (ibz_bitsize(n) <= 160) ? 4 : 6;
    assert(window >= 1 && window <= 6);

    /*
     * Step 0. Small Jacobi-base selection:
     * find a small prime a such that (a/n) = -1.
     */
    uint32_t a = qlapoty_find_jacobi_minus_one(n);

    if (a == 0)
        return 0;

    int nbitlen = n->bitlen;
    int numwords = (nbitlen + MODQRADIX + 1) / MODQRADIX;
    assert(numwords < MODQLIMBS);

    spint nn[MODQLIMBS], two_n[MODQLIMBS], one[MODQLIMBS];
    spint x[MODQLIMBS] = { 0 }, y[MODQLIMBS], tmp[MODQLIMBS];

    ibz_to_modq(nn, n);

    modqx_modcpy(nn, two_n, numwords);
    modqx_modshl(1, two_n, numwords);

    spint ndash = modqx_getndash(nn[0]);
    modqx_modone(one, two_n, numwords);

    /*
     * Convert the small prime a to the MODQ/Montgomery representation.
     */
    modqx_modcpy(one, x, numwords);

    spint acc[MODQLIMBS] = { 0 };
    uint32_t aa = a;

    while (aa != 0) {
        if (aa & 1) {
            modqx_modadd(acc, x, tmp, two_n, numwords);
            modqx_modcpy(tmp, acc, numwords);
        }

        aa >>= 1;

        if (aa != 0) {
            modqx_modadd(x, x, tmp, two_n, numwords);
            modqx_modcpy(tmp, x, numwords);
        }
    }

    modqx_modcpy(acc, x, numwords);

    /*
     * Step 1. Perform a sqrt with one modular exponentiation:
     *             res = a^((n-1)/4) mod n
     */
    ibz_t e = { 0 };
    ibz_div_2exp(&e, n, 2);

    modxpowe_non_ct(x, x, &e, nn, two_n, ndash, numwords, window);

    /*
     * Step 3. Verification:
     *             res^2 == -1 mod n
     *             using
     *             res^2 + 1 == 0 mod n.
     */
    modqx_modsqr(x, y, nn, ndash, numwords);
    modqx_modadd(y, one, y, two_n, numwords);

    if (!modqx_modis0(y, nn, ndash, numwords))
        return 0;

    /*
     * Step 4. Store the root in res.
     * In case of success, one can pass res and n to Cornacchia.
     */
    modqx_redc(x, x, nn, ndash, numwords);
    ibz_from_modq(res, x, numwords);
    res->bitlen = nbitlen;

    return -1;
}

// ===============================================================================================
// Constant-time fixed-point and wide-arithmetic layer (ibz_ct_*)
//
// Declared in mp.h. Constant-time in the limb VALUES; bitlengths, shift bounds and loop
// counts are public. Built on the word-level ct_* helpers in mp_ct.h.

static void mp_ashr_bits(digit_t *out, const digit_t *x, int nwords, int k);

// ---- narrowing to out_bitlen
//
// Three sites below set bitlen to the raw limb-aligned NLIMBS(out_bitlen) * NUM_BITS_LIMB and only
// then call ibz_set_bound_ct(out_bitlen). The order is load-bearing: it forces the DECREASE branch,
// which is what masks and sign-extends the top limb. Collapsing bitlen first silently skips it.

// ---- constant-time shifts by a SECRET amount
//
// mp_shl_ct and mp_shift_ct shift a raw nwords-limb two's complement buffer by a SECRET amount.

// Number of word-ladder stages needed to cover any |shift| <= max_shift. PUBLIC.
static int
mp_shift_word_stages(int32_t max_shift)
{
    int32_t max_ws = max_shift >> LOG2RADIX;
    int nw = 0;
    while ((max_ws >> nw) != 0)
        nw++;
    return nw;
}

// t <<= shift, truncated to nwords limbs. shift >= 0 and SECRET; max_shift PUBLIC.
// In place, no scratch: a left shift reads only indices <= i, so descending i and reading both
// source limbs into locals before writing t[i] is safe at every offset, including 0.
static void
mp_shl_ct(digit_t *t, int nwords, int32_t shift, int32_t max_shift)
{
    int nw = mp_shift_word_stages(max_shift);
    int32_t ws = shift >> LOG2RADIX;
    int32_t bs = shift & (NUM_BITS_LIMB - 1);

    for (int b = 0; b < nw; b++) {
        int off = 1 << b;                                  // PUBLIC
        digit_t m = (digit_t)0 - (digit_t)((ws >> b) & 1); // apply this stage?
        for (int i = nwords - 1; i >= 0; i--) {
            int src = i - off;
            digit_t cand = (src >= 0) ? t[src] : 0;
            t[i] ^= (t[i] ^ cand) & m;
        }
    }

    for (int i = nwords - 1; i >= 0; i--) {
        digit_t lo = t[i];
        digit_t hi = (i - 1 >= 0) ? t[i - 1] : 0;
        t[i] = (lo << bs) | ((hi >> 1) >> (NUM_BITS_LIMB - 1 - bs));
    }
}

// t = shift >= 0 ? (t << shift) : arithmetic-right-shift(t, -shift), truncated to nwords limbs.
// shift SECRET (including its SIGN), max_shift PUBLIC. `scratch` must have room for nwords limbs;
// ping-pong is required because the right direction reads t[i + off], so no iteration order is
// in-place-safe.
static void
mp_shift_ct(digit_t *t, digit_t *scratch, int nwords, int32_t shift, int32_t max_shift)
{
    int nw = mp_shift_word_stages(max_shift);
    int32_t sgn = shift >> 31;            // all-ones iff shift < 0
    int32_t mag = (shift ^ sgn) - sgn;    // |shift|
    digit_t dir = (digit_t)(sdigit_t)sgn; // all-ones iff shifting right
    int32_t ws = mag >> LOG2RADIX;
    int32_t bs = mag & (NUM_BITS_LIMB - 1);

    digit_t *cur = t, *nxt = scratch;
    digit_t sign = (digit_t)0 - (t[nwords - 1] >> (NUM_BITS_LIMB - 1)); // sign-extension digit

    for (int b = 0; b < nw; b++) {
        int off = 1 << b; // PUBLIC
        digit_t m = (digit_t)0 - (digit_t)((ws >> b) & 1);
        for (int i = 0; i < nwords; i++) {
            int sl = i - off, sr = i + off;
            digit_t cl = (sl >= 0) ? cur[sl] : 0;        // left: zero-fill at the bottom
            digit_t cr = (sr < nwords) ? cur[sr] : sign; // right: sign-extend at the top
            digit_t cand = cl ^ ((cl ^ cr) & dir);
            nxt[i] = cur[i] ^ ((cur[i] ^ cand) & m);
        }
        digit_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    for (int i = 0; i < nwords; i++) {
        digit_t lo = cur[i];
        digit_t hl = (i - 1 >= 0) ? cur[i - 1] : 0;        // carry-in for a left shift
        digit_t hr = (i + 1 < nwords) ? cur[i + 1] : sign; // carry-in for a right shift
        digit_t vl = (lo << bs) | ((hl >> 1) >> (NUM_BITS_LIMB - 1 - bs));
        digit_t vr = (lo >> bs) | ((hr << 1) << (NUM_BITS_LIMB - 1 - bs));
        nxt[i] = vl ^ ((vl ^ vr) & dir);
    }
    cur = nxt;

    if (cur != t)
        memcpy(t, cur, (size_t)nwords * sizeof(digit_t));
}

// ---- ibz_ct_shift

void
ibz_ct_shift(ibz_t *res, const ibz_t *a, int32_t shift, int32_t max_shift, int out_bitlen)
{
    assert(max_shift >= 0);
    // Computed at width max(bitlen(a), out_bitlen) so the truncation to out_bitlen happens last.
    int work_bitlen = (a->bitlen > out_bitlen) ? a->bitlen : out_bitlen;
    int nwords = NLIMBS(work_bitlen);
    assert(nwords <= IBZ_NLIMBS);

    ibz_t t = { 0 };
    ibz_copy(&t, a);
    ibz_set_bound_ct(&t, work_bitlen);

    digit_t scratch[IBZ_NLIMBS];
    mp_shift_ct(t.limbs, scratch, nwords, shift, max_shift);

    t.bitlen = NLIMBS(out_bitlen) * NUM_BITS_LIMB;
    ibz_set_bound_ct(&t, out_bitlen);
    ibz_copy(res, &t);
}

// ---- highmul

// out = a * b (schoolbook, unsigned, na/nb limbs). Caller zeroes out and sizes it to >= na+nb limbs.
//
// Deliberately not ADDC: the ddigit_t here is a genuine NUM_BITS_LIMB x NUM_BITS_LIMB ->
// 2*NUM_BITS_LIMB multiply-accumulate, not a vehicle for a single carry bit, and it maps to a
// native widening multiply on both radices.
static void
mp_mul_wide(digit_t *out, const digit_t *a, int na, const digit_t *b, int nb)
{
    for (int i = 0; i < na; i++) {
        ddigit_t carry = 0;
        for (int j = 0; j < nb; j++) {
            ddigit_t prod_ij = (ddigit_t)a[i] * b[j];
            ddigit_t sum = (ddigit_t)out[i + j] + prod_ij + carry;
            out[i + j] = (digit_t)sum;
            carry = sum >> NUM_BITS_LIMB;
        }
        out[i + nb] = (digit_t)carry;
    }
}

// out = sign_prod ? -mag : mag, two's complement. The negation is carry-propagating.
static void
mp_signed_finish(digit_t *out, const digit_t *mag, int nwords, digit_t sign_prod)
{
    digit_t negbuf[2 * IBZ_NLIMBS + 1];
    unsigned int carry = 1; // two's complement: ~mag + 1
    for (int i = 0; i < nwords; i++)
        ADDC(negbuf[i], carry, (digit_t)~mag[i], (digit_t)0, carry);
    ct_select_words(out, mag, negbuf, sign_prod, nwords); // mag if >=0, negbuf if < 0
}

// out = sign(a)*sign(b) * |a|*|b| (sign forced to +1 when a==0 or b==0), into a local
// (na+nb+1)-limb buffer that bypasses IBZ_MAX_BITS limits. Returns nwords.
static int
mp_wide_signed_mul(digit_t out[2 * IBZ_NLIMBS + 1], const ibz_t *a, const ibz_t *b)
{
    digit_t sign_a = ~(digit_t)ibz_is_positive(a); // all-ones iff a < 0
    digit_t sign_b = ~(digit_t)ibz_is_positive(b);
    digit_t sign_prod = (sign_a ^ sign_b) & ibz_ct_nonzero_mask(a) & ibz_ct_nonzero_mask(b);

    // ibz_cneg rather than ibz_abs: the sign masks are already in hand, and ibz_abs would only
    // recompute them. The { 0 } initialisers are load-bearing: cneg writes exactly
    // NLIMBS(bitlen) limbs and mp_mul_wide only reads na/nb of them.
    ibz_t abs_a = { 0 }, abs_b = { 0 };
    ibz_cneg(&abs_a, a, sign_a);
    ibz_cneg(&abs_b, b, sign_b);

    int na = NLIMBS(a->bitlen);
    int nb = NLIMBS(b->bitlen);
    int nwords = na + nb + 1; // +1 guard limb
    assert(nwords <= 2 * IBZ_NLIMBS + 1);

    digit_t prodbuf[2 * IBZ_NLIMBS + 1] = { 0 };
    mp_mul_wide(prodbuf, abs_a.limbs, na, abs_b.limbs, nb);

    mp_signed_finish(out, prodbuf, nwords, sign_prod);
    return nwords;
}

// Narrows buf's low NLIMBS(out_bitlen) limbs into a fresh ibz_t bound to out_bitlen. See the
// narrowing note at the top of the file for why bitlen is left limb-aligned first.
static void
mp_wide_to_ibz_bound(ibz_t *res, const digit_t *buf, int out_bitlen)
{
    int nout = NLIMBS(out_bitlen);
    ibz_t out = { 0 };
    memcpy(out.limbs, buf, (size_t)nout * sizeof(digit_t));
    out.bitlen = nout * NUM_BITS_LIMB;
    ibz_set_bound_ct(&out, out_bitlen);
    ibz_copy(res, &out);
}

void
ibz_ct_highmul_p(ibz_t *res, const ibz_t *a, const ibz_t *b, int P, int out_bitlen)
{
    digit_t signed_buf[2 * IBZ_NLIMBS + 1];
    int nwords = mp_wide_signed_mul(signed_buf, a, b);

    digit_t shifted[2 * IBZ_NLIMBS + 1];
    mp_ashr_bits(shifted, signed_buf, nwords, P);

    // nwords bounds the source buffer (up to 2*IBZ_NLIMBS+1), IBZ_NLIMBS the destination's limbs[].
    assert(NLIMBS(out_bitlen) <= nwords);
    assert(NLIMBS(out_bitlen) <= IBZ_NLIMBS);
    mp_wide_to_ibz_bound(res, shifted, out_bitlen);
}

void
ibz_ct_highmul_s(ibz_t *res, const ibz_t *a, const ibz_t *b, int32_t shift, int32_t max_shift, int out_bitlen)
{
    assert(max_shift >= 0);

    digit_t t[2 * IBZ_NLIMBS + 1];
    int nwords = mp_wide_signed_mul(t, a, b);

    digit_t scratch[2 * IBZ_NLIMBS + 1];
    mp_shift_ct(t, scratch, nwords, shift, max_shift);

    assert(NLIMBS(out_bitlen) <= nwords);
    assert(NLIMBS(out_bitlen) <= IBZ_NLIMBS);
    mp_wide_to_ibz_bound(res, t, out_bitlen);
}

// ---- mask comparisons

// mask = (a < b) ? -1 : 0
digit_t
ibz_ct_lt_mask(const ibz_t *a, const ibz_t *b)
{
    int La = NLIMBS(a->bitlen), Lb = NLIMBS(b->bitlen);
    int n = (La > Lb) ? La : Lb;
    digit_t sa = (La > 0) ? ((digit_t)0 - (a->limbs[La - 1] >> (NUM_BITS_LIMB - 1))) : 0;
    digit_t sb = (Lb > 0) ? ((digit_t)0 - (b->limbs[Lb - 1] >> (NUM_BITS_LIMB - 1))) : 0;

    // a - b over n+1 sign-extended limbs; the top limb's sign bit is the answer.
    unsigned int borrow = 0;
    digit_t top = 0;
    for (int i = 0; i <= n; i++) {
        digit_t x = (i < La) ? a->limbs[i] : sa;
        digit_t y = (i < Lb) ? b->limbs[i] : sb;
        SUBC(top, borrow, x, y, borrow);
    }
    return (digit_t)0 - (top >> (NUM_BITS_LIMB - 1));
}

digit_t
ibz_ct_nonzero_mask(const ibz_t *a)
{
    digit_t acc = 0;
    for (int i = 0; i < NLIMBS(a->bitlen); i++)
        acc |= a->limbs[i];
    return ct_barrier_digit((digit_t)0 - (digit_t)ct_is_digit_nonzero(acc));
}

// ---- ibz_ct_fp_recip

// Q = floor((xhi:xlo)/y), 128-bit/64-bit restoring division, assuming xhi < y and y != 0.
// Not merged with mp_ct.h's 64/64 ct_div_unsigned_nonzero: that one needs neither the explicit
// high bit nor the per-iteration barrier.
static uint64_t
ct_div128_by64(uint64_t xhi, uint64_t xlo, uint64_t y)
{
    assert(y != 0);
    assert(xhi < y);
    uint64_t Rlo = xhi, Q = 0;
    for (int i = 63; i >= 0; i--) {
        uint64_t Rhi = Rlo >> 63;
        Rlo = (Rlo << 1) | ((xlo >> i) & 1);
        uint64_t ge = ct_barrier_u64(Rhi | (uint64_t)(1 - ct_is_u64_lessthan(Rlo, y)));
        uint64_t mask = (uint64_t)0 - ge;
        Rlo = Rlo - (y & mask);
        Q = (Q << 1) | ge;
    }
    return Q;
}

// Newton-Raphson for W ~= 2^(P+s)/D, s = bitsize(D): w_{k+1} = w_k*(2 - Dn*w_k) (scale-P fixed
// point), quadratic convergence from a 64-bit seed.
void
ibz_ct_fp_recip(ibz_t *W, int32_t *s_out, const ibz_t *D, int P, int nrounds)
{
    assert(ibz_is_positive(D) && !ibz_is_zero(D));
    assert(P > 63);

    int s = ibz_bitsize_ct(D); // secret
    *s_out = s;

    int32_t shift = P - s; // secret
    int32_t max_shift = (P > ibz_get_bound(D)) ? P : ibz_get_bound(D);

    // w < 2^(P+1) at Dn's minimum 2^(P-1).
    const int WBITS = P + 4;

    ibz_t Dn = { 0 };
    ibz_ct_shift(&Dn, D, shift, max_shift, WBITS);
    // Dn now has exactly bitsize P: 2^(P-1) <= Dn < 2^P.

    uint64_t top64 = ibz_extract_u64(&Dn, P - 64);
    uint64_t q64 = ct_div128_by64(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, top64);

    // Seed w = q64 << (P-63). The shift amount is public here, but ibz_mul_2exp routes through
    // the non-CT normalise() and q64 is secret, so the masked shift is the one to use.
    ibz_t w = { 0 };
    digit_t buf[IBZ_NLIMBS] = { 0 };
    for (int j = 0; j * NUM_BITS_LIMB < 64; j++)
        buf[j] = (digit_t)(q64 >> (j * NUM_BITS_LIMB));
    ibz_copy_digits(&w, buf, (64 + NUM_BITS_LIMB - 1) / NUM_BITS_LIMB);
    ibz_ct_shl(&w, &w, P - 63, P - 63, WBITS);

    ibz_t two_scaled = { 0 };
    ibz_set(&two_scaled, 1, 2);
    ibz_mul_2exp(&two_scaled, &two_scaled, (uint32_t)(P + 1));
    ibz_set_bound_ct(&two_scaled, WBITS + 2);

    for (int i = 0; i < nrounds; i++) {
        ibz_t t = { 0 }, u = { 0 };
        ibz_ct_highmul_p(&t, &Dn, &w, P, WBITS);
        ibz_set_bound_ct(&t, WBITS + 2);
        ibz_sub(&u, &two_scaled, &t);
        ibz_ct_highmul_p(&w, &w, &u, P, WBITS);
    }

    ibz_copy(W, &w);
    ibz_set_bound_ct(W, WBITS);
}

// q = round(x / 2^P), ties toward +infinity, for P a nonnegative multiple of NUM_BITS_LIMB.
void
ibz_ct_round_shift_limb(ibz_t *q, const ibz_t *x, int P, int out_bitlen)
{
    assert(P >= 0 && P % NUM_BITS_LIMB == 0);
    assert(out_bitlen > 0 && out_bitlen % NUM_BITS_LIMB == 0);

    int p_limb = P / NUM_BITS_LIMB;
    int nq = out_bitlen / NUM_BITS_LIMB;
    assert(p_limb + nq <= IBZ_NLIMBS);

    ibz_t xw = { 0 };
    ibz_copy(&xw, x);
    ibz_set_bound_ct(&xw, (p_limb + nq) * NUM_BITS_LIMB);

    digit_t round_bit = 0;
    if (p_limb > 0)
        round_bit = (xw.limbs[p_limb - 1] >> (NUM_BITS_LIMB - 1)) & (digit_t)1;

    ibz_t res = { 0 };
    unsigned int carry = (unsigned int)round_bit;
    for (int i = 0; i < nq; i++)
        ADDC(res.limbs[i], carry, xw.limbs[p_limb + i], (digit_t)0, carry);
    res.bitlen = out_bitlen;
    ibz_copy(q, &res);
}

// ---- primitives mp does not provide

// out = arithmetic (SIGN-EXTENDING) right shift of the nwords-limb two's complement value x by
// k bits, in a single funnel-shift pass. k is PUBLIC. out and x must NOT alias.
// Same value as ibz_div_2exp, but on a raw buffer and in a single pass.
static void
mp_ashr_bits(digit_t *out, const digit_t *x, int nwords, int k)
{
    digit_t sign = (nwords > 0) ? ((digit_t)0 - (x[nwords - 1] >> (NUM_BITS_LIMB - 1))) : 0;
    int word_shift = k / NUM_BITS_LIMB;
    int bit_shift = k % NUM_BITS_LIMB;
    for (int i = 0; i < nwords; i++) {
        int64_t src = (int64_t)i + word_shift;
        digit_t lo = (src < nwords) ? x[src] : sign;
        digit_t hi = (src + 1 < nwords) ? x[src + 1] : sign;
        out[i] = (bit_shift == 0) ? lo : ((lo >> bit_shift) | (hi << (NUM_BITS_LIMB - bit_shift)));
    }
}

void
ibz_ct_shl(ibz_t *res, const ibz_t *a, int32_t shift, int32_t max_shift, int out_bitlen)
{
    assert(max_shift >= 0);
    int work_bitlen = (a->bitlen > out_bitlen) ? a->bitlen : out_bitlen;
    int nwords = NLIMBS(work_bitlen);
    assert(nwords <= IBZ_NLIMBS);
    ibz_copy(res, a);
    ibz_set_bound_ct(res, work_bitlen);

    mp_shl_ct(res->limbs, nwords, shift, max_shift);

    res->bitlen = NLIMBS(out_bitlen) * NUM_BITS_LIMB;
    ibz_set_bound_ct(res, out_bitlen);
}
