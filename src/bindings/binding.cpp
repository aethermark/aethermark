#include <pybind11/pybind11.h>
#include "binding.hpp"

PYBIND11_MODULE(_core, m)
{
    bind_main(m);
    bind_types(m);
    bind_core(m);
}
