#pragma once
#include <memory>
#include <pybind11/pybind11.h>

// Forward declare Node to avoid circular dependency
class Node;

class NodeWalker
{
public:
    /* ────────── Constructors ────────── */

    explicit NodeWalker(std::shared_ptr<Node> root);

    /* ────────── Methods ────────── */

    std::pair<bool, std::shared_ptr<Node>> next();
    void resumeAt(std::shared_ptr<Node> node, bool entering);

    /* ────────── Binding Methods ────────── */

    static void bind(pybind11::module &m);

private:
    std::shared_ptr<Node> current;
    std::shared_ptr<Node> root;
    bool entering;
};
