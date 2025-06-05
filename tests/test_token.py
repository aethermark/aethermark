from __future__ import annotations

import pytest

from aethermark import Nesting, Token


def test_constructor_defaults():
    t = Token("paragraph_open", "p", Nesting.Open)
    assert t.type == "paragraph_open"
    assert t.tag == "p"
    assert t.nesting == Nesting.Open
    assert t.level == 0
    assert t.attrs == []
    assert t.children is None
    assert t.content == ""
    assert t.markup == ""
    assert t.info == ""
    assert t.meta is None
    assert t.map is None
    assert t.block is False
    assert t.hidden is False


def test_attr_index_and_push():
    t = Token("type", "tag", Nesting.Self)
    assert t.attrIndex("id") == -1

    t.attrPush(("id", "123"))
    assert t.attrIndex("id") == 0

    t.attrPush(("class", "note"))
    assert t.attrIndex("class") == 1


def test_attr_set_and_get():
    t = Token("type", "tag", Nesting.Self)

    t.attrSet("id", "abc")
    assert t.attrIndex("id") == 0
    assert t.attrGet("id") == "abc"

    t.attrSet("id", "def")
    assert t.attrGet("id") == "def"

    assert t.attrGet("missing") is None


def test_attr_join():
    t = Token("type", "tag", Nesting.Self)

    t.attrJoin("class", "first")
    assert t.attrGet("class") == "first"

    t.attrJoin("class", "second")
    assert t.attrGet("class") == "first second"


def test_nested_tokens():
    parent = Token("container", "div", Nesting.Open)
    child1 = Token("paragraph_open", "p", Nesting.Open)
    child2 = Token("paragraph_close", "p", Nesting.Close)

    parent.children = [child1, child2]

    assert parent.children is not None
    assert len(parent.children) == 2
    assert parent.children[0].type == "paragraph_open"


def test_optional_fields_handling():
    t = Token("type", "tag", Nesting.Close)

    # Map
    assert t.map is None
    t.map = (5, 10)
    assert t.map == (5, 10)
    t.map = None
    assert t.map is None

    # Meta
    t.meta = "data"
    assert t.meta == "data"
    t.meta = None
    assert t.meta is None

    # Children
    assert t.children is None
    t.children = [Token("a", "a", Nesting.Self)]
    assert len(t.children) == 1
    t.children = None
    assert t.children is None


def test_enum_integrity():
    t = Token("test", "div", Nesting.Self)
    assert isinstance(t.nesting, Nesting)

    # Invalid enum should raise TypeError when passed
    with pytest.raises(TypeError):
        Token("bad", "x", 42)


def test_mutable_fields():
    t = Token("t", "x", Nesting.Open)

    t.level = 2
    t.block = True
    t.hidden = True

    assert t.level == 2
    assert t.block is True
    assert t.hidden is True


def test_string_field_mutation():
    t = Token("x", "x", Nesting.Self)

    t.content = "Hello"
    t.markup = "**"
    t.info = "bold"

    assert t.content == "Hello"
    assert t.markup == "**"
    assert t.info == "bold"
