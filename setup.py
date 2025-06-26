from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import os
import pybind11

class get_pybind_include:
    def __str__(self):
        return pybind11.get_include()

ext_modules = [
    Extension(
        'dbow2py',
        sources=['bindings/pybind_interface.cpp'],
        include_dirs=[
            get_pybind_include(),
            '/usr/include/opencv4',  # adjust if OpenCV is elsewhere
            '/usr/local/include',    # where DBoW2 is installed
        ],
        libraries=['DBoW2', 'opencv_core', 'opencv_features2d'],
        language='c++',
        extra_compile_args=['-std=c++17']
    ),
]

setup(
    name='dbow2py',
    version='0.1',
    author='David Esuga-Mopah',
    description='Python bindings for DBoW2 using pybind11',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    ext_modules=ext_modules,
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
)
