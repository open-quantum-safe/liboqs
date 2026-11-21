#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""
Measure the peak stack usage of liboqs KEM and signature implementations and
compare it with the `large-stack-usage` flags in docs/algorithms/**/*.yml.

The measurement itself is done by tests/stack_usage.c (stack painting); this
script builds liboqs in one or more configurations ("flavours"), runs it, works
out which YAML implementation each figure belongs to, and reports the flag the
measurement implies under a given threshold.

Intended to be run by maintainers when implementations are added or updated,
not as part of regular CI. See docs/STACK_USAGE.md.
"""

import argparse
import csv
import glob
import os
import platform
import re
import shutil
import subprocess
import sys

import yaml

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

FLAVOURS = {
    # name: (description, CMake arguments)
    'dist': ('portable binary with runtime CPU dispatch (OQS_DIST_BUILD=ON)',
             ['-DOQS_DIST_BUILD=ON']),
    'portable': ('portable C only (OQS_DIST_BUILD=OFF, OQS_OPT_TARGET=generic)',
                 ['-DOQS_DIST_BUILD=OFF', '-DOQS_OPT_TARGET=generic']),
    'memopt': ('memory-optimised implementations where available (OQS_MEMOPT_BUILD=ON)',
               ['-DOQS_DIST_BUILD=ON', '-DOQS_MEMOPT_BUILD=ON']),
}

# Families that are disabled by default but should be measured too.
DEFAULT_EXTRA_CMAKE_ARGS = ['-DOQS_ENABLE_KEM_CLASSIC_MCELIECE=ON']

# YAML required_flags -> names printed by stack_usage (OQS_CPU_EXT_*), per architecture.
X86_FLAGS = {'adx': 'ADX', 'aes': 'AES', 'avx': 'AVX', 'avx2': 'AVX2', 'avx512': 'AVX512',
             'bmi1': 'BMI1', 'bmi2': 'BMI2', 'pclmul': 'PCLMULQDQ', 'pclmulqdq': 'PCLMULQDQ',
             'popcnt': 'POPCNT', 'sse': 'SSE', 'sse2': 'SSE2', 'sse3': 'SSE3'}
ARM_FLAGS = {'aes': 'ARM_AES', 'sha2': 'ARM_SHA2', 'sha3': 'ARM_SHA3', 'neon': 'ARM_NEON'}
PORTABLE_ARCHS = {'64-bit little-endian'}


def host_arch():
    m = platform.machine().lower()
    if m in ('x86_64', 'amd64'):
        return 'x86_64'
    if m in ('aarch64', 'arm64'):
        return 'ARM64_V8'
    return m


def norm(name):
    return re.sub(r'[^a-z0-9]', '', name.lower())


# ---------------------------------------------------------------- YAML

def load_yaml_implementations():
    """norm(parameter-set name or alias) -> (family, name, [implementation dicts])"""
    out = {}
    for path in sorted(glob.glob(os.path.join(ROOT, 'docs', 'algorithms', '*', '*.yml'))):
        with open(path) as f:
            doc = yaml.safe_load(f)
        family = os.path.splitext(os.path.basename(path))[0]
        for ps in doc.get('parameter-sets', []) or []:
            entry = (family, ps['name'], ps.get('implementations', []) or [])
            out[norm(ps['name'])] = entry
            for alias in ps.get('alias', []) or []:
                out.setdefault(norm(alias), entry)
    return out


def platforms_of(impl):
    sp = impl.get('supported-platforms')
    if sp in (None, '', 'all'):
        return 'all'
    return sp


def is_portable(impl):
    if impl.get('upstream-id') == 'memopt':
        return False
    sp = platforms_of(impl)
    if sp == 'all':
        return True
    return all(p.get('architecture') in PORTABLE_ARCHS and not p.get('required_flags') for p in sp)


def platform_match(impl, arch, cpu):
    """Number of required CPU flags if impl is usable on arch/cpu, else None."""
    sp = platforms_of(impl)
    if sp == 'all':
        return None
    flagmap = X86_FLAGS if arch == 'x86_64' else ARM_FLAGS
    best = None
    for p in sp:
        if p.get('architecture') != arch:
            continue
        req = p.get('required_flags') or []
        if all(flagmap.get(r, r.upper()) in cpu for r in req):
            best = max(best or 0, len(req))
    return best


def measured_implementations(impls, flavour, arch, cpu):
    """Best guess at which YAML implementation(s) a figure from this build belongs to."""
    if flavour == 'memopt':
        memopt = [i for i in impls if i.get('upstream-id') == 'memopt']
        if memopt:
            return memopt
    if flavour != 'portable':
        scored = [(platform_match(i, arch, cpu), i) for i in impls if i.get('upstream-id') != 'memopt']
        scored = [(s, i) for s, i in scored if s is not None]
        if scored:
            top = max(s for s, _ in scored)
            return [i for s, i in scored if s == top]
    return [i for i in impls if is_portable(i)]


# ---------------------------------------------------------------- build & run

def flavour_of_cache(build_dir):
    cache = {}
    with open(os.path.join(build_dir, 'CMakeCache.txt')) as f:
        for line in f:
            m = re.match(r'^(OQS_DIST_BUILD|OQS_OPT_TARGET|OQS_MEMOPT_BUILD)(?::[A-Z]+)?=(.*)$', line.strip())
            if m:
                cache[m.group(1)] = m.group(2)
    on = lambda v: v.upper() in ('ON', '1', 'TRUE', 'YES')
    if on(cache.get('OQS_MEMOPT_BUILD', 'OFF')):
        return 'memopt'
    if not on(cache.get('OQS_DIST_BUILD', 'OFF')) and cache.get('OQS_OPT_TARGET', '') == 'generic':
        return 'portable'
    return 'dist'


def build(flavour, work_dir, extra_args, jobs):
    build_dir = os.path.join(work_dir, flavour)
    args = ['cmake', '-S', ROOT, '-B', build_dir, '-DCMAKE_BUILD_TYPE=Release']
    if shutil.which('ninja'):
        args += ['-GNinja']
    args += FLAVOURS[flavour][1] + extra_args
    print(f'[{flavour}] configuring in {build_dir}', file=sys.stderr)
    subprocess.run(args, check=True, stdout=subprocess.DEVNULL)
    print(f'[{flavour}] building stack_usage', file=sys.stderr)
    cmd = ['cmake', '--build', build_dir, '--target', 'stack_usage']
    if jobs:
        cmd += ['--parallel', str(jobs)]
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)
    return build_dir


def run(build_dir, alg_filter, stack_mib):
    exe = os.path.join(build_dir, 'tests', 'stack_usage')
    if not os.path.isfile(exe):
        sys.exit(f'{exe} not found (build the stack_usage target first)')
    cmd = [exe, '-s', str(stack_mib)] + ([alg_filter] if alg_filter else [])
    res = subprocess.run(cmd, stdout=subprocess.PIPE, universal_newlines=True)
    meta, rows = {}, []
    for line in res.stdout.splitlines():
        if line.startswith('#'):
            parts = line[1:].strip().split('\t')
            if len(parts) == 2:
                meta[parts[0]] = parts[1]
            continue
        parts = line.split('\t')
        if len(parts) == 5:
            rows.append({'kind': parts[0], 'alg': parts[1], 'op': parts[2],
                         'bytes': int(parts[3]), 'status': parts[4]})
    return meta, rows


# ---------------------------------------------------------------- report

def summarise(rows, flavour, meta, yaml_impls, threshold, arch):
    cpu = set(meta.get('cpu_extensions', '').split())
    per_alg = {}
    for r in rows:
        a = per_alg.setdefault((r['kind'], r['alg']), {'ops': {}, 'status': set()})
        a['ops'][r['op']] = r['bytes']
        a['status'].add(r['status'])
    out = []
    for (kind, alg), a in per_alg.items():
        peak_op, peak = max(a['ops'].items(), key=lambda t: t[1])
        status = 'ok' if a['status'] == {'ok'} else ','.join(sorted(a['status'] - {'ok'}))
        entry = yaml_impls.get(norm(alg))
        if entry is None and alg.endswith('-extmu'):
            # external-mu variants share the datasheet entry of their base parameter set
            entry = yaml_impls.get(norm(alg[:-len('-extmu')]))
        if entry is None:
            impl_ids, yaml_flag = '?', None
        else:
            impls = measured_implementations(entry[2], flavour, arch, cpu)
            impl_ids = '|'.join(dict.fromkeys(i.get('upstream-id', '?') for i in impls)) or '?'
            flags = {bool(i.get('large-stack-usage', False)) for i in impls}
            yaml_flag = flags.pop() if len(flags) == 1 else 'mixed'
        implied = peak > threshold
        if status != 'ok':
            verdict = 'n/a'
        elif yaml_flag is None or yaml_flag == 'mixed':
            verdict = 'unknown'
        else:
            verdict = 'ok' if yaml_flag == implied else 'MISMATCH'
        out.append({'flavour': flavour, 'kind': kind, 'alg': alg, 'family': entry[0] if entry else '',
                    'impl': impl_ids, 'peak_bytes': peak, 'peak_op': peak_op,
                    'yaml_flag': yaml_flag, 'implied_flag': implied, 'verdict': verdict,
                    'status': status})
    return out


def fmt_kib(b):
    return f'{b / 1024:.0f}'


def print_markdown(results, metas, threshold, only_mismatches):
    for flavour, meta in metas.items():
        print(f'- **{flavour}**: {FLAVOURS[flavour][0]}; liboqs {meta.get("liboqs", "?")}; '
              f'CPU extensions: {meta.get("cpu_extensions", "").strip() or "none"}; '
              f'default thread stack: {fmt_kib(int(meta.get("default_thread_stack_bytes", 0)))} KiB')
    print(f'\nThreshold: peak > {fmt_kib(threshold)} KiB implies `large-stack-usage: true`.\n')
    print('| Flavour | Algorithm | Implementation | Peak KiB | Peak op | YAML flag | Implied | Verdict |')
    print('|---|---|---|---:|---|---|---|---|')
    for r in sorted(results, key=lambda r: (r['flavour'], -r['peak_bytes'])):
        if only_mismatches and r['verdict'] in ('ok',):
            continue
        yf = r['yaml_flag'] if isinstance(r['yaml_flag'], str) else str(r['yaml_flag']).lower()
        verdict = r['verdict'] if r['status'] == 'ok' else f"{r['verdict']} ({r['status']})"
        print(f"| {r['flavour']} | {r['alg']} | {r['impl']} | {fmt_kib(r['peak_bytes'])} | {r['peak_op']} | "
              f"{yf} | {str(r['implied_flag']).lower()} | {verdict} |")
    n = {}
    for r in results:
        n[r['verdict']] = n.get(r['verdict'], 0) + 1
    print('\n' + ', '.join(f'{k}: {v}' for k, v in sorted(n.items())))


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('--flavour', action='append', choices=sorted(FLAVOURS),
                   help='build configuration to measure (repeatable; default: dist)')
    p.add_argument('--build-dir', action='append', default=[],
                   help='use an existing build directory instead of building (repeatable); '
                        'the flavour is derived from its CMakeCache.txt')
    p.add_argument('--work-dir', default=os.path.join(ROOT, 'build-stack-usage'),
                   help='where to create builds (default: %(default)s)')
    p.add_argument('--cmake-arg', action='append', default=[],
                   help='extra CMake argument for builds (repeatable); replaces the default '
                        + ' '.join(DEFAULT_EXTRA_CMAKE_ARGS))
    p.add_argument('--jobs', type=int, default=0, help='parallel build jobs')
    p.add_argument('--filter', default=None, help='only algorithms whose name contains this string')
    p.add_argument('--threshold-kib', type=int, default=512,
                   help='peak above which an implementation counts as large (default: %(default)s)')
    p.add_argument('--stack-mib', type=int, default=64,
                   help='size of the measurement stack (default: %(default)s)')
    p.add_argument('--mismatches-only', action='store_true', help='only list rows whose verdict is not ok')
    p.add_argument('--tsv', help='also write all results to this TSV file')
    p.add_argument('--check', action='store_true', help='exit non-zero if any MISMATCH is found')
    args = p.parse_args()

    if args.build_dir and args.flavour:
        p.error('use either --flavour (build) or --build-dir (existing build), not both')
    extra = args.cmake_arg or DEFAULT_EXTRA_CMAKE_ARGS
    threshold = args.threshold_kib * 1024
    arch = host_arch()
    yaml_impls = load_yaml_implementations()

    builds = [(flavour_of_cache(d), d) for d in args.build_dir]
    if not builds:
        builds = [(f, build(f, args.work_dir, extra, args.jobs)) for f in (args.flavour or ['dist'])]

    results, metas = [], {}
    for flavour, build_dir in builds:
        print(f'[{flavour}] measuring {build_dir}', file=sys.stderr)
        meta, rows = run(build_dir, args.filter, args.stack_mib)
        if not rows:
            sys.exit(f'no measurements from {build_dir}')
        metas[flavour] = meta
        results += summarise(rows, flavour, meta, yaml_impls, threshold, arch)

    print_markdown(results, metas, threshold, args.mismatches_only)
    if args.tsv:
        with open(args.tsv, 'w', newline='') as f:
            w = csv.DictWriter(f, fieldnames=list(results[0].keys()), delimiter='\t')
            w.writeheader()
            w.writerows(results)
    if args.check and any(r['verdict'] == 'MISMATCH' for r in results):
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
