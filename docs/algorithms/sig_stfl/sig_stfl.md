
Stateful Hash Based Signature

The security of hash based signatures (HBS) is based on the underlaying hash functions on which they are built.
NIST recommendation is that they are suitable for near term use to mitigate against attacks mounted by quantum computers.
While not a general purpose solution, they are useful means to authenticate boot or firmware images.

This package provides full support for a variety of variants for XMSS and LMS. 
Key generation, signature generation, and signature verification. 
Security of HBS also depends on the management of the state of the secret key. Secret keys can only used once to generate a signature.
Multiple signing with same key can reveal that key to an attacker.
Because of this, NIST recommends that key and signature aeration be done in hardware security modules.
Having said that, this library is fully functional for research purposes. Storage and management of the secret keys are left update applications using this feature.
Secret key storage is easily done when by supplying a callback function to the library.


Key State Management:
Application writers have to supply callback functions to store and update secret keys.
After a sign operation the secret key index is advanced and stored. This ensures one-time use of the key.
Signing operations will fail without this callback set because the private key cannot be advance (to prevent reuse).

Stateful keys can generate a finite number signatures. A counter tracks the limit when the key is created and is decremented after each signature is generated.
When the counter is down to 0, signature generations fails. Applications can query the remaining count via an API.

Multithreaded application will need to set lock and unlock callbacks for the library to use. This is needed to ensure that only one thread can access a given secret key
for signing and update operations.

