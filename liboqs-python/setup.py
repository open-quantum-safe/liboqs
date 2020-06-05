from setuptools import find_packages

from distutils.core import setup

setup(
    name='liboqs-python',
    version='0.2.1',
    author='Open Quantum Safe project',
    author_email='contact@openquantumsafe.org',
    packages=find_packages(exclude=('tests', 'docs', 'examples')),
    scripts=[],
    url='https://github.com/open-quantum-safe/liboqs-python',
    license='LICENSE.txt',
    description='Python wrapper for liboqs, providing post-quantum public key cryptography algorithms',
    long_description=open('README.md').read(),
)
