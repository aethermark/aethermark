from __future__ import annotations

from aethermark import Nesting, Token

# ------------------------
# Constructor
# ------------------------


def test_constructor_initializes_core_fields():
    token = Token(type="inline", tag="em", nesting=Nesting.OPEN)
    assert token.type == "inline"
    assert token.tag == "em"
    assert token.nesting == Nesting.OPEN


def test_defaults_other_fields_null_or_empty():
    token = Token(type="inline", tag="em", nesting=Nesting.OPEN)
    assert token.attrs == []
    assert token.map is None
    assert token.level == 0
    assert token.children is None
    assert token.content == ""
    assert token.markup == ""
    assert token.info == ""
    assert token.meta is None
    assert token.block is False
    assert token.hidden is False


# ------------------------
# Attribute Index
# ------------------------


def test_attr_index_returns_correct_index_for_existing_attribute():
    token = Token("inline", "a", Nesting.SELF)
    token.attrs = [("href", "https://example.com"), ("title", "Example")]
    assert token.attr_index("href") == 0
    assert token.attr_index("title") == 1


def test_attr_index_returns_minus_one_for_nonexistent_attribute():
    token = Token("inline", "a", Nesting.SELF)
    token.attrs = [("href", "https://example.com")]
    assert token.attr_index("title") == -1


def test_attr_index_empty_attrs_returns_minus_one():
    token = Token("inline", "a", Nesting.SELF)
    assert token.attr_index("href") == -1


# ------------------------
# Attribute Push
# ------------------------


def test_attr_push_adds_new_attribute():
    token = Token("inline", "a", Nesting.SELF)
    token.attr_push(("href", "https://example.com"))
    assert token.attrs == [("href", "https://example.com")]


def test_attr_push_appends_multiple_attributes():
    token = Token("inline", "a", Nesting.SELF)
    token.attr_push(("href", "https://example.com"))
    token.attr_push(("title", "Example"))
    assert token.attrs == [("href", "https://example.com"), ("title", "Example")]


# ------------------------
# Attribute Set
# ------------------------


def test_attr_set_adds_new_attribute_if_not_exists():
    token = Token("inline", "a", Nesting.SELF)
    token.attr_set("href", "https://example.com")
    assert token.attrs == [("href", "https://example.com")]


def test_attr_set_overwrites_existing_attribute_value():
    token = Token("inline", "a", Nesting.SELF)
    token.attrs = [("href", "https://old.com")]
    token.attr_set("href", "https://new.com")
    assert token.attrs == [("href", "https://new.com")]


# ------------------------
# Attribute Get
# ------------------------


def test_attr_get_returns_correct_value_if_attribute_exists():
    token = Token("inline", "a", Nesting.SELF)
    token.attrs = [("href", "https://example.com")]
    assert token.attr_get("href") == "https://example.com"


def test_attr_get_returns_none_if_attribute_missing():
    token = Token("inline", "a", Nesting.SELF)
    assert token.attr_get("title") is None


# ------------------------
# Attribute Join
# ------------------------


def test_attr_join_adds_attribute_if_not_exists():
    token = Token("inline", "span", Nesting.SELF)
    token.attr_join("class", "first")
    assert token.attrs == [("class", "first")]


def test_attr_join_appends_value_with_space_if_exists():
    token = Token("inline", "span", Nesting.SELF)
    token.attrs = [("class", "first")]
    token.attr_join("class", "second")
    assert token.attr_get("class") == "first second"


def test_attr_join_preserves_order_and_spacing_on_multiple_joins():
    token = Token("inline", "span", Nesting.SELF)
    token.attr_join("class", "first")
    token.attr_join("class", "second")
    token.attr_join("class", "third")
    assert token.attr_get("class") == "first second third"


def test_attr_set_then_get_returns_updated_value():
    token = Token("inline", "img", Nesting.SELF)
    token.attr_set("alt", "old")
    token.attr_set("alt", "new")
    assert token.attr_get("alt") == "new"
