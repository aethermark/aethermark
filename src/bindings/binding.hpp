#pragma once
#include <pybind11/pybind11.h>
namespace py = pybind11;

void bind_token(py::module_ &);
void bind_ruler(py::module_ &);
void bind_types(py::module_ &);
void bind_core(py::module_ &);
void bind_main(py::module_ &);
