from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "_quantamark",
        sources=[
            "bindings/quantamark_py.cpp",
            "src/quantamark.cpp",  # include your main library
        ],
        include_dirs=[
            "include",
            pybind11.get_include(),
        ],
        language="c++",
        extra_compile_args=["-std=c++17"],
    ),
]

setup(
    name="quantamark",
    version="0.1.4",
    ext_modules=ext_modules,
)
