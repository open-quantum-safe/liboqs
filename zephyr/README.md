# Zephyr Project Port

liboqs can be used as a module for the [Zephyr RTOS](https://www.zephyrproject.org/). 

## Installation 

You have to add liboqs to your West workspace using a [West Manifest](https://docs.zephyrproject.org/latest/develop/west/manifest.html#west-manifests)

In your manifest file (`west.yml`), add the following:
```
remotes:
    # <other remotes>
    - name: liboqs
      url-base: https://github.com/open-quantum-safe

projects:
    # <other projects>
    - name: liboqs
      path: modules/crypto/liboqs
      revision: main
      remote: liboqs
```

After adding the new information to your manifest file, run `west update` to download and install liboqs as a Zephyr module. After that, you can use it in your projects.

Currently, Zephyr versions 3.4 and 3.5 are supported. Please feel free to open an issue or a PR in case you need another version supported (without guarantee that older versions can be supported at all).

## Usage

### Disclaimer regarding random number generation

In order to properly use the Zephyr port of liboqs, you have to provide a custom callback function for random number generation using the `OQS_randombytes_custom_algorithm()` API. Otherwise, all key generation and signing operations will fail.

In the two provided sample applications (also see [Samples](#Samples)), a callback is set using the default `sys_rand_get()` method from Zephyr. This method, however, does **not** provide random data suitable for cryptographic operations and is only good for testing purposes. Make sure to use a proper entroy source from your hardware to obtain actual random data.   

### Configuration

The port provides a variety of configurable options using Kconfig. Once you have the liboqs module enabled with `CONFIG_LIBOQS=y`, you can manually enable or disable specific KEM or Signature algorithms using the `LIBOQS_ENABLE_KEM_xxx` and `LIBOQS_ENABLE_SIG_xxx` options.

The algorithms to be standardized by NIST are enabled by default, all others are disabled by default.

### Supported architectures

At the moment, the following architectures are supported with the Zephyr port:
* 32-bit ARM
* 64-bit ARM
* x86
* Native Posix

Other architectures supported by Zephyr are not supported with this port, as liboqs itself doesn't support these architectures (the modules can't be enabled in those cases). Please feel free to raise an issue or PR in case a new architecture is required.

### Samples

Two sample applications are provided, demonstrating the usage of the library from within Zephyr. See [`samples/KEMs`](https://github.com/open-quantum-safe/liboqs/tree/main/zephyr/samples/Signatures) and [`samples/Signatures`](https://github.com/open-quantum-safe/liboqs/tree/main/zephyr/samples/KEMs).