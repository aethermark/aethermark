from enum import Enum
from typing import Any, Callable

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
        OPEN: `+1` means the tag is opening.
        SELF: `0` means the tag is self-closing.
        CLOSE: `-1` means the tag is closing.
    """

    OPEN: int
    SELF: int
    CLOSE: int

class Token:
    """Represents a single token in the Markdown document."""

    # ---------- Constructors ---------- #
    def __init__(self, type: str, tag: str, nesting: Nesting) -> None:
        """
        Initialize a Token instance.

        :param type: Type of the token, e.g. "paragraph_open\".
        :param tag: HTML tag name, e.g. "p\".
        :param nesting: Level change of the token in the document.
        """

    # ---------- Properties ---------- #
    type: str
    """Type of the token, e.g. "paragraph_open\"."""

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

    # ---------- Methods ---------- #
    def attr_index(self, name: str) -> int:
        """
        Search attribute index by name.

        :param name: The name of the attribute.
        :return: Index of the attribute, or -1 if not found.
        """

    def attr_push(self, attrData: tuple[str, str]) -> None:
        """
        Add a new attribute.

        :param attrData: Tuple of (name, value).
        """

    def attr_set(self, name: str, value: str) -> None:
        """
        Set or update an attribute value.

        :param name: Attribute name.
        :param value: Attribute value.
        """

    def attr_get(self, name: str) -> str | None:
        """
        Get the value of an attribute by name.

        :param name: Attribute name.
        :return: The attribute value, or None if not found.
        """

    def attr_join(self, name: str, value: str) -> None:
        """
        Append a value to an existing attribute, separated by a space.
        If the attribute does not exist, creates it.

        :param name: Attribute name.
        :param value: Value to append.
        """

class RuleOptions:
    """
    Options for configuring rules in the Ruler.
    """

    # ---------- Properties ---------- #
    alt: list[str]
    """Array with names of "alternate" chains."""

class Ruler:
    """
    Helper class, used by core, block and inline to manage sequences of functions (rules):
    - keep rules in defined order
    - assign the name to each rule
    - enable/disable rules
    - add/replace rules
    - allow assign rules to additional named chains (in the same)
    - caching lists of active rules
    """

    # ---------- Constructors ---------- #
    def __init__(self) -> None:
        """Initialize a Ruler instance."""

    # ---------- Methods ---------- #
    def push(
        self, name: str, fn: Callable[[object], None], options: RuleOptions | None = ...
    ) -> None:
        """
        Push new rule to the end of chain.

        :param name: name of added rule.
        :param fn: rule function.
        :param options: rule options (not mandatory).
        """

    def at(
        self, name: str, fn: Callable[[object], None], options: RuleOptions | None = ...
    ) -> None:
        """
        Replace rule by name with new function & options. Throws error if name not found.

        :param name: rule name to replace.
        :param fn: new rule function.
        :param options: new rule options (not mandatory).
        """

    def before(
        self,
        before_name: str,
        name: str,
        fn: Callable[[object], None],
        options: RuleOptions | None = ...,
    ) -> None:
        """
        Add new rule to chain before one with given name.

        :params before_name: new rule will be added before this one.
        :params name: name of added rule.
        :params fn: rule function.
        :params options: rule options (not mandatory).
        """

    def after(
        self,
        after_name: str,
        name: str,
        fn: Callable[[object], None],
        options: RuleOptions | None = ...,
    ) -> None:
        """
        Add new rule to chain after one with given name.

        :params after_name: new rule will be added after this one.
        :parmas name: name of added rule.
        :params fn: rule function.
        :parmas options: rule options (not mandatory).
        """

    def enable(self, list: list[str], ignore_invalid: bool = ...) -> list[str]:
        """
        Enable rules with given names. If any rule name not found - throw Error. Errors can be disabled by second param.

        :params list: list of rule names to enable.
        :params ignore_invalid: set `true` to ignore errors when rule not found.
        :returns: list of found rule names (if no exception happened).
        """

    def disable(self, list: list[str], ignore_invalid: bool = ...) -> list[str]:
        """
        Disable rules with given names. If any rule name not found - throw Error. Errors can be disabled by second param.

        :params list: list of rule names to enable.
        :params ignore_invalid: set `true` to ignore errors when rule not found.
        :returns: Returns list of found rule names (if no exception happened).
        """

    def enable_only(self, list: list[str], ignore_invalid: bool = ...) -> None:
        """
        Enable rules with given names, and disable everything else. If any rule name is not found - throws Error. Errors can be disabled by second param.

        :params list: list of rule names to enable (whitelist).
        :params ignore_invalid: set `true` to ignore errors when rule not found.
        """

    def get_rules(self, chain_name: str) -> list[Callable[[object], None]]:
        """
        It analyzes rules configuration, compiles caches if not exists and returns result.

        :params chain_name: name of the chain from which rules are to be requested
        :returns: array of active functions (rules) for given chain name.
        """
