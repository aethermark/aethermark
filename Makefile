BUILD_DIR := build
BUILD_PLAY := $(BUILD_DIR)/playground
BUILD_TEST := $(BUILD_DIR)/test
BUILD_PY   := $(BUILD_DIR)/python
BUILD_REL  := $(BUILD_DIR)/release
BUILD_DBG  := $(BUILD_DIR)/debug

VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
REQ_FILE := requirements-dev.txt

BUILD_TYPE ?= Release
BUILD_PLAYGROUND ?= ON
BUILD_PYTHON ?= ON
BUILD_TESTS ?= ON

.PHONY: all clean venv activate \
    test test-cpp test-cpp-debug test-py test-py-static-typecheck \
    build build-py build-python-ext \
    release release-test \
    install install-cpp uninstall-cpp \
    lint play debug run-cov

all: build

help:
	@echo ""
	@echo "Available commands:"
	@echo ""
	@echo "|  Command                        | Description                                                                          |"
	@echo "| ------------------------------- | ------------------------------------------------------------------------------------ |"
	@echo "| make all                        | Builds the C++ static library (\`libaethermark.a\`).                                   |"
	@echo "| make clean                      | Removes all build artifacts, Python build outputs, and temporary files.              |"
	@echo "| make venv                       | Creates a Python virtual environment and installs development dependencies.          |"
	@echo "| make activate                   | Prints instructions for activating the virtual environment.                          |"
	@echo "| make test                       | Runs full test suite: C++ tests, Python tests, and MyPy static typecheck.            |"
	@echo "| make test-cpp                   | Runs all GoogleTest C++ unit tests.                                                  |"
	@echo "| make test-py                    | Runs all Python tests inside the \`python/\` directory using pytest.                   |"
	@echo "| make test-py-static-typecheck   | Runs MyPy static typechecking on \`python/aethermark\`.                                |"
	@echo "| make build                      | Cleans and rebuilds the entire C++ library from scratch.                             |"
	@echo "| make build-py                   | Builds the Python package (sdist + wheel) in the \`python/\` directory.                |"
	@echo "| make build-python-ext           | Builds ONLY the pybind11 extension module (\`_aethermark\`) without packaging.         |"
	@echo "| make release                    | Builds and uploads the package to **PyPI** using Twine.                              |"
	@echo "| make release-test               | Builds and uploads the package to **TestPyPI** for testing.                          |"
	@echo "| make install                    | Installs the built Python package into the current virtual environment.              |"
	@echo "| sudo make install-cpp           | Installs the C++ library + headers into \`/usr/local/lib\` and \`/usr/local/include\`.   |"
	@echo "| sudo make uninstall-cpp         | Uninstalls the C++ library + headers from \`/usr/local/lib\` and \`/usr/local/include\`. |"
	@echo "| make lint                       | Runs all configured pre-commit hooks and auto-fixes issues.                          |"
	@echo "| make debug                      | Builds the project in Debug mode with symbols enabled.                               |"
	@echo "| make play                       | Runs the C++ \"playground\" executable for manual experimentation.                     |"

clean:
	@echo "Cleaning build and Python artifacts..."
	rm -rf python/dist python/*.egg-info python/aethermark/*.so
	rm -rf $(BUILD_DIR)

venv:
	python3 -m venv $(VENV_DIR)
	$(PYTHON) -m pip install --upgrade pip
	$(PYTHON) -m pip install -r $(REQ_FILE)

activate:
	@echo "Run: source $(VENV_DIR)/bin/activate"

PROJECT_ROOT := $(shell pwd)

define cmake_build
	@mkdir -p $(1)
	cd $(1) && cmake \
		--log-level=ERROR \
		--no-warn-unused-cli \
		-DCMAKE_MESSAGE_LOG_LEVEL=ERROR \
		-DCMAKE_RULE_MESSAGES=OFF \
		-DCMAKE_VERBOSE_MAKEFILE=OFF \
		$(2) $(PROJECT_ROOT)
	cd $(1) && cmake --build . -- -s
endef

# ----------------------------
# C++ Test targets
# ----------------------------
test-cpp:
	$(call cmake_build,$(BUILD_TEST)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON)
	cd $(BUILD_TEST)/release && ctest --progress --output-on-failure

test-cpp-debug:
	$(call cmake_build,$(BUILD_TEST)/debug,-DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON)
	cd $(BUILD_TEST)/debug && ctest --progress --output-on-failure

# ----------------------------
# Python build targets
# ----------------------------
build-python-ext:
	$(call cmake_build,$(BUILD_PY)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON=ON)
	@mkdir -p python/aethermark
	@cp $(BUILD_PY)/release/_aethermark*.so python/aethermark/_aethermark.so

build-py: build-python-ext
	cd python && ../$(PYTHON) -m build --sdist

test-py:
	$(call cmake_build,$(BUILD_PY)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON=ON)
	@mkdir -p python/aethermark
	@cp $(BUILD_PY)/release/_aethermark*.so python/aethermark/_aethermark.so
	@cd python && ../$(PYTHON) -m pytest -v

test-py-static-typecheck:
	$(PYTHON) -m mypy python/aethermark

# ----------------------------
# Combined test (C++ + Python)
# ----------------------------
test: test-cpp test-py test-py-static-typecheck

# ----------------------------
# Playground / debug / coverage
# ----------------------------
play:
	$(call cmake_build,$(BUILD_PLAY)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_PLAYGROUND=ON)
	./$(BUILD_PLAY)/release/playground ./playground/play.md

debug:
	$(call cmake_build,$(BUILD_PLAY)/debug,-DCMAKE_BUILD_TYPE=Debug -DBUILD_PLAYGROUND=ON)

run-cov:
	$(call cmake_build,$(BUILD_REL)/debug,-DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON)
	$(MAKE) -C $(BUILD_REL)/debug coverage

# ----------------------------
# Install / Uninstall
# ----------------------------
install:
	cd python && ../$(PYTHON) -m pip install --force-reinstall ./dist/aethermark-*.tar.gz

install-cpp:
	$(call cmake_build,$(BUILD_REL)/release, \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=/usr/local \
		-DBUILD_PYTHON=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_PLAYGROUND=OFF)
	cd $(BUILD_REL)/release && cmake --build . --target install

uninstall-cpp:
	@echo "Running CMake uninstall target..."
	cd $(BUILD_REL)/release && cmake --build . --target uninstall --config Release

# ----------------------------
# Release
# ----------------------------
release:
	@echo "Uploading to PyPI..."
	rm -rf $(BUILD_REL)
	$(MAKE) build
	$(MAKE) build-py
	cd python && ../$(PYTHON) -m twine upload dist/*

release-test:
	@echo "Uploading to TestPyPI..."
	rm -rf $(BUILD_REL)
	$(MAKE) build
	$(MAKE) build-py
	cd python && ../$(PYTHON) -m twine upload --repository testpypi dist/* --verbose

# ----------------------------
# Lint
# ----------------------------
lint:
	$(PYTHON) -m pre_commit run --all-files --show-diff-on-failure --hook-stage manual --verbose

# ----------------------------
# Original build target (kept for all)
# ----------------------------
build:
	$(call cmake_build,$(BUILD_DIR)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON=$(BUILD_PYTHON) -DBUILD_TESTS=$(BUILD_TESTS) -DBUILD_PLAYGROUND=$(BUILD_PLAYGROUND))
