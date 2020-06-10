# Copyright (c) 2015, Google Inc.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

"""Enumerates source files for consumption by various build systems."""

import optparse
import os
import subprocess
import sys
import json


# OS_ARCH_COMBOS maps from OS and platform to the OpenSSL assembly "style" for
# that platform and the extension used by asm files.
OS_ARCH_COMBOS = [
    ('linux', 'arm', 'linux32', [], 'S'),
    ('linux', 'aarch64', 'linux64', [], 'S'),
    ('linux', 'ppc64le', 'ppc64le', [], 'S'),
    ('linux', 'x86', 'elf', ['-fPIC', '-DOPENSSL_IA32_SSE2'], 'S'),
    ('linux', 'x86_64', 'elf', [], 'S'),
    ('mac', 'x86', 'macosx', ['-fPIC', '-DOPENSSL_IA32_SSE2'], 'S'),
    ('mac', 'x86_64', 'macosx', [], 'S'),
    ('win', 'x86', 'win32n', ['-DOPENSSL_IA32_SSE2'], 'asm'),
    ('win', 'x86_64', 'nasm', [], 'asm'),
]

# NON_PERL_FILES enumerates assembly files that are not processed by the
# perlasm system.
NON_PERL_FILES = {
    ('linux', 'arm'): [
        'src/crypto/curve25519/asm/x25519-asm-arm.S',
        'src/crypto/poly1305/poly1305_arm_asm.S',
    ],
    ('linux', 'x86_64'): [
        'src/crypto/curve25519/asm/x25519-asm-x86_64.S',
    ],
    ('mac', 'x86_64'): [
        'src/crypto/curve25519/asm/x25519-asm-x86_64.S',
    ],
}

PREFIX = None


def PathOf(x):
  return x if not PREFIX else os.path.join(PREFIX, x)


class Android(object):

  def __init__(self):
    self.header = \
"""# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This file is created by generate_build_files.py. Do not edit manually.

"""

  def PrintVariableSection(self, out, name, files):
    out.write('%s := \\\n' % name)
    for f in sorted(files):
      out.write('  %s\\\n' % f)
    out.write('\n')

  def WriteFiles(self, files, asm_outputs):
    # New Android.bp format
    with open('sources.bp', 'w+') as blueprint:
      blueprint.write(self.header.replace('#', '//'))

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "libcrypto_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['crypto']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('    target: {\n')

      for ((osname, arch), asm_files) in asm_outputs:
        if osname != 'linux' or arch == 'ppc64le':
          continue
        if arch == 'aarch64':
          arch = 'arm64'

        blueprint.write('        android_%s: {\n' % arch)
        blueprint.write('            srcs: [\n')
        for f in sorted(asm_files):
          blueprint.write('                "%s",\n' % f)
        blueprint.write('            ],\n')
        blueprint.write('        },\n')

        if arch == 'x86' or arch == 'x86_64':
          blueprint.write('        linux_%s: {\n' % arch)
          blueprint.write('            srcs: [\n')
          for f in sorted(asm_files):
            blueprint.write('                "%s",\n' % f)
          blueprint.write('            ],\n')
          blueprint.write('        },\n')

      blueprint.write('    },\n')
      blueprint.write('}\n\n')

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "libssl_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['ssl']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('}\n\n')

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "bssl_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['tool']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('}\n\n')

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "boringssl_test_support_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['test_support']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('}\n\n')

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "boringssl_crypto_test_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['crypto_test']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('}\n\n')

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "boringssl_ssl_test_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['ssl_test']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('}\n\n')

      blueprint.write('cc_defaults {\n')
      blueprint.write('    name: "boringssl_tests_sources",\n')
      blueprint.write('    srcs: [\n')
      for f in sorted(files['test']):
        blueprint.write('        "%s",\n' % f)
      blueprint.write('    ],\n')
      blueprint.write('}\n')

    # Legacy Android.mk format, only used by Trusty in new branches
    with open('sources.mk', 'w+') as makefile:
      makefile.write(self.header)

      self.PrintVariableSection(makefile, 'crypto_sources', files['crypto'])

      for ((osname, arch), asm_files) in asm_outputs:
        if osname != 'linux':
          continue
        self.PrintVariableSection(
            makefile, '%s_%s_sources' % (osname, arch), asm_files)


class Bazel(object):
  """Bazel outputs files suitable for including in Bazel files."""

  def __init__(self):
    self.firstSection = True
    self.header = \
