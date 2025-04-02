#include <pybind11/pybind11.h>
#include "Example.hpp"
#include "Node.hpp"
#include "NodeWalker.hpp"

namespace py = pybind11;

PYBIND11_MODULE(aethermark, m)
{
    m.doc() = "Aethermark: A Python extension for Example and Node classes.";
    Example::bind(m);
    Node::bind(m);
    NodeWalker::bind(m);
}
