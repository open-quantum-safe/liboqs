# Supported platforms

This file documents the different platforms supported by `liboqs` and therefore defines three different support tiers:

## Support tiers

This classification is roughly based on the [rust platform support tier classification](https://doc.rust-lang.org/beta/rustc/platform-support.html):

### Tier 1

Tier 1 targets can be thought of as "guaranteed to work". The CI system builds and tests binary versions for each tier 1 target to make sure any change does not negatively affect those platforms. Platform-specific build documentation must exist. Tier 1 targets marked with a dagger (†) are additionally tested for constant-time behaviour. The CI system contains automated constant-time testing for each of these starred targets, and all failures are documented in the `tests/constant_time` directory. IMPORTANT: This does not mean that constant-time behaviour is guaranteed on these targets, or that non-constant-time behaviour is limited to documented exceptions. It does, however, mean that `liboqs` developers should track constant-time issues on these platforms.

### Tier 2

Tier 2 targets can be thought of as "guaranteed to build". The `liboqs` CI system contains builds for each tier 2 target; testing may or may not be available (typically depending on CI system platform availability). Therefore, tier 2 targets often work to quite a good degree and patches are always welcome! Tier 2 targets may also have known deficiencies caused by a lack of expertise to fix those on a given platform. Again, help and PRs to move platforms from tier 2 to tier 1 are always welcome.

### Tier 3

Tier 3 targets are those which the `liboqs` codebase has support for, but which the CI system does not build or test automatically, so they may or may not work. Platform-specific build documentation should exist.

## Platform tier policy

Tier 2 and tier 1 targets place work on `liboqs` core project developers as a whole, to avoid breaking the target. The broader `liboqs` community may also feel more inclined to support higher-tier targets in their work. Thus, these tiers require commensurate and ongoing efforts from the maintainers of the target, to demonstrate value and to minimize any disruptions to ongoing `liboqs` development.

This policy defines the requirements for accepting a proposed target at a given level of support.

Each tier builds on all the requirements from the previous tier, unless overridden by a stronger requirement.

Change of tier is subject to approval by the `liboqs` technical governance team.  This team is responsible for reviewing and evaluating the target, based on these requirements and their own judgment. The tea may apply additional requirements, including subjective requirements, such as to deal with issues not foreseen by this policy. (Such requirements may subsequently motivate additions to this policy.)

While these criteria attempt to document the policy, that policy still involves human judgment. Targets must fulfill the spirit of the requirements as well, as determined by the judgment of the approving team. Reviewers and team members evaluating targets and target-specific patches should always use their own best judgment regarding the quality of work, and the suitability of a target for the `liboqs` project. Neither this policy nor any decisions made regarding targets shall create any binding agreement or estoppel by any party.

Before filing an issue or pull request (PR) to introduce or promote a target, the target should already meet the corresponding tier requirements. This does not preclude an existing target's maintainers using issues (on the `liboqs` repository or otherwise) to track requirements that have not yet been met, as appropriate; however, before officially proposing the introduction or promotion of a target, it should meet all of the necessary requirements. A target proposal must quote the corresponding requirements verbatim and respond to them as part of explaining how the target meets those requirements. (For the requirements that simply state that the target or the target developers must not do something, it suffices to acknowledge the requirement.)

Several parts of this policy require providing target-specific documentation. Such documentation should typically appear in a subdirectory of the platform-support section of the `liboqs` manual, with a link from the target's entry in platform support.

Note that a target must have already received approval for the next lower tier, and spent a reasonable amount of time at that tier, before making a proposal for promotion to the next higher tier; this is true even if a target meets the requirements for several tiers at once. This policy leaves the precise interpretation of "reasonable amount of time" up to the approving team; the team may scale the amount of time required based on their confidence in the target and its demonstrated track record at its current tier. At a minimum, multiple stable releases of `liboqs` should typically occur between promotions of a target.

The availability or tier of a target in stable `liboqs` is not a hard stability guarantee about the future availability or tier of that target. Higher-level target tiers are an increasing commitment to the support of a target, and we will take that commitment and potential disruptions into account when evaluating the potential demotion or removal of a target that has been part of a stable release. The promotion or demotion of a target will not generally affect existing stable releases, only current development and future releases.

In this policy, the words "must" and "must not" specify absolute requirements that a target must meet to qualify for a tier. The words "should" and "should not" specify requirements that apply in almost all cases, but for which the approving teams may grant an exception for good reason. The word "may" indicates something entirely optional, and does not indicate guidance or recommendations. This language is based on [IETF RFC 2119](https://datatracker.ietf.org/doc/html/rfc2119).

## Platforms supported

### Tier 1

- x86_64/amd64/x64 for Ubuntu Linux (Focal)†
- x86_64/amd64/x64 for MacOS (XCode 14 and 15)
- aarch64 for Ubuntu (Focal)
- aarch64 for MacOS (XCode 15)
- armhf/ARM7 and aarch64 emulation on Ubuntu

### Tier 2

- x86_64/amd64/x64 for Windows 2022
- armeabi-v7a, arm64-v8a, x86, x86_64 for Android
- aarch64 for Apple iOS and tvOS (CMake `-DPLATFORM=OS64` and `TVOS`)
- arm64, arm (32 bit), x86, x86_64, riscv32, riscv64 for Zephyr

### Tier 3

- x86 for Windows
- ppc64le for Ubuntu (Focal)
- s390x for Ubuntu (Focal)