"""# This file is created by generate_build_files.py. Do not edit manually.

"""

  def PrintVariableSection(self, out, name, files):
    if not self.firstSection:
      out.write('\n')
    self.firstSection = False

    out.write('%s = [\n' % name)
    for f in sorted(files):
      out.write('    "%s",\n' % PathOf(f))
    out.write(']\n')

  def WriteFiles(self, files, asm_outputs):
    with open('BUILD.generated.bzl', 'w+') as out:
      out.write(self.header)

      self.PrintVariableSection(out, 'ssl_headers', files['ssl_headers'])
      self.PrintVariableSection(
          out, 'ssl_internal_headers', files['ssl_internal_headers'])
      self.PrintVariableSection(out, 'ssl_sources', files['ssl'])
      self.PrintVariableSection(out, 'ssl_c_sources', files['ssl_c'])
      self.PrintVariableSection(out, 'ssl_cc_sources', files['ssl_cc'])
      self.PrintVariableSection(out, 'crypto_headers', files['crypto_headers'])
      self.PrintVariableSection(
          out, 'crypto_internal_headers', files['crypto_internal_headers'])
      self.PrintVariableSection(out, 'crypto_sources', files['crypto'])
      self.PrintVariableSection(out, 'tool_sources', files['tool'])
      self.PrintVariableSection(out, 'tool_headers', files['tool_headers'])

      for ((osname, arch), asm_files) in asm_outputs:
        self.PrintVariableSection(
            out, 'crypto_sources_%s_%s' % (osname, arch), asm_files)

    with open('BUILD.generated_tests.bzl', 'w+') as out:
      out.write(self.header)

      out.write('test_support_sources = [\n')
      for filename in sorted(files['test_support'] +
                             files['test_support_headers'] +
                             files['crypto_internal_headers'] +
                             files['ssl_internal_headers']):
        if os.path.basename(filename) == 'malloc.cc':
          continue
        out.write('    "%s",\n' % PathOf(filename))

      out.write(']\n\n')

      self.PrintVariableSection(out, 'crypto_test_sources',
                                files['crypto_test'])
      self.PrintVariableSection(out, 'ssl_test_sources', files['ssl_test'])

      out.write('def create_tests(copts, crypto, ssl):\n')
      name_counts = {}
      for test in files['tests']:
        name = os.path.basename(test[0])
        name_counts[name] = name_counts.get(name, 0) + 1

      first = True
      for test in files['tests']:
        name = os.path.basename(test[0])
        if name_counts[name] > 1:
          if '/' in test[1]:
            name += '_' + os.path.splitext(os.path.basename(test[1]))[0]
          else:
            name += '_' + test[1].replace('-', '_')

        if not first:
          out.write('\n')
        first = False

        src_prefix = 'src/' + test[0]
        for src in files['test']:
          if src.startswith(src_prefix):
            src = src
            break
        else:
          raise ValueError("Can't find source for %s" % test[0])

        out.write('  native.cc_test(\n')
        out.write('      name = "%s",\n' % name)
        out.write('      size = "small",\n')
        out.write('      srcs = ["%s"] + test_support_sources,\n' %
            PathOf(src))

        data_files = []
        if len(test) > 1:

          out.write('      args = [\n')
          for arg in test[1:]:
            if '/' in arg:
              out.write('          "$(location %s)",\n' %
                  PathOf(os.path.join('src', arg)))
              data_files.append('src/%s' % arg)
            else:
              out.write('          "%s",\n' % arg)
          out.write('      ],\n')

        out.write('      copts = copts + ["-DBORINGSSL_SHARED_LIBRARY"],\n')

        if len(data_files) > 0:
          out.write('      data = [\n')
          for filename in data_files:
            out.write('          "%s",\n' % PathOf(filename))
          out.write('      ],\n')

        if 'ssl/' in test[0]:
          out.write('      deps = [\n')
          out.write('          crypto,\n')
          out.write('          ssl,\n')
          out.write('      ],\n')
        else:
          out.write('      deps = [crypto],\n')
        out.write('  )\n')


class GN(object):

  def __init__(self):
    self.firstSection = True
    self.header = \
