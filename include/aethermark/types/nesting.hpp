#pragma once

namespace aethermark::types
{
    enum class Nesting
    {
        OPEN = 1,
        SELF = 0,
        CLOSE = -1
    };
} // namespace aethermark::types
