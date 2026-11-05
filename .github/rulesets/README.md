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

## What changes in practice

The configured rule today says two approvals, but the effective rule depends
on who is merging. The table compares the effective behaviour now with the
proposal.

| Action on `main` | Now, repo admin | Now, write access (non-admin) | Proposed, everyone |
| --- | --- | --- | --- |
| Push a commit directly | allowed, logged as a bypass | blocked | blocked |
| Merge a PR with 0 approvals | allowed, logged as a bypass | blocked | blocked |
| Merge a PR with 1 approval | allowed, logged as a bypass | blocked | allowed |
| Merge a PR with 2 approvals | allowed | allowed | allowed |
| Merge your own PR | allowed with any approval count | needs 2 approvals from others | needs 1 approval from someone else |
| Force push or delete the branch | blocked | blocked | blocked |
| Merge commit (non-linear history) | blocked | blocked | blocked |

So the proposal is a tightening for admins and a loosening for everyone
else with write access (the `liboqs-committers` team), who today need two
approvals and would need one.

An approval counts only from a user with write access who is not the PR
author. Because stale approvals are dismissed on push, the approval must be
on the final commit. The minimum to land a change becomes two people: an
author and one approver, with nobody able to go around it.

### Measured against the last year

Of the 146 PRs merged into `main` between 2025-11-01 and 2026-11-01:

| Approvals at merge | PRs | Under the proposal |
| --- | --- | --- |
| 0 | 26 | blocked until someone approves |
| 1 | 62 | merge as before |
| 2 or more | 58 | merge as before |

The 26 zero-approval merges break down as 13 Dependabot PRs, 9 PRs the
author merged themselves (dstebila 3, xuganyu96 6) and 4 contributor PRs
merged by an admin without a review on record. All 26 were merged by
xuganyu96 (21) or dstebila (5). The 62 one-approval merges are the ones the
two-approval rule blocked on paper and admins bypassed; they are the reason
the proposal sets the count to one.

In other words, about one PR in six would have needed one more action, a
second maintainer clicking approve, and nothing else about the year's
history would have changed.

## Risks and how to handle them

- **Dependabot PRs need a human approver.** Thirteen of last year's
  zero-approval merges were Dependabot. Under the proposal the maintainer who
  merges them cannot also be the only reviewer; a second maintainer approves,
  or the merger approves and a second one merges. Dependabot already groups
  updates, which keeps the count low. An auto-approve workflow using the
  Actions token would satisfy the rule mechanically and defeat its purpose,
  so it is not recommended.

- **No emergency path.** With an empty bypass list a broken `main` or an
  embargoed security fix still needs an author and an approver. If the
  project wants an escape hatch, add the `oqs-admins` team as a bypass actor
  with `bypass_mode: "pull_request"`. That lets those admins merge a PR
  without approval but still blocks direct pushes, and every use is logged
  as a bypass. The recommendation is to start with the list empty and add
  this only if it turns out to be needed.

- **Lockout is not possible, but disabling is visible.** Rulesets govern git
  operations, not settings. Any admin can still edit or disable the ruleset
  at Settings > Rules > Rulesets. Doing so is recorded in the org audit log
  as a `repository_ruleset` event, so turning it off is a reviewable act
  rather than a silent bypass.

- **Overlap with the classic rule.** While both the classic rule and the
  ruleset exist, GitHub applies both and the stricter one wins. Non-admins
  keep needing two approvals until the classic rule is deleted; admins drop
  to one with no bypass immediately. Nothing locks anyone out during the
  overlap, but the classic rule should be deleted promptly so the written
  rule matches the effective one.

- **Approver who pushes to the PR.** With last-push approval off, a reviewer
  can push a commit to someone else's PR and approve it themselves. That
  still involves two accounts (author and reviewer) and matches today's
  behaviour. Setting `require_last_push_approval` to true closes it at the
  cost of one more approval whenever a maintainer pushes a fix-up to a
  contributor branch.

- **CI is not enforced.** Neither the classic rule nor the proposal requires
  status checks, so a PR with failing CI can still be merged, as it can
  today. Adding a `required_status_checks` rule is a separate decision; the
  checks to name should be the ones every PR runs, not the matrix jobs.

- **Author approvals never count.** Self-merge with no second person goes
  from "admins can" to "nobody can". Nine merges last year were of this
  kind, all small maintainer PRs. This is the behaviour the issue asks for.

## Rollout

The org is on the Enterprise plan, so rulesets support `evaluate` mode and
rule insights.

1. Import `main-proposed.json` with `enforcement` set to `evaluate` and
   leave it for a week or two. Settings > Rules > Insights then lists every
   push and merge the ruleset would have blocked, without blocking anything.
2. Review the insights. If the only entries are the expected ones (merges
   with no approval, direct pushes), switch the ruleset to `active`.
3. Delete classic rule 925128 so there is one source of truth.
4. `gh-pages` can keep its classic rule or move to `gh-pages-classic.json`;
   the two are equivalent.

To re-run the approval count above for a later period, list merged PRs with
their reviews through GraphQL and count, per PR, the reviewers other than
the author whose latest review is `APPROVED`.
