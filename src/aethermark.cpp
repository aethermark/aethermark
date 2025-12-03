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

// TODO(MukulWaval): Add default move, copy constructors and destructors for all
// clasess
// TODO(MukulWaval): Add documentation for all classes
// TODO(MukulWaval): Optimize the use of std::vector
// TODO(MukulWaval): Remove all unnecessary comments from all files

Aethermark::Aethermark()
    : /*inlineParser(), blockParser(),*/
      core_parser(),
      /* renderer(), */ options() {
  Configure("default");
}

Aethermark::Aethermark(const Options& opts)
    : /*inlineParser(), blockParser(),*/
      core_parser(),
      /* renderer(), */ options(opts) {
  Configure("default");
  Set(opts);
}

Aethermark& Aethermark::Set(const Options& opts) {
  options = opts;
  return *this;
}

Aethermark& Aethermark::Configure(const std::string& preset) {
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

std::deque<Token> Aethermark::Parse(const std::string& src, std::any env) {
  StateCore state(src, *this, env);
  core_parser.Process(state);
  return state.tokens;
}

std::deque<Token> Aethermark::ParseInline(const std::string& src,
                                          std::any env) {
  StateCore state(src, *this, env);
  state.inline_mode = true;
  core_parser.Process(state);
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

std::string Aethermark::NormalizeLink(const std::string& url) const {
  // TODO(MukulWaval): add proper mdurl + punycode equivalent
  return url;
}

std::string Aethermark::NormalizeLinkText(const std::string& url) const {
  // TODO(MukulWaval): add proper unicode/punycode normalization
  return url;
}

Aethermark& Aethermark::Use(std::function<void(Aethermark&)> plugin) {
  plugin(*this);
  return *this;
}

}  // namespace aethermark
