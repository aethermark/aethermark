from re import A
import pytest
import aethermark as m

def test_node_walker():
    """Test NodeWalker traversal."""
    root = m.Node("document", [[0, 0], [0, 10]])
    child1 = m.Node("paragraph", [[1, 0], [1, 5]])
    child2 = m.Node("heading", [[2, 0], [2, 6]])

    root.append_child(child1)
    root.append_child(child2)

    walker = root.walker()

    # Expected traversal sequence:
    # 1. Enter document
    # 2. Enter paragraph
    # 3. Exit paragraph
    # 4. Enter heading
    # 5. Exit heading
    # 6. Exit document

    expected_events = [
        (True, "document"),
        (True, "paragraph"),
        (False, "paragraph"),
        (True, "heading"),
        (False, "heading"),
        (False, "document"),
    ]

    for expected_entering, expected_type in expected_events:
        event = walker.next()
        assert event[0] == expected_entering, f"Expected entering={expected_entering}, got {event[0]}"
        assert event[1].type == expected_type, f"Expected type={expected_type}, got {event[1].type}"

def test_resume_at():
    """Test resuming walker at a specific node."""
    root = m.Node("document", [[0, 0], [0, 10]])
    child1 = m.Node("paragraph", [[1, 0], [1, 5]])
    child2 = m.Node("heading", [[2, 0], [2, 6]])

    root.append_child(child1)
    root.append_child(child2)

    walker = root.walker()
    walker.resume_at(child2, True)

    event = walker.next()
    assert event[0] == True  # Entering
    assert event[1].type == "heading"
    
if __name__ == "__main__":
    pytest.main()
