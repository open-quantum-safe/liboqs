# Contributing

The OQS core team welcomes all proposals to improve this project. This may take 
the form of [a discussion](https://github.com/open-quantum-safe/liboqs/discussions)
for input or feedback, possible bug reports or feature requests via [issues](https://github.com/open-quantum-safe/liboqs/issues)
as well as new code and documentation via a [pull request (PR)](https://github.com/open-quantum-safe/liboqs/pulls).

## Review and Feedback

We aim to provide timely feedback to any input. If you are uncertain as to whether
a particular contribution is welcome, needed or timely, please first open an [issue](https://github.com/open-quantum-safe/liboqs/issues)
particularly in case of possible bugs or new feature requests or create a
[discussion](https://github.com/open-quantum-safe/liboqs/discussions).

## Pull requests

Pull requests should clearly state their purpose, possibly referencing an existing
[issue](https://github.com/open-quantum-safe/liboqs/issues) when resolving it.

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

### Running CI locally

#### Github CI

[Act](https://github.com/nektos/act) is a tool facilitating local execution of
github CI jobs. When executed in the main `oqsprovider` directory, 

    act -l Displays all github CI jobs
    act -j some-job Executes "some-job"

When installing `act` as a github extension, prefix the commands with `gh `.

### New features

Any PR introducing a new feature is expected to contain a test of this feature
and this test should be part of the CI pipeline.

## Failsafe

If you feel your contribution is not getting proper attention, please be sure to
add a tag to one or more of our [most active contributors](https://github.com/open-quantum-safe/liboqs/graphs/contributors).

## Issues to start working on

If you feel like contributing but don't know what specific topic to work on,
please check the [open issues tagged "good first issue" or "help wanted"](https://github.com/open-quantum-safe/liboqs/issues).



