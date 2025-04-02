import pytest
import aethermark as m

def test_create_node():
    """Test creating a Node with type and source position."""
    node = m.Node("paragraph", [[0, 0], [0, 5]])
    
    assert node.type == "paragraph"
    assert node.sourcepos == [[0, 0], [0, 5]]

def test_append_and_prepend_child():
    """Test adding child nodes."""
    parent = m.Node("document", [[0, 0], [0, 10]])
    child1 = m.Node("paragraph", [[1, 0], [1, 5]])
    child2 = m.Node("heading", [[2, 0], [2, 6]])

    parent.append_child(child1)
    assert parent.first_child.type == "paragraph"

    parent.prepend_child(child2)
    assert parent.first_child.type == "heading"

def test_unlink_node():
    """Test unlinking a node from its parent."""
    parent = m.Node("document", [[0, 0], [0, 10]])
    child = m.Node("paragraph", [[1, 0], [1, 5]])

    parent.append_child(child)
    assert child.parent is not None  # Ensure it was added

    child.unlink()
    assert child.parent is None
    assert parent.first_child is None  # Should be empty now

def test_insert_after():
    """Test inserting a node after another node."""
    parent = m.Node("document", [[0, 0], [0, 10]])
    first_child = m.Node("paragraph", [[1, 0], [1, 5]])
    second_child = m.Node("heading", [[2, 0], [2, 6]])

    parent.append_child(first_child)
    first_child.insert_after(second_child)

    assert first_child.next == second_child
    assert second_child.prev == first_child
    assert parent.first_child == first_child
    assert parent.last_child == second_child

def test_insert_before():
    """Test inserting a node before another node."""
    parent = m.Node("document", [[0, 0], [0, 10]])
    first_child = m.Node("paragraph", [[1, 0], [1, 5]])
    new_first_child = m.Node("heading", [[2, 0], [2, 6]])

    parent.append_child(first_child)
    first_child.insert_before(new_first_child)

    assert new_first_child.next == first_child
    assert first_child.prev == new_first_child
    assert parent.first_child == new_first_child
    assert parent.last_child == first_child

if __name__ == "__main__":
    pytest.main()
