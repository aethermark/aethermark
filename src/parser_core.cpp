// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/parser_core.hpp"

#include <string>
#include <vector>

#include "aethermark/rules_core/rules_core.hpp"

namespace aethermark {

static const std::vector<std::pair<std::string, RuleCore>> core_rules = {
    {"normalize", RuleNormalize},  {"block", RuleBlock},
    {"inline", RuleInline},        {"linkify", RuleLinkify},
    {"replacements", RuleReplace}, {"smartquotes", RuleSmartquotes},
    {"text_join", RuleTextJoin},
};

ParserCore::ParserCore() : ruler{} {
  for (auto& [name, fn] : core_rules) {
    ruler.Push(name, fn);
  }
}

void ParserCore::Process(StateCore& state) {
  std::vector<RuleCore> rules = this->ruler.GetRules("");
  for (int i = 0, l = rules.size(); i < l; i++) {
    rules[i](state);
  }
}

}  // namespace aethermark
