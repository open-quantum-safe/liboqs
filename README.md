liboqs
======

liboqs is a C library for quantum-resistant cryptographic algorithms.

**This is a development branch that is currently not for use.**

Adding a new NIST submission
----------------------------

Suppose the algorithm we want to add is called `potato`.

- Download the ZIP file of the submission from the NIST website (or elsewhere)
- Unzip the file
- Create new directories `src/kem/potato` and `src/kem/potato/upstream`
- Copy the contents of the ZIP file (except the known answer tests (KAT) and the supporting documentation folders) into `src/kem/potato/upstream`
- Do a `git add` and `git commit` on the newly added files (so that we get a fresh snapshot of the files before any objects are built)
- Copy (and rename appropriately) `src/kem/dummy1/kem_dummy1.c`, `src/kem/dummy1/kem_dummy1.h`, and `src/kem/dummy1/Makefile` to `src/kem/potato`
- Edit `src/kem/potato/kem_potato.h` to create a copy of the macros and function prototypes for each algorithm to expose; copy the correct lengths into the length macros from the upstream algorithm's api.h file
- Edit `src/kem/potato/kem_potato.c` to create a copy of the constructor for each algorithm to expose; set the correct method_name, claimed_nist_level, and ind_cca values.
- Edit `src/kem/potato/Makefile`
	- At the top of the Makefile, set up which algorithms are available in each architecture and whatever architecture-specific flags need to be passed down to the underlying Makefile
	- Edit the lower part of the Makefile to build a .a file for each algorithm to expose
	- Use the `scripts/symbol_alias.sh` script to expose the `OQS_KEM_potato_keypair`, `OQS_KEM_potato_encaps`, and `OQS_KEM_potato_decaps` functions
	- Use the `scripts/symbol_unexport.sh` script to unexpose all other global symbols in the .a file.  You can find these by running `nm -g potato.a | grep ' T '` and `nm -g potato.a | grep ' D '`
- Edit `src/kem/Makefile` at the `EDIT-WHEN-ADDING-KEM` marker
- Edit `src/kem/kem.h` at the two `EDIT-WHEN-ADDING-KEM` markers
- Edit `src/kem/kem.c` at the `EDIT-WHEN-ADDING-KEM` marker
- Edit `Makefile` at the `EDIT-WHEN-ADDING-KEM` marker
