import aethermark


def test_doc():
    doc = aethermark.__doc__
    assert doc is not None
    assert "Aethermark" in doc
    assert "Python bindings for the Aethermark C++ library" in doc
