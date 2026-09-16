# liboqs CI Family Usage Report

An automated, weekly workflow reporting how much GitHub Actions CI
compute time is spent per algorithm family (BIKE, Classic McEliece,
ML-KEM, ML-DSA, SLH-DSA, XMSS, ...), to help prioritize CI optimization
work.

## Installation

Add `.github/workflows/ci-family-usage-report.yml` and
`scripts/ci_family_report.py` to the repository (no other footprint, no
extra secrets needed). Optionally adjust the cron schedule in the
workflow file (default: Mondays 03:00 UTC), or trigger it manually via
Actions -> "CI Family Usage Report" -> "Run workflow".

## How it works

`ci_family_report.py` is a single CLI with six subcommands, one per
pipeline stage:

- `generate-definitions` -- reads `src/{kem,sig,sig_stfl}/*.h` and groups
  algorithms into families by source subdirectory (no hardcoded list, so
  new algorithms are picked up automatically)
- `build-matrix` -- turns that into a GitHub Actions matrix
- `measure-family` -- builds+tests one family in isolation
  (`-DOQS_MINIMAL_BUILD=<family>`), run once per matrix entry in parallel
- `merge` -- combines the per-family measurements into one CSV
- `fetch-ci-data` -- pulls real job durations from recent CI runs via the
  GitHub API and classifies each job (dedicated-family / shared full-build
  / generic overhead)
- `report` -- combines both into the final Markdown+CSV report

The workflow chains these across three jobs (`generate-family-matrix` ->
`profile-family` matrix -> `aggregate-and-report`) and publishes the
result as a job summary, an artifact (90-day retention), and -- on
scheduled runs -- as a comment on a single tracking issue ("CI family
usage reports (automated)", auto-created on first run), providing a
browsable report history without committing generated files to the repo.

## Estimation methodology

Families with a dedicated CI job (currently SLH-DSA, tested in isolation
via `-DOQS_MINIMAL_BUILD=SIG_slh_dsa` while regular jobs disable it) get
their real measured CI time directly -- no model. All other families
share a "full build" job; their slice of that shared time is estimated
from the ratio of their isolated local build+test times.

**Known limitation:** this can underestimate families with
disproportionately expensive test suites. SLH-DSA itself is a good
example -- ~25s locally, but 70-90 min in its real dedicated CI job. If a
family looks off, check whether it has a dedicated CI job pattern like
SLH-DSA's and add it to `DEDICATED_FAMILY_PATTERNS` in the script.

## Testing locally

New workflows only become dispatchable via the GitHub UI/CLI once merged
to the default branch. To test beforehand, use
[`act`](https://github.com/nektos/act) (already recommended in
`CONTRIBUTING.md`):

```bash
mkdir -p /tmp/act-artifacts
act workflow_dispatch \
  -W .github/workflows/ci-family-usage-report.yml \
  --artifact-server-path /tmp/act-artifacts \
  --input n_ci_runs=2 \
  -s GITHUB_TOKEN="$(gh auth token)"
```

`--artifact-server-path` is required for artifact upload/download between
jobs to work under `act`. The token only needs read access to public
repos. Expect this to take a while locally, dominated by the
`profile-family` matrix building all 18 families.

Individual subcommands can also be run directly without `act`; see
`python3 ci_family_report.py <subcommand> --help`.