"""# Copyright (c) 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# This file is created by generate_build_files.py. Do not edit manually.

"""

  def PrintVariableSection(self, out, name, files):
    if not self.firstSection:
      out.write('\n')
    self.firstSection = False

    out.write('%s = [\n' % name)
    for f in sorted(files):
      out.write('  "%s",\n' % f)
    out.write(']\n')

  def WriteFiles(self, files, asm_outputs):
    with open('BUILD.generated.gni', 'w+') as out:
      out.write(self.header)

      self.PrintVariableSection(out, 'crypto_sources',
                                files['crypto'] + files['crypto_headers'] +
                                files['crypto_internal_headers'])
      self.PrintVariableSection(out, 'ssl_sources',
                                files['ssl'] + files['ssl_headers'] +
                                files['ssl_internal_headers'])

      for ((osname, arch), asm_files) in asm_outputs:
        self.PrintVariableSection(
            out, 'crypto_sources_%s_%s' % (osname, arch), asm_files)

      fuzzers = [os.path.splitext(os.path.basename(fuzzer))[0]
                 for fuzzer in files['fuzz']]
      self.PrintVariableSection(out, 'fuzzers', fuzzers)

    with open('BUILD.generated_tests.gni', 'w+') as out:
      self.firstSection = True
      out.write(self.header)

      self.PrintVariableSection(out, 'test_support_sources',
                                files['test_support'] +
                                files['test_support_headers'])
      self.PrintVariableSection(out, 'crypto_test_sources',
                                files['crypto_test'])
      self.PrintVariableSection(out, 'ssl_test_sources', files['ssl_test'])
      out.write('\n')

      out.write('template("create_tests") {\n')

      all_tests = []
      for test in sorted(files['test']):
        test_name = 'boringssl_%s' % os.path.splitext(os.path.basename(test))[0]
        all_tests.append(test_name)

        out.write('  executable("%s") {\n' % test_name)
        out.write('    sources = [\n')
        out.write('      "%s",\n' % test)
        out.write('    ]\n')
        out.write('    sources += test_support_sources\n')
        out.write('    if (defined(invoker.configs_exclude)) {\n')
        out.write('      configs -= invoker.configs_exclude\n')
        out.write('    }\n')
        out.write('    configs += invoker.configs\n')
        out.write('    deps = invoker.deps\n')
        out.write('  }\n')
        out.write('\n')

      out.write('  group(target_name) {\n')
      out.write('    deps = [\n')
      for test_name in sorted(all_tests):
        out.write('      ":%s",\n' % test_name)
      out.write('    ]\n')
      out.write('  }\n')
      out.write('}\n')


class GYP(object):

  def __init__(self):
    self.header = \
"""# Copyright (c) 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# This file is created by generate_build_files.py. Do not edit manually.

"""

  def PrintVariableSection(self, out, name, files):
    out.write('    \'%s\': [\n' % name)
    for f in sorted(files):
      out.write('      \'%s\',\n' % f)
    out.write('    ],\n')

  def WriteFiles(self, files, asm_outputs):
    with open('boringssl.gypi', 'w+') as gypi:
      gypi.write(self.header + '{\n  \'variables\': {\n')

      self.PrintVariableSection(gypi, 'boringssl_ssl_sources',
                                files['ssl'] + files['ssl_headers'] +
                                files['ssl_internal_headers'])
      self.PrintVariableSection(gypi, 'boringssl_crypto_sources',
                                files['crypto'] + files['crypto_headers'] +
                                files['crypto_internal_headers'])

      for ((osname, arch), asm_files) in asm_outputs:
        self.PrintVariableSection(gypi, 'boringssl_%s_%s_sources' %
                                  (osname, arch), asm_files)

      gypi.write('  }\n}\n')


def FindCMakeFiles(directory):
  """Returns list of all CMakeLists.txt files recursively in directory."""
  cmakefiles = []

  for (path, _, filenames) in os.walk(directory):
    for filename in filenames:
      if filename == 'CMakeLists.txt':
        cmakefiles.append(os.path.join(path, filename))

  return cmakefiles


def NoTests(dent, is_dir):
  """Filter function that can be passed to FindCFiles in order to remove test
  sources."""
  if is_dir:
    return dent != 'test'
  return 'test.' not in dent and not dent.startswith('example_')


def OnlyTests(dent, is_dir):
  """Filter function that can be passed to FindCFiles in order to remove
  non-test sources."""
  if is_dir:
    return dent != 'test'
  return '_test.' in dent or dent.startswith('example_')


def AllFiles(dent, is_dir):
  """Filter function that can be passed to FindCFiles in order to include all
  sources."""
  return True


def NotGTestMain(dent, is_dir):
  return dent != 'gtest_main.cc'


def SSLHeaderFiles(dent, is_dir):
  return dent in ['ssl.h', 'tls1.h', 'ssl23.h', 'ssl3.h', 'dtls1.h']


def FindCFiles(directory, filter_func):
  """Recurses through directory and returns a list of paths to all the C source
  files that pass filter_func."""
  cfiles = []

  for (path, dirnames, filenames) in os.walk(directory):
    for filename in filenames:
      if not filename.endswith('.c') and not filename.endswith('.cc'):
        continue
      if not filter_func(filename, False):
        continue
      cfiles.append(os.path.join(path, filename))

    for (i, dirname) in enumerate(dirnames):
      if not filter_func(dirname, True):
        del dirnames[i]

  return cfiles


