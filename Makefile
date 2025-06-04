VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
PIP := $(VENV_DIR)/bin/pip

ifeq ($(OS),Windows_NT)
    PYTHON := $(VENV_DIR)/Scripts/python.exe
    PIP := $(VENV_DIR)/Scripts/pip.exe
endif

setup:
    python3 -m venv $(VENV_DIR)
    $(PIP) install --upgrade pip
    $(PIP) install -e .[test]
    $(PIP) install -r requirements-dev.txt
    $(PYTHON) -m pre-commit install

install:
    $(PIP) install -e .[test]
    $(PIP) install -r requirements-dev.txt

test:
    $(PYTHON) -m pytest

lint:
ifeq ($(OS),Windows_NT)
    $(VENV_DIR)/Scripts/ruff check . --fix --show-fixes
else
    $(VENV_DIR)/bin/ruff check . --fix --show-fixes
endif

build:
    $(PYTHON) -m cibuildwheel --output-dir dist

coverage:
    $(PYTHON) -m pytest --cov=aethermark --cov-report=term-missing

coverage-html:
    $(PYTHON) -m pytest --cov=aethermark --cov-report=html

clean:
    rm -rf build/ dist/ *.egg-info htmlcov .pytest_cache .tox .ruff_cache

clean-all: clean
    rm -rf $(VENV_DIR)

release: clean build upload

upload:
    $(PYTHON) -m twine upload dist/*

release-test: clean build upload-test

upload-test:
    $(PYTHON) -m twine upload --repository testpypi dist/*
