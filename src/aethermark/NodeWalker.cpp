#include "Node.hpp"
#include "NodeWalker.hpp"

/* ────────── Constructors ────────── */

NodeWalker::NodeWalker(std::shared_ptr<Node> root)
    : current(root), root(root), entering(true) {}

/* ────────── Methods ────────── */

std::pair<bool, std::shared_ptr<Node>> NodeWalker::next()
{
    if (!current)
        return {false, nullptr};

    std::shared_ptr<Node> cur = current;
    bool wasEntering = entering; // Capture entering status before updating

    if (entering)
    {
        // If entering and container, move to first child
        if (cur->isContainer() && cur->getFirstChild())
        {
            current = cur->getFirstChild();
        }
        else
        {
            entering = false;
        }
    }
    else if (cur == root)
    {
        // If exiting the root, end traversal
        current = nullptr;
    }
    else if (!cur->getNext())
    {
        // Move to parent when no next sibling
        current = cur->getParent();
        entering = false;
    }
    else
    {
        // Move to next sibling
        current = cur->getNext();
        entering = true;
    }

    return {wasEntering, cur};
}

void NodeWalker::resumeAt(std::shared_ptr<Node> node, bool enteringState)
{
    current = node;
    entering = enteringState;
}

/* ────────── Binding Methods ────────── */

void NodeWalker::bind(pybind11::module &m)
{
    static bool registered = false; // Prevent re-registering
    if (registered)
        return;
    registered = true;

    pybind11::class_<NodeWalker>(m, "NodeWalker", R"pbdoc(
        A tree walker for traversing Node structures.

        Example:
            >>> walker = aethermark.NodeWalker(root_node)
            >>> while True:
            ...     entering, node = walker.next()
            ...     if node is None:
            ...         break
            ...     print("Entering" if entering else "Exiting", node.type)
    )pbdoc")
        /* ────────── Constructors ────────── */
        .def(pybind11::init<std::shared_ptr<Node>>(), R"pbdoc(
            Initializes a NodeWalker starting at the given root node.
        )pbdoc")

        /* ────────── Methods ────────── */
        .def("next", &NodeWalker::next, R"pbdoc(
            Advances to the next node in the traversal.

            Returns:
                Tuple[bool, Node]:
                    - A boolean indicating whether we are entering the node.
                    - The current node, or None if traversal is finished.
        )pbdoc")
        .def("resume_at", &NodeWalker::resumeAt, R"pbdoc(
            Resumes traversal at the given node.

            Args:
                node (Node): The node to resume traversal at.
                entering (bool): Whether to start in the entering state.
        )pbdoc");
}
