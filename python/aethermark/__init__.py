# pylint: disable=no-name-in-module,import-error
"""
Aethermark - High-performance Markdown parser with AFM extensions.
Extends CommonMark with enhanced syntax while remaining backward-compatible.

Example:
  >>> import aethermark
  >>> md = aethermark.AFMParser()
  >>> html = md.render("**AFM** adds _native_ syntax!")
  >>> print(html)
  <p><strong>AFM</strong> adds <em>native</em> syntax!</p>

See: https://github.com/aethermark/aethermark
"""
from _aethermark import greet  # noqa: F401

__all__ = ["greet"]
__version__ = "0.0.20"
