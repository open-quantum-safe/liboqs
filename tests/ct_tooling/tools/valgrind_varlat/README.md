# Valgrind-Varlat
This directory contains the necessary files to execute Valgrind's memcheck tool on liboqs with [Daniel Bernstein's Kyberslash patches](https://kyberslash.cr.yp.to/papers.html) (valgrind-try-patch-20250805.txt and valgrind-varlat-patch-20250805.txt) and another patch including variable latency warnings in the suppression block (valgrind_varlat_sup_block.txt).

Valgrind-Varlat handles false-positive warnings by storing their suppression block into `.txt` files within the `false_positives/` directory. These files are passed during the tools execution, successfully disregarding those warnings that are cathegorized as not constant-time issues after review.

## Valgrind-Varlat Install Requirements
In order to successfully execute Valgrind-Varlat's test using the tooling developed in this subrepository follow the next steps:

- Install valgrind using the official git repository. Go to [Daniel Bernstein's suggested](https://sourceforge.net/p/valgrind/mailman/message/59216875/) [commit](https://sourceware.org/git/?p=valgrind.git;a=commit;h=112f1080b7c21e37dfce0a2e589d0dc7aa115afa) to cleanly apply Kyberslash patches to Valgrind without encountering dependency issues: 112f1080b7c21e37dfce0a2e589d0dc7aa115afa.

```
VALGRIND_REPO="https://sourceware.org/git/valgrind.git"
TRY_PATCH=<your-path-to-valgrind-try-patch-20250805.txt>
VARLAT_PATCH=<your-path-to-valgrind-varlat-patch-20250805.txt>
SUP_BLOCK_PATCH=<your-path-to-valgrind-varlat-sup-block.txt">
INSTALL_DIR="$HOME/valgrind_varlat"

# Clone the Valgrind repository
git clone "$VALGRIND_REPO" valgrind_varlat
git checkout 112f1080b7c21e37dfce0a2e589d0dc7aa115afa
cd valgrind_varlat
```

- Apply Bernstein's patches.

```
git apply $TRY_PATCH
git apply $VARLAT_PATCH
```

- Apply the suppression block patch.

```
git apply $SUP_BLOCK_PATCH
```

- Include the resultant version of valgrind into PATH under .

```
# Build and install valgrind_varlat
./autogen.sh
./configure --prefix="$INSTALL_DIR"
make -j$(nproc)
sudo make install

# Rename the executable
sudo mv "$INSTALL_DIR/bin/valgrind" "$INSTALL_DIR/bin/valgrind_varlat"

# Add valgrind_varlat to PATH
echo "export PATH=\"$INSTALL_DIR/bin:\$PATH\"" >> ~/.bashrc
source ~/.bashrc
```

To check whether the installation was successful, you can use the varlat tests provided in the Kyberslash patch. Compile valgrind/memcheck/tests/varlat.c  with `gcc -o varlat varlat.c` and execute `valgrind_varlat --tool=memcheck --variable-latency-errors=yes --gen-suppressions=all ./varlat`. If the output ressembles something like the following output, valgrind_varlat was installed successfully:

```
==5335== Memcheck, a memory error detector
==5335== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==5335== Using Valgrind-3.26.0.GIT and LibVEX; rerun with -h for copyright info
==5335== Command: ./varlat
==5335== 
==5335== Variable-latency instruction operand of size 4 is secret/uninitialised
==5335==    at 0x4001176: storage_init (in /home/.../valgrind_varlat/memcheck/tests/varlat)
==5335==    by 0x40011BB: main (in /home/.../valgrind_varlat/memcheck/tests/varlat)
==5335== 
{
   <insert_a_suppression_name_here>
   Memcheck:Value4
   variable-latency: yes
   fun:storage_init
   fun:main
}
==5335== 
==5335== HEAP SUMMARY:
==5335==     in use at exit: 0 bytes in 0 blocks
==5335==   total heap usage: 1 allocs, 1 frees, 1 bytes allocated
==5335== 
==5335== All heap blocks were freed -- no leaks are possible
==5335== 
==5335== Use --track-origins=yes to see where uninitialised values come from
==5335== For lists of detected and suppressed errors, rerun with: -s
==5335== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

```

## Algorithm Testing
Valgrind-Varlat produces numerous warnings, making complete storage impractical in terms of memory and runtime. The test script therefore processes output as follows:
- It captures each unique suppression block (the {...} content with full warning details) and logs it to a file.
- Only these unique blocks are counted as warnings, avoiding duplication from repeated occurrences during execution.

The testing framework currently skips all SPHINCS and SLH-DSA tests due to the execessive length of time they require to execute.

## False positive handling
Here is an example of a suppression file of a known false-positive for the Kyber algorithm:

```text
    {
    Rejection sampling to produce public "A" matrix
    Memcheck:Cond
    fun:rej_uniform
    fun:PQCLEAN_KYBER*_CLEAN_gen_matrix
    }
```

The brackets wrap a single error that is to be suppressed. Within the brackets, the first line is a comment. The remaining lines tell Valgrind to ignore any "Memcheck:Cond" errors that occur when a function named rej_uniform is called from a function whose name matches the glob pattern PQCLEAN_KYBER*_CLEAN_gen_matrix.

Before this suppression file was written, a run of this script produced the following output.
```text
    ==594== Conditional jump or move depends on uninitialised value(s)
    ==594==    at 0x22550D: rej_uniform (indcpa.c:133)
    ==594==    by 0x225654: PQCLEAN_KYBER512_CLEAN_gen_matrix (indcpa.c:177)
    ==594==    by 0x2257D1: PQCLEAN_KYBER512_CLEAN_indcpa_keypair (indcpa.c:216)
    ==594==    by 0x1B6C1E: PQCLEAN_KYBER512_CLEAN_crypto_kem_keypair (kem.c:26)
    ==594==    by 0x1B6B9F: OQS_KEM_kyber_512_keypair (kem_kyber_512.c:56)
    ==594==    by 0x10D123: OQS_KEM_keypair (kem.c:818)
    ==594==    by 0x10AD07: kem_test_correctness (test_kem.c:103)
    ==594==    by 0x10B4E7: test_wrapper (test_kem.c:186)
    ==594==    by 0x4CDAFA2: start_thread (pthread_create.c:486)
    ==594==    by 0x4DED4CE: clone (clone.S:95)
    ==594==
    {
       <insert_a_suppression_name_here>
       Memcheck:Cond
       fun:rej_uniform
       fun:PQCLEAN_KYBER512_CLEAN_gen_matrix
       fun:PQCLEAN_KYBER512_CLEAN_indcpa_keypair
       fun:PQCLEAN_KYBER512_CLEAN_crypto_kem_keypair
       fun:OQS_KEM_kyber_512_keypair
       fun:OQS_KEM_keypair
       fun:kem_test_correctness
       fun:test_wrapper
       fun:start_thread
       fun:clone
    }
```
The lines beginning with "==" are a Valgrind error message. The bracketed text is a suppression file template. To produce the final suppression file we added a comment, replaced "512" with a  wildcard (since an identical error occurs in other Kyber parameter sets), and truncated the backtrace (since the extra lines provide no interesting information to auditors).

The "fun:rej_uniform" line says to ignore _all_ Memcheck:Cond errors in rej_uniform, but Valgrind told us that line 133 was the problem. Any "fun:name" line in the backtrace can be replaced by an equivalent "src:file:line", so we could have narrowed the scope of our suppression:
```text
    {
       Rejection sampling to produce public "A" matrix
       Memcheck:Cond
       src:indcpa.c:133 # fun:rej_uniform
       fun:PQCLEAN_KYBER*_CLEAN_gen_matrix
    }
```
Here "# fun:rej_uniform" is a comment. An update to the Kyber source code might break our suppression file by changing the line number, and leaving the function name as a comment might help a future reviewer.

An ellipsis (...) can serve as a wildcard for a portion of the backtrace. We could have written:
```text
    {
       Rejection sampling to produce public "A" matrix
       Memcheck:Cond
       ...
       fun:PQCLEAN_KYBER*_CLEAN_gen_matrix
    }
```
But this is perhaps too concise. Remember that the goal here is to help auditors.

Further information can be found in Valgrind's manual. See
    https://www.valgrind.org/docs/manual/manual-core.html#manual-core.suppress
and
    https://www.valgrind.org/docs/manual/mc-manual.html#mc-manual.suppfiles

## Dependencies
Remember to install the prerequisites for building liboqs:

`sudo apt install -y build-essential cmake ninja-build gcc g++ clang python3 python3-pip valgrind libssl-dev realpath python3 python3-pip`
`pip3 install pytest`
