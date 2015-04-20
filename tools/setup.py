from distutils.core import setup
from distutils.extension import Extension

import os.path
import sys

setup(name='mandala',
    ext_modules=[
        Extension('hello', ['../mandala/python_mgr.cpp'],
        include_dirs=['C:\\boost_1_55_0', '..\ext\glm-0.9.4.4'],
        library_dirs=['C:\\boost_1_55_0\stage\lib'])
    ])