def FindHeaderFiles(directory, filter_func):
  """Recurses through directory and returns a list of paths to all the header files that pass filter_func."""
  hfiles = []

  for (path, dirnames, filenames) in os.walk(directory):
    for filename in filenames:
      if not filename.endswith('.h'):
        continue
      if not filter_func(filename, False):
        continue
      hfiles.append(os.path.join(path, filename))

      for (i, dirname) in enumerate(dirnames):
        if not filter_func(dirname, True):
          del dirnames[i]

  return hfiles


def ExtractPerlAsmFromCMakeFile(cmakefile):
  """Parses the contents of the CMakeLists.txt file passed as an argument and
  returns a list of all the perlasm() directives found in the file."""
  perlasms = []
  with open(cmakefile) as f:
    for line in f:
      line = line.strip()
      if not line.startswith('perlasm('):
        continue
      if not line.endswith(')'):
        raise ValueError('Bad perlasm line in %s' % cmakefile)
      # Remove "perlasm(" from start and ")" from end
      params = line[8:-1].split()
      if len(params) < 2:
        raise ValueError('Bad perlasm line in %s' % cmakefile)
      perlasms.append({
          'extra_args': params[2:],
          'input': os.path.join(os.path.dirname(cmakefile), params[1]),
          'output': os.path.join(os.path.dirname(cmakefile), params[0]),
      })

  return perlasms


def ReadPerlAsmOperations():
  """Returns a list of all perlasm() directives found in CMake config files in
  src/."""
  perlasms = []
  cmakefiles = FindCMakeFiles('src')

  for cmakefile in cmakefiles:
    perlasms.extend(ExtractPerlAsmFromCMakeFile(cmakefile))

  return perlasms


def PerlAsm(output_filename, input_filename, perlasm_style, extra_args):
  """Runs the a perlasm script and puts the output into output_filename."""
  base_dir = os.path.dirname(output_filename)
  if not os.path.isdir(base_dir):
    os.makedirs(base_dir)
  subprocess.check_call(
      ['perl', input_filename, perlasm_style] + extra_args + [output_filename])


def ArchForAsmFilename(filename):
  """Returns the architectures that a given asm file should be compiled for
  based on substrings in the filename."""

  if 'x86_64' in filename or 'avx2' in filename:
    return ['x86_64']
  elif ('x86' in filename and 'x86_64' not in filename) or '586' in filename:
    return ['x86']
  elif 'armx' in filename:
    return ['arm', 'aarch64']
  elif 'armv8' in filename:
    return ['aarch64']
  elif 'arm' in filename:
    return ['arm']
  elif 'ppc' in filename:
    return ['ppc64le']
  else:
    raise ValueError('Unknown arch for asm filename: ' + filename)


def WriteAsmFiles(perlasms):
  """Generates asm files from perlasm directives for each supported OS x
  platform combination."""
  asmfiles = {}

  for osarch in OS_ARCH_COMBOS:
    (osname, arch, perlasm_style, extra_args, asm_ext) = osarch
    key = (osname, arch)
    outDir = '%s-%s' % key

    for perlasm in perlasms:
      filename = os.path.basename(perlasm['input'])
      output = perlasm['output']
      if not output.startswith('src'):
        raise ValueError('output missing src: %s' % output)
      output = os.path.join(outDir, output[4:])
      if output.endswith('-armx.${ASM_EXT}'):
        output = output.replace('-armx',
                                '-armx64' if arch == 'aarch64' else '-armx32')
      output = output.replace('${ASM_EXT}', asm_ext)

      if arch in ArchForAsmFilename(filename):
        PerlAsm(output, perlasm['input'], perlasm_style,
                perlasm['extra_args'] + extra_args)
        asmfiles.setdefault(key, []).append(output)

  for (key, non_perl_asm_files) in NON_PERL_FILES.iteritems():
    asmfiles.setdefault(key, []).extend(non_perl_asm_files)

  return asmfiles


def IsGTest(path):
  with open(path) as f:
    return "#include <gtest/gtest.h>" in f.read()


