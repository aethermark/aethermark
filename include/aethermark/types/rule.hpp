#pragma once

namespace aethermark::types
{
    template <typename T>
    struct Rule
    {
        std::string name;
        bool enabled;
        T fn;
        std::vector<std::string> alt;
    };
} // namespace aethermark::types
