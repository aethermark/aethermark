// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include <pybind11/pybind11.h>

#include "aethermark/aethermark.hpp"
#include "aethermark/greet.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_aethermark, m) {
  m.doc() = R"pbdoc(
"""
Aethermark - High-performance Markdown parser with AFM extensions.

Extends CommonMark with enhanced syntax while remaining backward-compatible.

Example:
  >>> import aethermark
  >>> md = aethermark.AFMParser()
  >>> html = md.render("**AFM** adds _native_ syntax!")
  >>> print(html)
  <p><strong>AFM</strong> adds <em>native</em> syntax!</p>

See: https://github.com/aethermark/aethermark
"""
  )pbdoc";

  // TODO(MukulWaval): Bind top level Aethermark functionalities here.

  m.def("greet", &aethermark::Greet, R"pbdoc(
            Greet someone by name.

            Args:
                name (str): Name of the person.

            Returns:
                str: Greeting string.
          )pbdoc");

  m.attr("__version__") = "0.0.22";
}
