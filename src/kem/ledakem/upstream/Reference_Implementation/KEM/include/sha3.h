/**
 *
 * <sha3.h>
 *
 * @version 1.0 (September 2017)
 *
 * Reference ISO-C99 Implementation of LEDAkem cipher" using GCC built-ins.
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#pragma once

#define LITTLE_ENDIAN

void Keccak(      unsigned int rate,
                  unsigned int capacity,
                  const unsigned char *input,
                  unsigned long long int inputByteLen,
                  unsigned char delimitedSuffix,
                  unsigned char *output,
                  unsigned long long int outputByteLen);

/**
  *  Function to compute SHA3-256 on the input message.
  *  The output length is fixed to 32 bytes.
  */
static inline
void sha3_256(const unsigned char *input,
              unsigned int inputByteLen,
              unsigned char *output)
{
   Keccak(1088, 512, input, inputByteLen, 0x06, output, 32);
}

/**
  *  Function to compute SHA3-384 on the input message.
  *  The output length is fixed to 48 bytes.
  */
static inline
void sha3_384(const unsigned char *input,
              unsigned int inputByteLen,
              unsigned char *output)
{
   Keccak(832, 768, input, inputByteLen, 0x06, output, 48);
}

/**
  *  Function to compute SHA3-512 on the input message.
  *  The output length is fixed to 64 bytes.
  */
static inline
void sha3_512(const unsigned char *input,
              unsigned int inputByteLen,
              unsigned char *output)
{
   Keccak(576, 1024, input, inputByteLen, 0x06, output, 64);
}
