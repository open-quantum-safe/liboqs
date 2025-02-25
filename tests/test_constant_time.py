# SPDX-License-Identifier: MIT

""" test_constant_time.py

The goal of this script is to ensure that every instance of secret-dependant
control flow in liboqs is documented. This script does not ensure that all of
the software in liboqs is constant time. Rather, it is intended to aid auditors
in their search for non-constant time behaviour.

WARNING: This script currently runs test_kem and test_sig on random seeds.
It is not coverage guided. It will miss instances of non-constant time
behaviour in code paths that are rarely executed.

This script requires Valgrind version >= 3.14.0, and it only gives meaningful
results if test_kem and test_sig have been compiled with CMAKE_BUILD_TYPE=Debug
and OQS_ENABLE_TEST_CONSTANT_TIME.


How this script works
---------------------
This script runs test_kem (and/or test_sig) through Valgrind's Memcheck tool.
Valgrind executes the test program and issues an error message if/whenever the
program's control flow depends on uninitialized data. As observed by Adam
Langley [1], if we tell Valgrind that secrets are uninitialized, then Valgrind
will tell us about secret-dependant control flow.

Assuming that each scheme in liboqs passes test_kat, our test_kem and test_sig
programs are structured such that all secret data can be traced back to a call
to OQS_randombytes. The tests intercept calls to OQS_randombytes and tell
Valgrind that every random byte is uninitialized. Hence, Valgrind will issue an
error if (but not only if!) our tests branch on secret data.

Since there may be false positives, we say that Valgrind identifies "suspected
non-constant time behaviour".

Again, the purpose of this script is to ensure that suspected non-constant time
behaviour is documented. This script ships with a collection of Valgrind
"suppression files". Each suppression file documents one or more instances of
suspected non-constant time behaviour in liboqs.

The suppression files are also used to silence errors from Valgrind. If this
script runs without error, then all of the suspected non-constant time behaviour
in liboqs has been documented. If this script fails, then a new suppression
file should be written.


How to write suppression files
------------------------------
Valgrind will output a suppression file template along with its error message.
It's your job to copy this template to the correct location, edit it, and tell
this script about the new file.

Suppression files for KEMs are stored in
    liboqs/tests/constant_time/kem/{passes,issues}/.
Suppression files for signature schemes are stored in
    liboqs/tests/constant_time/sig/{passes,issues}/.

This script does not differentiate between the passes and issues
subdirectories.  The label is for auditors. We "give a pass" to an error that
is known not to be a security threat, and we store the corresponding
suppression file in the "passes" subdirectory. We "raise an issue" about any
other error, and we store the corresponding suppression file in the "issues"
subdirectory.

If you are unsure where your suppression file belongs, then save it to the
"issues" subdirectory.

Once you've written a suppression file, give it a descriptive file name and
tell this script about it. There are json files called passes.json and
issues.json in
    liboqs/tests/constant_time/{kem,sig}/
These json files contain dictionaries of the form
    { "Scheme name" : ["list", "of", "suppression", "files"], ... }
Add the name of your suppression file to the appropriate list to suppress
the errors that you have documented.


How to write a good suppression file
------------------------------------
Here is an example of a suppression file:
    {
       Rejection sampling to produce public "A" matrix
       Memcheck:Cond
       fun:rej_uniform
       fun:PQCLEAN_KYBER*_CLEAN_gen_matrix
    }

The brackets wrap a single error that is to be suppressed. Within the brackets,
the first line is a comment. The remaining lines tell Valgrind to ignore
any "Memcheck:Cond" errors that occur when a function named rej_uniform is
called from a function whose name matches the glob pattern
PQCLEAN_KYBER*_CLEAN_gen_matrix.

Before this suppression file was written, a run of this script produced the
following output.

    ==594== Conditional jump or move depends on uninitialised value(s)
    ==594==    at 0x22550D: rej_uniform (indcpa.c:133)
    ==594==    by 0x225654: PQCLEAN_KYBER512_CLEAN_gen_matrix (indcpa.c:177)
    ==594==    by 0x2257D1: PQCLEAN_KYBER512_CLEAN_indcpa_keypair (indcpa.c:216)
    ==594==    by 0x1B6C1E: PQCLEAN_KYBER512_CLEAN_crypto_kem_keypair (kem.c:26)
    ==594==    by 0x1B6B9F: OQS_KEM_kyber_512_keypair (kem_kyber_512.c:56)
    ==594==    by 0x10D123: OQS_KEM_keypair (kem.c:818)
    ==594==    by 0x10AD07: kem_test_correctness (test_kem.c:103)
    ==594==    by 0x10B4E7: test_wrapper (test_kem.c:186)
    ==594==    by 0x4CDAFA2: start_thread (pthread_create.c:486)
    ==594==    by 0x4DED4CE: clone (clone.S:95)
    ==594==
    {
       <insert_a_suppression_name_here>
       Memcheck:Cond
       fun:rej_uniform
       fun:PQCLEAN_KYBER512_CLEAN_gen_matrix
       fun:PQCLEAN_KYBER512_CLEAN_indcpa_keypair
       fun:PQCLEAN_KYBER512_CLEAN_crypto_kem_keypair
       fun:OQS_KEM_kyber_512_keypair
       fun:OQS_KEM_keypair
       fun:kem_test_correctness
       fun:test_wrapper
       fun:start_thread
       fun:clone
    }

The lines beginning with "==" are a Valgrind error message. The bracketed text
is a suppression file template. To produce the final suppression file we
added a comment, replaced "512" with a wildcard (since an identical error occurs
in other Kyber parameter sets), and truncated the backtrace (since the extra lines
provide no interesting information to auditors).

The "fun:rej_uniform" line says to ignore _all_ Memcheck:Cond errors in
rej_uniform, but Valgrind told us that line 133 was the problem. Any
"fun:name" line in the backtrace can be replaced by an equivalent
"src:file:line", so we could have narrowed the scope of our suppression:
    {
       Rejection sampling to produce public "A" matrix
       Memcheck:Cond
       src:indcpa.c:133 # fun:rej_uniform
       fun:PQCLEAN_KYBER*_CLEAN_gen_matrix
    }
Here "# fun:rej_uniform" is a comment. An update to the Kyber source code might
break our suppression file by changing the line number, and leaving the function
name as a comment might help a future reviewer.

An ellipsis (...) can serve as a wildcard for a portion of the backtrace.
We could have written:
    {
       Rejection sampling to produce public "A" matrix
       Memcheck:Cond
       ...
       fun:PQCLEAN_KYBER*_CLEAN_gen_matrix
    }
But this is perhaps too concise. Remember that the goal here is to help auditors.

Further information can be found in Valgrind's manual. See
    https://www.valgrind.org/docs/manual/manual-core.html#manual-core.suppress
and
    https://www.valgrind.org/docs/manual/mc-manual.html#mc-manual.suppfiles

Credits
-------
The observation that Valgrind can be used to identify non-constant time
behaviour is due to Adam Langley [1, 2]. Mortiz Neikes' TIMECOP project applies
Langley's idea to the SUPERCOP benchmarking suite [3]. Versions of SUPERCOP
starting with 20200816 include TIMECOP and apply Langley's idea to randombytes
calls in particular [4]. We have borrowed the idea of instrumenting randombytes
calls from SUPERCOP.

[1] https://github.com/agl/ctgrind
[2] https://boringssl.googlesource.com/boringssl/+/a6a049a6fb51a052347611d41583a0622bc89d60
[2] https://post-apocalyptic-crypto.org/timecop/index.html
[3] http://bench.cr.yp.to/tips.html#timecop
"""


