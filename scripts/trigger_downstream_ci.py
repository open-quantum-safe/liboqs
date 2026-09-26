#!/usr/bin/env python3
# SPDX-License-Identifier: MIT

"""Trigger a downstream OQS project's CI and wait for its result.

Sends a repository_dispatch or workflow_dispatch event through the GitHub
REST API, locates the workflow run(s) spawned by that event, and polls them
until every run reaches the terminal status "completed" or a timeout
expires. A single repository_dispatch may start several workflows (e.g.
oqs-boringssl runs both shared.yml and static.yml), so all spawned runs are
checked.

Uses only the Python standard library. Runs in CI and locally; point
--api-base at a mock server for offline testing (see the PR's test harness).

Exit codes: 0 = all downstream runs succeeded, 1 = dispatch failure, no run
spawned, run failure, or timeout, 2 = usage error.
"""

import argparse
import functools
import json
import os
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from datetime import datetime, timedelta, timezone

TERMINAL_STATUS = "completed"
# "neutral" and "skipped" do not indicate downstream breakage; everything
# else (failure, timed_out, cancelled, startup_failure, stale,
# action_required) is treated as a failure so that no false success is
# reported (the root cause of issue #2474).
SUCCESS_CONCLUSIONS = {"success", "neutral", "skipped"}


@functools.lru_cache(maxsize=None)
def _opener(host):
    """Never route loopback traffic (local mock-server testing) through
    system or environment proxies; a proxy cannot reach the tester's
    localhost. Real GitHub API traffic keeps normal proxy handling."""
    if host in ("127.0.0.1", "::1", "localhost"):
        return urllib.request.build_opener(urllib.request.ProxyHandler({}))
    return urllib.request.build_opener()


def _request(method, url, token, data=None):
    headers = {
        "Accept": "application/vnd.github+json",
        "Authorization": f"Bearer {token}",
        "X-GitHub-Api-Version": "2022-11-28",
        "User-Agent": "oqs-downstream-trigger",
    }
    body = json.dumps(data).encode() if data is not None else None
    req = urllib.request.Request(url, data=body, headers=headers, method=method)
    opener = _opener(urllib.parse.urlparse(url).hostname or "")
    try:
        with opener.open(req, timeout=60) as resp:
            raw = resp.read()
            return resp.status, json.loads(raw) if raw else {}, dict(resp.headers)
    except urllib.error.HTTPError as e:
        raw = e.read()
        try:
            parsed = json.loads(raw) if raw else {}
        except json.JSONDecodeError:
            parsed = {"raw": raw.decode(errors="replace")}
        return e.code, parsed, dict(e.headers)
    except urllib.error.URLError as e:
        print(f"::warning::network error contacting {url}: {e}")
        return 0, {}, {}


def _check_rate_limit(headers):
    remaining = headers.get("X-RateLimit-Remaining")
    if remaining is not None and remaining.isdigit() and int(remaining) == 0:
        print("::warning::GitHub API rate limit exhausted; resets at epoch "
              f"{headers.get('X-RateLimit-Reset')}")


def _step_summary(line):
    path = os.environ.get("GITHUB_STEP_SUMMARY")
    if path:
        with open(path, "a", encoding="utf-8") as fh:
            fh.write(line + "\n")


def send_dispatch(args):
    if args.trigger_type == "repository_dispatch":
        url = f"{args.api_base}/repos/{args.repo}/dispatches"
        payload = {"event_type": args.event_type}
    else:
        url = (f"{args.api_base}/repos/{args.repo}"
               f"/actions/workflows/{args.workflow}/dispatches")
        payload = {"ref": args.ref}
    status, body, headers = _request("POST", url, args.token, payload)
    _check_rate_limit(headers)
    if status != 204:
        print(f"::error::dispatch to {args.repo} failed: "
              f"HTTP {status} -> {body}")
        return False
    print(f"{args.repo}: dispatch accepted (HTTP 204, {args.trigger_type})")
    return True


