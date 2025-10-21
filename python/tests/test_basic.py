import quantamark

def test_doc():
    doc = quantamark.__doc__
    assert doc is not None
    assert "Quantamark" in doc
    assert "Python bindings for the Quantamark C++ library" in doc