# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := build
TEST_DIR := tests
LIB_NAME := libquantamark.a
PYTHON_DIR := python
PYTHON_DIST := $(PYTHON_DIR)/dist

# Virtual environment setup
VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
PIP := $(PYTHON) -m pip
PYTEST := $(PYTHON) -m pytest

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
REQ_FILE := requirements-dev.txt

# Phony targets
.PHONY: all clean check build venv activate pytest python-build python-upload-test python-upload python-install test

# Default target
all: $(LIB_NAME)

# Build static library
$(LIB_NAME): $(OBJS)
	ar rcs $@ $^

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: check pytest

# Google Test runner
check: $(LIB_NAME)
	@echo "🧪 Running Google Tests..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/*.cpp $(LIB_NAME) -lgtest -lpthread -o $(OBJ_DIR)/tests
	./$(OBJ_DIR)/tests

# Rebuild everything
build: clean all

# Virtual environment setup
venv:
	@echo "🐍 Creating virtual environment..."
	python3 -m venv $(VENV_DIR)
	@echo "📦 Installing dependencies from $(REQ_FILE)..."
	$(VENV_DIR)/bin/python -m pip install --upgrade pip
	@if [ -f $(REQ_FILE) ]; then \
		$(VENV_DIR)/bin/python -m pip install -r $(REQ_FILE); \
	else \
		echo "⚠️  $(REQ_FILE) not found. Installing minimal base deps..."; \
		$(VENV_DIR)/bin/python -m pip install setuptools wheel build twine pytest; \
	fi
	@echo "✅ Virtual environment ready! Activate with: source $(VENV_DIR)/bin/activate"

# Activation helper message
activate:
	@echo "To activate the environment, run:"
	@echo "source $(VENV_DIR)/bin/activate"

# Run Python tests
pytest: $(VENV_DIR)/bin/python
	@echo "🧪 Running Python tests..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m pytest -v

# Build Python package
python-build: $(VENV_DIR)/bin/python
	@echo "📦 Preparing source for build..."
	@cp -r src $(PYTHON_DIR)/src
	@cp -r include $(PYTHON_DIR)/include
	@echo "📦 Building Python package (in venv)..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m build --sdist
	@echo "🧹 Cleaning up copied src/include folders..."
	rm -rf $(PYTHON_DIR)/src
	rm -rf $(PYTHON_DIR)/include

# Upload to Test PyPI
python-upload-test: python-build
	@echo "🚀 Uploading package to Test PyPI..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m twine upload --repository testpypi dist/* --verbose

# Upload to PyPI
python-upload: $(VENV_DIR)/bin/python python-build
	@echo "🚀 Uploading package to PyPI..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m twine upload dist/*

# Install the built Python package locally
python-install: python-build
	@echo "📥 Installing Python package locally..."
	cd $(PYTHON_DIR) && ../$(VENV_DIR)/bin/python -m pip install --force-reinstall ./dist/quantamark-*.tar.gz

# Clean all build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -rf $(OBJ_DIR) $(LIB_NAME) $(PYTHON_DIST)
	rm -rf $(PYTHON_DIR)/dist
	rm -rf $(PYTHON_DIR)/*.egg-info
