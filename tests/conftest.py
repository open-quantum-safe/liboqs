import os
import pytest
import subprocess

def pytest_addoption(parser):
    parser.addoption("--build_tool", action="store", help="build_tool: The tool (make, ninja, etc.) used to build the artifacts.")

@pytest.fixture
def build_tool(request):
    return request.config.getoption("--build_tool")
