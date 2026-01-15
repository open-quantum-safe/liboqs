# OQS Builder
This is the replacement for `copy_from_upstream.py`

**Progress**:
- ✅ clone remote repository
- ✅ apply patches
- ✅ move source file from upstream into `liboqs/src`
- 🔨 Re-produce existing builds
    - ✅ ML-KEM
- ✅ Parse `oqsbuildfile.yml` into structured data instead of Python dictionary
- 🔨 Experiment with converting `oqsbuildfile.yml` into TOML. TOML is better because I can key a table with dot-separatec namespacing, which reduces the need for excessive indents.
- 🔨 `copies` should support direct mapping, re-usable mapping in the same oqsbuildfile, and remote mapping from `META.yaml`
- ⚠️ figure out how to check feature parity with `copy_from_upstream.py`

`oqsbuildfile` documentation can be found [here](./docs/oqsbuildfile.md).
