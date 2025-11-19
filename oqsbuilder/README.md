# OQS Builder
This is the replacement for `copy_from_upstream.py`

**Progress**:
- ✅ clone remote repository
- ✅ apply patches
- 🚧 move source file from upstream into `liboqs/src`
    - 🚧 ML-KEM
- ⚠️ render `CMakeLists.txt` and other build files
- ⚠️ figure out how to check feature parity with `copy_from_upstream.py`

## Spot checks
```bash
# Should print version
python -m oqsbuilder --version

python -m oqsbuilder --file oqsbuilder/INTEGRATIONS.yml copy
```

# OQS Build File
The build file `oqsbuildfile.yml` is the single source of truth from which the entire library (source files, header files, build files, documentation) is assembled and/or generated.

For each implementation, a `IMPL_KEY` will be defined like this. The value of `IMPL_KEY` is with the key of the corresponding implementation.

```cmake
# src/kem/ml_kem/CMakeLists.txt

if(OQS_ENABLE_KEM_ml_kem_512)
    set(IMPL_KEY "mlkem-native_ml-kem-512_ref")
    # ... other stuff ...
endif()
```

## Implementations
- Within each implementation, `includes` and `compile_opts` are all going to be literal strings. These literal strings may contain cmake variables that are processed by cmake. I do not want to complicate things with my own in-house "template render" unless absolutely necessary
- `copies` maps destination paths (relative to `src/<kem|sig|stfl_sig>/<impl_key>`) to source path (relative to upstream root)
- `sources` lists the source files that will go into the corresponding object (see below), but this list is optional; if it is not specified, then we filter `copies` by file extensions

```cmake
add_library(ml_kem_512_ref OBJECT <... list of source files ...>)
```

## KEMs
- KEM schemes are listed under the top-level key `kems`. Their files are listed under `src/kem`.
- The key of each KEM family is the name of the subdirectory. For example, ML-KEM files are located under `src/kem/ml_kem`. The key of each implementation under the same family is the name of the subdirectory. For example, the implementation `mlkem-native_ml-kem-512_ref` is located under `src/kem/ml_kem/mlkem-native_ml-kem-512_ref`.
- `sources` under each `impl` can contain both source files (`.c`, `.S`) and header/config files (`.h`)
- Each KEM family has a family-level `CMakeLists.txt` file. For example, ML-KEM has a `src/kem/ml_kem/CMakeLists.txt` file. There is no implementation-level list file. Under each implementation, the cmake variable `CMAKE_CURRENT_LIST_DIR` refers to the family-level list file.
