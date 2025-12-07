// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_core/state_core.hpp"

#include <string>

namespace aethermark {

StateCore::StateCore(const std::string& src, Aethermark& md, std::any env)
    : src(src), md(md), env(env) {}

}  // namespace aethermark
