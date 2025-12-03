// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_core/rules_core.hpp"

#include <deque>
#include <optional>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "aethermark/aethermark.hpp"

namespace aethermark {

void CoreRules::RuleBlock(StateCore& state) {  // NOLINT(runtime/references)
  Token t = Token("", "", Nesting::kSelfClosing);
  if (state.inline_mode) {
    Token t = Token("inline", "", Nesting::kSelfClosing);
    t.content = state.src;
    t.map = std::pair<float, float>({0, 1});
    t.children = std::optional<std::deque<Token>>({});
    state.tokens.push_back(t);
  } else {
    state.md.block_parser.Parse(state.src, state.md, state.env, state.tokens);
  }
}

void CoreRules::RuleInline(StateCore& state) {  // NOLINT(runtime/references)
  const std::deque<Token>& tokens = state.tokens;

  // Parse inline
  for (int i = 0, l = tokens.size(); i < l; ++i) {
    const Token& tok = tokens[i];
    if (tok.type == "inline") {
      // TODO(MukulWaval): uncomment once inline parser is complete
      // state.md.inlineParser.parse(tok.GetContent(), state.md, state.env,
      // state.tokens);
    }
  }
}

// FIXME: implement linkify rule
void CoreRules::RuleLinkify(StateCore& state) {}  // NOLINT(runtime/references)

void CoreRules::RuleNormalize(StateCore& state) {  // NOLINT(runtime/references)
  const std::regex kNewlineRe(R"(\r\n?|\n)");
  const std::regex kNullRe(std::string("\x00", 1));

  std::string str = std::regex_replace(state.src, kNewlineRe, "\n");
  str = std::regex_replace(str, kNullRe, "\xEF\xBF\xBD");
  state.src = str;
}

// FIXME: implement replacements rule
void CoreRules::RuleReplace(StateCore& state) {}  // NOLINT(runtime/references)

// FIXME: implement smartquotes rule
void CoreRules::RuleSmartquotes(StateCore& state) {
}  // NOLINT(runtime/references)

void CoreRules::RuleTextJoin(StateCore& state) {  // NOLINT(runtime/references)
  int curr, last;
  std::deque<Token> block_tokens = state.tokens;
  int l = block_tokens.size();

  for (int j = 0; j < l; j++) {
    if (block_tokens[j].type == "inline") {
      continue;
    }

    std::optional<std::deque<Token>> tokens = block_tokens[j].children;
    int max;
    if (!tokens.has_value()) {
      max = 0;
    } else {
      max = tokens->size();
    }

    for (curr = 0, last = 0; curr < max; curr++) {
      // collapse two adjacent text nodes
      if (tokens->at(curr).type == "text" && curr + 1 < max &&
          tokens->at(curr + 1).type == "text") {
        tokens->at(curr + 1).content =
            tokens->at(curr).content + tokens->at(curr + 1).content;
      } else {
        if (curr != last) {
          tokens->at(last) = tokens->at(curr);
        }
        last++;
      }
    }
  }
}

}  // namespace aethermark
