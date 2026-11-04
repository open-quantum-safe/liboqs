# Contributing

The OQS core team welcomes all proposals to improve this project. This may take 
the form of [a discussion](https://github.com/open-quantum-safe/liboqs/discussions)
for input or feedback, possible bug reports or feature requests via [issues](https://github.com/open-quantum-safe/liboqs/issues)
as well as new code and documentation via a [pull request (PR)](https://github.com/open-quantum-safe/liboqs/pulls).

## Baseline design goal

OQS is a collection of many different PQC algorithms, maintained by a small team of people who are not guaranteed to be versed in the intricate details of each algorithm.

Therefore, all contributions to the general logic of the project should be as independent of any single algorithm such as to ease long-term maintainability. If changes are contributed catering to the properties of a specific algorithm, it is expected that consideration is given at least how the other algorithms of the same type (KEM or SIG) should cater to the proposed changes, e.g., by way of a new, generally satisfiable API.

All contributions to a specific algorithm ideally come with the willingness to provide long-term support, or at least a contact person that can help the OQS team pinpoint potential problems with the algorithm.

## Review and Feedback

We aim to provide timely feedback to any input. If you are uncertain as to whether
a particular contribution is welcome, needed or timely, please first open an [issue](https://github.com/open-quantum-safe/liboqs/issues)
particularly in case of possible bugs or new feature requests or create a
[discussion](https://github.com/open-quantum-safe/liboqs/discussions).

## Pull requests

Pull requests should clearly state their purpose, possibly referencing an existing
[issue](https://github.com/open-quantum-safe/liboqs/issues) when resolving it.

Pull requests containing code, documentation, or text produced with the help of generative AI must declare that in the pull request description and describe the nature of the use. Contributors are expected to have verified and affirm such contributions themselves before submission.
Contributors using AI assistants are encouraged to read the OpenSSF's [Security-Focused Guide for AI Code Assistant Instructions](https://best.openssf.org/Security-Focused-Guide-for-AI-Code-Assistant-Instructions).

All PRs should move to "Ready for Review" stage only if all CI tests pass (are green).

The OQS core team is happy to provide feedback also to Draft PRs in order to improve
them before the final "Review" stage.

### Coding style

This project has adopted a slightly modified [Google code formatting style](https://astyle.sourceforge.net/astyle.html#_style=google) for the core components
of the library as documented in the [style template](.astylerc).
The `astyle` tool is used to check formatting in CI.
Due to variations in behaviour across version and platforms, it is possible to encounter CI failures even if code has been locally formatted with `astyle`.
To assist with this inconvenience, we provide a convenience script which runs `astyle` in the same Docker image that we use for the CI checks:
```bash
LIBOQS_DIR=<liboqs directory> ./scripts/format_code.sh
```
This script has been tested on x86\_64 Ubuntu and arm64 macOS. Contributions for other platforms are welcome and appreciated!

### Git hooks

This project uses git hooks to automatically run linters and formatters. Git
hooks are stored under `.githooks/`. After cloning this repository, you can
point git to use these hooks by setting `core.hooksPath`:

```bash
git config core.hooksPath .githooks
```

> `core.hooksPath` is available in git 2.9 or newer; for older versions of git,
> please manually set up symlinks or copy the hooks into `.git/hooks`.

### Public and internal APIs

Public API functions are marked with the `OQS_API` keyword. These functions are made available to users of the library.
OQS also has an internal API for common code such as hashing and memory management.
The OQS test programs use this internal API.
These programs link against the `oqs-internal` library, which is built alongside the main library.
The main library also contains the common code, but it does not expose it to external callers.
The internal library is not installed via `ninja install`, and source code should not link against it.

### Continuous Integration (CI)

`liboqs` uses GitHub Actions for CI.
For a comprehensive overview of our CI setup, see [CI.md](CI.md).

#### Running CI on your branch

OQS attempts to be responsible with resource usage and only runs a minimal set of tests automatically on push.
A more thorough test suite runs automatically on pull requests.
To trigger these tests before creating a PR, include the string "[full tests]" in a commit message.
Other trigger strings are documented in [CI.md](CI.md#push.yml).

#### Running CI locally

[Act](https://github.com/nektos/act) is a tool facilitating local execution of
GitHub CI jobs. When executed in the main `liboqs` directory, 

    act -l Displays all GitHub CI jobs
    act -j some-job Executes "some-job"

When installing `act` as a GitHub extension, prefix the commands with `gh `.

## Modifications to CI

Modifications to GitHub Actions workflows are checked with [actionlint](https://github.com/rhysd/actionlint) during the [basic.yml](.github/workflows/basic.yml) job, protecting the CI chain and against wrong approval decisions based on improper CI runs.  Changes to these workflows can be validated locally with `actionlint`:

```bash
actionlint .github/workflows/*.yml
```

or running the CI locally (as above):

```bash
act workflow_call -W '.github/workflows/basic.yml'
```

### New features

Any PR introducing a new feature is expected to contain a test of this feature
and this test should be part of the CI pipeline.

## Failsafe

If you feel your contribution is not getting proper attention, please be sure to
add a tag to one or more of our [most active contributors](https://github.com/open-quantum-safe/liboqs/graphs/contributors).

## Issues to start working on

If you feel like contributing but don't know what specific topic to work on,
please check the [open issues tagged "good first issue" or "help wanted"](https://github.com/open-quantum-safe/liboqs/issues).

You can also take a look at the [contribution wishlist](https://github.com/open-quantum-safe/liboqs/wiki/Contribution-wishlist) for more substantial contributions we are interested in.

## Custom CodeQL queries

There are two ways in which CodeQL runs in this repo: via GitHub's standard
code scanning settings, and via `.github/workflows/codeql.yml`:
- GitHub's standard scans are run via liboqs' repository code scanning settings.
  Results appear as code scanning alerts in the repository's Security tab.
- `.github/workflows/codeql.yml` runs liboqs's own CodeQL queries on every pull
  request and fails if any of them reports a result. Every result is listed in
  the job log; GitHub also shows the first 10 as error annotations on the pull
  request. Its results are not uploaded 
  to code scanning: while default setup is enabled, GitHub rejects CodeQL results uploaded 
  by other workflows ([details](https://docs.github.com/en/code-security/reference/code-scanning/sarif-files/troubleshoot-sarif-uploads/default-setup-enabled)).
  The workflow can also be run manually from the repository's Actions tab.

The custom queries and their tests live in `.github/codeql/`:
- `.github/codeql/queries/` holds the queries (e.g. `openssl-return-check.ql`,
  which flags `EVP_*` calls whose status code is discarded instead of checked
  with `OQS_OPENSSL_GUARD`)
- `.github/codeql/test/<query-name>/` holds a unit test for each query: a small
  C file with good and bad examples, a `.qlref` naming the query, and a
  `.expected` file listing the results the query must report.

The commands below need the CodeQL CLI: either install the GitHub CLI extension
(`gh extension install github/gh-codeql`, then use `gh codeql` in place of
`codeql`), or download a CodeQL bundle from the
[github/codeql-action releases](https://github.com/github/codeql-action/releases).

To add a query, add the `.ql` file and its test directory, then generate the
`.expected` file and check that it lists exactly the bad examples:

```bash
codeql test run --learn .github/codeql/test
```

Run `codeql test run .github/codeql/test` (without `--learn`) to check that
existing queries still behave as expected.
