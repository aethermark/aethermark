![Aethermark](/favicon.png)

# [Aethermark](https://aethermark.dev)

[![Unit Tests](https://github.com/MukulWaval/aethermark/actions/workflows/test.yml/badge.svg)](https://github.com/MukulWaval/aethermark/actions/workflows/test.yml)
![PyPI](https://img.shields.io/pypi/v/aethermark)

Aethermark is a high-performance, extensible Markdown parser and renderer built with `pybind11`. It introduces **Aethermark-Flavored Markdown (AFM)**, a custom dialect that enhances standard Markdown with additional features and improved rendering capabilities.

## Features

- **Optimized Performance**: Leverages `pybind11` for efficient execution.
- **Custom Dialect (AFM)**: Extends Markdown with additional syntax and enhancements.
- **Extensible**: Easily integrates with other Python projects and allows custom extensions.
- **Accurate Rendering**: Provides precise and consistent Markdown output.

## Installation

You can install Aethermark directly from PyPI:

```sh
pip install aethermark
```

## Usage

Aethermark provides a simple API to parse and render Markdown:

```
import aethermark

md_text = """

# Hello, Aethermark!

This is an example of **Aethermark-Flavored Markdown (AFM)**.
"""

html_output = aethermark.render(md_text)
print(html_output)

```

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests on [GitHub](https://github.com/MukulWaval/aethermark).

## License

Aethermark is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
