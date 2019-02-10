#!/bin/bash

# sudo apt-get install qemu

set -e

CHOST=arm-linux-gnueabi

qemu-arm -cpu cortex-a8 -L /usr/arm-linux-gnueabi tests/example_kem
qemu-arm -cpu cortex-a8 -L /usr/arm-linux-gnueabi tests/test_kem

# qemu-arm -cpu cortex-a8 -L /usr/arm-linux-gnueabi tests/example_sig
# qemu-arm -cpu cortex-a8 -L /usr/arm-linux-gnueabi tests/test_sig
