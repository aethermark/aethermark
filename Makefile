# ==========================
# Paths
# ==========================
BUILD_DIR := build
VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
REQ_FILE := requirements-dev.txt

# CMake build type
BUILD_TYPE ?= Release
BUILD_PLAYGROUND ?= OFF
BUILD_PYTHON ?= OFF
BUILD_TESTS ?= OFF

# Targets
.PHONY: all clean venv activate build test test-cpp test-py test-py-static-typecheck \
        build-py release release-test install install-cpp uninstall-cpp \
        lint play debug build-python-ext

# ==========================
# Default
# ==========================
all: build

# ==========================
# C++ / CMake
# ==========================
build:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DBUILD_PYTHON=$(BUILD_PYTHON) \
		-DBUILD_TESTS=$(BUILD_TESTS) \
		-DBUILD_PLAYGROUND=$(BUILD_PLAYGROUND) \
		..
	cd $(BUILD_DIR) && cmake --build . --config $(BUILD_TYPE)

debug:
	$(MAKE) clean
	$(MAKE) BUILD_TYPE=Debug BUILD_PLAYGROUND=ON build

test-cpp:
	$(MAKE) clean
	$(MAKE) BUILD_TESTS=ON build
	cd $(BUILD_DIR) && ctest --output-on-failure


install-cpp:
	$(MAKE) clean
	$(MAKE) build
	cd $(BUILD_DIR) && cmake --install . --prefix /usr/local

uninstall-cpp:
	@echo "Running CMake uninstall target..."
	@cd $(BUILD_DIR) && cmake --build . --target uninstall

play:
	$(MAKE) clean
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_PLAYGROUND=ON \
		..
	cd $(BUILD_DIR) && cmake --build . --config Debug
	cd $(BUILD_DIR) && ./playground

# ==========================
# Python / venv
# ==========================
venv:
	python3 -m venv $(VENV_DIR)
	$(PYTHON) -m pip install --upgrade pip
	@if [ -f $(REQ_FILE) ]; then \
		$(PYTHON) -m pip install -r $(REQ_FILE); \
	else \
		$(PYTHON) -m pip install setuptools wheel build twine pytest pre-commit mypy; \
	fi

activate:
	@echo "Run: source $(VENV_DIR)/bin/activate"

# ==========================
# Python Bindings (from CMake)
# ==========================
build-python-ext:
	$(MAKE) BUILD_PYTHON=ON build
	@mkdir -p python/aethermark
	@cp $(BUILD_DIR)/aethermark_py*.so python/aethermark/_aethermark.so

test-py:
	cd python && ../$(PYTHON) -m pytest -v

test-py-static-typecheck:
	$(PYTHON) -m mypy python/aethermark

# ==========================
# Python Packaging
# ==========================
build-py: build-python-ext
	cd python && ../$(PYTHON) -m build --sdist

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

install:
	cd python && ../$(PYTHON) -m pip install --force-reinstall ./dist/aethermark-*.tar.gz

# ==========================
# Lint
# ==========================
lint:
	$(PYTHON) -m pre_commit run --all-files --show-diff-on-failure --hook-stage manual --verbose

# ==========================
# Clean
# ==========================
clean:
	@echo "Cleaning build and Python artifacts..."
	rm -rf python/dist python/*.egg-info python/aethermark/*.so
	rm -rf $(BUILD_DIR)
