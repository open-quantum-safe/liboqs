#!/usr/bin/env python

# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import os
import platform
import subprocess
import sys
from distutils.command.build import build

import pkg_resources

import setuptools
from setuptools import find_packages, setup
from setuptools.command.install import install
from setuptools.command.test import test


if (
    pkg_resources.parse_version(setuptools.__version__) <
    pkg_resources.parse_version("18.5")
):
    raise RuntimeError(
        "cryptography requires setuptools 18.5 or newer, please upgrade to a "
        "newer version of setuptools"
    )

base_dir = os.path.dirname(__file__)
src_dir = os.path.join(base_dir, "src")

# When executing the setup.py, we need to be able to import ourselves, this
# means that we need to add the src/ directory to the sys.path.
sys.path.insert(0, src_dir)

about = {}
with open(os.path.join(src_dir, "cryptography", "__about__.py")) as f:
    exec(f.read(), about)


VECTORS_DEPENDENCY = "cryptography_vectors=={0}".format(about['__version__'])

# `setup_requirements` must be kept in sync with `pyproject.toml`
setup_requirements = ["cffi>=1.7,!=1.11.3"]

if platform.python_implementation() == "PyPy":
    if sys.pypy_version_info < (5, 3):
        raise RuntimeError(
            "cryptography is not compatible with PyPy < 5.3. Please upgrade "
            "PyPy to use this library."
        )

test_requirements = [
    "pytest>=3.6.0",
    "pretend",
    "iso8601",
    "pytz",
    "hypothesis>=1.11.4",
]


# If there's no vectors locally that probably means we are in a tarball and
# need to go and get the matching vectors package from PyPi
if not os.path.exists(os.path.join(base_dir, "vectors/setup.py")):
    test_requirements.append(VECTORS_DEPENDENCY)


class PyTest(test):
    def finalize_options(self):
        test.finalize_options(self)
        self.test_args = []
        self.test_suite = True

        # This means there's a vectors/ folder with the package in here.
        # cd into it, install the vectors package and then refresh sys.path
        if VECTORS_DEPENDENCY not in test_requirements:
            subprocess.check_call(
                [sys.executable, "setup.py", "install"], cwd="vectors"
            )
            pkg_resources.get_distribution("cryptography_vectors").activate()

    def run_tests(self):
        # Import here because in module scope the eggs are not loaded.
        import pytest
        test_args = [os.path.join(base_dir, "tests")]
        errno = pytest.main(test_args)
        sys.exit(errno)


def keywords_with_side_effects(argv):
    """
    Get a dictionary with setup keywords that (can) have side effects.

    :param argv: A list of strings with command line arguments.
    :returns: A dictionary with keyword arguments for the ``setup()`` function.

    This setup.py script uses the setuptools 'setup_requires' feature because
    this is required by the cffi package to compile extension modules. The
    purpose of ``keywords_with_side_effects()`` is to avoid triggering the cffi
    build process as a result of setup.py invocations that don't need the cffi
    module to be built (setup.py serves the dual purpose of exposing package
    metadata).

    All of the options listed by ``python setup.py --help`` that print
    information should be recognized here. The commands ``clean``,
    ``egg_info``, ``register``, ``sdist`` and ``upload`` are also recognized.
    Any combination of these options and commands is also supported.

    This function was originally based on the `setup.py script`_ of SciPy (see
    also the discussion in `pip issue #25`_).

    .. _pip issue #25: https://github.com/pypa/pip/issues/25
    .. _setup.py script: https://github.com/scipy/scipy/blob/master/setup.py
    """
    no_setup_requires_arguments = (
        '-h', '--help',
        '-n', '--dry-run',
        '-q', '--quiet',
        '-v', '--verbose',
        '-V', '--version',
        '--author',
        '--author-email',
        '--classifiers',
        '--contact',
        '--contact-email',
        '--description',
        '--egg-base',
        '--fullname',
        '--help-commands',
        '--keywords',
        '--licence',
        '--license',
        '--long-description',
        '--maintainer',
        '--maintainer-email',
        '--name',
        '--no-user-cfg',
        '--obsoletes',
        '--platforms',
        '--provides',
        '--requires',
        '--url',
        'clean',
        'egg_info',
        'register',
        'sdist',
        'upload',
    )

    def is_short_option(argument):
        """Check whether a command line argument is a short option."""
        return len(argument) >= 2 and argument[0] == '-' and argument[1] != '-'

    def expand_short_options(argument):
        """Expand combined short options into canonical short options."""
        return ('-' + char for char in argument[1:])

    def argument_without_setup_requirements(argv, i):
        """Check whether a command line argument needs setup requirements."""
        if argv[i] in no_setup_requires_arguments:
            # Simple case: An argument which is either an option or a command
            # which doesn't need setup requirements.
            return True
        elif (is_short_option(argv[i]) and
              all(option in no_setup_requires_arguments
                  for option in expand_short_options(argv[i]))):
            # Not so simple case: Combined short options none of which need
            # setup requirements.
            return True
        elif argv[i - 1:i] == ['--egg-base']:
            # Tricky case: --egg-info takes an argument which should not make
            # us use setup_requires (defeating the purpose of this code).
            return True
        else:
            return False

    if all(argument_without_setup_requirements(argv, i)
           for i in range(1, len(argv))):
        return {
            "cmdclass": {
                "build": DummyBuild,
                "install": DummyInstall,
                "test": DummyPyTest,
            }
        }
    else:
        cffi_modules = [
            "src/_cffi_src/build_openssl.py:ffi",
            "src/_cffi_src/build_constant_time.py:ffi",
            "src/_cffi_src/build_padding.py:ffi",
        ]

        return {
            "setup_requires": setup_requirements,
            "cmdclass": {
                "test": PyTest,
            },
            "cffi_modules": cffi_modules
        }


