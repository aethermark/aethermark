# ==========================
# Paths
# ==========================
BUILD_DIR := build
VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
REQ_FILE := requirements-dev.txt

# CMake build type
BUILD_TYPE ?= Release

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
		-DBUILD_PYTHON=ON \
		-DBUILD_TESTS=ON \
		-DBUILD_PLAYGROUND=OFF \
		..
	cd $(BUILD_DIR) && cmake --build . --config $(BUILD_TYPE)

debug:
	$(MAKE) BUILD_TYPE=Debug build

test-cpp:
	cd $(BUILD_DIR) && ctest --output-on-failure

install-cpp:
	cd $(BUILD_DIR) && cmake --install . --prefix /usr/local

uninstall-cpp:
	@echo "Running CMake uninstall target..."
	@cd $(BUILD_DIR) && cmake --build . --target uninstall

play:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_PLAYGROUND=ON ..
	@cd $(BUILD_DIR) && cmake --build . --config Debug
	@cd $(BUILD_DIR) && ./playground

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
	$(MAKE) build
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
	cd python && ../$(PYTHON) -m twine upload dist/*

release-test:
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
	rm -rf $(BUILD_DIR)/playground $(BUILD_DIR)/unit_tests $(BUILD_DIR)/libaethermark.a $(BUILD_DIR)/aethermark_py*
