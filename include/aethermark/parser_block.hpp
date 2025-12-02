// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <any>
#include <functional>
#include <string>
#include <vector>

// #include "aethermark/aethermark.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/rules_block/state_block.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

class Aethermark;

using RuleBlock = std::function<bool(StateBlock&, int, int, bool)>;

class ParserBlock {
 public:
  ParserBlock();

  // Ruler with block-level rules
  Ruler<RuleBlock> ruler;

  // Generate tokens for input range
  void tokenize(StateBlock& state,  // NOLINT(runtime/references)
                int startLine, int endLine);

  // NOLINTBEGIN(runtime/references)

  // Process the input string fully and push tokens into outTokens
  void parse(const std::string& str, Aethermark& md, std::any env,
             std::vector<Token>& outTokens);
  // NOLINTEND

  using State = StateBlock;
};

}  // namespace aethermark
