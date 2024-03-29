"""
This is the .py file that is needed to build the cpp source code to object used for python extension.
To use this file, in the command line, change the directory to the one included in this file and type
'python setup.py build' or 'python3 setup.py build'
the .pyd or .so file are the compiled objects in the build directory
"""
from distutils.core import Extension, setup
import os
import platform

# Check the current platform and set the environment variables and compile arguments accordingly
if platform.system() == "Darwin":  # macOS
    os.environ["CC"] = "g++"
    os.environ["CXX"] = "g++"
    extra_compile_args = ["-std=c++20", "-O3"]  # change to -std=c++2a if -std=c++20 is not usable
elif platform.system() == "Windows":  # Windows
    extra_compile_args = ["/std:c++20", "/O2"]
else:  # Linux
    extra_compile_args = ["-std=c++20", "-O3"]  # change to -std=c++2a if -std=c++20 is not usable

module1 = Extension(
    "align4d",
    sources=["align4d_cpython_extension.cpp", "align.cpp", "msa.cpp", "postprocess.cpp", "preprocess.cpp"],
    extra_compile_args=extra_compile_args
)

setup(
    name="align4d",
    version="1.1.0",
    ext_modules=[
        module1,
    ],
    python_requires=">=3.8"
)
