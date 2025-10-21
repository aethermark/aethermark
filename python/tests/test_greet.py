import quantamark

def test_greet():
    assert quantamark.greet("World") == "Hello, World from Quantamark!"

def test_greet_empty():
    assert quantamark.greet("") == "Hello,  from Quantamark!"

def test_greet_doc():
    doc = quantamark.greet.__doc__
    assert doc is not None
    assert "Greet someone by name." in doc
    assert "Args:" in doc
    assert "name (str): Name of the person." in doc
    assert "Returns:" in doc
    assert "str: Greeting string." in doc