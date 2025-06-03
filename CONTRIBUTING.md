# Contributing to This Project

Thank you for your interest in contributing! This document guides you through setting up your development environment, running tests, linting, and building the package.

---

## Setup

### 1. Create a virtual environment

```bash
python3 -m venv .venv
```

### 2. Activate the virtual environment

- On Linux/macOS:

```bash
source .venv/bin/activate
```

- On Windows (PowerShell):

```bash
.venv\Scripts\Activate.ps1
```

---

## Installing Dependencies

### Install development dependencies

```bash
pip install -r requirements-dev.txt
```

### Install the package (editable mode) with test dependencies

```bash
pip install -e .[test]
```

---

## Common Development Commands (via Makefile)

You can use the following `make` commands from the project root:

| Command              | Description                                         |
| :------------------- | :-------------------------------------------------- |
| `make setup`         | Creates venv and installs all dependencies          |
| `make install`       | Installs package and test dependencies              |
| `make test`          | Runs tests with pytest                              |
| `make lint`          | Runs code linters (`ruff`)                          |
| `make coverage`      | Runs tests and shows coverage report                |
| `make coverage-html` | Runs tests and generates HTML coverage report       |
| `make build`         | Builds source and wheel distributions               |
| `make release`       | Cleans, builds, and uploads to PyPI                 |
| `make release-test`  | Cleans, builds, and uploads to TestPyPI             |
| `make clean`         | Cleans build artifacts and caches (keeps venv)      |
| `make clean-all`     | Cleans everything including the virtual environment |

---

## Testing

Run tests with coverage report:

```bash
make coverage
```

Or simply:

```bash
pytest
```

---

## Building and Publishing

### Build the package

```bash
make build
```

### Upload to PyPI

```bash
twine upload dist/*
```

### Upload to Test PyPI (for testing releases)

```bash
twine upload --repository testpypi dist/*.tar.gz
```

install using:

```bash
pip install --index-url https://test.pypi.org/simple --extra-index-url https://pypi.org/simple aethermark
```

---

## Linting

Run linters to check code style and quality:

```bash
make lint
```

or directly:

```bash
ruff check .
```

---

Thank you for contributing! If you find any issues or want to improve the project, feel free to open a pull request or an issue.
