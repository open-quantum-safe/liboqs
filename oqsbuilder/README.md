# OQS Builder
This is the replacement for `copy_from_upstream.py`

**Progress**:
- ✅ clone remote repository
- ✅ apply patches
- ✅ move source file from upstream into `liboqs/src`
- 🔨 Re-produce existing builds
    - ✅ ML-KEM
        - ✅ render family-level `CMakeLists.txt`
        - ✅ render family-level header file (e.g. `kem_ml_kem.h`)
        - ✅ render family-level source file (e.g. `kem_ml_kem_512.c`)
        - 🔨 render `.CMake/alg_support.cmake`
        - 🔨 render documentation
- ✅ Parse `oqsbuildfile.yml` into structured data instead of Python dictionary
- 🔨 Experiment with converting `oqsbuildfile.yml` into TOML. TOML is better because I can key a table with dot-separated namespacing instead of indenting by more than 3 levels
- 🔨 `copies` should support direct mapping, re-usable mapping in the same oqsbuildfile, and remote mapping from `META.yaml`
- ⚠️ figure out how to check feature parity with `copy_from_upstream.py`

`oqsbuildfile` documentation can be found [here](./docs/oqsbuildfile.md).
