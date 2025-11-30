// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/parser_core.hpp"

#include <string>
#include <vector>

#include "aethermark/rules_core/rules_core.hpp"

namespace aethermark {

static const std::vector<std::pair<std::string, RuleCore>> core_rules = {
    {"normalize", rule_normalize},  {"block", rule_block},
    {"inline", rule_inline},        {"linkify", rule_linkify},
    {"replacements", rule_replace}, {"smartquotes", rule_smartquotes},
    {"text_join", rule_text_join},
};

Core::Core() : ruler{} {
  for (auto& [name, fn] : core_rules) {
    ruler.push(name, fn);
  }
}

void Core::process(StateCore& state) {
  std::vector<RuleCore> rules = this->ruler.getRules("");
  for (int i = 0, l = rules.size(); i < l; i++) {
    rules[i](state);
  }
}

}  // namespace aethermark
