#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "token.hpp"
#include "ruler.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

void bind_nesting(py::module_ &m)
{
    py::enum_<Nesting>(m, "Nesting")
        .value("OPEN", Nesting::OPEN)
        .value("SELF", Nesting::SELF)
        .value("CLOSE", Nesting::CLOSE)
        .export_values();
}

void bind_token(py::module_ &m)
{
    py::class_<Token>(m, "Token")
        .def(py::init<std::string, std::string, Nesting>(), py::arg("type"), py::arg("tag"), py::arg("nesting"))
        .def_readwrite("type", &Token::type)
        .def_readwrite("tag", &Token::tag)
        .def_readwrite("attrs", &Token::attrs)
        .def_readwrite("map", &Token::map)
        .def_readwrite("nesting", &Token::nesting)
        .def_readwrite("level", &Token::level)
        .def_readwrite("children", &Token::children)
        .def_readwrite("content", &Token::content)
        .def_readwrite("markup", &Token::markup)
        .def_readwrite("info", &Token::info)
        .def_readwrite("meta", &Token::meta)
        .def_readwrite("block", &Token::block)
        .def_readwrite("hidden", &Token::hidden)
        .def("attr_index", &Token::attrIndex, py::arg("name"))
        .def("attr_push", &Token::attrPush, py::arg("attr_data"))
        .def("attr_set", &Token::attrSet, py::arg("name"), py::arg("value"))
        .def("attr_get", &Token::attrGet, py::arg("name"))
        .def("attr_join", &Token::attrJoin, py::arg("name"), py::arg("value"));
}

void bind_rule_options(py::module_ &m)
{
    py::class_<RuleOptions>(m, "RuleOptions")
        .def(py::init<>())
        .def_readwrite("alt", &RuleOptions::alt);
}

void bind_ruler(py::module_ &m)
{
    using RuleFunc = std::function<void(py::object)>;

    py::class_<Ruler<RuleFunc>>(m, "Ruler")
        .def(py::init<>())
        .def("push", &Ruler<RuleFunc>::push, py::arg("name"), py::arg("fn"), py::arg("options") = RuleOptions())
        .def("at", &Ruler<RuleFunc>::at, py::arg("name"), py::arg("fn"), py::arg("options") = RuleOptions())
        .def("before", &Ruler<RuleFunc>::before, py::arg("before_name"), py::arg("name"), py::arg("fn"), py::arg("options") = RuleOptions())
        .def("after", &Ruler<RuleFunc>::after, py::arg("after_name"), py::arg("name"), py::arg("fn"), py::arg("options") = RuleOptions())
        .def("enable", &Ruler<RuleFunc>::enable, py::arg("list"), py::arg("ignore_invalid") = false)
        .def("disable", &Ruler<RuleFunc>::disable, py::arg("list"), py::arg("ignore_invalid") = false)
        .def("enable_only", &Ruler<RuleFunc>::enableOnly, py::arg("list"), py::arg("ignore_invalid") = false)
        .def("get_rules", &Ruler<RuleFunc>::getRules, py::arg("chain_name"));
}

PYBIND11_MODULE(_core, m)
{
    m.doc() = R"pbdoc(
        Aethermark - High-Performance Markdown Processing with Pybind11

        Aethermark is a blazing-fast Markdown parser and renderer built with pybind11,
        introducing Aethermark-Flavored Markdown (AFM) - a powerful extension to
        CommonMark that combines rigorous standards compliance with enhanced features.

        Key Features:
        ───────────────────────────────────────────────────────────────────────────────
        • Optimized Performance: C++ core with Python bindings delivers 5-10x faster
        parsing than pure Python implementations
        • AFM Dialect: Extends CommonMark with thoughtful syntax enhancements while
        maintaining full backward compatibility
        • Extensible Architecture: Plugin system for custom syntax, renderers, and
        post-processors
        • Precision Rendering: AST-based processing guarantees consistent,
        standards-compliant output
        • Modern Tooling: Full type hints, comprehensive docs, and IDE support

        Example Usage:
        ───────────────────────────────────────────────────────────────────────────────
        >>> import aethermark
        >>> md = aethermark.AFMParser()
        >>> html = md.render("**AFM** adds _native_ syntax extensions!")
        >>> print(html)
        <p><strong>AFM</strong> adds <em>native</em> syntax extensions!</p>

        Advanced Features:
        ───────────────────────────────────────────────────────────────────────────────
        • Direct AST manipulation for programmatic document construction
        • Source mapping for accurate error reporting
        • Custom render targets (HTML, LaTeX, etc.)
        • Thread-safe batch processing

        Why Aethermark?
        ───────────────────────────────────────────────────────────────────────────────
        For projects requiring:
        • Scientific/technical documentation with complex markup needs
        • High-volume Markdown processing
        • Custom Markdown extensions without compromising performance
        • Perfect round-trip parsing/render cycles

        Learn more at: https://github.com/aethermark/aethermark
    )pbdoc";

    bind_token(m);
    bind_nesting(m);
    bind_rule_options(m);
    bind_ruler(m);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
