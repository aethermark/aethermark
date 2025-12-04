# pylint: disable=missing-function-docstring,missing-module-docstring
import aethermark


def test_doc():
    doc = aethermark.__doc__
    assert doc is not None
    assert "Aethermark" in doc


def test_greet():
    m = aethermark.greet("Mukul")
    assert m == "Hello, Mukul!"