setup_requires_error = ("Requested setup command that needs 'setup_requires' "
                        "while command line arguments implied a side effect "
                        "free command or option.")


class DummyBuild(build):
    """
    This class makes it very obvious when ``keywords_with_side_effects()`` has
    incorrectly interpreted the command line arguments to ``setup.py build`` as
    one of the 'side effect free' commands or options.
    """

    def run(self):
        raise RuntimeError(setup_requires_error)


class DummyInstall(install):
    """
    This class makes it very obvious when ``keywords_with_side_effects()`` has
    incorrectly interpreted the command line arguments to ``setup.py install``
    as one of the 'side effect free' commands or options.
    """

    def run(self):
        raise RuntimeError(setup_requires_error)


class DummyPyTest(test):
    """
    This class makes it very obvious when ``keywords_with_side_effects()`` has
    incorrectly interpreted the command line arguments to ``setup.py test`` as
    one of the 'side effect free' commands or options.
    """

    def run_tests(self):
        raise RuntimeError(setup_requires_error)


with open(os.path.join(base_dir, "README.rst")) as f:
    long_description = f.read()


setup(
    name=about["__title__"],
    version=about["__version__"],

    description=about["__summary__"],
    long_description=long_description,
    license=about["__license__"],
    url=about["__uri__"],

    author=about["__author__"],
    author_email=about["__email__"],

    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: Apache Software License",
        "License :: OSI Approved :: BSD License",
        "Natural Language :: English",
        "Operating System :: MacOS :: MacOS X",
        "Operating System :: POSIX",
        "Operating System :: POSIX :: BSD",
        "Operating System :: POSIX :: Linux",
        "Operating System :: Microsoft :: Windows",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.4",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: Implementation :: CPython",
        "Programming Language :: Python :: Implementation :: PyPy",
        "Topic :: Security :: Cryptography",
    ],

    package_dir={"": "src"},
    packages=find_packages(where="src", exclude=["_cffi_src", "_cffi_src.*"]),
    include_package_data=True,

    python_requires='>=2.7,!=3.0.*,!=3.1.*,!=3.2.*,!=3.3.*',

    install_requires=[
        "idna >= 2.1",
        "asn1crypto >= 0.21.0",
        "six >= 1.4.1",
    ] + setup_requirements,
    tests_require=test_requirements,
    extras_require={
        ":python_version < '3'": ["enum34", "ipaddress"],

        "test": test_requirements,
        "docs": [
            "sphinx >= 1.6.5",
            "sphinx_rtd_theme",
        ],
        "docstest": [
            "doc8",
            "pyenchant >= 1.6.11",
            "readme_renderer >= 16.0",
            "sphinxcontrib-spelling >= 4.0.1",
        ],
        "pep8test": [
            "flake8",
            "flake8-import-order",
            "pep8-naming",
        ],
    },

    # for cffi
    zip_safe=False,
    ext_package="cryptography.hazmat.bindings",
    **keywords_with_side_effects(sys.argv)
)
