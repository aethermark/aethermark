// NOLINT(whitespace/indent_namespace)

// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/aethermark.hpp"

#include <deque>
#include <stdexcept>
#include <string>
#include <vector>

// #include "aethermark/parser_block.hpp"
#include "aethermark/parser_core.hpp"
// #include "aethermark/parser_inline.hpp"
// #include "aethermark/renderer.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

Aethermark::Aethermark()
    : /*inlineParser(), blockParser(),*/  // NOLINT(whitespace/indent_namespace)
      coreParser(),                       // NOLINT(whitespace/indent_namespace)
      /* renderer(), */ options() {       // NOLINT(whitespace/indent_namespace)
  configure("default");
}

Aethermark::Aethermark(const Options& opts)
    : /*inlineParser(), blockParser(),*/  // NOLINT(whitespace/indent_namespace)
      coreParser(),                       // NOLINT(whitespace/indent_namespace)
      /* renderer(), */ options(opts) {   // NOLINT(whitespace/indent_namespace)
  configure("default");
  set(opts);
}

Aethermark& Aethermark::set(const Options& opts) {
  options = opts;
  return *this;
}

Aethermark& Aethermark::configure(const std::string& preset) {
  // TODO(MukulWaval): load presets from C++ equivalents of cfg_default /
  // cfg_zero / cfg_commonmark For now, accept only "default"
  if (preset != "default" && preset != "zero" && preset != "commonmark") {
    throw std::runtime_error("Invalid preset: " + preset);
  }

  // Here you’d load component/ruler configs.
  // Keeping it empty for scaffolding.

  return *this;
}

// Aethermark& Aethermark::enable(const std::vector<std::string>& list,
//                                bool ignoreInvalid) {
//   std::vector<std::string> enabled;

//   for (auto* chain :
//        {&coreParser.ruler, &blockParser.ruler, &inlineParser.ruler}) {
//     auto r = chain->enable(list, true);
//     enabled.insert(enabled.end(), r.begin(), r.end());
//   }

//   // inline ruler2 (if implemented)
//   if (inlineParser.ruler2.has_value()) {
//     auto r2 = inlineParser.ruler2->enable(list, true);
//     enabled.insert(enabled.end(), r2.begin(), r2.end());
//   }

//   for (const auto& name : list) {
//     if (std::find(enabled.begin(), enabled.end(), name) == enabled.end()) {
//       if (!ignoreInvalid) {
//         throw std::runtime_error("Unknown rule: " + name);
//       }
//     }
//   }
//   return *this;
// }

// Aethermark& Aethermark::disable(const std::vector<std::string>& list,
//                                 bool ignoreInvalid) {
//   std::vector<std::string> disabled;

//   for (auto* chain :
//        {&coreParser.ruler, &blockParser.ruler, &inlineParser.ruler}) {
//     auto r = chain->disable(list, true);
//     disabled.insert(disabled.end(), r.begin(), r.end());
//   }

//   if (inlineParser.ruler2.has_value()) {
//     auto r2 = inlineParser.ruler2->disable(list, true);
//     disabled.insert(disabled.end(), r2.begin(), r2.end());
//   }

//   for (const auto& name : list) {
//     if (std::find(disabled.begin(), disabled.end(), name) == disabled.end())
//     {
//       if (!ignoreInvalid) {
//         throw std::runtime_error("Unknown rule: " + name);
//       }
//     }
//   }
//   return *this;
// }

std::deque<Token> Aethermark::parse(const std::string& src, std::any env) {
  StateCore state(src, *this, env);
  coreParser.process(state);
  return state.tokens;
}

std::deque<Token> Aethermark::parseInline(
    const std::string& src,  // NOLINT(whitespace/indent_namespace)
    std::any env) {          // NOLINT(whitespace/indent_namespace)
  StateCore state(src, *this, env);
  state.inlineMode = true;
  coreParser.process(state);
  return state.tokens;
}

// std::string Aethermark::render(const std::string& src, std::any env) {
//   auto tokens = parse(src, env);
//   return renderer.render(tokens, options, env);
// }

// std::string Aethermark::renderInline(const std::string& src, std::any env) {
//   auto tokens = parseInline(src, env);
//   return renderer.render(tokens, options, env);
// }

// bool Aethermark::validateLink(const std::string& url) const {
//   // Basic match to Markdown-It’s behavior
//   std::string lower;
//   lower.reserve(url.size());
//   for (char c : url) lower.push_back(std::tolower(c));

//   if (lower.starts_with("javascript:") || lower.starts_with("vbscript:") ||
//       lower.starts_with("file:")) {
//     return false;
//   }

//   if (lower.starts_with("data:")) {
//     if (lower.starts_with("data:image/gif") ||
//         lower.starts_with("data:image/png") ||
//         lower.starts_with("data:image/jpeg") ||
//         lower.starts_with("data:image/webp")) {
//       return true;
//     }
//     return false;
//   }
//   return true;
// }

std::string Aethermark::normalizeLink(const std::string& url) const {
  // TODO(MukulWaval): add proper mdurl + punycode equivalent
  return url;
}

std::string Aethermark::normalizeLinkText(const std::string& url) const {
  // TODO(MukulWaval): add proper unicode/punycode normalization
  return url;
}

Aethermark& Aethermark::use(std::function<void(Aethermark&)> plugin) {
  plugin(*this);
  return *this;
}

}  // namespace aethermark
