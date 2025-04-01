#include <pybind11/pybind11.h>
#include "example.hpp"

namespace py = pybind11;

PYBIND11_MODULE(aethermark, m)
{
    m.doc() = "Aethermark: A Python extension for Example and Node classes.";

    py::class_<Example>(m, "Example", R"pbdoc(
        A simple example class.

        Example usage:
            >>> ex = aethermark.Example()
            >>> ex.greet()
            'Hello from Example!'
    )pbdoc")
        .def(py::init<>(), R"pbdoc(
            Initializes an Example object.
        )pbdoc")
        .def("greet", &Example::greet, R"pbdoc(
            Returns a greeting message.

            Returns:
                str: A greeting string.

            Example:
                >>> ex = aethermark.Example()
                >>> ex.greet()
                'Hello from Example!'
        )pbdoc");
}
