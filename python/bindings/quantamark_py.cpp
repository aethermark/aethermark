#include <pybind11/pybind11.h>
#include "quantamark/quantamark.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_quantamark, m)
{
    m.doc() = "Python bindings for the Quantamark C++ library";

    m.def("greet", &quantamark::greet,
          R"pbdoc(
            Greet someone by name.

            Args:
                name (str): Name of the person.

            Returns:
                str: Greeting string.
          )pbdoc");
}