import helpers
import json
import os
import pytest
import sys
import re

REQ_LIBOQS_BUILD_OPTS = ['OQS_ENABLE_TEST_CONSTANT_TIME',
                         'OQS_DEBUG_BUILD']

# Error suppression based on file and line number was introduced in
# Valgrind 3.14.0 (9 October 2018).
# https://www.valgrind.org/docs/manual/dist.news.html
MIN_VALGRIND_VERSION = [3, 14, 0]

VALGRIND = ['valgrind',
            # '-v', # Turn on -v to see which suppression files are used
            '--tool=memcheck',
            '--gen-suppressions=all',
            '--error-exitcode=1',
            '--max-stackframe=20480000',
            '--num-callers=20',
            ]

# The following two functions read the json files
#   liboqs/tests/constant_time/{kem,sig}/{passes,issues}.json
# into python dictionaries `ct_passes' and `ct_issues', which
# are of the form
# { 'kem' : { 'Kem Name'   : ['list', 'of', 'filenames'], ... },
#   'sig' : {   'Sig Name' : ['list', 'of', 'filenames'], ... }
# }

ct_passes = {'kem': None, 'sig': None}
ct_issues = {'kem': None, 'sig': None}

def get_ct_passes(t, name):
    ct_t = os.path.join('tests', 'constant_time', t)
    if ct_passes[t] is None:
        with open(os.path.join(ct_t, 'passes.json'), 'r') as fp:
            ct_passes[t] = json.load(fp)
    passes = ct_passes[t].get(name,[])
    return [os.path.join(ct_t, 'passes', f) for f in passes]

