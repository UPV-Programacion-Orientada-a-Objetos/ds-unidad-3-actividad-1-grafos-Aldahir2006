from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    name="NeuroNet",
    ext_modules=cythonize([
        Extension("neuronet_core", 
                 sources=["wrapper/bridge.pyx", "src/NeuroNet.cpp"],
                 include_dirs=["src"],
                 language="c++",
                 extra_compile_args=["/Ox", "/EHsc"])
    ])
)
