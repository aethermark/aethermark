#include <pybind11/pybind11.h>
#include "aethermark/aethermark.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_aethermark, m)
{
    m.doc() = "Python bindings for the Aethermark C++ library";

    m.def("greet", &aethermark::greet,
          R"pbdoc(
            Greet someone by name.

            Args:
                name (str): Name of the person.

            Returns:
                str: Greeting string.
          )pbdoc");
}