def get_ct_issues(t, name):
    ct_t = os.path.join('tests', 'constant_time', t)
    if ct_issues[t] is None:
        with open(os.path.join(ct_t, 'issues.json'), 'r') as fp:
            ct_issues[t] = json.load(fp)
    issues = ct_issues[t].get(name,[])
    return [os.path.join(ct_t, 'issues', f) for f in issues]


@helpers.filtered_test
@helpers.test_requires_build_options(*REQ_LIBOQS_BUILD_OPTS)
@helpers.test_requires_valgrind_version_at_least(*MIN_VALGRIND_VERSION)
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_constant_time_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if ('SKIP_ALGS' in os.environ) and len(os.environ['SKIP_ALGS'])>0:
        for algexp in os.environ['SKIP_ALGS'].split(','):
            if len(re.findall(algexp, kem_name))>0:
               pytest.skip("Test disabled by alg filter")
    passes = get_ct_passes('kem', kem_name)
    issues = get_ct_issues('kem', kem_name)
    output = helpers.run_subprocess(
             VALGRIND + [
                *(['--suppressions='+f for f in passes]),
                *(['--suppressions='+f for f in issues]),
                helpers.path_to_executable('test_kem'),
                kem_name
             ]
    )

@helpers.filtered_test
@helpers.test_requires_build_options(*REQ_LIBOQS_BUILD_OPTS)
@helpers.test_requires_valgrind_version_at_least(*MIN_VALGRIND_VERSION)
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_constant_time_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    if ('SKIP_ALGS' in os.environ) and len(os.environ['SKIP_ALGS'])>0:
        for algexp in os.environ['SKIP_ALGS'].split(','):
            if len(re.findall(algexp, sig_name))>0:
               pytest.skip("Test disabled by alg filter")
    passes = get_ct_passes('sig', sig_name)
    issues = get_ct_issues('sig', sig_name)
    output = helpers.run_subprocess(
             VALGRIND + [
                *(['--suppressions='+f for f in passes]),
                *(['--suppressions='+f for f in issues]),
                helpers.path_to_executable('test_sig'),
                sig_name
             ]
    )

if __name__ == '__main__':
    pytest.main(sys.argv)

# Unused/obsolete suppressions are a burden on reviewers. You can find out which suppressions
# are used by passing the -v flag to valgrind. To find unused suppressions we have to extract
# a list of available suppressions first. You can use awk to find lines that contain only a '{'.
# Increment these line numbers by 1 to match the output of valgrind -v, then compare against
# the used suppressions.
#
# awk '$0 ~ /^{$/{print FILENAME ":" NR+1}' suppression files > /tmp/available_suppressions
# valgrind -v --suppressions=[...] ./build/tests/test_kem KEM_NAME 2>&1 \
#   | grep used_suppression \
#   | awk '{ print $NF }' > /tmp/used_suppressions
# cat /tmp/used_suppressions /tmp/available_suppressions | sort | uniq -u
