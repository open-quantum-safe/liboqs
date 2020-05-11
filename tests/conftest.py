# SPDX-License-Identifier: MIT

import os
import pytest
import subprocess

def pytest_addoption(parser):
    parser.addoption("--build_tool", action="store", help="build_tool: The tool (make, ninja, etc.) used to build the artifacts.")
    parser.addoption("--use_liboqs_so", action="store_true", help="open_liboqs_so: Ask test_namespace.py to use liboqs.so instead of liboqs.a")

@pytest.fixture
def build_tool(request):
    return request.config.getoption("--build_tool")

@pytest.fixture
def use_liboqs_so(request):
    return request.config.getoption("--use_liboqs_so")
