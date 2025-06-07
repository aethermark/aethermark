#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "aethermark/token.hpp"
#include "aethermark/ruler.hpp"
#include "binding.hpp"

namespace py = pybind11;

void bind_token(py::module_ &m)
{
    py::class_<aethermark::Token>(m, "Token")
        .def(py::init<std::string, std::string, aethermark::types::Nesting>(), py::arg("type"), py::arg("tag"), py::arg("nesting"))
        .def_readwrite("type", &aethermark::Token::type)
        .def_readwrite("tag", &aethermark::Token::tag)
        .def_readwrite("attrs", &aethermark::Token::attrs)
        .def_readwrite("map", &aethermark::Token::map)
        .def_readwrite("nesting", &aethermark::Token::nesting)
        .def_readwrite("level", &aethermark::Token::level)
        .def_readwrite("children", &aethermark::Token::children)
        .def_readwrite("content", &aethermark::Token::content)
        .def_readwrite("markup", &aethermark::Token::markup)
        .def_readwrite("info", &aethermark::Token::info)
        .def_readwrite("meta", &aethermark::Token::meta)
        .def_readwrite("block", &aethermark::Token::block)
        .def_readwrite("hidden", &aethermark::Token::hidden)
        .def("attr_index", &aethermark::Token::attrIndex, py::arg("name"))
        .def("attr_push", &aethermark::Token::attrPush, py::arg("attr_data"))
        .def("attr_set", &aethermark::Token::attrSet, py::arg("name"), py::arg("value"))
        .def("attr_get", &aethermark::Token::attrGet, py::arg("name"))
        .def("attr_join", &aethermark::Token::attrJoin, py::arg("name"), py::arg("value"));
}

void bind_ruler(py::module_ &m)
{
    using RuleFunc = std::function<void(py::object)>;

    py::class_<aethermark::Ruler<RuleFunc>>(m, "Ruler")
        .def(py::init<>())
        .def("push", &aethermark::Ruler<RuleFunc>::push, py::arg("name"), py::arg("fn"), py::arg("options") = aethermark::types::RuleOptions())
        .def("at", &aethermark::Ruler<RuleFunc>::at, py::arg("name"), py::arg("fn"), py::arg("options") = aethermark::types::RuleOptions())
        .def("before", &aethermark::Ruler<RuleFunc>::before, py::arg("before_name"), py::arg("name"), py::arg("fn"), py::arg("options") = aethermark::types::RuleOptions())
        .def("after", &aethermark::Ruler<RuleFunc>::after, py::arg("after_name"), py::arg("name"), py::arg("fn"), py::arg("options") = aethermark::types::RuleOptions())
        .def("enable", &aethermark::Ruler<RuleFunc>::enable, py::arg("list"), py::arg("ignore_invalid") = false)
        .def("disable", &aethermark::Ruler<RuleFunc>::disable, py::arg("list"), py::arg("ignore_invalid") = false)
        .def("enable_only", &aethermark::Ruler<RuleFunc>::enableOnly, py::arg("list"), py::arg("ignore_invalid") = false)
        .def("get_rules", &aethermark::Ruler<RuleFunc>::getRules, py::arg("chain_name"));
}

void bind_core(py::module_ &m)
{
    bind_token(m);
    bind_ruler(m);
}
