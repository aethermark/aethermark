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


def greet(name: str) -> str:
    """
    Greet someone by name.

    Args:
        name (str): Name of the person.

    Returns:
        str: Greeting string.
    """
    ...
