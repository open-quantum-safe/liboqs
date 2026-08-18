# Continuous Integration (CI)

This document aims to provide a accessible yet comprehensive overview of the liboqs CI setup.

## GitHub Actions

liboqs relies on GitHub Actions for almost all of its CI and makes extensive use of [reusable workflows](https://docs.github.com/en/actions/sharing-automations/reusing-workflows).
All workflow files are located in the `.github/workflows` subdirectory.

### Caller workflows

These workflows are triggered by GitHub events (for example, a pull request or a release).
They implement the logic dictating which tests should run on which events.

#### <a name="push.yml"></a> Push workflow (`push.yml`)

This workflow is triggered by pushes to non-`main` branches.
It calls only [basic checks](#basic.yml) unless one of the following strings is included in the commit message:
- "[full tests]": calls [all platform tests](#platforms.yml).
- "[extended tests]": calls the [extended tests](#extended.yml).
- "[trigger downstream]": calls the [downstream release tests](#downstream-release.yml).

To trigger multiple test suites, include multiple trigger strings in the commit message.
For example, "[full tests] [trigger downstream]" will trigger both the platform tests and the downstream release tests.

#### <a name="pr.yml"></a> Pull request workflow (`pr.yml`)

This workflow runs on pull requests.
It calls [basic checks](#basic.yml), [code coverage tests](#code-coverage.yml), [platform tests](#platforms.yml) and [scorecard analysis](#scorecard.yml).

#### <a name="commit-to-main.yml"></a> Commit-to-main workflow (`commit-to-main.yml`)

This workflow runs on pushes to the `main` branch (typically done automatically when a pull request is merged).
It calls [platform tests](#platforms.yml), [code coverage tests](#code-coverage.yml), [scorecard analysis](#scorecard.yml), and [basic downstream tests](#downstream-basic.yml).

#### <a name="weekly.yml"></a> Weekly workflow (`weekly.yml`)

This workflow is triggered by a weekly schedule.
It calls [extended tests](#extended.yml), [scorecard analysis](#scorecard.yml), and [continuous benchmarking](#kem-bench.yml-sig-bench.yml)

#### <a name="release.yml"></a> Release workflow (`release.yml`)

This workflow is triggered when a release (including a pre-release) is published on GitHub.
It calls [extended tests](#extended) and [downstream release tests](#downstream-release.yml).

### Callable workflows

These workflows are not triggered directly by any GitHub event.
They are instead called by one of the [caller workflows](#caller-workflows).
Users with "write" permissions can also trigger them manually via the GitHub web UI or REST API.

#### <a name="basic.yml"></a> Basic checks (`basic.yml`)

This workflow runs a minimal set of tests that should pass before heavier tests are triggered.

#### <a name="code-coverage.yml"></a> Code coverage tests (`code-coverage.yml`)

This workflow runs code coverage tests and uploads the results to [Coveralls.io](https://coveralls.io/github/open-quantum-safe/liboqs).

#### <a name="<platform>.yml"></a> Individual platform tests (`<platform>.yml`)

These workflows contain tests for the individual [platforms supported by liboqs](PLATFORMS.md).
Currently, these include
- `android.yml`,
- `apple.yml`,
- `macos.yml`,
- `linux.yml`,
- `windows.yml`, and
- `zephyr.yml`.

All of these these are wrapped by [`platforms.yml`](#platforms.yml).

#### <a name="platforms.yml"></a> All platform tests (`platforms.yml`)

This workflow calls all of the [platform-specific tests](#<platform>.yml).

#### <a name="extended.yml"></a> Extended tests (`extended.yml`)

This workflow calls tests which are either resource intensive or rarely need to be triggered.
Currently, this includes constant-time testing with valgrind and the full suite of NIST Known Answer Tests.

#### <a name="downstream-basic.yml"></a> Basic downstream trigger (`downstream-basic.yml`)

This workflow triggers basic CI for a selection of projects that depend on `liboqs`.
Currently, these include
- [`OQS OpenSSL3 provider`](https://github.com/open-quantum-safe/oqs-provider)
- [`OQS-BoringSSL`](https://github.com/open-quantum-safe/boringssl)
- [`OQS-OpenSSH`](https://github.com/open-quantum-safe/openssh)
- [`OQS Demos`](https://github.com/open-quantum-safe/oqs-demos)
- [`liboqs-cpp`](https://github.com/open-quantum-safe/liboqs-cpp)
- [`liboqs-go`](https://github.com/open-quantum-safe/liboqs-go)
- [`liboqs-python`](https://github.com/open-quantum-safe/liboqs-python)

Callers must include `secrets: inherit` in order for the appropriate access tokens to be passed to this workflow.

#### <a name="downstream-release.yml"></a> Downstream release trigger (`downstream-release.yml`)

This workflow triggers release tests for a selection of projects that depend on `liboqs`.
Currently, this is only the [`OQS OpenSSL3 provider`](https://github.com/open-quantum-safe/oqs-provider).
Callers must include `secrets: inherit` in order for the appropriate access tokens to be passed to this workflow.

#### <a name="scorecard.yml"></a> OpenSSF scorecard analysis (`scorecard.yml`)

This workflow runs the [OpenSSF scorecard](https://github.com/ossf/scorecard) tool.
It is additionally triggered automatically when branch protection rules are changed.
Callers must include `secrets: inherit` in order for the appropriate access tokens to be passed to this workflow.

#### <a name="kem-bench.yml-sig-bench.yml"></a> KEMs and signatures continuous benchmarking (`kem-bench.yml` and `sig-bench-yml`)

These workflows execute a benchmarkig framework to retrieve the performance of KEM and signature algorithms in CPU cycles.
When new algorithms are added to the codebase, they must be included inside the algorithms matrices found within these files.

## Travis CI

In the past, we used Travis CI to test on [some IBM platforms](PLATFORMS.md#tier-3-1) that are not supported by GitHub Actions.
Our Travis builds are currently disabled pending resolution of [issue #2068](https://github.com/open-quantum-safe/liboqs/issues/2068).
