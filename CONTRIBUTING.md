# Contributing to This Project

Thank you for your interest in contributing! This document guides you through setting up your development environment, running tests, linting, and building the package.

---

## Setup

### 1. Create a virtual environment

```bash
make venv
```

### 2. Activate the virtual environment

```bash
make activate
```

### 3. GTest Installtion

```bash
sudo apt update
sudo apt install -y g++ make python3-dev python3-venv \
 libgtest-dev cmake
cd /usr/src/googletest
sudo cmake .
sudo make
sudo cp lib/\*.a /usr/lib
```

### 4. Pre-commit installation

```bash
pre-commit install
```

---

## Install the package

```bash
make install
```

---

## Common Development Commands (via Makefile)

You can use the following `make` commands from the project root:

| **Command**                     | **Description**                                                             |
| :------------------------------ | :-------------------------------------------------------------------------- |
| `make all`                      | Builds the C++ static library (`libaethermark.a`).                          |
| `make clean`                    | Removes all build artifacts, Python dists, and temporary files.             |
| `make venv`                     | Creates a Python virtual environment and installs development dependencies. |
| `make activate`                 | Prints instructions to activate the virtual environment.                    |
| `make test`                     | Runs C++, Python, Static Typecheck test suites.                             |
| `make test-cpp`                 | Runs all GoogleTest-based C++ unit tests.                                   |
| `make test-py`                  | Runs all Python tests inside the `python/` directory with pytest.           |
| `make test-py-static-typecheck` | Runs MyPy static type checking on `python/aethermark`.                      |
| `make build`                    | Cleans and rebuilds the C++ library from scratch.                           |
| `make build-py`                 | Builds the Python package (sdist) inside `python/` using `build`.           |
| `make release`                  | Builds and uploads the package to **PyPI** using Twine.                     |
| `make release-test`             | Builds and uploads the package to **TestPyPI** for testing.                 |
| `make install`                  | Installs the built Python package locally into the virtual environment.     |
| `make lint`                     | Runs all configured **pre-commit** hooks and auto-fixes issues.             |

**Test PyPI Installation**

```bash
pip install --index-url https://test.pypi.org/simple/ \
 --extra-index-url https://pypi.org/simple \
 aethermark==version
```

---

Thank you for contributing! If you find any issues or want to improve the project, feel free to open a pull request or an issue.
