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

### 3. Pre-commit installation

```bash
pre-commit install

pre-commit install --hook-type pre-push

# Test on all files once
pre-commit run --all-files --show-diff-on-failure --hook-stage manual
```

---

## Install the package

```bash
make install
```

## Install & Use C++ module

```bash
sudo make install-cpp
sudo ldconfig
```

uninstalltion:

```bash
sudo make uninstall-cpp
```

**Usage**:
Add this make file:

```makefile
# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I/usr/local/include
LDFLAGS := -L/usr/local/lib -laethermark

# Files
SRC := main.cpp
TARGET := main

.PHONY: all clean run

# Default target: build executable
all: $(TARGET)

# Compile and link
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)
```

Now use `make run`.

---

## Common Development Commands (via Makefile)

You can use the following `make` commands from the project root:

| **Command**                     | **Description**                                                                      |
| ------------------------------- | ------------------------------------------------------------------------------------ |
| `make all`                      | Builds the C++ static library (`libaethermark.a`).                                   |
| `make clean`                    | Removes all build artifacts, Python build outputs, and temporary files.              |
| `make venv`                     | Creates a Python virtual environment and installs development dependencies.          |
| `make activate`                 | Prints instructions for activating the virtual environment.                          |
| `make test`                     | Runs full test suite: C++ tests, Python tests, and MyPy static typecheck.            |
| `make test-cpp`                 | Runs all GoogleTest C++ unit tests.                                                  |
| `make test-py`                  | Runs all Python tests inside the `python/` directory using pytest.                   |
| `make test-py-static-typecheck` | Runs MyPy static typechecking on `python/aethermark`.                                |
| `make build`                    | Cleans and rebuilds the entire C++ library from scratch.                             |
| `make build-py`                 | Builds the Python package (sdist + wheel) in the `python/` directory.                |
| `make build-python-ext`         | Builds ONLY the pybind11 extension module (`_aethermark`) without packaging.         |
| `make release`                  | Builds and uploads the package to **PyPI** using Twine.                              |
| `make release-test`             | Builds and uploads the package to **TestPyPI** for testing.                          |
| `make install`                  | Installs the built Python package into the current virtual environment.              |
| `sudo make install-cpp`         | Installs the C++ library + headers into `/usr/local/lib` and `/usr/local/include`.   |
| `sudo make uninstall-cpp`       | Uninstalls the C++ library + headers from `/usr/local/lib` and `/usr/local/include`. |
| `make lint`                     | Runs all configured pre-commit hooks and auto-fixes issues.                          |
| `make debug`                    | Builds the project in Debug mode with symbols enabled.                               |
| `make play`                     | Builds + runs the C++ "playground" executable for manual experimentation.            |

**Test PyPI Installation**

```bash
pip install --index-url https://test.pypi.org/simple/ \
 --extra-index-url https://pypi.org/simple \
 aethermark==version
```

## Recommendations

For vs-code:

- `cpplint`
- `pylance`
- `yapf`

---

Thank you for contributing! If you find any issues or want to improve the project, feel free to open a pull request or an issue.
