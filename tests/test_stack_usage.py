# SPDX-License-Identifier: MIT

# Smoke tests for the stack usage tooling (tests/stack_usage.c and
# scripts/measure_stack_usage.py). They exercise only the first enabled KEM and
# signature algorithm (honouring --algs/OQS_TEST_ALGS and SKIP_ALGS); a full
# measurement is a manual step, see docs/STACK_USAGE.md.

import os
import re
import sys

import helpers
import pytest

pytestmark = pytest.mark.skipif(sys.platform.startswith('win'),
                                reason='stack_usage needs fork, mmap and pthreads')

KINDS = {
    'kem': (helpers.available_kems_by_name, helpers.is_kem_enabled_by_name, ['keypair', 'encaps', 'decaps']),
    'sig': (helpers.available_sigs_by_name, helpers.is_sig_enabled_by_name, ['keypair', 'sign', 'verify']),
}


def pick_alg(kind, config):
    """First enabled algorithm of this kind, in header order, that passes the test filters."""
    available, is_enabled, _ = KINDS[kind]
    requested = getattr(config, 'oqs_algs', None) or set()
    skip = [e for e in os.environ.get('SKIP_ALGS', '').split(',') if e]
    for name in available():
        if requested and name not in requested:
            continue
        if any(re.search(e, name) for e in skip):
            continue
        if is_enabled(name):
            return name
    return None


def stack_usage_executable():
    path = os.path.join(helpers.get_current_build_dir_name(), 'tests', 'stack_usage')
    if not os.path.isfile(path):
        pytest.skip('stack_usage not built on this platform/configuration')
    return path


def parse(output):
    meta, rows = {}, []
    for line in output.splitlines():
        if line.startswith('#'):
            parts = line[1:].strip().split('\t')
            if len(parts) == 2:
                meta[parts[0]] = parts[1]
        elif line:
            parts = line.split('\t')
            assert len(parts) == 5, 'malformed line: {}'.format(line)
            rows.append(parts)
    assert rows, 'no measurements in output:\n' + output
    return meta, rows


@pytest.mark.parametrize('kind', sorted(KINDS))
def test_stack_usage_harness(kind, request):
    exe = stack_usage_executable()
    alg = pick_alg(kind, request.config)
    if alg is None:
        pytest.skip('no {} algorithm enabled/selected'.format(kind))
    meta, rows = parse(helpers.run_subprocess([exe, alg]))
    stack_bytes = int(meta['measurement_stack_bytes'])
    rows = [r for r in rows if r[1] == alg]
    assert [r[2] for r in rows] == KINDS[kind][2]
    for _, _, op, peak, status in rows:
        assert status == 'ok', '{} {} failed'.format(alg, op)
        assert 0 < int(peak) < stack_bytes, '{} {}: implausible peak {}'.format(alg, op, peak)


def test_measure_stack_usage_script(request):
    pytest.importorskip('yaml')  # the driver script reads docs/algorithms/**/*.yml
    stack_usage_executable()
    alg = pick_alg('kem', request.config) or pick_alg('sig', request.config)
    if alg is None:
        pytest.skip('no algorithm enabled/selected')
    out = helpers.run_subprocess([sys.executable, os.path.join('scripts', 'measure_stack_usage.py'),
                                  '--build-dir', helpers.get_current_build_dir_name(), '--filter', alg])
    assert '| ' + alg + ' |' in out
    assert 'unknown' not in out and 'n/a' not in out, out
