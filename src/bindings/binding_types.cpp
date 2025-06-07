#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "aethermark/types/rule_options.hpp"
#include "aethermark/types/rule.hpp"
#include "aethermark/types/nesting.hpp"
#include "binding.hpp"

namespace py = pybind11;

void bind_rule_options(py::module_ &m)
{
    py::class_<aethermark::types::RuleOptions>(m, "RuleOptions")
        .def(py::init<>())
        .def_readwrite("alt", &aethermark::types::RuleOptions::alt);
}

void bind_nesting(py::module_ &m)
{
    py::enum_<aethermark::types::Nesting>(m, "Nesting")
        .value("OPEN", aethermark::types::Nesting::OPEN)
        .value("SELF", aethermark::types::Nesting::SELF)
        .value("CLOSE", aethermark::types::Nesting::CLOSE)
        .export_values();
}

void bind_types(py::module_ &m)
{
    bind_nesting(m);
    bind_rule_options(m);
}
