Contributing to liboqs nist-branch
==================================

This branch takes a "light touch" approach to incorporating new implementations:

- Source code from a NIST submission will be included ideally with no changes, in an "upstream" subdirectory.
- A thin wrapper will be written to provide the implementation using the liboqs API.
- The implementation will be added to the build process.
- To avoid namespace collisions between different algorithms, symbol renaming will be used on the compiled files.

This file describes the step-by-step procedure to add a new KEM algorithm to liboqs nist-branch.  Separate instructions apply for adding an algorithm to master branch.  

If you get stuck or are unsure of what to do, feel free to contact us via a Github issue/pull request/@mention, or email one of the team.

Basic steps
-----------

Suppose the module we want to add is a KEM called `potato`.  Some NIST submissions contain multiple algorithms at different security levels or with different parameters, for example `potato_512`, `potato_1024`, `potato_2048`.  In liboqs, we create a single source code directory `src/kem/potato`, and a single wrapper file `kem_potato.c`, but that wrapper file contains wrappers for each of the different parameterizations, named for example `OQS_KEM_potato_512`, ....  

### Getting started

1. Ensure that the implementation meets the acceptance criteria stated in [README.md](https://github.com/open-quantum-safe/liboqs/blob/ds-nist-branch/README.md), including that:
	- the implementation is licensed under an acceptable open source license; and
	- that there are no known breaks of the algorithm (see the "View comments" link for the submission on the [NIST Round 1 submission page](https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Round-1-Submissions)).
2. Make a new working branch off of nist-branch, preferably with the word `nist` somewhere in the branch name.
3. Create new directories `src/kem/potato` and `src/kem/potato/upstream`

### Adding the upstream implementation

1. Download the ZIP file of the submission from the NIST website (or elsewhere).
2. Copy the contents of the ZIP file (except the known answer tests (KAT) and the supporting documentation folders) into `src/kem/potato/upstream`.
3. If `src/kem/potato/upstream` does not already contain a `LICENSE.txt` file, confirm the license of the implementation and add a corresponding LICENSE.txt file.
4. Do a `git add` and `git commit` on the newly added files (so that we get a fresh snapshot of the files before any objects are built).

### Creating the OQS wrapper

1. From another KEM implementation's directory, copy (and rename appropriately) `src/kem/whatever/kem_whatever.c` and `src/kem/whatever/kem_whatever.h` to `src/kem/potato`.
2. Edit `src/kem/potato/kem_potato.h` to create a copy of the macros and function prototypes for each algorithm to expose; copy the correct lengths into the length macros from the upstream algorithm's `api.h` file.
3. Edit `src/kem/potato/kem_potato.c` to create a copy of the constructor for each algorithm to expose; set the correct `method_name`, `claimed_nist_level` (1-5), and `ind_cca` values.
4. Edit `src/kem/kem.h` at the `EDIT-WHEN-ADDING-KEM` marker to add the analogous lines for potato.
5. Edit `src/kem/kem.c` at the two `EDIT-WHEN-ADDING-KEM` markers to add the analogous lines for potato.

`.c` and `.h` files in liboqs (other than in `upstream` directories) must meet the OQS coding convention and style and are checked by the pretty-printer.  See https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for details.  You can use `make prettyprint` to run the pretty-printer; you will need to install `clang-format` version 3.9, as per the instructions at the link above.

### Adding to the build system

1. From another KEM implementation's directory, copy `src/kem/whatever/Makefile` to `src/kem/potato`.
2. Edit `src/kem/potato/Makefile`.
3. At the top of the Makefile, set up which algorithms are available in each architecture and whatever architecture-specific flags need to be passed down to the underlying Makefile.
4. Edit the lower part of the Makefile to create a compilation target for each algorithm/parameterization.  You can do this either by calling the upstream implementations Makefile to get it to generate `.o` or `.a` files, or by directly specifying compilation commands.  You should remove the upstream implementation algorithm's `randombytes` function from being built, as we will instead use the `randombytes` function in liboqs; in many NIST implementations with an `rng.c` file, you should just need to remove `rng.c` from being built.
5. In the compilation target for each algorithm/parameterization (`potato_512`, ...), you will need to call various scripts to collect together object files, rename global symbols, and hide local symbols.  Below we suppose we are working with the `potato_512` parameterization.
	1. If the upstream implementation has generated `.o` files, use `scripts/collect_objects.sh` to collect those into liboqs's `.objs` directory.  (See `src/kem/newhopenist/Makefile` for an example.)
	2. If the upstream implementation has generated a `.a` file, use `scripts/explode_and_collect.sh` to generate the `.o` files for liboqs's `.objs` director. (See `src/kem/frodokem/Makefile` for an example.)
	3. Create a file `symbols_global_rename_512.txt` indicating how to rename the existing function names for the globally exposed symbols (namely, the mapping to obtain `OQS_KEM_potato_512_keygen`, `OQS_KEM_potato_512_encaps`, and `OQS_KEM_potato_512_decaps`); the format of this file should be two symbols per line, the old name and the new name, separated by a space.  Add a line in the compilation target to run `scripts/symbols_global_rename.sh` with `symbols_global_rename_512.txt`.
	4. Create a file `symbols_local.txt` containing the names of all other publicly visible symbols from the upstream implementation that need to be removed from the global namespace; the format of the file is one symbol per line.  You can find these by symbols by running `nm -g potato.a | grep ' T '` and `nm -g potato.a | grep ' D '`, or on the individual object files (`*.o`).  The same file can be used for all parameterizations of this algorithm.  On macOS, symbols will be prefixed with an underscore (`_`); you should omit the underscore.  Add a line in the compilation target to run `scripts/symbols_local.sh` with `symbols_local.txt`.
6. Edit `src/kem/Makefile` at the `EDIT-WHEN-ADDING-KEM` marker to include `src/kem/potato/Makefile`.
7. Edit `src/Makefile` at the `EDIT-WHEN-ADDING-KEM` marker to add the various algorithms/parameterizations (`potato_512`, ...) to the list of KEMs enabled by default.

### Testing

1. Try building use `make clean; make`.
2. Our build system is configured so that any warning when compiling a non-upstream file is treated as an error.  Fix any such warnings/errors that arise.
3. Make sure the `./test_kem` works and includes your algorithm.
4. Check that `./speed_kem` works, includes your algorithm, and that the performance is inline with the expected performance documented in the submission.
5. For each algorithm (`potato_512`, ...), create a corresponding file `src/kem/potato/potato_512.kat` containing the **first** known answer test response values from the NIST submission.  Make sure that `./kat_kem` generates the corresponding file under `kat_kem_rsp`, and then run `scripts/check_kats.sh` to check that the KATs match.
6. Do a `git commit`.
7. Run `make prettyprint` to reformat non-upstream `.c` and `.h` files according to our coding conventions.  We use clang-format version 3.9.  (Unfortunately, different versions of clang-format may produce different results with the same configuration file, so you must use clang-format version 3.9).  
	- To install and use clang-format 3.9 on Ubuntu:
		- Try `sudo apt install clang-format-3.9`
		- If that doesn't work, try the following:
			1. `sudo add-apt-repository 'deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main'`
			2. `wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -`
			3. `sudo apt-get update -qq `
			4. `sudo apt-get install -qq -y clang-format-3.9`
		- You may have to run with `make prettyprint CLANGFORMAT=clang-format-3.9`.
	- To install and use clang-format 3.9 on macOS using brew:
		1. `brew unlink clang-format`
		2. `brew install https://raw.githubusercontent.com/Homebrew/homebrew-core/0c4314c499576b28e4c082b591228a8f940954c0/Formula/clang-format.rb`
		3. `brew switch clang-format 2016-06-27`
8. Check any changes made by the pretty-printer to ensure the meaning of the code did not change.
9. Do a `git commit`.

### Documentation

1. Add an algorithm datasheet in `docs/algorithms` for your module (`kem_potato.md`) containing information about each algorithm (`kem_potato_512`, ...) following the examples in the other files in this directory.  You may find the online Markdown table generator at http://www.tablesgenerator.com/markdown_tables helpful -- you can paste Markdown in, graphically edit, and then copy Markdown out.
2. Do a `git commit`.

### Submitting

1. Run `make pre-push` to run (almost) all of the the tests that our continuous integration system will run.  Fix any warnings or errors before continuing.
2. Make a pull request against `nist-branch` on Github.
3. Add the `nist-branch` label and the `not ready for merge` label.
4. Submitting a pull request will activate the Travis continuous integration build system, which builds liboqs on a variety of platforms.  Depending on the time of day and the load on Travis, the build may complete within a few minutes or be queued for sometimes up to an hour.
5. Once the Travis build is complete, check the status of the build.  If it failed, click on the red X, see which build targets failed, and check the logs to try to identify the problem so you can fix it.  Common reasons for Travis build failures include:
	- prettyprint inconsistencies
	- non-namespaced global symbols
	- compiler warnings in the non-upstream files treated as errors
6. Once your pull request is passing Travis builds, remove the `not ready for merge` label, and request a review from one of the team (either `dstebila` or `smashra`).
7. We'll review the code, test out the build, and follow up with you via comments on the pull request page.

Thanks for contributing to liboqs!
