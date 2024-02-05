# SPDX-License-Identifier: MIT

import functools
import os
import os.path
import pytest
import re
import subprocess
import sys
import json

kats = {}
kats["kem"] = None
kats["sig"] = None

def run_subprocess(command, working_dir='.', env=None, expected_returncode=0, input=None, ignore_returncode=False):
    """
    Helper function to run a shell command and report success/failure
    depending on the exit status of the shell command.
    """
    env_ = os.environ.copy()
    if env is not None:
        env_.update(env)
    env = env_

    # Note we need to capture stdout/stderr from the subprocess,
    # then print it, which pytest will then capture and
    # buffer appropriately
    print(working_dir + " > " + " ".join(command))

    result = subprocess.run(
            command,
            input=input,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            cwd=working_dir,
            env=env,
        )

    if not(ignore_returncode) and (result.returncode != expected_returncode):
        print(result.stdout.decode('utf-8'))
        assert False, "Got unexpected return code {}".format(result.returncode)
    return result.stdout.decode('utf-8')

def available_kems_by_name():
    available_names = []
    with open(os.path.join('src', 'kem', 'kem.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_KEM_alg_"):
                kem_name = line.split(' ')[2]
                kem_name = kem_name[1:-2]
                available_names.append(kem_name)
    return available_names

def is_kem_enabled_by_name(name):
    symbol = None
    with open(os.path.join('src', 'kem', 'kem.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_KEM_alg_"):
                kem_symbol = line.split(' ')[1]
                kem_symbol = kem_symbol[len("OQS_KEM_alg_"):]
                kem_name = line.split(' ')[2]
                kem_name = kem_name[1:-2]
                if kem_name == name:
                    symbol = kem_symbol
                    break
    if symbol == None: return False
    header = os.path.join(get_current_build_dir_name(), 'include', 'oqs', 'oqsconfig.h')
    with open(header) as fh:
        for line in fh:
            if line.startswith("#define OQS_ENABLE_KEM_"):
                kem_symbol = line.split(' ')[1]
                kem_symbol = kem_symbol[len("OQS_ENABLE_KEM_"):].rstrip()
                if kem_symbol == symbol:
                    return True
    return False

def available_sigs_by_name():
    available_names = []
    with open(os.path.join('src', 'sig', 'sig.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_SIG_alg_"):
                sig_name = line.split(' ')[2]
                sig_name = sig_name[1:-2]
                available_names.append(sig_name)
    return available_names

def is_sig_enabled_by_name(name):
    symbol = None
    with open(os.path.join('src', 'sig', 'sig.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_SIG_alg_"):
                sig_symbol = line.split(' ')[1]
                sig_symbol = sig_symbol[len("OQS_SIG_alg_"):]
                sig_name = line.split(' ')[2]
                sig_name = sig_name[1:-2]
                if sig_name == name:
                    symbol = sig_symbol
                    break
    if symbol == None: return False
    header = os.path.join(get_current_build_dir_name(), 'include', 'oqs', 'oqsconfig.h')
    with open(header) as fh:
        for line in fh:
            if line.startswith("#define OQS_ENABLE_SIG_"):
                sig_symbol = line.split(' ')[1]
                sig_symbol = sig_symbol[len("OQS_ENABLE_SIG_"):].rstrip()
                if sig_symbol == symbol:
                    return True
    return False

def filtered_test(func):
    funcname = func.__name__[len("test_"):]

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        if ('SKIP_ALGS' in os.environ) and len(os.environ['SKIP_ALGS'])>0:
            for algexp in os.environ['SKIP_ALGS'].split(','):
                for arg in args:
                    if len(re.findall(algexp, arg))>0:
                        pytest.skip("Test disabled by alg filter")
                for arg in kwargs:
                    if len(re.findall(algexp, kwargs[arg]))>0:
                        pytest.skip("Test disabled by alg filter")
        if ('SKIP_TESTS' in os.environ) and (funcname in os.environ['SKIP_TESTS'].lower().split(',')):
            pytest.skip("Test disabled by filter")
        else:
            return func(*args, **kwargs)
    return wrapper

# So far, build dir name has been hard coded to "build".
# This function makes it dependent on the availability of the environment variable OQS_BUILD_DIR:
# If OQS_BUILD_DIR is not set, behave as before, returning hard-coded build name set as per README
def get_current_build_dir_name():
    if 'OQS_BUILD_DIR' in os.environ:
        return os.environ['OQS_BUILD_DIR']
    return 'build'

def path_to_executable(program_name):
    path = "."
    path = os.path.join(path, get_current_build_dir_name(), "tests")
    for executable in [
        os.path.join(path, program_name),
        os.path.join(path, program_name + ".EXE"),
        os.path.join(path, program_name + ".exe")]:
            if os.path.isfile(executable):
                return executable
    assert False, "Unable to find executable file {}".format(program_name)

def available_use_options_by_name():
    enabled_use_options = []
    with open(os.path.join(get_current_build_dir_name(), 'include', 'oqs', 'oqsconfig.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_USE_"):
                option_name = line.split(' ')[1][len("OQS_USE_"):].strip('\n')
                enabled_use_options.append(option_name)
    return enabled_use_options

def is_use_option_enabled_by_name(name):
    return name in available_use_options_by_name()

def get_kats(t):
   if kats[t] is None:
     with open(os.path.join('tests', 'KATs', t, 'kats.json'), 'r') as fp:
      kats[t] = json.load(fp)
   return kats[t]

@functools.lru_cache()
def get_valgrind_version():
    try:
        version = run_subprocess(['valgrind', '--version'])
        x,y,z = map(int, version.replace('valgrind-','').split('.'))
    except:
        x,y,z = 0,0,0
    return x, y, z

def test_requires_valgrind_version_at_least(x,y,z):
    (X,Y,Z) = get_valgrind_version()
    return pytest.mark.skipif((X < x) or (X == x and Y < y) or (X == x and Y == y and Z < z),
                reason='Test requires Valgrind >= {}.{}.{}'.format(x,y,z))

@functools.lru_cache()
def test_requires_build_options(*options):
    enabled = {opt : False for opt in options}
    with open(os.path.join(get_current_build_dir_name(), 'include', 'oqs', 'oqsconfig.h')) as fh:
        for line in fh:
            opt = line.split(' ')[1] if line.startswith('#define ') else None
            if opt in options:
                enabled[opt] = True
    missing = ', '.join([opt for opt in options if not enabled[opt]])
    return pytest.mark.skipif(not all(enabled.values()),
                reason='Test requires missing build options {}'.format(missing))


@functools.lru_cache()
def test_requires_qemu(platform, mincpu):
    no_qemu=False
    try:
        run_subprocess(["qemu-"+platform+"-static", "-cpu", mincpu, path_to_executable('test_kem')], ignore_returncode=True)
    except:
        no_qemu=True
    return pytest.mark.skipif(no_qemu,
                reason='Test requires qemu-{}-static -cpu {}'.format(platform, mincpu))
