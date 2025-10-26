# pylint: disable=missing-function-docstring,missing-module-docstring
import aethermark


def test_doc():
    doc = aethermark.__doc__
    assert doc is not None
    assert "Aethermark" in doc
