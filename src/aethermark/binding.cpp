#include <pybind11/pybind11.h>
#include "Node.hpp"
#include "NodeWalker.hpp"

namespace py = pybind11;

PYBIND11_MODULE(aethermark, m)
{
    m.doc() = "Aethermark: A Python extension for Example and Node classes.";
    Node::bind(m);
    NodeWalker::bind(m);
}
