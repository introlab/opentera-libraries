import os
from setuptools import setup

with open('python/requirements.txt') as f:
    required = f.read().splitlines()

setup(install_requires=required)
