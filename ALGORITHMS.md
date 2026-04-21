# Supported algorithms

This file documents the algorithms currently integrated into `liboqs`, with per-variant detail on standardization status, upstream maintenance, OQS support tier, security level, constant-time status, formal verification, and available optimizations.

## Definitions

### Upstream-maintenance

The **Upstream maintenance** column records the maintenance level the upstream implementation team has committed to, using one of the labels below. A label is only set when the upstream has confirmed it; otherwise the entry reads `TBD`.

- **Actively maintained.** There is a named, responsive upstream contact or team. Upstream commits to investigating security reports in a timely manner and incorporating fixes. Upstream releases updates that OQS can track. Upstream has a public written statement describing this commitment, which OQS links to.
- **Best effort.** There is a named upstream contact who acknowledges OQS's integration. Upstream will attempt to respond to security reports and incorporate fixes when able, with no commitment on timing or release cadence. Users should not depend on timely upstream fixes.
- **No active maintenance.** There is no responsive upstream contact, or upstream has publicly stated wind-down or end-of-life. Fixes will not necessarily be incorporated from upstream. Users considering this algorithm should plan accordingly (alternatives, migration, or maintaining their own fork).
- **TBD.** OQS has not yet obtained a confirmed statement from the upstream. This is not a judgment about the upstream; it means the conversation has not happened or not concluded.

## OQS support tier

The **OQS tier** column records OQS's own support commitment for each algorithm. Tiers are self-assigned by the OQS technical governance team via PR review, and are structurally parallel to the platform tiers defined in [PLATFORMS.md](PLATFORMS.md). Promotion or demotion between tiers is proposed via PR and promotion requires the algorithm to have spent reasonable time at its current tier. Tier assignments are not a recommendation for production use of any algorithm in `liboqs`; see the project-wide disclaimer in [README.md](README.md).

- **Tier 1 — Core.** OQS's highest level of commitment and scrutiny. Standardized by a recognized body (NIST, ISO, IETF); upstream marked `Actively maintained` with a public support statement; covered by the OQS security response process; constant-time tested on all Tier 1 platforms where applicable; built and tested on every Tier 1 platform in CI, every parameter set; at least two OQS committers familiar with the code; has an identified entry in `CODEOWNERS`.
- **Tier 2 — Supported.** Receives general OQS support, with weaker guarantees than Tier 1. Standards-track or standardized; upstream at least `Best effort`; covered by the OQS security response process on a best-effort basis; built and tested on all Tier 1 platforms in CI; at least one OQS committer familiar with the code; has an identified entry in `CODEOWNERS`.
- **Tier 3 — Community.** Maintained primarily through community contributions. Any standardization level, including "Not selected" and algorithms under wind-down; upstream status may be any value including `No active maintenance`; builds in CI, tests best-effort; no OQS commitment beyond accepting community PRs and refusing to knowingly ship broken code. Users are expected to evaluate suitability themselves.

## Key encapsulation mechanisms

<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_KEM_START -->
<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_KEM_END -->

## Signature schemes

<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_START -->
<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_END -->

## Stateful signature schemes

<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_STFL_START -->
<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_STFL_END -->
