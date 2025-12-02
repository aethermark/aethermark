// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_core/rules_core.hpp"

#include <optional>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace aethermark {

using mapType = std::pair<float, float>;
using childrenType = std::optional<std::vector<Token>>;

void rule_block(StateCore& state) {  // NOLINT(runtime/references)
  Token t = Token("", "", Nesting::SELF_CLOSING);
  if (state.inlineMode) {
    Token t = Token("inline", "", Nesting::SELF_CLOSING);
    t.content = state.src;
    t.map = mapType({0, 1});
    t.children = childrenType({});
    state.tokens.push_back(t);
  } else {
    // state.md.blockParser.parse(state.src, state.md, state.env, state.tokens);
  }
}

void rule_inline(StateCore& state) {  // NOLINT(runtime/references)
  const std::vector<Token>& tokens = state.tokens;

  // Parse inline
  for (int i = 0, l = tokens.size(); i < l; ++i) {
    const Token& tok = tokens[i];
    if (tok.type == "inline") {
      // state.md.inlineParser.parse(tok.GetContent(), state.md, state.env,
      // state.tokens);
    }
  }
}

// FIXME: implement linkify rule
void rule_linkify(StateCore& state) {}  // NOLINT(runtime/references)

void rule_normalize(StateCore& state) {  // NOLINT(runtime/references)
  std::regex NEWLINE_RE("\r\n?|\n/g");
  std::regex NULL_RE("\0/g");

  std::string str = std::regex_replace(state.src, NEWLINE_RE, "\n");
  str = std::regex_replace(str, NULL_RE, "\uFFFD");
  state.src = str;
}

// FIXME: implement replacements rule
void rule_replace(StateCore& state) {}  // NOLINT(runtime/references)

// FIXME: implement smartquotes rule
void rule_smartquotes(StateCore& state) {}  // NOLINT(runtime/references)

void rule_text_join(StateCore& state) {  // NOLINT(runtime/references)
  int curr, last;
  std::vector<Token> blockTokens = state.tokens;
  int l = blockTokens.size();

  for (int j = 0; j < l; j++) {
    if (blockTokens[j].type == "inline") {
      continue;
    }

    std::optional<std::vector<Token>> tokens = blockTokens[j].children;
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
