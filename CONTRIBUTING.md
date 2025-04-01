# Contributing to Aethermark

Thank you for your interest in contributing to Aethermark! We welcome contributions of all kinds, including bug reports, feature requests, documentation improvements, and code contributions.

## Getting Started

1. **Fork the Repository**: Click the “Fork” button on the [GitHub repository](https://github.com/MukulWaval/aethermark) to create your own copy.
2. **Clone the Fork**: Clone your fork to your local machine:
   ```sh
   git clone https://github.com/MukulWaval/aethermark.git
   cd aethermark
   ```
3. **Create a Branch**: Create a new branch for your feature or fix:
   ```sh
   git checkout -b my-feature-branch
   ```
4. **Install Dependencies**:
   ```sh
   pip install -r requirements.txt
   ```
5. **Make Your Changes**: Implement your changes and ensure everything is working as expected.
6. **Run Tests**:
   ```sh
   pytest
   ```
7. **Commit Your Changes**:
   ```sh
   git add .
   git commit -m "Describe your changes"
   ```
8. **Push to Your Fork**:
   ```sh
   git push origin my-feature-branch
   ```
9. **Open a Pull Request**: Submit a pull request (PR) to the `main` branch on the official repository. Provide a clear description of your changes.

## Code Guidelines

- Follow Python’s PEP8 style guide.
- Write clear, concise, and well-documented code.
- Ensure backward compatibility where possible.
- Add tests for new features or bug fixes.

## Reporting Issues

If you encounter a bug or have a feature request, please open an issue on [GitHub](https://github.com/MukulWaval/aethermark/issues) with detailed information.

## Notes

1. Make virtual environment using:
   ```bash
   python3 -m venv .venv
   source .venv/bin/activate
   ```
2. Install dependencies using:
   ```bash
   python3 -m pip install -r requirements.txt
   ```
3. Build using:
   ```bash
   python3 -m build
   ```
4. Upload you build to test pypi using:
   ```bash
   python3 -m twine upload --repository testpypi dist/aethermark-*.tar.gz  --verbose
   ```
5. Download the test package from pypi using:
   ```bash
   pip install -i https://test.pypi.org/simple/ --extra-index-url https://pypi.org/simple/ aethermark --force-reinstall
   ```
6. Test using:
   ```bash
   pytest
   ```
   or
   ```bash
   python3 tests/test.py
   ```

## Licensing

By contributing, you agree that your contributions will be licensed under the project's MIT License.

---

We appreciate your contributions and look forward to working with you!
