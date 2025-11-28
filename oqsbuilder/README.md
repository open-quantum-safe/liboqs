# OQS Builder
This is the replacement for `copy_from_upstream.py`

**Progress**:
- ✅ clone remote repository
- ✅ apply patches
- 🚧 move source file from upstream into `liboqs/src`
    - 🚧 ML-KEM
- 🚧 render family-level `CMakeLists.txt`
- 🚧 render family-level header file (e.g. `kem_ml_kem.h`)
- 🚧 render family-level source file (e.g. `kem_ml_kem_512.c`)
- ⚠️ figure out how to check feature parity with `copy_from_upstream.py`


# OQS Build File
The build file `oqsbuildfile.yml` is the single source of truth from which the entire library (source files, header files, build files, documentation) is assembled and/or generated.

## Upstreams
Upstreams are external repositories from which `liboqs` curates source code.

### `git_url`

### `git_branch`

### `git_commit`

### `patches`

## Implementations
Each KEM, signature, and/or stateful signature can have one or more implementations.The `impls` field under each family maps one implementation key (e.g. `mlkem-native_ml_kem_768_aarch64`) to one set of implementation metadata.

### `arch`
Key of the [architecture](#architectures) of this implementation.

### External APIdeclarations
For KEM implementations, there are five functions to declares:
- `keypair`: name of the function that generates the key pair
- `keypair_derand` (optional): name of the function that generates the key pair, using randomness expanded from a user-supplied seed
- `enc`: name of the function that encapsulates a secret using a public key
- `enc_derand`: (optiona) name of the function that encapsulates a secret, using randomness expanded from a user-supplied seed
- `dec`: name of the function that decapsulates a ciphertext

### `upstream`
An upstream key. It must match one of the upstreams described under the `upstreams` section.

### `param`
Each implementation implements a single parameter set.

### `<impl_key>.enable_by`
Define the C pre-processing macro and/or CMake variable with which this implementation will be enabled. For example:

```yaml
cupqc_ml-kem-1024_cuda:
  enable_by: OQS_ENABLE_KEM_ml_kem_512_cuda
```

Will translate to:

```cmake
# src/kem/ml_kem/CMakeLists.txt
if(OQS_ENABLE_KEM_ml_kem_512_cuda)
    # ...
endif()
```

If not specified, then this implementation is always included.

Notes:
- Some implementations are further hidden behind platform guards. For example, CUDA implementations also need `OQS_USE_CUPQC` in addition to the individual implementation `enable_by`

### `copies`  
A description of how the content of an implementation should be assembled. This field can be a single string or a dictionary.
    - *Dictionary*: maps destination path to source path. Each destination path is relative to the implementation subdirectory (i.e. relative to `liboqs/src/<kem|sig|stfl_sig>/<family_key>/<impl_key>`). Each source path is relative to the upstream repository's root directory.
    - *Key*: references a reusable set of `dst:src` mappings. For example, `mlkem-native_ml_kem_<512|768|1024>_aarch` all use the same `copies` mapping, so each of the copy field could just be as follows:

```yaml
ml_kem:
  impls:
    mlkem-native_ml-kem-512_aarch64:
      copies: mlkem-native-aarch64-copies
    mlkem-native_ml-kem-768_aarch64:
      copies: mlkem-native-aarch64-copies
    mlkem-native_ml-kem-1024_aarch64:
      copies: mlkem-native-aarch64-copies

copies:
  mlkem-native-aarch64-copies:
    dst1: src1
    dst2: src2
```

### `includes` (optional)
A list of strings. Each item will be added to the cmake command `target_include_directories`.

```yaml
ml_kem:
  impls:
    mlkem-native_ml-kem-1024_aarch64:
      includes:
        private:
          - "${CMAKE_CURRENT_LIST_DIR}/${IMPL_KEY}"
          - "${PROJECT_SOURCE_DIR}/src/common/pqclean_shims"
```

For example, the `includes` field above will be translated into:

```cmake
target_include_directories(
    ml_kem_1024_aarch64
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mlkem-native_ml-kem-1024_aarch64
        ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims
)
```

This field is optional.

### `compile_opts` (optional)
List of compiler options that feed into `target_compile_options`.

```yaml
compile_opts: 
  public:
    - "-DMLK_CONFIG_PARAMETER_SET=1024"
    - "-DMLK_CONFIG_FILE=\"${CMAKE_CURRENT_LIST_DIR}/${IMPL_KEY}/integration/liboqs/config_x86_64.h\""
  private: ["-mavx2", "-mbmi2", "-mpopcnt"]
```

Translates to:

```cmake
target_compile_options(
    ml_kem_1024_x86_64 
    PRIVATE  
        -mavx2  -mbmi2  -mpopcnt
)
target_compile_options(
    ml_kem_1024_x86_64 
        PUBLIC 
        -DMLK_CONFIG_PARAMETER_SET=1024 
        -DMLK_CONFIG_FILE=".../integration/liboqs/config_x86_64.h"
)
```

This field is optional. Each of `public`, `private` is also optional.

### `link_libs` (optional)
List of library names that feed into `target_link_libraries`.

```yaml
link_libs:
  private: ["icicle::icicle_pqc_package"]
```

Translates to

```cmake
target_link_libraries(_target PRIVATE icicle::icicle_pqc_package)
```

### `cuda_arch` (optional)
**Currently used for [cuPQC's](https://docs.nvidia.com/cuda/cupqc/guides/getting_started.html) implementations only.**

As of Nov 2025, liboqs has cuPQC's ML-KEM implementation, but cuPQC also has a ML-DSA implementation that we might want to integrate later.

```yaml
cuda_arch: OFF
```

Will translate to the following settings for [`CUDA_ARCHITECTURES`](https://cmake.org/cmake/help/latest/prop_tgt/CUDA_ARCHITECTURES.html)

```cmake
set_property(TARGET _target PROPERTY CUDA_ARCHITECTURES OFF)
```

## Families
Each cryptographic primitive (KEM, signature, or stateful signature) has one or more families. For example, ML-KEM, ML-DSA, and SLH-DSA are three distinct families of primitives. Each family can have many [parameter sets](#parameter-set) and many implementations. Each implementation implements exactly one parameter set.

- KEM schemes are listed under the top-level key `kems`. Their files are listed under `src/kem`.
- The key of each KEM family is the name of the subdirectory. For example, ML-KEM files are located under `src/kem/ml_kem`. The key of each implementation under the same family is the name of the subdirectory. For example, the implementation `mlkem-native_ml-kem-512_ref` is located under `src/kem/ml_kem/mlkem-native_ml-kem-512_ref`.
- `sources` under each `impl` can contain both source files (`.c`, `.S`) and header/config files (`.h`)
- Each KEM family has a family-level `CMakeLists.txt` file. For example, ML-KEM has a `src/kem/ml_kem/CMakeLists.txt` file. There is no implementation-level list file. Under each implementation, the cmake variable `CMAKE_CURRENT_LIST_DIR` refers to the family-level list file.

### `header`
**Optional:** name of the family-level header file. Defaults to `{kem|sig|stfl_sig}_{family_key}.h`

### `version`
> **This definition diverges from `copy_from_upstream`**.

Some family of algorithms went through multiple verions, such as Kyber having distinct NIST Round 2, Round 3, and Round 4 version. **`liboqs` will integrate one version per family**. If there is a case to support multiple versions of the same family, it will be listed as a separate family:

```yaml
kems:
  kyber-r2:
    version: "NIST Round 2"
  kyber-r3:
    version: "NIST Round 3"
```

## Architecture
The `architectures` section describes various compilation architectures.

### `<arch_key>.enable_by`
**(optional)** The C pre-processing macro that enables this architecture. If this field is empty, then this architecture is always enabled (for example, portable implementations with `arch: "portable"` should never be disabled).

Architecture-level flag should surround implementation-level flags. If an architecture flag is disabled, then no implementation under this architecture is enabled.

Example:

```yaml
# oqsbuildfile.yml
kems:
  families:
    ml_kem:
      impls:
        icicle_ml-kem-768_icicle_cuda:
          arch: icicle_cuda
          enable_by: OQS_ENABLE_KEM_ml_kem_768_icicle_cuda

architectures:
  icicle_cuda:
    enable_by: "OQS_USE_ICICLE"
```

This configuration corresponds with:

```c
#if defined(OQS_USE_ICICLE)
#if defined(OQS_ENABLE_KEM_ml_kem_768_icicle_cuda)
    /* ICICLE ML-KEM-768 API calls */
#endif /* OQS_ENABLE_KEM_ml_kem_768_icicle_cuda */
#endif /* OQS_USE_ICICLE */
```

## Parameter Set
Each KEM/SIG/STFL_SIG scheme can have one or more parameter sets listed under the `params` key. For example:

```yaml
kems:
  families:
    ml_kem:
      params:
        ml_kem_512: # ...
        ml_kem_768: # ...
        ml_kem_1024: # ...
sigs:
  families:
    ml_dsa:
      params:
        ml_dsa_44: # ...
        ml_dsa_65: # ...
        ml_dsa_87: # ...
```

### `nist_level`
The security level of this parameter set, measured in [NIST level](https://csrc.nist.gov/projects/post-quantum-cryptography/post-quantum-cryptography-standardization/evaluation-criteria/security-(evaluation-criteria)). Must be one of 1, 2, 3, 4, 5.

### `ind_cca`
A boolean indicating whether this scheme achieves IND-CCA security. This field only exists under KEM families.

### `api_src`
**Optional:** name of the source file that contains the OQS common API (e.g. `OQS_KEM_ml_kem_512_new`) for this parameter set (e.g. `kem_ml_kem_512.c`). Defaults to `<kem|sig|stfl_sig>_<param_key>.c`

### `<param_key>.enable_by`
Specify the C pre-processing macro that enables this parameter set. Note that if the parameter set is disabled, then no individual implementation will be enabled even if the individual implementation is enabled.

For example:

```yaml
ml_kem_512:
  enable_by: OQS_ENABLE_KEM_ml_kem_512
```

translates to the following cmake and C code:

```cmake
if(OQS_ENABLE_KEM_ml_kem_512)
    add_library(... OBJECT kem_ml_kem_512.c)
endif()
```

```c
#ifdef OQS_ENABLE_KEM_ml_kem_512
OQS_KEM *OQS_KEM_ml_kem_512_new(void) { /* ... */ }
#endif /* OQS_ENABLE_KEM_ml_kem_512 */
```

The cmake variable is translated to [C macro](https://github.com/open-quantum-safe/liboqs/blob/97f6b86b1b6d109cfd43cf276ae39c2e776aed80/src/oqsconfig.h.cmake) using [`cmakedefine`](https://cmake.org/cmake/help/latest/command/configure_file.html#transformations) directive.

### `default_impl`
Specify a key to an implementation that implements this parameter set. The default implementation's [`enable_by`](#imple_keyenable_by) flag will be overwritten by this parameter set's [`enable_by`](#param_keyenable_by) flag so that if this parameter set is enabled, the default implementation is automatically enabled.

### `pklen`
Length of public key (bytes)

### `sklen`
Length of secret key (bytes)

### `ctlen`
Length of ciphertext (bytes). This field should exists only for KEM families.

### `sslen`
Length of shared secret (bytes). This field should exists only for KEM families.

### `keypair_seedlen`
**(optional)** Length of seed for generating keypair (bytes). If this field is not specified, it will default to 0 to indicate that this family (KEM, SIG, or STFL_SIG) does not support deterministic key generation.

### `encap_seedlen`
**(optionsl)** Length of seed for encapsulation (bytes). This field only exists for KEM families. If this field is not specified, it will default to 0 to indicate that this KEM family does not support deterministic encapsulation.