def find_runs(args, since):
    """Return {run_id: html_url} for runs spawned by our dispatch."""
    created = ">=" + since.strftime("%Y-%m-%dT%H:%M:%SZ")
    url = (f"{args.api_base}/repos/{args.repo}/actions/runs"
           f"?event={args.trigger_type}"
           f"&created={urllib.parse.quote(created)}&per_page=30")

    def fetch():
        status, body, headers = _request("GET", url, args.token)
        _check_rate_limit(headers)
        if status != 200:
            print(f"{args.repo}: run lookup got HTTP {status}")
            return {}
        runs = body.get("workflow_runs", [])
        if args.workflow:
            runs = [r for r in runs
                    if r.get("path", "").endswith(args.workflow)]
        return {r["id"]: r["html_url"] for r in runs}

    for attempt in range(1, args.find_attempts + 1):
        found = fetch()
        if found:
            # One dispatch can spawn several workflows whose runs appear a
            # few seconds apart; re-list once after a grace period so
            # siblings are not missed.
            time.sleep(args.find_backoff)
            found.update(fetch())
            for run_id, run_url in sorted(found.items()):
                print(f"{args.repo}: found run {run_id} -> {run_url}")
            return found
        time.sleep(args.find_backoff * attempt)
    print(f"::error::{args.repo}: dispatch was accepted but no workflow run "
          f"appeared after {args.find_attempts} attempts")
    return {}


def poll_runs(args, runs):
    """Poll every run until terminal or deadline; return {run_id: conclusion}."""
    deadline = time.monotonic() + args.timeout
    conclusions = {}
    while len(conclusions) < len(runs) and time.monotonic() < deadline:
        for run_id in runs:
            if run_id in conclusions:
                continue
            status, body, headers = _request(
                "GET", f"{args.api_base}/repos/{args.repo}/actions/runs/{run_id}",
                args.token)
            _check_rate_limit(headers)
            if status != 200:
                print(f"{args.repo}: poll of run {run_id} got HTTP {status}")
                continue
            run_status = body.get("status")
            conclusion = body.get("conclusion")
            print(f"{args.repo}: run {run_id} status={run_status} "
                  f"conclusion={conclusion}")
            if run_status == TERMINAL_STATUS:
                conclusions[run_id] = conclusion
        if len(conclusions) < len(runs):
            time.sleep(args.poll_interval)
    return conclusions


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--repo", required=True, help="owner/repo")
    parser.add_argument("--trigger-type", required=True,
                        choices=["repository_dispatch", "workflow_dispatch"])
    parser.add_argument("--event-type", default="liboqs-upstream-trigger",
                        help="repository_dispatch event type")
    parser.add_argument("--workflow", default=None,
                        help="workflow file name for workflow_dispatch "
                             "(e.g. ubuntu.yaml)")
    parser.add_argument("--ref", default="main",
                        help="git ref for workflow_dispatch")
    parser.add_argument("--timeout", type=int, default=2700,
                        help="seconds to wait for the run(s) to complete")
    parser.add_argument("--poll-interval", type=int, default=20)
    parser.add_argument("--find-attempts", type=int, default=12,
                        help="attempts to locate the spawned run(s)")
    parser.add_argument("--find-backoff", type=int, default=5,
                        help="base seconds between locate attempts "
                             "(multiplied by the attempt number)")
    parser.add_argument("--api-base",
                        default=os.environ.get("GITHUB_API_BASE",
                                               "https://api.github.com"))
    parser.add_argument("--token",
                        default=os.environ.get("OQSBOT_GITHUB_ACTIONS"))
    args = parser.parse_args()

    if not args.token:
        print("::error::no token (set OQSBOT_GITHUB_ACTIONS or pass --token)")
        return 2
    if args.workflow == "":
        args.workflow = None
    if args.trigger_type == "workflow_dispatch" and not args.workflow:
        print("::error::--workflow is required for workflow_dispatch")
        return 2

    since = datetime.now(timezone.utc) - timedelta(seconds=30)
    if not send_dispatch(args):
        _step_summary(f"- :x: `{args.repo}`: dispatch failed")
        return 1

    runs = find_runs(args, since)
    if not runs:
        _step_summary(f"- :x: `{args.repo}`: dispatch accepted but no "
                      f"workflow run appeared")
        return 1

    conclusions = poll_runs(args, runs)
    failed = []
    for run_id, run_url in sorted(runs.items()):
        conclusion = conclusions.get(run_id)
        if conclusion is None:
            failed.append((run_id, "timeout", run_url))
        elif conclusion not in SUCCESS_CONCLUSIONS:
            failed.append((run_id, conclusion, run_url))

    if not failed:
        print(f"::notice::{args.repo}: all {len(runs)} downstream run(s) "
              f"succeeded")
        _step_summary(f"- :white_check_mark: `{args.repo}`: "
                      f"{len(runs)} run(s) succeeded")
        return 0
    for run_id, reason, run_url in failed:
        print(f"::error::{args.repo}: run {run_id} -> {reason} ({run_url})")
        _step_summary(f"- :x: `{args.repo}`: run {run_id} {reason} — "
                      f"{run_url}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
