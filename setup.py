import os
from setuptools import setup

with open('python/opentera_libraries/requirements.txt') as f:
    required = f.read().splitlines()

setup(install_requires=required)
