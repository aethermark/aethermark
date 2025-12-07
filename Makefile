BUILD_DIR := build
VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
REQ_FILE := requirements-dev.txt

BUILD_TYPE ?= Release
BUILD_PLAYGROUND ?= OFF
BUILD_PYTHON ?= OFF
BUILD_TESTS ?= OFF

.PHONY: all clean venv activate \
		test test-cpp test-py test-py-static-typecheck \
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

test:
	$(MAKE) clean
	$(MAKE) BUILD_TESTS=ON BUILD_PYTHON=ON build
	@mkdir -p python/aethermark
	@cp $(BUILD_DIR)/_aethermark*.so python/aethermark/_aethermark.so
	@cd python && ../$(PYTHON) -m build --sdist
	@cd python && ../$(PYTHON) -m pip install --force-reinstall ./dist/aethermark-*.tar.gz
	@cd python && ../$(PYTHON) -m pytest -v
	@cd $(BUILD_DIR) && ctest --output-on-failure
	$(MAKE) test-py-static-typecheck

test-cpp:
	$(MAKE) clean
	$(MAKE) BUILD_TESTS=ON build
	cd $(BUILD_DIR) && ctest --output-on-failure

test-py:
	$(MAKE) clean
	$(MAKE) build-py
	@cd python && ../$(PYTHON) -m pytest -v

test-py-static-typecheck:
	$(PYTHON) -m mypy python/aethermark

build:
	$(MAKE) clean
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DBUILD_PYTHON=$(BUILD_PYTHON) \
		-DBUILD_TESTS=$(BUILD_TESTS) \
		-DBUILD_PLAYGROUND=$(BUILD_PLAYGROUND) \
		..
	cd $(BUILD_DIR) && cmake --build . --config $(BUILD_TYPE)

build-py: build-python-ext
	cd python && ../$(PYTHON) -m build --sdist

build-python-ext:
	$(MAKE) BUILD_PYTHON=ON build
	@mkdir -p python/aethermark
	@cp $(BUILD_DIR)/_aethermark*.so python/aethermark/_aethermark.so

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

install-cpp:
	$(MAKE) clean
	$(MAKE) build
	cd $(BUILD_DIR) && cmake --install . --prefix /usr/local

uninstall-cpp:
	@echo "Running CMake uninstall target..."
	@cd $(BUILD_DIR) && cmake --build . --target uninstall

lint:
	$(PYTHON) -m pre_commit run --all-files --show-diff-on-failure --hook-stage manual --verbose

play:
	./build/playground ./playground/play.md

debug:
	$(MAKE) clean
	$(MAKE) BUILD_TYPE=Debug BUILD_PLAYGROUND=ON build

run-cov:
	$(MAKE) clean
	cmake -B build -DENABLE_COVERAGE=ON
	cmake --build build
	$(MAKE) -C build coverage
