# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
CXXFLAGS_DEBUG := -std=c++17 -Wall -Wextra -g -Iinclude -O0

# Directories
SRC_DIR := src
OBJ_DIR := build
TEST_DIR := tests
LIB_NAME := libaethermark.a
PYTHON_DIR := python
PYTHON_DIST := $(PYTHON_DIR)/dist

# Virtual environment setup
VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
PIP := $(PYTHON) -m pip
PYTEST := $(PYTHON) -m pytest

# Recursive wildcard
rwildcard = $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2)) $(filter $(subst *,%,$2),$1)

# Source and object files
SRCS := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
REQ_FILE := requirements-dev.txt

PREFIX ?= /usr/local

PLAY_SRC = playground/main.cpp
PLAY_BIN = playground/play

# Phony targets
.PHONY: all clean venv activate test test-cpp test-py test-py-static-typecheck build \
        build-py release release-test install install-cpp uninstall-cpp lint play debug

# Default target
all: $(LIB_NAME)

# Build static library
$(LIB_NAME): $(OBJS)
	ar rcs $@ $^

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Alias for debug scripts
lib: $(LIB_NAME)

# Clean all build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJ_DIR) $(LIB_NAME) $(PYTHON_DIST)
	rm -rf $(PYTHON_DIR)/dist
	rm -rf $(PYTHON_DIR)/*.egg-info

# Virtual environment setup
venv:
	@echo "Creating virtual environment..."
	python3 -m venv $(VENV_DIR)
	@echo "Installing dependencies from $(REQ_FILE)..."
	$(VENV_DIR)/bin/python -m pip install --upgrade pip
	@if [ -f $(REQ_FILE) ]; then \
		$(VENV_DIR)/bin/python -m pip install -r $(REQ_FILE); \
	else \
		echo "$(REQ_FILE) not found. Installing minimal base deps..."; \
		$(VENV_DIR)/bin/python -m pip install setuptools wheel build twine pytest; \
	fi
	@echo "Virtual environment ready! Activate with: source $(VENV_DIR)/bin/activate"

# Activation helper message
activate:
	@echo "To activate the environment, run:"
	@echo "source $(VENV_DIR)/bin/activate"

# Run all tests
test: test-cpp test-py test-py-static-typecheck

# Google Test runner
test-cpp: $(LIB_NAME)
	@echo "Running Google Tests..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/*.cpp $(LIB_NAME) -lgtest -lpthread -o $(OBJ_DIR)/tests
	./$(OBJ_DIR)/tests

# Run Python tests
test-py: $(VENV_DIR)/bin/python
	@echo "Running Python tests..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m pytest -v

test-py-static-typecheck:
	@echo "Running mypy type checking..."
	@if [ -d .venv ]; then \
		. .venv/bin/activate && mypy python/aethermark; \
	else \
		mypy python/aethermark; \
	fi

# Rebuild everything
build: clean all

# Build Python package
build-py: $(VENV_DIR)/bin/python
	@echo "Preparing source for build..."
	@cp -r src $(PYTHON_DIR)/src
	@cp -r include $(PYTHON_DIR)/include
	@echo "Building Python package (in venv)..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m build --sdist
	@echo "Cleaning up copied src/include folders..."
	rm -rf $(PYTHON_DIR)/src
	rm -rf $(PYTHON_DIR)/include

# Upload to PyPI
release: $(VENV_DIR)/bin/python build-py
	@echo "Uploading package to PyPI..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m twine upload dist/*

# Upload to Test PyPI
release-test: build-py
	@echo "Uploading package to Test PyPI..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m twine upload --repository testpypi dist/* --verbose

# Install the built Python package locally
install:
	@echo "Cleaning old builds..."
	rm -rf python/dist/*
	make build-py
	@echo "Installing Python package locally..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m pip install --force-reinstall ./dist/aethermark-*.tar.gz

# Install C++ library and headers
install-cpp: all
	@echo "Installing C++ library to $(PREFIX)..."
	@mkdir -p $(PREFIX)/lib
	@mkdir -p $(PREFIX)/include/aethermark
	@cp $(LIB_NAME) $(PREFIX)/lib/
	@cp -r include/aethermark/* $(PREFIX)/include/aethermark/
	@echo "Installed headers to $(PREFIX)/include/aethermark and library to $(PREFIX)/lib"
	@echo "You may need to run: sudo ldconfig"

# Uninstall C++ library and headers
uninstall-cpp:
	@echo "Removing installed library from $(PREFIX)..."
	@rm -f $(PREFIX)/lib/$(LIB_NAME)
	@rm -rf $(PREFIX)/include/aethermark
	@echo "Uninstalled C++ library"

# Run pre-commit hooks and auto-fix issues
lint: $(VENV_DIR)/bin/python
	@echo "Running pre-commit hooks with auto-fix..."
	@$(VENV_DIR)/bin/python -m pre_commit run --all-files --show-diff-on-failure --hook-stage manual --verbose

# Run playground binary
play:
	$(CXX) $(CXXFLAGS) -Iinclude $(PLAY_SRC) \
		-L. -laethermark \
		-o $(PLAY_BIN)
	@echo "Run: ./$(PLAY_BIN)"

# Full debug rebuild
debug: clean
	$(MAKE) CXXFLAGS="$(CXXFLAGS_DEBUG)" lib
	g++ $(CXXFLAGS_DEBUG) playground/main.cpp \
		-L. -laethermark \
		-o playground/play
