# Additional procedures for code maintenance 

## Managing pinned dependencies

The OpenSSF, via the [scorecard](https://securityscorecards.dev/) project recommends that projects pin any
dependencies they use:

* to ensure reproducibility
* to reduce the risk for rogue dependency updates to compromise software

It's important to note that this requires any changes to dependencies are properly reviewed, and
these changes, by design, should not be automatic in themselves, though automated tools may provide recommendations.

### Python dependencies

Python dependencies used in the build process such as within `.github/workflows` should be pinned to a specific version to ensure reproducibility.

This is achieved by:

* Ensuring the required hash is in the `requirements.txt`.
* Using the `--require-hashes` option on any `pip install` command line which causes pip to require hashes for all dependencies.

To add a new, or changed dependency:

* Ensure the `pip-compile` tool is installed via the [pip-tools](https://pypi.org/project/pip-tools/) package.
* Update `requirements.in` with added, modified, or deleted dependencies. 
* Update requirements.txt using `pip-compile --generate-hashes --output-file=requirements.txt requirements.in`.
* Verify correct functionality.
* Check in both `requirements.txt` and `requirements.in`.

 Note: `requirements.in` acts purely as a template in this process. It is not used during the installation of a dependency.

### Github Actions

All actions used in `.github/workflows` should pin the exact version of the action they are using, for
example a step such as:

```yaml
- name: Checkout code
  uses: actions/checkout@0ad4b8fadaa221de15dcec353f45205ec38ea70b # pin@v4
```

The exact hash specified after `@` is the git commit hash within the repo where the action is found.

The [pin github action](https://github.com/mheap/pin-github-action) tool can be used to maintain these
by, for example, running:

```shell
pin-github-action unix.yml
```

This will add the appropriate hash if not present, along with a comment, and also update each hash in accordance with any existing comment.

For major updates, update the comment ie `pin@v4` to `pin@v5` and the tool will attempt to find the new hash.

The comment should not be removed, and should exclusively be used for updating the version.

A full explanation of how the tool operates can be found in the [documentation](https://github.com/mheap/pin-github-action).

To help in explanation here's an example of a similar code fragment between tool executions:

* Original entry is `uses: actions/checkout@v3`
* run `pin-github-action unix.yml`
* We now see `uses: actions/checkout@f43a0e5ff2bd294095638e18286ca9a3d1956744 # pin@v3`
* later we want to go to v4, so update the text to `uses: actions/checkout@f43a0e5ff2bd294095638e18286ca9a3d1956744 # pin@v4`
* Now run `pin-github-action unix.yml` to correct the sha
* File now shows `uses: actions/checkout@692973e3d937129bcbf40652eb9f2f61becf3332 # pin@v4`

When changes have been made, correct functionality of the Github actions should be verified by reviewing the Github action logs and outputs. The SHA inserted by the tool can be searched for in Github to check it is associated with the expected version.
