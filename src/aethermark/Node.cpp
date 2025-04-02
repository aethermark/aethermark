#include <pybind11/stl.h>
#include "Node.hpp"

/* ────────── Constructors ────────── */

Node::Node(const std::string &type, const std::vector<std::vector<int>> &sourcepos)
    : _type(type), _sourcepos(sourcepos), _level(0) {}

/* ────────── Methods ────────── */

void Node::appendChild(std::shared_ptr<Node> child)
{
    child->unlink();
    child->_parent = shared_from_this();
    if (_lastChild)
    {
        _lastChild->_next = child;
        child->_prev = _lastChild;
        _lastChild = child;
    }
    else
    {
        _firstChild = child;
        _lastChild = child;
    }
}

void Node::prependChild(std::shared_ptr<Node> child)
{
    child->unlink();
    child->_parent = shared_from_this();
    if (_firstChild)
    {
        _firstChild->_prev = child;
        child->_next = _firstChild;
        _firstChild = child;
    }
    else
    {
        _firstChild = child;
        _lastChild = child;
    }
}

void Node::unlink()
{
    if (_prev)
    {
        _prev->_next = _next;
    }
    else if (_parent)
    {
        _parent->_firstChild = _next;
    }
    if (_next)
    {
        _next->_prev = _prev;
    }
    else if (_parent)
    {
        _parent->_lastChild = _prev;
    }
    _parent = nullptr;
    _next = nullptr;
    _prev = nullptr;
}

void Node::insertAfter(std::shared_ptr<Node> sibling)
{
    sibling->unlink();
    sibling->_next = _next;
    if (sibling->_next)
    {
        sibling->_next->_prev = sibling;
    }
    sibling->_prev = shared_from_this();
    _next = sibling;
    sibling->_parent = _parent;
    if (!sibling->_next)
    {
        sibling->_parent->_lastChild = sibling;
    }
}

void Node::insertBefore(std::shared_ptr<Node> sibling)
{
    sibling->unlink();
    sibling->_prev = _prev;
    if (sibling->_prev)
    {
        sibling->_prev->_next = sibling;
    }
    sibling->_next = shared_from_this();
    _prev = sibling;
    sibling->_parent = _parent;
    if (!sibling->_prev)
    {
        sibling->_parent->_firstChild = sibling;
    }
}

std::unique_ptr<NodeWalker> Node::walker()
{
    return std::make_unique<NodeWalker>(shared_from_this());
}

/* ────────── Properties ────────── */

std::string Node::getType() const { return _type; }
void Node::setType(const std::string &s) { _type = s; }

std::vector<std::vector<int>> Node::getSourcepos() const { return _sourcepos; }
void Node::setSourcepos(const std::vector<std::vector<int>> &s) { _sourcepos = s; }

std::string Node::getLiteral() const { return _literal; }
void Node::setLiteral(const std::string &s) { _literal = s; }

std::string Node::getDestination() const { return _destination; }
void Node::setDestination(const std::string &s) { _destination = s; }

std::string Node::getTitle() const { return _title; }
void Node::setTitle(const std::string &s) { _title = s; }

std::string Node::getInfo() const { return _info; }
void Node::setInfo(const std::string &s) { _info = s; }

int Node::getLevel() const { return _level; }
void Node::setLevel(int s) { _level = s; }

/* ────────── Readonly Properties ────────── */

std::shared_ptr<Node> Node::getFirstChild() const { return _firstChild; }

std::shared_ptr<Node> Node::getLastChild() const { return _lastChild; }

std::shared_ptr<Node> Node::getNext() const { return _next; }

std::shared_ptr<Node> Node::getPrev() const { return _prev; }

std::shared_ptr<Node> Node::getParent() const { return _parent; }

bool Node::isContainer() const { return true; }

/* ────────── Binding Methods ────────── */

void Node::bind(pybind11::module &m)
{
    static bool registered = false; // Prevent re-registering
    if (registered)
        return;
    registered = true;

    pybind11::class_<Node, std::shared_ptr<Node>>(m, "Node", R"pbdoc(
        A tree node structure for hierarchical data representation.

        Example:
            >>> node = aethermark.Node("root")
            >>> child = aethermark.Node("child")
            >>> node.append_child(child)
            >>> node.first_child.type
            'child'
    )pbdoc")
        /* ────────── Constructors ────────── */
        .def(pybind11::init<const std::string &, const std::vector<std::vector<int>> &>(), R"pbdoc(
            Initializes a Node with a type and optional source position.
        )pbdoc")

        /* ────────── Methods ────────── */
        .def("append_child", &Node::appendChild, R"pbdoc(
            Appends a child node.
        )pbdoc")
        .def("prepend_child", &Node::prependChild, R"pbdoc(
            Prepends a child node.
        )pbdoc")
        .def("unlink", &Node::unlink, R"pbdoc(
            Removes this node from its parent.
        )pbdoc")
        .def("insert_after", &Node::insertAfter, R"pbdoc(
            Inserts this node after a given sibling.
        )pbdoc")
        .def("insert_before", &Node::insertBefore, R"pbdoc(
            Inserts this node before a given sibling.
        )pbdoc")
        .def("walker", &Node::walker, R"pbdoc(
            Returns a NodeWalker to traverse the tree.
        )pbdoc")

        /* ────────── Properties ────────── */
        .def_property("type", &Node::getType, &Node::setType, R"pbdoc(
            The type of the node.
        )pbdoc")
        .def_property("sourcepos", &Node::getSourcepos, &Node::setSourcepos, R"pbdoc(
            The source position of the node.
        )pbdoc")
        .def_property("literal", &Node::getLiteral, &Node::setLiteral)
        .def_property("destination", &Node::getDestination, &Node::setDestination)
        .def_property("title", &Node::getTitle, &Node::setTitle)
        .def_property("info", &Node::getInfo, &Node::setInfo)
        .def_property("level", &Node::getLevel, &Node::setLevel)

        /* ────────── Readonly Properties ────────── */
        .def_property_readonly("first_child", &Node::getFirstChild, R"pbdoc(
            The first child of this node (read-only).
        )pbdoc")
        .def_property_readonly("last_child", &Node::getLastChild, R"pbdoc(
            The last child of this node (read-only).
        )pbdoc")
        .def_property_readonly("next", &Node::getNext, R"pbdoc(
            The next sibling node (read-only).
        )pbdoc")
        .def_property_readonly("prev", &Node::getPrev, R"pbdoc(
            The previous sibling node (read-only).
        )pbdoc")
        .def_property_readonly("parent", &Node::getParent, R"pbdoc(
            The parent node (read-only).
        )pbdoc")
        .def_property_readonly("container", &Node::isContainer, R"pbdoc(
            Whether the node is a container (read-only).
        )pbdoc");
}
