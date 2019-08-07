##  Contributing new quantum-safe algorithms

All submissions must meet acceptance criteria given below. Algorithms may be removed if they no longer meet the acceptance criteria.

- **Algorithmic requirements:**
	- The algorithm must be submitted to the NIST Post-Quantum Cryptography project, or posted as update to an existing algorithm, and must be present in the current round
	- Algorithms whose security is considered effectively broken are not eligible for addition; see the Lifecycle section below for conditions on their removal.
	- KEMs can be IND-CPA or IND-CCA-secure, at any NIST security level.
	- Signature schemes can be EUF-CMA-secure, at any NIST security level.
- **Targets**:
	- **Operating systems:** The code must build on Linux, macOS, and Windows.
	- **Architecture:** The code must build at least on x86 and x64.  Build targets are available for various processor-specific features.
- **Source code requirements:**
	- **License:** Source code licensed under the MIT License, the BSD license, or in the public domain can be directly incorporated into the repository.  GPL code will not be included in the repository.
	- **Code quality:** The source code must meet the following quality checks, some of which are enforced by our continuous integration system:
		- Public API functions must be documented with appropriate Doxygen comments.
		- Source code must be formatted in accordance with our formatting requirements (`make prettyprint`).
		- All secrets in memory must be zero'ed out before being deallocated/released using our `OQS_MEM_*` functions.
		- Exported symbols must be namespaced.
		- We will periodically run static analysis other analysis tools to identify additional potential flaws.
	- **Common function usage:** The source code should make use of liboqs' random number generator, AES implementation, and SHA-3 implementation, rather than its own implementation.
- **Maintenance**: We hope the contributor will intend to help update the implementation in liboqs as the algorithm evolves or as we make enhancements to the library.
The goals and acceptance criteria for new algorithms on liboqs master branch are stated in the README.md.  In particular, our goals are to include high quality implementations for which the contributor aims to help with ongoing maintenance.

To discuss the contribution of a new algorithm to liboqs master branch, please contact [Douglas Stebila](https://www.douglas.stebila.ca/about/contact/) or post an issue on GitHub for discussion.

Additional information, including coding conventions, can be found on the [wiki](https://github.com/open-quantum-safe/liboqs/wiki).

## Contributing other enhancements

Bug reports can be filed as issues on GitHub.  Please use the "bug" label and indicate which version of liboqs the issue relates to (by specifying the commit hash), and what platform was in use (CPU, operating system, compiler).

If you have some improvements to an algorithm, are interested in getting liboqs to build on a new platform, or want to explore integrating liboqs into a new application, feel free to open an issue or contribute a pull request.

## Lifecycle

**Release cycle:** We aim to make releases 2 to 3 months.  Plans for each individual release can be found on the [Github projects board](https://github.com/open-quantum-safe/liboqs/projects/).  

**Versioning:** For approximately the period 2018-2019, we plan to label our releases with a variant of semantic versioning, using the notation *0.Y.Z*.  Semantic versioning allows that anything may change between *0.whatever* versions.  We intend to use *X.Y.Z* labelling as follows, for *X=0*.

- *X=0* will be used for approximately the period 2018-2019.  Later in 2019, we will revisit whether we are ready to make a *1.0.0* release, and specify a new verisoning method at that time.
- *Y* will be incremented when backwards incompatible changes are introduced that either change the public API or change the input/output behaviour of a cryptographic algorithm.  See explanation below.
- *Z* will be incremented when backwards compatible bug fixes are introduced.  

**Algorithm deprecation:** If an algorithm in master branch is found to be insecure or does not advance to the next round of the NIST competition, but is included in version $0.Y.Z$, it will be marked as deprecated using a compile time warning in version $0.(Y+1).Z'$ and removed in version $0.(Y+2).Z''$.

**Algorithm compatibility:** Unlike existing standardization cryptographic algorithms (SHA-2, SHA-3, PKCS\#1v1.5, nistp256, ...), post-quantum algorithms are under active development, and the mathematical algorithm of a cryptographic scheme may change: a particular name (e.g., "FrodoKEM-AES-640") may refer to different mathematical algorithms over time.  liboqs may update implementations as algorithms evolve.  During the $0.Y.Z$ phase of liboqs, versions $0.Y.Z$ and version $0.(Y+1).Z'$ may not be interoperable.  liboqs-reliant applications can check the `alg_version` member of the `OQS_KEM` data structure for each algorithm to obtain an identifier of the algorithm version used in a particular implementation; implementations returning the same `alg_version` for an algorithm will be interoperable.

**API stability:** The public API of liboqs master branch is currently considered to be the functions and macros in `oqs/common.h`, `oqs/oqsconfig.h`, `oqs/kem.h`, `oqs/rand.h`, `oqs/sig.h`, and includes all functions marked with `OQS_API`.  During the $0.Y.Z$ phase of liboqs, incompatible changes to the public API will lead to incrementing $Y$.

## License

liboqs is licensed primarily under the MIT License.  Submissions to liboqs master branch must be under the MIT License, a compatible license, or in the public domain.

## Testing

Builds are tested using continuous integration systems as follows:

- macOS 10.13.3 (clang 9.1.0) using Travis CI
- Ubuntu 14.04.5 (Trusty), with gcc, on ARM Cortex-A8 (via QEMU) using Travis CI
- Ubuntu 16.04.6 (Xenial), with gcc-4.9, gcc-5, gcc-6, gcc-7, and gcc-8, on x86_64 using CircleCI
- Ubuntu 18.04.2 (Bionic), with gcc-7, on x86_64 using CircleCI
- Debian 10 (Buster), with gcc-8.3.0, on amd64 using CircleCI
- Windows Server 2016, with Visual Studio 2017, on x86_64 using AppVeyor

You can locally run any of the integration tests that CircleCI runs.  First, you need to install CircleCI's local command line interface as indicated in the [installation instructions](https://circleci.com/docs/2.0/local-cli/).  Then:

	circleci local execute --job <jobname>

where `<jobname>` is one of the following:

- `debian-buster-amd64`
- `ubuntu-xenial-x86_64-gcc49`
- `ubuntu-xenial-x86_64-gcc5`
- `ubuntu-xenial-x86_64-gcc6`
- `ubuntu-xenial-x86_64-gcc7`
- `ubuntu-xenial-x86_64-gcc8`
- `ubuntu-xenial-x86_64-gcc8-noopenssl`
- `ubuntu-xenial-x86_64-gcc8-noshared`
- `ubuntu-bionic-x86_64`

You may receive an error "Failed uploading test results directory" when running CircleCI locally, that is expected behaviour.