def main(platforms):
  crypto_c_files = FindCFiles(os.path.join('src', 'crypto'), NoTests)
  ssl_source_files = FindCFiles(os.path.join('src', 'ssl'), NoTests)
  tool_c_files = FindCFiles(os.path.join('src', 'tool'), NoTests)
  tool_h_files = FindHeaderFiles(os.path.join('src', 'tool'), AllFiles)

  # Generate err_data.c
  with open('err_data.c', 'w+') as err_data:
    subprocess.check_call(['go', 'run', 'err_data_generate.go'],
                          cwd=os.path.join('src', 'crypto', 'err'),
                          stdout=err_data)
  crypto_c_files.append('err_data.c')

  test_support_c_files = FindCFiles(os.path.join('src', 'crypto', 'test'),
                                    NotGTestMain)
  test_support_h_files = (
      FindHeaderFiles(os.path.join('src', 'crypto', 'test'), AllFiles) +
      FindHeaderFiles(os.path.join('src', 'ssl', 'test'), AllFiles))

  test_c_files = []
  crypto_test_files = ['src/crypto/test/gtest_main.cc']
  # TODO(davidben): Remove this loop once all tests are converted.
  for path in FindCFiles(os.path.join('src', 'crypto'), OnlyTests):
    if IsGTest(path):
      crypto_test_files.append(path)
    else:
      test_c_files.append(path)

  ssl_test_files = FindCFiles(os.path.join('src', 'ssl'), OnlyTests)
  ssl_test_files.append('src/crypto/test/gtest_main.cc')

  fuzz_c_files = FindCFiles(os.path.join('src', 'fuzz'), NoTests)

  ssl_h_files = (
      FindHeaderFiles(
          os.path.join('src', 'include', 'openssl'),
          SSLHeaderFiles))

  def NotSSLHeaderFiles(filename, is_dir):
    return not SSLHeaderFiles(filename, is_dir)
  crypto_h_files = (
      FindHeaderFiles(
          os.path.join('src', 'include', 'openssl'),
          NotSSLHeaderFiles))

  ssl_internal_h_files = FindHeaderFiles(os.path.join('src', 'ssl'), NoTests)
  crypto_internal_h_files = FindHeaderFiles(
      os.path.join('src', 'crypto'), NoTests)

  with open('src/util/all_tests.json', 'r') as f:
    tests = json.load(f)
  # For now, GTest-based tests are specified manually.
  tests = [test for test in tests if test[0] not in ['crypto/crypto_test',
                                                     'decrepit/decrepit_test',
                                                     'ssl/ssl_test']]
  test_binaries = set([test[0] for test in tests])
  test_sources = set([
      test.replace('.cc', '').replace('.c', '').replace(
          'src/',
          '')
      for test in test_c_files])
  if test_binaries != test_sources:
    print 'Test sources and configured tests do not match'
    a = test_binaries.difference(test_sources)
    if len(a) > 0:
      print 'These tests are configured without sources: ' + str(a)
    b = test_sources.difference(test_binaries)
    if len(b) > 0:
      print 'These test sources are not configured: ' + str(b)

  files = {
      'crypto': crypto_c_files,
      'crypto_headers': crypto_h_files,
      'crypto_internal_headers': crypto_internal_h_files,
      'crypto_test': sorted(crypto_test_files),
      'fuzz': fuzz_c_files,
      'ssl': ssl_source_files,
      'ssl_c': [s for s in ssl_source_files if s.endswith('.c')],
      'ssl_cc': [s for s in ssl_source_files if s.endswith('.cc')],
      'ssl_headers': ssl_h_files,
      'ssl_internal_headers': ssl_internal_h_files,
      'ssl_test': sorted(ssl_test_files),
      'tool': tool_c_files,
      'tool_headers': tool_h_files,
      'test': test_c_files,
      'test_support': test_support_c_files,
      'test_support_headers': test_support_h_files,
      'tests': tests,
  }

  asm_outputs = sorted(WriteAsmFiles(ReadPerlAsmOperations()).iteritems())

  for platform in platforms:
    platform.WriteFiles(files, asm_outputs)

  return 0


if __name__ == '__main__':
  parser = optparse.OptionParser(usage='Usage: %prog [--prefix=<path>]'
      ' [android|bazel|gn|gyp]')
  parser.add_option('--prefix', dest='prefix',
      help='For Bazel, prepend argument to all source files')
  options, args = parser.parse_args(sys.argv[1:])
  PREFIX = options.prefix

  if not args:
    parser.print_help()
    sys.exit(1)

  platforms = []
  for s in args:
    if s == 'android':
      platforms.append(Android())
    elif s == 'bazel':
      platforms.append(Bazel())
    elif s == 'gn':
      platforms.append(GN())
    elif s == 'gyp':
      platforms.append(GYP())
    else:
      parser.print_help()
      sys.exit(1)

  sys.exit(main(platforms))
