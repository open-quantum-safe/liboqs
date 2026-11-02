# Repository rulesets

GitHub does not read these files automatically. They are the JSON form of the
repository's branch protection, kept here so that changes to it can be reviewed
in a pull request and applied by import. Import a file in the web UI at
Settings > Rules > Rulesets > New ruleset > Import a ruleset, or with the API:

```sh
gh api -X POST repos/open-quantum-safe/liboqs/rulesets --input .github/rulesets/<file>.json
```

Settings exported from the UI carry extra fields (`id`, `source`,
`source_type`, timestamps). They are not needed for import and are left out
here.

## Current state (classic branch protection, read 2026-11-01)

Three files reproduce the two classic rules exactly as they stand. Classic
protection splits its settings into two groups: the review, status check and
linear history settings are bypassable by admins when "Do not allow bypassing
the above settings" is unchecked, which it is; the force push and deletion
blocks apply to everyone including admins. A ruleset has a single bypass list
for all of its rules, so the `main` rule needs two rulesets to say the same
thing.

| File | Classic rule | Rules | Bypass |
| --- | --- | --- | --- |
| `main-classic-pr-and-history.json` | 925128 (`main`) | pull request with 2 approvals, dismiss stale approvals, linear history | repository admins, always |
| `main-classic-no-force-push-no-delete.json` | 925128 (`main`) | block force pushes, block deletion | none |
| `gh-pages-classic.json` | 63918842 (`gh-pages`) | block force pushes, block deletion | none |

The classic `main` rule has no required status checks, no code owner review,
no signed commit requirement and no push restrictions, so none appear here.
Importing these three files changes nothing about who can do what; they
exist as a baseline to diff the proposal against.

## Proposal for #2662

`main-proposed.json` is the ruleset discussed in issue #2662. It follows the
agreement in that thread between dstebila and baentsch: one required
approval instead of two, and no admin bypass.

- Targets the default branch (`~DEFAULT_BRANCH`) rather than the literal name
  `main`.
- Blocks deletion and force pushes.
- Requires linear history, matching the repository's merge settings (merge
  commits are already disabled; squash and rebase are allowed).
- Requires a pull request with 1 approving review, stale approvals dismissed
  on push.
- Empty bypass list. Nobody, admin or not, can push to `main` directly or
  merge below the approval threshold. The teams that currently hold admin and
  so currently bypass the rule are `oqs-admins`, `liboqs-maintainers` and
  `bots`. No workflow in this repository pushes to `main`, so the `bots` team
  loses nothing in practice.

Settings deliberately left at their current values, each a one-line change if
wanted later: code owner review off, last-push approval off, conversation
resolution not required, no required status checks.

Rollout: import `main-proposed.json` with enforcement `active`, confirm the
next few PRs merge as expected, then delete classic rule 925128 so there is
one source of truth. `gh-pages` can keep its classic rule or move to
`gh-pages-classic.json`; the two are equivalent.
