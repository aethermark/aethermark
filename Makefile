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
BUILD_PLAYGROUND ?= OFF
BUILD_PYTHON ?= OFF
BUILD_TESTS ?= OFF

.PHONY: all clean venv activate \
    test test-cpp test-cpp-debug test-py test-py-static-typecheck \
    build build-py build-python-ext \
    release release-test \
    install install-cpp uninstall-cpp \
    lint play debug run-cov

all: build

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
	cd $(1) && cmake $(2) $(PROJECT_ROOT)
	cd $(1) && cmake --build .
endef

# ----------------------------
# C++ Test targets
# ----------------------------
test-cpp:
	$(call cmake_build,$(BUILD_TEST)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON)
	cd $(BUILD_TEST)/release && ctest --output-on-failure

test-cpp-debug:
	$(call cmake_build,$(BUILD_TEST)/debug,-DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON)
	cd $(BUILD_TEST)/debug && ctest --output-on-failure

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
	$(call cmake_build,$(BUILD_REL)/release,-DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON=OFF -DBUILD_TESTS=OFF -DBUILD_PLAYGROUND=OFF)
	cd $(BUILD_REL)/release && cmake --build . --config Release --target install --prefix /usr/local

uninstall-cpp:
	@echo "Running CMake uninstall target..."
	cd $(BUILD_REL)/release && cmake --build . --target uninstall --config Release

# ----------------------------
# Release
# ----------------------------
release:
	$(MAKE) clean
	$(MAKE) build
	$(MAKE) build-py
	cd python && ../$(PYTHON) -m twine upload dist/*

release-test:
	$(MAKE) clean
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
