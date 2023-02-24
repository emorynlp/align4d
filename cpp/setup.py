"""
This is the .py file that is needed to build the cpp source code to object used for python extension.
To use this file, in the command line, change the directory to the one included in this file and type
'python setup.py build' or 'python3 setup.py build'
the .pyd or .so file are the compiled objects in the build directory
"""
from distutils.core import Extension, setup
import os


# os.environ["CC"] = "g++-12" #  for mac os
# os.environ["CXX"] = "g++-12" #  for mac os

module1 = Extension(
    "align4d",
    sources=["align4d_cpython_extension.cpp", "align.cpp", "msa.cpp", "postprocess.cpp", "preprocess.cpp"],
    extra_compile_args=["/std:c++20", "/O2"],  # for MSVC
    # extra_compile_args=["-std=c++2a", "-O2"]  # for G++， change to -std=c++20 if you can
)

setup(
    name="align4d",
    version="1.0.0",
    ext_modules=[
        module1,
    ],
    python_requires=">=3.8"
)
