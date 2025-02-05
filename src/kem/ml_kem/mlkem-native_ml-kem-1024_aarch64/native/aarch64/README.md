[//]: # (SPDX-License-Identifier: CC-BY-4.0)

# AArch64 backend (little endian)

This directory contains a native backend for little endian AArch64 systems. It is derived from the following research
works:

- _Neon NTT: Faster Dilithium, Kyber, and Saber on Cortex-A72 and Apple M1_, Hanno Becker, Vincent Hwang, Matthias
  J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang, [https://eprint.iacr.org/2021/986](https://eprint.iacr.org/2021/986)
- _Fast and Clean: Auditable high-performance assembly via constraint solving_, Amin Abdulrahman, Hanno Becker, Matthias
  J. Kannwischer, Fabien Klein, [https://eprint.iacr.org/2022/1303](https://eprint.iacr.org/2022/1303)


## Variants

This backend comes in two versions: "clean" and optimized. The "clean" backend is handwritten and meant to be easy to read and modify; for example, is heavily leverages register aliases and assembly macros. This directory contains the optimized version, which is automatically generated from the clean one via [SLOTHY](https://github.com/slothy-optimizer/slothy). Currently, the
target architecture is Cortex-A55, but you can easily re-optimize the code for a different microarchitecture supported
by SLOTHY, by adjusting the parameters in [optimize.sh](../../../test/aarch64_clean/src/optimize.sh).
