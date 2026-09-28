# SPDX-License-Identifier: MIT


def normalize_platform(platform):
    """Translate upstream Arm platform names to liboqs CPU extensions."""
    if platform["architecture"] == "arm_8":
        platform["architecture"] = "ARM64_V8"

    if platform["architecture"] == "ARM64_V8" and "required_flags" in platform:
        arm_flags = {
            "asimd": "arm_neon",
            "aes": "arm_aes",
            "sha3": "arm_sha3",
        }
        platform["required_flags"] = [
            arm_flags.get(flag, flag) for flag in platform["required_flags"]
        ]

    return platform
