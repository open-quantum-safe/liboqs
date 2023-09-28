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

To check adherence of any new code to this, it therefore is highly recommended to
run the following command in the project main directory prior to finishing a PR:

    find src tests -name '*.[ch]' | grep -v '/external/' | grep -v 'kem/.*/.*/.*' | grep -v 'sig/.*/.*/.*' | xargs astyle --dry-run --options=.astylerc | grep Format

### Running CI locally

#### CircleCI

If encountering CI errors in CircleCI, it may be helpful to execute the test jobs
locally to debug. This can be facilitated by executing the command

   circleci local execute --job some-test-job

assuming "some-test-job" is the name of the test to be executed and the CircleCI
[command line tools have been installed](https://circleci.com/docs/local-cli).

#### Github CI

[Act](https://github.com/nektos/act) is a tool facilitating local execution of
github CI jobs. When executed in the main `oqsprovider` directory, 

    act -l Displays all github CI jobs
    act -j some-job Executes "some-job"

When installing `act` as a github extension, prefix the commands with `gh `.

### New features

Any PR introducing a new feature is expected to contain a test of this feature
and this test should be part of the CI pipeline, preferably using Github CI.

## Failsafe

If you feel your contribution is not getting proper attention, please be sure to
add a tag to one or more of our [most active contributors](https://github.com/open-quantum-safe/liboqs/graphs/contributors).

## Issues to start working on

If you feel like contributing but don't know what specific topic to work on,
please check the [open issues tagged "good first issue" or "help wanted"](https://github.com/open-quantum-safe/liboqs/issues).



