from enum import Enum
from typing import Any

"""
Aethermark - High-Performance Markdown Processing with Pybind11

Aethermark is a blazing-fast Markdown parser and renderer built with pybind11,
introducing Aethermark-Flavored Markdown (AFM) - a powerful extension to
CommonMark that combines rigorous standards compliance with enhanced features.

Key Features:
───────────────────────────────────────────────────────────────────────────────
• Optimized Performance: C++ core with Python bindings delivers 5-10x faster
  parsing than pure Python implementations
• AFM Dialect: Extends CommonMark with thoughtful syntax enhancements while
  maintaining full backward compatibility
• Extensible Architecture: Plugin system for custom syntax, renderers, and
  post-processors
• Precision Rendering: AST-based processing guarantees consistent,
  standards-compliant output
• Modern Tooling: Full type hints, comprehensive docs, and IDE support

Example Usage:
───────────────────────────────────────────────────────────────────────────────
>>> import aethermark
>>> md = aethermark.AFMParser()
>>> html = md.render("**AFM** adds _native_ syntax extensions!")
>>> print(html)
<p><strong>AFM</strong> adds <em>native</em> syntax extensions!</p>

Advanced Features:
───────────────────────────────────────────────────────────────────────────────
• Direct AST manipulation for programmatic document construction
• Source mapping for accurate error reporting
• Custom render targets (HTML, LaTeX, etc.)
• Thread-safe batch processing

Why Aethermark?
───────────────────────────────────────────────────────────────────────────────
For projects requiring:
• Scientific/technical documentation with complex markup needs
• High-volume Markdown processing
• Custom Markdown extensions without compromising performance
• Perfect round-trip parsing/render cycles

Learn more at: https://github.com/aethermark/aethermark
"""

class Nesting(Enum):
    """
    Represents the nesting state of a token in the Markdown document.

    Attributes:
        Open: `+1` means the tag is opening.
        Self: `0` means the tag is self-closing.
        Close: `-1` means the tag is closing.
    """

    Open: int
    Self: int
    Close: int

class Token:
    """Represents a single token in the Markdown document."""

    # ---------- Constructors ----------#
    def __init__(self, type: str, content: str, position: int) -> None:
        """
        Initialize a Token instance.

        :param type: The type of the token (e.g., 'text', 'heading').
        :param content: The content of the token.
        :param position: The position of the token in the source document.
        """

    # ---------- Properties ----------#
    type: str
    """Type of the token, e.g. "paragraph_open\""""

    tag: str
    """HTML tag name, e.g. "p\""""

    attrs: list[tuple[str, str]]
    """HTML attributes. Format: `[ [ name1, value1 ], [ name2, value2 ] ]`"""

    map: tuple[int, int] | None
    """Source map info. Format: `[ line_begin, line_end ]`"""

    nesting: Nesting
    """Level change of the token in the document."""

    level: int
    """Nesting level, the same as `state.level`"""

    children: list[Token] | None
    """An array of child nodes (inline and img tokens)"""

    content: str
    """In a case of self-closing tag (code, html, fence, etc.), it has contents of this tag."""

    markup: str
    """'*' or '_' for emphasis, fence string for fence, etc."""

    info: str
    """
    - Info string for "fence" tokens
    - The value "auto" for autolink "link_open" and "link_close" tokens
    - The string value of the item marker for ordered-list "list_item_open" tokens
    """

    meta: Any | None
    """A place for plugins to store an arbitrary data."""

    block: bool
    """
    True for block-level tokens, false for inline tokens.
    Used in renderer to calculate line breaks.
    """

    hidden: bool
    """If it's true, ignore this element when rendering. Used for tight lists to hide paragraphs."""

    # ---------- Methods ----------#
    def attrIndex(self, name: str) -> int:
        """
        Search attribute index by name.

        :param name: The name of the attribute.
        :return: Index of the attribute, or -1 if not found.
        """

    def attrPush(self, attrData: tuple[str, str]) -> None:
        """
        Add a new attribute.

        :param attrData: Tuple of (name, value).
        """

    def attrSet(self, name: str, value: str) -> None:
        """
        Set or update an attribute value.

        :param name: Attribute name.
        :param value: Attribute value.
        """

    def attrGet(self, name: str) -> str | None:
        """
        Get the value of an attribute by name.

        :param name: Attribute name.
        :return: The attribute value, or None if not found.
        """

    def attrJoin(self, name: str, value: str) -> None:
        """
        Append a value to an existing attribute, separated by a space.
        If the attribute does not exist, creates it.

        :param name: Attribute name.
        :param value: Value to append.
        """
